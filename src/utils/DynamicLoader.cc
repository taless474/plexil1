/* Copyright (c) 2006-2021, Universities Space Research Association (USRA).
*  All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of the Universities Space Research Association nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY USRA ``AS IS'' AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL USRA BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
* TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
* USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "plexil-config.h"

#include "DynamicLoader.h"

#include "Debug.hh"

#include "plexil-stddef.h" // NULL

#ifdef HAVE_DLFCN_H
// Include files needed only if dynamic linking is available
#include <dlfcn.h>
#include <string>
#include <stack>
#include "plexil-stdlib.h" // atexit()
#endif

#ifdef HAVE_DLFCN_H
// Forward declarations only required if dynamic linking is available.
static int initModule(const char *moduleName, void *dl_handle);
static void *loadLibrary(const char *libName);
#endif

extern "C"
int dynamicLoadModule(const char* moduleName, 
                      const char* libPath)
{
#ifdef HAVE_DLFCN_H
  // Try to initialize it, in hopes it's already loaded
  if (initModule(moduleName, RTLD_DEFAULT)) {
    debugMsg("DynamicLoader:loadModule", " for " << moduleName << " succeeded");
    return 1;
  }

  // Try to load it.
  std::string libName;
  if (!libPath || !*libPath) { // null or empty
    // construct library name from module name
    libName = "lib" + std::string(moduleName);
    debugMsg("DynamicLoader:loadModule",
             " no library name provided for module \""
             << moduleName << "\", using default value of \""
             << libName << "\"");
    libPath = libName.c_str();
  }

  void *dl_handle = loadLibrary(libPath);
  if (!dl_handle) {
    debugMsg("DynamicLoader:loadModule",
             " for " << moduleName << " failed; library " << libPath << " not found");
    return 0;
  }

  debugMsg("DynamicLoader:loadModule",
           "for " << moduleName << ", found library " << libPath);

  // Try to initialize it again
  if (initModule(moduleName, dl_handle)) {
    debugMsg("DynamicLoader:loadModule", " for " << moduleName << " succeeded");
    return 1;
  }
#endif // HAVE_DLFCN_H

  debugMsg("DynamicLoader:loadModule",
           " unable to initialize \"" << moduleName << '\"');
  return 0;
}

//
// *** Below this line requires dynamic loading functionality ***
//

#ifdef HAVE_DLFCN_H

//! \brief Library file extensions to search for;
//!        a null-terminated array of NUL-terminated character strings.
static const char* LIBRARY_EXTENSIONS[] = {".so", ".dylib", NULL};

//! \brief The handles returned from dlopen(). These must be closed at exit.
static std::stack<void *> s_handles;

//! \brief Cleanup function to run at program exit.
//!        Closes all open shared library handles.
static void dynamicLoaderCleanUp()
{
  while (!s_handles.empty()) {
    dlclose(s_handles.top());
    s_handles.pop();
  }
}

//! \brief Ensure that the cleanup function has been registered exactly once.
static void ensureFinalizer()
{
  static bool sl_inited = false;
  if (!sl_inited) {
    // Dynamic loader cleanup cannot be run until all other cleanup is done,
    // so use atexit() instead of plexilAddFinalizer().
    atexit(&dynamicLoaderCleanUp);
    sl_inited = true;
  }
}

//! @brief Attempt to dynamically load the named file.
//! @param fname The file name.
//! @return The dlopen() handle if successful, NULL otherwise.
static void *tryLoadFile(const char *fname)
{
  void *handle = NULL;
  ensureFinalizer();
  handle = dlopen(fname, RTLD_NOW | RTLD_GLOBAL);
  if (handle) {
    debugMsg("DynamicLoader:tryLoadFile",
             " dlopen of " << fname << " successful");
    s_handles.push(handle);
  }
  else {
    debugMsg("DynamicLoader:tryLoadFile",
             " dlopen failed on file " << fname << ": " << dlerror());
  }
  return handle;
}

//! @brief Load the named library.
//! @param libName The library name, with or without the appropriate extension.
//! @return The dlopen() handle if successful, NULL otherwise.
void *loadLibrary(const char *libName)
{
  void *handle = tryLoadFile(libName);
  if (handle) {
    debugMsg("DynamicLoader:loadLibrary",
             " successfully loaded library " << libName);
    return handle;
  }

  // Try adding the appropriate extension
  size_t i = 0;
  while (LIBRARY_EXTENSIONS[i]) {
    std::string libPath = libName;
    libPath += LIBRARY_EXTENSIONS[i++];
    handle = tryLoadFile(libPath.c_str());
    if (handle) {
      debugMsg("DynamicLoader:loadLibrary",
               " successfully loaded library " << libPath);
      return handle;
    }
  }

  debugMsg("DynamicLoader:loadLibrary",
           " unable to find library \"" << libName << "\"");
  return NULL;
}

//! @brief Find the named symbol.
//! @param symName The name of the symbol to locate.
//! @param dl_handle If supplied, the return value from dlopen() or loadLibrary() above.
//! @return The symbol value if successful, NULL otherwise.
//! @note If NULL may be a valid result, you should be calling dlsym() directly
//!       instead of this convenience wrapper.
static void *findSymbol(char const *symName, void *dl_handle)
{
  void *sym = dlsym(dl_handle, symName);
  if (!sym) {
    // error,  or is symbol actually NULL?
    char const *err = dlerror();
    condDebugMsg(err,
                 "DynamicLoader:findSymbol",
                 " dlsym failed for symbol \"" << symName << "\": " << err);
    condDebugMsg(!err,
                 "DynamicLoader:findSymbol",
                 " succeeded, symbol \"" << symName << "\" is NULL");
    // either way, return NULL
    return NULL;
  }
  debugMsg("DynamicLoader:findSymbol",
           " succeeded for \"" << symName << '"');
  return sym;
}

//! @brief Call the module's init function.
//! @param moduleName The name of the module
//! @param dl_handle The return value from dlopen() or loadLibrary(), or one of the
//!                  special values RTLD_DEFAULT, RTLD_NEXT, RTLD_SELF, RTLD_MAIN_ONLY.
//! @return true if the function was found and called, false otherwise.
//! @note Expects to call init<moduleName>() with no args.
int initModule(const char *moduleName, void *dl_handle) 
{
  std::string funcName = std::string("init") + moduleName;
  void *func_as_void_ptr = findSymbol(funcName.c_str(), dl_handle);
  if (!func_as_void_ptr) {
    debugMsg("DynamicLoader:initModule",
             " failed; init function for module " << moduleName << " not found");
    return 0;
  }

  void (*func)() = reinterpret_cast<void (*)()>(func_as_void_ptr);
  // FIXME - Could blow up spectacularly, how to defend?
  (*func)();

  debugMsg("DynamicLoader:initModule",
           " for module " << moduleName << " succeeded");
  return 1;
}

#endif // #ifdef HAVE_DLFCN_H

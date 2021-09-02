// Copyright (c) 2006-2021, Universities Space Research Association (USRA).
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the Universities Space Research Association nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY USRA ``AS IS'' AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL USRA BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
// OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
// TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
// USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef PLEXIL_PLAN_LIBRARY_HH
#define PLEXIL_PLAN_LIBRARY_HH

#include <string>
#include <vector>

namespace pugi
{
  class xml_document;
}

namespace PLEXIL
{
  class SymbolTable;

  //! \struct Library
  //! \brief Internal representation of a loaded LibraryNode.
  //! \ingroup XML-parser
  struct Library {
    //! \brief Pointer to a pre-checked XML document.
    pugi::xml_document *doc;

    //! \brief Pointer to the symbol table created by checking the document.
    SymbolTable *symtab;

    //! \brief Default constructor.
    Library()
      : doc(NULL), symtab(NULL)
    {}

    //! \brief Constructor from a document and a symbol table.
    //! \param d Pointer to the XML document.
    //! \param s Pointer to the symbol table.
    Library(pugi::xml_document *d, SymbolTable *s)
      : doc(d), symtab(s)
    {}
  };

  //! \ingroup XML-parser
  ///@{

  //! \brief Get the current library search path.
  //! \return Const reference to the path.
  extern std::vector<std::string> const & getLibraryPaths();
  
  //! \brief Put the named directory at the head of the library search path.
  //! \param dirname Const reference to the directory string
  extern void prependLibraryPath(std::string const &dirname);
  
  //! \brief Put the named directory at the tail of the library search path.
  //! \param dirname Const reference to the directory string.
  extern void appendLibraryPath(std::string const &dirname);

  //! \brief Set the library paths directly.
  //! \param paths Const reference to a vector of the desired library directory
  //!        names in the desired order.
  extern void setLibraryPaths(std::vector<std::string> const &paths);

  //! \brief Check whether the named library is loaded.
  //! \param name The library name.
  //! \return true if loaded, false if not.
  extern bool isLibraryLoaded(char const *name);

  //! \brief Return the named Library, if found.
  //! \param name The name of the library sought.
  //! \param loadIfNotFound (Optional) When true, attempt to load the named library from a file.
  //! \return Const pointer to the Library descriptor; NULL if not found.
  extern Library const *getLibraryNode(char const *name,
                                       bool loadIfNotFound = true);

  //! \brief Load the requested library node from a file,
  //!        using the current library path.
  //! \param nodeName Name of the library to load.
  //! \return Pointer to the requested Library, or NULL.
  extern Library const *loadLibraryNode(char const *nodeName);

  //! \brief Load the library node definition contained in the given XML document.
  //! \param doc Pointer to the XML document.
  //! \return The Library if successful, NULL otherwise.
  //! \note Ownership of the document is transferred to the caller.
  extern Library const *loadLibraryDocument(pugi::xml_document *doc);

  ///@}

} // namespace PLEXIL

#endif // PLEXIL_PLAN_LIBRARY_HH

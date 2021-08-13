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

#ifndef THREAD_SPAWN_HEADER
#define THREAD_SPAWN_HEADER

#include "plexil-config.h"

#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#endif

//! \brief Macro for the type of a pointer to a thread function.
//! \ingroup Utils
#define THREAD_FUNC_PTR void* (*)(void*)

//! \brief Create a new thread from the given function and parameter.
//! \param threadFunc The function to run as the thread's top level.
//! \param arg An argument to pass to the top level function.
//! \param thread_id Reference to a variable to hold the ID of the spawned thread.
//! \return true if the thread was created successfully; false if not.
//! \deprecated Superseded by std::thread in PLEXIL 6.
//! \ingroup Utils
bool threadSpawn(void* (*threadFunc)(void*), void *arg, pthread_t& thread_id);

#endif // THREAD_SPAWN_HEADER

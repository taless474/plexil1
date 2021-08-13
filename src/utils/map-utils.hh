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

#ifndef PLEXIL_MAP_UTILS_HH
#define PLEXIL_MAP_UTILS_HH

#include <string>

#include "plexil-string.h"

//
// Comparator classes for use with SimpleMap class template
//

namespace PLEXIL
{

  //! \struct CStringComparator
  //! \brief Comparator class for comparing two null-terminated character strings,
  //!        or a std::string and a null-terminated string.
  //! \see SimpleMap
  //! \ingroup Utils
  struct CStringComparator
  {
    //! \brief Compare two null-terminated character strings.
    //! \param a Const reference to a pointer to const character string.
    //! \param b Const reference to another pointer to const character string.
    //! \return true if the first string is strictly less than the second, false otherwise.
    bool operator()(char const * const &a, char const * const &b) const
    {
      return (strcmp(a, b) < 0);
    }

    //! \brief Compare a std::string instance to a null-terminated character string.
    //! \param a Const reference to a std::string.
    //! \param b Const reference to another pointer to const character string.
    //! \return true if the first string is strictly less than the second, false otherwise.
    bool operator()(std::string const &a, char const * const &b) const
    {
      return a < b;
    }

    //! \brief Are two null-terminated character strings equal?
    //! \param a Const reference to a pointer to const character string.
    //! \param b Const reference to another pointer to const character string.
    //! \return true if the strings are equal, false otherwise.
    bool equal(char const * const &a, char const * const &b) const
    {
      return !strcmp(a, b);
    }

    //! \brief Are a std::string instance and a null-terminated character string equal?
    //! \param a Const reference to a std::string.
    //! \param b Const reference to another pointer to const character string.
    //! \return true if the first string is equal to the second, false otherwise.
    bool equal(std::string const &a, char const * const &b) const
    {
      return a == b;
    }
  };

} // namespace PLEXIL

#endif // PLEXIL_MAP_UTILS_HH

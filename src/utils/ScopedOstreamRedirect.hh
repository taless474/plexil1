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

#ifndef SCOPED_OSTREAM_REDIRECT_HH
#define SCOPED_OSTREAM_REDIRECT_HH

#include <ostream>

namespace PLEXIL
{

  //! \class ScopedOstreamRedirect
  //! \brief Redirects output sent to a std::ostream instance to
  //!        another std::ostream instance.  Redirection ends when the
  //!        ScopedOstreamRedirect instance is destroyed.
  //! \note Based on http://stackoverflow.com/questions/4753153/redirecting-stdcout
  //! \ingroup Utils
  class ScopedOstreamRedirect
  {
  public:

	/**
	 * @brief Construct a redirection for the given output stream.
	 * @param inOriginal Reference to the output stream being redirected.
	 * @param inRedirect Reference to the output stream receiving the redirection.
	 */
    ScopedOstreamRedirect(std::ostream & inOriginal, std::ostream & inRedirect) :
	  mOriginal(inOriginal),
	  mOldBuffer(inOriginal.rdbuf(inRedirect.rdbuf()))
    {}

	/**
	 * @brief Destructor. Cancels the redirection.
	 */
    ~ScopedOstreamRedirect()
    {
	  mOriginal.rdbuf(mOldBuffer);
    }

  private:

	// Not implemented
	ScopedOstreamRedirect();
    ScopedOstreamRedirect(const ScopedOstreamRedirect&);
    ScopedOstreamRedirect& operator=(const ScopedOstreamRedirect&);

    std::ostream & mOriginal; //!< The stream being redirected.
    std::streambuf * mOldBuffer; //!< The original buffer of the redirected stream.
  };

}

#endif // SCOPED_OSTREAM_REDIRECT_HH

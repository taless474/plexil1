/* Copyright (c) 2006-2008, Universities Space Research Association (USRA).
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
#ifndef RESPONSE_FACTORY_HH
#define RESPONSE_FACTORY_HH

#include <string>
#include <iostream>

/*
 * @brief Function template to parse one object from an input stream.
 * @param instream The stream to read an object from.
 * @param result The variable where the result should be stored.
 * @return True if a value was read, false otherwise.
 * @note Can fail due to being at EOF - this should not be considered an error 
         when an unknown number of values will be read from the stream.
 */

template< typename T > bool parseType(std::istream& instream, T& result)
{
  // Clear stream error status prior to parsing
  instream.clear();
  instream >> result;
  if (instream.fail())
    {
      if (!instream.eof())
	std::cerr << "parseType: Error: cannot parse line" << std::endl;
      return false;
    }
  return true;
}

class ResponseBase;

class ResponseFactory
{
public:
  ResponseFactory(){}
  virtual ~ResponseFactory(){}
  virtual ResponseBase* parse(const std::string& cmdName, timeval tDelay, 
                              std::istream& inStr) = 0;
private:
};

#endif // RESPONSE_FACTORY_HH

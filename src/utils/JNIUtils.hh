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

#ifndef PLEXIL_JNI_UTILS_HH
#define PLEXIL_JNI_UTILS_HH

#include <jni.h>
#include <string>
#include <vector>

//
// Utilities for accessing Java data
//

namespace PLEXIL {

  /**
   * \class JNIUtils
   * \brief A class whose static methods facilitate access to Java objects.
   * \ingroup Utils
   */ 
  class JNIUtils
  {
  public:

	//! \brief Constructor from pointer to a JNIEnv.
    //! \param Pointer to the JNIEnv.
	JNIUtils(JNIEnv* env);

    //! \brief Copy constructor.
    //! \param other Const reference to the JNIUtils instance to copy.
	JNIUtils(const JNIUtils&);

	//! \brief Destructor.
	~JNIUtils();

	//! \brief Copy assignment operator.
    //! \param orig Const reference to the JNIUtils instance to copy.
	JNIUtils& operator=(const JNIUtils& orig);

	/**
	 * \brief Construct a freshly allocated copy of the Java string.
	 * \param javaString The JNI string object.
	 * \return A freshly allocated copy of the Java string, or NULL.
	 */
	char* getJavaStringCopy(jstring javaString);

	/**
     * \brief Extract the strings from a Java string array in argc/argv format.
	 * \param javaArgv The JNI string array.
	 * \param argcReturn A reference to an int variable to hold the argument count.
	 * \param argvReturn A reference to a char** variable to hold the argument vector.
	 * \return true if the operation was successful, false otherwise.
	 * \note Both the strings and the string array are freshly
	 *       allocated. The caller is responsible for deleting them.
	 */
	bool getArgcArgv(jobjectArray javaArgv, int &argcReturn, char** &argvReturn);

	/**
	 * \brief Construct a Java String object from the given C char* object.
	 * \param cstr Pointer to a C character string.
	 * \return A freshly allocated Java jstring object.
	 */
	jstring makeJavaString(const char* cstr);

	/**
	 * \brief Construct a Java array of String objects.
	 * \param size The number of elements in the array.
	 * \return A freshly allocated Java string array object.
	 */
	jobjectArray makeJavaStringArray(jsize size);

	/**
	 * \brief Given a Java String[], return an array of char*.
	 * \param The Java String[] object.
	 * \return A freshly allocated vector of strings.
     * \note The caller is responsible for deleting the returned vector.
	 */
	std::vector<std::string>* getJavaStringArray(jobjectArray ary);

	//! \brief Get the JNIEnv pointer.
    //! \return The pointer.
	JNIEnv* getEnv() { return m_env; }

  private:

	// Deliberately unimplemented
	JNIUtils(); 

	JNIEnv* m_env;        //!< A pointer to the JNI environment object.
	jclass m_stringClass; //!< The Java string class.

  }; // class JNIUtils

} // namespace PLEXIL

#endif // PLEXIL_JNI_UTILS_HH

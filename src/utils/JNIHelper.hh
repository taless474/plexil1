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

#ifndef JNI_HELPER_H
#define JNI_HELPER_H

#include <jni.h>

#include "plexil-stddef.h" // NULL

/**
 * \class JNIHelper
 * \brief A class which helps provide access to key JNI pointers.
 *
 * Declares a JNIHelper stack-allocated variable at the start of a
 * JNI-called function, and any code called by that function will have
 * access to the JNI.
 *
 * \note Will only work in single-threaded applications!
 *
 * \ingroup Utils
 */
class JNIHelper
{
public:
  /**
   * \brief Constructor.
   * \param env Pointer to the JNIEnv object.
   * \param java_this The JNI pointer to the Java "this" object.
   */
  JNIHelper(JNIEnv* env, jobject java_this);

  /**
   * \brief Virtual destructor.
   */
  virtual ~JNIHelper();

  /**
   * \brief Instance accessor.
   */
  JNIHelper& getInstance() { return *s_instance; }

  /**
   * \brief Get the pointer to the JNIEnv object.
   * \return The JNIEnv pointer.
   */
  static JNIEnv* getJNIEnv();

  /**
   * \brief Get the pointer to the Java object from the currently active JNI call.
   * \return The object pointer.
   */
  static jobject getJavaThis();

  /**
   * \brief Get the pointer to the JNIEnv object.
   * \return The JNIEnv pointer.
   */
  JNIEnv* getEnv() { return m_env; }
  
  /**
   * \brief Get the Java class java.lang.Class.
   * \return The class pointer.
   */
  jclass getClassClass();

  /**
   * \brief Return true if the object is an array, false otherwise.
   */
  bool isArray(jobject object);

  /**
   * \brief Return true if the class is an array class, false otherwise.
   */
  bool isArrayClass(jclass klass);

  /**
   * \brief Get the class's name from the JNI.
   * \return Freshly allocated copy of the class name.
   * \note Caller owns the return value and is responsible to delete it.
   */
  char* getClassName(jclass klass);

private:

  // Deliberately not implemented
  JNIHelper();
  JNIHelper(const JNIHelper&);
  JNIHelper& operator=(const JNIHelper&);

protected:

  //
  // Static (class) member variables
  //

  //* The active JNIHelper instance.
  static JNIHelper* s_instance;

  //
  // Member (instance) variables
  //

  //* Pointer to the JNI environment.
  JNIEnv* m_env;
  //* Pointer to the Java object.
  jobject m_java_this;
  //* Previous instance (in case of reentrant calls)
  JNIHelper* m_previous;

  //
  // Utility variables
  //

  //* "Class" class
  jclass m_classClass;

  // Cached method IDs
  jmethodID m_isArrayMethod;
  jmethodID m_getNameMethod;
};

/**
 * \class JavaObject
 * \brief Wrapper class for Java object references.  Automatically
 *        releases the reference when the instance is destroyed.
 *
 * \note Intended to be used as a stack-allocated variable.  The
 *       destructor will be called automatically when the variable
 *       goes out of scope.
 *
 * \ingroup Utils
 */
class JavaObject
{
public:
  //! \brief Default constructor.
  JavaObject() : m_jobj(NULL) {}

  //! \brief Constructor from a Java object reference.
  //! \param obj Java object reference.
  JavaObject(jobject obj) : m_jobj(obj) {}

  //! \brief Virtual destructor.
  virtual ~JavaObject()
  {
	if (m_jobj != NULL)
	  JNIHelper::getJNIEnv()->DeleteLocalRef(m_jobj);
  }

  //! \brief Assignment operator from Java object reference.
  //! \param obj Java object reference.
  //! \return Reference to *this.
  //! \note Releases any Java object reference held prior to the call.
  JavaObject& operator=(const jobject& obj)
  {
 	if (m_jobj != NULL) {
	  JNIHelper::getJNIEnv()->DeleteLocalRef(m_jobj);
 	}
 	m_jobj = obj;
 	return *this;
  }

  //! \brief Overloaded dereference operator.
  //! \return Reference to the object.
  _jobject& operator->() { return *m_jobj; }

  //! \brief Accessor.
  //! \return The wrapped object reference.
  jobject get_jobject() { return m_jobj; }

private:
  // Deliberately unimplemented
  JavaObject(const JavaObject&);
  JavaObject& operator=(const JavaObject&);

protected:
  jobject m_jobj; //!< The object reference.
};

/**
 * \class JavaClass
 * \brief Wrapper class for references to Java classes.  Automatically
 *        releases the reference when the instance is destroyed.
 *
 * \note Intended to be used as a stack-allocated variable.  The
 *        destructor is called automatically when the variable goes
 *        out of scope.
 *
 * \ingroup Utils
 */
class JavaClass : public JavaObject
{
public:
  //! \brief Default constructor.
  JavaClass() : JavaObject() {}

  //! \brief Constructor from a Java class reference.
  //! \param cls The Java class reference.
  JavaClass(jclass cls) : JavaObject((jobject) cls) {}

  //! \brief Virtual destructor,
  virtual ~JavaClass() {}

  //! \brief Assignment operator from Java class reference.
  //! \param cls Const reference to the Java class reference.
  //! \return Reference to *this.
  //! \note Releases any Java object reference held prior to the call.
  JavaClass& operator=(const jclass& cls)
  {
	JavaObject::operator=((const jobject&) cls);
	return *this;
  }

  //! \brief Overloaded dereference operator.
  //! \return Reference to the class object.
  _jclass& operator->()
  {
	return (_jclass&) *m_jobj; 
  }

  //! \brief Accessor.
  //! \return The Java class reference.
  jclass get_jclass()
  {
    return (jclass) m_jobj;
  }

private:
  // Deliberately unimplemented
  JavaClass(const JavaClass&);
  JavaClass& operator=(const JavaClass&);
};

#endif // JNI_HELPER_H

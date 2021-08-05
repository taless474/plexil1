// Copyright (c) 2006-2021, Universities Space Research Association (USRA).
//  All rights reserved.
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

#ifndef EXEC_LISTENER_FACTORY_H
#define EXEC_LISTENER_FACTORY_H

#include "pugixml.hpp"

#include <map>

namespace PLEXIL
{
  //
  // Forward declarations
  //

  class ExecListener;

  //! \class ExecListenerFactory
  //! \brief Factory class for ExecListener instances.  Implements the AbstractFactory design pattern.
  //! \ingroup App-Framework
  class ExecListenerFactory 
  {
  public:

    //! \brief Creates a new ExecListener instance as specified by the configuration XML.
    //! \param xml Const handle to the configuration XML.
    //! \return Pointer to the new ExecListener.
    static ExecListener *createInstance(pugi::xml_node const xml);

    //! \brief Creates a new ExecListener instance for the type associated with the name and
    //!        the configuration XML.
    //! \param name Const reference to the name for the listener type.
    //! \param xml Const handle to the configuration XML to be passed to the constructor.
    //! \return Pointer to the new ExecListener.
    static ExecListener *createInstance(std::string const &name, 
                                        pugi::xml_node const xml);

    //! \brief Checks whether or not a ExecListenerFactory is registered for this name.
    //! \param name Const reference to the name string.
    //! \return True if a factory is registered for this name, false otherwise
    static bool isRegistered(std::string const &name);

    //! \brief Deallocate all factories
    static void purge();

    //! \brief Get the name of this factory.
    //! \return Const reference to the name string.
    std::string const &getName() const {return m_name;}

  protected:

    //! \brief Virtual destructor,
    virtual ~ExecListenerFactory()
    {}

    //! \brief Registers an ExecListenerFactory with the given name.
    //! \param name Const reference to the name string.
    //! \param factory Pointer to the ExecListenerFactory instance.
    static void registerFactory(std::string const &name, ExecListenerFactory* factory);

    //! \brief Instantiates a new ExecListener of the type specified by the configuration XML.
    //! \param xml Const handle to the configuration XML.
    //! \return Pointer to the new ExecListener.
    //! \note Pure virtual function, implemented by the ConcreteExecListenerFactory class template.
    virtual ExecListener *create(pugi::xml_node const xml) const = 0;

    //! \brief Constructor.
    //! \param name Const reference to the factory name.
    ExecListenerFactory(std::string const &name)
      : m_name(name)
    {
      registerFactory(m_name, this);
    }

  private:
    // Deliberately unimplemented
    ExecListenerFactory();
    ExecListenerFactory(const ExecListenerFactory&);
    ExecListenerFactory& operator=(const ExecListenerFactory&);

    //! \brief Singleton accessor to the map from names (std::string) to concrete ExecListenerFactory instances.
    //! \return Reference to the map.
    static std::map<std::string, ExecListenerFactory*>& factoryMap();

    const std::string m_name; /*!< Name used for lookup */
  };

  //! \class ConcreteExecListenerFactory
  //! \brief Concrete factory class template, specialized by listener class.
  //! \ingroup App-Framework
  template<class ListenerType>
  class ConcreteExecListenerFactory : public ExecListenerFactory 
  {
  public:
    //! \brief Constructor.
    //! \param name Const reference to the factory name
    ConcreteExecListenerFactory(std::string const &name)
      : ExecListenerFactory(name) 
    {}

    //! \brief Virtual destructor.
    virtual ~ConcreteExecListenerFactory()
    {}

  private:
    // Deliberately unimplemented
    ConcreteExecListenerFactory();
    ConcreteExecListenerFactory(const ConcreteExecListenerFactory&);
    ConcreteExecListenerFactory& operator=(const ConcreteExecListenerFactory&);

    //! \brief Instantiates a new ExecListener of the type specified by the configuration XML.
    //! \param xml Const handle to the configuration XML.
    //! \return Pointer to the new ExecListener.
    ExecListener *create(pugi::xml_node const xml) const
    {
      return new ListenerType(xml);
    }
  };


//! \brief Associates an ExecListener class with a name, by constructing a ConcreteExecListenerFactory instance for the ExecListener class.
//! \param CLASS The name of the listener class, unquoted.
//! \param NAME The name to associate with that listener class, as a quoted string.
//! \ingroup App-Framework
#define REGISTER_EXEC_LISTENER(CLASS,NAME) {new PLEXIL::ConcreteExecListenerFactory<CLASS>(NAME);}

} // namespace PLEXIL

#endif // EXEC_LISTENER_FACTORY_H

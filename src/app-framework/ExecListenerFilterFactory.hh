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

#ifndef EXEC_LISTENER_FILTER_FACTORY_H
#define EXEC_LISTENER_FILTER_FACTORY_H

#include "pugixml.hpp"

#include <map>

namespace PLEXIL
{
  //
  // Forward declarations
  //

  class ExecListenerFilter;

  //! \class ExecListenerFilterFactory
  //! \brief Factory class for ExecListenerFilter instances.  Implements the AbstractFactory design pattern.
  //! \see ExecListener
  //! \see ExecListenerFilter
  //! \ingroup App-Framework
  class ExecListenerFilterFactory 
  {
  public:

    //! \brief Creates a new ExecListenerFilter instance with the type associated with the name and
    //!        the given configuration XML.
    //! \param xml Const handle to the configuration XML specifying the filter.
    //! \return Pointer to the new ExecListenerFilter.
    static ExecListenerFilter *createInstance(pugi::xml_node const xml);

    //! \brief Creates a new ExecListenerFilter instance with the type associated with the name and
    //!        the given configuration XML.
    //! \param name The registered name for the factory.
    //! \param xml Const handle to the configuration XML to be passed
    //!            to the ExecListenerFilter constructor.
    //! \return Pointer to the new ExecListenerFilter.
    static ExecListenerFilter *createInstance(std::string const &name, 
                                              pugi::xml_node const xml);

    //! \brief Deallocate all factories
    static void purge();

    //! \brief Get the name of this factory.
    //! \return Const reference to the name.
    std::string const &getName() const {return m_name;}

  protected:

    //! \brief Virtual destructor.
    virtual ~ExecListenerFilterFactory()
    {}

    //! \brief Registers an ExecListenerFilterFactory with the specific name.
    //! \param name Const reference to the name.
    //! \param factory Pointer to the ExecListenerFilterFactory instance.
    static void registerFactory(std::string const &name, ExecListenerFilterFactory* factory);

    //! \brief Instantiates a new ExecListenerFilter of the appropriate type.
    //! \param xml Const handle to the configuration XML for the instantiated filter
    //! \return Pointer to the new ExecListenerFilter.
    virtual ExecListenerFilter *create(pugi::xml_node const xml) const = 0;

    //! \brief Constructor.
    //! \param name Const reference to the factory name.
    ExecListenerFilterFactory(std::string const &name)
      : m_name(name)
    {
      registerFactory(m_name, this);
    }

  private:
    // Deliberately unimplemented
    ExecListenerFilterFactory();
    ExecListenerFilterFactory(ExecListenerFilterFactory const &);
    ExecListenerFilterFactory& operator=(ExecListenerFilterFactory const &);

    //! \brief Shorthand for the type of the factory map.
    typedef std::map<std::string, ExecListenerFilterFactory*> FactoryMap;

    //! \brief Singleton accessor to the map from names to concrete
    //!        ExecListenerFilterFactory instances.
    static FactoryMap &factoryMap();

    std::string const m_name; /*!< Name used for lookup */
  };

  //! \class ConcreteExecListenerFilterFactory
  //! \brief Concrete factory class template, specialized for each filter type.
  //! \ingroup App-Framework
  template<class FilterType>
  class ConcreteExecListenerFilterFactory : public ExecListenerFilterFactory 
  {
  public:

    //! \brief Constructor.
    //! \param name Const reference to the factory's name.
    ConcreteExecListenerFilterFactory(std::string const &name)
      : ExecListenerFilterFactory(name) 
    {}

  private:
    // Deliberately unimplemented
    ConcreteExecListenerFilterFactory();
    ConcreteExecListenerFilterFactory(ConcreteExecListenerFilterFactory const &);
    ConcreteExecListenerFilterFactory& operator=(ConcreteExecListenerFilterFactory const &);

    //! \brief Instantiates a new ExecListenerFilter of the appropriate type.
    //! \param xml The configuration XML for the instantiated filter.
    //! \return The new ExecListenerFilter.
    ExecListenerFilter *create(pugi::xml_node const xml) const
    {
      return new FilterType(xml);
    }
  };

//! \brief Associates an ExecListenerFilter class with a name,
//!        by constructing a ConcreteExecListenerFilterFactory
//!        instance for the ExecListenerFilter class.
//! \param CLASS The name of the filter class, unquoted.
//! \param NAME The name to associate with that filter class, as a quoted string.
//! \ingroup App-Framework
#define REGISTER_EXEC_LISTENER_FILTER(CLASS,NAME) {new PLEXIL::ConcreteExecListenerFilterFactory<CLASS>(std::string(NAME));}

} // namespace PLEXIL

#endif // EXEC_LISTENER_FILTER_FACTORY_H

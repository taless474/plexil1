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

#ifndef PLEXIL_LISTENABLE_HH
#define PLEXIL_LISTENABLE_HH

#include "ExpressionListener.hh"

namespace PLEXIL
{

  // Forward reference; class is defined below.
  class Listenable;

  //! \class ListenableUnaryOperator
  //! \brief An abstract base class for a functor called by Listenable::doSubexprs.
  //! \see Listenable::doSubexprs
  //! \note This class is deleted in PLEXIL 6, replaced by a typedef based on std::function.
  //! \addtogroup Expression The %PLEXIL Expression subsystem
  class ListenableUnaryOperator
  {
  public:
    //! \brief Operator
    //! \param l Pointer to a Listenable instance.
    virtual void operator()(Listenable *l) const = 0;

    //! \brief Default constructor.
    ListenableUnaryOperator() {}

    //! \brief Virtual destructor.
    virtual ~ListenableUnaryOperator() {}
  };

  //! \class Listenable
  //! \brief Defines the API for objects to which an ExpressionListener may wish to listen.
  //! \note The base class methods do nothing. 
  //! \see Notifier
  //! \see Propagator
  //! \ingroup Expression
  class Listenable
  {
  public:

    //! \brief Virtual destructor.
    virtual ~Listenable()
    {
    }

    //! \brief Add a change listener to this object.
    //! \param ptr Pointer to the listener.
    //! \note The default method does nothing.
    virtual void addListener(ExpressionListener *ptr)
    {
    }

    //! \brief Remove a change listener from this object.
    //! \param ptr Pointer to the listener to remove.
    //! \note The default method does nothing.
    virtual void removeListener(ExpressionListener *ptr)
    {
    }

    //! \brief Make this object active in the notification network.
    //! \note The default method does nothing.
    virtual void activate()
    {
    }

    //! \brief Make this object inactive in the notification network.
    //! \note The default method does nothing.
    virtual void deactivate()
    {
    }

    //! \brief Query whether this object is active in the notification network.
    //! \return true if active, false if not.
    //! \note The default method returns true.
    virtual bool isActive() const
    {
      return true;
    }

    //
    // Member functions which derived classes may override
    // Mostly support for listener network setup and teardown
    //

    //! \brief Query whether an object can generate change notifications.
    //! \return True if so, false if not.
    //! \note The default method returns false.
    //! \note This is generally true for leaf nodes which are not constant; however,
    //!       some interior nodes (e.g. Lookup, random generator) may also generate changes
    //!       of their own accord.
    virtual bool isPropagationSource() const
    {
      return false;
    }

    //! \brief Call a function on all subexpressions of this object.
    //! \param oper A functor; it must implement an operator() method of one argument, a Listenable, returning void.
    //! \note The default method does nothing.
    virtual void doSubexprs(ListenableUnaryOperator const &oper)
    {
    }
 
  protected:

    //! \brief Default constructor.
    //! \note Protected; may only be called from derived classes.
    Listenable()
    {
    }

  };

}

#endif // PLEXIL_LISTENABLE_HH

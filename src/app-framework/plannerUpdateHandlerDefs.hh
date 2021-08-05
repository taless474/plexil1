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

#ifndef PLANNER_UPDATE_HANDLER_DEFS_HH
#define PLANNER_UPDATE_HANDLER_DEFS_HH

#include <functional>

namespace PLEXIL
{
  // Forward reference
  class AdapterExecInterface;
  class Update;

  //
  // Type aliases for functions and objects which perform PlannerUpdates in PLEXIL
  //

  //! \typedef PlannerUpdateHandlerBase
  //! \brief Abstraction for a function signature.

#if __cplusplus >= 201103L
  typedef std::function<void(Update *, AdapterExecInterface *)> PlannerUpdateHandlerBase;
#else
  typedef std::binary_function<Update *, AdapterExecInterface *, void> PlannerUpdateHandlerBase;
#endif

  //! \struct PlannerUpdateHandler
  //! \brief A functor whose operator() method should implement publication of an Update.
  //! \see Update

  //! \ingroup App-Framework

  struct PlannerUpdateHandler : public PlannerUpdateHandlerBase
  {
    virtual ~PlannerUpdateHandler()
    {
    }

    //! \brief Publish an Update to the outside world.

    //! Sends the contents of the Update to an external recipient,
    //! typically (but not always) a planner.  It reports success or
    //! failure back to the AdapterExecInterface.

    //! \see Update
    //! \see AdapterExecInterface::handleUpdateAck

    virtual void operator()(Update *, AdapterExecInterface *) = 0;
  };


  //! \typedef PlannerUpdateFn
  //! \brief Prototype of a function to publish an Update to the outside world.

  //! A PlannerUpdateFn function sends the contents of the Update to
  //! an external recipient, typically (but not always) a planner.  It
  //! reports success or failure back to the AdapterExecInterface.
  
  //! \see AdapterExecInterface::handleUpdateAck
  //! \see Update

  //! \ingroup App-Framework

  typedef void (*PlannerUpdateFn)(Update *upd, AdapterExecInterface *intf);

  //! \brief A default function for use as a PlannerUpdateFn.
  //! \note Function is defined in InterfaceManager.cc.
  //! \ingroup App-Framework
  
  void defaultPlannerUpdateFn(Update *upd, AdapterExecInterface *intf);
  
}

#endif // PLANNER_UPDATE_HANDLER_DEFS_HH

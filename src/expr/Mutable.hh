/* Copyright (c) 2006-2014, Universities Space Research Association (USRA).
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

#ifndef PLEXIL_MUTABLE_HH
#define PLEXIL_MUTABLE_HH

#include "Expression.hh"

#include <vector>

namespace PLEXIL {

  /**
   * @class Mutable
   * @brief Base class for expressions whose value may change. Implements expression listener notification.
   * @note Values need not be stored in the instance; e.g. node state and timepoint variables,
   *       aliases for other variables or expressions.
   */

  class Mutable : public Expression
  {
  public:
    // *** FIXME: Should default constructor be public or protected? ***

    /**
     * @brief Destructor.
     */
    virtual ~Mutable();

    //
    // Core Mutable behavior
    //

    /**
     * @brief Determine whether this expression is active.
     * @return true if active, false if not.
     */
    bool isActive() const;

    /**
     * @brief Make this expression active.  It will publish value changes and it will accept
     *        incoming change notifications.
     * @note Default method. Calls handleActivation() if previously inactive.
     * @see handleActivation()
     */
    void activate();

    /**
     * @brief Make this Expression inactive.  It will not publish value changes, nor will it
     *        accept incoming change notifications.
     * @note Default method. Calls handleDeactivation() if transitioning from active to inactive.
     * @see handleDeactivation()
     */
    void deactivate();

    /**
     * @brief Add a listener for changes to this Expression's value.
     * @param id The Id of the listener to notify.
     */
    void addListener(ExpressionListenerId id);

    /**
     * @brief Remove a listener from this Expression.
     * @param id The Id of the listener to remove.
     */
    void removeListener(ExpressionListenerId id);

    /**
     * @brief Notify this expression that a subexpression's value has changed.
     */
    void notifyChanged();

  protected:

    /**
     * @brief Default constructor.
     * @note Only available to derived classes.
     */
    Mutable();

    /**
     * @brief Make this expression active.  It will publish value changes and it will accept
     *        incoming change notifications.
     * @note Default method does nothing.
     */
    virtual void handleActivate();

    /**
     * @brief Make this expression inactive.  It will not publish value changes, nor will it
     *        accept incoming change notifications.
     * @note Default method does nothing.
     */
    virtual void handleDeactivate();

    /**
     * @brief Called by notifyChanged() when the expression is active.
     * @note Default method does nothing.
     */
    virtual void handleChange();

    /**
     * @brief Notify all listeners that this expression's value has changed.
     */
    void publishChange();

  private:

    // Essential member variables
    std::vector<ExpressionListenerId> m_outgoingListeners; /*<! For outgoing message notifications (this expression's value has changed) */
    unsigned int m_activeCount;
  };

} // namespace PLEXIL

#endif // PLEXIL_MUTABLE_HH

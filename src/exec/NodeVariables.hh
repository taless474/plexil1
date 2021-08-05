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

#ifndef PLEXIL_NODE_VARIABLES_HH
#define PLEXIL_NODE_VARIABLES_HH

//
// Plan-visible expressions derived from node internal state
//

#include "GetValueImpl.hh"

namespace PLEXIL {

  // Forward references
  class NodeImpl;

  //! \class StateVariable
  //! \brief Represents the state of a %PLEXIL Node.
  //! \ingroup Exec-Core
  class StateVariable :
    public GetValueImpl<uint16_t>
  {
  public:

    //! \brief Constructor.
    //! \param node Reference to the NodeImpl which owns the variable.
    StateVariable(NodeImpl &node);

    //! \brief Virtual destructor.
    virtual ~StateVariable();

    //! \brief Return a print name for the expression type.
    //! \return Pointer to const character string.
    const char *exprName() const
    {
      return "StateVariable";
    }

    //! \brief Return the type of the expression's value.
    //! \return The value type.
    //! \note Always returns NODE_STATE_TYPE.
    ValueType valueType() const
    {
      return NODE_STATE_TYPE;
    }

    //! \brief Determine whether the value of this expression is known or unknown.
    //! \return True if known, false otherwise.
    //! \note A StateVariable's value is always known.
    inline bool isKnown() const
    {
      return true;
    }

    //! \brief Copy the value of this object to a result variable.
    //! \param result Reference to an appropriately typed place to store the value.
    //! \return True if the value is known, false if unknown or the value cannot be
    //!         represented as the desired type.
    bool getValue(uint16_t &result) const;

    //! \brief Print the expression's value to a stream.
    //! \param s Reference to the stream.
    void printValue(std::ostream& s) const;

    //! \brief Print additional specialized information about an expression to a stream.
    //! \param s Reference to the output stream.
    void printSpecialized(std::ostream &s) const;

    //
    // Listenable API
    //

    virtual void doSubexprs(ListenableUnaryOperator const &oper);

    void addListener(ExpressionListener *l);

    void removeListener(ExpressionListener *l);

  private:
    // Not implemented
    StateVariable();
    StateVariable(const StateVariable &);
    StateVariable &operator=(const StateVariable &);

    NodeImpl &m_node;
  };

  class OutcomeVariable :
    public GetValueImpl<uint16_t>
  {
  public:
    /**
     * \brief Constructor.
     */
    OutcomeVariable(NodeImpl &node);

    /**
     * \brief Destructor.
     */
    ~OutcomeVariable();

    const char *exprName() const
    {
      return "OutcomeVariable";
    }

    /**
     * \brief Get the type of the expression's value.
     */
    ValueType valueType() const
    {
      return OUTCOME_TYPE;
    }

    bool isKnown() const;

    /**
     * \brief Get the current value of the node's state.
     */
    bool getValue(uint16_t &) const;

    /**
     * \brief Print the expression's value to the given stream.
     * \param s The output stream.
     */
    void printValue(std::ostream& s) const;

    void printSpecialized(std::ostream &s) const;

    // Listenable API
    virtual void doSubexprs(ListenableUnaryOperator const &oper);

    // Listenable API
    void addListener(ExpressionListener *l);
    void removeListener(ExpressionListener *l);

  private:

    // Not implemented
    OutcomeVariable();
    OutcomeVariable(const OutcomeVariable &);
    OutcomeVariable &operator=(const OutcomeVariable &);

    NodeImpl &m_node;
  };

  class FailureVariable :
    public GetValueImpl<uint16_t>
  {
  public:
    /**
     * \brief Constructor.
     */
    FailureVariable(NodeImpl &node);

    /**
     * \brief Destructor.
     */
    ~FailureVariable();

    const char *exprName() const
    {
      return "FailureVariable";
    }

    /**
     * \brief Get the type of the expression's value.
     */
    ValueType valueType() const
    {
      return FAILURE_TYPE;
    }

    bool isKnown() const;

    /**
     * \brief Get the current value of the node's state.
     */
    bool getValue(uint16_t &) const;

    /**
     * \brief Print the expression's value to the given stream.
     * \param s The output stream.
     */
    void printValue(std::ostream& s) const;

    void printSpecialized(std::ostream &s) const;

    // Listenable API
    virtual void doSubexprs(ListenableUnaryOperator const &oper);

    // Listenable API
    void addListener(ExpressionListener *l);
    void removeListener(ExpressionListener *l);

  private:

    // Not implemented
    FailureVariable();
    FailureVariable(const FailureVariable &);
    FailureVariable &operator=(const FailureVariable &);

    NodeImpl &m_node;
  };

} // namespace PLEXIL

#endif // PLEXIL_NODE_VARIABLES_HH

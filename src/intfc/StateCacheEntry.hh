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

#ifndef PLEXIL_STATE_CACHE_ENTRY_HH
#define PLEXIL_STATE_CACHE_ENTRY_HH

#include "ValueType.hh"

#include <vector>

namespace PLEXIL
{
  class CachedValue;
  class Expression;
  class Lookup;
  class State;
  class Value;


  //! \class StateCacheEntry
  //! \brief Provides the external API for a state cache entry,
  //!        and value-type-independent state and functionality.
  //! \ingroup External-Interface
  class StateCacheEntry
  {
  public:

    //! \brief Default constructor.
    StateCacheEntry();

    //! \brief Copy constructor.
    StateCacheEntry(StateCacheEntry const &orig);

    //! \brief Virtual destructor.
    virtual ~StateCacheEntry();

    //! \brief Return the value type of this entry.
    //! \return The value type.
    ValueType const valueType() const;

    //! \brief Is the value of this entry known?
    //! \return true if known, false if unknown. 
    bool isKnown() const;

    //
    // API to Lookup
    //

    //! \brief Register a Lookup expression with this State.
    //! \param s Const reference to the state.
    //! \param l Pointer to the Lookup.
    void registerLookup(State const &s, Lookup *l);

    //! \brief Remove the association between a Lookup expression and this State.
    //! \param s Const reference to the state.
    //! \param l Pointer to the Lookup.
    virtual void unregisterLookup(State const &s, Lookup *l);

    //! \brief Update the LookupOnChange thresholds for the given state.
    //! \param s Const reference to the state.
    void updateThresholds(State const &s);

    //! \brief Get the CachedValue instance associated with this entry.
    //! \return Const pointer to the CachedValue.
    //! \note Read access to the actual value is through the helper object.
    CachedValue const *cachedValue() const;

    //! \brief Set the value associated with this state to unknown.
    //! \note If this is a change, notifies all active lookups.
    void setUnknown();

    //! \brief Update the cache entry with the given new value.
    //! \param val Const reference to the new value.
    //! \note If the new value differs from the old, notifies all active lookups of the new value.
    void update(Boolean const &val);
    void update(Integer const &val);
    void update(Real const &val);
    void update(String const &val);

    //! \brief Update the cache entry with the given new value.
    //! \param valPtr Const pointer to the new value.
    //! \note If the new value differs from the old, notifies all active lookups of the new value.
    //! \note The caller is responsible for deleting the object pointed to upon return.
    void updatePtr(String const *valPtr);
    void updatePtr(BooleanArray const *valPtr);
    void updatePtr(IntegerArray const *valPtr);
    void updatePtr(RealArray const *valPtr);
    void updatePtr(StringArray const *valPtr);

    //! \brief Update the value of this entry from the given Value instance
    //! \param val Const reference to a Value.
    void update(Value const &val);

  private:
    // Assign disallowed
    StateCacheEntry &operator=(StateCacheEntry const &);

    //
    // Internal functions
    //

    //! \brief Notify all Lookups associated with this entry of a change in value.
    void notify() const;

    //! \brief Ensure that there is a CachedValue of the desired type.
    //! \param v The desired ValueType.
    //! \return true if the CachedValue's type is compatible with the requested type,
    //!         false if not.
    bool ensureCachedValue(ValueType v = UNKNOWN_TYPE);

    //
    // Helpers
    //


    //! \brief Update cached thresholds after any change in lookups.
    //! \return true if thresholds still required, false if not.
    bool integerUpdateThresholds(State const &s);

    //! \brief Update cached thresholds after any change in lookups.
    //! \return true if thresholds still required, false if not.
    bool realUpdateThresholds(State const &s);

    // Member data
    std::vector<Lookup *> m_lookups; //!< The Lookup expressions currently associated with this entry.
    CachedValue *m_value;            //!< Object to hold the most recent value associated with the  State.  May be NULL.
    CachedValue *m_lowThreshold;     //!< Object to hold the most recently computed low threshold.  May be NULL.
    CachedValue *m_highThreshold;    //!< Object to hold the most recently computed high threshold.  May be NULL.
  };

} // namespace PLEXIL

#endif // PLEXIL_STATE_CACHE_ENTRY_HH

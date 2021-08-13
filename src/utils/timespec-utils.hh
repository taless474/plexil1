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

//
// Utilities for timespec arithmetic
//

// Forward declaration
struct timespec;

//! \brief Is the first timespec less than the second?
//! \param t1 Const reference to a timespec.
//! \param t2 Const reference to another timespec.
//! \return true if the first argument is less than the second, false otherwise.
//! \ingroup Utils
bool operator<(const struct timespec& t1, const struct timespec& t2);

//! \brief Is the first timespec less than the second?
//! \param t1 Const reference to a timespec.
//! \param t2 Const reference to another timespec.
//! \return true if the first argument is greater than the second, false otherwise.
//! \ingroup Utils
bool operator>(const struct timespec& t1, const struct timespec& t2);

//! \brief Are two timespecs equal?
//! \param t1 Const reference to a timespec.
//! \param t2 Const reference to another timespec.
//! \brief Returns true if the first argument is identical to the second, false otherwise.
//! \ingroup Utils
bool operator==(const struct timespec& t1, const struct timespec& t2);

//! \brief Add the values of two timespec instances, creating a new timespec.
//! \param t1 Const reference to a timespec.
//! \param t2 Const reference to another timespec.
//! \return A timespec value representing the sum of the two arguments.
//! \ingroup Utils
struct timespec operator+ (const struct timespec& t1, const struct timespec& t2);

//! \brief Subtract one timespec from another, creating a new timespec.
//! \param t1 Const reference to a timespec.
//! \param t2 Const reference to the timespec to subtract from the first timespec.
//! \return A timespec value representing t1 - t2.
//! \ingroup Utils
struct timespec operator- (const struct timespec& t1, const struct timespec& t2);

//! \brief Convert a double to a timespec.
//! \param d A double representing time since the Unix epoch, in seconds.
//! \return A timespec value approximating the time represented by the double.
//! \ingroup Utils
struct timespec doubleToTimespec(double d);

//! \brief Convert a double to a timespec.
//! \param d A double representing time since the Unix epoch, in seconds.
//! \param result Reference to a timespec where the result will be stored.
//! \ingroup Utils
void doubleToTimespec(double d, struct timespec& result);

//! \brief Convert a timespec to a double.
//! \param tv Const reference to a timespec.
//! \return A double approximating the time represented by the
//!         timespec, in seconds since the Unix epoch.
//! \ingroup Utils
double timespecToDouble(const struct timespec& tv);

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

#include "ArithmeticOperators.hh"
#include "Constant.hh"
#include "Function.hh"
#include "TestSupport.hh"
#include "TrivialListener.hh"
#include "UserVariable.hh"

using namespace PLEXIL;

//
// Functions and operators
//

template <typename R>
class Passthrough : public Operator<R>
{
public:
  Passthrough()
  {
    Operator<R>::m_name = "PT";
  }

  ~Passthrough()
  {
  }

  bool operator()(R &result, const ExpressionId &arg) const
  {
    R temp;
    if (!arg->getValue(temp))
      return false;
    result = temp;
    return true;
  }
};

static bool testUnaryBasics()
{
  BooleanConstant treu(true);
  IntegerConstant fortytwo(42);
  RealConstant pie(3.14);
  StringConstant fou("Foo");

  Passthrough<bool> ptb;
  Passthrough<int32_t> pti;
  Passthrough<double> ptd;
  Passthrough<std::string> pts;

  UnaryFunction<bool> boule(&ptb, treu.getId());
  UnaryFunction<int32_t> inty(&pti, fortytwo.getId());
  UnaryFunction<double> dub(&ptd, pie.getId());
  UnaryFunction<double> intd(&ptd, fortytwo.getId());
  UnaryFunction<std::string> str(&pts, fou.getId());

  // Test that all are unknown when inactive
  assertTrue_1(!boule.isKnown());
  assertTrue_1(!inty.isKnown());
  assertTrue_1(!dub.isKnown());
  assertTrue_1(!intd.isKnown());
  assertTrue_1(!str.isKnown());

  // Activate and check values
  boule.activate();
  inty.activate();
  dub.activate();
  intd.activate();
  str.activate();

  bool tempb;
  int32_t tempi;
  double tempd, tempdi;
  std::string temps;
  assertTrue_1(boule.getValue(tempb));
  assertTrue_1(inty.getValue(tempi));
  assertTrue_1(dub.getValue(tempd));
  assertTrue_1(intd.getValue(tempdi));
  assertTrue_1(str.getValue(temps));

  assertTrue_1(tempb == true);
  assertTrue_1(tempi == 42);
  assertTrue_1(tempd == 3.14);
  assertTrue_1(tempdi == 42);
  assertTrue_1(temps == std::string("Foo"));

  // TODO - test propagation of changes through variable and fn

  return true;
}
 
static bool testUnaryPropagation()
{
  BooleanVariable treu(true);
  IntegerVariable fortytwo(42);
  RealVariable pie(3.14);
  StringVariable fou("Foo");

  Passthrough<bool> ptb;
  Passthrough<int32_t> pti;
  Passthrough<double> ptd;
  Passthrough<std::string> pts;

  UnaryFunction<bool> boule(&ptb, treu.getId());
  UnaryFunction<int32_t> inty(&pti, fortytwo.getId());
  UnaryFunction<double> dub(&ptd, pie.getId());
  UnaryFunction<double> intd(&ptd, fortytwo.getId());
  UnaryFunction<std::string> str(&pts, fou.getId());

  bool bchanged = false;
  bool ichanged = false;
  bool rchanged = false;
  bool r2changed = false;
  bool schanged = false;

  TrivialListener bl(bchanged);
  TrivialListener il(ichanged);
  TrivialListener rl(rchanged);
  TrivialListener rl2(r2changed);
  TrivialListener sl(schanged);

  boule.addListener(bl.getId());
  inty.addListener(il.getId());
  dub.addListener(rl.getId());
  intd.addListener(rl2.getId());
  str.addListener(sl.getId());

  // Check propagation doesn't happen when inactive
  treu.setValue(false);
  fortytwo.setValue(43);
  pie.setValue(2.718);
  fou.setValue(std::string("fu"));

  assertTrue_1(!bchanged);
  assertTrue_1(!ichanged);
  assertTrue_1(!rchanged);
  assertTrue_1(!r2changed);
  assertTrue_1(!schanged);

  // Check that variables get activated when functions do
  boule.activate();
  assertTrue_1(treu.isActive());
  inty.activate();
  assertTrue_1(fortytwo.isActive());
  dub.activate();
  assertTrue_1(pie.isActive());
  // inty and intd share the same variable
  inty.deactivate();
  intd.activate();
  assertTrue_1(fortytwo.isActive());
  str.activate();
  assertTrue_1(fou.isActive());
  // reactivate inty
  inty.activate();

  // Assign again (activation reinitialized variable values)
  treu.setValue(false);
  fortytwo.setValue(43);
  pie.setValue(2.718);
  fou.setValue(std::string("fu"));

  bool boolv;
  int32_t intv;
  double dubv;
  std::string strv;

  // Check function values
  assertTrue_1(boule.getValue(boolv));
  assertTrue_1(!boolv);
  assertTrue_1(inty.getValue(intv));
  assertTrue_1(intv == 43);
  assertTrue_1(intd.getValue(dubv));
  assertTrue_1(dubv == 43);
  assertTrue_1(dub.getValue(dubv));
  assertTrue_1(dubv == 2.718);
  assertTrue_1(str.getValue(strv));
  assertTrue_1(strv == std::string("fu"));

  // Check propagation does happen when active
  treu.setUnknown();
  fortytwo.setUnknown();
  pie.setUnknown();
  fou.setUnknown();

  assertTrue_1(bchanged);
  assertTrue_1(ichanged);
  assertTrue_1(rchanged);
  assertTrue_1(r2changed);
  assertTrue_1(schanged);

  // Clean up
  boule.removeListener(bl.getId());
  inty.removeListener(il.getId());
  dub.removeListener(rl.getId());
  intd.removeListener(rl2.getId());
  str.removeListener(sl.getId());

  return true;
}

static bool testBinaryBasics()
{
  Addition<int32_t> intAdd;
  Addition<double> realAdd;

  IntegerVariable won(1);
  IntegerConstant too(2);
  RealVariable tree(3);
  RealConstant fore(4);

  BinaryFunction<int32_t> intFn(&intAdd, won.getId(), too.getId());
  BinaryFunction<double> realFn(&realAdd, tree.getId(), fore.getId());

  int32_t itemp;
  double rtemp;

  bool ichanged = false;
  bool rchanged = false;

  TrivialListener il(ichanged);
  TrivialListener rl(rchanged);

  intFn.addListener(il.getId());
  realFn.addListener(rl.getId());

  // Check that variables and functions are inactive when created
  assertTrue_1(!intFn.isActive());
  assertTrue_1(!realFn.isActive());
  assertTrue_1(!won.isActive());
  assertTrue_1(!tree.isActive());

  // Check that values are unknown when inactive
  assertTrue_1(!won.isKnown());
  assertTrue_1(!won.getValue(itemp));
  assertTrue_1(!tree.isKnown());
  assertTrue_1(!tree.getValue(rtemp));
  assertTrue_1(!intFn.isKnown());
  assertTrue_1(!intFn.getValue(itemp));
  assertTrue_1(!realFn.isKnown());
  assertTrue_1(!realFn.getValue(rtemp));

  // Activate expressions, check that both they and their arguments are now active
  intFn.activate();
  realFn.activate();
  assertTrue_1(intFn.isActive());
  assertTrue_1(realFn.isActive());
  assertTrue_1(won.isActive());
  assertTrue_1(tree.isActive());

  // Check that values are known and reasonable
  assertTrue_1(won.isKnown());
  assertTrue_1(tree.isKnown());
  assertTrue_1(intFn.isKnown());
  assertTrue_1(realFn.isKnown());
  assertTrue_1(won.getValue(itemp));
  assertTrue_1(tree.getValue(rtemp));
  assertTrue_1(itemp == 1);
  assertTrue_1(rtemp == 3);
  assertTrue_1(intFn.getValue(itemp));
  assertTrue_1(realFn.getValue(rtemp));
  assertTrue_1(itemp == 3);
  assertTrue_1(rtemp == 7);

  // Notifications should have happened upon activation
  assertTrue_1(ichanged);
  assertTrue_1(rchanged);

  // Set the variables unknown and check that they and epxressions are now unknown
  ichanged = rchanged = false;
  won.setUnknown();
  tree.setUnknown();
  assertTrue_1(!won.isKnown());
  assertTrue_1(!won.getValue(itemp));
  assertTrue_1(!tree.isKnown());
  assertTrue_1(!tree.getValue(rtemp));
  assertTrue_1(!intFn.isKnown());
  assertTrue_1(!intFn.getValue(itemp));
  assertTrue_1(!realFn.isKnown());
  assertTrue_1(!realFn.getValue(rtemp));

  // Check that notifications have occurred, and clear them
  assertTrue_1(ichanged);
  assertTrue_1(rchanged);
  ichanged = rchanged = false;

  // Reset variables, check that values are known and reasonable
  won.deactivate();
  tree.deactivate();
  won.reset();
  tree.reset();
  won.activate();
  tree.activate();
  assertTrue_1(won.isKnown());
  assertTrue_1(tree.isKnown());
  assertTrue_1(intFn.isKnown());
  assertTrue_1(realFn.isKnown());
  assertTrue_1(won.getValue(itemp));
  assertTrue_1(tree.getValue(rtemp));
  assertTrue_1(itemp == 1);
  assertTrue_1(rtemp == 3);
  assertTrue_1(intFn.getValue(itemp));
  assertTrue_1(realFn.getValue(rtemp));
  assertTrue_1(itemp == 3);
  assertTrue_1(rtemp == 7);

  // Check that notifications have occurred
  assertTrue_1(ichanged);
  assertTrue_1(rchanged);

  // Clean up
  intFn.removeListener(il.getId());
  realFn.removeListener(rl.getId());

  return true;
}

static bool testNaryBasics()
{
  Addition<int32_t> intAdd;
  Addition<double> realAdd;

  IntegerVariable won(1);
  IntegerConstant too(2);
  IntegerVariable tree(3);

  RealConstant fore(4);
  RealVariable fivefive(5.5);
  RealVariable sixfive(6.5);

  std::vector<ExpressionId> exprs;
  const std::vector<bool> garbage(3, false);

  exprs.push_back(won.getId());
  exprs.push_back(too.getId());
  exprs.push_back(tree.getId());

  NaryFunction<int32_t> intFn(&intAdd, exprs, garbage);

  exprs.clear();
  exprs.push_back(fore.getId());
  exprs.push_back(fivefive.getId());
  exprs.push_back(sixfive.getId());

  NaryFunction<double> realFn(&realAdd, exprs, garbage);

  int32_t itemp;
  double rtemp;

  bool ichanged = false;
  bool rchanged = false;

  TrivialListener il(ichanged);
  TrivialListener rl(rchanged);

  intFn.addListener(il.getId());
  realFn.addListener(rl.getId());

  // Check that variables and functions are inactive when created
  assertTrue_1(!intFn.isActive());
  assertTrue_1(!realFn.isActive());
  assertTrue_1(!won.isActive());
  assertTrue_1(!tree.isActive());
  assertTrue_1(!fivefive.isActive());
  assertTrue_1(!sixfive.isActive());

  // Check that values are unknown when inactive
  assertTrue_1(!intFn.isKnown());
  assertTrue_1(!intFn.getValue(itemp));
  assertTrue_1(!realFn.isKnown());
  assertTrue_1(!realFn.getValue(rtemp));

  // Activate expressions, check that both they and their arguments are now active
  intFn.activate();
  realFn.activate();
  assertTrue_1(intFn.isActive());
  assertTrue_1(realFn.isActive());
  assertTrue_1(won.isActive());
  assertTrue_1(tree.isActive());
  assertTrue_1(fivefive.isActive());
  assertTrue_1(sixfive.isActive());

  // Check that values are known and reasonable
  assertTrue_1(intFn.isKnown());
  assertTrue_1(realFn.isKnown());
  assertTrue_1(intFn.getValue(itemp));
  assertTrue_1(realFn.getValue(rtemp));
  assertTrue_1(itemp == 6);
  assertTrue_1(rtemp == 16);

  // notifications should have happened upon activation
  assertTrue_1(ichanged);
  assertTrue_1(rchanged);

  // Set the variables unknown and check that they and epxressions are now unknown
  ichanged = rchanged = false;
  tree.setUnknown();
  fivefive.setUnknown();
  assertTrue_1(!tree.isKnown());
  assertTrue_1(!tree.getValue(itemp));
  assertTrue_1(!fivefive.isKnown());
  assertTrue_1(!fivefive.getValue(rtemp));
  assertTrue_1(!intFn.isKnown());
  assertTrue_1(!intFn.getValue(itemp));
  assertTrue_1(!realFn.isKnown());
  assertTrue_1(!realFn.getValue(rtemp));

  // Check that notifications have occurred, and clear them
  assertTrue_1(ichanged);
  assertTrue_1(rchanged);
  ichanged = rchanged = false;

  // Reset variables, check that values are known and reasonable
  tree.deactivate();
  fivefive.deactivate();
  tree.reset();
  fivefive.reset();
  tree.activate();
  fivefive.activate();
  assertTrue_1(tree.isKnown());
  assertTrue_1(fivefive.isKnown());
  assertTrue_1(intFn.isKnown());
  assertTrue_1(realFn.isKnown());
  assertTrue_1(tree.getValue(itemp));
  assertTrue_1(fivefive.getValue(rtemp));
  assertTrue_1(itemp == 3);
  assertTrue_1(rtemp == 5.5);
  assertTrue_1(intFn.getValue(itemp));
  assertTrue_1(realFn.getValue(rtemp));
  assertTrue_1(itemp == 6);
  assertTrue_1(rtemp == 16);

  // Check that notifications have occurred
  assertTrue_1(ichanged);
  assertTrue_1(rchanged);

  // Clean up
  intFn.removeListener(il.getId());
  realFn.removeListener(rl.getId());

  return true;
}

bool functionsTest()
{
  runTest(testUnaryBasics);
  runTest(testUnaryPropagation);
  runTest(testBinaryBasics);
  runTest(testNaryBasics);
  return true;
}

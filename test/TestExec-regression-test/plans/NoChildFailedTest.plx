<?xml version="1.0" encoding="UTF-8"?><PlexilPlan xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" FileName="plans/NoChildFailedTest.ple"><Node NodeType="NodeList" epx="UncheckedSequence" ColNo="0" LineNo="1"><NodeId>NoChildFailedTest</NodeId><VariableDeclarations><DeclareVariable ColNo="2" LineNo="4"><Name>foo</Name><Type>Integer</Type></DeclareVariable></VariableDeclarations><PostCondition ColNo="2" LineNo="6"><AND ColNo="4" LineNo="9"><EQInternal ColNo="30" LineNo="6"><NodeOutcomeVariable ColNo="22" LineNo="6"><NodeId>Case1</NodeId></NodeOutcomeVariable><NodeOutcomeValue>FAILURE</NodeOutcomeValue></EQInternal><EQInternal ColNo="21" LineNo="7"><NodeOutcomeVariable ColNo="13" LineNo="7"><NodeId>Case2</NodeId></NodeOutcomeVariable><NodeOutcomeValue>FAILURE</NodeOutcomeValue></EQInternal><EQInternal ColNo="21" LineNo="8"><NodeOutcomeVariable ColNo="13" LineNo="8"><NodeId>Case3</NodeId></NodeOutcomeVariable><NodeOutcomeValue>FAILURE</NodeOutcomeValue></EQInternal><EQInternal ColNo="21" LineNo="9"><NodeOutcomeVariable ColNo="13" LineNo="9"><NodeId>Case4</NodeId></NodeOutcomeVariable><NodeOutcomeValue>SUCCESS</NodeOutcomeValue></EQInternal></AND></PostCondition><NodeBody><NodeList><Node NodeType="NodeList" epx="CheckedSequence" ColNo="1" LineNo="11"><NodeId>Case1</NodeId><InvariantCondition><NoChildFailed><NodeRef dir="self"/></NoChildFailed></InvariantCondition><NodeBody><NodeList><Node ColNo="2" LineNo="13" NodeType="Assignment"><NodeId>Child1_1</NodeId><InvariantCondition ColNo="6" LineNo="15"><BooleanValue>false</BooleanValue></InvariantCondition><NodeBody><Assignment ColNo="6" LineNo="16"><IntegerVariable>foo</IntegerVariable><NumericRHS><IntegerValue>1</IntegerValue></NumericRHS></Assignment></NodeBody></Node></NodeList></NodeBody></Node><Node NodeType="NodeList" epx="CheckedSequence" ColNo="1" LineNo="20"><NodeId>Case2</NodeId><StartCondition><Finished><NodeRef dir="sibling">Case1</NodeRef></Finished></StartCondition><InvariantCondition><NoChildFailed><NodeRef dir="self"/></NoChildFailed></InvariantCondition><NodeBody><NodeList><Node ColNo="2" LineNo="22" NodeType="Assignment"><NodeId>Child2_1</NodeId><NodeBody><Assignment ColNo="6" LineNo="24"><IntegerVariable>foo</IntegerVariable><NumericRHS><IntegerValue>2</IntegerValue></NumericRHS></Assignment></NodeBody></Node><Node ColNo="2" LineNo="26" NodeType="Assignment"><NodeId>Child2_2</NodeId><StartCondition><Finished><NodeRef dir="sibling">Child2_1</NodeRef></Finished></StartCondition><InvariantCondition ColNo="6" LineNo="28"><BooleanValue>false</BooleanValue></InvariantCondition><NodeBody><Assignment ColNo="6" LineNo="29"><IntegerVariable>foo</IntegerVariable><NumericRHS><IntegerValue>3</IntegerValue></NumericRHS></Assignment></NodeBody></Node></NodeList></NodeBody></Node><Node NodeType="NodeList" epx="CheckedSequence" ColNo="1" LineNo="33"><NodeId>Case3</NodeId><StartCondition><Finished><NodeRef dir="sibling">Case2</NodeRef></Finished></StartCondition><InvariantCondition><NoChildFailed><NodeRef dir="self"/></NoChildFailed></InvariantCondition><NodeBody><NodeList><Node ColNo="2" LineNo="35" NodeType="Assignment"><NodeId>Child3_1</NodeId><ExitCondition ColNo="6" LineNo="37"><BooleanValue>true</BooleanValue></ExitCondition><NodeBody><Assignment ColNo="6" LineNo="38"><IntegerVariable>foo</IntegerVariable><NumericRHS><IntegerValue>4</IntegerValue></NumericRHS></Assignment></NodeBody></Node><Node ColNo="2" LineNo="40" NodeType="Assignment"><NodeId>Child3_2</NodeId><StartCondition><Finished><NodeRef dir="sibling">Child3_1</NodeRef></Finished></StartCondition><InvariantCondition ColNo="6" LineNo="42"><BooleanValue>false</BooleanValue></InvariantCondition><NodeBody><Assignment ColNo="6" LineNo="43"><IntegerVariable>foo</IntegerVariable><NumericRHS><IntegerValue>5</IntegerValue></NumericRHS></Assignment></NodeBody></Node></NodeList></NodeBody></Node><Node NodeType="NodeList" epx="CheckedSequence" ColNo="1" LineNo="47"><NodeId>Case4</NodeId><StartCondition><Finished><NodeRef dir="sibling">Case3</NodeRef></Finished></StartCondition><InvariantCondition><NoChildFailed><NodeRef dir="self"/></NoChildFailed></InvariantCondition><NodeBody><NodeList><Node ColNo="2" LineNo="49" NodeType="Assignment"><NodeId>Child4_1</NodeId><NodeBody><Assignment ColNo="6" LineNo="51"><IntegerVariable>foo</IntegerVariable><NumericRHS><IntegerValue>6</IntegerValue></NumericRHS></Assignment></NodeBody></Node><Node ColNo="2" LineNo="53" NodeType="Assignment"><NodeId>Child4_2</NodeId><StartCondition><Finished><NodeRef dir="sibling">Child4_1</NodeRef></Finished></StartCondition><ExitCondition ColNo="6" LineNo="55"><BooleanValue>true</BooleanValue></ExitCondition><NodeBody><Assignment ColNo="6" LineNo="56"><IntegerVariable>foo</IntegerVariable><NumericRHS><IntegerValue>7</IntegerValue></NumericRHS></Assignment></NodeBody></Node></NodeList></NodeBody></Node></NodeList></NodeBody></Node></PlexilPlan>
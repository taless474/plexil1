<?xml version="1.0" encoding="UTF-8"?><PlexilPlan xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:tr="extended-plexil-translator" FileName="AddArray.ple"><Node NodeType="NodeList" epx="Sequence" LineNo="1" ColNo="0"><NodeId>Root</NodeId><VariableDeclarations><DeclareArray LineNo="2" ColNo="2"><Name>foo</Name><Type>Real</Type><MaxSize>4</MaxSize><InitialValue><ArrayValue Type="Real"><RealValue>0.0</RealValue><RealValue>0.0</RealValue><RealValue>0.0</RealValue><RealValue>0.0</RealValue></ArrayValue></InitialValue></DeclareArray><DeclareVariable LineNo="3" ColNo="2"><Name>temp</Name><Type>Real</Type><InitialValue><RealValue>0.0</RealValue></InitialValue></DeclareVariable></VariableDeclarations><InvariantCondition><NoChildFailed><NodeRef dir="self"/></NoChildFailed></InvariantCondition><NodeBody><NodeList><Node NodeType="NodeList" epx="While" LineNo="4" ColNo="2"><NodeId>while__0</NodeId><RepeatCondition><Succeeded><NodeRef dir="child">ep2cp_WhileTest</NodeRef></Succeeded></RepeatCondition><NodeBody><NodeList><Node NodeType="Empty" epx="Condition"><NodeId>ep2cp_WhileTest</NodeId><PostCondition><LT><RealVariable>temp</RealVariable><RealValue>3.0</RealValue></LT></PostCondition></Node><Node NodeType="NodeList" epx="Action" LineNo="5" ColNo="2"><NodeId>BLOCK__1</NodeId><InvariantCondition><NoChildFailed><NodeRef dir="self"/></NoChildFailed></InvariantCondition><StartCondition><Succeeded><NodeRef dir="sibling">ep2cp_WhileTest</NodeRef></Succeeded></StartCondition><SkipCondition><PostconditionFailed><NodeRef dir="sibling">ep2cp_WhileTest</NodeRef></PostconditionFailed></SkipCondition><NodeBody><NodeList><Node NodeType="Assignment" LineNo="6" ColNo="4"><NodeId>SimpleAssignment</NodeId><NodeBody><Assignment LineNo="6" ColNo="24"><RealVariable>temp</RealVariable><NumericRHS><ArrayElement><ArrayVariable>foo</ArrayVariable><Index><IntegerValue>1</IntegerValue></Index></ArrayElement></NumericRHS></Assignment></NodeBody></Node><Node NodeType="Assignment" LineNo="7" ColNo="4"><NodeId>SimpleArrayAssignment</NodeId><StartCondition><Finished><NodeRef dir="sibling">SimpleAssignment</NodeRef></Finished></StartCondition><NodeBody><Assignment LineNo="7" ColNo="29"><ArrayElement><ArrayVariable>foo</ArrayVariable><Index><IntegerValue>1</IntegerValue></Index></ArrayElement><NumericRHS><ADD LineNo="7" ColNo="45"><ArrayElement><ArrayVariable>foo</ArrayVariable><Index><IntegerValue>1</IntegerValue></Index></ArrayElement><RealValue>1.0</RealValue></ADD></NumericRHS></Assignment></NodeBody></Node><Node NodeType="Assignment" LineNo="8" ColNo="4"><NodeId>SimpleArrayAssignment2</NodeId><StartCondition><Finished><NodeRef dir="sibling">SimpleArrayAssignment</NodeRef></Finished></StartCondition><NodeBody><Assignment LineNo="8" ColNo="30"><ArrayElement><ArrayVariable>foo</ArrayVariable><Index><IntegerValue>2</IntegerValue></Index></ArrayElement><NumericRHS><ADD LineNo="8" ColNo="46"><ArrayElement><ArrayVariable>foo</ArrayVariable><Index><IntegerValue>2</IntegerValue></Index></ArrayElement><RealValue>2.0</RealValue></ADD></NumericRHS></Assignment></NodeBody></Node></NodeList></NodeBody></Node></NodeList></NodeBody></Node></NodeList></NodeBody></Node></PlexilPlan>
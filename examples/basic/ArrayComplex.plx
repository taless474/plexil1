<?xml version="1.0" encoding="UTF-8"?><PlexilPlan xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:tr="extended-plexil-translator" FileName="ArrayComplex.ple"><Node NodeType="NodeList" epx="Sequence" LineNo="1" ColNo="0"><NodeId>ArrayComplex</NodeId><VariableDeclarations><DeclareArray LineNo="3" ColNo="2"><Name>foo</Name><Type>Real</Type><MaxSize>4</MaxSize><InitialValue><ArrayValue Type="Real"><RealValue>0.0</RealValue><RealValue>0.0</RealValue><RealValue>0.0</RealValue><RealValue>0.0</RealValue></ArrayValue></InitialValue></DeclareArray><DeclareArray LineNo="4" ColNo="2"><Name>bar</Name><Type>Real</Type><MaxSize>30</MaxSize></DeclareArray></VariableDeclarations><InvariantCondition><NoChildFailed><NodeRef dir="self"/></NoChildFailed></InvariantCondition><NodeBody><NodeList><Node NodeType="NodeList" epx="For" LineNo="6" ColNo="2"><NodeId>Initialize</NodeId><VariableDeclarations><DeclareVariable LineNo="6" ColNo="19"><Name>i</Name><Type>Integer</Type><InitialValue><IntegerValue>0</IntegerValue></InitialValue></DeclareVariable></VariableDeclarations><NodeBody><NodeList><Node NodeType="NodeList" epx="aux"><NodeId>ep2cp_ForLoop</NodeId><SkipCondition><NOT><LT><IntegerVariable>i</IntegerVariable><IntegerValue>30</IntegerValue></LT></NOT></SkipCondition><RepeatCondition><BooleanValue>true</BooleanValue></RepeatCondition><NodeBody><NodeList><Node NodeType="Assignment" LineNo="6" ColNo="50"><NodeId>BarInit</NodeId><NodeBody><Assignment LineNo="6" ColNo="59"><ArrayElement><ArrayVariable>bar</ArrayVariable><Index><IntegerVariable>i</IntegerVariable></Index></ArrayElement><NumericRHS><RealValue>0.0</RealValue></NumericRHS></Assignment></NodeBody></Node><Node NodeType="Assignment" epx="LoopVariableUpdate"><NodeId>ep2cp_ForLoopUpdater</NodeId><StartCondition><Finished><NodeRef dir="sibling">BarInit</NodeRef></Finished></StartCondition><NodeBody><Assignment><IntegerVariable>i</IntegerVariable><NumericRHS><ADD LineNo="6" ColNo="44"><IntegerVariable>i</IntegerVariable><IntegerValue>1</IntegerValue></ADD></NumericRHS></Assignment></NodeBody></Node></NodeList></NodeBody></Node></NodeList></NodeBody></Node><Node NodeType="NodeList" epx="For" LineNo="8" ColNo="2"><NodeId>for__0</NodeId><VariableDeclarations><DeclareVariable LineNo="8" ColNo="7"><Name>i</Name><Type>Real</Type><InitialValue><RealValue>0.0</RealValue></InitialValue></DeclareVariable></VariableDeclarations><StartCondition><Finished><NodeRef dir="sibling">Initialize</NodeRef></Finished></StartCondition><NodeBody><NodeList><Node NodeType="NodeList" epx="aux"><NodeId>ep2cp_ForLoop</NodeId><SkipCondition><NOT><LT><RealVariable>i</RealVariable><RealValue>10.0</RealValue></LT></NOT></SkipCondition><RepeatCondition><BooleanValue>true</BooleanValue></RepeatCondition><NodeBody><NodeList><Node NodeType="NodeList" epx="Sequence" LineNo="9" ColNo="2"><NodeId>BLOCK__1</NodeId><InvariantCondition><NoChildFailed><NodeRef dir="self"/></NoChildFailed></InvariantCondition><NodeBody><NodeList><Node NodeType="Assignment" LineNo="16" ColNo="4"><NodeId>SimpleArrayAssignment</NodeId><NodeBody><Assignment LineNo="16" ColNo="28"><ArrayElement><ArrayVariable>foo</ArrayVariable><Index><IntegerValue>1</IntegerValue></Index></ArrayElement><NumericRHS><ADD LineNo="16" ColNo="44"><ArrayElement><ArrayVariable>foo</ArrayVariable><Index><IntegerValue>1</IntegerValue></Index></ArrayElement><RealValue>1.0</RealValue></ADD></NumericRHS></Assignment></NodeBody></Node><Node NodeType="Assignment" LineNo="17" ColNo="4"><NodeId>SimpleArrayAssignment2</NodeId><StartCondition><Finished><NodeRef dir="sibling">SimpleArrayAssignment</NodeRef></Finished></StartCondition><NodeBody><Assignment LineNo="17" ColNo="28"><ArrayElement><ArrayVariable>bar</ArrayVariable><Index><IntegerValue>2</IntegerValue></Index></ArrayElement><NumericRHS><ADD LineNo="17" ColNo="44"><ArrayElement><ArrayVariable>bar</ArrayVariable><Index><IntegerValue>2</IntegerValue></Index></ArrayElement><RealValue>2.0</RealValue></ADD></NumericRHS></Assignment></NodeBody></Node><Node NodeType="Assignment" LineNo="18" ColNo="4"><NodeId>SimpleArrayAssignment3</NodeId><StartCondition><Finished><NodeRef dir="sibling">SimpleArrayAssignment2</NodeRef></Finished></StartCondition><NodeBody><Assignment LineNo="18" ColNo="28"><ArrayElement><ArrayVariable>foo</ArrayVariable><Index><IntegerValue>3</IntegerValue></Index></ArrayElement><NumericRHS><ADD LineNo="18" ColNo="44"><ArrayElement><ArrayVariable>foo</ArrayVariable><Index><IntegerValue>3</IntegerValue></Index></ArrayElement><RealValue>3.0</RealValue></ADD></NumericRHS></Assignment></NodeBody></Node><Node NodeType="Assignment" LineNo="19" ColNo="4"><NodeId>SimpleArrayAssignment4</NodeId><StartCondition><Finished><NodeRef dir="sibling">SimpleArrayAssignment3</NodeRef></Finished></StartCondition><NodeBody><Assignment LineNo="19" ColNo="28"><ArrayElement><ArrayVariable>bar</ArrayVariable><Index><IntegerValue>15</IntegerValue></Index></ArrayElement><NumericRHS><ADD LineNo="19" ColNo="46"><ArrayElement><ArrayVariable>bar</ArrayVariable><Index><IntegerValue>15</IntegerValue></Index></ArrayElement><RealValue>4.0</RealValue></ADD></NumericRHS></Assignment></NodeBody></Node></NodeList></NodeBody></Node><Node NodeType="Assignment" epx="LoopVariableUpdate"><NodeId>ep2cp_ForLoopUpdater</NodeId><StartCondition><Finished><NodeRef dir="sibling">BLOCK__1</NodeRef></Finished></StartCondition><NodeBody><Assignment><RealVariable>i</RealVariable><NumericRHS><ADD LineNo="8" ColNo="33"><RealVariable>i</RealVariable><IntegerValue>1</IntegerValue></ADD></NumericRHS></Assignment></NodeBody></Node></NodeList></NodeBody></Node></NodeList></NodeBody></Node></NodeList></NodeBody></Node></PlexilPlan>
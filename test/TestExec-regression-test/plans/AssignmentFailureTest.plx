<?xml version="1.0" encoding="UTF-8"?><PlexilPlan FileName="AssignmentFailureTest.ple"><Node NodeType="NodeList" epx="Concurrence" LineNo="1" ColNo="0"><NodeId>AssignmentFailureTest</NodeId><VariableDeclarations><DeclareVariable LineNo="4" ColNo="2"><Name>spoil</Name><Type>Boolean</Type><InitialValue><BooleanValue>false</BooleanValue></InitialValue></DeclareVariable><DeclareVariable LineNo="5" ColNo="2"><Name>b</Name><Type>Boolean</Type><InitialValue><BooleanValue>true</BooleanValue></InitialValue></DeclareVariable><DeclareVariable LineNo="6" ColNo="2"><Name>i</Name><Type>Integer</Type><InitialValue><IntegerValue>1</IntegerValue></InitialValue></DeclareVariable><DeclareVariable LineNo="7" ColNo="2"><Name>r</Name><Type>Real</Type><InitialValue><RealValue>1.0</RealValue></InitialValue></DeclareVariable><DeclareVariable LineNo="8" ColNo="2"><Name>s</Name><Type>String</Type><InitialValue><StringValue>ess</StringValue></InitialValue></DeclareVariable><DeclareArray LineNo="9" ColNo="2"><Name>ra</Name><Type>Real</Type><MaxSize>2</MaxSize><InitialValue><RealValue>1.0</RealValue><RealValue>2.0</RealValue></InitialValue></DeclareArray><DeclareArray LineNo="10" ColNo="2"><Name>sa</Name><Type>String</Type><MaxSize>2</MaxSize><InitialValue><StringValue>foo</StringValue><StringValue>bar</StringValue></InitialValue></DeclareArray></VariableDeclarations><EndCondition><BooleanVariable>spoil</BooleanVariable></EndCondition><NodeBody><NodeList><Node NodeType="Assignment" LineNo="14" ColNo="0"><NodeId>BoolFail</NodeId><InvariantCondition><NOT><BooleanVariable>spoil</BooleanVariable></NOT></InvariantCondition><NodeBody><Assignment LineNo="17" ColNo="4"><BooleanVariable>b</BooleanVariable><BooleanRHS><BooleanValue>false</BooleanValue></BooleanRHS></Assignment></NodeBody></Node><Node NodeType="Assignment" LineNo="20" ColNo="0"><NodeId>IntFail</NodeId><InvariantCondition><NOT><BooleanVariable>spoil</BooleanVariable></NOT></InvariantCondition><NodeBody><Assignment LineNo="23" ColNo="4"><IntegerVariable>i</IntegerVariable><NumericRHS><IntegerValue>42</IntegerValue></NumericRHS></Assignment></NodeBody></Node><Node NodeType="Assignment" LineNo="26" ColNo="0"><NodeId>RealFail</NodeId><InvariantCondition><NOT><BooleanVariable>spoil</BooleanVariable></NOT></InvariantCondition><NodeBody><Assignment LineNo="29" ColNo="4"><RealVariable>r</RealVariable><NumericRHS><RealValue>69.0</RealValue></NumericRHS></Assignment></NodeBody></Node><Node NodeType="Assignment" LineNo="32" ColNo="0"><NodeId>StringFail</NodeId><InvariantCondition><NOT><BooleanVariable>spoil</BooleanVariable></NOT></InvariantCondition><NodeBody><Assignment LineNo="35" ColNo="4"><StringVariable>s</StringVariable><StringRHS><StringValue>S</StringValue></StringRHS></Assignment></NodeBody></Node><Node NodeType="Assignment" LineNo="38" ColNo="0"><NodeId>ArrayFail</NodeId><InvariantCondition><NOT><BooleanVariable>spoil</BooleanVariable></NOT></InvariantCondition><NodeBody><Assignment LineNo="41" ColNo="4"><ArrayVariable>ra</ArrayVariable><ArrayRHS><ArrayValue Type="Real"><RealValue>42.0</RealValue><RealValue>69.0</RealValue></ArrayValue></ArrayRHS></Assignment></NodeBody></Node><Node NodeType="Assignment" LineNo="44" ColNo="0"><NodeId>StringArrayFail</NodeId><InvariantCondition><NOT><BooleanVariable>spoil</BooleanVariable></NOT></InvariantCondition><NodeBody><Assignment LineNo="47" ColNo="4"><ArrayVariable>sa</ArrayVariable><ArrayRHS><ArrayValue Type="String"><StringValue>ha</StringValue><StringValue>hee</StringValue></ArrayValue></ArrayRHS></Assignment></NodeBody></Node><Node NodeType="Assignment" LineNo="50" ColNo="0"><NodeId>theSpoiler</NodeId><NodeBody><Assignment LineNo="51" ColNo="2"><BooleanVariable>spoil</BooleanVariable><BooleanRHS><BooleanValue>true</BooleanValue></BooleanRHS></Assignment></NodeBody></Node></NodeList></NodeBody></Node></PlexilPlan>
<?xml version="1.0" encoding="UTF-8"?><PlexilPlan xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" FileName="AssignFailureWithConflict.ple"><Node NodeType="NodeList" epx="Concurrence" ColNo="0" LineNo="1"><NodeId>AssignFailureWithConflict</NodeId><VariableDeclarations><DeclareVariable ColNo="2" LineNo="4"><Name>n</Name><Type>Integer</Type><InitialValue><IntegerValue>0</IntegerValue></InitialValue></DeclareVariable><DeclareVariable ColNo="2" LineNo="5"><Name>spoil</Name><Type>Boolean</Type><InitialValue><BooleanValue>false</BooleanValue></InitialValue></DeclareVariable></VariableDeclarations><PostCondition ColNo="2" LineNo="6"><EQNumeric ColNo="18" LineNo="6"><IntegerVariable>n</IntegerVariable><IntegerValue>2</IntegerValue></EQNumeric></PostCondition><NodeBody><NodeList><Node ColNo="0" LineNo="8" NodeType="Assignment"><NodeId>theFailure</NodeId><Priority>2</Priority><InvariantCondition ColNo="4" LineNo="11"><NOT ColNo="23" LineNo="11"><BooleanVariable>spoil</BooleanVariable></NOT></InvariantCondition><NodeBody><Assignment ColNo="4" LineNo="12"><IntegerVariable>n</IntegerVariable><NumericRHS><ADD ColNo="10" LineNo="12"><IntegerVariable>n</IntegerVariable><IntegerValue>1</IntegerValue></ADD></NumericRHS></Assignment></NodeBody></Node><Node ColNo="0" LineNo="15" NodeType="Assignment"><NodeId>theSpoiler</NodeId><NodeBody><Assignment ColNo="2" LineNo="16"><BooleanVariable>spoil</BooleanVariable><BooleanRHS><BooleanValue>true</BooleanValue></BooleanRHS></Assignment></NodeBody></Node><Node ColNo="0" LineNo="18" NodeType="Assignment"><NodeId>theConflict</NodeId><Priority>1</Priority><StartCondition ColNo="4" LineNo="21"><BooleanVariable>spoil</BooleanVariable></StartCondition><NodeBody><Assignment ColNo="4" LineNo="22"><IntegerVariable>n</IntegerVariable><NumericRHS><IntegerValue>2</IntegerValue></NumericRHS></Assignment></NodeBody></Node></NodeList></NodeBody></Node></PlexilPlan>
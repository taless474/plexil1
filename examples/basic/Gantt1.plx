<?xml version="1.0" encoding="UTF-8"?><PlexilPlan xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:tr="extended-plexil-translator" FileName="examples/basic/Gantt1.ple"><GlobalDeclarations LineNo="5" ColNo="5"><StateDeclaration LineNo="5" ColNo="5"><Name>time</Name><Return><Name>_return_0</Name><Type>Real</Type></Return></StateDeclaration></GlobalDeclarations><Node NodeType="NodeList" epx="Sequence" LineNo="7" ColNo="0"><NodeId>Gantt1</NodeId><VariableDeclarations><DeclareVariable LineNo="9" ColNo="2"><Name>x</Name><Type>Integer</Type><InitialValue><IntegerValue>0</IntegerValue></InitialValue></DeclareVariable><DeclareVariable LineNo="9" ColNo="2"><Name>y</Name><Type>Integer</Type><InitialValue><IntegerValue>0</IntegerValue></InitialValue></DeclareVariable><DeclareVariable LineNo="10" ColNo="2"><Name>thresh</Name><Type>Real</Type><InitialValue><RealValue>0.1</RealValue></InitialValue></DeclareVariable></VariableDeclarations><InvariantCondition><NoChildFailed><NodeRef dir="self"/></NoChildFailed></InvariantCondition><NodeBody><NodeList><Node NodeType="Assignment" LineNo="12" ColNo="2"><NodeId>Activity1</NodeId><NodeBody><Assignment LineNo="14" ColNo="4"><IntegerVariable>x</IntegerVariable><NumericRHS><IntegerValue>1</IntegerValue></NumericRHS></Assignment></NodeBody></Node><Node NodeType="Assignment" LineNo="17" ColNo="2"><NodeId>Activity2</NodeId><StartCondition><Finished><NodeRef dir="sibling">Activity1</NodeRef></Finished></StartCondition><EndCondition><GE><LookupOnChange><Name><StringValue>time</StringValue></Name><Tolerance><RealVariable>thresh</RealVariable></Tolerance></LookupOnChange><ADD LineNo="19" ColNo="56"><NodeTimepointValue><NodeId>Activity1</NodeId><NodeStateValue>FINISHED</NodeStateValue><Timepoint>START</Timepoint></NodeTimepointValue><RealValue>1.0</RealValue></ADD></GE></EndCondition><NodeBody><Assignment LineNo="20" ColNo="4"><IntegerVariable>x</IntegerVariable><NumericRHS><IntegerValue>2</IntegerValue></NumericRHS></Assignment></NodeBody></Node><Node NodeType="NodeList" epx="Concurrence" LineNo="23" ColNo="2"><NodeId>Activity3</NodeId><StartCondition><Finished><NodeRef dir="sibling">Activity2</NodeRef></Finished></StartCondition><EndCondition><GE><LookupOnChange><Name><StringValue>time</StringValue></Name><Tolerance><RealVariable>thresh</RealVariable></Tolerance></LookupOnChange><ADD LineNo="25" ColNo="56"><NodeTimepointValue><NodeId>Activity2</NodeId><NodeStateValue>FINISHED</NodeStateValue><Timepoint>START</Timepoint></NodeTimepointValue><RealValue>1.0</RealValue></ADD></GE></EndCondition><NodeBody><NodeList><Node NodeType="Assignment" LineNo="26" ColNo="4"><NodeId>Activity4</NodeId><EndCondition><GE><LookupOnChange><Name><StringValue>time</StringValue></Name><Tolerance><RealVariable>thresh</RealVariable></Tolerance></LookupOnChange><ADD LineNo="28" ColNo="59"><NodeTimepointValue><NodeId>Activity3</NodeId><NodeStateValue>EXECUTING</NodeStateValue><Timepoint>START</Timepoint></NodeTimepointValue><RealValue>1.0</RealValue></ADD></GE></EndCondition><NodeBody><Assignment LineNo="29" ColNo="6"><IntegerVariable>x</IntegerVariable><NumericRHS><IntegerValue>3</IntegerValue></NumericRHS></Assignment></NodeBody></Node><Node NodeType="Assignment" LineNo="32" ColNo="4"><NodeId>Activity5</NodeId><EndCondition><GE><LookupOnChange><Name><StringValue>time</StringValue></Name><Tolerance><RealVariable>thresh</RealVariable></Tolerance></LookupOnChange><ADD LineNo="34" ColNo="59"><NodeTimepointValue><NodeId>Activity4</NodeId><NodeStateValue>EXECUTING</NodeStateValue><Timepoint>START</Timepoint></NodeTimepointValue><RealValue>1.0</RealValue></ADD></GE></EndCondition><NodeBody><Assignment LineNo="35" ColNo="6"><IntegerVariable>y</IntegerVariable><NumericRHS><IntegerValue>4</IntegerValue></NumericRHS></Assignment></NodeBody></Node></NodeList></NodeBody></Node><Node NodeType="Assignment" LineNo="39" ColNo="2"><NodeId>Activity6</NodeId><StartCondition><Finished><NodeRef dir="sibling">Activity3</NodeRef></Finished></StartCondition><EndCondition><GE><LookupOnChange><Name><StringValue>time</StringValue></Name><Tolerance><RealVariable>thresh</RealVariable></Tolerance></LookupOnChange><ADD LineNo="41" ColNo="56"><NodeTimepointValue><NodeId>Activity3</NodeId><NodeStateValue>FINISHED</NodeStateValue><Timepoint>START</Timepoint></NodeTimepointValue><RealValue>1.0</RealValue></ADD></GE></EndCondition><NodeBody><Assignment LineNo="42" ColNo="4"><IntegerVariable>x</IntegerVariable><NumericRHS><IntegerValue>5</IntegerValue></NumericRHS></Assignment></NodeBody></Node></NodeList></NodeBody></Node></PlexilPlan>
<?xml version="1.0" encoding="UTF-8"?><PlexilPlan xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:tr="extended-plexil-translator" FileName="DriveToTarget.ple"><GlobalDeclarations LineNo="4" ColNo="0"><CommandDeclaration LineNo="4" ColNo="0"><Name>rover_drive</Name><Parameter><Type>Integer</Type></Parameter></CommandDeclaration><CommandDeclaration LineNo="5" ColNo="0"><Name>rover_stop</Name></CommandDeclaration><CommandDeclaration LineNo="6" ColNo="0"><Name>take_navcam</Name></CommandDeclaration><CommandDeclaration LineNo="7" ColNo="0"><Name>take_pancam</Name></CommandDeclaration><CommandDeclaration LineNo="8" ColNo="0"><Name>turn_on_heater</Name></CommandDeclaration><StateDeclaration LineNo="10" ColNo="5"><Name>time</Name><Return><Name>_return_0</Name><Type>Real</Type></Return></StateDeclaration><StateDeclaration LineNo="11" ColNo="5"><Name>temperature</Name><Return><Name>_return_0</Name><Type>Real</Type></Return></StateDeclaration><StateDeclaration LineNo="12" ColNo="8"><Name>target_in_view</Name><Return><Name>_return_0</Name><Type>Boolean</Type></Return></StateDeclaration></GlobalDeclarations><Node NodeType="NodeList" epx="Concurrence" LineNo="16" ColNo="0"><NodeId>DriveToTarget</NodeId><VariableDeclarations><DeclareVariable LineNo="16" ColNo="2"><Name>drive_done</Name><Type>Boolean</Type><InitialValue><BooleanValue>false</BooleanValue></InitialValue></DeclareVariable><DeclareVariable LineNo="16" ColNo="2"><Name>timeout</Name><Type>Boolean</Type><InitialValue><BooleanValue>false</BooleanValue></InitialValue></DeclareVariable></VariableDeclarations><NodeBody><NodeList><Node NodeType="Command" LineNo="17" ColNo="10"><NodeId>Drive</NodeId><NodeBody><Command><Name><StringValue>rover_drive</StringValue></Name><Arguments LineNo="18" ColNo="22"><IntegerValue>10</IntegerValue></Arguments></Command></NodeBody></Node><Node NodeType="NodeList" epx="Sequence" LineNo="21" ColNo="4"><NodeId>StopForTimeout</NodeId><StartCondition><GE><LookupOnChange><Name><StringValue>time</StringValue></Name></LookupOnChange><IntegerValue>10</IntegerValue></GE></StartCondition><InvariantCondition><NOT><AND><EQInternal><NodeOutcomeVariable><NodeRef dir="child">Concurrence__0</NodeRef></NodeOutcomeVariable><NodeOutcomeValue>FAILURE</NodeOutcomeValue></EQInternal><EQInternal><NodeStateVariable><NodeRef dir="child">Concurrence__0</NodeRef></NodeStateVariable><NodeStateValue>FINISHED</NodeStateValue></EQInternal></AND></NOT></InvariantCondition><NodeBody><NodeList><Node NodeType="NodeList" epx="Concurrence" LineNo="22" ColNo="4"><NodeId>Concurrence__0</NodeId><NodeBody><NodeList><Node NodeType="Command" LineNo="22" ColNo="12"><NodeId>Stop</NodeId><NodeBody><Command><Name><StringValue>rover_stop</StringValue></Name></Command></NodeBody></Node><Node NodeType="Assignment" LineNo="24" ColNo="22"><NodeId>SetTimeoutFlag</NodeId><NodeBody><Assignment><BooleanVariable>timeout</BooleanVariable><BooleanRHS><BooleanValue>true</BooleanValue></BooleanRHS></Assignment></NodeBody></Node></NodeList></NodeBody></Node></NodeList></NodeBody></Node><Node NodeType="NodeList" epx="Concurrence" LineNo="29" ColNo="2"><NodeId>StopForTarget</NodeId><StartCondition><LookupOnChange><Name><StringValue>target_in_view</StringValue></Name></LookupOnChange></StartCondition><SkipCondition><BooleanVariable>timeout</BooleanVariable></SkipCondition><NodeBody><NodeList><Node NodeType="Command" LineNo="31" ColNo="10"><NodeId>Stop</NodeId><NodeBody><Command><Name><StringValue>rover_stop</StringValue></Name></Command></NodeBody></Node><Node NodeType="Assignment" LineNo="33" ColNo="18"><NodeId>SetDriveFlag</NodeId><NodeBody><Assignment><BooleanVariable>drive_done</BooleanVariable><BooleanRHS><BooleanValue>true</BooleanValue></BooleanRHS></Assignment></NodeBody></Node></NodeList></NodeBody></Node><Node NodeType="Command" LineNo="39" ColNo="4"><NodeId>TakeNavcam</NodeId><StartCondition><BooleanVariable>timeout</BooleanVariable></StartCondition><SkipCondition><BooleanVariable>drive_done</BooleanVariable></SkipCondition><NodeBody><Command><Name><StringValue>take_navcam</StringValue></Name></Command></NodeBody></Node><Node NodeType="Command" LineNo="46" ColNo="4"><NodeId>TakePancam</NodeId><StartCondition><BooleanVariable>drive_done</BooleanVariable></StartCondition><SkipCondition><BooleanVariable>timeout</BooleanVariable></SkipCondition><NodeBody><Command><Name><StringValue>take_pancam</StringValue></Name></Command></NodeBody></Node><Node NodeType="Command" LineNo="54" ColNo="4"><NodeId>Heater</NodeId><SkipCondition><BooleanVariable>timeout</BooleanVariable></SkipCondition><StartCondition><LT><LookupOnChange><Name><StringValue>temperature</StringValue></Name></LookupOnChange><IntegerValue>0</IntegerValue></LT></StartCondition><EndCondition><GE><LookupOnChange><Name><StringValue>temperature</StringValue></Name></LookupOnChange><IntegerValue>10</IntegerValue></GE></EndCondition><NodeBody><Command><Name><StringValue>turn_on_heater</StringValue></Name></Command></NodeBody></Node></NodeList></NodeBody></Node></PlexilPlan>
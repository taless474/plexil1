<?xml version="1.0" encoding="UTF-8"?><PlexilPlan xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:tr="extended-plexil-translator" FileName="DriveToTarget.ple"><GlobalDeclarations LineNo="3" ColNo="0"><CommandDeclaration LineNo="3" ColNo="0"><Name>rover_drive</Name><Parameter><Type>Integer</Type></Parameter></CommandDeclaration><CommandDeclaration LineNo="4" ColNo="0"><Name>rover_stop</Name></CommandDeclaration><CommandDeclaration LineNo="5" ColNo="0"><Name>take_navcam</Name></CommandDeclaration><CommandDeclaration LineNo="6" ColNo="0"><Name>take_pancam</Name></CommandDeclaration><CommandDeclaration LineNo="7" ColNo="0"><Name>turn_on_heater</Name></CommandDeclaration><StateDeclaration LineNo="9" ColNo="5"><Name>time</Name><Return><Name>_return_0</Name><Type>Real</Type></Return></StateDeclaration><StateDeclaration LineNo="10" ColNo="5"><Name>temperature</Name><Return><Name>_return_0</Name><Type>Real</Type></Return></StateDeclaration><StateDeclaration LineNo="11" ColNo="8"><Name>target_in_view</Name><Return><Name>_return_0</Name><Type>Boolean</Type></Return></StateDeclaration></GlobalDeclarations><Node NodeType="NodeList" epx="Concurrence" LineNo="14" ColNo="0"><NodeId>DriveToTarget</NodeId><VariableDeclarations><DeclareVariable LineNo="16" ColNo="2"><Name>drive_done</Name><Type>Boolean</Type><InitialValue><BooleanValue>false</BooleanValue></InitialValue></DeclareVariable><DeclareVariable LineNo="16" ColNo="2"><Name>timeout</Name><Type>Boolean</Type><InitialValue><BooleanValue>false</BooleanValue></InitialValue></DeclareVariable></VariableDeclarations><NodeBody><NodeList><Node NodeType="Command" LineNo="17" ColNo="2"><NodeId>Drive</NodeId><NodeBody><Command LineNo="17" ColNo="10"><Name><StringValue>rover_drive</StringValue></Name><Arguments><IntegerValue>10</IntegerValue></Arguments></Command></NodeBody></Node><Node NodeType="NodeList" epx="Sequence" LineNo="18" ColNo="2"><NodeId>StopForTimeout</NodeId><StartCondition><GE><LookupOnChange><Name><StringValue>time</StringValue></Name></LookupOnChange><IntegerValue>10</IntegerValue></GE></StartCondition><InvariantCondition><NoChildFailed><NodeRef dir="self"/></NoChildFailed></InvariantCondition><NodeBody><NodeList><Node NodeType="NodeList" epx="Concurrence" LineNo="21" ColNo="4"><NodeId>Concurrence__0</NodeId><NodeBody><NodeList><Node NodeType="Command" LineNo="22" ColNo="6"><NodeId>Stop</NodeId><NodeBody><Command LineNo="22" ColNo="12"><Name><StringValue>rover_stop</StringValue></Name></Command></NodeBody></Node><Node NodeType="Assignment" LineNo="23" ColNo="6"><NodeId>SetTimeoutFlag</NodeId><NodeBody><Assignment LineNo="23" ColNo="22"><BooleanVariable>timeout</BooleanVariable><BooleanRHS><BooleanValue>true</BooleanValue></BooleanRHS></Assignment></NodeBody></Node></NodeList></NodeBody></Node></NodeList></NodeBody></Node><Node NodeType="NodeList" epx="Concurrence" LineNo="27" ColNo="2"><NodeId>StopForTarget</NodeId><StartCondition><LookupOnChange><Name><StringValue>target_in_view</StringValue></Name></LookupOnChange></StartCondition><SkipCondition><BooleanVariable>timeout</BooleanVariable></SkipCondition><NodeBody><NodeList><Node NodeType="Command" LineNo="31" ColNo="4"><NodeId>Stop</NodeId><NodeBody><Command LineNo="31" ColNo="10"><Name><StringValue>rover_stop</StringValue></Name></Command></NodeBody></Node><Node NodeType="Assignment" LineNo="32" ColNo="4"><NodeId>SetDriveFlag</NodeId><NodeBody><Assignment LineNo="32" ColNo="18"><BooleanVariable>drive_done</BooleanVariable><BooleanRHS><BooleanValue>true</BooleanValue></BooleanRHS></Assignment></NodeBody></Node></NodeList></NodeBody></Node><Node NodeType="Command" LineNo="35" ColNo="2"><NodeId>TakeNavcam</NodeId><StartCondition><BooleanVariable>timeout</BooleanVariable></StartCondition><SkipCondition><BooleanVariable>drive_done</BooleanVariable></SkipCondition><NodeBody><Command LineNo="39" ColNo="4"><Name><StringValue>take_navcam</StringValue></Name></Command></NodeBody></Node><Node NodeType="Command" LineNo="42" ColNo="2"><NodeId>TakePancam</NodeId><StartCondition><BooleanVariable>drive_done</BooleanVariable></StartCondition><SkipCondition><BooleanVariable>timeout</BooleanVariable></SkipCondition><NodeBody><Command LineNo="46" ColNo="4"><Name><StringValue>take_pancam</StringValue></Name></Command></NodeBody></Node><Node NodeType="Command" LineNo="49" ColNo="2"><NodeId>Heater</NodeId><SkipCondition><BooleanVariable>timeout</BooleanVariable></SkipCondition><StartCondition><LT><LookupOnChange><Name><StringValue>temperature</StringValue></Name></LookupOnChange><IntegerValue>0</IntegerValue></LT></StartCondition><EndCondition><GE><LookupOnChange><Name><StringValue>temperature</StringValue></Name></LookupOnChange><IntegerValue>10</IntegerValue></GE></EndCondition><NodeBody><Command LineNo="54" ColNo="4"><Name><StringValue>turn_on_heater</StringValue></Name></Command></NodeBody></Node></NodeList></NodeBody></Node></PlexilPlan>
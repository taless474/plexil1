<?xml version="1.0" encoding="UTF-8"?><PlexilPlan xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" FileName="InactiveAncestorInvariantTest.ple"><Node NodeType="NodeList" epx="Concurrence" ColNo="0" LineNo="1"><NodeId>InactiveAncestorInvariantTest</NodeId><InvariantCondition ColNo="2" LineNo="5"><NOT ColNo="21" LineNo="5"><Executing ColNo="22" LineNo="5"><NodeId>TheParent</NodeId></Executing></NOT></InvariantCondition><NodeBody><NodeList><Node NodeType="NodeList" epx="Sequence" ColNo="1" LineNo="7"><NodeId>TheParent</NodeId><InvariantCondition><NoChildFailed><NodeRef dir="self"/></NoChildFailed></InvariantCondition><NodeBody><NodeList><Node ColNo="2" LineNo="9" NodeType="Empty"><NodeId>TheChild</NodeId></Node></NodeList></NodeBody></Node></NodeList></NodeBody></Node></PlexilPlan>
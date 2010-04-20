<?xml version="1.0" encoding="UTF-8"?>
<PlexilPlan xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:tr="extended-plexil-translator">
   <Node NodeType="NodeList" FileName="simulator-numeric-arrays-test.ple" LineNo="5"
         ColNo="1">
      <NodeId>Interface</NodeId>
      <VariableDeclarations>
              <DeclareArray>
                  <Name>nums</Name>
                  <Type>Integer</Type>
                  <MaxSize>2</MaxSize>
                  <InitialValue>
                      <IntegerValue>5</IntegerValue>
                      <IntegerValue>10</IntegerValue>
                  </InitialValue>
              </DeclareArray>
          </VariableDeclarations>
      <NodeBody>
         <NodeList>
                  <Node NodeType="NodeList" epx="Sequence" FileName="simulator-numeric-arrays-test.ple"
                  LineNo="11"
                  ColNo="17">
               <NodeId>RecGetName</NodeId>
               <VariableDeclarations>
                  <DeclareVariable>
                     <Name>ep2cp_hdl</Name>
                     <Type>String</Type>
                  </DeclareVariable>
               </VariableDeclarations>
               <InvariantCondition>
                  <AND>
                     <AND>
                        <EQInternal>
                           <NodeStateVariable>
                              <NodeId>Interface</NodeId>
                           </NodeStateVariable>
                           <NodeStateValue>EXECUTING</NodeStateValue>
                        </EQInternal>
                     </AND>
                     <NOT>
                        <OR>
                           <EQInternal>
                              <NodeOutcomeVariable>
                                 <NodeId>ep2cp_CmdWait</NodeId>
                              </NodeOutcomeVariable>
                              <NodeOutcomeValue>FAILURE</NodeOutcomeValue>
                           </EQInternal>
                           <EQInternal>
                              <NodeOutcomeVariable>
                                 <NodeId>ep2cp_CmdAction_GetNums</NodeId>
                              </NodeOutcomeVariable>
                              <NodeOutcomeValue>FAILURE</NodeOutcomeValue>
                           </EQInternal>
                        </OR>
                     </NOT>
                  </AND>
               </InvariantCondition>
               <NodeBody>
                  <NodeList>
                     <Node NodeType="Command">
                        <NodeId>ep2cp_CmdWait</NodeId>
                        <EndCondition>
                           <IsKnown>
                              <StringVariable>ep2cp_hdl</StringVariable>
                           </IsKnown>
                        </EndCondition>
                        <NodeBody>
                           <Command>
                              <StringVariable>ep2cp_hdl</StringVariable>
                              <Name>
                                 <StringValue>ReceiveCommand</StringValue>
                              </Name>
                              <Arguments>
                                 <StringValue>GetNums</StringValue>
                              </Arguments>
                           </Command>
                        </NodeBody>
                     </Node>
                     <Node NodeType="NodeList">
                        <NodeId>ep2cp_CmdAction_GetNums</NodeId>
                        <StartCondition>
                           <AND>
                              <EQInternal>
                                 <NodeStateVariable>
                                    <NodeId>ep2cp_CmdWait</NodeId>
                                 </NodeStateVariable>
                                 <NodeStateValue>FINISHED</NodeStateValue>
                              </EQInternal>
                           </AND>
                        </StartCondition>
                        <NodeBody>
                           <NodeList>
                              <Node NodeType="Command" FileName="simulator-numeric-arrays-test.ple" LineNo="13"
                                    ColNo="33">
                                 <NodeId>RecGetName__CHILD__2</NodeId>
                                 <NodeBody>
                                    <Command>
                                       <Name>
                                          <StringValue>SendReturnValue</StringValue>
                                       </Name>
                                  
                                       <Arguments>
                                          <StringVariable>ep2cp_hdl</StringVariable>
                                          <ArrayVariable>nums</ArrayVariable>
                                       </Arguments>
                                    </Command>
                                 </NodeBody>
                              </Node>
                           </NodeList>
                        </NodeBody>
                     </Node>
                  </NodeList>
               </NodeBody>
            </Node>
              </NodeList>
      </NodeBody>
   </Node>
</PlexilPlan>
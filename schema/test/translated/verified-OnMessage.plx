<?xml version="1.0" encoding="UTF-8"?>
<PlexilPlan xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:tr="extended-plexil-translator">
   <Node NodeType="NodeList" epx="Sequence">
      <NodeId>ep2cp_Sequence_d2e1</NodeId>
      <VariableDeclarations>
         <DeclareVariable>
            <Name>ep2cp_hdl</Name>
            <Type>String</Type>
         </DeclareVariable>
      </VariableDeclarations>
      <InvariantCondition>
         <AND>
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
                        <NodeId>ep2cp_MsgAction_</NodeId>
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
                        <StringValue>ReceiveMessage</StringValue>
                     </Name>
                     <Arguments>
                        <StringValue>move rover</StringValue>
                     </Arguments>
                  </Command>
               </NodeBody>
            </Node>
            <Node NodeType="NodeList">
               <NodeId>ep2cp_MsgAction_</NodeId>
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
                     <Node NodeType="Empty">
                        <NodeId>foo</NodeId>
                     </Node>
                     <Node NodeType="Empty">
                        <NodeId>bar</NodeId>
                     </Node>
                  </NodeList>
               </NodeBody>
            </Node>
         </NodeList>
      </NodeBody>
   </Node>
</PlexilPlan>
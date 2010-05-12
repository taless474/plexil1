<?xml version="1.0" encoding="UTF-8"?>
<PlexilPlan xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:tr="extended-plexil-translator">
   <Node NodeType="NodeList" epx="Sequence">
      <NodeId>ep2cp_Sequence_d1e5</NodeId>
      <VariableDeclarations>
         <DeclareVariable>
            <Name>foo</Name>
            <Type>Integer</Type>
            <InitialValue>
               <IntegerValue>0</IntegerValue>
            </InitialValue>
         </DeclareVariable>
      </VariableDeclarations>
      <InvariantCondition>
         <AND>
            <NOT>
               <OR>
                  <EQInternal>
                     <NodeOutcomeVariable>
                        <NodeId>One</NodeId>
                     </NodeOutcomeVariable>
                     <NodeOutcomeValue>FAILURE</NodeOutcomeValue>
                  </EQInternal>
                  <EQInternal>
                     <NodeOutcomeVariable>
                        <NodeId>Two</NodeId>
                     </NodeOutcomeVariable>
                     <NodeOutcomeValue>FAILURE</NodeOutcomeValue>
                  </EQInternal>
               </OR>
            </NOT>
         </AND>
      </InvariantCondition>
      <NodeBody>
         <NodeList>
            <Node NodeType="Assignment">
               <NodeId>One</NodeId>
               <PostCondition>
                  <EQNumeric>
                     <IntegerVariable>foo</IntegerVariable>
                     <IntegerValue>3</IntegerValue>
                  </EQNumeric>
               </PostCondition>
               <NodeBody>
                  <Assignment>
                     <IntegerVariable>foo</IntegerVariable>
                     <NumericRHS>
                        <IntegerValue>3</IntegerValue>
                     </NumericRHS>
                  </Assignment>
               </NodeBody>
               <SkipCondition>
                  <EQNumeric>
                     <IntegerVariable>foo</IntegerVariable>
                     <IntegerValue>3</IntegerValue>
                  </EQNumeric>
               </SkipCondition>
            </Node>
            <Node NodeType="Assignment">
               <NodeId>Two</NodeId>
               <StartCondition>
                  <AND>
                     <EQInternal>
                        <NodeStateVariable>
                           <NodeId>One</NodeId>
                        </NodeStateVariable>
                        <NodeStateValue>FINISHED</NodeStateValue>
                     </EQInternal>
                  </AND>
               </StartCondition>
               <PostCondition>
                  <EQNumeric>
                     <IntegerVariable>foo</IntegerVariable>
                     <IntegerValue>6</IntegerValue>
                  </EQNumeric>
               </PostCondition>
               <NodeBody>
                  <Assignment>
                     <IntegerVariable>foo</IntegerVariable>
                     <NumericRHS>
                        <IntegerValue>6</IntegerValue>
                     </NumericRHS>
                  </Assignment>
               </NodeBody>
               <SkipCondition>
                  <AND>
                     <EQInternal>
                        <NodeStateVariable>
                           <NodeId>One</NodeId>
                        </NodeStateVariable>
                        <NodeStateValue>FINISHED</NodeStateValue>
                     </EQInternal>
                     <EQNumeric>
                        <IntegerVariable>foo</IntegerVariable>
                        <IntegerValue>0</IntegerValue>
                     </EQNumeric>
                  </AND>
               </SkipCondition>
            </Node>
         </NodeList>
      </NodeBody>
   </Node>
</PlexilPlan>
<?xml version="1.0" encoding="UTF-8"?>
<!-- Generated by PlexiLisp --><PlexilPlan xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:tr="extended-plexil-translator">
   <Node NodeType="Assignment">
      <NodeId>GetValue</NodeId>
      <Interface>
         <InOut>
            <DeclareVariable>
               <Name>foo</Name>
               <Type>Boolean</Type>
            </DeclareVariable>
         </InOut>
      </Interface>
      <StartCondition>
         <LookupOnChange>
            <Name>
               <StringValue>ValueState</StringValue>
            </Name>
         </LookupOnChange>
      </StartCondition>
      <NodeBody>
         <Assignment>
            <BooleanVariable>foo</BooleanVariable>
            <BooleanRHS>
               <BooleanValue>1</BooleanValue>
            </BooleanRHS>
         </Assignment>
      </NodeBody>
   </Node>
</PlexilPlan>
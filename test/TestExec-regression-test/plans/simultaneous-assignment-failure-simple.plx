<?xml version="1.0" encoding="UTF-8"?>
<PlexilPlan xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:tr="extended-plexil-translator">
   <Node NodeType="NodeList" epx="Concurrence" LineNo="2" ColNo="37">
      <NodeId>SimultaneousAssignmentFailureSimple</NodeId>
      <VariableDeclarations>
         <DeclareVariable LineNo="2" ColNo="2">
            <Name>foo</Name>
            <Type>Integer</Type>
            <InitialValue>
               <IntegerValue>0</IntegerValue>
            </InitialValue>
         </DeclareVariable>
      </VariableDeclarations>
      <NodeBody>
         <NodeList>
            <Node NodeType="Assignment" LineNo="5" ColNo="4">
               <NodeId>node1</NodeId>
               <NodeBody>
                  <Assignment>
                     <IntegerVariable>foo</IntegerVariable>
                     <NumericRHS>
                        <IntegerValue>2</IntegerValue>
                     </NumericRHS>
                  </Assignment>
               </NodeBody>
            </Node>
            <Node NodeType="Assignment" LineNo="8" ColNo="4">
               <NodeId>node2</NodeId>
               <NodeBody>
                  <Assignment>
                     <IntegerVariable>foo</IntegerVariable>
                     <NumericRHS>
                        <IntegerValue>3</IntegerValue>
                     </NumericRHS>
                  </Assignment>
               </NodeBody>
            </Node>
         </NodeList>
      </NodeBody>
   </Node>
</PlexilPlan>
<?xml version="1.0" encoding="UTF-8"?><!-- Generated by PlexiLisp --><PlexilPlan><Node NodeType="NodeList"><NodeId>repeat4</NodeId><VariableDeclarations>
      <DeclareVariable>
        <Name>done</Name>
        <Type>Boolean</Type>
        <InitialValue>
          <BooleanValue>false</BooleanValue>
        </InitialValue>
      </DeclareVariable>
      <DeclareVariable>
        <Name>doset</Name>
        <Type>Boolean</Type>
        <InitialValue>
          <BooleanValue>true</BooleanValue>
        </InitialValue>
      </DeclareVariable>
    </VariableDeclarations><EndCondition><BooleanVariable>done</BooleanVariable></EndCondition><NodeBody>
      <NodeList>
        <Node NodeType="Assignment"><NodeId>A</NodeId><StartCondition><BooleanVariable>doset</BooleanVariable></StartCondition><RepeatCondition><BooleanValue>1</BooleanValue></RepeatCondition><NodeBody>
            <Assignment>
              <BooleanVariable>doset</BooleanVariable>
              <BooleanRHS>
                <BooleanValue>0</BooleanValue>
              </BooleanRHS>
            </Assignment>
          </NodeBody></Node>
        <Node NodeType="Assignment"><NodeId>B</NodeId><StartCondition><EQInternal><NodeStateVariable><NodeId>A</NodeId></NodeStateVariable><NodeStateValue>ITERATION_ENDED</NodeStateValue></EQInternal></StartCondition><RepeatCondition><BooleanValue>1</BooleanValue></RepeatCondition><NodeBody>
            <Assignment>
              <BooleanVariable>done</BooleanVariable>
              <BooleanRHS>
                <BooleanValue>1</BooleanValue>
              </BooleanRHS>
            </Assignment>
          </NodeBody></Node>
      </NodeList>
    </NodeBody></Node></PlexilPlan>
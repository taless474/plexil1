<?xml version="1.0" encoding="UTF-8"?>
<PlexilPlan xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:tr="extended-plexil-translator">
  <Node NodeType="NodeList" epx="For">
    <NodeId>Root</NodeId>
    <VariableDeclarations>
      <DeclareVariable>
        <Name>foo</Name>
        <Type>Boolean</Type>
        <InitialValue>
          <BooleanValue>true</BooleanValue>
        </InitialValue>
      </DeclareVariable>
      <DeclareVariable>
        <Name>count</Name>
        <Type>Integer</Type>
        <InitialValue>
          <IntegerValue>0</IntegerValue>
        </InitialValue>
      </DeclareVariable>
    </VariableDeclarations>
    <NodeBody>
      <NodeList>
        <Node NodeType="NodeList" epx="aux">
          <NodeId>ep2cp_ForLoop</NodeId>
          <SkipCondition>
            <NOT>
              <LT>
                <IntegerVariable>count</IntegerVariable>
                <IntegerValue>3</IntegerValue>
              </LT>
            </NOT>
          </SkipCondition>
          <RepeatCondition>
            <BooleanValue>true</BooleanValue>
          </RepeatCondition>
          <NodeBody>
            <NodeList>
              <Node NodeType="NodeList" epx="Sequence">
                <NodeId>ep2cp_Sequence_d1e71</NodeId>
                <InvariantCondition>
                  <NOT>
                    <OR>
                      <Failed>
                        <NodeRef dir="child">Two</NodeRef>
                      </Failed>
                      <Failed>
                        <NodeRef dir="child">Four</NodeRef>
                      </Failed>
                    </OR>
                  </NOT>
                </InvariantCondition>
                <NodeBody>
                  <NodeList>
                    <Node NodeType="Empty">
                      <NodeId>Two</NodeId>
                    </Node>
                    <Node NodeType="Empty">
                      <NodeId>Four</NodeId>
                      <StartCondition>
                        <Finished>
                          <NodeRef dir="sibling">Two</NodeRef>
                        </Finished>
                      </StartCondition>
                    </Node>
                  </NodeList>
                </NodeBody>
              </Node>
              <Node NodeType="Assignment" epx="LoopVariableUpdate">
                <NodeId>ep2cp_ForLoopUpdater</NodeId>
                <StartCondition>
                  <Finished>
                    <NodeRef dir="sibling">ep2cp_Sequence_d1e71</NodeRef>
                  </Finished>
                </StartCondition>
                <NodeBody>
                  <Assignment>
                    <IntegerVariable>count</IntegerVariable>
                    <NumericRHS>
                      <ADD>
                        <IntegerVariable>count</IntegerVariable>
                        <IntegerValue>1</IntegerValue>
                      </ADD>
                    </NumericRHS>
                  </Assignment>
                </NodeBody>
              </Node>
            </NodeList>
          </NodeBody>
        </Node>
      </NodeList>
    </NodeBody>
  </Node>
</PlexilPlan>

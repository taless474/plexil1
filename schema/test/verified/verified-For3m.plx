<?xml version="1.0" encoding="UTF-8"?>
<PlexilPlan>
  <GlobalDeclarations>
    <MutexDeclaration>
      <Name>m</Name>
    </MutexDeclaration>
  </GlobalDeclarations>
  <Node NodeType="NodeList" epx="For">
    <NodeId>ep2cp_For_d1e12</NodeId>
    <VariableDeclarations>
      <DeclareVariable>
        <Name>count</Name>
        <Type>Real</Type>
        <InitialValue>
          <RealValue>0.0</RealValue>
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
                <RealVariable>count</RealVariable>
                <RealValue>1.0</RealValue>
              </LT>
            </NOT>
          </SkipCondition>
          <RepeatCondition>
            <BooleanValue>true</BooleanValue>
          </RepeatCondition>
          <NodeBody>
            <NodeList>
              <Node NodeType="NodeList" epx="Concurrence">
                <NodeId>ep2cp_Concurrence_d1e58</NodeId>
                <Mutexes>
                  <Name>
                    <StringValue>m</StringValue>
                  </Name>
                </Mutexes>
                <NodeBody>
                  <NodeList>
                    <Node NodeType="Empty">
                      <NodeId>One</NodeId>
                    </Node>
                    <Node NodeType="Empty">
                      <NodeId>Three</NodeId>
                    </Node>
                  </NodeList>
                </NodeBody>
              </Node>
              <Node NodeType="Assignment" epx="LoopVariableUpdate">
                <NodeId>ep2cp_ForLoopUpdater</NodeId>
                <StartCondition>
                  <EQInternal>
                    <NodeStateVariable>
                      <NodeRef dir="sibling">ep2cp_Concurrence_d1e58</NodeRef>
                    </NodeStateVariable>
                    <NodeStateValue>FINISHED</NodeStateValue>
                  </EQInternal>
                </StartCondition>
                <NodeBody>
                  <Assignment>
                    <RealVariable>count</RealVariable>
                    <NumericRHS>
                      <ADD>
                        <RealVariable>count</RealVariable>
                        <RealValue>0.2</RealValue>
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

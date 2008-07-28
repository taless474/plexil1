  <PlexilPlan xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="plexil.xsd">
      <Node NodeType="NodeList">
          <VariableDeclarations>
              <DeclareVariable>
                  <Name>StartX</Name>
                  <Type>Integer</Type>
              </DeclareVariable>
              <DeclareVariable>
                  <Name>StartY</Name>
                  <Type>Integer</Type>
              </DeclareVariable>
              <DeclareVariable>
                  <Name>EndX</Name>
                  <Type>Integer</Type>
              </DeclareVariable>
              <DeclareVariable>
                  <Name>EndY</Name>
                  <Type>Integer</Type>
              </DeclareVariable>
          </VariableDeclarations>
          <NodeId>FourSteps</NodeId>
          <PostCondition>
              <AND>
                  <EQNumeric>
                      <IntegerVariable>StartX</IntegerVariable>
                      <IntegerVariable>EndX</IntegerVariable>
                  </EQNumeric>
                  <EQNumeric>
                      <IntegerVariable>StartY</IntegerVariable>
                      <IntegerVariable>EndY</IntegerVariable>
                  </EQNumeric>
              </AND>
          </PostCondition>
          <NodeBody>
              <NodeList>
                  <Node NodeType="NodeList">
                      <VariableDeclarations>
                          <DeclareArray>
                              <Name>RobotState</Name>
                              <Type>Real</Type>
                              <MaxSize>3</MaxSize>
                          </DeclareArray>
                      </VariableDeclarations>
                      <NodeId>GetStartPosition</NodeId>
                      <NodeBody>
                          <NodeList>
                              <Node NodeType="Command">
                                  <NodeId>GetRobotState</NodeId>
                                  <EndCondition>
                                      <IsKnown>
                                          <ArrayElement>
                                              <Name>RobotState</Name>
                                              <Index>
                                                  <IntegerValue>0</IntegerValue>
                                              </Index>
                                          </ArrayElement>
                                      </IsKnown>
                                  </EndCondition>
                                  <NodeBody>
                                      <Command>
                                          <ArrayVariable>RobotState</ArrayVariable>
                                          <Name>
                                              <StringValue>QueryRobotState</StringValue>
                                          </Name>
                                      </Command>
                                  </NodeBody>
                              </Node>
                              <Node NodeType="Assignment">
                                  <NodeId>SetStartX</NodeId>
                                  <StartCondition>
                                      <EQInternal>
                                          <NodeStateVariable>
                                              <NodeId>GetRobotState</NodeId>
                                          </NodeStateVariable>
                                          <NodeStateValue>FINISHED</NodeStateValue>
                                      </EQInternal>
                                  </StartCondition>
                                  <NodeBody>
                                      <Assignment>
                                          <IntegerVariable>StartX</IntegerVariable>
                                          <NumericRHS>
                                              <ArrayElement>
                                                  <Name>RobotState</Name>
                                                  <Index>
                                                      <IntegerValue>0</IntegerValue>
                                                  </Index>
                                              </ArrayElement>
                                          </NumericRHS>
                                      </Assignment>
                                  </NodeBody>
                              </Node>
                              <Node NodeType="Assignment">
                                  <NodeId>SetStartY</NodeId>
                                  <StartCondition>
                                      <EQInternal>
                                          <NodeStateVariable>
                                              <NodeId>GetRobotState</NodeId>
                                          </NodeStateVariable>
                                          <NodeStateValue>FINISHED</NodeStateValue>
                                      </EQInternal>
                                  </StartCondition>
                                  <NodeBody>
                                      <Assignment>
                                          <IntegerVariable>StartY</IntegerVariable>
                                          <NumericRHS>
                                              <ArrayElement>
                                                  <Name>RobotState</Name>
                                                  <Index>
                                                      <IntegerValue>1</IntegerValue>
                                                  </Index>
                                              </ArrayElement>
                                          </NumericRHS>
                                      </Assignment>
                                  </NodeBody>
                              </Node>
                          </NodeList>
                      </NodeBody>
                  </Node>
                  <Node NodeType="Command">
                      <VariableDeclarations>
                          <DeclareVariable>
                              <Name>result</Name>
                              <Type>Integer</Type>
                          </DeclareVariable>
                      </VariableDeclarations>
                      <NodeId>MoveLeft</NodeId>
                      <StartCondition>
                          <EQInternal>
                              <NodeStateVariable>
                                  <NodeId>GetStartPosition</NodeId>
                              </NodeStateVariable>
                              <NodeStateValue>FINISHED</NodeStateValue>
                          </EQInternal>
                      </StartCondition>
                      <EndCondition>
                          <IsKnown>
                              <IntegerVariable>result</IntegerVariable>
                          </IsKnown>
                      </EndCondition>
                      <PostCondition>
                          <EQNumeric>
                              <IntegerVariable>result</IntegerVariable>
                              <IntegerValue>1</IntegerValue>
                          </EQNumeric>
                      </PostCondition>
                      <NodeBody>
                          <Command>
                              <IntegerVariable>result</IntegerVariable>
                              <Name>
                                  <StringValue>MoveLeft</StringValue>
                              </Name>
                          </Command>
                      </NodeBody>
                  </Node>
                  <Node NodeType="Command">
                      <VariableDeclarations>
                          <DeclareVariable>
                              <Name>result</Name>
                              <Type>Integer</Type>
                          </DeclareVariable>
                      </VariableDeclarations>
                      <NodeId>MoveUp</NodeId>
                      <StartCondition>
                          <EQInternal>
                              <NodeStateVariable>
                                  <NodeId>MoveLeft</NodeId>
                              </NodeStateVariable>
                              <NodeStateValue>FINISHED</NodeStateValue>
                          </EQInternal>
                      </StartCondition>
                      <EndCondition>
                          <IsKnown>
                              <IntegerVariable>result</IntegerVariable>
                          </IsKnown>
                      </EndCondition>
                      <PostCondition>
                          <EQNumeric>
                              <IntegerVariable>result</IntegerVariable>
                              <IntegerValue>1</IntegerValue>
                          </EQNumeric>
                      </PostCondition>
                      <NodeBody>
                          <Command>
                              <IntegerVariable>result</IntegerVariable>
                              <Name>
                                  <StringValue>MoveUp</StringValue>
                              </Name>
                          </Command>
                      </NodeBody>
                  </Node>
                  <Node NodeType="Command">
                      <VariableDeclarations>
                          <DeclareVariable>
                              <Name>result</Name>
                              <Type>Integer</Type>
                          </DeclareVariable>
                      </VariableDeclarations>
                      <NodeId>MoveRight</NodeId>
                      <StartCondition>
                          <EQInternal>
                              <NodeStateVariable>
                                  <NodeId>MoveUp</NodeId>
                              </NodeStateVariable>
                              <NodeStateValue>FINISHED</NodeStateValue>
                          </EQInternal>
                      </StartCondition>
                      <EndCondition>
                          <IsKnown>
                              <IntegerVariable>result</IntegerVariable>
                          </IsKnown>
                      </EndCondition>
                      <PostCondition>
                          <EQNumeric>
                              <IntegerVariable>result</IntegerVariable>
                              <IntegerValue>1</IntegerValue>
                          </EQNumeric>
                      </PostCondition>
                      <NodeBody>
                          <Command>
                              <IntegerVariable>result</IntegerVariable>
                              <Name>
                                  <StringValue>MoveRight</StringValue>
                              </Name>
                          </Command>
                      </NodeBody>
                  </Node>
                  <Node NodeType="Command">
                      <VariableDeclarations>
                          <DeclareVariable>
                              <Name>result</Name>
                              <Type>Integer</Type>
                          </DeclareVariable>
                      </VariableDeclarations>
                      <NodeId>MoveDown</NodeId>
                      <StartCondition>
                          <EQInternal>
                              <NodeStateVariable>
                                  <NodeId>MoveRight</NodeId>
                              </NodeStateVariable>
                              <NodeStateValue>FINISHED</NodeStateValue>
                          </EQInternal>
                      </StartCondition>
                      <EndCondition>
                          <IsKnown>
                              <IntegerVariable>result</IntegerVariable>
                          </IsKnown>
                      </EndCondition>
                      <PostCondition>
                          <EQNumeric>
                              <IntegerVariable>result</IntegerVariable>
                              <IntegerValue>1</IntegerValue>
                          </EQNumeric>
                      </PostCondition>
                      <NodeBody>
                          <Command>
                              <IntegerVariable>result</IntegerVariable>
                              <Name>
                                  <StringValue>MoveDown</StringValue>
                              </Name>
                          </Command>
                      </NodeBody>
                  </Node>
                  <Node NodeType="NodeList">
                      <VariableDeclarations>
                          <DeclareArray>
                              <Name>RobotState</Name>
                              <Type>Real</Type>
                              <MaxSize>3</MaxSize>
                          </DeclareArray>
                      </VariableDeclarations>
                      <NodeId>GetEndPosition</NodeId>
                      <StartCondition>
                          <EQInternal>
                              <NodeStateVariable>
                                  <NodeId>MoveDown</NodeId>
                              </NodeStateVariable>
                              <NodeStateValue>FINISHED</NodeStateValue>
                          </EQInternal>
                      </StartCondition>
                      <NodeBody>
                          <NodeList>
                              <Node NodeType="Command">
                                  <NodeId>GetRobotState</NodeId>
                                  <EndCondition>
                                      <IsKnown>
                                          <ArrayElement>
                                              <Name>RobotState</Name>
                                              <Index>
                                                  <IntegerValue>0</IntegerValue>
                                              </Index>
                                          </ArrayElement>
                                      </IsKnown>
                                  </EndCondition>
                                  <NodeBody>
                                      <Command>
                                          <ArrayVariable>RobotState</ArrayVariable>
                                          <Name>
                                              <StringValue>QueryRobotState</StringValue>
                                          </Name>
                                      </Command>
                                  </NodeBody>
                              </Node>
                              <Node NodeType="Assignment">
                                  <NodeId>SetEndX</NodeId>
                                  <StartCondition>
                                      <EQInternal>
                                          <NodeStateVariable>
                                              <NodeId>GetRobotState</NodeId>
                                          </NodeStateVariable>
                                          <NodeStateValue>FINISHED</NodeStateValue>
                                      </EQInternal>
                                  </StartCondition>
                                  <NodeBody>
                                      <Assignment>
                                          <IntegerVariable>EndX</IntegerVariable>
                                          <NumericRHS>
                                              <ArrayElement>
                                                  <Name>RobotState</Name>
                                                  <Index>
                                                      <IntegerValue>0</IntegerValue>
                                                  </Index>
                                              </ArrayElement>
                                          </NumericRHS>
                                      </Assignment>
                                  </NodeBody>
                              </Node>
                              <Node NodeType="Assignment">
                                  <NodeId>SetEndY</NodeId>
                                  <StartCondition>
                                      <EQInternal>
                                          <NodeStateVariable>
                                              <NodeId>GetRobotState</NodeId>
                                          </NodeStateVariable>
                                          <NodeStateValue>FINISHED</NodeStateValue>
                                      </EQInternal>
                                  </StartCondition>
                                  <NodeBody>
                                      <Assignment>
                                          <IntegerVariable>EndY</IntegerVariable>
                                          <NumericRHS>
                                              <ArrayElement>
                                                  <Name>RobotState</Name>
                                                  <Index>
                                                      <IntegerValue>1</IntegerValue>
                                                  </Index>
                                              </ArrayElement>
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

  <PlexilPlan xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="plexil.xsd">
      <Node NodeType="NodeList">
          <NodeId>SimpleTask</NodeId>
          <NodeBody>
              <NodeList>
                  <Node NodeType="Command">
                      <VariableDeclarations>
                          <DeclareVariable>
                              <Name>returnValue</Name>
                              <Type>Integer</Type>
                              <InitialValue>
                                  <IntegerValue>-1</IntegerValue>
                              </InitialValue>
                          </DeclareVariable>
                      </VariableDeclarations>
                      <NodeId>C1</NodeId>
                      <EndCondition>
                          <EQNumeric>
                              <IntegerVariable>returnValue</IntegerVariable>
                              <IntegerValue>10</IntegerValue>
                          </EQNumeric>
                      </EndCondition>
                      <PostCondition>
                          <EQInternal>
                              <NodeCommandHandleVariable>
                                  <NodeId>C1</NodeId>
                              </NodeCommandHandleVariable>
                              <NodeCommandHandleValue>COMMAND_SUCCESS</NodeCommandHandleValue>
                          </EQInternal>
                      </PostCondition>
                      <NodeBody>
                          <Command>
                              <ResourceList>
                                  <Resource>
                                      <ResourceName>
                                          <StringValue>&quot;sys_memory&quot;</StringValue>
                                      </ResourceName>
                                      <ResourceLowerBound>
                                          <RealValue>0.5</RealValue>
                                      </ResourceLowerBound>
                                      <ResourceUpperBound>
                                          <RealValue>0.5</RealValue>
                                      </ResourceUpperBound>
                                      <ResourcePriority>
                                          <IntegerValue>20</IntegerValue>
                                      </ResourcePriority>
                                  </Resource>
                                  <Resource>
                                      <ResourceName>
                                          <StringValue>&quot;arm&quot;</StringValue>
                                      </ResourceName>
                                      <ResourcePriority>
                                          <IntegerValue>20</IntegerValue>
                                      </ResourcePriority>
                                  </Resource>
                              </ResourceList>
                              <IntegerVariable>returnValue</IntegerVariable>
                              <CommandName>c1</CommandName>
                          </Command>
                      </NodeBody>
                  </Node>
                  <Node NodeType="Command">
                      <VariableDeclarations>
                          <DeclareVariable>
                              <Name>mem_priority</Name>
                              <Type>Integer</Type>
                              <InitialValue>
                                  <IntegerValue>30</IntegerValue>
                              </InitialValue>
                          </DeclareVariable>
                          <DeclareVariable>
                              <Name>returnValue</Name>
                              <Type>Integer</Type>
                              <InitialValue>
                                  <IntegerValue>-1</IntegerValue>
                              </InitialValue>
                          </DeclareVariable>
                      </VariableDeclarations>
                      <NodeId>C2</NodeId>
                      <RepeatCondition>
                          <EQInternal>
                              <NodeCommandHandleVariable>
                                  <NodeId>C2</NodeId>
                              </NodeCommandHandleVariable>
                              <NodeCommandHandleValue>COMMAND_DENIED</NodeCommandHandleValue>
                          </EQInternal>
                      </RepeatCondition>
                      <PostCondition>
                          <EQInternal>
                              <NodeCommandHandleVariable>
                                  <NodeId>C2</NodeId>
                              </NodeCommandHandleVariable>
                              <NodeCommandHandleValue>COMMAND_SUCCESS</NodeCommandHandleValue>
                          </EQInternal>
                      </PostCondition>
                      <EndCondition>
                          <EQNumeric>
                              <IntegerVariable>returnValue</IntegerVariable>
                              <IntegerValue>10</IntegerValue>
                          </EQNumeric>
                      </EndCondition>
                      <NodeBody>
                          <Command>
                              <ResourceList>
                                  <Resource>
                                      <ResourceName>
                                          <StringValue>&quot;sys_memory&quot;</StringValue>
                                      </ResourceName>
                                      <ResourceLowerBound>
                                          <RealValue>0.3</RealValue>
                                      </ResourceLowerBound>
                                      <ResourceUpperBound>
                                          <RealValue>0.3</RealValue>
                                      </ResourceUpperBound>
                                      <ResourcePriority>
                                          <IntegerVariable>mem_priority</IntegerVariable>
                                      </ResourcePriority>
                                  </Resource>
                              </ResourceList>
                              <IntegerVariable>returnValue</IntegerVariable>
                              <CommandName>c2</CommandName>
                          </Command>
                      </NodeBody>
                  </Node>
                  <Node NodeType="Command">
                      <VariableDeclarations>
                          <DeclareVariable>
                              <Name>vision_priority</Name>
                              <Type>Integer</Type>
                              <InitialValue>
                                  <IntegerValue>10</IntegerValue>
                              </InitialValue>
                          </DeclareVariable>
                          <DeclareVariable>
                              <Name>returnValue</Name>
                              <Type>Integer</Type>
                              <InitialValue>
                                  <IntegerValue>-1</IntegerValue>
                              </InitialValue>
                          </DeclareVariable>
                      </VariableDeclarations>
                      <NodeId>C3</NodeId>
                      <PostCondition>
                          <EQInternal>
                              <NodeCommandHandleVariable>
                                  <NodeId>C3</NodeId>
                              </NodeCommandHandleVariable>
                              <NodeCommandHandleValue>COMMAND_SUCCESS</NodeCommandHandleValue>
                          </EQInternal>
                      </PostCondition>
                      <EndCondition>
                          <EQNumeric>
                              <IntegerVariable>returnValue</IntegerVariable>
                              <IntegerValue>10</IntegerValue>
                          </EQNumeric>
                      </EndCondition>
                      <NodeBody>
                          <Command>
                              <ResourceList>
                                  <Resource>
                                      <ResourceName>
                                          <StringValue>&quot;vision_system&quot;</StringValue>
                                      </ResourceName>
                                      <ResourcePriority>
                                          <IntegerVariable>vision_priority</IntegerVariable>
                                      </ResourcePriority>
                                  </Resource>
                              </ResourceList>
                              <IntegerVariable>returnValue</IntegerVariable>
                              <CommandName>c3</CommandName>
                          </Command>
                      </NodeBody>
                  </Node>
              </NodeList>
          </NodeBody>
      </Node>
  </PlexilPlan>

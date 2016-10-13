<?xml version="1.0" encoding="UTF-8"?>
<PlexilPlan xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
  <Node NodeType="NodeList" epx="Concurrence">
    <NodeId>TestSequence</NodeId>
    <NodeBody>
      <NodeList>
        <Node NodeType="NodeList" epx="Sequence">
          <NodeId>Recv1</NodeId>
          <VariableDeclarations>
            <DeclareVariable>
              <Name>arg1</Name>
              <Type>String</Type>
            </DeclareVariable>
            <DeclareVariable>
              <Name>arg2</Name>
              <Type>Boolean</Type>
            </DeclareVariable>
            <DeclareVariable>
              <Name>arg3</Name>
              <Type>Integer</Type>
            </DeclareVariable>
            <DeclareVariable>
              <Name>arg4</Name>
              <Type>Real</Type>
            </DeclareVariable>
            <DeclareVariable>
              <Name>ep2cp_hdl</Name>
              <Type>String</Type>
            </DeclareVariable>
          </VariableDeclarations>
          <InvariantCondition>
            <NOT>
              <OR>
                <AND>
                  <EQInternal>
                    <NodeOutcomeVariable>
                      <NodeRef dir="child">ep2cp_CmdWait</NodeRef>
                    </NodeOutcomeVariable>
                    <NodeOutcomeValue>FAILURE</NodeOutcomeValue>
                  </EQInternal>
                  <EQInternal>
                    <NodeStateVariable>
                      <NodeRef dir="child">ep2cp_CmdWait</NodeRef>
                    </NodeStateVariable>
                    <NodeStateValue>FINISHED</NodeStateValue>
                  </EQInternal>
                </AND>
                <AND>
                  <EQInternal>
                    <NodeOutcomeVariable>
                      <NodeRef dir="child">ep2cp_CmdGetParam_arg1</NodeRef>
                    </NodeOutcomeVariable>
                    <NodeOutcomeValue>FAILURE</NodeOutcomeValue>
                  </EQInternal>
                  <EQInternal>
                    <NodeStateVariable>
                      <NodeRef dir="child">ep2cp_CmdGetParam_arg1</NodeRef>
                    </NodeStateVariable>
                    <NodeStateValue>FINISHED</NodeStateValue>
                  </EQInternal>
                </AND>
                <AND>
                  <EQInternal>
                    <NodeOutcomeVariable>
                      <NodeRef dir="child">ep2cp_CmdGetParam_arg2</NodeRef>
                    </NodeOutcomeVariable>
                    <NodeOutcomeValue>FAILURE</NodeOutcomeValue>
                  </EQInternal>
                  <EQInternal>
                    <NodeStateVariable>
                      <NodeRef dir="child">ep2cp_CmdGetParam_arg2</NodeRef>
                    </NodeStateVariable>
                    <NodeStateValue>FINISHED</NodeStateValue>
                  </EQInternal>
                </AND>
                <AND>
                  <EQInternal>
                    <NodeOutcomeVariable>
                      <NodeRef dir="child">ep2cp_CmdGetParam_arg3</NodeRef>
                    </NodeOutcomeVariable>
                    <NodeOutcomeValue>FAILURE</NodeOutcomeValue>
                  </EQInternal>
                  <EQInternal>
                    <NodeStateVariable>
                      <NodeRef dir="child">ep2cp_CmdGetParam_arg3</NodeRef>
                    </NodeStateVariable>
                    <NodeStateValue>FINISHED</NodeStateValue>
                  </EQInternal>
                </AND>
                <AND>
                  <EQInternal>
                    <NodeOutcomeVariable>
                      <NodeRef dir="child">ep2cp_CmdGetParam_arg4</NodeRef>
                    </NodeOutcomeVariable>
                    <NodeOutcomeValue>FAILURE</NodeOutcomeValue>
                  </EQInternal>
                  <EQInternal>
                    <NodeStateVariable>
                      <NodeRef dir="child">ep2cp_CmdGetParam_arg4</NodeRef>
                    </NodeStateVariable>
                    <NodeStateValue>FINISHED</NodeStateValue>
                  </EQInternal>
                </AND>
                <AND>
                  <EQInternal>
                    <NodeOutcomeVariable>
                      <NodeRef dir="child">ep2cp_CmdAction_recv_test</NodeRef>
                    </NodeOutcomeVariable>
                    <NodeOutcomeValue>FAILURE</NodeOutcomeValue>
                  </EQInternal>
                  <EQInternal>
                    <NodeStateVariable>
                      <NodeRef dir="child">ep2cp_CmdAction_recv_test</NodeRef>
                    </NodeStateVariable>
                    <NodeStateValue>FINISHED</NodeStateValue>
                  </EQInternal>
                </AND>
                <AND>
                  <EQInternal>
                    <NodeOutcomeVariable>
                      <NodeRef dir="child">ep2cp_CmdReturn</NodeRef>
                    </NodeOutcomeVariable>
                    <NodeOutcomeValue>FAILURE</NodeOutcomeValue>
                  </EQInternal>
                  <EQInternal>
                    <NodeStateVariable>
                      <NodeRef dir="child">ep2cp_CmdReturn</NodeRef>
                    </NodeStateVariable>
                    <NodeStateValue>FINISHED</NodeStateValue>
                  </EQInternal>
                </AND>
              </OR>
            </NOT>
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
                      <StringValue>recv_test</StringValue>
                    </Arguments>
                  </Command>
                </NodeBody>
              </Node>
              <Node NodeType="Command">
                <NodeId>ep2cp_CmdGetParam_arg1</NodeId>
                <StartCondition>
                  <EQInternal>
                    <NodeStateVariable>
                      <NodeRef dir="sibling">ep2cp_CmdWait</NodeRef>
                    </NodeStateVariable>
                    <NodeStateValue>FINISHED</NodeStateValue>
                  </EQInternal>
                </StartCondition>
                <EndCondition>
                  <IsKnown>
                    <StringVariable>arg1</StringVariable>
                  </IsKnown>
                </EndCondition>
                <NodeBody>
                  <Command>
                    <StringVariable>arg1</StringVariable>
                    <Name>
                      <StringValue>GetParameter</StringValue>
                    </Name>
                    <Arguments>
                      <StringVariable>ep2cp_hdl</StringVariable>
                      <IntegerValue>0</IntegerValue>
                    </Arguments>
                  </Command>
                </NodeBody>
              </Node>
              <Node NodeType="Command">
                <NodeId>ep2cp_CmdGetParam_arg2</NodeId>
                <StartCondition>
                  <EQInternal>
                    <NodeStateVariable>
                      <NodeRef dir="sibling">ep2cp_CmdGetParam_arg1</NodeRef>
                    </NodeStateVariable>
                    <NodeStateValue>FINISHED</NodeStateValue>
                  </EQInternal>
                </StartCondition>
                <EndCondition>
                  <IsKnown>
                    <BooleanVariable>arg2</BooleanVariable>
                  </IsKnown>
                </EndCondition>
                <NodeBody>
                  <Command>
                    <BooleanVariable>arg2</BooleanVariable>
                    <Name>
                      <StringValue>GetParameter</StringValue>
                    </Name>
                    <Arguments>
                      <StringVariable>ep2cp_hdl</StringVariable>
                      <IntegerValue>1</IntegerValue>
                    </Arguments>
                  </Command>
                </NodeBody>
              </Node>
              <Node NodeType="Command">
                <NodeId>ep2cp_CmdGetParam_arg3</NodeId>
                <StartCondition>
                  <EQInternal>
                    <NodeStateVariable>
                      <NodeRef dir="sibling">ep2cp_CmdGetParam_arg2</NodeRef>
                    </NodeStateVariable>
                    <NodeStateValue>FINISHED</NodeStateValue>
                  </EQInternal>
                </StartCondition>
                <EndCondition>
                  <IsKnown>
                    <IntegerVariable>arg3</IntegerVariable>
                  </IsKnown>
                </EndCondition>
                <NodeBody>
                  <Command>
                    <IntegerVariable>arg3</IntegerVariable>
                    <Name>
                      <StringValue>GetParameter</StringValue>
                    </Name>
                    <Arguments>
                      <StringVariable>ep2cp_hdl</StringVariable>
                      <IntegerValue>2</IntegerValue>
                    </Arguments>
                  </Command>
                </NodeBody>
              </Node>
              <Node NodeType="Command">
                <NodeId>ep2cp_CmdGetParam_arg4</NodeId>
                <StartCondition>
                  <EQInternal>
                    <NodeStateVariable>
                      <NodeRef dir="sibling">ep2cp_CmdGetParam_arg3</NodeRef>
                    </NodeStateVariable>
                    <NodeStateValue>FINISHED</NodeStateValue>
                  </EQInternal>
                </StartCondition>
                <EndCondition>
                  <IsKnown>
                    <RealVariable>arg4</RealVariable>
                  </IsKnown>
                </EndCondition>
                <NodeBody>
                  <Command>
                    <RealVariable>arg4</RealVariable>
                    <Name>
                      <StringValue>GetParameter</StringValue>
                    </Name>
                    <Arguments>
                      <StringVariable>ep2cp_hdl</StringVariable>
                      <IntegerValue>3</IntegerValue>
                    </Arguments>
                  </Command>
                </NodeBody>
              </Node>
              <Node NodeType="NodeList">
                <NodeId>ep2cp_CmdAction_recv_test</NodeId>
                <StartCondition>
                  <EQInternal>
                    <NodeStateVariable>
                      <NodeRef dir="sibling">ep2cp_CmdGetParam_arg4</NodeRef>
                    </NodeStateVariable>
                    <NodeStateValue>FINISHED</NodeStateValue>
                  </EQInternal>
                </StartCondition>
                <NodeBody>
                  <NodeList>
                    <Node NodeType="Command">
                      <NodeId>Recv1__CHILD__1</NodeId>
                      <NodeBody>
                        <Command>
                          <Name>
                            <StringValue>pprint</StringValue>
                          </Name>
                          <Arguments>
                            <StringValue>Received</StringValue>
                            <StringVariable>arg1</StringVariable>
                            <BooleanVariable>arg2</BooleanVariable>
                            <IntegerVariable>arg3</IntegerVariable>
                            <RealVariable>arg4</RealVariable>
                          </Arguments>
                        </Command>
                      </NodeBody>
                    </Node>
                  </NodeList>
                </NodeBody>
              </Node>
              <Node NodeType="Command">
                <NodeId>ep2cp_CmdReturn</NodeId>
                <StartCondition>
                  <EQInternal>
                    <NodeStateVariable>
                      <NodeRef dir="sibling">ep2cp_CmdAction_recv_test</NodeRef>
                    </NodeStateVariable>
                    <NodeStateValue>FINISHED</NodeStateValue>
                  </EQInternal>
                </StartCondition>
                <NodeBody>
                  <Command>
                    <Name>
                      <StringValue>SendReturnValue</StringValue>
                    </Name>
                    <Arguments>
                      <StringVariable>ep2cp_hdl</StringVariable>
                      <BooleanValue>true</BooleanValue>
                    </Arguments>
                  </Command>
                </NodeBody>
              </Node>
            </NodeList>
          </NodeBody>
        </Node>
        <Node NodeType="NodeList" epx="Sequence">
          <NodeId>Recv2</NodeId>
          <VariableDeclarations>
            <DeclareVariable>
              <Name>arg1</Name>
              <Type>String</Type>
            </DeclareVariable>
            <DeclareVariable>
              <Name>arg2</Name>
              <Type>Boolean</Type>
            </DeclareVariable>
            <DeclareVariable>
              <Name>arg3</Name>
              <Type>Integer</Type>
            </DeclareVariable>
            <DeclareVariable>
              <Name>arg4</Name>
              <Type>Real</Type>
            </DeclareVariable>
            <DeclareVariable>
              <Name>ep2cp_hdl</Name>
              <Type>String</Type>
            </DeclareVariable>
          </VariableDeclarations>
          <SkipCondition>
            <BooleanValue>true</BooleanValue>
          </SkipCondition>
          <InvariantCondition>
            <NOT>
              <OR>
                <AND>
                  <EQInternal>
                    <NodeOutcomeVariable>
                      <NodeRef dir="child">ep2cp_CmdWait</NodeRef>
                    </NodeOutcomeVariable>
                    <NodeOutcomeValue>FAILURE</NodeOutcomeValue>
                  </EQInternal>
                  <EQInternal>
                    <NodeStateVariable>
                      <NodeRef dir="child">ep2cp_CmdWait</NodeRef>
                    </NodeStateVariable>
                    <NodeStateValue>FINISHED</NodeStateValue>
                  </EQInternal>
                </AND>
                <AND>
                  <EQInternal>
                    <NodeOutcomeVariable>
                      <NodeRef dir="child">ep2cp_CmdGetParam_arg1</NodeRef>
                    </NodeOutcomeVariable>
                    <NodeOutcomeValue>FAILURE</NodeOutcomeValue>
                  </EQInternal>
                  <EQInternal>
                    <NodeStateVariable>
                      <NodeRef dir="child">ep2cp_CmdGetParam_arg1</NodeRef>
                    </NodeStateVariable>
                    <NodeStateValue>FINISHED</NodeStateValue>
                  </EQInternal>
                </AND>
                <AND>
                  <EQInternal>
                    <NodeOutcomeVariable>
                      <NodeRef dir="child">ep2cp_CmdGetParam_arg2</NodeRef>
                    </NodeOutcomeVariable>
                    <NodeOutcomeValue>FAILURE</NodeOutcomeValue>
                  </EQInternal>
                  <EQInternal>
                    <NodeStateVariable>
                      <NodeRef dir="child">ep2cp_CmdGetParam_arg2</NodeRef>
                    </NodeStateVariable>
                    <NodeStateValue>FINISHED</NodeStateValue>
                  </EQInternal>
                </AND>
                <AND>
                  <EQInternal>
                    <NodeOutcomeVariable>
                      <NodeRef dir="child">ep2cp_CmdGetParam_arg3</NodeRef>
                    </NodeOutcomeVariable>
                    <NodeOutcomeValue>FAILURE</NodeOutcomeValue>
                  </EQInternal>
                  <EQInternal>
                    <NodeStateVariable>
                      <NodeRef dir="child">ep2cp_CmdGetParam_arg3</NodeRef>
                    </NodeStateVariable>
                    <NodeStateValue>FINISHED</NodeStateValue>
                  </EQInternal>
                </AND>
                <AND>
                  <EQInternal>
                    <NodeOutcomeVariable>
                      <NodeRef dir="child">ep2cp_CmdGetParam_arg4</NodeRef>
                    </NodeOutcomeVariable>
                    <NodeOutcomeValue>FAILURE</NodeOutcomeValue>
                  </EQInternal>
                  <EQInternal>
                    <NodeStateVariable>
                      <NodeRef dir="child">ep2cp_CmdGetParam_arg4</NodeRef>
                    </NodeStateVariable>
                    <NodeStateValue>FINISHED</NodeStateValue>
                  </EQInternal>
                </AND>
                <AND>
                  <EQInternal>
                    <NodeOutcomeVariable>
                      <NodeRef dir="child">ep2cp_CmdAction_recv_test</NodeRef>
                    </NodeOutcomeVariable>
                    <NodeOutcomeValue>FAILURE</NodeOutcomeValue>
                  </EQInternal>
                  <EQInternal>
                    <NodeStateVariable>
                      <NodeRef dir="child">ep2cp_CmdAction_recv_test</NodeRef>
                    </NodeStateVariable>
                    <NodeStateValue>FINISHED</NodeStateValue>
                  </EQInternal>
                </AND>
                <AND>
                  <EQInternal>
                    <NodeOutcomeVariable>
                      <NodeRef dir="child">ep2cp_CmdReturn</NodeRef>
                    </NodeOutcomeVariable>
                    <NodeOutcomeValue>FAILURE</NodeOutcomeValue>
                  </EQInternal>
                  <EQInternal>
                    <NodeStateVariable>
                      <NodeRef dir="child">ep2cp_CmdReturn</NodeRef>
                    </NodeStateVariable>
                    <NodeStateValue>FINISHED</NodeStateValue>
                  </EQInternal>
                </AND>
              </OR>
            </NOT>
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
                      <StringValue>recv_test</StringValue>
                    </Arguments>
                  </Command>
                </NodeBody>
              </Node>
              <Node NodeType="Command">
                <NodeId>ep2cp_CmdGetParam_arg1</NodeId>
                <StartCondition>
                  <EQInternal>
                    <NodeStateVariable>
                      <NodeRef dir="sibling">ep2cp_CmdWait</NodeRef>
                    </NodeStateVariable>
                    <NodeStateValue>FINISHED</NodeStateValue>
                  </EQInternal>
                </StartCondition>
                <EndCondition>
                  <IsKnown>
                    <StringVariable>arg1</StringVariable>
                  </IsKnown>
                </EndCondition>
                <NodeBody>
                  <Command>
                    <StringVariable>arg1</StringVariable>
                    <Name>
                      <StringValue>GetParameter</StringValue>
                    </Name>
                    <Arguments>
                      <StringVariable>ep2cp_hdl</StringVariable>
                      <IntegerValue>0</IntegerValue>
                    </Arguments>
                  </Command>
                </NodeBody>
              </Node>
              <Node NodeType="Command">
                <NodeId>ep2cp_CmdGetParam_arg2</NodeId>
                <StartCondition>
                  <EQInternal>
                    <NodeStateVariable>
                      <NodeRef dir="sibling">ep2cp_CmdGetParam_arg1</NodeRef>
                    </NodeStateVariable>
                    <NodeStateValue>FINISHED</NodeStateValue>
                  </EQInternal>
                </StartCondition>
                <EndCondition>
                  <IsKnown>
                    <BooleanVariable>arg2</BooleanVariable>
                  </IsKnown>
                </EndCondition>
                <NodeBody>
                  <Command>
                    <BooleanVariable>arg2</BooleanVariable>
                    <Name>
                      <StringValue>GetParameter</StringValue>
                    </Name>
                    <Arguments>
                      <StringVariable>ep2cp_hdl</StringVariable>
                      <IntegerValue>1</IntegerValue>
                    </Arguments>
                  </Command>
                </NodeBody>
              </Node>
              <Node NodeType="Command">
                <NodeId>ep2cp_CmdGetParam_arg3</NodeId>
                <StartCondition>
                  <EQInternal>
                    <NodeStateVariable>
                      <NodeRef dir="sibling">ep2cp_CmdGetParam_arg2</NodeRef>
                    </NodeStateVariable>
                    <NodeStateValue>FINISHED</NodeStateValue>
                  </EQInternal>
                </StartCondition>
                <EndCondition>
                  <IsKnown>
                    <IntegerVariable>arg3</IntegerVariable>
                  </IsKnown>
                </EndCondition>
                <NodeBody>
                  <Command>
                    <IntegerVariable>arg3</IntegerVariable>
                    <Name>
                      <StringValue>GetParameter</StringValue>
                    </Name>
                    <Arguments>
                      <StringVariable>ep2cp_hdl</StringVariable>
                      <IntegerValue>2</IntegerValue>
                    </Arguments>
                  </Command>
                </NodeBody>
              </Node>
              <Node NodeType="Command">
                <NodeId>ep2cp_CmdGetParam_arg4</NodeId>
                <StartCondition>
                  <EQInternal>
                    <NodeStateVariable>
                      <NodeRef dir="sibling">ep2cp_CmdGetParam_arg3</NodeRef>
                    </NodeStateVariable>
                    <NodeStateValue>FINISHED</NodeStateValue>
                  </EQInternal>
                </StartCondition>
                <EndCondition>
                  <IsKnown>
                    <RealVariable>arg4</RealVariable>
                  </IsKnown>
                </EndCondition>
                <NodeBody>
                  <Command>
                    <RealVariable>arg4</RealVariable>
                    <Name>
                      <StringValue>GetParameter</StringValue>
                    </Name>
                    <Arguments>
                      <StringVariable>ep2cp_hdl</StringVariable>
                      <IntegerValue>3</IntegerValue>
                    </Arguments>
                  </Command>
                </NodeBody>
              </Node>
              <Node NodeType="NodeList">
                <NodeId>ep2cp_CmdAction_recv_test</NodeId>
                <StartCondition>
                  <EQInternal>
                    <NodeStateVariable>
                      <NodeRef dir="sibling">ep2cp_CmdGetParam_arg4</NodeRef>
                    </NodeStateVariable>
                    <NodeStateValue>FINISHED</NodeStateValue>
                  </EQInternal>
                </StartCondition>
                <NodeBody>
                  <NodeList>
                    <Node NodeType="Command">
                      <NodeId>Recv2__CHILD__1</NodeId>
                      <NodeBody>
                        <Command>
                          <Name>
                            <StringValue>pprint</StringValue>
                          </Name>
                          <Arguments>
                            <StringValue>Received</StringValue>
                            <StringVariable>arg1</StringVariable>
                            <BooleanVariable>arg2</BooleanVariable>
                            <IntegerVariable>arg3</IntegerVariable>
                            <RealVariable>arg4</RealVariable>
                          </Arguments>
                        </Command>
                      </NodeBody>
                    </Node>
                  </NodeList>
                </NodeBody>
              </Node>
              <Node NodeType="Command">
                <NodeId>ep2cp_CmdReturn</NodeId>
                <StartCondition>
                  <EQInternal>
                    <NodeStateVariable>
                      <NodeRef dir="sibling">ep2cp_CmdAction_recv_test</NodeRef>
                    </NodeStateVariable>
                    <NodeStateValue>FINISHED</NodeStateValue>
                  </EQInternal>
                </StartCondition>
                <NodeBody>
                  <Command>
                    <Name>
                      <StringValue>SendReturnValue</StringValue>
                    </Name>
                    <Arguments>
                      <StringVariable>ep2cp_hdl</StringVariable>
                      <BooleanValue>true</BooleanValue>
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
</PlexilPlan>

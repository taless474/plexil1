  <PlexilPlan xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="http://plexil.svn.sourceforge.net/viewvc/plexil/trunk/schema/core-plexil.xsd" FileName="lib1.ple">
      <GlobalDeclarations/>
      <Node FileName="lib1.ple" LineNo="5" ColNo="1" NodeType="NodeList">
          <Interface>
              <In>
                  <DeclareVariable>
                      <Name>lb</Name>
                      <Type>Boolean</Type>
                  </DeclareVariable>
                  <DeclareVariable>
                      <Name>li</Name>
                      <Type>Integer</Type>
                  </DeclareVariable>
                  <DeclareVariable>
                      <Name>lr</Name>
                      <Type>Real</Type>
                  </DeclareVariable>
                  <DeclareVariable>
                      <Name>ls</Name>
                      <Type>String</Type>
                  </DeclareVariable>
              </In>
          </Interface>
          <NodeId>LibTest1</NodeId>
          <NodeBody>
              <NodeList>
                  <Node FileName="lib1.ple" LineNo="17" ColNo="13" NodeType="Command">
                      <NodeId>FooCall1</NodeId>
                      <NodeBody>
                          <Command>
                              <Name>
                                  <StringValue>foo</StringValue>
                              </Name>
                              <Arguments>
                                  <BooleanVariable>lb</BooleanVariable>
                                  <IntegerVariable>li</IntegerVariable>
                                  <RealVariable>lr</RealVariable>
                                  <StringVariable>ls</StringVariable>
                              </Arguments>
                          </Command>
                      </NodeBody>
                  </Node>
              </NodeList>
          </NodeBody>
      </Node>
  </PlexilPlan>

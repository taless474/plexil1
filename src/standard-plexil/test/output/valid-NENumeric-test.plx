  <PlexilPlan xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="http://plexil.svn.sourceforge.net/viewvc/plexil/trunk/schema/supported-plexil.xsd" FileName="NENumeric-test.ple">
      <Node FileName="NENumeric-test.ple" LineNo="2" ColNo="1" NodeType="Empty">
          <VariableDeclarations>
              <DeclareVariable>
                  <Name>x</Name>
                  <Type>Real</Type>
              </DeclareVariable>
          </VariableDeclarations>
          <NodeId>testNode</NodeId>
          <StartCondition>
              <NENumeric>
                  <RealVariable>x</RealVariable>
                  <RealValue>10.0</RealValue>
              </NENumeric>
          </StartCondition>
      </Node>
  </PlexilPlan>

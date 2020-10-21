// Copyright (c) 2006-2020, Universities Space Research Association (USRA).
//  All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//    * Neither the name of the Universities Space Research Association nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY USRA ``AS IS'' AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL USRA BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
// OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
// TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
// USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

package plexil;

import java.util.Set;
import java.util.TreeSet;

import org.antlr.runtime.*;
import org.antlr.runtime.tree.*;

import org.w3c.dom.Element;

public class DoNode extends PlexilTreeNode
{
    private NodeContext m_bodyContext = null;

    public DoNode(Token t)
    {
        super(t);
    }

    public DoNode(DoNode n)
    {
        super(n);
		m_bodyContext = n.m_bodyContext;
    }

	public Tree dupNode()
	{
		return new DoNode(this);
	}

    //
    // N.B. The extra complexity in the checking logic is to ensure the body
    // is contained in a separate name binding context from the do loop
    // as a whole.
    //

    /**
     * @brief Prepare for the semantic check.
     */
    public void earlyCheck(NodeContext parentContext, CompilerState state)
    {
        earlyCheckSelf(parentContext, state);
        this.getChild(0).earlyCheck(m_bodyContext, state); // body
        this.getChild(1).earlyCheck(parentContext, state); // do-test expression
    }

    public void earlyCheckSelf(NodeContext parentContext, CompilerState state)
    {
        // See if we have a node ID
        String nodeId = null;
        PlexilTreeNode parent = this.getParent();
        if (parent != null && parent instanceof ActionNode) {
            nodeId = ((ActionNode) parent).getNodeId();
        }
        else {
            // should never happen
            state.addDiagnostic(this,
                                "Internal error: DoNode instance has no parent ActionNode",
                                Severity.FATAL);
        }
        // Construct body binding context
        m_bodyContext = new NodeContext(parentContext, nodeId + "_DO_BODY");
    }

    /**
     * @brief Semantic check.
     * @note Uses separate context for body.
     */
    public void check(NodeContext parentContext, CompilerState state)
    {
        checkSelf(parentContext, state);
        this.getChild(0).check(m_bodyContext, state); // body
        this.getChild(1).check(parentContext, state); // do test
    }

    public void checkSelf(NodeContext context, CompilerState state)
    {
        ExpressionNode doTest = (ExpressionNode) this.getChild(1);
        if (!doTest.assumeType(PlexilDataType.BOOLEAN_TYPE, state)) {
            state.addDiagnostic(doTest,
                                "\"do\" test expression is not Boolean",
                                Severity.ERROR);
        }
    }

    protected void constructXML()
    {
        super.constructXML();

        Element action = CompilerState.newElement("Action");
        m_xml.appendChild(action);
        action.appendChild(this.getChild(0).getXML());

        Element condition = CompilerState.newElement("Condition");
        m_xml.appendChild(condition);
        condition.appendChild(this.getChild(1).getXML());
        condition.setAttribute("LineNo", String.valueOf(this.getChild(1).getLine()));
        condition.setAttribute("ColNo", String.valueOf(this.getChild(1).getCharPositionInLine()));
    }

    protected String getXMLElementName() { return "Do"; }

}

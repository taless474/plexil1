// Copyright (c) 2006-2011, Universities Space Research Association (USRA).
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

import org.antlr.runtime.*;
import org.antlr.runtime.tree.*;

import net.n3.nanoxml.*;

public class OnCommandNode extends PlexilTreeNode
{
    public OnCommandNode(Token t)
    {
        super(t);
    }

    public void checkSelf(NodeContext context, CompilerState state)
    {
        /*  This causes crash.  Not sure what to really check here.
        ExpressionNode nameExp = (ExpressionNode) this.getChild(0);
        if (!nameExp.assumeType(PlexilDataType.STRING_TYPE, state)) {
            state.addDiagnostic
            (nameExp,
            "The name expression to the " + this.getToken().getText()
            + " statement was not a string expression",
            Severity.ERROR);
            }
        */
    }

    public void constructXML()
    {
        super.constructXML();

        // Construct the name element, but don't output it yet.  Only literal
        // names are supported at present.
        //
        IXMLElement name = new XMLElement ("Name");
        IXMLElement stringVal = new XMLElement ("StringValue");
        stringVal.setContent(this.getChild (0).getText());
        name.addChild (stringVal);

        // Second child: parameters (optional), or action
        PlexilTreeNode second = this.getChild (1);


        if (second.getType() == PlexilLexer.PARAMETERS) {
            // Parameters are output first, followed by name
            m_xml.addChild (second.getXML());
            m_xml.addChild (name);            
            // And we now know the third child is the action
            m_xml.addChild (this.getChild(2).getXML());
        } else {
            // the second AST child was the action, which follows name
            m_xml.addChild (name);
            m_xml.addChild (second.getXML());
        }
    }
}


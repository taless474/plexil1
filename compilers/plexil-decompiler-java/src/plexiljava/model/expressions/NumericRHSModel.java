package plexiljava.model.expressions;

import plexiljava.decompilation.DecompilableStringBuilder;
import plexiljava.model.BaseModel;
import plexiljava.model.NodeModel;
import plexiljava.model.functions.FunctionModel;
import plexiljava.model.lookups.LookupModel;
import plexiljava.model.operators.OperatorModel;

public class NumericRHSModel extends NodeModel {

	public NumericRHSModel(BaseModel node) {
		super(node);
	}

	@Override
	public boolean verify() {
		return hasChild(OperatorModel.class) || hasChild(LookupModel.class) || hasChild(ArrayElementModel.class) || !qualities.isEmpty();
	}
	
	@Override
	public String translate(int indentLevel) throws PatternRecognitionFailureException {
		DecompilableStringBuilder dsb = new DecompilableStringBuilder();
		dsb.addIndent(indentLevel);
		
		if( hasChild(OperatorModel.class) ) {
			dsb.append(getChild(OperatorModel.class).decompile(0));
		} else if( hasChild(LookupModel.class) ) {
			dsb.append(getChild(LookupModel.class).decompile(0));
		} else if( hasChild(ArrayElementModel.class) ) {
			dsb.append(getChild(ArrayElementModel.class).decompile(0));
		} else if( hasChild(FunctionModel.class) ) {
			dsb.append(getChild(FunctionModel.class).decompile(0));
		} else {
			dsb.append(qualities.get(0).getValue());
		}
		
		return dsb.toString();
	}
	
}

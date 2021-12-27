package fin.parser;

import org.antlr.v4.runtime.tree.ParseTree;
import org.antlr.v4.runtime.tree.ParseTreeProperty;

public interface TreeAnnotator {
	
    ParseTreeProperty<Object> getValues();
	
    default void setValue(ParseTree node, Object value) {
        getValues().put(node, value);
    }

    default Object getValue(ParseTree node) {
        return getValues().get(node);
    }

    default <T extends Object> T getValue(ParseTree node, Class<T> type) {
        if (node == null) return null;
        Object e = getValues().get(node);
        return e != null && type.isAssignableFrom(e.getClass()) ? type.cast(e) : null;
    }
}

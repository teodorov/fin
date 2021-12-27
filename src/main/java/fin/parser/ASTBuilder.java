package fin.parser;

import fin.grammar.FinBaseListener;
import fin.grammar.FinParser;
import fin.model.*;
import org.antlr.v4.runtime.tree.ParseTreeProperty;
import org.antlr.v4.runtime.tree.TerminalNode;

import java.util.ArrayList;
import java.util.Collection;

public class ASTBuilder extends FinBaseListener implements TreeAnnotator {
    public ParseTreeProperty<Object> values = new ParseTreeProperty<>();
    public ParseTreeProperty<Object> getValues() {
        return values;
    }

    FinFactory factory = FinFactory.eINSTANCE;

    @Override
    public void exitIdentifierList(FinParser.IdentifierListContext ctx) {
        Collection<String> node = new ArrayList<>();
        for (TerminalNode t : ctx.IDENTIFIER()) {
            node.add(t.getText());
        }
        setValue(ctx, node);
    }

    @SuppressWarnings("unchecked")
    @Override
    public void exitAgentDeclaration(FinParser.AgentDeclarationContext ctx) {
        AgentDeclaration node = factory.createAgentDeclaration();

        node.setName(ctx.IDENTIFIER().getText());

        node.getPorts().addAll(getValue(ctx.identifierList(), Collection.class));

        setValue(ctx, node);
    }

    @Override
    public void exitAgentDeclarationList(FinParser.AgentDeclarationListContext ctx) {
        Collection<AgentDeclaration> node = new ArrayList<>();

        for (FinParser.AgentDeclarationContext item : ctx.agentDeclaration()) {
            node.add(getValue(item, AgentDeclaration.class));
        }

        setValue(ctx, node);
    }

    @Override
    public void exitName(FinParser.NameContext ctx) {
        Name node = factory.createName();
        node.setName(ctx.IDENTIFIER().getText());
        setValue(ctx, node);
    }

    @Override
    public void exitAgentInstance(FinParser.AgentInstanceContext ctx) {
        AgentInstance node = factory.createAgentInstance();
        node.setName(ctx.IDENTIFIER().getText());

        for (FinParser.NetContext item : ctx.net()) {
            node.getConnections().add(getValue(item, Net.class));
        }
        setValue(ctx, node);
    }

    @Override
    public void exitEquation(FinParser.EquationContext ctx) {
        Equation node = factory.createEquation();
        node.setLhs(getValue(ctx.net(0), Net.class));
        node.setRhs(getValue(ctx.net(1), Net.class));
        setValue(ctx, node);
    }

    @Override
    public void exitNetList(FinParser.NetListContext ctx) {
        NetList node = factory.createNetList();
        for (FinParser.NetContext item : ctx.net()) {
            node.getComponents().add(getValue(item, Net.class));
        }
        setValue(ctx, node);
    }

    @Override
    public void exitActivePair(FinParser.ActivePairContext ctx) {
        ActivePair node = factory.createActivePair();
        node.setLhs(getValue(ctx.net(0), Net.class));
        node.setRhs(getValue(ctx.net(1), Net.class));
        setValue(ctx, node);
    }

    @Override
    public void exitRuleDeclaration(FinParser.RuleDeclarationContext ctx) {
        Rule node = factory.createRule();
        node.setActivePair(getValue(ctx.activePair(), ActivePair.class));
        node.setTarget(getValue(ctx.net(), Net.class));
        setValue(ctx, node);
    }

    @Override
    public void exitRuleDeclarationList(FinParser.RuleDeclarationListContext ctx) {
        Collection<Rule> node = new ArrayList<>();

        for (FinParser.RuleDeclarationContext item : ctx.ruleDeclaration()) {
            node.add(getValue(item, Rule.class));
        }
        setValue(ctx, node);
    }

    @SuppressWarnings("unchecked")
    @Override
    public void exitInteractionNet(FinParser.InteractionNetContext ctx) {
        NetDeclaration node = factory.createNetDeclaration();
        if (ctx.agentDeclarationList() != null) {
            node.getAgents().addAll(getValue(ctx.agentDeclarationList(), Collection.class));
        }
        if (ctx.ruleDeclarationList() != null) {
            node.getRules().addAll(getValue(ctx.ruleDeclarationList(), Collection.class));
        }
        node.setNet(getValue(ctx.net(), Net.class));
        setValue(ctx, node);
    }
}

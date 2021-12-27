package fin.parser;


import fin.grammar.FinLexer;
import fin.grammar.FinParser;
import fin.model.NetDeclaration;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.tree.ParseTree;
import org.antlr.v4.runtime.tree.ParseTreeWalker;
import org.eclipse.emf.common.util.URI;
import org.eclipse.emf.ecore.resource.Resource;
import org.eclipse.emf.ecore.resource.ResourceSet;
import org.eclipse.emf.ecore.resource.impl.ResourceSetImpl;
import org.eclipse.emf.ecore.xmi.impl.XMIResourceFactoryImpl;

import java.io.File;
import java.io.IOException;
import java.util.Collections;

public class Parser {
    private static Parser instance = new Parser();

    public static NetDeclaration parse(File file) throws IOException {
        ANTLRFileStream fs = new ANTLRFileStream(file.getAbsolutePath());

        return instance.parse(fs);
    }

    public static NetDeclaration parse(String program) {
        ANTLRInputStream is = new ANTLRInputStream(program);

        return instance.parse(is);
    }

    public static URI generateModelXMI(NetDeclaration exp, String fileName) throws IOException {
        return instance.saveXMI(exp, fileName);
    }


    public NetDeclaration parse(ANTLRInputStream is) {
        FinLexer lexer = new FinLexer(is);
        CommonTokenStream tokens = new CommonTokenStream(lexer);
        FinParser parser = new FinParser(tokens);
        ParseTree tree;

        tree = parser.interactionNet();

        ParseTreeWalker walker = new ParseTreeWalker();

        ASTBuilder builder = new ASTBuilder();

        //TODO define a clear error handling strategy for Parsing
        parser.removeErrorListeners();
        parser.addErrorListener(new ErrorHandler());

        try {
            walker.walk(builder, tree);
            //SymbolResolver resolver = new SymbolResolver(builder.values);
            //walker.walk(resolver, tree);
            return builder.getValue(tree, NetDeclaration.class);//resolver.getValue(tree, type);

        } catch (Error e) {
            java.lang.System.err.println(e.getMessage());
            return null;
        }
    }

    public URI saveXMI(NetDeclaration object, String fileName) throws IOException {
        ResourceSet resourceSet = new ResourceSetImpl();
        resourceSet.getResourceFactoryRegistry().getExtensionToFactoryMap().put("xmi", new XMIResourceFactoryImpl());
        Resource r = resourceSet.createResource(URI.createFileURI(fileName));
        r.getContents().add(object);
        r.save(Collections.emptyMap());
        return r.getURI();
    }

    public static class ErrorHandler extends BaseErrorListener {
        @Override
        public void syntaxError(Recognizer<?, ?> rec, Object offendingSymbol, int line, int column, String msg, RecognitionException e) {
            throw new RuntimeException(e.getLocalizedMessage());
        }
    }
}
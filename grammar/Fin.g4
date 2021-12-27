grammar Fin;

interactionNet : agentDeclarationList? ruleDeclarationList? net;

agentDeclarationList : agentDeclaration (COMMA agentDeclaration)* COMMA?;

agentDeclaration : IDENTIFIER (LPAREN identifierList RPAREN)?;
identifierList : (IDENTIFIER (COMMA IDENTIFIER)*)?;

ruleDeclarationList : ruleDeclaration (SEMICOLON ruleDeclaration)* COMMA?;
ruleDeclaration : activePair (rewritesTo net)?;
rewritesTo : IMPLIES | ASCII_IMPLIES;

activePair : net join net;
join : JOIN | ASCII_JOIN;

net : IDENTIFIER                                        #Name
    | IDENTIFIER (LPAREN (net (COMMA net)*)? RPAREN)?   #AgentInstance
    | net operator=(EQ | TILDE) net                     #Equation
    | net COMMA net                                     #NetList
    ;

IDENTIFIER : [a-zA-Z][a-zA-Z_0-9]*;

EQ : '=';
TILDE : '~';
COMMA : ',';
SEMICOLON : ';';
LPAREN : '(';
RPAREN : ')';
JOIN: '>><<';
ASCII_JOIN: '><';
IMPLIES: '==>>';
ASCII_IMPLIES: '=>';

LINE_COMMENT : '//' .*? '\n' -> skip ;
COMMENT : '/*' .*? '*/' -> skip ;
WS : [ \r\t\n]+ -> skip ;
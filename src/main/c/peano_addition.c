//
// Created by Ciprian TEODOROV on 29/12/2021.
//

#include <stdlib.h>
#include <peano_addition.h>

fin_configuration_t *peano_addition(fin_net_t *net) {
    fin_configuration_t *the_configuration = allocate_configuration(3, 2);

//ADD the agents
    the_configuration->m_agent_declarations[0] = (fin_agent_declaration_t) {"Z", 0};
    the_configuration->m_agent_declarations[1] = (fin_agent_declaration_t) {"S", 1};
    the_configuration->m_agent_declarations[2] = (fin_agent_declaration_t) {"+", 2};

    fin_agent_declaration_t *theZ = &the_configuration->m_agent_declarations[0];
    fin_agent_declaration_t *theS = &the_configuration->m_agent_declarations[1];
    fin_agent_declaration_t *thePlus = &the_configuration->m_agent_declarations[2];

//ADD the rules
//NO RULES YET
    the_configuration->m_rules = NULL;
    the_configuration->m_net = net;

    return the_configuration;
}
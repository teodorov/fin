//
// Created by Ciprian TEODOROV on 28/12/2021.
//

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <allocator.h>

struct fin_agent_declaration_s {
    char *  m_name;     //the name of the agent
    int     m_arity;    //the arity of the agent (number of auxiliary ports)
    char*   m_ports[];  //the names of the auxiliary ports
};
typedef struct fin_agent_declaration_s fin_agent_declaration_t;



/*
 * TODO: we could replace the 64bit pointer towards the agent declaration (m_declaration) by a 32 bit header.
 * */

struct fin_agent_header_s {
    uint16_t    m_declaration_idx;  //the index of the agent in the agent declarations table
    uint8_t     m_arity;            //the number of ports of the agent, storing this here will prevent a pointer fetch
    uint8_t     m_unused;           //here we could hide some implicit agents, like epsilon, immediate values, etc
};
typedef struct fin_agent_header_s fin_agent_header_t;

struct fin_agent_s {
    fin_agent_declaration_t *   m_declaration;
    void *                      m_connections[0]; // a table of m_declaration->m_arity pointers
};
typedef struct fin_agent_s fin_agent_t;

struct fin_name_s {
    char * m_name;
};
typedef struct fin_name_s fin_name_t;

struct fin_term_s {
    char m_is_agent;
    union {
        fin_agent_t  *m_agent;
        fin_name_t   *m_name;
    };
};
typedef struct fin_term_s fin_term_t;

struct fin_equation_s {
    void *m_lhs;
    void *m_rhs;
};
typedef struct fin_equation_s fin_equation_t;

struct fin_net_s {
    fin_term_t     *m_terms;
    fin_equation_t  *m_equations[2];
};
typedef struct fin_net_s fin_net_t;

struct fin_rule_s {
    fin_agent_declaration_t *m_agent_lhs;
    fin_agent_declaration_t *m_agent_rhs;
    fin_net_t m_lhs;
    fin_net_t m_rhs;
};
typedef struct fin_rule_s fin_rule_t;

struct fin_configuration_s {
    fin_agent_declaration_t *   m_agent_declarations;   //all declared agents
    fin_rule_t *                m_rules;                //all rewrite rules
    fin_net_t  *                m_net;                  //the net that plays
};
typedef struct fin_configuration_s fin_configuration_t;

void print_agent_declaration(fin_agent_declaration_t *agent) {
    printf("%s(", agent->m_name);
    char is_first = 1;
    for (int i = 0; i < agent->m_arity; i++) {
        if (is_first) {
            is_first = 0;
        } else {
            printf(", ");
        }
        printf("%s", agent->m_ports[i]);
    }
    printf(")\n");
}

int main() {
    printf("Hello, World!\n");

    fin_agent_declaration_t *a1 = malloc(sizeof(fin_agent_declaration_t) + 2 * sizeof(char *));

    *a1 = (fin_agent_declaration_t){
            .m_name = "add",
            .m_arity = 2
    };
    a1->m_ports[0] = "x1";
    a1->m_ports[1] = "x2";

    print_agent_declaration(a1);

    return a1->m_arity;
}

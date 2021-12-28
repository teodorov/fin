//
// Created by Ciprian TEODOROV on 28/12/2021.
//

#ifndef FAST_INTERACTION_NETS_FIN_CORE_H
#define FAST_INTERACTION_NETS_FIN_CORE_H

#include <stdint.h>

struct fin_agent_declaration_s {
    char *m_name;     //the name of the agent
    uint32_t m_arity;    //the arity of the agent (number of auxiliary ports)
};
typedef struct fin_agent_declaration_s fin_agent_declaration_t;

typedef void *fin_port_t;

struct fin_instance_s {
    fin_agent_declaration_t *m_declaration;
    fin_port_t m_connections[]; // a table of m_declaration->m_arity+1 pointers
};
typedef struct fin_instance_s fin_instance_t;

typedef void *fin_name_instance_t;
struct fin_net_s {
    uint32_t m_names_size;
    uint32_t m_instances_size;
    fin_name_instance_t *m_names;
    fin_instance_t **m_instances;
};
typedef struct fin_net_s fin_net_t;

struct fin_rule_s {
    fin_agent_declaration_t *m_agent_lhs;
    fin_agent_declaration_t *m_agent_rhs;
    fin_net_t *m_lhs;
    fin_net_t *m_rhs;
};
typedef struct fin_rule_s fin_rule_t;

struct fin_configuration_s {
    uint32_t m_declaration_count;
    uint32_t m_rule_count;
    fin_agent_declaration_t *   m_agent_declarations;   //an array with the agent declarations inside
    fin_rule_t *                m_rules;                //an array with the rewrite rules inside
    fin_net_t  *                m_net;                  //a pointer towards the net that plays
};
typedef struct fin_configuration_s fin_configuration_t;

fin_configuration_t *allocate_configuration(uint32_t declaration_count, uint32_t rule_count);
void free_configuration(fin_configuration_t *configuration);

// operation on nets
fin_net_t *allocate_net(uint32_t names_size, uint32_t instances_size);

void free_net(fin_net_t *net);

void print_net(fin_net_t *net);

// operation with names
fin_name_instance_t *get_name(fin_net_t *net, uint32_t name_id);

// operations with agents
//fin_agent_declaration_t *allocate_declaration(char *name, uint32_t arity);
//void free_declaration(fin_agent_declaration_t *agent_declaration);

fin_instance_t *allocate_instance(fin_agent_declaration_t *declaration);

void free_instance(fin_instance_t *instance);

void print_agent_instance(fin_instance_t *instance);

// ports & connections
fin_port_t *get_port(fin_instance_t *instance, uint32_t port_id);

void connect(fin_port_t *port1, fin_port_t *port2);


#endif //FAST_INTERACTION_NETS_FIN_CORE_H

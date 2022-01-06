//
// Created by Ciprian TEODOROV on 28/12/2021.
//

#ifndef FAST_INTERACTION_NETS_FIN_CORE_H
#define FAST_INTERACTION_NETS_FIN_CORE_H

#include <stdint.h>
#include <stdio.h>

struct fin_agent_declaration_s {
    char *m_name;     //the name of the agent
    uint32_t m_arity;    //the arity of the agent (number of auxiliary ports)
};
typedef struct fin_agent_declaration_s fin_agent_declaration_t;

typedef void* fin_port_t;

#define PRINCIPAL 0x01
#define AS_PRINCIPAL(p) ((fin_port_t)(((uintptr_t)(p)) | (PRINCIPAL)))
#define IS_PRINCIPAL(p) ((fin_port_t)(((uintptr_t)(p)) & (PRINCIPAL)))
#define GET_POINTER(p) ((fin_port_t*)(((uintptr_t)(p)) & ~(PRINCIPAL)))
#define PRINCIPAL2INSTANCE(p) ( (fin_instance_t *) (GET_POINTER( (p) ) - 3 ) )

typedef struct fin_instance_s fin_instance_t;
struct fin_instance_s {
    fin_instance_t *m_previous;
    fin_instance_t *m_next;
    fin_agent_declaration_t *m_declaration;
    fin_port_t m_connections[]; // a table of m_declaration->m_arity+1 pointers
};

struct fin_active_pairs_s {
    uint32_t m_capacity;
    fin_instance_t **m_set;
    uint32_t m_sp;
};
typedef struct fin_active_pairs_s fin_active_pairs_t;

typedef void *fin_name_instance_t;
struct fin_net_s {
    uint32_t m_names_size;
    fin_name_instance_t *m_names;
    fin_instance_t *m_instances;
};
typedef struct fin_net_s fin_net_t;

struct fin_rule_s {
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
    fin_active_pairs_t          m_active_pairs;
};
typedef struct fin_configuration_s fin_configuration_t;

fin_configuration_t *allocate_configuration(uint32_t declaration_count, uint32_t rule_count);
void free_configuration(fin_configuration_t *configuration);

// operation on nets
fin_net_t *allocate_net(uint32_t names_size);

void free_net(fin_net_t *net);

void print_net(fin_net_t *net);

void to_dot_net(FILE *file, fin_net_t *net);

fin_instance_t *add_instance(fin_net_t *net, fin_instance_t *instance);
void remove_and_free_instance(fin_net_t *net, fin_instance_t *instance);

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

typedef void (*fin_active_pair_handler_t)(fin_instance_t*, fin_instance_t*, fin_configuration_t *);
void rewrite_active_pair(
        fin_net_t *io_net,
        fin_instance_t *in_first,
        fin_instance_t *in_second,
        fin_rule_t *in_rule,
        fin_active_pair_handler_t in_user_handler,
        fin_configuration_t * opaque);

fin_rule_t *matching_rule(
        fin_configuration_t *in_configuration,
        fin_instance_t *in_first,
        fin_instance_t *in_second);

fin_net_t *reduce(fin_configuration_t *io_configuration);

void add_active_pair(fin_instance_t *a, fin_instance_t *b, fin_configuration_t *io_configuration);

#endif //FAST_INTERACTION_NETS_FIN_CORE_H

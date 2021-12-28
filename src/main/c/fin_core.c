//
// Created by Ciprian TEODOROV on 28/12/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <fin_core.h>

fin_name_instance_t *allocate_names(uint32_t number_of_names) {
    fin_name_instance_t *names = malloc(sizeof(fin_name_instance_t) * number_of_names);
    if (names == NULL) {
        fprintf(stderr, "[%s,%d] Cannot allocate %d names", __FILE__, __LINE__, number_of_names);
        exit(1);
    }
    return names;
}

fin_name_instance_t *get_name(fin_net_t *net, uint32_t name_id) {
    if (net == NULL) {
        fprintf(stderr, "[%s,%d] Cannot retrieve name %d from NULL", __FILE__, __LINE__, name_id);
        exit(1);
    }
    return &net->m_names[name_id];
}

void print_agent_instance(fin_instance_t *instance) {
    if (instance == NULL) {
        fprintf(stdout, "no_instance\n");
        return;
    }
    fprintf(stdout, "%s(%p) [", instance->m_declaration->m_name, instance);
    char is_first = 1;
    for (int i = 0; i < instance->m_declaration->m_arity + 1; i++) {
        if (is_first) {
            is_first = 0;
        } else {
            fprintf(stdout, ", ");
        }
        fprintf(stdout, "%p->%p", &instance->m_connections[i], instance->m_connections[i]);
    }
    fprintf(stdout, "]\n");
}

fin_instance_t *allocate_instance(fin_agent_declaration_t *declaration) {
    if (declaration == NULL) {
        fprintf(stderr, "[%s,%d] Cannot allocate instance for NULL", __FILE__, __LINE__);
        exit(1);
    }
    fin_instance_t *the_instance = malloc(sizeof(fin_instance_t) + sizeof(void *) * declaration->m_arity + 1);
    if (the_instance == NULL) {
        fprintf(stderr, "[%s,%d] Cannot allocate instance for agent %s", __FILE__, __LINE__, declaration->m_name);
        exit(1);
    }
    the_instance->m_declaration = declaration;
    return the_instance;
}

void free_instance(fin_instance_t *instance) {
    if (instance == NULL) return;
    free(instance);
}

fin_port_t *get_port(fin_instance_t *instance, uint32_t port_id) {
    if (instance == NULL) {
        fprintf(stderr, "[%s,%d] Cannot retrieve port %d from NULL", __FILE__, __LINE__, port_id);
        exit(1);
    }
    if (port_id > instance->m_declaration->m_arity) {
        fprintf(stderr, "[%s,%d] Cannot retrieve port %d from an instance of %s of arity %d ", __FILE__, __LINE__,
                port_id, instance->m_declaration->m_name, instance->m_declaration->m_arity);
        exit(1);
    }
    return &instance->m_connections[port_id];
}

void connect(fin_port_t *port1, fin_port_t *port2) {
    if (port1 == NULL || port2 == NULL) {
        fprintf(stderr, "[%s,%d] Cannot connect ports [%p -- %p]", __FILE__, __LINE__, port1, port2);
        exit(1);
    }

    *port1 = port2;
    *port2 = port1;
}

fin_net_t *allocate_net(uint32_t names_size, uint32_t instances_size) {
    fin_net_t *the_net = calloc(1, sizeof(fin_net_t));
    if (the_net == NULL) {
        fprintf(stderr, "[%s,%d] Cannot allocate a net of %d names and %d agents", __FILE__, __LINE__, names_size,
                instances_size);
        exit(1);
    }
    the_net->m_names_size = names_size;
    the_net->m_instances_size = instances_size;
    the_net->m_names = allocate_names(names_size);


    fin_instance_t **instances = malloc(sizeof(fin_name_instance_t) * instances_size);
    if (instances == NULL) {
        fprintf(stderr, "[%s,%d] Cannot allocate %d instances", __FILE__, __LINE__, instances_size);
        exit(1);
    }

    the_net->m_instances = instances;

    return the_net;
}

void free_net(fin_net_t *net) {
    if (net == NULL) {
        return;
    }
    if (net->m_names != NULL) {
        free(net->m_names);
    }
    if (net->m_instances != NULL) {
        for (int i = 0; i < net->m_instances_size; i++) {
            if (net->m_instances[i] == NULL) continue;
            free_instance(net->m_instances[i]);
        }
        free(net->m_instances);
    }

    free(net);
}

void print_net(fin_net_t *net) {
    if (net == NULL) {
        fprintf(stdout, "empty net\n");
        return;
    }
    fprintf(stdout, "{\n\tnames[");
    char is_first = 1;
    for (int i = 0; i < net->m_names_size; i++) {
        if (is_first) {
            is_first = 0;
        } else {
            fprintf(stdout, ", ");
        }
        fprintf(stdout, "%p->%p", &net->m_names[i], net->m_names[i]);
    }
    fprintf(stdout, "]\n");

    for (int i = 0; i < net->m_instances_size; i++) {
        fprintf(stdout, "\t");
        print_agent_instance(net->m_instances[i]);
    }
    fprintf(stdout, "}\n");
}

//fin_agent_declaration_t *allocate_declaration(char *name, uint32_t arity) {
//    fin_agent_declaration_t *agent = malloc(sizeof(fin_agent_declaration_t));
//    if (agent == NULL) {
//        fprintf(stderr, "[%s,%d] Cannot allocate agent_declaration for %s(%d)", __FILE__, __LINE__, name, arity);
//        exit(1);
//    }
//    agent->m_name = name;
//    agent->m_arity = arity;
//    return agent;
//}
//
//void free_declaration(fin_agent_declaration_t *agent_declaration) {
//    if (agent_declaration == NULL) return;
//    free(agent_declaration);
//}

fin_rule_t *allocate_rule() {
    //TODO
    return NULL;
}
void free_rule(fin_rule_t *rule) {
    if (rule == NULL) return;
    //TODO
}

fin_configuration_t *allocate_configuration(uint32_t declaration_count, uint32_t rule_count) {
    fin_configuration_t *the_configuration = malloc(sizeof(fin_configuration_t));
    if (the_configuration == NULL) {
        fprintf(stderr, "[%s,%d] Cannot allocate a configuration with %d agent types and %d rules", __FILE__, __LINE__, declaration_count, rule_count);
        exit(1);
    }

    the_configuration->m_declaration_count = declaration_count;
    the_configuration->m_rule_count = rule_count;
    the_configuration->m_agent_declarations = malloc(sizeof(fin_agent_declaration_t) * declaration_count);
    if (the_configuration->m_agent_declarations == NULL) {
        fprintf(stderr, "[%s,%d] Cannot allocate a configuration with %d agent types and %d rules", __FILE__, __LINE__, declaration_count, rule_count);
        exit(1);
    }
    the_configuration->m_rules = malloc(sizeof(fin_rule_t) * rule_count);
    if (the_configuration->m_rules == NULL) {
        fprintf(stderr, "[%s,%d] Cannot allocate a configuration with %d agent types and %d rules", __FILE__, __LINE__, declaration_count, rule_count);
        exit(1);
    }
    return the_configuration;
}

void free_configuration(fin_configuration_t *configuration) {
    if (configuration == NULL) return;
    if (configuration->m_agent_declarations != NULL) {
        free(configuration->m_agent_declarations);
    }
    if (configuration->m_rules != NULL) {
        free(configuration->m_rules);
    }
    free(configuration);
}





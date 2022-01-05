//
// Created by Ciprian TEODOROV on 28/12/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <fin_core.h>
#include <assert.h>

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
    fprintf(stdout, "] %p<-->%p\n", instance->m_previous, instance->m_next);
}

void to_dot_agent_instance(FILE *file, fin_instance_t *instance) {
    fprintf(file, "\tsubgraph cluster%p { style=filled; color=lightgray; label=\"%s\"; \n\t\tp%p [shape=point, label=\"%d\"]\n",
            instance,
            instance->m_declaration->m_name,
            &instance->m_connections[0],
            0);
    for (int i=1; i < instance->m_declaration->m_arity + 1; i++) {
        fprintf(file, "\t\tp%p [shape=plain, label=\"%d\"]\n", &instance->m_connections[i], i);
    }
    fprintf(file, "\n\t}\n");
    for (int i=0; i < instance->m_declaration->m_arity + 1; i++) {
        fprintf(file, "\tp%p -> p%p [arrowhead=none]\n", &instance->m_connections[i], instance->m_connections[i]);
    }
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
    the_instance->m_previous = NULL;
    the_instance->m_next = NULL;
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

fin_net_t *allocate_net(uint32_t names_size) {
    fin_net_t *the_net = calloc(1, sizeof(fin_net_t));
    if (the_net == NULL) {
        fprintf(stderr, "[%s,%d] Cannot allocate a net of %d names", __FILE__, __LINE__, names_size);
        exit(1);
    }
    the_net->m_names_size = names_size;
    the_net->m_names = allocate_names(names_size);

    the_net->m_instances = NULL;

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
        while (net->m_instances != NULL) {
            fin_instance_t *tmp = net->m_instances;
            net->m_instances = net->m_instances->m_next;
            if (net->m_instances != NULL) {
                net->m_instances->m_previous = NULL;
            }
            free_instance(tmp);
        }
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

    fin_instance_t *current = net->m_instances;
    while (current != NULL) {
        fprintf(stdout, "\t");
        print_agent_instance(current);
        current = current->m_next;
    }
    fprintf(stdout, "}\n");
}

void to_dot_net(FILE *file, fin_net_t *net) {
    fprintf(file, "digraph {\n");
    for (int i = 0; i<net->m_names_size; i++) {
        fprintf(file, "\tp%p [shape=circle, label=\"%d\", style=filled, color=lightblue]\n", &net->m_names[i], i);
        fprintf(file, "\tp%p -> p%p [arrowhead=none] \n", &net->m_names[i], net->m_names[i]);
    }
    fin_instance_t *current = net->m_instances;
    while (current != NULL) {
        to_dot_agent_instance(file, current);
        current = current->m_next;
    }
    fprintf(file, "}\n");
}

fin_instance_t *add_instance(fin_net_t *net, fin_instance_t *instance) {
    if (net == NULL || instance == NULL) return NULL;
    instance->m_next = net->m_instances;
    instance->m_previous = NULL;
    if (net->m_instances != NULL) {
        net->m_instances->m_previous = instance;
    }
    net->m_instances = instance;
    return instance;
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

//fin_rule_t *allocate_rule() {
//    //TODO
//    return NULL;
//}
//void free_rule(fin_rule_t *rule) {
//    if (rule == NULL) return;
//    //TODO
//}

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
        for (int i = 0; i<configuration->m_rule_count; i++) {
            free_net(configuration->m_rules[i].m_lhs);
            free_net(configuration->m_rules[i].m_rhs);
        }
        free(configuration->m_rules);
    }
    free(configuration);
}

int32_t find_name_index(fin_net_t *net, fin_port_t port) {
    for (int i=0;i<net->m_names_size; i++) {
        if (port == &net->m_names[i]) {
            return i;
        }
    }
    return -1;
}

//BEGIN poor man hashmap, just two arrays
struct poor_hashmap_s {
    uint32_t m_capacity;
    uint32_t m_size;
    fin_port_t *m_keys;
    fin_port_t *m_values;
};
typedef struct poor_hashmap_s poor_hashmap_t;
poor_hashmap_t *create_hashmap(uint32_t capacity) {
    poor_hashmap_t *map = malloc(sizeof(poor_hashmap_t));
    map->m_capacity = capacity;
    map->m_size = 0;
    map->m_keys = calloc(capacity, sizeof(fin_port_t));
    map->m_values = calloc(capacity, sizeof(fin_port_t));
    return map;
}
void free_hashmap(poor_hashmap_t *map) {
    free(map->m_keys);
    free(map->m_values);
    free(map);
}

void map_add(poor_hashmap_t *map, fin_port_t key, fin_port_t value) {
    for (int i = 0; i<map->m_size; i++) {
        if (map->m_keys[i] == key) {
            return;
        }
    }
    map->m_keys[map->m_size] = key;
    map->m_values[map->m_size] = value;
    map->m_size++;
}

fin_port_t map_get(poor_hashmap_t *map, fin_port_t key) {
    for (int i = 0; i<map->m_size; i++) {
        if (map->m_keys[i] == key) {
            return map->m_values[i];
        }
    }
    return NULL;
}
//END poor man hashmap

/**
 * Implements the copy iterating twice over the net.
 * First iteration creates the new net and maps the old addresses to the new ones.
 * The second iteration uses the map to connect the nodes in the new map.
 * */
fin_net_t *copy_net0(fin_net_t *in_net) {
    //TODO: need to copy the rewrite net

    poor_hashmap_t *map = create_hashmap(100);

    fin_net_t *the_net = allocate_net(in_net->m_names_size);

    for (int i=0;i<in_net->m_names_size; i++) {
        fin_port_t key_port = get_name(in_net, i);
        fin_port_t value_port = get_name(the_net, i);
        map_add(map, key_port, value_port);
    }

    fin_instance_t *current = in_net->m_instances;
    fin_instance_t *last = the_net->m_instances;
    while (current != NULL) {
        fin_instance_t *the_instance = allocate_instance(current->m_declaration);
        if (last == NULL) {
            the_net->m_instances = the_instance;
        } else {
            last->m_next = the_instance;
            the_instance->m_previous = last;
        }
        last = the_instance;

        for (int i=0; i<=the_instance->m_declaration->m_arity; i++) {
            fin_port_t key_port = get_port(current, i);
            fin_port_t value_port = get_port(the_instance, i);
            map_add(map, key_port, value_port);
        }

        current = current->m_next;
    }


    for (int i=0;i<in_net->m_names_size; i++) {
        the_net->m_names[i] = map_get(map, in_net->m_names[i]);
    }

    current = in_net->m_instances;
    fin_instance_t *target = the_net->m_instances;
    while (current != NULL) {
        for (int i=0; i<=current->m_declaration->m_arity; i++) {
            target->m_connections[i] = map_get(map, current->m_connections[i]);
        }

        current = current->m_next;
        target = target->m_next;
    }

    free_hashmap(map);

    return the_net;
}

struct resolve_pair_s {
    fin_port_t *m_target;
    fin_port_t m_query;
};
typedef struct resolve_pair_s resolve_pair_t;
struct vector_s {
    uint32_t m_capacity;
    uint32_t m_size;
    resolve_pair_t *m_data;
};
typedef struct vector_s vector_t;

vector_t *create_vector(uint32_t capacity) {
    vector_t *vector = malloc(sizeof(vector_t));
    if (vector == NULL) {
        fprintf(stderr, "[%s,%d] Cannot allocate a vector of %d ports", __FILE__, __LINE__, capacity);
        exit(1);
    }

    vector->m_data = calloc(capacity, sizeof(resolve_pair_t));
    vector->m_capacity = capacity;
    vector->m_size = 0;
    return vector;
}

void free_vector(vector_t *vector) {
    if (vector == NULL) return;
    free(vector);
}

void push_vector(vector_t *vector, fin_port_t *target, fin_port_t query) {
    if (vector->m_size >= vector->m_capacity) {
        fprintf(stderr, "[%s,%d] Cannot push, the vector is full", __FILE__, __LINE__);
        exit(1);
    }
    vector->m_data[vector->m_size].m_target = target;
    vector->m_data[vector->m_size].m_query = query;
    vector->m_size++;
}

resolve_pair_t *pop_vector(vector_t *vector) {
    if (vector->m_size <= 0) {
        return NULL;
    }
    return &vector->m_data[--vector->m_size];
}

/**
 * Implements the copy in a single pass, using an unresolved stack to store forward pointers.
 * */
fin_net_t *copy_net(fin_net_t *in_net) {
    poor_hashmap_t *map = create_hashmap(100);
    vector_t *unresolved = create_vector(100);

    fin_net_t *the_net = allocate_net(in_net->m_names_size);

    for (int i=0;i<in_net->m_names_size; i++) {
        fin_port_t key_port = get_name(in_net, i);
        fin_port_t value_port = get_name(the_net, i);
        map_add(map, key_port, value_port);

        fin_port_t value = map_get(map, in_net->m_names[i]);
        if (value != NULL) {
            the_net->m_names[i] = value;
        } else {
            push_vector(unresolved, &the_net->m_names[i], in_net->m_names[i]);
        }

    }

    fin_instance_t *current = in_net->m_instances;
    fin_instance_t *last = the_net->m_instances;
    while (current != NULL) {
        fin_instance_t *the_instance = allocate_instance(current->m_declaration);
        if (last == NULL) {
            the_net->m_instances = the_instance;
        } else {
            last->m_next = the_instance;
            the_instance->m_previous = last;
        }
        last = the_instance;

        for (int i=0; i<=the_instance->m_declaration->m_arity; i++) {
            fin_port_t key_port = get_port(current, i);
            fin_port_t value_port = get_port(the_instance, i);
            map_add(map, key_port, value_port);

            fin_port_t value = map_get(map, current->m_connections[i]);
            if (value != NULL) {
                the_instance->m_connections[i] = value;
            } else {
                push_vector(unresolved, &the_instance->m_connections[i], current->m_connections[i]);
            }
        }

        current = current->m_next;
    }

    resolve_pair_t *to_resolve = NULL;
    while ((to_resolve = pop_vector(unresolved)) != NULL) {
        *(to_resolve->m_target) = map_get(map, to_resolve->m_query);
    }

    free_vector(unresolved);
    free_hashmap(map);

    return the_net;
}

void rewrite_active_pair(fin_net_t *io_net, fin_instance_t *in_first, fin_instance_t *in_second, fin_rule_t *in_rule) {
    fin_instance_t *the_first  = in_first;
    fin_instance_t *the_second = in_second;

//    print_net(io_net);

    fin_instance_t *the_first_pattern = NULL;
    fin_instance_t *the_second_pattern = NULL;
    fin_instance_t *current = in_rule->m_lhs->m_instances;
    while (current != NULL) {
        if (current->m_declaration == the_first->m_declaration) {
            the_first_pattern = current;
            if (the_second_pattern != NULL) break;
        } else if (current->m_declaration == the_second->m_declaration) {
            the_second_pattern = current;
            if (the_first_pattern != NULL) break;
        }
        current = current->m_next;
    }
    //copy the target net from the rule
    fin_net_t* the_target = copy_net(in_rule->m_rhs);
    //connect the io_net to the rewrite
    uint32_t arity = the_first->m_declaration->m_arity;
    for (int i=1; i<=arity; i++) {
        fin_port_t port = the_first_pattern->m_connections[i];
        //QUESTION: can we do this without finding the index ?
        int32_t index = find_name_index(in_rule->m_lhs, port);

//        fprintf(stdout, "port %p, index %d\n", port, index);

        connect(
                the_first->m_connections[i],
                *get_name(the_target, index));
        assert (the_target->m_names[index] == *get_name(the_target, index));
    }

    arity = the_second->m_declaration->m_arity;
    for (int i=1; i<=arity; i++) {
        fin_port_t port = the_second_pattern->m_connections[i];
        //QUESTION: can we do this without finding the index ?
        int32_t index = find_name_index(in_rule->m_lhs, port);

//        fprintf(stdout, "port %p, index %d\n", port, index);

        connect(
                the_second->m_connections[i],
                *get_name(the_target, index) );
        assert (the_target->m_names[index] == *get_name(the_target, index));
    }

    //remove and free the matched instances and insert the new ones
    if (the_first->m_next != NULL) {
        the_first->m_next->m_previous = the_first->m_previous;
    }
    if (the_first->m_previous != NULL) {
        the_first->m_previous->m_next = the_first->m_next;
    }
    if (io_net->m_instances == the_first) {
        io_net->m_instances = the_first->m_next;
    }
    free_instance(the_first);

    if (the_second->m_next != NULL) {
        the_second->m_next->m_previous = the_second->m_previous;
    }
    if (the_second->m_previous != NULL) {
        the_second->m_previous->m_next = the_second->m_next;
    }
    if (io_net->m_instances == the_second) {
        io_net->m_instances = the_second->m_next;
    }
    free_instance(the_second);

    //add the instances from the target to the net being rewritten (io_net)
    while (the_target->m_instances != NULL) {
        fin_instance_t *to_add = the_target->m_instances;
        the_target->m_instances = the_target->m_instances->m_next;
        add_instance(io_net, to_add);

    }
    //free the target
    free_net(the_target);

//    print_net(io_net);
    //TODO: need to detect the apparition of a new active pair.
}




//
// Created by Ciprian TEODOROV on 29/12/2021.
//

#include <stdlib.h>
#include <peano.h>

fin_configuration_t *peano_addition() {
    fin_configuration_t *the_configuration = allocate_configuration(3, 2);

    //ADD the agents
    the_configuration->m_agent_declarations[0] = (fin_agent_declaration_t) {"Z",0};
    the_configuration->m_agent_declarations[1] = (fin_agent_declaration_t) {"S",1};
    the_configuration->m_agent_declarations[2] = (fin_agent_declaration_t) {"+",2};

    fin_agent_declaration_t *theZ = &the_configuration->m_agent_declarations[0];
    fin_agent_declaration_t *theS = &the_configuration->m_agent_declarations[1];
    fin_agent_declaration_t *theP = &the_configuration->m_agent_declarations[2];

    //ADD the rules
    // Z + => 0 + n = n
    fin_net_t *the_pattern = allocate_net(2);
    fin_instance_t * z0 = add_instance(the_pattern, allocate_instance(theZ));
    fin_instance_t * p0 = add_instance(the_pattern, allocate_instance(theP));
    connect(get_port(z0, 0), get_port(p0, 0));
    connect(get_port(p0, 1), get_name(the_pattern, 0));
    connect(get_port(p0, 2), get_name(the_pattern, 1));

    fin_net_t *the_result = allocate_net(2);
    connect(get_name(the_result, 0), get_name(the_result, 1));

    //add Z + rule
    the_configuration->m_rules[0] = (fin_rule_t) {
            the_pattern,
            the_result
    };

    // S + => (S x) + y = S ( x + y )
    the_pattern = allocate_net(3);
    fin_instance_t * s1 = add_instance(the_pattern, allocate_instance(theS));
    fin_instance_t * p1 = add_instance(the_pattern, allocate_instance(theP));
    connect(get_port(s1, 0), get_port(p1, 0));
    connect(get_port(s1, 1), get_name(the_pattern, 0));
    connect(get_port(p1, 1), get_name(the_pattern, 1));
    connect(get_port(p1, 2), get_name(the_pattern, 2));

    the_result = allocate_net(3);
    s1 = add_instance(the_result, allocate_instance(theS));
    p1 = add_instance(the_result, allocate_instance(theP));
    connect(get_port(p1, 0), get_name(the_result, 0));
    connect(get_port(p1, 1), get_name(the_result, 1));
    connect(get_port(p1, 2), get_port(s1, 1));
    connect(get_port(s1, 0), get_name(the_result, 2));

    the_configuration->m_rules[1] = (fin_rule_t) {
            the_pattern,
            the_result
    };
    return the_configuration;
}

fin_configuration_t *peano_add_2_1() {
    fin_configuration_t *the_configuration = peano_addition();
    //Create an active net
    fin_net_t *the_net = the_configuration->m_net = allocate_net(1);

    fin_agent_declaration_t *theZ = find_agent(the_configuration, "Z");
    fin_agent_declaration_t *theS = find_agent(the_configuration, "S");
    fin_agent_declaration_t *theP = find_agent(the_configuration, "+");

    //create the instances
    fin_instance_t * aZ = add_instance(the_net, allocate_instance(theZ));
    fin_instance_t * aS = add_instance(the_net, allocate_instance(theS));
    fin_instance_t * bZ = add_instance(the_net, allocate_instance(theZ));
    fin_instance_t * bS = add_instance(the_net, allocate_instance(theS));
    fin_instance_t * aP = add_instance(the_net, allocate_instance(theP));

    fin_instance_t *cS = add_instance(the_net, allocate_instance(theS));

    /*NET for config 0 -- BEGIN*/
    //connect the instances
    connect(get_port(aZ, 0), get_port(cS, 1));
    connect(get_port(cS, 0), get_port(aS, 1));
    connect(get_port(aS, 0), get_port(aP, 0));
    add_active_pair(aS, aP, the_configuration);

    connect(get_port(bZ, 0), get_port(bS, 1));
    connect(get_port(bS, 0), get_port(aP, 1));

    connect(get_port(aP, 2), get_name(the_net, 0));

    return the_configuration;
}

fin_configuration_t *peano_number(uint32_t n) {
    fin_configuration_t *the_configuration = peano_addition();
    fin_agent_declaration_t *theZ = find_agent(the_configuration, "Z");
    fin_agent_declaration_t *theS = find_agent(the_configuration, "S");

    fin_net_t *the_net = the_configuration->m_net = allocate_net(1);

    fin_instance_t *current = add_instance(the_net, allocate_instance(theZ));
    for (int i = 0; i < n; i++) {
        fin_instance_t * the_successor = add_instance(the_net, allocate_instance(theS));
        connect(get_port(current, 0), get_port(the_successor, 1));
        current = the_successor;
    }
    connect(get_port(current, 0), get_name(the_net, 0));
    return the_configuration;
}

fin_configuration_t *peano_add(uint32_t n, uint32_t m) {
    fin_configuration_t *the_configuration = peano_addition();
    fin_agent_declaration_t *theZ = find_agent(the_configuration, "Z");
    fin_agent_declaration_t *theS = find_agent(the_configuration, "S");
    fin_agent_declaration_t *theP = find_agent(the_configuration, "+");

    return the_configuration;
}
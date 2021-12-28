#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <allocator.h>

#include <fin_core.h>






int main() {

    fin_configuration_t *the_configuration = allocate_configuration(3, 2);

    //ADD the agents
    the_configuration->m_agent_declarations[0] = (fin_agent_declaration_t) {"Z",0};
    the_configuration->m_agent_declarations[1] = (fin_agent_declaration_t) {"S",1};
    the_configuration->m_agent_declarations[2] = (fin_agent_declaration_t) {"+",2};

    fin_agent_declaration_t *theZ = &the_configuration->m_agent_declarations[0];
    fin_agent_declaration_t *theS = &the_configuration->m_agent_declarations[1];
    fin_agent_declaration_t *thePlus = &the_configuration->m_agent_declarations[2];

    //ADD the rules
    //NO RULES YET
    the_configuration->m_rules = NULL;

    //Create the net
    fin_net_t *the_net = the_configuration->m_net = allocate_net(1, 5);

    //create the instances
    fin_instance_t * aZ = the_net->m_instances[0] = allocate_instance(theZ);
    fin_instance_t * aS = the_net->m_instances[1] = allocate_instance(theS);
    fin_instance_t * bZ = the_net->m_instances[2] = allocate_instance(theZ);
    fin_instance_t * bS = the_net->m_instances[3] = allocate_instance(theS);
    fin_instance_t * aP = the_net->m_instances[4] = allocate_instance(thePlus);

    //connect the instances
    connect(get_port(aZ, 0), get_port(aS, 1));
    connect(get_port(aS, 0), get_port(aP, 0));

    connect(get_port(bZ, 0), get_port(bS, 1));
    connect(get_port(bS, 0), get_port(aP, 1));

    connect(get_port(aP, 2), get_name(the_net, 0));

    //put the net in a configuration
    the_configuration->m_net = the_net;

    print_net(the_net);
//    print_agent_instance(aP);

    //free the net
    free_net(the_net);
    //free the configuration
    free_configuration(the_configuration);

    return 0;
}

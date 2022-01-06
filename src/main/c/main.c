#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <fin_core.h>


struct ap {
    fin_instance_t *a;
    fin_instance_t *b;
};

void ap_handler(fin_instance_t *a, fin_instance_t *b, void *data) {
    char *x = a->m_declaration->m_name;
    char *y = b->m_declaration->m_name;
    fprintf(stdout, "CB active pair %s(%p)-%s(%p)\n", x, &a->m_connections[0], y, &b->m_connections[0]);
    if (data == NULL) return;
    ((struct ap*) data)->a = a;
    ((struct ap*) data)->b = b;
}

int main() {
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


    //Create an active net
    fin_net_t *the_net = the_configuration->m_net = allocate_net(1);

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
    /* NET for config 0 -- END*/
    /*NET for config 1 -- BEGIN*/

//    connect(get_port(aZ, 0), get_port(aP, 0));
//    connect(get_port(aP, 1), get_port(bS, 0));
//    connect(get_port(aP, 2), get_port(aS, 1));
//    connect(get_port(bZ, 0), get_port(bS, 1));
//    connect(get_port(aS, 0), get_name(the_net, 0));

    /*NET for config 1 -- END*/

    fprintf(stdout, "rule : Z +\n");
    print_net(the_configuration->m_rules[0].m_lhs);
    print_net(the_configuration->m_rules[0].m_rhs);

    fprintf(stdout, "rule : S +\n");
    print_net(the_configuration->m_rules[1].m_lhs);
    print_net(the_configuration->m_rules[1].m_rhs);

    fprintf(stdout, "active net :\n");
    print_net(the_net);

    to_dot_net(stdout, the_net);

    fprintf(stdout, "rewrite net :\n");
    /* rewrite CONFIG 0 => CONFIG 1*/
//    rewrite_active_pair(
//            the_net,
//            aS, aP,
//            &the_configuration->m_rules[1],
//            ap_handler, NULL);

    /*rewrite CONFIG 1 => CONFIG 2*/
//    rewrite_active_pair(
//            the_net,
//            aZ,aP,
//            &the_configuration->m_rules[0],
//            ap_handler, NULL);

    /* rewrite CONFIG 0 => CONFIG 2*/
//    struct ap the_ap;
//    rewrite_active_pair(
//            the_net,
//            aS, aP,
//            matching_rule(the_configuration, aS, aP),
//            ap_handler, &the_ap);
//    rewrite_active_pair(
//            the_net,
//            the_ap.a, the_ap.b,
//            matching_rule(the_configuration, the_ap.a, the_ap.b),
//            ap_handler, NULL);
//    to_dot_net(stdout, the_net);

    /*reduce*/
    reduce(the_configuration);

    //free the net
    free_net(the_net);
    //free the configuration
    free_configuration(the_configuration);

    return 0;
}


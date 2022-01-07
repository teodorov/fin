//
// Created by Ciprian TEODOROV on 29/12/2021.
//

#include <peano.h>

fin_environment_t *peano_addition_agents() {
    fin_environment_t *the_environment = allocate_environment(3, 2);

    //ADD the agents
    the_environment->m_agent_declarations[0] = (fin_agent_declaration_t) {"Z", 0};
    the_environment->m_agent_declarations[1] = (fin_agent_declaration_t) {"S", 1};
    the_environment->m_agent_declarations[2] = (fin_agent_declaration_t) {"+", 2};
    return the_environment;
}

fin_net_t *peano_zero_plus_left(fin_environment_t *peano) {
    fin_agent_declaration_t *theZ = find_agent(peano, "Z");
    fin_agent_declaration_t *theP = find_agent(peano, "+");

    fin_net_t *the_net = allocate_net(2);
    fin_instance_t * z0 = add_instance(the_net, allocate_instance(theZ));
    fin_instance_t * p0 = add_instance(the_net, allocate_instance(theP));
    connect(get_port(z0, 0), get_port(p0, 0));
    connect(get_port(p0, 1), get_name(the_net, 0));
    connect(get_port(p0, 2), get_name(the_net, 1));

    return the_net;
}

fin_net_t *peano_zero_plus_right(fin_environment_t *peano) {
    fin_net_t *the_net = allocate_net(2);
    connect(get_name(the_net, 0), get_name(the_net, 1));
    return the_net;
}

fin_net_t *peano_s_plus_left(fin_environment_t *peano) {
    fin_agent_declaration_t *theS = find_agent(peano, "S");
    fin_agent_declaration_t *theP = find_agent(peano, "+");

    fin_net_t *the_net = allocate_net(3);
    fin_instance_t * s1 = add_instance(the_net, allocate_instance(theS));
    fin_instance_t * p1 = add_instance(the_net, allocate_instance(theP));
    connect(get_port(s1, 0), get_port(p1, 0));
    connect(get_port(s1, 1), get_name(the_net, 0));
    connect(get_port(p1, 1), get_name(the_net, 1));
    connect(get_port(p1, 2), get_name(the_net, 2));

    return the_net;
}

fin_net_t *peano_s_plus_right(fin_environment_t *peano) {
    fin_agent_declaration_t *theS = find_agent(peano, "S");
    fin_agent_declaration_t *theP = find_agent(peano, "+");

    fin_net_t *the_net = allocate_net(3);
    fin_instance_t *s1 = add_instance(the_net, allocate_instance(theS));
    fin_instance_t *p1 = add_instance(the_net, allocate_instance(theP));
    connect(get_port(p1, 0), get_name(the_net, 0));
    connect(get_port(p1, 1), get_name(the_net, 1));
    connect(get_port(p1, 2), get_port(s1, 1));
    connect(get_port(s1, 0), get_name(the_net, 2));

    return the_net;
}

fin_environment_t *peano_addition_environment() {
    fin_environment_t *the_environment = peano_addition_agents();

    // [Z+] 0 + n = n
    the_environment->m_rules[0] = (fin_rule_t) {
            peano_zero_plus_left(the_environment),
            peano_zero_plus_right(the_environment)
    };

    // [S+] (S x) + y = S ( x + y )
    the_environment->m_rules[1] = (fin_rule_t) {
            peano_s_plus_left(the_environment),
            peano_s_plus_right(the_environment)
    };

    return the_environment;
}

fin_net_t *peano_add_2_1(fin_environment_t *peano) {
    fin_environment_t *the_environment = peano;
    if (the_environment == NULL) {
        the_environment = peano_addition_environment();
    }
    //Create an active net
    fin_net_t *the_net = allocate_net(1);

    fin_agent_declaration_t *theZ = find_agent(the_environment, "Z");
    fin_agent_declaration_t *theS = find_agent(the_environment, "S");
    fin_agent_declaration_t *theP = find_agent(the_environment, "+");

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
    add_active_pair(the_net, aS, aP);

    connect(get_port(bZ, 0), get_port(bS, 1));
    connect(get_port(bS, 0), get_port(aP, 1));

    connect(get_port(aP, 2), get_name(the_net, 0));

    return the_net;
}

fin_net_t *peano_number(fin_environment_t *peano, uint32_t n) {
    fin_environment_t *the_environment = peano;
    if (the_environment == NULL) {
        the_environment = peano_addition_environment();
    }
    fin_agent_declaration_t *theZ = find_agent(the_environment, "Z");
    fin_agent_declaration_t *theS = find_agent(the_environment, "S");

    fin_net_t *the_net = allocate_net(1);

    fin_instance_t *current = add_instance(the_net, allocate_instance(theZ));
    for (int i = 0; i < n; i++) {
        fin_instance_t * the_successor = add_instance(the_net, allocate_instance(theS));
        connect(get_port(current, 0), get_port(the_successor, 1));
        current = the_successor;
    }
    connect(get_port(current, 0), get_name(the_net, 0));
    return the_net;
}

fin_net_t *peano_add(fin_environment_t *peano, uint32_t n, uint32_t m) {
    fin_environment_t *the_environment = peano;
    if (the_environment == NULL) {
        the_environment = peano_addition_environment();
    }
    fin_agent_declaration_t *theP = find_agent(the_environment, "+");

    fin_net_t *the_net = allocate_net(1);
    fin_instance_t *aP = add_instance(the_net, allocate_instance(theP));
    connect(get_name(the_net, 0), get_port(aP, 2));

    add_net(the_net, peano_number(the_environment, n), 1, get_port(aP, 0), 0);
    add_net(the_net, peano_number(the_environment, m), 1, get_port(aP, 1), 0);

    return the_net;
}

fin_environment_t *peano_agents() {
    fin_environment_t *the_environment = allocate_environment(6, 8);

    //ADD the agents
    the_environment->m_agent_declarations[0] = (fin_agent_declaration_t) {"Z", 0};
    the_environment->m_agent_declarations[1] = (fin_agent_declaration_t) {"S", 1};
    the_environment->m_agent_declarations[2] = (fin_agent_declaration_t) {"+", 2};

    the_environment->m_agent_declarations[3] = (fin_agent_declaration_t) {"*", 2};
    the_environment->m_agent_declarations[4] = (fin_agent_declaration_t) {"eps", 0};
    the_environment->m_agent_declarations[5] = (fin_agent_declaration_t) {"delta", 2};
    return the_environment;
}

fin_net_t *peano_zero_mul_left(fin_environment_t *peano) {
    fin_agent_declaration_t *theZ = find_agent(peano, "Z");
    fin_agent_declaration_t *theM = find_agent(peano, "*");

    fin_net_t *the_net = allocate_net(2);
    fin_instance_t * z = add_instance(the_net, allocate_instance(theZ));
    fin_instance_t * m = add_instance(the_net, allocate_instance(theM));
    connect(get_port(z, 0), get_port(m, 0));
    connect(get_port(m, 1), get_name(the_net, 0));
    connect(get_port(m, 2), get_name(the_net, 1));
    return the_net;
}

fin_net_t *peano_zero_mul_right(fin_environment_t *peano) {
    fin_agent_declaration_t *theZ = find_agent(peano, "Z");
    fin_agent_declaration_t *theEps = find_agent(peano, "eps");

    fin_net_t *the_net = allocate_net(2);
    fin_instance_t * z = add_instance(the_net, allocate_instance(theZ));
    fin_instance_t * eps = add_instance(the_net, allocate_instance(theEps));

    connect(get_port(eps, 0), get_name(the_net, 0));
    connect(get_port(z, 0), get_name(the_net, 1));

    return the_net;
}

fin_net_t *peano_zero_eps_left(fin_environment_t *peano) {
    fin_agent_declaration_t *theZ = find_agent(peano, "Z");
    fin_agent_declaration_t *theEps = find_agent(peano, "eps");

    fin_net_t *the_net = allocate_net(0);
    fin_instance_t * z = add_instance(the_net, allocate_instance(theZ));
    fin_instance_t * eps = add_instance(the_net, allocate_instance(theEps));

    connect(get_port(z, 0), get_port(eps, 0));

    return the_net;
}

fin_net_t *peano_zero_delta_left(fin_environment_t *peano) {
    fin_agent_declaration_t *theZ = find_agent(peano, "Z");
    fin_agent_declaration_t *theDelta = find_agent(peano, "delta");

    fin_net_t *the_net = allocate_net(2);
    fin_instance_t * z = add_instance(the_net, allocate_instance(theZ));
    fin_instance_t * delta = add_instance(the_net, allocate_instance(theDelta));
    connect(get_port(z, 0), get_port(delta, 0));
    connect(get_port(delta, 1), get_name(the_net, 0));
    connect(get_port(delta, 2), get_name(the_net, 1));
    return the_net;
}

fin_net_t *peano_zero_delta_right(fin_environment_t *peano) {
    fin_agent_declaration_t *theZ = find_agent(peano, "Z");
    fin_agent_declaration_t *theDelta = find_agent(peano, "delta");

    fin_net_t *the_net = allocate_net(2);
    fin_instance_t * z1 = add_instance(the_net, allocate_instance(theZ));
    fin_instance_t * z2 = add_instance(the_net, allocate_instance(theZ));

    connect(get_port(z1, 0), get_name(the_net, 0));
    connect(get_port(z2, 0), get_name(the_net, 1));
    return the_net;
}

fin_net_t *peano_s_mul_left(fin_environment_t *peano) {
    fin_agent_declaration_t *theS = find_agent(peano, "S");
    fin_agent_declaration_t *theM = find_agent(peano, "*");

    fin_net_t *the_net = allocate_net(3);
    fin_instance_t * s = add_instance(the_net, allocate_instance(theS));
    fin_instance_t * m = add_instance(the_net, allocate_instance(theM));
    connect(get_port(s, 0), get_port(m, 0));
    connect(get_port(s, 1), get_name(the_net, 0));
    connect(get_port(m, 1), get_name(the_net, 1));
    connect(get_port(m, 2), get_name(the_net, 2));

    return the_net;
}

fin_net_t *peano_s_mul_right(fin_environment_t *peano) {
    fin_agent_declaration_t *theP = find_agent(peano, "+");
    fin_agent_declaration_t *theM = find_agent(peano, "*");
    fin_agent_declaration_t *theD = find_agent(peano, "delta");

    fin_net_t *the_net = allocate_net(3);
    fin_instance_t * p = add_instance(the_net, allocate_instance(theP));
    fin_instance_t * m = add_instance(the_net, allocate_instance(theM));
    fin_instance_t * d = add_instance(the_net, allocate_instance(theD));


    connect(get_port(m, 0), get_name(the_net, 0));
    connect(get_port(d, 0), get_name(the_net, 1));
    connect(get_port(d, 1), get_port(m, 1));

    connect(get_port(p, 0), get_port(m, 2));
    connect(get_port(p, 1), get_port(d, 2));
    connect(get_port(p, 2), get_name(the_net, 2));

    return the_net;
}


fin_net_t *peano_s_eps_left(fin_environment_t *peano) {
    fin_agent_declaration_t *theS = find_agent(peano, "S");
    fin_agent_declaration_t *theE = find_agent(peano, "eps");

    fin_net_t *the_net = allocate_net(1);
    fin_instance_t * s = add_instance(the_net, allocate_instance(theS));
    fin_instance_t * e = add_instance(the_net, allocate_instance(theE));
    connect(get_port(s, 0), get_port(e, 0));
    connect(get_port(s, 1), get_name(the_net, 0));

    return the_net;
}

fin_net_t *peano_s_eps_right(fin_environment_t *peano) {
    fin_agent_declaration_t *theE = find_agent(peano, "eps");

    fin_net_t *the_net = allocate_net(1);
    fin_instance_t * e = add_instance(the_net, allocate_instance(theE));
    connect(get_port(e, 0), get_name(the_net, 0));

    return the_net;
}

fin_net_t *peano_s_delta_left(fin_environment_t *peano) {
    fin_agent_declaration_t *theS = find_agent(peano, "S");
    fin_agent_declaration_t *theD = find_agent(peano, "delta");

    fin_net_t *the_net = allocate_net(3);
    fin_instance_t * s = add_instance(the_net, allocate_instance(theS));
    fin_instance_t * d = add_instance(the_net, allocate_instance(theD));
    connect(get_port(d, 0), get_port(s, 0));
    connect(get_port(d, 1), get_name(the_net, 0));
    connect(get_port(d, 2), get_name(the_net, 1));
    connect(get_port(s, 1), get_name(the_net, 2));

    return the_net;
}

fin_net_t *peano_s_delta_right(fin_environment_t *peano) {
    fin_agent_declaration_t *theS = find_agent(peano, "S");
    fin_agent_declaration_t *theD = find_agent(peano, "delta");

    fin_net_t *the_net = allocate_net(3);
    fin_instance_t * s1 = add_instance(the_net, allocate_instance(theS));
    fin_instance_t * s2 = add_instance(the_net, allocate_instance(theS));
    fin_instance_t * d = add_instance(the_net, allocate_instance(theD));

    connect(get_port(s1, 0), get_name(the_net, 0));
    connect(get_port(s1, 1), get_port(d, 1));

    connect(get_port(s2, 0), get_name(the_net, 1));
    connect(get_port(s2, 1), get_port(d, 2));

    connect(get_port(d, 0), get_name(the_net, 2));
    return the_net;
}


fin_environment_t *peano_environment() {
    fin_environment_t *the_environment = peano_agents();

    // [Z_+] 0 + n = n
    the_environment->m_rules[0] = (fin_rule_t) {
            peano_zero_plus_left(the_environment),
            peano_zero_plus_right(the_environment)
    };

    // [S_+] (S x) + y = S ( x + y )
    the_environment->m_rules[1] = (fin_rule_t) {
            peano_s_plus_left(the_environment),
            peano_s_plus_right(the_environment)
    };

    //[Z_*] 0*y = 0
    the_environment->m_rules[2] = (fin_rule_t) {
            peano_zero_mul_left(the_environment),
            peano_zero_mul_right(the_environment)
    };
    //[Z_eps] Z><eps = nothing
    the_environment->m_rules[3] = (fin_rule_t) {
            peano_zero_eps_left(the_environment),
            NULL
    };

    //[Z_delta] Z><delta = Z Z
    the_environment->m_rules[4] = (fin_rule_t) {
            peano_zero_delta_left(the_environment),
            peano_zero_delta_right(the_environment)
    };

    //[S_*] S i * y = (i * y) + y
    the_environment->m_rules[5] = (fin_rule_t) {
            peano_s_mul_left(the_environment),
            peano_s_mul_right(the_environment)
    };
    //[S_eps] S><eps = eps
    the_environment->m_rules[6] = (fin_rule_t) {
            peano_s_eps_left(the_environment),
            peano_s_eps_right(the_environment)
    };
    //[S_delta] S><delta = S S
    the_environment->m_rules[7] = (fin_rule_t) {
            peano_s_delta_left(the_environment),
            peano_s_delta_right(the_environment)
    };

    return the_environment;
}

fin_net_t *peano_mul(fin_environment_t *peano, uint32_t n, uint32_t m) {
    fin_environment_t *the_environment = peano;
    if (the_environment == NULL) {
        the_environment = peano_environment();
    }
    fin_agent_declaration_t *theM = find_agent(the_environment, "*");

    fin_net_t *the_net = allocate_net(1);
    fin_instance_t *aM = add_instance(the_net, allocate_instance(theM));
    connect(get_name(the_net, 0), get_port(aM, 2));

    add_net(the_net, peano_number(the_environment, n), 1, get_port(aM, 0), 0);
    add_net(the_net, peano_number(the_environment, m), 1, get_port(aM, 1), 0);

    return the_net;
}
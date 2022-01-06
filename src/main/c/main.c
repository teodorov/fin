#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <fin_core.h>
#include <peano.h>

int main() {
    fin_environment_t *the_addition_environment = peano_addition();

//    fin_net_t *the_add_2_1 = peano_add_2_1(the_addition_environment);
//    reduce(the_addition_environment, the_add_2_1);
//    to_dot_net(stdout, the_add_2_1);
//    free_net(the_add_2_1);


    fin_net_t *the_add = peano_add(the_addition_environment, 10000, 200);
    reduce(the_addition_environment, the_add);
//    to_dot_net(stdout, the_add);
    free_net(the_add);

    free_environment(the_addition_environment);
    return 0;
}


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <fin_core.h>
#include <peano.h>
#include <time.h>

int main() {
//    fin_environment_t *the_addition_environment = peano_addition_environment();
//    fin_net_t *the_add = peano_add(the_addition_environment, 10, 10);
//    reduce(the_addition_environment, the_add);
//    to_dot_net(stdout, the_add);
//    free_net(the_add);
//    free_environment(the_addition_environment);

    fin_environment_t *the_peano_environment = peano_environment();
    fin_net_t *the_mul = peano_mul(the_peano_environment, 200, 100);

    clock_t start = clock();

    reduce(the_peano_environment, the_mul);

    clock_t end = clock();

    printf("Elapsed: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

//    to_dot_net(stdout, the_mul);
    free_net(the_mul);
    free_environment(the_peano_environment);
    return 0;
}


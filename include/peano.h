//
// Created by Ciprian TEODOROV on 29/12/2021.
//

#ifndef FAST_INTERACTION_NETS_PEANO_H
#define FAST_INTERACTION_NETS_PEANO_H
#include <fin_core.h>

fin_environment_t *peano_addition_environment();
fin_net_t *peano_add_2_1(fin_environment_t * peano);
fin_net_t *peano_number(fin_environment_t *peano, uint32_t n);
fin_net_t *peano_add(fin_environment_t *peano, uint32_t n, uint32_t m);

fin_environment_t *peano_environment();
fin_net_t *peano_mul(fin_environment_t *peano, uint32_t n, uint32_t m);

#endif //FAST_INTERACTION_NETS_PEANO_H

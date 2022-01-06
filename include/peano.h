//
// Created by Ciprian TEODOROV on 29/12/2021.
//

#ifndef FAST_INTERACTION_NETS_PEANO_H
#define FAST_INTERACTION_NETS_PEANO_H
#include <fin_core.h>

fin_configuration_t *peano_addition();
fin_configuration_t *peano_add_2_1();
fin_configuration_t *peano_number(fin_configuration_t *peano, uint32_t n);
fin_configuration_t *peano_add(fin_configuration_t *peano, uint32_t n, uint32_t m);

#endif //FAST_INTERACTION_NETS_PEANO_H

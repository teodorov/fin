//
// Created by Ciprian TEODOROV on 24/12/2021.
//

#ifndef FAST_INTERACTION_NETS_ALLOCATOR_H
#define FAST_INTERACTION_NETS_ALLOCATOR_H

#include <stdint.h>

void *fin_malloc(size_t size);
void	*fin_calloc(size_t count, size_t size);
void fin_free(void *ptr);
//uint64_t malloc_sizes[1 << 9];
#define malloc fin_malloc
#define calloc fin_calloc
#define free fin_free

#endif //FAST_INTERACTION_NETS_ALLOCATOR_H

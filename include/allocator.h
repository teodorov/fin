//
// Created by Ciprian TEODOROV on 24/12/2021.
//

#ifndef FAST_INTERACTION_NETS_ALLOCATOR_H
#define FAST_INTERACTION_NETS_ALLOCATOR_H

#include <stdint.h>

typedef struct mem_region_s mem_region_t;
mem_region_t *region_allocate(size_t in_capacity, size_t in_chunk_size);
void region_free(mem_region_t * io_region);
void* fin_malloc(mem_region_t *io_region);
void *fin_calloc(mem_region_t *io_region);
void fin_free(mem_region_t *io_region, void *ptr);

#endif //FAST_INTERACTION_NETS_ALLOCATOR_H

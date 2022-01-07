//
// Created by Ciprian TEODOROV on 24/12/2021.
//

#include <stddef.h>
#include <allocator.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>
#include <bitset.h>


//TODO: maybe it is useful to implement an arity based block allocator, on free list of blocks for each arity

typedef struct mem_region_s mem_region_t;
struct mem_region_s {
    size_t m_capacity;
    size_t m_chunk_size;
    void  *m_region_start;
    size_t m_region_size;
    mem_region_t *m_next;
    char  *m_data_start;
    char   m_data[];
};

mem_region_t *region_allocate(size_t in_capacity, size_t in_chunk_size) {
    size_t size = sizeof(mem_region_t) + BITNSLOTS(in_capacity) + in_capacity * (in_chunk_size);
    mem_region_t * region = mmap(
            NULL, /* Address (we use NULL to let the kernel decide) */
            size, /* Size of memory block to allocate */
            PROT_READ | PROT_WRITE, /* Memory protection flags */
            MAP_PRIVATE | MAP_ANONYMOUS, /* Type of mapping */
            -1, /* file descriptor */
            0 /* offset to start at within the file */);

    //if MAP_FAILED, rip
    if (region == MAP_FAILED) {
        fprintf(stderr, "mmap error\n");
        exit(1);
    }

    region->m_capacity      = in_capacity;
    region->m_chunk_size    = in_chunk_size;
    region->m_region_start  = region;
    region->m_region_size   = size;
    region->m_next          = NULL;
    region->m_data_start    = region->m_data + BITNSLOTS(in_capacity);
    return region;
}

void region_free(mem_region_t * io_region) {
    //munmap that pointer
    int ret = munmap(
            io_region->m_region_start,
            io_region->m_region_size);
    if (ret == -1) {
        fprintf(stderr, "unmmap error\n");
        exit(1);
    }
}

void * regions[1 << 10];
void* fin_malloc(size_t size) {
    if (size >= (1<<10)) {
        printf("malloc used\n");
        return malloc(size);
    }
    mem_region_t *region = regions[size];
    if (region == NULL) {
        region = regions[size] = region_allocate(1<<10, size);
    }
    mem_region_t *previous = NULL;
    while (region != NULL) {
        previous = region;
        //find a free spot
        for (size_t i = 0; i < region->m_capacity; i++) {
            if (!BITTEST(region->m_data, i)) {
                BITSET(region->m_data, i);
                return region->m_data_start + i*region->m_chunk_size;
            }
        }
        //no free spot found in this region, go to the next
        region = region->m_next;
    }
    //no free spot found in any regions
    region = region_allocate(previous->m_capacity, previous->m_chunk_size);
    previous->m_next = region;

    BITSET(region->m_data, 0);
    return region->m_data_start;
}

void *fin_calloc(size_t count, size_t size) {
    void * ptr = fin_malloc(count * size);
    memset(ptr, 0, count*size);
    return ptr;
}

void fin_free(void *ptr) {
//    (((char *)ptr)[-2]) = 0;
    for (int i = 0; i<1<<10; i++) {
        if (regions[i] != NULL) {
            mem_region_t *region = regions[i];
            while (region != NULL) {
                if (ptr > (void*)region && ptr < (void *)((char *)region + region->m_region_size) ) {
                    BITCLEAR(region->m_data, (((char*)ptr) - (region->m_data_start)) / region->m_chunk_size );
                    return;
                }
                region = region->m_next;
            }
        }
    }
}
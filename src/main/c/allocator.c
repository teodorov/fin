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

typedef struct block_s block_t;
struct block_s {
    block_t *m_next;
};

struct mem_region_s {
    size_t m_capacity;
    size_t m_chunk_size;
    void  *m_region_start;
    size_t m_region_size;
    mem_region_t *m_next;
    mem_region_t *m_last;
    block_t *m_free_blocks;
    char   m_data[];
};

mem_region_t *region_allocate(size_t in_capacity, size_t in_chunk_size) {
    size_t size = sizeof(mem_region_t) + in_capacity * (in_chunk_size);
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
    region->m_last          = NULL;
    region->m_free_blocks   = (block_t*)region->m_data;

    block_t *block = region->m_free_blocks;
    for (int i = 0; i<in_capacity-1; i++) {
        block->m_next = (block_t*)(((char*)block) + in_chunk_size);
        block = block->m_next;
    }
    block->m_next = NULL;

    return region;
}

void region_free(mem_region_t * io_region) {

    mem_region_t *current = io_region;
    mem_region_t *previous = NULL;
    while (current != NULL) {
        previous = current;
        current = current->m_next;

        int ret = munmap(
                previous->m_region_start,
                previous->m_region_size);
        if (ret == -1) {
            fprintf(stderr, "unmmap error\n");
            exit(1);
        }
    }


}

void* fin_malloc(mem_region_t *io_region) {
    if (io_region->m_free_blocks == NULL) {
        mem_region_t *new = region_allocate(io_region->m_capacity, io_region->m_chunk_size);
        if (io_region->m_next == NULL) {
            io_region->m_next = new;
            io_region->m_last = new;
        } else {
            io_region->m_last->m_next = new;
            io_region->m_last = new;
        }
        io_region->m_free_blocks = new->m_free_blocks;
    }
    block_t *block = io_region->m_free_blocks;
    io_region->m_free_blocks = io_region->m_free_blocks->m_next;
    return block;
}

void *fin_calloc(mem_region_t *io_region) {
    void * ptr = fin_malloc(io_region);
    memset(ptr, 0, io_region->m_chunk_size);
    return ptr;
}

void fin_free(mem_region_t *io_region, void *ptr) {
    block_t *block = (block_t *)ptr;
    block->m_next = io_region->m_free_blocks;
    io_region->m_free_blocks = block;
}
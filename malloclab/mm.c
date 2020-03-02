/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * Implicit List with Boundary Tags  
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "vegetable",
    /* First member's full name */
    "gaofanfei",
    /* First member's email address */
    "gfanfei@gmail.com",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

typedef uint64_t word_t;

const size_t dsize = 2 * sizeof(word_t);

size_t min_block_size = 128;   //need to change

static size_t alloc_space = 2048;

typedef struct block
{
    word_t header;
    unsigned char payload[0];
}block_t;

block_t *heap_start;
block_t *heap_end;

static size_t get_size(block_t *block);
static int get_alloc(block_t *block);
static void *get_payload(block_t *block);
static word_t *find_prev_footer(block_t *block);
static word_t *header_to_footer(block_t *block);
static int alloc_memory(size_t asize);
static block_t *find_next(block_t *block);
static block_t *find_prev(block_t *block);
static block_t *find_fit(size_t asize);
static void write_header(block_t *block, size_t size, int alloc);
static void write_footer(block_t *block, size_t size, int alloc);
static void split_block(block_t *block, size_t asize);
static void coalesce_block(block_t *block);


static size_t get_size(block_t *block)
{
    word_t header = block->header;
    return (header & ~0x1);
}

static int get_alloc(block_t *block)
{
    word_t header = block->header;
    return (header & 0x1);
}

static void *get_payload(block_t *block)
{
    return (void*)block->payload;

}

static word_t *find_prev_footer(block_t *block)
{
    return &(block->header) - 1;
}


static word_t *header_to_footer(block_t *block)
{
    size_t asize = get_size(block);
    return (word_t *) (block->payload + asize - dsize);
}

static void print_heap()
{
    printf("\n**********heap info***********\n");
    for (block_t *p = heap_start; p != heap_end; p = find_next(p))
    {
        size_t asize = get_size(p);
        int alloc = get_alloc(p);
        printf("\naddr: %p  asize : %d  alloc : %d\n", p, asize, alloc);
    }
    printf("\n**********heap info***********\n");
}

static int alloc_memory(size_t asize)
{
    size_t num_bytes = asize > (alloc_space + dsize) ? asize : (alloc_space + dsize);
    // alloc_space <<= 1;  //扩容采用容量加倍策略
    void *p = mem_sbrk(num_bytes);

    write_header(p, num_bytes, 0);
    write_footer(p, num_bytes, 0);
    
    heap_end = find_next(p);
    write_header(heap_end, sizeof(word_t), 1);

    return 1;
}

static block_t *find_next(block_t *block)
{
    return (block_t*) ((unsigned char*) block  + get_size(block));
}

static block_t *find_prev(block_t *block)
{
    word_t *prev_footer = find_prev_footer(block);
    size_t prev_asize = (*prev_footer & ~0x1L);

    return (block_t*) ((unsigned char*) block - prev_asize);
}

static block_t *find_fit(size_t asize)
{
    block_t *block;

    for (block = heap_start; block != heap_end; block = find_next(block))
    {
        if (!get_alloc(block) && (asize <= get_size(block)))
        {
            return block;
        }
    }

    return NULL;
}

static void write_header(block_t *block, size_t size, int alloc)
{
    block->header = (size | alloc);
}

static void write_footer(block_t *block, size_t size, int alloc)
{
    word_t *footer = header_to_footer(block);
    *footer = (size | alloc);
}

static block_t* payload_to_header(void *ptr)
{
    return (block_t *) ((unsigned char *)ptr - offsetof(block_t, payload));
}

static void split_block(block_t *block, size_t asize)
{
    size_t block_size = get_size(block);

    if ((block_size - asize) >= min_block_size)
    {
        write_header(block, asize, 1);
        write_footer(block, asize, 1);
        block_t *block_next = find_next(block);
        write_header(block_next, block_size - asize, 0);
        write_footer(block_next, block_size - asize, 0);
    }
}

static void coalesce_block(block_t *block)
{
    block_t *prev = find_prev(block);
    block_t *next = find_next(block);

    int prev_alloc = get_alloc(prev);
    int next_alloc = get_alloc(next);
    size_t prev_asize = get_size(prev);
    size_t next_asize = get_size(next);
    size_t curr_asize = get_size(block);

    if (!prev_alloc && !next_alloc)
    {
        size_t s = prev_asize + next_asize + curr_asize; 
        write_header(prev, s, 0);
        write_footer(prev, s, 0);
    }
    else if (!prev_alloc)
    {
        size_t s = prev_asize + curr_asize;
        write_header(prev, s, 0);
        write_footer(prev, s, 0);
    }
    else if (!next_alloc)
    {
        size_t s = curr_asize + next_asize;
        write_header(block, s, 0);
        write_footer(block, s, 0);
    }
    
    return;
}

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    void *p = mem_sbrk(alloc_space);
    heap_start = (block_t *)p;

    write_header(heap_start, sizeof(word_t), 1);

    block_t *ptr = find_next(heap_start);
    write_header(ptr, alloc_space - dsize, 0);
    write_footer(ptr, alloc_space - dsize, 0);

    heap_end = find_next(ptr);
    write_header(heap_end, sizeof(word_t), 1);

    // print_heap();

    return 1;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    // printf("\nmalloc size : %d\n", size);
    
    size_t asize = ALIGN(size) + dsize;

    block_t *block = find_fit(asize);
    if (block == NULL)
    {
        alloc_memory(asize);
        block = find_fit(asize);
        if (block == NULL)
            return NULL;
    }
    
    size_t block_size = get_size(block);
    write_header(block, block_size, 1);
    write_footer(block, block_size, 1);

    split_block(block, asize);

    return get_payload(block);
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    block_t *block = payload_to_header(ptr);
    size_t size = get_size(block);

    write_header(block, size, 0);
    write_footer(block, size, 0);
    
    coalesce_block(block);
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;
    
    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL;
    copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
    if (size < copySize)
      copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}
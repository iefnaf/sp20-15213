/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * v2.0
 * Explicit free lists, first fit strategy
 * 
 *                        Free blocks                   Allocated blocks
 *                     ----------------                -----------------
 *                     .    header    .                .     header    .
 *    prev->next-->    .    next      .                .     payload   .
 *                     .    prev      .                .               .
 *                     .              .                .               .
 *                     .              .                .               .
 *                     .              .                .               .
 *                     .    footer    .                .     footer    .
 *                     ----------------                -----------------
 *
 *      the next | prev pointer points to the next | prev free block's next field(the word after the header).
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

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
    "gaofanfei2020@163.com",
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

#define WSIZE 4
#define DSIZE 8
#define CHUNKSIZE (1<<12) /* Extend heap by this amount (bytes) */
#define MINBLOCKSIZE 16

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define PACK(size, alloc) ((size) | (alloc)) /* Pack a size and allocated bit into a word */

/* Read and write a word at address p */
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))
/* Read the size and the alloc field field from address p */
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)
/* Given block ptr bp, compute address of its header and footer*/
#define HDRP(bp) ((char*)(bp) - WSIZE)
#define FTRP(bp) ((char*)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)
/* Given block ptr bp, compute address of next and prev blocks */
#define NEXT_BLKP(bp) ((char*)(bp) + GET_SIZE(HDRP(bp)))
#define PREV_BLKP(bp) ((char*)(bp) - GET_SIZE((char*)(bp)-DSIZE))
/* Get and set prev or next pointer from address p */
#define GET_PREV(p) (*(unsigned int *)(p))
#define SET_PREV(p, prev) (*(unsigned int *)(p) = (prev))
#define GET_NEXT(p) (*((unsigned int *)(p)+1))
#define SET_NEXT(p, val) (*((unsigned int *)(p)+1) = (val))

static char* heap_listp;
static char* free_list_head;

static void* extend_heap(size_t words);
static void* coalesce(void *bp);
static void* first_fit(size_t asize);
static void* find_fit(size_t asize);
static void place(void* bp, size_t asize);
int mm_init(void);
void *mm_malloc(size_t size);
void mm_free(void *bp);
void *mm_realloc(void *ptr, size_t size);

/*
 * remove_from_free_list - remove the block from free list
 * if there is only one block, set free_list_head = NULL
 */ 
static void remove_from_free_list(void *bp)
{
    if (bp == NULL || GET_ALLOC(HDRP(bp)))
        return;
    
    void* prev = GET_PREV(bp);
    void* next = GET_NEXT(bp);

    SET_PREV(bp, 0);
    SET_NEXT(bp, 0);

    if (prev == NULL && next == NULL)
    {
        free_list_head = NULL;
    }
    else if (prev == NULL)
    {
        SET_PREV(next, 0);
        free_list_head = next;
    }
    else if (next == NULL)
    {
        SET_NEXT(prev, 0);
    }
    else
    {
        SET_NEXT(prev, next);
        SET_PREV(next, prev);
    } 
}

/* 
 * insert_to_free_list - insert block bp at the head of the free list
 */
static void insert_to_free_list(void* bp)
{
    if (bp == NULL)
        return;
    
    if (free_list_head == NULL)
    {
        free_list_head = bp;
        return;
    }
    
    SET_NEXT(bp, free_list_head);
    SET_PREV(free_list_head, bp);
    free_list_head = bp;
}

/*
 * extend_heap - extend heap by words * word(4 bytes)
 */
static void* extend_heap(size_t words)
{
    char* bp;
    size_t size;

    /* Allocate an even number of words to maintain alignment */
    size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;
    if ((long)(bp = mem_sbrk(size)) == -1)
        return NULL;

    /* Initialize free block header/footer and the epilogue header */
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    SET_PREV(bp, 0);
    SET_NEXT(bp, 0);

    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));

    return coalesce(bp); // coalesce if the previous block is free.
}

/*
 * coalesce - merge adjacent empty blocks.
 * 
 */
static void* coalesce(void *bp)
{
    void* prev_bp = PREV_BLKP(bp);
    void* next_bp = NEXT_BLKP(bp);
    size_t prev_alloc = GET_ALLOC(FTRP(prev_bp));
    size_t next_alloc = GET_ALLOC(HDRP(next_bp));

    size_t size = GET_SIZE(HDRP(bp));

    if (prev_alloc && next_alloc)
    {

    }
    else if (prev_alloc && !next_alloc)
    {
        remove_from_free_list(next_bp);
        size += GET_SIZE(HDRP(next_bp));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
    }
    else if (!prev_alloc && next_alloc)
    {
        remove_from_free_list(prev_bp);
        size += GET_SIZE(HDRP(prev_bp));
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(prev_bp), PACK(size, 0));

        bp = prev_bp;
    }
    else
    {
        remove_from_free_list(prev_bp);
        remove_from_free_list(next_bp);
        size += GET_SIZE(HDRP(prev_bp)) + GET_SIZE(FTRP(next_bp));
        PUT(HDRP(prev_bp), PACK(size, 0));
        PUT(FTRP(next_bp), PACK(size, 0));

        bp = prev_bp;
    }
    insert_to_free_list(bp);
    return bp;
}

/*
 * first_fit - use first fit strategy to find an empty block.
 */
static void* first_fit(size_t asize)
{
    for (void* bp = free_list_head; bp != 0; bp = GET_NEXT(bp))
    {
        if (GET_SIZE(HDRP(bp)) >= asize)
        {
            return bp;
        }
    }
    return NULL;
}

static void* find_fit(size_t asize)
{
    return first_fit(asize);
}

/*
 * place - remove the block bp from free list,
 *         and only split if the remaining part is equal to or larger than the size of the smallest block
 */
static void place(void* bp, size_t asize)
{
    size_t size = GET_SIZE(HDRP(bp));
    remove_from_free_list(bp);

    if ((size - asize) >= MINBLOCKSIZE) // split block
    {
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));
        void* new_bp = NEXT_BLKP(bp);
        PUT(HDRP(new_bp), PACK(size-asize, 0));
        PUT(FTRP(new_bp), PACK(size-asize, 0));
        SET_PREV(new_bp, 0);
        SET_NEXT(new_bp, 0);
        coalesce(new_bp);
    }
    else // do not split
    {
        PUT(HDRP(bp), PACK(size, 1));
        PUT(FTRP(bp), PACK(size, 1));
    }
}

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    if ((heap_listp = mem_sbrk(4*WSIZE)) == (void*)-1)
        return -1;
    PUT(heap_listp, 0); // alignment padding
    PUT(heap_listp + (1 * WSIZE), PACK(DSIZE, 1)); //prologue header
    PUT(heap_listp + (2 * WSIZE), PACK(DSIZE, 1)); //prologue footer
    PUT(heap_listp + (3 * WSIZE), PACK(0, 1)); //epilogue header
    heap_listp += (2 * WSIZE);
    free_list_head = NULL;

    if (extend_heap(CHUNKSIZE/WSIZE) == NULL)
        return -1;
    return 0;
}

/* 
 * mm_malloc - find a free block in free list ,if there is no one exists, exthed the heap 
 *
 */
void *mm_malloc(size_t size)
{
    size_t asize; // adjusted block size
    size_t extendsize; // amount to extend heap if no fit
    char* bp;

    if (size == 0)
        return NULL;

    // adjusted block size to include overhead and alignment reqs
    if (size <= DSIZE)
        asize = 2*DSIZE;
    else
        asize = DSIZE * ((size + (DSIZE) + (DSIZE - 1)) / DSIZE);
    
    if ((bp = find_fit(asize)) != NULL)
    {
        place(bp, asize);
        return bp;
    }

    // no fit found, get more memory and place the block
    extendsize = MAX(asize, CHUNKSIZE);
    if ((bp = extend_heap(extendsize / WSIZE)) == NULL)
        return NULL;
    place(bp, asize);
    return bp;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *bp)
{
    if (bp == NULL)
        return;
    size_t size = GET_SIZE(HDRP(bp));

    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    SET_PREV(bp, 0);
    SET_NEXT(bp, 0);
    coalesce(bp);
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
    size = GET_SIZE(HDRP(oldptr));
    copySize = GET_SIZE(HDRP(newptr));
    if (size < copySize)
      copySize = size;
    memcpy(newptr, oldptr, copySize-WSIZE);
    mm_free(oldptr);
    return newptr;
}
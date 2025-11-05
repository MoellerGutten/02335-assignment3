/**
* @file   mm.c
* @Author 02335 team
* @date   September, 2024
* @brief  Memory management skeleton.
* 
*/

#include <stdint.h>

#include "mm.h"



/* Proposed data structure elements */

typedef struct header {
	struct header * next;     // Bit 0 is used to indicate free block 
	uint64_t user_block[0];   // Standard trick: Empty array to make sure start of user block is aligned
} BlockHeader;

/* Macros to handle the free flag at bit 0 of the next pointer of header pointed at by p */
#define GET_NEXT(p)    (void *) ((uintptr_t) (p->next) & -2)    /* Here we mask out flag bit by using -2 = 0xFFF...FE which is sign extended to the length of pointer*/
#define SET_NEXT(p,n)  p->next = (void *) ((GET_FREE(p) | ( (uintptr_t) n & -2))) /* Here we essentially flag + next (with ls bit cleared), so flag is preserved  */
#define GET_FREE(p)    (uint8_t) ( (uintptr_t) (p->next) & 0x1 )   /* OK -- do not change */
#define SET_FREE(p,f)  p->next = (void *) ((((uintptr_t) (p->next)) & -2 ) | (f & 1))  /*Here we mask out everything but the ls bit and add the ls bit of f to it (either 0 or 1)*/
#define SIZE(p)        (size_t) ( ((uintptr_t) GET_NEXT(p)) - ((uintptr_t) p) - sizeof(BlockHeader) ) /* using GET_NEXT(p) instead of p->next to make sure free flag doesn't interfere*/ 

#define MIN_SIZE     (8)   // A block should have at least 8 bytes available for the user


static BlockHeader * first = NULL;
static BlockHeader * current = NULL;

/**
* @name    simple_init
* @brief   Initialize the block structure within the available memory
*
*/
void simple_init() {
	// UNTESTED !!! added " + start % MIN_SIZE (8)" to align
	uintptr_t aligned_memory_start = memory_start % MIN_SIZE == 0 ? memory_start : memory_start - memory_start % MIN_SIZE + MIN_SIZE;  /* TODO: Alignment */
	// UNTESTED !!! added " - end % MIN_SIZE (8)" to align. -1 since memory_end points to the first bit of memory we *can'* use
	uintptr_t aligned_memory_end = memory_end - memory_end % MIN_SIZE - 1;    /* TODO: Alignment */
	
	/* Already initalized ? */
	if (first == NULL) {
		/* Check that we have room for at least one free block and an end header */
		if (aligned_memory_start + 2*sizeof(BlockHeader) + MIN_SIZE <= aligned_memory_end) {
			/* TODO: Place first and last blocks and set links and free flags properly */
			
			// UNTESTED !!!
			first = (void *) aligned_memory_start;
			SET_FREE(first, 0);
			BlockHeader * last = (void *) (aligned_memory_end - sizeof(BlockHeader));
			SET_FREE(last, 1);
			SET_NEXT(first, last);
			SET_NEXT(last, first);
		}
		current = first;     
	} 
}


/**
* @name    simple_malloc
* @brief   Allocate at least size contiguous bytes of memory and return a pointer to the first byte.
*
* This function should behave similar to a normal malloc implementation. 
*
* @param size_t size Number of bytes to allocate.
* @retval Pointer to the start of the allocated memory or NULL if not possible.
*
*/
void* simple_malloc(size_t size) {
	if (first == NULL) {
		simple_init();
		if (first == NULL) return NULL;
	}
	
	// UNTESTED !!! added the expression below to align
	size_t aligned_size = size % MIN_SIZE == 0 ? size : size - size % MIN_SIZE + MIN_SIZE;  /* TODO: Alignment */
	
	/* Search for a free block */
	BlockHeader * search_start = current;
	BlockHeader * temp = NULL;
	do {
		
		if (GET_FREE(current)) {
			
			/* Possibly coalesce consecutive free blocks here */
			
			/* Check if free block is large enough */
			if (SIZE(current) >= aligned_size) {
				/* Will the remainder be large enough for a new block? */
				if (SIZE(current) - aligned_size < sizeof(BlockHeader) + MIN_SIZE) {
					/* TODO: Use block as is, marking it non-free*/
					// UNTESTED !!!I
					SET_FREE(current, 1);
					temp = current;
				} else {
					/* TODO: Carve aligned_size from block and allocate new free block for the rest */
					// UNTESTED !!!
					SET_FREE(current, 1);
					SET_NEXT(temp, GET_NEXT(current));
					SET_NEXT(current, (current + aligned_size + sizeof(BlockHeader)));
					temp = current;
				}
				// UNTESTED !!!
				current = GET_NEXT(current);
				return (void *) (temp + sizeof(BlockHeader)); /* TODO: Return address of current's user_block and advance current */
			}
		}
		
		current = GET_NEXT(current);
	} while (current != search_start);
	
	/* None found */
	return NULL;
}


/**
* @name    simple_free
* @brief   Frees previously allocated memory and makes it available for subsequent calls to simple_malloc
*
* This function should behave similar to a normal free implementation. 
*
* @param void *ptr Pointer to the memory to free.
*
*/
void simple_free(void * ptr) {
	// UNTESTED !!!
	BlockHeader * block = ptr; /* TODO: Find block corresponding to ptr */
	if (GET_FREE(block)) {
		/* Block is not in use -- probably an error */
		return;
	}
	
	/* TODO: Free block */
	SET_FREE(block, 0);
	
	/* Possibly coalesce consecutive free blocks here */
}


/* Include test routines */

#include "mm_aux.c"

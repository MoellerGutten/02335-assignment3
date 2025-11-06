/**
 * @file   mm.c
 * @Author 02335 team
 * @date   September, 2024
 * @brief  Memory management skeleton.
 *
 */

#include <stdint.h>
#include <stdlib.h>
#include "mm.h"

/* Proposed data structure elements */

typedef struct header {
	struct header *next;	// Bit 0 is used to indicate free block
	uint64_t user_block[0]; // Standard trick: Empty array to make sure start of user block is aligned
} BlockHeader;

/* Macros to handle the free flag at bit 0 of the next pointer of header pointed at by p */

/* Here we mask out flag bit by using -2 = 0xFFF...FE which is sign extended to the length of pointer*/
#define GET_NEXT(p) (void *)((uintptr_t)(p->next) & -2)

/* Here we essentially flag + next (with ls bit cleared), so flag is preserved  */
#define SET_NEXT(p, n) p->next = (void *)((GET_FREE(p) | ((uintptr_t)n & -2)))

/* OK -- do not change */
#define GET_FREE(p) (uint8_t)((uintptr_t)(p->next) & 0x1)

/*Here we mask out everything but the ls bit and add the ls bit of f to it (either 0 or 1) */
#define SET_FREE(p, f) p->next = (void *)(((uintptr_t)(p->next) & -2) | (f & 0x1))

/* using GET_NEXT(p) instead of p->next to make sure free flag doesn't interfere*/
#define SIZE(p) (size_t)(((uintptr_t)GET_NEXT(p)) - ((uintptr_t)p) - sizeof(BlockHeader))



#define MIN_SIZE (8) // A block should have at least 8 bytes available for the user
#define FREE (0)
#define ALLOCATED (1)

static BlockHeader *first = NULL;
static BlockHeader *current = NULL;

/**
 * @name    simple_init
 * @brief   Initialize the block structure within the available memory
 *
 */
void simple_init() {
	// Aligns memory start and end
	uintptr_t aligned_memory_start = memory_start % MIN_SIZE == 0 ? memory_start : memory_start - memory_start % MIN_SIZE + MIN_SIZE; 
	uintptr_t aligned_memory_end = memory_end - memory_end % MIN_SIZE;

	/* Already initialized ? */
	if (first == NULL) {
		/* Check that we have room for at least one free block and an end header */
		if (aligned_memory_start + 2 * sizeof(BlockHeader) + MIN_SIZE <= aligned_memory_end) {
			// Linking First and last (dummy) headers together before malloc
			first = (void *)aligned_memory_start;
			SET_FREE(first, FREE);
			BlockHeader *last = (void *)((char *)aligned_memory_end - sizeof(BlockHeader));
			SET_FREE(last, ALLOCATED);
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
void *simple_malloc(size_t size) {
	if (first == NULL) {
		simple_init();
		if (first == NULL)
			return NULL;
	}

	// Aligns (pads) the size of the given memory block
	size_t aligned_size = size % MIN_SIZE == 0 ? size : size - size % MIN_SIZE + MIN_SIZE; 

	/* Search for a free block */
	BlockHeader *search_start = current;
	BlockHeader *tempNext = NULL;
	BlockHeader *returnAddr = NULL;

	do {
		int freebit = GET_FREE(current);
		if (freebit == FREE) {
			/* Coalesces consecutive free blocks here */
			BlockHeader *next = GET_NEXT(current);
			BlockHeader *last_next = next;

			while (GET_FREE(next) == FREE && next != current) {
				last_next = next;
				next = GET_NEXT(next);
			}
			SET_NEXT(current, last_next);

			/* Check if free block is large enough */
			if (SIZE(current) >= aligned_size) {
				/* Will the remainder be large enough for a new block? */
				if (SIZE(current) - aligned_size < sizeof(BlockHeader) + MIN_SIZE) {
					// Just enough space between headers to insert block. The memory < 16 bytes will be included as padding
					returnAddr = current;
					SET_FREE(current, ALLOCATED);
					current = GET_NEXT(current);
				}
				else {
					/*Carves aligned_size from block and allocate new free block for the rest */
					tempNext = GET_NEXT(current);
					SET_NEXT(current, ((char *)current + aligned_size + sizeof(BlockHeader)));
					SET_FREE(current, ALLOCATED);
					returnAddr = current;
					current = GET_NEXT(current);
					SET_FREE(current, FREE);	
					SET_NEXT(current, tempNext);
				}

				return (void *)(returnAddr->user_block); /* Returns address of current's user_block and advance current */
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
void simple_free(void *ptr) {
	if (ptr == NULL)
		return;

	BlockHeader *block = (BlockHeader *)ptr - 1;

	if (GET_FREE(block) == FREE) {
		/* Block is not in use -- probably an error */
		return;
	}
	uintptr_t aligned_memory_end = memory_end - memory_end % MIN_SIZE;
	BlockHeader *last = (void *)((char *)aligned_memory_end - sizeof(BlockHeader));

	// Should not free the dummy block, so returns
	if (block == last) {
		return;
	}

	// frees block
	SET_FREE(block, FREE);
}

/* Include test routines */
#include "mm_aux.c"

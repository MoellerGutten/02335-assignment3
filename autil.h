typedef struct {
    int *array;
    int index;
    int size;
} Array;

/**
 * Initialize an empty array of default size.
 */
void init_arr(Array *arr);

/**
 * Insert an element into an array.
 */
void insert_arr(Array *arr, int element);

/**
 * Deallocate an array.
 */
void free_arr(Array *arr);

/**
 * Print the contents of an array.
 */
void print_arr(Array *arr);

/**
 * Remove the last added element of an array.
 */
void remove_element(Array *arr);
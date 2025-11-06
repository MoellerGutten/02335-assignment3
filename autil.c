#include "autil.h"
#include "stdlib.h"
#include "io.h"
#include "mm.h"
#define INITIAL_ARRAY_SIZE 10

void init_arr(Array *arr) {
    arr->array = simple_malloc(INITIAL_ARRAY_SIZE * sizeof(int));
    arr->index = 0;
    arr->size = INITIAL_ARRAY_SIZE;
}

void insert_arr(Array *arr, int element) {
    // if array is full
    if (arr->index == arr->size) {
        // double array size and reallocate
        // CHANGES: Manually copy old values into the new array, since we have not implemented a realloc(..) function
        int oldvalues[arr->size];
        arr->size *= 2;
        int *new_arr = simple_malloc(arr->size * sizeof(int));
        for (int i = 0; i < arr->index; i++) {
            oldvalues[i] = arr->array[i];
        }
        if (new_arr == NULL) {
            write_string("realloc failed at insert_arr, exiting...");
            exit(1);
        }
        arr->array = new_arr;
        for (int i = 0; i < arr->index; i++) {
            arr->array[i] = oldvalues[i];
        }
    }

    // add element and increment index
    arr->array[arr->index++] = element;
}

void free_arr(Array *arr) {
    simple_free(arr->array);
    arr->array = NULL;
    arr->index = 0;
    arr->size = 0;
}

void remove_element(Array *arr) {
    // do nothing if array is empty
    if (arr->index == 0) {
        return;
    }

    // remove last element
    arr->index--;

    // if array is relatively empty and has previously been increased in size, we decrease the size
    if (arr->index <= arr->size / 3 && arr->size > INITIAL_ARRAY_SIZE) {
        arr->size = arr->size / 2;
        int *new_arr = simple_malloc(arr->size * sizeof(int));
        if (new_arr == NULL) {
            write_string("realloc failed at remove_element, exiting...");
            exit(1);
        }
        arr->array = new_arr;
    }
}

void print_arr(Array *arr) {
    for (int i = 0; i < arr->index; i++) {
        if (i != 0) {
            write_char(',');
        }
        write_int(arr->array[i]);
    }
}

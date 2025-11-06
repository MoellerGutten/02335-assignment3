/* You are not allowed to use <stdio.h> */
#include "io.h"
#include "termios.h"
#include "unistd.h"
#include "stdlib.h"
#include "autil.h"
#include "mm.h"

/**
 * Handle the 'a' command.
 */
void handle_a(Array *arr);

/**
 * Handle the 'b' command.
 */
void handle_b();

/**
 * Handle the 'c' command.
 */
void handle_c(Array *arr);

/**
 * Increment the counter.
 */
void increment_counter();

/**
 * Gracefully handle quitting the program
 */
void handle_quit(Array *arr);

/**
 * Counts commands sent to the program.
 */
int counter;

/**
 * @name  main
 * @brief This function is the entry point to your program
 * @return 0 for success, anything else for failure
 *
 *
 * Then it has a place for you to implementation the command
 * interpreter as  specified in the handout.
 */
int main() {
    // initialization
    counter = 0;
    Array array;
    init_arr(&array);

    while (1) {
        char c = read_char();
        if (c == 'a') {
            handle_a(&array);
        } else if (c == 'b') {
            handle_b();
        } else if (c == 'c') {
            handle_c(&array);
        } else {
            handle_quit(&array);
            break; // exit
        }
    }
    return 0;
}

void handle_a(Array *arr) {
    insert_arr(arr, counter);
    increment_counter();
}

void handle_b() {
    increment_counter();
}

void handle_c(Array *arr) {
    remove_element(arr);
    increment_counter();
}

void increment_counter() {
    counter++;
}

void handle_quit(Array *arr) {
    print_arr(arr);
    write_char(';');
    free_arr(arr);
}

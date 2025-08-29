/**
 * @file main.c
 * @brief Demonstration and test suite for the dArray dynamic array library.
 *
 * === HOW TO USE ===
 * To test a different data type, change the TEST_TYPE macro below to
 * one of the valid options: TYPE_INT, TYPE_FLOAT, or TYPE_DOUBLE.
 */

#include <stdio.h>
#include <stdlib.h>
#include "darray.h" // Including your library header

// ===========================================================================
// == CONFIGURATION (VERSÃO CORRIGIDA)                                      ==
// ===========================================================================
// 1. Damos um "apelido" numérico para cada tipo de teste
#define TYPE_INT    0
#define TYPE_FLOAT  1
#define TYPE_DOUBLE 2

// 2. AGORA, você escolhe o tipo que quer testar usando o NÚMERO/APELIDO
#define TEST_TYPE   TYPE_DOUBLE // <-- Altere aqui para TYPE_INT ou TYPE_DOUBLE
// ===========================================================================


// --- Preprocessor logic that now works correctly ---
#if TEST_TYPE == TYPE_INT
    typedef int test_t;
    #define API_TYPE INT
    #define FMT_STR "%d"
    #define TEST_VALUES(i) ((test_t)(i + 1) * 10)
    #define VAL_99 99
    #define VAL_55 55
    #define VAL_50 50
#elif TEST_TYPE == TYPE_FLOAT
    typedef float test_t;
    #define API_TYPE FLOAT
    #define FMT_STR "%.2f"
    #define TEST_VALUES(i) ((test_t)(i + 1) * 10.5f)
    #define VAL_99 99.0f
    #define VAL_55 55.0f
    #define VAL_50 50.0f
#elif TEST_TYPE == TYPE_DOUBLE
    typedef double test_t;
    #define API_TYPE DOUBLE
    #define FMT_STR "%.2lf"
    #define TEST_VALUES(i) ((test_t)(i + 1) * 10.5)
    #define VAL_99 99.0
    #define VAL_55 55.0
    #define VAL_50 50.0
#else
    #error "Invalid TEST_TYPE defined. Use TYPE_INT, TYPE_FLOAT, or TYPE_DOUBLE."
#endif


// O resto do seu arquivo main.c (print_array, run_test_suite, etc.) continua igual...
// ...

// --- Test Helper Functions ---

/**
 * @brief A helper function to print the state and contents of the array.
 */
void print_array(const char* title, dArray* array) {
    printf("--- %s ---\n", title);
    if (!array) {
        printf("Array is NULL.\n\n");
        return;
    }

    printf("Capacity: %zu | In Use: %zu\n", array_get_capacity(array), array_get_size(array));
    printf("Contents: [ ");
    for (size_t i = 0; i < array_get_size(array); i++) {
        test_t value;
        if (array_get(array, i, &value)) {
            printf(FMT_STR " ", value);
        }
    }
    printf("]\n\n");
}

/**
 * @brief Main function that runs all tests for the configured type.
 */
void run_test_suite() {
    printf("\n>>> STARTING TESTS FOR DYNAMIC ARRAY <<<\n\n");

    printf("1. Creating a new array with capacity for 5 elements...\n");
    dArray* array = array_new(API_TYPE, 5);
    print_array("Initial State", array);

    printf("2. Checking if the array is empty...\n");
    printf("Is array empty? %s\n\n", array_is_empty(array) ? "Yes" : "No");

    printf("3. Appending 7 elements...\n");
    for (int i = 0; i < 7; i++) {
        test_t val = TEST_VALUES(i);
        array_append(array, &val);
    }
    print_array("After appending 7 elements", array);
    
    printf("4. Testing get/set at index 3...\n");
    test_t got_value;
    array_get(array, 3, &got_value);
    printf("Value at index 3 is: " FMT_STR "\n", got_value);
    test_t new_value_for_set = VAL_99;
    printf("Changing value at index 3 to " FMT_STR "...\n", new_value_for_set);
    array_set(array, 3, &new_value_for_set);
    print_array("After set", array);

    printf("5. Popping the last element...\n");
    test_t popped_value;
    if (array_pop(array, &popped_value)) {
        printf("Popped value: " FMT_STR "\n", popped_value);
    }
    print_array("After pop", array);

    printf("6. Inserting value " FMT_STR " at index 2...\n", (test_t)VAL_55);
    test_t value_to_insert = VAL_55;
    array_insert(array, 2, &value_to_insert);
    print_array("After insert", array);

    printf("7. Removing element at index 0...\n");
    array_remove_by_index(array, 0);
    print_array("After removing index 0", array);
    
    printf("8. Removing element with value " FMT_STR "...\n", (test_t)VAL_50);
    test_t value_to_remove = VAL_50;
    array_remove_by_value(array, &value_to_remove);
    print_array("After removing value 50", array);

    printf("9. Finding the index of value " FMT_STR "...\n", (test_t)VAL_99);
    size_t found_index;
    test_t value_to_find = VAL_99;
    if (array_find(array, &value_to_find, &found_index)) {
        printf("Value " FMT_STR " found at index %zu.\n\n", value_to_find, found_index);
    }

    printf("10. Reversing the array...\n");
    array_reverse(array);
    print_array("After reverse", array);

    printf("11. Sorting the array...\n");
    array_sort(array);
    print_array("After sort", array);
    
    printf("12. Searching for value " FMT_STR " with binary search...\n", (test_t)VAL_55);
    size_t binary_search_index;
    if (array_binary_search(array, &value_to_insert, &binary_search_index, true)) {
        printf("Value " FMT_STR " found at index: %zu\n", value_to_insert, binary_search_index);
    } else {
        printf("Value " FMT_STR " not found!\n", value_to_insert);
    }
    printf("\n");

    printf("13. Shrinking the array to fit its contents...\n");
    array_shrink(array);
    print_array("After shrink", array);

    printf("14. Clearing the array...\n");
    array_clear(array);
    print_array("After clear", array);

    printf("15. Deleting the array...\n");
    array_delete(&array);
    print_array("After delete", array);
}


// --- Main Function ---
int main(void) {
    printf("=======================================\n");
    printf("==   STARTING DARRAY LIBRARY TEST    ==\n");
    printf("=======================================\n");

    run_test_suite();

    printf("\n=======================================\n");
    printf("==      ALL TESTS COMPLETED          ==\n");
    printf("=======================================\n");

    return 0;
}
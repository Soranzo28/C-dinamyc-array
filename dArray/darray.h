#ifndef DARRAY_H
#define DARRAY_H

#include <stdlib.h>
#include <stdbool.h>

typedef enum {INT, FLOAT, DOUBLE} var_types;
typedef struct dArray dArray; 

//Public functions
dArray* array_new(var_types type, size_t start_size);
bool array_append(dArray* array, void* new_element);
bool array_delete(dArray** array);
bool array_pop(dArray* array, void* store_var);
bool array_remove_by_value(dArray* array, void* value);
bool array_remove_by_index(dArray* array, size_t index);
bool array_get(dArray* array, size_t index, void* store_variable);
bool array_set(dArray* array, size_t index, void* new_value);
bool array_is_empty(dArray* array);
void array_clear(dArray* array);
bool array_insert(dArray* array, size_t index, void* new_value);
bool array_shrink(dArray* array);
bool array_find(dArray* array, void* value, size_t* store_index);
void array_sort(dArray* array);
bool array_reverse(dArray* array);
bool array_binary_search(dArray* array, void* element, bool already_sorted);
#endif
#include "darray.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * @brief This is the main structure to control a dynamic array
 * 
 * This struct stores all the necessary information for manipulation and operations.
 * Those fields should not be directly acessed, instead use the API functions. 
 * @see array_new()
 * @see array_destroy()
 */
struct dArray {
    void* dArray;      ///< Generic pointer to the array on the heap
    size_t total_size; ///< Total capacity allocated (how many elements fits in)
    size_t used_size;  ///< How many elements actually exists 
    var_types type;    ///< The variable type that this array stores (INT, FLOAT, DOUBLE)
};

//Private functions declaration
static bool array_realloc(dArray* array);
static int compare_int(const void* a, const void* b);
static int compare_float(const void* a, const void* b);
static int compare_double(const void* a, const void* b);
static size_t get_type_size(var_types type);

//Public functions implementation


/**
 * @brief Creates a new dynamic array
 * @param[in] type       The type that the array will store
 * @param[in] start_size The total_size that the array will begin with @see struct dArray
 * @return               A dArray pointer to your new fresh dynamic array
 */
dArray* array_new(var_types type, size_t start_size){
    if (start_size == 0){
        fprintf(stderr, "ERROR! Size must be bigger than 0!\n");
        return NULL;
    }

    dArray* new_array = malloc(sizeof(dArray));
    if (!new_array){
        fprintf(stderr, "ERROR! Failed to allocate memory!\n");
        return NULL;
    }

    size_t type_size = get_type_size(type);
    if (type_size == 0){
        fprintf(stderr, "ERROR! Unable to allocate a new array!\n");
        free(new_array);
        return NULL;
    }
    new_array->dArray = malloc(start_size * type_size);
    if (!new_array->dArray){
        fprintf(stderr, "ERROR! Failed to allocate memory for the data buffer!\n");
        free(new_array);
        return NULL;
    }
    new_array->total_size = start_size;
    new_array->used_size = 0;
    new_array->type = type;
    return new_array;
}

/**
 * @brief Appends a new element to the end of the list
 * 
 * @param[in,out] array       The target array
 * @param[in]     new_element The element that will be appended
 */
bool array_append(dArray* array, void* new_element){
    if (array_is_full(array)){
        if (!array_realloc(array)){return false;}
    }
    void* pointer;
    switch (array->type){
        case INT:
            pointer = array->dArray;
            ((int*)pointer)[array->used_size] = *(int*)new_element;
            break;
        case FLOAT:
            pointer = array->dArray;
            ((float*)pointer)[array->used_size] = *(float*)new_element;
            break;
        case DOUBLE:
            pointer = array->dArray;
            ((double*)pointer)[array->used_size] = *(double*)new_element;
            break;
    }
    array->used_size++;
    return true;
}

/**
 * @brief Destroy the array
 * 
 * @param[in,out] array The target array
 * @return              True if success or false if the array does not exist
 */
bool array_delete(dArray** array){
    if (!array || !*array){
        fprintf(stderr, "The array does not exist!\n");
        return false;
    }

    free((*array)->dArray);
    (*array)->dArray = NULL;
    free(*array);
    *array = NULL;
    return true;
}

/**
 * @brief Remove the last element and stores it on store_var
 * 
 * @param[in,out] array     The target array
 * @param[out]    store_var A pointer to the variable that will store the popped value.
 * @returns                 True if popped successfully, false if there is not any elements to be popped
 */
bool array_pop(dArray* array, void* store_var){
    if (array->used_size == 0){
        fprintf(stderr, "ERROR! The list does not have any elements!\n");
        return false;
    }

    array->used_size--;
    size_t type_size = get_type_size(array->type);
    void* source = ((char*)array->dArray) + array->used_size * type_size;

    memcpy(store_var, source, type_size);
    return true;
}

/**
 * @brief Removes the first occurrence of specified element
 * 
 * @param[in,out] array The target array
 * @param[in]     value The value that will be removed
 * @returns             True if success, false if the element was not found
 */
bool array_remove_by_value(dArray* array, void* value){
    size_t type_size = get_type_size(array->type);
    int found_index = -1;
    void* pointer = array->dArray;
    for (int i=0; i < array->used_size; i++){
        switch(array->type){
            case INT:
                if (((int*)pointer)[i] == *(int*)value){
                    found_index = i;
                    break;
                }
                break;
            case FLOAT:
                if (((float*)pointer)[i] == *(float*)value){
                    found_index = i;
                    break;
                } 
                break;
            case DOUBLE:
                if (((double*)pointer)[i] == *(double*)value){
                    found_index = i;
                    break;
                }
                break;
        }
        if (found_index != -1){
            break;
        }
    }

    if (found_index == -1){
        fprintf(stderr, "ERROR! Element not found!\n");
        return false;
    }

    for (int i = found_index; i < array->used_size-1; i ++){
        switch (array->type){
            case INT:
                ((int*)pointer)[i] = ((int*)pointer)[i+1];
                break;
            case FLOAT:
                ((float*)pointer)[i] = ((float*)pointer)[i+1];
                break;
            case DOUBLE:
                ((double*)pointer)[i] = ((double*)pointer)[i+1];
                break;

        }
    }
    array->used_size--;
    return true;
}

/**
 * @brief Removes the element at specified index
 * 
 * @param[in,out] array The target array
 * @param[in]     index The index
 * @return              True if success, false if index is out of range.
 */
bool array_remove_by_index(dArray* array, size_t index){
    if (index >= array->used_size){
        fprintf(stderr, "ERROR! Index is out of range\n");
        return false;
    }
    size_t type_size = get_type_size(array->type);
    size_t bytes_to_move = (array->used_size -1 - index)*type_size;
    if (bytes_to_move > 0){
        void* destination = (char*)array->dArray + (index*type_size);
        void* source = (char*)array->dArray + (index+1) *type_size;
        memmove(destination, source, bytes_to_move);
    }
    array->used_size--;
    return true;
}

/**
 * @brief Gets the value at specified index and stores it at specified variable
 * 
 * @param[in]  array           The target array
 * @param[in]  index           The specified index
 * @param[out] store_variable  The variable that will store the value 
 * @return                     True if success, false if index is out of range
 */
bool array_get(dArray* array, size_t index, void* store_variable){
    if (index >= array->used_size){
        fprintf(stderr, "ERROR! Index out of range!\n");
        return false;
    }
    size_t type_size = get_type_size(array->type);
    void* temp_pointer = (char*)array->dArray + index*type_size;
    memcpy(store_variable, temp_pointer, type_size);
    return true;
}

/**
 * @brief Sets the value at specified index to specified value
 * 
 * @param[in,out] array     The target array
 * @param[in]     index     The specified index
 * @param[in]     new_value The value that will be stored at index
 * @return                  True if succes, false if index is out of range
 */
bool array_set(dArray* array, size_t index, void* new_value){
    if (index >= array->used_size){
        printf("ERROR! Index out of range!\n");
        return false;
    } 
    size_t type_size = get_type_size(array->type);
    void* temp_pointer = (char*)array->dArray + index*type_size;
    memcpy(temp_pointer, new_value, type_size);
    return true;
}

/**
 * @brief Checks if the array contains any elements
 * 
 * @param[in] array The target array
 * @return          True if is empty (used_size is 0), false if it is not
 */
bool array_is_empty(dArray* array){
    return array->used_size == 0 ? true : false; ///< Redudance ternary operator is used to make the code more readable
}

/**
 * @brief Clears the array (used_sized is set to 0)
 * @note This function does not free the allocated memory. It only resets the
 * count of used elements, making the array appear empty. The capacity
 * remains unchanged. Calling it on an already empty array is a safe no-op.
 * 
 * @param[in,out] array The target array to be cleared
 */
void array_clear(dArray* array){
    if (array){ ///< Checks if the array pointer itself is not NULL
        array->used_size = 0;
    }
}

/**
 * @brief Inserts a new value at specified index
 * @note This function pushes all the elements with index > specified index to the right
 * This may be slow depending on the size of your array.
 * 
 * @param[in,out] array     The target array
 * @param[in]     index     The specified index
 * @param[in]     new_value The new value that will be stored at index
 * @return True if success, false if index is out of range or memory allocation fail
 */
bool array_insert(dArray* array, size_t index, void* new_value){
    if (index > array->used_size){
        fprintf(stderr, "ERROR! Index out of range!\n");
        return false;
    }
    
    if (array->used_size + 1 >= array->total_size){ ///< Intencionally i decided to trigger the reallocation if there is only one space left, because yes.
        array_realloc(array);
    }

    size_t type_size = get_type_size(array->type);

    void    * source = (char*)array->dArray + index*type_size;
    void* destination = source+(type_size);
    size_t bytes_to_move = (array->used_size - index)* type_size;
    if (bytes_to_move > 0){
        memmove(destination, source, bytes_to_move);
    }
    memcpy(source, new_value, type_size);
    array->used_size++; 
    return true;
}
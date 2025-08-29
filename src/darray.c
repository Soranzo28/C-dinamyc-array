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
 * @see array_delete()
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
static bool array_is_full(dArray* array);

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
    int found_index = -1;
    void* pointer = array->dArray;
    for (int i=0; i < (int)array->used_size; i++){
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

    for (int i = found_index; i < (int)array->used_size-1; i ++){
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
        if (!array_realloc(array)){
            fprintf(stderr, "ERROR! Unable to reallocate the array!\n");
            return false;
        }
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

/**
 * @brief Shrinks the array's size to be equals to used_size
 * Realloc's the array size to used_size, making used_size == total_size
 * @param[in,out] array The target array
 * @return True if success, false if the array has no elements or its already shrinked
 */
bool array_shrink(dArray *array){
    if (array->used_size == 0){
        fprintf(stderr, "ERROR! The array have no elements!\n");
        return false;
    }
    if (array->used_size == array->total_size){
        fprintf(stderr, "ERROR! The array is already shrinked!\n");
        return false;
    }

    size_t type_size = get_type_size(array->type);
    void* temp = realloc(array->dArray, type_size*array->used_size);
    if (temp){
        array->dArray = temp;
        array->total_size = array->used_size;
        return true;
    }
    fprintf(stderr, "ERROR! Unable to shrink the array!\n");
    return false;
}

/**
 * @brief Find specified value and stores it's index at store_index
 * 
 * @param[in]  array       The target
 * @param[in]  value       The value that you want to find
 * @param[out] store_index The variable that will store the index 
 * @return True if success, false if element not found
 */
bool array_find(dArray* array, void* value, size_t* store_index){
    int found_index = -1;
    void* pointer = array->dArray;
    for (int i=0; i < (int)array->used_size; i++){
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
        printf("ERROR! Element not found!\n");
        return false;
    }
    *store_index = (size_t)found_index;
    return true;
}

/**
 * @brief Utilizes C built-in qsort to sort the array
 * 
 * @param[in,out] array The target array
 */
void array_sort(dArray* array){
    size_t type_size = get_type_size(array->type);
    switch(array->type){
        case INT:
            qsort(array->dArray, array->used_size, type_size, compare_int);
            return;
        case FLOAT:
            qsort(array->dArray, array->used_size, type_size, compare_float);
            return;
        case DOUBLE:
            qsort(array->dArray, array->used_size, type_size, compare_double);
            return;
    }
}
/**
 * @brief Reverses the elements on the array, trade the first for the last and so on
 * 
 * @param[in, out] array The target array
 * @return True if success, false if unable to allocate auxiliar variable
 */
bool array_reverse(dArray* array){
    char *header = NULL, *tail = NULL;
    size_t type_size = get_type_size(array->type);
    header = (char*)array->dArray;
    tail = (char*)array->dArray + (array->used_size-1) * type_size;
    void* temp = malloc(type_size);
    if (!temp){
        fprintf(stderr, "ERROR! Unable to allocate auxiliar variable!\n");
        return false;
    }
    while(1){
        memcpy(temp, tail, type_size);
        memcpy(tail, header, type_size);
        memcpy(header, temp, type_size);
        header += type_size;
        tail -= type_size;
        if (header == tail){
            break;
        }
        if (header+type_size == tail){
            memcpy(temp, tail, type_size);
            memcpy(tail, header, type_size);
            memcpy(header, temp, type_size);  
            break;
        }
    }
    free(temp);
    return true;
}


/**
 * @brief Utilizes binary search to find a specified element
 * @note It accepts a parameter that enable the function to sort the array for you
 * since this function can not work if the array is not sorted.
 * 
 * @param[in] array The target array
 * @param[in] element The element to be found
 * @param[out] store_index The variable that will store the found index
 * @param[in] already_sorted If false, it will use @see array_sort() to sort the array for you
 * @return True if found, false if not found
 */
bool array_binary_search(dArray* array, void* element, size_t* store_index, bool already_sorted){
    if (!already_sorted){
        array_sort(array);
    }
    size_t type_size = get_type_size(array->type);
    size_t middle_value = array->used_size / 2;
    void* middle_value_pointer = (char*)array->dArray + (middle_value*type_size);
    int header = 0, tail = array->used_size-1;
    int cmp;
    while(1){
        switch(array->type){
            case INT:
                cmp = compare_int(middle_value_pointer, element);
                break;
            case FLOAT:
                cmp = compare_float(middle_value_pointer, element);
                break;
            case DOUBLE:
                cmp = compare_double(middle_value_pointer, element);
                break;
        }

        if (cmp == 0){
            *store_index = middle_value;
            return true;
        }
        if (cmp > 0){
            tail = middle_value-1;
        }
        if (cmp < 0){
            header = middle_value+1;
        }
        if (tail < header){
            printf("NUMBER NOT FOUND!\n");
            return false;
        }
        middle_value = header + (tail-header)/2;
        middle_value_pointer = (char*)array->dArray + (middle_value*type_size);
    }
}

/**
 * @brief Get array->used_size
 * 
 * @param[in] array The target array
 * @return array->used_size , if array == NULL returns 0
 */
size_t array_get_size(const dArray* array) {
    return array ? array->used_size : 0;
}

/**
 * @brief Get array->total_size
 * 
 * @param[in] array The target array
 * @return array->total_size , if array == NULL returns 0
 */
size_t array_get_capacity(const dArray* array) {
    return array ? array->total_size : 0;
}

//Static function implementation

/**
 * @brief The actually mage here, this function reallocates the array based on it's size, grows 1.5x each time.
 * 
 * @param[in,out] array The target array
 */
static bool array_realloc(dArray* array){
    size_t new_size = array->total_size + (array->total_size >> 1);
    size_t type_size = get_type_size(array->type);

    if (new_size == array->total_size){
        new_size = array->total_size+1;
    }

    if (type_size == 0){
        fprintf(stderr, "ERROR! Unable to reallocate the array!\n");
        return false;
    }
    void* temp = realloc(array->dArray, new_size*type_size);
    if (temp){
        array->dArray = temp;;
        array->total_size = new_size;
        return true;
    }
    fprintf(stderr, "ERROR! Unable to reallocate a bigger array");
    return false;
}

/**
 * @brief Required function to qsort work, @see array_sort()
 * Compares the first number with the second one.
 * 
 * @param[in] a The first element
 * @param[in] b The second element
 * @return 1 if first is bigger, 0 if is equal and -1 if it's smaller
 */
int compare_int(const void* a, const void* b) {
   return (*(int*)a - *(int*)b);
}

/**
 * @brief Required function to qsort work, @see array_sort()
 * Compares the first number with the second one.
 * 
 * @param[in] a The first element
 * @param[in] b The second element
 * @return 1 if first is bigger, 0 if is equal and -1 if it's smaller
 */
int compare_float(const void* a, const void* b) {
   float var_a = *(const float*)a;
   float var_b = *(const float*)b;
   if (var_a > var_b){return 1;}
   if (var_a < var_b){return -1;}
   else{return 0;}
}

/**
 * @brief Required function to qsort work, @see array_sort()
 * Compares the first number with the second one.
 * 
 * @param[in] a The first element
 * @param[in] b The second element
 * @return 1 if first is bigger, 0 if is equal and -1 if it's smaller
 */
int compare_double(const void* a, const void* b) {
   double var_a = *(const double*)a;
   double var_b = *(const double*)b;
   if (var_a > var_b){return 1;}
   if (var_a < var_b){return -1;}
   else{return 0;}
}

/**
 * @brief One of the most called funwelction, enters the enum var_types type and return it's size
 * 
 * @param[in] type Is the a var_types type (INT, FLOAT, DOUBLE)
 * @return sizeof(type)
 */
size_t get_type_size(var_types type){
    switch(type){
        case INT: return sizeof(int);
        case FLOAT: return sizeof(float);
        case DOUBLE: return sizeof(double);
    }
    fprintf(stderr, "ERROR! Invalid type!\n");
    return 0;
}

/**
 * @brief Checks if the target array is full
 * 
 * @param[in] array The target array
 * @return True if total_size == used_size, false if not
 */
static bool array_is_full(dArray* array){
    if ((array->total_size) == array->used_size){
        return true;
    }
    return false;
}
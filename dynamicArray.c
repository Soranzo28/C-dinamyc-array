#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*
METHODS
array_new(var_types type, size_t start_size) // Creates new dynamic array
array_append(dArray *array, void* new_element) //Appends a new element to the end of the array
array_delete(dArray** array) //Deletes the current array
array_pop(dArray* array, void* store_var) //Removes the last element and returns it to specified variable
array_remove_by_value(dArray* array, void* value) //Remove the first ocurrence of the value
array_remove_by_index(dArray* array, size_t index) //Remove the element at specified index
array_get() //Takes the value at index and copy to specified variable
array_set() //Substitues the value at index for another
array_is_empty() //Verify if the array is empty
array_clear() //Removes all array elements without freeing them.
array_insert() //Inserts a new value at a specified index, pushing all elements to the right
array_shrink() //Reduces array's capacity to be equals to used elements
array_find() //Searches for a value and returns it index
array_sort //Sort array's elements
array_reverse //Inverts element's order
TODO array_binary_search(int already_sorted) //Uses binary search to find an element, a adicional parameter let the user choose if he wants to the func to sort it before or if its already sorted
*/

typedef enum{
    INT,
    FLOAT,
    DOUBLE,
} var_types;

typedef struct{
    void* dArray;
    size_t total_size;
    size_t used_size;
    var_types type;
}dArray;

int compare_int(const void* a, const void* b) {
   return (*(int*)a - *(int*)b);
}

int compare_float(const void* a, const void* b) {
   float var_a = *(const float*)a;
   float var_b = *(const float*)b;
   if (var_a > var_b){return 1;}
   if (var_a < var_b){return -1;}
   else{return 0;}
}

int compare_double(const void* a, const void* b) {
   double var_a = *(const double*)a;
   double var_b = *(const double*)b;
   if (var_a > var_b){return 1;}
   if (var_a < var_b){return -1;}
   else{return 0;}
}

size_t get_type_size(var_types type){
    switch(type){
        case INT: return sizeof(int);
        case FLOAT: return sizeof(float);
        case DOUBLE: return sizeof(double);
    }
    printf("ERROR! Invalid type!\n");
    return 0;
}

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


bool array_find(dArray* array, void* value, size_t* store_index){
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
        printf("ERROR! Element not found!\n");
        return false;
    }
    *store_index = (size_t)found_index;
    return true;
}

bool array_get(dArray* array, size_t index, void* store_variable){
    if (index >= array->used_size){
        printf("ERROR! Index out of range!\n");
        return false;
    }
    size_t type_size = get_type_size(array->type);
    void* temp_pointer = (char*)array->dArray + index*type_size;
    memcpy(store_variable, temp_pointer, type_size);
    printf("WARNING! Copyed the value at index %lu to specified variable!\n", index);
    return true;
}

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

bool array_is_empty(dArray* array){
    return array->used_size == 0 ? true : false;
}


dArray* array_new( var_types type, size_t start_size){
    if (start_size == 0){
        printf("ERROR! Size must be bigger than 0!\n");
        return NULL;
    }

    dArray* new_array = malloc(sizeof(dArray));
    if (!new_array){
        printf("ERROR! Failed to allocate memory!\n");
        return NULL;
    }

    size_t type_size = get_type_size(type);
    if (type_size == 0){
        printf("ERROR! Unable to allocate a new array!\n");
        return NULL;
    }
    new_array->dArray = malloc(start_size * type_size);

    new_array->total_size = start_size;
    new_array->used_size = 0;
    new_array->type = type;
    printf("WARNING! Succesfully allocated initial array!\n");
    return new_array;
}

bool array_is_full(dArray* array){
    if ((array->total_size) == array->used_size){
        return true;
    }
    return false;
}

void array_clear(dArray* array){
    void* temp_pointer = array->dArray;
    size_t type_size = get_type_size(array->type);
    memset(temp_pointer, 0, array->total_size*type_size);
    array->used_size = 0;
    printf("WARNING! The array has been cleared!\n");
}

bool array_shrink(dArray *array){
    if (array->used_size == 0){
        printf("ERROR! The array have no elements!\n");
        return false;
    }
    if (array->used_size == array->total_size){
        printf("ERROR! The array is already shrinked!\n");
        return false;
    }

    size_t type_size = get_type_size(array->type);
    void* temp = realloc(array->dArray, type_size*array->used_size);
    if (temp){
        array->dArray = temp;
        array->total_size = array->used_size;
        printf("WARNING! The array has been shrinked!\n");
        return true;
    }
    printf("ERROR! Unable to shrink the array!\n");
    return false;
}

bool array_realloc(dArray* array){
    size_t new_size = array->total_size + (array->total_size >> 1);
    size_t type_size = get_type_size(array->type);

    if (new_size == array->total_size){
        new_size = array->total_size+1;
    }

    if (type_size == 0){
        printf("ERROR! Unable to reallocate the array!\n");
        return false;
    }
    void* temp = realloc(array->dArray, new_size*type_size);
    if (temp){
        array->dArray = temp;;
        printf("WARNING! The array has been realocated! New size: %lu\n", new_size);
        array->total_size = new_size;
        return true;
    }
    printf("ERROR! Unable to reallocate a bigger array");
    return false;
}

bool array_insert(dArray* array, size_t index, void* new_value){
    if (index > array->used_size){
        printf("ERROR! Index out of range!\n");
        return false;
    }
    size_t type_size = get_type_size(array->type);
    void    * source = (char*)array->dArray + index*type_size;

    if (array->used_size + 1 >= array->total_size){
        array_realloc(array);
    }
    void* destination = source+(type_size);
    size_t bytes_to_move = (array->used_size - index)* type_size;
    if (bytes_to_move > 0){
        memmove(destination, source, bytes_to_move);
    }
    memcpy(source, new_value, type_size);
    return true;
}


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

bool array_delete(dArray** array){
    if (!*array || !array){
        printf("The array does not exist!\n");
        return false;
    }

    free((*array)->dArray);
    (*array)->dArray = NULL;
    free(*array);
    printf("WARINING! Successefuly deleted array!\n");
    *array = NULL;
    return true;
}

bool array_pop(dArray* array, void* store_var){
    if (array->used_size == 0){
        printf("ERROR! The list does not have any elements!\n");
        return false;
    }

    array->used_size--;
    size_t type_size = get_type_size(array->type);
    void* source = ((char*)array->dArray) + array->used_size * type_size;

    memcpy(store_var, source, type_size);
    return true;
}

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
        printf("ERROR! Element not found!\n");
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


//You can note that we havwe two very simillar functions but implemented in two different ways, i did that just to test both ways.
//Personally, the second one (this one blow), is much better, but i did it with help of IA (gemini)
bool array_remove_by_index(dArray* array, size_t index){
    if (index >= array->used_size){
        printf("ERROR! Index is out of range\n");
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
    printf("WARNING! Removed the value at index %lu!\n", index);
    return true;
}

bool array_reverse(dArray* array){
    void *header = NULL, *tail = NULL;
    size_t type_size = get_type_size(array->type);
    header = (char*)array->dArray;
    tail = (char*)array->dArray + (array->used_size-1) * type_size;
    void* temp = malloc(type_size);
    if (!temp){
        printf("ERROR! Unable to allocate auxiliar array!\n");
        return false;
    }
    printf("\n\n entrou no loop\n\n");
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
    return true;
}

int main(void){
    // --- SETUP ---
    dArray* array = array_new(INT, 3);
    if (!array) {
        return 1; // Sai se a criação falhar
    }

    int x = 50;
    printf("Adicionando 20 elementos, de 50 a 31...\n");
    for (int i = 0; i < 20; i++) {
        if (!array_append(array, &x)) {
            fprintf(stderr, "Falha ao adicionar elemento!\n");
            break;
        }
        x--;
    }
    printf("Elementos adicionados com sucesso.\n");
    array_reverse(array);
    // --- BLOCO DE DEBUG ---
    printf("\n--- INICIANDO DEBUG ---\n");
    printf("Tipo do Array: %d (0=INT)\n", array->type);
    printf("Tamanho em Uso (used_size): %zu\n", array->used_size);
    printf("Tamanho Total (total_size): %zu\n", array->total_size);

    if (array->used_size > 0) {
        printf("Conteúdo completo do array:\n");
        for (size_t i = 0; i < array->used_size; i++) {
            printf("  Índice %zu: %d\n", i, ((int*)array->dArray)[i]);
        }

        // --- A VERIFICAÇÃO FINAL ---
        printf("\nVerificando o último elemento (índice %zu):\n", array->used_size - 1);
        
        // Calculando o ponteiro exatamente como discutimos
        void* pointer_para_ultimo = (char*)array->dArray + (array->used_size - 1) * get_type_size(array->type);
        int ultimo_valor_lido = *(int*)pointer_para_ultimo;
        
        printf("  Valor lido: %d\n", ultimo_valor_lido);

        if (ultimo_valor_lido == 31) {
            printf("  Resultado: CORRETO! O valor é 31, como esperado.\n");
        } else {
            printf("  Resultado: INCORRETO! O valor deveria ser 31.\n");
        }

    } else {
        printf("O array está vazio.\n");
    }
    printf("--- FIM DO DEBUG ---\n\n");

    // Lembre-se de destruir o array no final
    array_delete(&array);
    
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include "darray.h" // Inclui a sua biblioteca

// --- Funções Auxiliares para Teste ---

/**
 * @brief Uma função auxiliar para imprimir o estado e o conteúdo de um array de inteiros.
 */
void print_int_array(const char* title, dArray* array) {
    printf("--- %s ---\n", title);
    if (!array) {
        printf("Array é NULL.\n\n");
        return;
    }

    // ANTES: Acesso direto (não compilaria com a struct opaca)
    // printf("Capacidade: %zu | Em Uso: %zu\n", array->total_size, array->used_size);

    // DEPOIS: Usando as funções públicas (getters) - CORRETO!
    printf("Capacidade: %zu | Em Uso: %zu\n", array_get_capacity(array), array_get_size(array));

    printf("Conteúdo: [ ");
    for (size_t i = 0; i < array_get_size(array); i++) { // Usando getter no loop também!
        int value;
        if (array_get(array, i, &value)) {
            printf("%d ", value);
        }
    }
    printf("]\n\n");
}

/**
 * @brief Função principal que testa todas as funcionalidades para um array de INT.
 */
void test_integer_array() {
    printf("\n>>> INICIANDO TESTES COM ARRAY DE INTEIROS <<<\n\n");

    // --- Teste: array_new ---
    printf("1. Criando um novo array com capacidade para 5 inteiros...\n");
    dArray* array = array_new(INT, 5);
    print_int_array("Estado Inicial", array);

    // --- Teste: array_is_empty ---
    printf("2. Verificando se o array está vazio...\n");
    printf("O array está vazio? %s\n\n", array_is_empty(array) ? "Sim" : "Não");

    // --- Teste: array_append ---
    printf("3. Adicionando 7 elementos (deve forçar uma realocação)...\n");
    for (int i = 0; i < 7; i++) {
        int val = (i + 1) * 10; // 10, 20, 30, 40, 50, 60, 70
        array_append(array, &val);
    }
    print_int_array("Após adicionar 7 elementos", array);
    printf("O array está vazio? %s\n\n", array_is_empty(array) ? "Sim" : "Não");

    // --- Teste: array_get e array_set ---
    printf("4. Testando get/set no índice 3...\n");
    int got_value;
    array_get(array, 3, &got_value);
    printf("Valor no índice 3 é: %d\n", got_value);
    int new_value_for_set = 99;
    printf("Alterando o valor no índice 3 para %d...\n", new_value_for_set);
    array_set(array, 3, &new_value_for_set);
    print_int_array("Após set", array);

    // --- Teste: array_pop ---
    printf("5. Testando pop para remover o último elemento...\n");
    int popped_value;
    if (array_pop(array, &popped_value)) {
        printf("Valor removido com pop: %d\n", popped_value);
    }
    print_int_array("Após pop", array);

    // --- Teste: array_insert ---
    printf("6. Inserindo o valor 55 no meio do array (índice 2)...\n");
    int value_to_insert = 55;
    array_insert(array, 2, &value_to_insert);
    print_int_array("Após insert", array);

    // --- Teste: array_remove_by_index e array_remove_by_value ---
    printf("7. Removendo o primeiro elemento (índice 0)...\n");
    array_remove_by_index(array, 0);
    print_int_array("Após remover índice 0", array);

    printf("8. Removendo o elemento com valor 50...\n");
    int value_to_remove = 50;
    array_remove_by_value(array, &value_to_remove);
    print_int_array("Após remover valor 50", array);

    // --- Teste: array_find ---
    printf("9. Procurando pelo índice do valor 99...\n");
    size_t found_index;
    int value_to_find = 99;
    if (array_find(array, &value_to_find, &found_index)) {
        printf("Valor %d encontrado no índice %zu.\n\n", value_to_find, found_index);
    }

    // --- Teste: array_reverse ---
    printf("10. Revertendo o array...\n");
    array_reverse(array);
    print_int_array("Após reverse", array);

    // --- Teste: array_sort ---
    printf("11. Ordenando o array...\n");
    array_sort(array);
    print_int_array("Após sort", array);
    
    // --- Teste: array_binary_search ---
    printf("12. Buscando pelo valor 55 com busca binária...\n");
    // (A sua função de busca binária atual imprime o resultado, não retorna o índice)
    // Supondo que você a altere para retornar ssize_t, como sugerido:
    // ssize_t bs_index = array_binary_search(array, &value_to_insert);
    // printf("Busca binária encontrou o valor no índice: %zd\n\n", bs_index);
    size_t store_index;
    if(array_binary_search(array, &value_to_insert, &store_index, true)){
        printf("Número 55 encontrado no index: %zu\n", store_index);
    } // Chamando sua função atual
    else{
        printf("Número não encontrado!\n");
    }
    printf("\n");

    // --- Teste: array_shrink ---
    printf("13. Reduzindo a capacidade do array para o tamanho exato em uso...\n");
    array_shrink(array);
    print_int_array("Após shrink", array);

    // --- Teste: array_clear ---
    printf("14. Limpando o array...\n");
    array_clear(array);
    print_int_array("Após clear", array);

    // --- Teste: array_destroy ---
    printf("15. Destruindo o array...\n");
    array_destroy(&array);
    print_int_array("Após destroy", array);
}


// --- Função Main ---
int main(void) {
    printf("=======================================\n");
    printf("== INICIANDO TESTE DA BIBLIOTECA DARRAY ==\n");
    printf("=======================================\n");

    test_integer_array();

    // Você pode adicionar mais funções de teste aqui para FLOAT e DOUBLE!
    // test_double_array();

    printf("\n=======================================\n");
    printf("==   TESTES CONCLUÍDOS COM SUCESSO   ==\n");
    printf("=======================================\n");

    return 0;
}
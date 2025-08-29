# Makefile para o Projeto de Array Dinâmico (versão profissional)

# --- Variáveis ---
CC = gcc
CFLAGS = -Wall -Wextra -g -std=c11
TARGET = meu_programa

# O diretório onde os arquivos fonte (.c e .h) estão
SRC_DIR = src

# Lista todos os seus arquivos .c que estão dentro de SRC_DIR
SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/darray.c

# Truque do Make: cria a lista de arquivos objeto (.o) automaticamente
# a partir da lista de fontes. Ex: src/main.c -> src/main.o
OBJECTS = $(SOURCES:.c=.o)

# --- Regras ---
all: $(TARGET)

# A regra de linkagem agora usa as variáveis com os caminhos corretos
$(TARGET): $(OBJECTS)
	# A flag -o $@ significa "coloque a saída no alvo desta regra"
	# O $^ significa "use todos os pré-requisitos"
	$(CC) $(CFLAGS) -o $@ $^

# Esta é uma regra de compilação genérica.
# Ela ensina ao make como criar QUALQUER arquivo .o a partir de um arquivo .c
# dentro da pasta src/. É muito mais limpa que ter uma regra para cada arquivo.
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/darray.h
	# A flag -c significa "compile, não linke"
	# A variável $< significa "o primeiro pré-requisito" (o arquivo .c)
	# A variável $@ significa "o alvo" (o arquivo .o)
	$(CC) $(CFLAGS) -c $< -o $@


# --- Ações ---
clean:
	# Remove o executável e todos os arquivos .o de dentro da pasta src/
	rm -f $(TARGET) $(SRC_DIR)/*.o

run: all
	./$(TARGET)

.PHONY: all clean run
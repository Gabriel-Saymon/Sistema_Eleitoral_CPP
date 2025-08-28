# Nome do compilador
CPP = g++
# Opções de compilação
CFLAGS = -Wall -g
# Flags para o C++ (versão 17)
CPPFLAGS = -std=c++17
# Lista de arquivos-fonte
FONTES = $(wildcard *.cpp)
# Lista dos arquivos-objeto
OBJETOS = $(FONTES:.cpp=.o)
# Nome do arquivo executável
EXECUTAVEL = vereadores

# Alvo principal é o executável
all: $(EXECUTAVEL)

# Para linkar o executável, precisamos dos arquivos-objeto
$(EXECUTAVEL): $(OBJETOS)
	@$(CPP) -o $@ $^

# Alvo para cada arquivo-objeto depende do código fonte
%.o: %.cpp
	@$(CPP) $(CPPFLAGS) -c $(CFLAGS) $< -o $@

# Comando para execução (exemplo para Vitória)
run: $(EXECUTAVEL)
	@./$(EXECUTAVEL) $(CDMUN) candidatos.csv votacao.csv 06/10/2024

# Comando para limpeza
clean:
	@rm -f *.o $(EXECUTAVEL)
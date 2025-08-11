CXX      := g++
SRC_DIR  := src
CXXFLAGS := -std=c++17 -Wall -Wextra -O2 -I$(SRC_DIR)
LDFLAGS  :=

TARGET   := vereadores


SRCS := \
	$(SRC_DIR)/main.cpp \
	$(SRC_DIR)/model/Candidato.cpp \
	$(SRC_DIR)/model/Partido.cpp \
	$(SRC_DIR)/service/Eleicao.cpp \
	$(SRC_DIR)/service/Relatorio.cpp \
	$(SRC_DIR)/util/Leitor.cpp

OBJS := $(SRCS:.cpp=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Exemplo de execução:
# make run CDMUN=57053 CAND=consulta_cand_2024_AC.csv VOTES=votacao_secao_2024_AC.csv DATE=06/10/2024
CDMUN ?= 57053
CAND  ?= candidatos.csv
VOTES ?= votacao.csv
DATE  ?= 06/10/2024

run: $(TARGET)
	./$(TARGET) $(CDMUN) $(CAND) $(VOTES) $(DATE)

clean:
	rm -f $(OBJS) $(TARGET)

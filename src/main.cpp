#include <iostream>
#include <string>
#include "service/Eleicao.h"
#include "service/Relatorio.h"
#include "util/Leitor.h"
#include "date.h"

static void usage(const char* prog){
    std::cerr << "Uso: " << prog << " <codigo_municipio> <candidatos.csv> <votacao.csv> <dd/MM/yyyy>\n";
}

int main(int argc, char** argv){
    if (argc != 5){
        usage(argv[0]);
        return 1;
    }
    std::string codMun = argv[1];
    std::string arqCand = argv[2];
    std::string arqVotos = argv[3];
    Date data = parseDate(argv[4]);

    Eleicao eleicao(codMun, data);
    Leitor leitor(arqCand);
    leitor.leituraCandidatos(eleicao);
    leitor.leituraVotos(eleicao, arqVotos);

    Relatorio rel(eleicao);
    rel.gerarRelatorios();
    return 0;
}
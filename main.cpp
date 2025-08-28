#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "Eleicao.h"
#include "Leitor.h"
#include "Relatorio.h"
#include "Utils.h"

int main(int argc, char *argv[]) {
    if (argc != 5) {
        std::cout << "Uso: ./vereadores <codigo_municipio> <arquivo_candidatos> <arquivo_votacao> <data_eleicao>" << std::endl;
        return 1;
    }

    try {
        std::string codigoMunicipio = argv[1];
        std::string caminhoCandidatos = argv[2];
        std::string caminhoVotos = argv[3];
        Utils::Date dataEleicao = Utils::Date::fromString(argv[4]);

        Eleicao eleicao(codigoMunicipio, dataEleicao);
        Leitor leitor;
        
        leitor.leituraCandidatos(eleicao, caminhoCandidatos);
        leitor.leituraVotos(eleicao, caminhoVotos);

        Relatorio relatorio(eleicao);
        relatorio.gerarRelatorios();

    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
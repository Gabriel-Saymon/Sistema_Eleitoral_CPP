#ifndef RELATORIO_H
#define RELATORIO_H

#include "Eleicao.h"
#include <vector>
#include <string>
#include <locale>

class Relatorio {
    Eleicao& eleicao;
    std::vector<Candidato*> candidatosOrdenados;
    std::vector<Partido*> partidosOrdenados;

public:
    Relatorio(Eleicao& eleicao);
    void gerarRelatorios();

private:
    void printNumeroVagas();
    void printCandidatosEleitos();
    void printCandidatosMaisVotados(); 
    void printPrejudicadosEBeneficiados();
    void printVotacaoPorPartido();
    void printPrimeiroUltimoPorPartido();
    void printDistribuicaoFaixaEtaria();
    void printDistribuicaoGenero();
    void printTotalVotos();
};

#endif
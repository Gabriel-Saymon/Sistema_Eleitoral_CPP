#pragma once
#include <vector>
#include <string>

class Eleicao;
class Candidato;
class Partido;

class Relatorio {
public:
    explicit Relatorio(Eleicao& e) : e_(e) {}
    void gerarRelatorios();

private:
    Eleicao& e_;
    // helpers
    void printNumeroVagas();
    void printEleitos();
    void printTopN();
    void printPrejudicadosEBeneficiados();
    void printVotacaoPorPartido();
    void printPrimeiroUltimoPorPartido();
    void printFaixaEtaria();
    void printGenero();
    void printTotais();

    // utils
    static const char* palavraVoto(int v){ return (v==0 || v==1) ? "voto" : "votos"; }
    static const char* palavraNominal(int v){ return (v==0 || v==1) ? "nominal" : "nominais"; }
    static const char* palavraEleitos(int v){ return (v==1) ? "candidato eleito" : "candidatos eleitos"; }
};
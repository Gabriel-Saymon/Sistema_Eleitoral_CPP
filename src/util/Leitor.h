#pragma once
#include <string>

class Eleicao;

class Leitor {
public:
    Leitor(std::string caminhoCandidatos) : caminhoCandidatos_(std::move(caminhoCandidatos)) {}
    void leituraCandidatos(Eleicao& eleicao);
    void leituraVotos(Eleicao& eleicao, const std::string& caminhoVotos);
private:
    std::string caminhoCandidatos_;
};
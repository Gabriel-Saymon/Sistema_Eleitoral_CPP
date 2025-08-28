#ifndef LEITOR_H
#define LEITOR_H

#include <string>
#include "Eleicao.h"

class Leitor {
public:
    void leituraCandidatos(Eleicao& eleicao, const std::string& caminhoArquivo);
    void leituraVotos(Eleicao& eleicao, const std::string& caminhoArquivo);
};

#endif
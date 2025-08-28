#ifndef ELEICAO_H
#define ELEICAO_H

#include <string>
#include <map>
#include <vector>
#include "Candidato.h"
#include "Partido.h"
#include "Utils.h"

class Eleicao {
    std::string codMunicipio;
    Utils::Date dataEleicao; // Adicionado namespace Utils
    int qtdVagas;
    std::map<int, Candidato*> candidatos;
    std::map<int, Partido*> partidosMap;
    std::vector<Partido*> partidosList;

public:
    Eleicao(const std::string& codMunicipio, const Utils::Date& dataEleicao); // Adicionado namespace Utils
    ~Eleicao();

    void adicionaCandidato(Candidato* cand);
    void adicionaPartido(Partido* partido);

    Partido* getPartidoPorNumero(int numero);
    const std::string& getCodMunicipio() const;
    const Utils::Date& getDataEleicao() const;
    const std::map<int, Candidato*>& getCandidatos() const;
    std::map<int, Candidato*>& getCandidatosMap();
    const std::vector<Partido*>& getPartidos() const;
    int getNumeroCandidatosEleitos() const;
};

#endif
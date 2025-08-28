#ifndef PARTIDO_H
#define PARTIDO_H

#include <string>
#include <vector>
#include <numeric>
#include "Candidato.h"

class Partido {
    int numero;
    std::string sigla;
    std::vector<Candidato*> candidatos;
    int votosLegenda;

public:
    Partido(int numero, const std::string& sigla);

    // Getters
    int getNumero() const;
    const std::string& getSigla() const;
    int getVotosLegenda() const;
    const std::vector<Candidato*>& getCandidatos() const;
    std::vector<Candidato*>& getCandidatos();
    int getVotosNominais() const;
    int getTotalVotos() const;

    void addCandidato(Candidato* c);
    void addVotosLegenda(int qtd);
};

#endif
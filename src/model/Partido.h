#pragma once
#include <string>
#include <vector>

class Candidato;

class Partido {
public:
    Partido(int numero, std::string sigla) : numero_(numero), sigla_(std::move(sigla)) {}

    int getNumero() const { return numero_; }
    const std::string& getSigla() const { return sigla_; }
    int getVotosLegenda() const { return votosLegenda_; }
    void addVotosLegenda(int v){ votosLegenda_ += v; }

    void addCandidato(Candidato* c){ candidatos_.push_back(c); }
    const std::vector<Candidato*>& getCandidatos() const { return candidatos_; }
    std::vector<Candidato*>& getCandidatos() { return candidatos_; }

    int getVotosNominais() const;
    int getTotalVotos() const { return getVotosNominais() + votosLegenda_; }

private:
    int numero_;
    std::string sigla_;
    int votosLegenda_{0};
    std::vector<Candidato*> candidatos_;
};
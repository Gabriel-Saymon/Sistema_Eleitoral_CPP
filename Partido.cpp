#include "Partido.h"

Partido::Partido(int numero, const std::string& sigla)
    : numero(numero), sigla(sigla), votosLegenda(0) {}

int Partido::getNumero() const { return numero; }
const std::string& Partido::getSigla() const { return sigla; }
int Partido::getVotosLegenda() const { return votosLegenda; }
const std::vector<Candidato*>& Partido::getCandidatos() const { return candidatos; }
std::vector<Candidato*>& Partido::getCandidatos() { return candidatos; }

int Partido::getVotosNominais() const {
    return std::accumulate(candidatos.begin(), candidatos.end(), 0, 
        [](int sum, const Candidato* c) {
            return sum + c->getQtdVotos();
        });
}

int Partido::getTotalVotos() const {
    return getVotosNominais() + votosLegenda;
}

void Partido::addCandidato(Candidato* c) {
    candidatos.push_back(c);
}

void Partido::addVotosLegenda(int qtd) {
    votosLegenda += qtd;
}
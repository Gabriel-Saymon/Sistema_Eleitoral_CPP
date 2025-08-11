#include "Eleicao.h"
#include "../helpers.h"          // normalize_code
#include "../model/Partido.h"
#include "../model/Candidato.h"

// DEFINIÇÃO com a MESMA assinatura do .h
Eleicao::Eleicao(const std::string& codMunicipio, const Date& dataEleicao)
    : codMunicipioNorm_(normalize_code(codMunicipio)),
      dataEleicao_(dataEleicao) {}

Eleicao::~Eleicao() = default;

void Eleicao::adicionaPartido(std::unique_ptr<Partido> p){
    int numero = p->getNumero();
    partidos_.emplace(numero, std::move(p));
}

void Eleicao::adicionaCandidato(std::unique_ptr<Candidato> c){
    int numero = c->getNrCandidato();
    candidatos_.emplace(numero, std::move(c));
}

Partido* Eleicao::getPartidoPorNumero(int numero) const {
    auto it = partidos_.find(numero);
    return (it==partidos_.end()) ? nullptr : it->second.get();
}

Candidato* Eleicao::getCandidatoPorNumero(int numero) const {
    auto it = candidatos_.find(numero);
    return (it==candidatos_.end()) ? nullptr : it->second.get();
}

std::vector<Partido*> Eleicao::getPartidos() const {
    std::vector<Partido*> v;
    v.reserve(partidos_.size());
    for (auto& kv : const_cast<std::map<int,std::unique_ptr<Partido>>&>(partidos_)) {
        v.push_back(kv.second.get());
    }
    return v;
}
std::vector<Candidato*> Eleicao::getCandidatos() const {
    std::vector<Candidato*> v;
    v.reserve(candidatos_.size());
    for (auto& kv : const_cast<std::map<int,std::unique_ptr<Candidato>>&>(candidatos_)) {
        v.push_back(kv.second.get());
    }
    return v;
}

int Eleicao::getNumeroCandidatosEleitos() const {
    int cnt=0;
    for (auto& kv : const_cast<std::map<int,std::unique_ptr<Candidato>>&>(candidatos_)) {
        if (kv.second->isEleito()) cnt++;
    }
    return cnt;
}
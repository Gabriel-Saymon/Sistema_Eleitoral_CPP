#include "Eleicao.h"

Eleicao::Eleicao(const std::string& codMunicipio, const Utils::Date& dataEleicao)
    : codMunicipio(codMunicipio), dataEleicao(dataEleicao), qtdVagas(0) {}

Eleicao::~Eleicao() {
    for (auto const& [key, val] : candidatos) {
        delete val;
    }
    // Partidos já são deletados pelo map de candidatos, pois eles são donos dos candidatos
    for (auto const& [key, val] : partidosMap) {
         delete val;
    }
}

void Eleicao::adicionaCandidato(Candidato* cand) {
    candidatos[cand->getNrCandidato()] = cand;
    if (cand->isEleito()) {
        qtdVagas++;
    }
}

void Eleicao::adicionaPartido(Partido* partido) {
    partidosMap[partido->getNumero()] = partido;
    partidosList.push_back(partido);
}

Partido* Eleicao::getPartidoPorNumero(int numero) {
    auto it = partidosMap.find(numero);
    return (it != partidosMap.end()) ? it->second : nullptr;
}

const std::string& Eleicao::getCodMunicipio() const { return codMunicipio; }
const Utils::Date& Eleicao::getDataEleicao() const { return dataEleicao; }
const std::map<int, Candidato*>& Eleicao::getCandidatos() const { return candidatos; }
std::map<int, Candidato*>& Eleicao::getCandidatosMap() { return candidatos; }
const std::vector<Partido*>& Eleicao::getPartidos() const { return partidosList; }
int Eleicao::getNumeroCandidatosEleitos() const { return qtdVagas; }
#include "Candidato.h"
#include "Partido.h"

Candidato::Candidato(const std::string& sgUe, int cdCargo, int nrCandidato, const std::string& nmCandidato, 
                     const std::string& nmUrnaCandidato, int nrPartido, const std::string& sgPartido, 
                     int nrFederacao, const Utils::Date& dtNascimento, int cdGenero, int cdSitTotTurno, Partido* p)
    : sgUe(sgUe), cdCargo(cdCargo), nrCandidato(nrCandidato), nmCandidato(nmCandidato),
      nmUrnaCandidato(nmUrnaCandidato), nrPartido(nrPartido), sgPartido(sgPartido),
      nrFederacao(nrFederacao), dtNascimento(dtNascimento), cdGenero(cdGenero), 
      cdSitTotTurno(cdSitTotTurno), qtdVotos(0), partido(p) {}

int Candidato::getNrCandidato() const { return nrCandidato; }
int Candidato::getQtdVotos() const { return qtdVotos; }
bool Candidato::isEleito() const { return cdSitTotTurno == 2 || cdSitTotTurno == 3; }
int Candidato::getNrFederacao() const { return nrFederacao; }
const std::string& Candidato::getNmUrnaCandidato() const { return nmUrnaCandidato; }
const std::string& Candidato::getSgPartido() const { return sgPartido; }
const Utils::Date& Candidato::getDtNascimento() const { return dtNascimento; }
int Candidato::getIdade(const Utils::Date& dataEleicao) const { return Utils::calculateAge(this->dtNascimento, dataEleicao); }
int Candidato::getCdGenero() const { return cdGenero; }
int Candidato::getNrPartido() const { return nrPartido; }
void Candidato::addVotos(int qtd) { this->qtdVotos += qtd; }
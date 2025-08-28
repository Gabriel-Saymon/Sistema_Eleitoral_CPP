#ifndef CANDIDATO_H
#define CANDIDATO_H

#include <string>
#include "Utils.h"

class Partido;

class Candidato {
    std::string sgUe;
    int cdCargo;
    int nrCandidato;
    std::string nmCandidato;
    std::string nmUrnaCandidato;
    int nrPartido;
    std::string sgPartido;
    int nrFederacao;
    Utils::Date dtNascimento;
    int cdGenero;
    int cdSitTotTurno;
    int qtdVotos;
    Partido* partido;

public:
    Candidato(const std::string& sgUe, int cdCargo, int nrCandidato, const std::string& nmCandidato, 
              const std::string& nmUrnaCandidato, int nrPartido, const std::string& sgPartido, 
              int nrFederacao, const Utils::Date& dtNascimento, int cdGenero, int cdSitTotTurno, Partido* p);

    int getNrCandidato() const;
    int getQtdVotos() const;
    bool isEleito() const;
    int getNrFederacao() const;
    const std::string& getNmUrnaCandidato() const;
    const std::string& getSgPartido() const;
    const Utils::Date& getDtNascimento() const;
    int getIdade(const Utils::Date& dataEleicao) const;
    int getCdGenero() const;
    int getNrPartido() const;

    void addVotos(int qtd);
};

#endif
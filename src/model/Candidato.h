#pragma once
#include <string>
#include "date.h"

class Partido; // forward

class Candidato {
public:
    Candidato(std::string sgUe, int cargo, int nrCandidato,
              std::string nmCandidato, std::string nmUrna,
              int nrPartido, std::string sgPartido, int nrFederacao,
              Date dtNascimento, int cdGenero, int cdSitTotTurno,
              Partido* partido);

    // getters
    const std::string& getSgUe() const { return sgUe_; }
    int getCargo() const { return cargo_; }
    int getNrCandidato() const { return nrCandidato_; }
    const std::string& getNmCandidato() const { return nmCandidato_; }
    const std::string& getNmUrnaCandidato() const { return nmUrna_; }
    int getNrPartido() const { return nrPartido_; }
    const std::string& getSgPartido() const { return sgPartido_; }
    int getNrFederacao() const { return nrFederacao_; }
    const Date& getDtNascimento() const { return dtNascimento_; }
    int getCdGenero() const { return cdGenero_; }
    int getCdSitTotTurno() const { return cdSitTotTurno_; }
    int getQtdVotos() const { return qtdVotos_; }
    Partido* getPartido() const { return partido_; }

    // logic
    bool isEleito() const { return cdSitTotTurno_ == 2; } // simples
    void addVotos(int v){ qtdVotos_ += v; }

private:
    std::string sgUe_;
    int cargo_;
    int nrCandidato_;
    std::string nmCandidato_;
    std::string nmUrna_;
    int nrPartido_;
    std::string sgPartido_;
    int nrFederacao_;
    Date dtNascimento_;
    int cdGenero_;
    int cdSitTotTurno_;
    int qtdVotos_{0};
    Partido* partido_{nullptr};
};
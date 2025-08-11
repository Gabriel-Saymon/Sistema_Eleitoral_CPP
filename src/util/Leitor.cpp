#include "Leitor.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include "../service/Eleicao.h"
#include "../model/Candidato.h"
#include "../model/Partido.h"
#include "../helpers.h"
#include "../date.h"

static std::vector<std::string> split_semicolon(const std::string& line){
    std::vector<std::string> cols;
    std::string cur;
    std::istringstream iss(line);
    while (std::getline(iss, cur, ';')) {
        cols.push_back(remove_quotes(cur));
    }
    return cols;
}

void Leitor::leituraCandidatos(Eleicao& eleicao){
    std::ifstream in(caminhoCandidatos_);
    if(!in.is_open()){
        std::cerr << "Erro ao abrir candidatos: " << caminhoCandidatos_ << "\n";
        std::exit(1);
    }
    std::string line;
    std::getline(in, line); // header

    while(std::getline(in, line)){
        auto cols = split_semicolon(line);
        if (cols.size() < 49) continue;

        std::string sgUe  = cols[11];
        if (normalize_code(sgUe) != eleicao.getCodMunicipioNorm()) continue;

        int cdCargo = std::stoi(cols[13]);
        if (cdCargo != 13) continue;

        int cdSit   = std::stoi(cols[48]);
        if (cdSit == -1) continue;

        int nrCandidato = std::stoi(cols[16]);
        std::string nmCandidato = cols[17];
        std::string nmUrna = cols[18];
        int nrPartido = std::stoi(cols[25]);
        std::string sgPartido = cols[26];
        int nrFederacao = std::stoi(cols[28]);
        Date dtNasc = parseDate(cols[36]);
        int cdGenero = std::stoi(cols[38]);

        Partido* partido = eleicao.getPartidoPorNumero(nrPartido);
        if (!partido){
            auto np = std::make_unique<Partido>(nrPartido, sgPartido);
            partido = np.get();
            eleicao.adicionaPartido(std::move(np));
        }

        auto cand = std::make_unique<Candidato>(
            sgUe, cdCargo, nrCandidato, nmCandidato, nmUrna,
            nrPartido, sgPartido, nrFederacao, dtNasc, cdGenero, cdSit, partido
        );
        partido->addCandidato(cand.get());
        eleicao.adicionaCandidato(std::move(cand));
    }
}

void Leitor::leituraVotos(Eleicao& eleicao, const std::string& caminhoVotos){
    std::ifstream in(caminhoVotos);
    if(!in.is_open()){
        std::cerr << "Erro ao abrir votos: " << caminhoVotos << "\n";
        std::exit(1);
    }
    std::string line;
    std::getline(in, line); // header

    while(std::getline(in, line)){
        auto cols = split_semicolon(line);
        if (cols.size() < 22) continue;

        std::string sgUe = cols[11];
        if (normalize_code(sgUe) != eleicao.getCodMunicipioNorm()) continue;

        int cdCargo = std::stoi(cols[17]);
        if (cdCargo != 13) continue;

        int nrVotavel = std::stoi(cols[19]);
        int qtVotos   = std::stoi(cols[21]);

        if (nrVotavel >= 95 && nrVotavel <= 98) continue;

        // 5 dígitos = voto nominal
        if (std::to_string(nrVotavel).size() == 5){
            if (auto* c = eleicao.getCandidatoPorNumero(nrVotavel)) {
                c->addVotos(qtVotos);
            }
        } else {
            if (auto* p = eleicao.getPartidoPorNumero(nrVotavel)) {
                p->addVotosLegenda(qtVotos);
            }
        }
    }
}
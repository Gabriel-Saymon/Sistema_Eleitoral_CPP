#include "Leitor.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <map>

void Leitor::leituraCandidatos(Eleicao& eleicao, const std::string& caminhoArquivo) {
    std::ifstream file(caminhoArquivo);
    if (!file.is_open()) {
        throw std::runtime_error("Erro ao abrir candidatos: " + caminhoArquivo);
    }
    
    std::string linha;
    std::getline(file, linha);

    //Primeira passada: mapear todos os partidos do arquivo
    std::map<int, std::string> todosOsPartidos;
    while (std::getline(file, linha)) {
        if (linha.empty()) continue;
        std::vector<std::string> cols = Utils::split(linha, ';');
        if (cols.size() < 27) continue;

        int nrPartido = std::stoi(cols[25]);
        std::string sgPartido = Utils::iso_8859_1_to_utf8(cols[26]);
        todosOsPartidos.try_emplace(nrPartido, sgPartido);
    }
    
    // Adiciona todos os partidos encontrados na eleição
    for (const auto& [num, sigla] : todosOsPartidos) {
        eleicao.adicionaPartido(new Partido(num, sigla));
    }

    //Segunda passada: criar e adicionar candidatos (apenas válidos)
    file.clear();
    file.seekg(0, std::ios::beg);
    std::getline(file, linha);

    while (std::getline(file, linha)) {
        if (linha.empty()) continue;
        
        std::vector<std::string> cols = Utils::split(linha, ';');
        if (cols.size() < 49) continue; 

        int codigoLinha = std::stoi(cols[11]); 
        int codigoEleicao = std::stoi(eleicao.getCodMunicipio());
        int cdCargo = std::stoi(cols[13]);
        if (codigoLinha != codigoEleicao || cdCargo != 13) {
            continue;
        }
        
        int cdSit = std::stoi(cols[48]);

        //ignora candidaturas inválidas (CD_SIT_TOT_TURNO == -1)
        if (cdSit == -1) {
            continue;
        }

        int nrPartido = std::stoi(cols[25]);
        Partido* partido = eleicao.getPartidoPorNumero(nrPartido);
        if (partido == nullptr) continue;
        
        // Campos textuais com conversão ISO-8859-1 -> UTF-8 (mantendo seu mapeamento atual)
        std::string nmCandidato = Utils::iso_8859_1_to_utf8(cols[17]);
        std::string nmUrna = Utils::iso_8859_1_to_utf8(cols[18]);
        
        int nrCandidato = std::stoi(cols[16]);
        int nrFederacao = std::stoi(cols[28]);
        Utils::Date dtNasc = Utils::Date::fromString(cols[36]); // dd/mm/aaaa
        int cdGenero = std::stoi(cols[38]);

        Candidato* cand = new Candidato(
            cols[11],            // SG_UE
            cdCargo,             // CD_CARGO
            nrCandidato,         // NR_CANDIDATO
            nmCandidato,         // NM_CANDIDATO (completo)
            nmUrna,              // NM_URNA_CANDIDATO
            nrPartido,           // NR_PARTIDO
            partido->getSigla(), // SG_PARTIDO
            nrFederacao,         // NR_FEDERACAO
            dtNasc,              // DT_NASCIMENTO
            cdGenero,            // CD_GENERO
            cdSit,               // CD_SIT_TOT_TURNO
            partido              // ponteiro para o partido
        );
        
        eleicao.adicionaCandidato(cand);
        partido->addCandidato(cand);
    }
}


void Leitor::leituraVotos(Eleicao& eleicao, const std::string& caminhoArquivo) {
    std::ifstream file(caminhoArquivo);
    if (!file.is_open()) {
        throw std::runtime_error("Erro ao abrir votos: " + caminhoArquivo);
    }

    std::string linha;
    std::getline(file, linha);

    while (std::getline(file, linha)) {
        if(linha.empty()) continue;

        std::vector<std::string> cols = Utils::split(linha, ';');
        if (cols.size() < 22) continue;

        int codigoLinha = std::stoi(cols[11]);
        int codigoEleicao = std::stoi(eleicao.getCodMunicipio());
        int cdCargo = std::stoi(cols[17]);
        if (codigoLinha != codigoEleicao || cdCargo != 13) {
            continue;
        }

        int nrVotavel = std::stoi(cols[19]);
        if (nrVotavel >= 95 && nrVotavel <= 98) continue;

        int qtVotos = std::stoi(cols[21]);

        if (std::to_string(nrVotavel).length() == 5) { 
            auto& candidatosMap = eleicao.getCandidatosMap();
            auto it = candidatosMap.find(nrVotavel);
            if (it != candidatosMap.end()) {
                it->second->addVotos(qtVotos);
            }
        } else {
            Partido* p = eleicao.getPartidoPorNumero(nrVotavel);
            if (p != nullptr) {
                p->addVotosLegenda(qtVotos);
            }
        }
    }
}
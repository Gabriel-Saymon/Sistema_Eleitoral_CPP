#pragma once
#include <map>
#include <memory>
#include <vector>
#include <string>
#include "date.h"    // mantém

class Candidato;
class Partido;

class Eleicao {
public:
    // USE SEMPRE a mesma assinatura aqui e no .cpp
    Eleicao(const std::string& codMunicipio, const Date& dataEleicao);
    ~Eleicao();

    const std::string& getCodMunicipioNorm() const { return codMunicipioNorm_; }
    const Date& getDataEleicao() const { return dataEleicao_; }

    void adicionaPartido(std::unique_ptr<Partido> p);
    void adicionaCandidato(std::unique_ptr<Candidato> c);
    Partido*   getPartidoPorNumero(int numero) const;
    Candidato* getCandidatoPorNumero(int numero) const;
    std::vector<Partido*>   getPartidos() const;
    std::vector<Candidato*> getCandidatos() const;
    int getNumeroCandidatosEleitos() const;

private:
    std::string codMunicipioNorm_;
    Date        dataEleicao_;
    std::map<int, std::unique_ptr<Partido>>   partidos_;
    std::map<int, std::unique_ptr<Candidato>> candidatos_;
};

#include "Relatorio.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <sstream>
#include <locale>

struct pt_br_numpunct : std::numpunct<char> {
protected:
    char do_thousands_sep() const override { return '.'; }
    char do_decimal_point() const override { return ','; }
    std::string do_grouping() const override { return "\3"; }
};

std::string format_int(int num) {
    std::stringstream ss;
    ss.imbue(std::locale(std::cout.getloc(), new pt_br_numpunct()));
    ss << num;
    return ss.str();
}

Relatorio::Relatorio(Eleicao& el) : eleicao(el) {
    for(auto const& [key, val] : eleicao.getCandidatos()){
        candidatosOrdenados.push_back(val);
    }
    
    partidosOrdenados = eleicao.getPartidos();

    std::sort(partidosOrdenados.begin(), partidosOrdenados.end(), [](const Partido* a, const Partido* b){
        if (a->getTotalVotos() != b->getTotalVotos()) {
            return a->getTotalVotos() > b->getTotalVotos();
        }
        return a->getNumero() < b->getNumero();
    });
}

void Relatorio::printNumeroVagas() {
    std::cout << "Número de vagas: " << eleicao.getNumeroCandidatosEleitos() << "\n\n";
}

void Relatorio::printCandidatosEleitos() {
    std::cout << "Vereadores eleitos:" << std::endl;
    std::vector<Candidato*> eleitos;
    for(auto* c : candidatosOrdenados) {
        if (c->isEleito()) {
            eleitos.push_back(c);
        }
    }
    
    std::sort(eleitos.begin(), eleitos.end(), [&](const Candidato* a, const Candidato* b){
        if (a->getQtdVotos() != b->getQtdVotos()) {
            return a->getQtdVotos() > b->getQtdVotos();
        }
        return a->getDtNascimento() < b->getDtNascimento();
    });

    int pos = 1;
    for (const auto* c : eleitos) {
        std::string prefix = (c->getNrFederacao() != -1) ? "*" : "";
        std::cout << pos++ << " - " << prefix << c->getNmUrnaCandidato() << " (" << c->getSgPartido() << ", " << format_int(c->getQtdVotos()) << (c->getQtdVotos() <= 1 ? " voto" : " votos") << ")" << std::endl;
    }
    std::cout << std::endl;
}

void Relatorio::printCandidatosMaisVotados() {
    std::cout << "Candidatos mais votados (em ordem decrescente de votação e respeitando número de vagas):" << std::endl;
    
    std::vector<Candidato*> rankingCompleto = candidatosOrdenados;
    std::sort(rankingCompleto.begin(), rankingCompleto.end(), [&](const Candidato* a, const Candidato* b){
        if (a->getQtdVotos() != b->getQtdVotos()) {
            return a->getQtdVotos() > b->getQtdVotos();
        }
        return a->getDtNascimento() < b->getDtNascimento();
    });

    int numVagas = eleicao.getNumeroCandidatosEleitos();
    for (int i = 0; i < numVagas && i < (int)rankingCompleto.size(); ++i) {
        const auto* c = rankingCompleto[i];
        std::string prefix = (c->getNrFederacao() != -1) ? "*" : "";
        std::cout << (i + 1) << " - " << prefix << c->getNmUrnaCandidato() << " (" << c->getSgPartido() << ", " << format_int(c->getQtdVotos()) << (c->getQtdVotos() <= 1 ? " voto" : " votos") << ")" << std::endl;
    }
    std::cout << std::endl;
}

void Relatorio::printPrejudicadosEBeneficiados(){
    std::vector<Candidato*> rankingCompleto = candidatosOrdenados;
    std::sort(rankingCompleto.begin(), rankingCompleto.end(), [&](const Candidato* a, const Candidato* b){
        if (a->getQtdVotos() != b->getQtdVotos()) {
            return a->getQtdVotos() > b->getQtdVotos();
        }
        return a->getDtNascimento() < b->getDtNascimento();
    });

    int numVagas = eleicao.getNumeroCandidatosEleitos();
    std::vector<Candidato*> prejudicados;
    std::vector<Candidato*> beneficiados;

    for (size_t i = 0; i < rankingCompleto.size(); ++i) {
        Candidato* c = rankingCompleto[i];
        if (i < (size_t)numVagas) {
            if (!c->isEleito()) prejudicados.push_back(c);
        } else {
            if (c->isEleito()) beneficiados.push_back(c);
        }
    }

    std::cout << "Teriam sido eleitos se a votação fosse majoritária, e não foram eleitos:\n(com sua posição no ranking de mais votados)" << std::endl;
    for (const auto* c : prejudicados) {
        auto it = std::find(rankingCompleto.begin(), rankingCompleto.end(), c);
        int pos = std::distance(rankingCompleto.begin(), it) + 1;
        std::string prefix = (c->getNrFederacao() != -1) ? "*" : "";
        std::cout << pos << " - " << prefix << c->getNmUrnaCandidato() << " (" << c->getSgPartido() << ", " << format_int(c->getQtdVotos()) << (c->getQtdVotos() <= 1 ? " voto" : " votos") << ")" << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Eleitos, que se beneficiaram do sistema proporcional:\n(com sua posição no ranking de mais votados)" << std::endl;
    for (const auto* c : beneficiados) {
        auto it = std::find(rankingCompleto.begin(), rankingCompleto.end(), c);
        int pos = std::distance(rankingCompleto.begin(), it) + 1;
        std::string prefix = (c->getNrFederacao() != -1) ? "*" : "";
        std::cout << pos << " - " << prefix << c->getNmUrnaCandidato() << " (" << c->getSgPartido() << ", " << format_int(c->getQtdVotos()) << (c->getQtdVotos() <= 1 ? " voto" : " votos") << ")" << std::endl;
    }
    std::cout << std::endl;
}

void Relatorio::printVotacaoPorPartido() {
    std::cout << "Votação dos partidos e número de candidatos eleitos:" << std::endl;
    int pos = 1;
    for (const auto* p : partidosOrdenados) {
        int nominais = p->getVotosNominais();
        int legenda = p->getVotosLegenda();
        int total = p->getTotalVotos();
        
        long eleitosCount = std::count_if(p->getCandidatos().begin(), p->getCandidatos().end(), [](const Candidato* c){
            return c->isEleito();
        });

        std::cout << pos++ << " - " << p->getSigla() << " - " << p->getNumero() << ", "
                  << format_int(total) << (total <= 1 ? " voto" : " votos") << " ("
                  << format_int(nominais) << (nominais <= 1 ? " nominal" : " nominais") << " e "
                  << format_int(legenda) << " de legenda), "
                  << eleitosCount << (eleitosCount <= 1 ? " candidato eleito" : " candidatos eleitos") << std::endl;
    }
    std::cout << std::endl;
}

void Relatorio::printPrimeiroUltimoPorPartido(){
    std::cout << "Primeiro e último colocados de cada partido:" << std::endl;

    // Partidos com ao menos 1 voto nominal (>0) entram no relatório
    std::vector<Partido*> partidosComCandidatosValidos;
    for (auto* p : partidosOrdenados) {
        if (!p->getCandidatos().empty() && p->getVotosNominais() > 0) {
            partidosComCandidatosValidos.push_back(p);
        }
    }

    // Ordena partidos pelo voto do mais votado (desc). Empate: número do partido (asc).
    std::sort(partidosComCandidatosValidos.begin(), partidosComCandidatosValidos.end(),
        [&](Partido* a, Partido* b){
            const auto& candsA = a->getCandidatos();
            const auto& candsB = b->getCandidatos();

            Candidato* maisVotadoA = *std::max_element(
                candsA.begin(), candsA.end(),
                [](const Candidato* ca, const Candidato* cb){
                    return ca->getQtdVotos() < cb->getQtdVotos();
                }
            );
            Candidato* maisVotadoB = *std::max_element(
                candsB.begin(), candsB.end(),
                [](const Candidato* ca, const Candidato* cb){
                    return ca->getQtdVotos() < cb->getQtdVotos();
                }
            );

            if (maisVotadoA->getQtdVotos() != maisVotadoB->getQtdVotos()) {
                return maisVotadoA->getQtdVotos() > maisVotadoB->getQtdVotos();
            }
            return a->getNumero() < b->getNumero();
        }
    );

    int pos = 1;
    for (auto* p : partidosComCandidatosValidos) {
        // PRIMEIRO: apenas candidatos com votos > 0
        std::vector<Candidato*> candsComVotos;
        for (auto* c : p->getCandidatos()) {
            if (c->getQtdVotos() > 0) {
                candsComVotos.push_back(c);
            }
        }
        if (candsComVotos.empty()) {
            continue; // segurança
        }

        // Ordena para pegar o PRIMEIRO: votos desc; empate -> mais velho primeiro
        std::sort(candsComVotos.begin(), candsComVotos.end(),
            [&](const Candidato* a, const Candidato* b){
                if (a->getQtdVotos() != b->getQtdVotos()) {
                    return a->getQtdVotos() > b->getQtdVotos(); // desc
                }
                return a->getDtNascimento() < b->getDtNascimento(); // empate: mais velho primeiro
            }
        );
        Candidato* primeiro = candsComVotos.front();

        // ÚLTIMO: entre TODOS os candidatos válidos do partido (inclui 0 voto)
        const auto& todos = p->getCandidatos();
        auto compMenorVotoNumeroMaisVelho = [&](const Candidato* a, const Candidato* b){
            if (a->getQtdVotos() != b->getQtdVotos()) {
                return a->getQtdVotos() < b->getQtdVotos();      // menor votação primeiro
            }
            if (a->getNrCandidato() != b->getNrCandidato()) {
                return a->getNrCandidato() < b->getNrCandidato(); // desempate: menor número
            }
            return a->getDtNascimento() < b->getDtNascimento();   // fallback: mais velho primeiro
        };
        Candidato* ultimo = *std::min_element(todos.begin(), todos.end(), compMenorVotoNumeroMaisVelho);

        std::cout << pos++ << " - " << p->getSigla() << " - " << p->getNumero() << ", "
                  << primeiro->getNmUrnaCandidato() << " (" << primeiro->getNrCandidato()
                  << ", " << format_int(primeiro->getQtdVotos()) << (primeiro->getQtdVotos() <= 1 ? " voto" : " votos") << ") / "
                  << ultimo->getNmUrnaCandidato() << " (" << ultimo->getNrCandidato()
                  << ", " << format_int(ultimo->getQtdVotos()) << (ultimo->getQtdVotos() <= 1 ? " voto" : " votos") << ")"
                  << std::endl;
    }
    std::cout << std::endl;
}

void Relatorio::printDistribuicaoFaixaEtaria() {
    std::cout << "Eleitos, por faixa etária (na data da eleição):" << std::endl;
    int totalEleitos = eleicao.getNumeroCandidatosEleitos();
    
    std::cout << std::fixed << std::setprecision(2);
    std::cout.imbue(std::locale(std::cout.getloc(), new pt_br_numpunct()));

    if(totalEleitos == 0) {
        std::cout << "      Idade < 30: 0 (0,00%)" << std::endl;
        std::cout << "30 <= Idade < 40: 0 (0,00%)" << std::endl;
        std::cout << "40 <= Idade < 50: 0 (0,00%)" << std::endl;
        std::cout << "50 <= Idade < 60: 0 (0,00%)" << std::endl;
        std::cout << "60 <= Idade     : 0 (0,00%)" << std::endl;
        std::cout << std::endl;
        return;
    };

    int f1=0, f2=0, f3=0, f4=0, f5=0;
    for (const auto& [key, c] : eleicao.getCandidatos()) {
        if (c->isEleito()) {
            int idade = c->getIdade(eleicao.getDataEleicao());
            if (idade < 30) f1++;
            else if (idade < 40) f2++;
            else if (idade < 50) f3++;
            else if (idade < 60) f4++;
            else f5++;
        }
    }
    
    std::cout << "      Idade < 30: " << f1 << " (" << (double)f1*100/totalEleitos << "%)" << std::endl;
    std::cout << "30 <= Idade < 40: " << f2 << " (" << (double)f2*100/totalEleitos << "%)" << std::endl;
    std::cout << "40 <= Idade < 50: " << f3 << " (" << (double)f3*100/totalEleitos << "%)" << std::endl;
    std::cout << "50 <= Idade < 60: " << f4 << " (" << (double)f4*100/totalEleitos << "%)" << std::endl;
    std::cout << "60 <= Idade     : " << f5 << " (" << (double)f5*100/totalEleitos << "%)" << std::endl;
    std::cout << std::endl;
}

void Relatorio::printDistribuicaoGenero() {
    std::cout << "Eleitos, por gênero:" << std::endl;
    int totalEleitos = eleicao.getNumeroCandidatosEleitos();

    std::cout << std::fixed << std::setprecision(2);
    std::cout.imbue(std::locale(std::cout.getloc(), new pt_br_numpunct()));

    if(totalEleitos == 0) {
        std::cout << "Feminino:  0 (0,00%)" << std::endl;
        std::cout << "Masculino: 0 (0,00%)" << std::endl;
        std::cout << std::endl;
        return;
    };

    int feminino = 0, masculino = 0;
    for (const auto& [key, c] : eleicao.getCandidatos()) {
        if (c->isEleito()) {
            if (c->getCdGenero() == 4) feminino++;
            else if (c->getCdGenero() == 2) masculino++;
        }
    }

    std::cout << "Feminino:  " << feminino << " (" << (double)feminino * 100 / totalEleitos << "%)" << std::endl;
    std::cout << "Masculino: " << masculino << " (" << (double)masculino * 100 / totalEleitos << "%)" << std::endl;
    std::cout << std::endl;
}

void Relatorio::printTotalVotos() {
    int nominais = 0;
    int legenda = 0;
    for(const auto* p : eleicao.getPartidos()){
        nominais += p->getVotosNominais();
        legenda += p->getVotosLegenda();
    }
    int total = nominais + legenda;

    std::cout << "Total de votos válidos:    " << format_int(total) << std::endl;
    
    std::cout << std::fixed << std::setprecision(2);
    std::cout.imbue(std::locale(std::cout.getloc(), new pt_br_numpunct()));

    if(total == 0){
        std::cout << "Total de votos nominais:   0 (0,00%)" << std::endl;
        std::cout << "Total de votos de legenda: 0 (0,00%)" << std::endl;
        return;
    }

    std::cout << "Total de votos nominais:   " << format_int(nominais) << " (" << (double)nominais*100/total << "%)" << std::endl;
    std::cout << "Total de votos de legenda: " << format_int(legenda) << " (" << (double)legenda*100/total << "%)" << std::endl;
}

void Relatorio::gerarRelatorios() {
    printNumeroVagas();
    printCandidatosEleitos();
    printCandidatosMaisVotados();
    printPrejudicadosEBeneficiados();
    printVotacaoPorPartido();
    printPrimeiroUltimoPorPartido();
    printDistribuicaoFaixaEtaria();
    printDistribuicaoGenero();
    printTotalVotos();
}
#include "Relatorio.h"
#include "Eleicao.h"
#include "../model/Candidato.h"
#include "../model/Partido.h"
#include "../helpers.h"
#include "../date.h"

#include <iostream>
#include <algorithm>
#include <unordered_set>

static auto cmpCand(const Date& ref){
    return [&ref](const Candidato* a, const Candidato* b){
        if (a->getQtdVotos() != b->getQtdVotos())
            return a->getQtdVotos() > b->getQtdVotos();
        int ageA = ageOn(a->getDtNascimento(), ref);
        int ageB = ageOn(b->getDtNascimento(), ref);
        return ageA > ageB; // mais velho primeiro
    };
}

void Relatorio::gerarRelatorios(){
    printNumeroVagas();
    printEleitos();
    printTopN();
    printPrejudicadosEBeneficiados();
    printVotacaoPorPartido();
    printPrimeiroUltimoPorPartido();
    printFaixaEtaria();
    printGenero();
    printTotais();
}

void Relatorio::printNumeroVagas(){
    std::cout << "Numero de vagas: " << e_.getNumeroCandidatosEleitos() << "\n\n";
}

void Relatorio::printEleitos(){
    auto cand = e_.getCandidatos();
    std::sort(cand.begin(), cand.end(), cmpCand(e_.getDataEleicao()));
    std::cout << "Vereadores eleitos:\n";
    int pos=1;
    for (auto* c : cand){
        if (!c->isEleito()) continue;
        std::cout << pos++ << " - ";
        if (c->getNrFederacao() != -1) std::cout << "*";
        std::cout << c->getNmUrnaCandidato() << " (" << c->getSgPartido()
                  << ", " << format_thousands(c->getQtdVotos()) << " votos)\n";
    }
    std::cout << "\n";
}

void Relatorio::printTopN(){
    auto cand = e_.getCandidatos();
    std::sort(cand.begin(), cand.end(), cmpCand(e_.getDataEleicao()));
    int n = e_.getNumeroCandidatosEleitos();
    std::cout << "Candidatos mais votados (em ordem decrescente de votacao e respeitando numero de vagas):\n";
    for (int i=0;i<n && i<(int)cand.size();++i){
        auto* c = cand[i];
        std::cout << (i+1) << " - ";
        if (c->getNrFederacao() != -1) std::cout << "*";
        std::cout << c->getNmUrnaCandidato() << " (" << c->getSgPartido()
                  << ", " << format_thousands(c->getQtdVotos()) << " votos)\n";
    }
    std::cout << "\n";
}

void Relatorio::printPrejudicadosEBeneficiados(){
    auto ranking = e_.getCandidatos();
    std::sort(ranking.begin(), ranking.end(), cmpCand(e_.getDataEleicao()));

    int n = e_.getNumeroCandidatosEleitos();
    std::vector<Candidato*> topN;
    for (int i=0;i<n && i<(int)ranking.size();++i) topN.push_back(ranking[i]);

    std::unordered_set<const Candidato*> topSet(topN.begin(), topN.end());

    std::vector<Candidato*> prejudicados, beneficiados;
    for (size_t i=0;i<ranking.size();++i){
        Candidato* c = ranking[i];
        bool inTop = topSet.count(c)>0;
        if (inTop && !c->isEleito()) prejudicados.push_back(c);
        if (!inTop && c->isEleito()) beneficiados.push_back(c);
    }

    std::cout << "Teriam sido eleitos se a votacao fosse majoritaria, e nao foram eleitos:\n";
    std::cout << "(com sua posicao no ranking de mais votados)\n";
    for (auto* c : prejudicados){
        int pos = 1 + (int)(std::find(ranking.begin(), ranking.end(), c) - ranking.begin());
        std::cout << pos << " - ";
        if (c->getNrFederacao() != -1) std::cout << "*";
        std::cout << c->getNmUrnaCandidato() << " (" << c->getSgPartido()
                  << ", " << format_thousands(c->getQtdVotos()) << " votos)\n";
    }
    std::cout << "\nEleitos, que se beneficiaram do sistema proporcional:\n";
    std::cout << "(com sua posicao no ranking de mais votados)\n";
    for (auto* c : beneficiados){
        int pos = 1 + (int)(std::find(ranking.begin(), ranking.end(), c) - ranking.begin());
        std::cout << pos << " - ";
        if (c->getNrFederacao() != -1) std::cout << "*";
        std::cout << c->getNmUrnaCandidato() << " (" << c->getSgPartido()
                  << ", " << format_thousands(c->getQtdVotos()) << " votos)\n";
    }
    std::cout << "\n";
}

void Relatorio::printVotacaoPorPartido(){
    auto partidos = e_.getPartidos();
    // ordenar: total desc; empate por numero asc
    std::sort(partidos.begin(), partidos.end(), [](const Partido* a, const Partido* b){
        int ta = a->getTotalVotos(), tb = b->getTotalVotos();
        if (ta != tb) return ta > tb;
        return a->getNumero() < b->getNumero();
    });
    std::cout << "Votacao dos partidos e numero de candidatos eleitos:\n";
    int idx=1;
    for (auto* p : partidos){
        int nom = p->getVotosNominais();
        int leg = p->getVotosLegenda();
        int tot = nom + leg;
        // conta eleitos no partido
        int eleitos = 0;
        for (auto* c : p->getCandidatos()) if (c->isEleito()) eleitos++;

        std::cout << (idx<10 ? " " : "") << idx << " - "
                  << p->getSigla() << " - " << p->getNumero() << ", "
                  << format_thousands(tot) << " " << (tot<=1? "voto" : "votos") << " ("
                  << format_thousands(nom) << " " << (nom<=1? "nominal" : "nominais") << " e "
                  << format_thousands(leg) << " de legenda), "
                  << eleitos << " " << ((eleitos==1) ? "candidato eleito" : "candidatos eleitos") << "\n";
        idx++;
    }
    std::cout << "\n";
}

void Relatorio::printPrimeiroUltimoPorPartido(){
    auto partidos = e_.getPartidos();
    const Date& ref = e_.getDataEleicao();

    // ordenar candidatos em cada partido
    for (auto* p : partidos){
        auto& v = p->getCandidatos();
        std::sort(v.begin(), v.end(), cmpCand(ref));
    }

    // coletar primeiro de partidos com pelo menos 2 candidatos
    std::vector<Candidato*> primeiros;
    for (auto* p : partidos){
        if (p->getCandidatos().size() >= 2){
            primeiros.push_back(p->getCandidatos().front());
        }
    }

    // ordenar a lista de primeiros: votos do primeiro desc, numero partido asc, idade desc
    std::sort(primeiros.begin(), primeiros.end(), [&ref](const Candidato* a, const Candidato* b){
        if (a->getQtdVotos() != b->getQtdVotos()) return a->getQtdVotos() > b->getQtdVotos();
        if (a->getNrPartido() != b->getNrPartido()) return a->getNrPartido() < b->getNrPartido();
        int ageA = ageOn(a->getDtNascimento(), ref);
        int ageB = ageOn(b->getDtNascimento(), ref);
        return ageA > ageB;
    });

    std::cout << "Primeiro e ultimo colocados de cada partido:\n";
    int idx=1;
    for (auto* c : primeiros){
        // ultimo do mesmo partido
        // reobter partido pelo numero (mais robusto)
        // (na nossa modelagem Candidato tem nrPartido; as listas foram ordenadas)
        Partido* partido = nullptr;
        for (auto* p : partidos) if (p->getNumero() == c->getNrPartido()) { partido = p; break; }
        if (!partido) continue;
        const auto& lista = partido->getCandidatos();
        if (lista.size() < 2) continue;
        const Candidato* ultimo = lista.back();

        if (idx < 10) std::cout << ' ';
            std::cout << idx << " - "
          << c->getSgPartido() << " - " << c->getNrPartido() << ", "
          << c->getNmUrnaCandidato() << " (" << c->getNrCandidato() << ", "
          << format_thousands(c->getQtdVotos()) << " " << Relatorio::palavraVoto(c->getQtdVotos()) << ") / "
          << ultimo->getNmUrnaCandidato() << " (" << ultimo->getNrCandidato() << ", "
          << format_thousands(ultimo->getQtdVotos()) << " " << Relatorio::palavraVoto(ultimo->getQtdVotos()) << ")\n";
            idx++;
    }
    std::cout << "\n";
}

void Relatorio::printFaixaEtaria(){
    auto cand = e_.getCandidatos();
    const Date& ref = e_.getDataEleicao();
    int faixas[5] = {0,0,0,0,0};
    int total=0;
    for (auto* c : cand){
        if (!c->isEleito()) continue;
        int idade = ageOn(c->getDtNascimento(), ref);
        if      (idade < 30) faixas[0]++;
        else if (idade < 40) faixas[1]++;
        else if (idade < 50) faixas[2]++;
        else if (idade < 60) faixas[3]++;
        else                 faixas[4]++;
        total++;
    }
    auto pct=[&](int v)->std::string{
        double p = total>0 ? (100.0 * v / total) : 0.0;
        return format_percent(p);
    };
    std::cout << "Eleitos, por faixa etaria (na data da eleicao):\n";
    std::cout << "      Idade < 30:   " << faixas[0] << " (" << pct(faixas[0]) << "%)\n";
    std::cout << "30 <= Idade < 40:   " << faixas[1] << " (" << pct(faixas[1]) << "%)\n";
    std::cout << "40 <= Idade < 50:   " << faixas[2] << " (" << pct(faixas[2]) << "%)\n";
    std::cout << "50 <= Idade < 60:   " << faixas[3] << " (" << pct(faixas[3]) << "%)\n";
    std::cout << "60 <= Idade     :   " << faixas[4] << " (" << pct(faixas[4]) << "%)\n\n";
}

void Relatorio::printGenero(){
    auto cand = e_.getCandidatos();
    int fem=0, mas=0, total=0;
    for (auto* c : cand){
        if (!c->isEleito()) continue;
        if (c->getCdGenero() == 4) fem++;
        else if (c->getCdGenero() == 2) mas++;
        total++;
    }
    auto pct=[&](int v)->std::string{
        double p = total>0 ? (100.0 * v / total) : 0.0;
        return format_percent(p);
    };
    std::cout << "Eleitos, por genero:\n";
    std::cout << "Feminino:  " << fem << " (" << pct(fem) << "%)\n";
    std::cout << "Masculino: " << mas << " (" << pct(mas) << "%)\n\n";
}

void Relatorio::printTotais(){
    auto cand = e_.getCandidatos();
    auto partidos = e_.getPartidos();
    long long nom=0, leg=0;
    for (auto* c : cand) nom += c->getQtdVotos();
    for (auto* p : partidos) leg += p->getVotosLegenda();
    long long tot = nom + leg;
    double pNom = tot>0 ? (100.0 * nom / tot) : 0.0;
    double pLeg = tot>0 ? (100.0 * leg / tot) : 0.0;
    std::cout << "Total de votos validos:    " << format_thousands(tot) << "\n";
    std::cout << "Total de votos nominais:   " << format_thousands(nom) << " (" << format_percent(pNom) << "%)\n";
    std::cout << "Total de votos de legenda: " << format_thousands(leg) << " (" << format_percent(pLeg) << "%)\n";
}
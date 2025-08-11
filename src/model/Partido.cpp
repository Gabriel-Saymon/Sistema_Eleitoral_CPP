#include "Partido.h"
#include "Candidato.h"

int Partido::getVotosNominais() const {
    int sum = 0;
    for (auto* c : candidatos_) sum += c->getQtdVotos();
    return sum;
}
#include "Candidato.h"
#include "Partido.h"

Candidato::Candidato(std::string sgUe, int cargo, int nrCandidato,
                     std::string nmCandidato, std::string nmUrna,
                     int nrPartido, std::string sgPartido, int nrFederacao,
                     Date dtNascimento, int cdGenero, int cdSitTotTurno,
                     Partido* partido)
: sgUe_(std::move(sgUe)),
  cargo_(cargo),
  nrCandidato_(nrCandidato),
  nmCandidato_(std::move(nmCandidato)),
  nmUrna_(std::move(nmUrna)),
  nrPartido_(nrPartido),
  sgPartido_(std::move(sgPartido)),
  nrFederacao_(nrFederacao),
  dtNascimento_(dtNascimento),
  cdGenero_(cdGenero),
  cdSitTotTurno_(cdSitTotTurno),
  partido_(partido)
{}
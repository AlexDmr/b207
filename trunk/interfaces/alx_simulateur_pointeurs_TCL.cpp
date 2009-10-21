#include "alx_simulateur_pointeurs_TCL.h"

//_____________________________________________________________________________
//_____________________________________________________________________________
//_____________________________________________________________________________
alx_simulateur_pointeurs_TCL::alx_simulateur_pointeurs_TCL( int *e_x, int *e_y
                                                          , alx_point2D *bg
                                                          , alx_vecteur2D *vh
                                                          , alx_vecteur2D *vv) : alx_simulateur_pointeurs_generique( e_x, e_y
                                                                                                                   , bg
                                                                                                                   , vh
                                                                                                                   , vv )
{}

//_____________________________________________________________________________
//_____________________________________________________________________________
//_____________________________________________________________________________
void alx_simulateur_pointeurs_TCL::abonner_a_simulation(alx_methode_rappel *m)
{while( desabonner_de_simulation(m) );
 L_rap_simulation.Ajouter_methode_a_la_fin( m );
}
//_____________________________________________________________________________
const bool alx_simulateur_pointeurs_TCL::desabonner_de_simulation(alx_methode_rappel *m)
{return L_rap_simulation.Retirer_methode( m );
}


void alx_simulateur_pointeurs_TCL::simuler()
{L_rap_simulation.Rappeler( this );
}

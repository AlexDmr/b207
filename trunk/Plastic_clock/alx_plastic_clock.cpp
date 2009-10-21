#include "alx_plastic_clock.h"

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
alx_plastic_clock::alx_plastic_clock(const double tx, const double ty, alx_interface_sdl_opengl_1 *interface_m)
{fen_plastic_clock = new alx_noeud_fiche_sdl_opengl("Plastic clock", tx, ty, interface_m);
 horloge_paris     = new alx_noeud_fiche_ovale_sdl_opengl("Paris"   , 64, 64, interface_m);
 horloge_new_york  = new alx_noeud_fiche_ovale_sdl_opengl("New-York", 64, 64, interface_m);

 fen_plastic_clock->Ajouter_fils( horloge_paris    );
 fen_plastic_clock->Ajouter_fils( horloge_new_york );

 
}

//______________________________________________________________________________
alx_plastic_clock::~alx_plastic_clock()
{}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________


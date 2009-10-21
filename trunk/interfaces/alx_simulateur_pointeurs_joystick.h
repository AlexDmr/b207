#ifndef __ALX_SIMULATEUR_POINTEURS_JOYSTICK_H__
#define __ALX_SIMULATEUR_POINTEURS_JOYSTICK_H__

#include "alx_simulateur_pointeurs_generique.h"
#include <SDL.h>

class Etat_joystick
{public:
   int x, y, boutons;
   Etat_joystick() {}
   Etat_joystick(const int x, const int y, const int b)
    {this->x       = x;
     this->y       = y;
     this->boutons = b;}
};
 
class alx_simulateur_pointeurs_joystick : public alx_simulateur_pointeurs_generique
{private:
   Etat_joystick *etat_joysticks; 
   int nb_joysticks;
   
 public :
   alx_simulateur_pointeurs_joystick( int *e_x, int *e_y
                                    , alx_point2D *bg
                                    , alx_vecteur2D *vh, alx_vecteur2D *vv
                                    , int nb_joysticks);

   //int  Ajouter_rapporteur(alx_simulateur_pointeurs *r);
   //bool Retirer_rapporteur(alx_simulateur_pointeurs *r);

   void simuler();
};

#endif

 
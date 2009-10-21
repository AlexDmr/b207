#ifndef __ALX_NOEUD_TAPIS_H__
#define __ALX_NOEUD_TAPIS_H__

#include "..\..\physique\alx_classes_bases.h"

#include "..\..\physique\alx_model_simulation_physique1.h"
#include "..\..\physique\alx_model_simulation_bidon1.h"

#include "..\..\interfaces\alx_noeud_scene.h"

#include "..\..\opengl\alx_sommet_opengl.h"
#include "..\..\opengl\config_opengl.h"

//------------------------------------------------------------------------------
// Les messages:
//  0 : Contact
//    0 : avec un point
//      * : adresse du point
//------------------------------------------------------------------------------
class alx_noeud_tapis : public alx_noeud_scene, public alx_classe_base_liant
{
 private:
   alx_model_simulation_physique1 mp_tapis;
   void redim_zone(const double lg, const double ht);
   void Point_contact_detecte(alx_point2D *pt);
   
 public :
  // Les constructeurs
   alx_noeud_tapis(const double lg, const double ht);

  // les méthodes
   void Afficher();
   info_du_contenant* Contient(alx_point2D &pt, int action);

   void rationnaliser(int num);
};

typedef alx_noeud_tapis* P_alx_noeud_tapis;

#endif


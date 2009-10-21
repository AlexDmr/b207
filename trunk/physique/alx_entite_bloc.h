#ifndef __ALX_ENTITE_BLOC_H__
#define __ALX_ENTITE_BLOC_H__

#include "alx_classes_bases.h"
#include "alx_model_simulation_physique1.h"
#include "alx_model_simulation_bidon1.h"


class alx_entite_bloc : public alx_classe_base_liant
{
 protected:
  void init_models();

 public:
 // les différents modèles :
  alx_model_simulation_physique1   model_classique_et_choc;
  alx_model_simulation_bidon1      model_boite;
//  alx_model_simulation_bidon1      model_boite_mouvement;

 // Les constructeurs :
  alx_entite_bloc();
  alx_entite_bloc(double e);
  
 // Les méthodes :
  void rationnaliser(int num_model);
  void Ajouter_segment2D(const alx_segment2D &);
  void Ajouter_segment2D(const alx_point2D &, alx_point2D &);
  
  void maj(const alx_entite_bloc &b)
   {
    // Mise à jour des modèles
    model_classique_et_choc.maj( b.model_classique_et_choc );
    model_boite.maj( b.model_boite );
    // Pas de rationnalisation à faire.
   }
   
  alx_entite_bloc &operator =(const alx_entite_bloc &b)    // Surcharge du  =
   {
    maj(b);
    return *this;
   }

};

#endif
 
#ifndef __ALX_ENTITE_PALET_H__
#define __ALX_ENTITE_PALET_H__

#include "alx_classes_bases.h"
#include "alx_model_simulation_physique1.h"
#include "alx_model_simulation_bidon1.h"

class alx_entite_palet : public alx_classe_base_liant
{
 protected:
  void init_models();

 public:
 // les différents modèles :
  alx_model_simulation_physique1   model_classique_et_choc;
//  alx_model_simulation_bidon1      model_boite;
  alx_model_simulation_bidon1      model_boite_mouvement;

 // Les constructeurs :
  alx_entite_palet();
  alx_entite_palet( const double rayon, const double masse, const double elasticite
                  , const alx_point2D &depart, const alx_point2D &arrive
                  , const double t_deb, const double t_fin);

 // Les méthodes :
  void rationnaliser(int num_model);

  void maj(const alx_entite_palet &p)
   {
    // Mise à jour des modèles
    model_classique_et_choc.maj( p.model_classique_et_choc );
    model_boite_mouvement.maj( p.model_boite_mouvement );
    // Pas de rationnalisation à faire.
   }
   
  alx_entite_palet &operator =(const alx_entite_palet &p)    // Surcharge du  =
   {
    maj(p);
    return *this;
   }

};

#endif

 

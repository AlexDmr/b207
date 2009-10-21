#ifndef __ALX_SIMULATEUR_BOITE_RECTANGULAIRE_H__
#define __ALX_SIMULATEUR_BOITE_RECTANGULAIRE_H__

//#include <list.h>
#include "alx_classes_bases.h"
#include "alx_model_simulation_bidon1.h"
#include "alx_geometrie2D.h"

class alx_simulateur_boite_rectangulaire : alx_classe_base_liant
{
 public:
  double est, ouest, nord, sud;
  alx_liste<alx_model_simulation_bidon1*> L_entite;

 // les constructeurs :
  alx_simulateur_boite_rectangulaire();
  alx_simulateur_boite_rectangulaire( double e
                                    , double o
                                    , double s
                                    , double n);
  ~alx_simulateur_boite_rectangulaire();

 // Les méthodes :
  void inscrire(alx_model_simulation_bidon1 *m);
  void retirer (alx_model_simulation_bidon1 *m);
  void retirer (int num);

  void simuler();
};

#endif

 
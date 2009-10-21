#ifndef __ALX_MODEL_SIMULATION_BIDON1_H__
#define __ALX_MODEL_SIMULATION_BIDON1_H__

//#include <list.h>
#include "alx_physique2D.h"
#include "alx_classes_bases.h"

/******************************************************************************/
/*********************** alx_model_simulation_physique1 ***********************/
/******************************************************************************/
class alx_model_simulation_bidon1 : public alx_classe_base_model
{
 public:
  // Les données:
   double x, y, l, h; // frontières de la zone englobante.

  // Les constructeurs:
   alx_model_simulation_bidon1();
   ~alx_model_simulation_bidon1();

  // Les méthodes:
   void Construire(double x1, double x2, double y1, double y2);
   bool Recouvre  (alx_model_simulation_bidon1 &);
   bool Recouvre_x(alx_model_simulation_bidon1 &);
   bool Recouvre_y(alx_model_simulation_bidon1 &);

   void maj(const alx_model_simulation_bidon1 &);
   alx_model_simulation_bidon1 &operator =(const alx_model_simulation_bidon1 &);    // Surcharge du  =
};

#endif


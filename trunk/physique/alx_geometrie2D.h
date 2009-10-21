#ifndef __ALXGEOM2DH__
#define __ALXGEOM2DH__


class alx_deplacement2D;
class alx_droite2D;
class alx_segment2D;
class alx_point2D;
class alx_vecteur2D;
class alx_primitive2D;
class alx_ellipse2D;


#include "alx_type.h"
#include "math_alex.h"
#include "alx_point2D.h"
#include "alx_vecteur2D.h"

#include "alx_ellipse2D.h"
#include "alx_cercle2D.h"
#include "alx_droite2D.h"
#include "alx_segment2D.h"

#include "alx_deplacement2D.h"

#include "alx_primitive2D.h"

#include "constantes.h"
/*
extern const alx_point2D   pt_nul; //(0.0, 0.0);
extern const alx_vecteur2D vect_h  //(1.0, 0.0)
                         , vect_v; //(0.0, 1.0);
extern const alx_droite2D  axe_h  //(pt_nul, vect_h)
                        ,  axe_v; //(pt_nul, vect_v);
*/

inline const double Interpolation_lineaire( const double a
                                          , const double b
                                          , const double t )
{return a+(b-a)*t;}

inline void Interpolation_lineaire_V( const unsigned int n
                                    , const double *V1
                                    , const double *V2
                                    , const double t
                                    , double *VR )
{unsigned int i;
 for(i=0;i<n;i++)
   VR[i] = Interpolation_lineaire(V1[i], V2[i], t);
}

#endif

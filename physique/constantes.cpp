#include "constantes.h"

const double PI = 3.141592653589;
const double EPSILON = 0.00001;

const alx_point2D   pt_nul(0.0, 0.0);
const alx_vecteur2D vect_h(1.0, 0.0)
                  , vect_v(0.0, 1.0);
const alx_droite2D  axe_h(pt_nul, vect_h)
                 ,  axe_v(pt_nul, vect_v);


const alx_point2D&   Pt_nul() {return pt_nul;}
const alx_vecteur2D& Vect_h() {return vect_h;}
const alx_vecteur2D& Vect_v() {return vect_v;}
const alx_droite2D&  Axe_h()  {return axe_h;}
const alx_droite2D&  Axe_v()  {return axe_v;}



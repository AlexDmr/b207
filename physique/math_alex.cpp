#ifndef __ALXMATHCPP__
#define __ALXMATHCPP__

#include  "math_alex.h"

template <class T> inline T Amax( T a, T b) {return a>b ? a : b; }
template <class T> inline T Amin( T a, T b) {return a<b ? a : b; }

template <class Nombre> inline Nombre Asqr(Nombre x)
{return x*x;}

template <class Nombre> inline Nombre absd(Nombre x)
{if(x>0) return x;
  else return -x;}

template <class Nombre> inline int signe(Nombre x)
{if(x >= 0) return  1;
 return -1;}

#endif


#ifndef __ALXMATH__
#define __ALXMATH__

#include <math.h>
#include <stdlib.h>

extern const double EPSILON; // = 0.00001;
extern const double PI;      // = 3.141592653589;


template <class Nombre> inline Nombre Asqr (Nombre x);
template <class Nombre> inline Nombre absd (Nombre x);

template <class T> inline T Amax( T a, T b);// {return a>b ? a : b; }
template <class T> inline T Amin( T a, T b);// {return a<b ? a : b; }

inline double Modulo_double(double v, const double base)
{while(v<=0)    v += base;
 while(v>=base) v -= base;
 return v;
}

inline const unsigned int Puissance_2_sup(const unsigned int n, const unsigned int m)
{unsigned int rep = m;
 while(rep < n)
   rep = rep<<1; 
 return rep;
}
#endif


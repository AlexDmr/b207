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

double Modulo_double(double v, const double base);

#endif


#include "alx_model_simulation_bidon1.h"


// Les constructeurs:
alx_model_simulation_bidon1::alx_model_simulation_bidon1()
{
 x = 0.0;
 y = 0.0;
 l = 0.0;
 h = 0.0;
}

alx_model_simulation_bidon1::~alx_model_simulation_bidon1()
{}

// Les méthodes:
void alx_model_simulation_bidon1::Construire(double x1, double x2, double y1, double y2)
{
 if(x1 < x2)
  {x = x1;
   l = x2 - x1;}
  else
   {x = x2;
    l = x1 - x2;}

 if(y1 < y2)
  {y = y1;
   h = y2 - y1;}
  else
   {y = y2;
    h = y1 - y2;}
}

bool alx_model_simulation_bidon1::Recouvre_x(alx_model_simulation_bidon1 &m)
{
 return !( (x+l < m.x)
         ||(m.x + m.l < x) );
}

bool alx_model_simulation_bidon1::Recouvre_y(alx_model_simulation_bidon1 &m)
{
 return !( (y+h < m.y)
         ||(m.y + m.h < y) );
}

bool alx_model_simulation_bidon1::Recouvre(alx_model_simulation_bidon1 &m)
{
 return !( (x+l < m.x)
         ||(m.x + m.l < x)
         ||(y+h < m.y)
         ||(m.y + m.h < y) );
}

void alx_model_simulation_bidon1::maj(const alx_model_simulation_bidon1 &m)
{x = m.x;
 y = m.y;
 l = m.l;
 h = m.h;}

alx_model_simulation_bidon1 &alx_model_simulation_bidon1::operator =(const alx_model_simulation_bidon1 &m)
{maj(m);
 return *this;}




 
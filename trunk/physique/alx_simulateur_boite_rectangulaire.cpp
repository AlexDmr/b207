#include "alx_simulateur_boite_rectangulaire.h"

alx_simulateur_boite_rectangulaire::alx_simulateur_boite_rectangulaire()
{est = ouest = nord = sud = 0.0;}

alx_simulateur_boite_rectangulaire::alx_simulateur_boite_rectangulaire( double e
                                                                      , double o
                                                                      , double s
                                                                      , double n)

{est   = e;
 ouest = o;
 sud   = s;
 nord  = n;}

alx_simulateur_boite_rectangulaire::~alx_simulateur_boite_rectangulaire()
{L_entite.Vider();}

// Les méthodes :
void alx_simulateur_boite_rectangulaire::inscrire(alx_model_simulation_bidon1 *m)
{L_entite.Ajouter_au_debut(m);}

void alx_simulateur_boite_rectangulaire::retirer (alx_model_simulation_bidon1 *m)
{
 alx_element_liste<alx_model_simulation_bidon1*> *it, *fin;

 fin = L_entite.Fin();
 it  = L_entite.Premier();

 while( (it  != fin)
      &&(it->E() != m) )
  {it = it->svt;}

 if(it != fin) L_entite.Retirer(it);
}

void alx_simulateur_boite_rectangulaire::retirer (int num)
{
 int pos = 0;

 alx_element_liste<alx_model_simulation_bidon1*> *it, *fin;

 fin = L_entite.Fin();
 it  = L_entite.Premier();

 while( (it  != fin)
      &&(pos != num) )
  {it  = it->svt;
   pos++;}

 if(it != fin) L_entite.Retirer(it);
}

void alx_simulateur_boite_rectangulaire::simuler()
{
 alx_element_liste<alx_model_simulation_bidon1*> *it, *fin;
 alx_model_simulation_bidon1 *ptr;
 
 fin = L_entite.Fin();

 for(it=L_entite.Premier(); it!=fin;it=it->svt)
  {
   ptr = it->E();
   if( (ptr->y+ptr->h > sud)
     ||(ptr->y   < nord) )
    {
     ptr->infos_comp[0] = 1;
     ptr->nb_infos_comp = 1;
     ptr->Oyez_oyez();
    }

   if( (ptr->x+ptr->l > ouest)
     ||(ptr->x   < est) )
    {
     ptr->infos_comp[0] = 2;
     ptr->nb_infos_comp = 1;
     ptr->Oyez_oyez();
    }

  }
}



 
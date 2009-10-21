#include "alx_entite_bloc.h"
#include "math_alex.cpp"

void alx_entite_bloc::init_models()
{
 alx_point2D tmp(0.0, 0.0);
 
 model_classique_et_choc.L_dep.Ajouter_au_debut( alx_deplacement2D(tmp, tmp, 0.0, 1.0) );
 model_classique_et_choc.mobilite = Talx_immobile;
 model_classique_et_choc.abonner_a(this, 1);
 model_boite.abonner_a(this, 2);
}

 // Les constructeurs :
alx_entite_bloc::alx_entite_bloc(double e)
{
 init_models();
 model_classique_et_choc.elasticite = e;
}

alx_entite_bloc::alx_entite_bloc()
{
 init_models();
 model_classique_et_choc.elasticite = 1.0;
}

/******************************************************************************/
/******************************* Les méthodes *********************************/
/******************************************************************************/
void alx_entite_bloc::Ajouter_segment2D(const alx_segment2D &s)
{
 model_classique_et_choc.Ajouter_segment2D(s);
}

void alx_entite_bloc::Ajouter_segment2D(const alx_point2D &pt_deb, alx_point2D &pt_fin)
{
 model_classique_et_choc.Ajouter_segment2D(pt_deb, pt_fin);
}

/******************************************************************************/
void alx_entite_bloc::rationnaliser(int num_model)
{
 alx_liste<alx_segment2D> *L_seg = & (model_classique_et_choc.L_segment2D);
 alx_element_liste<alx_segment2D> *it_seg, *it_seg_fin;

 double minx, miny, maxx, maxy;

 switch(num_model)
  {
   case 1: // Alerte du modèle physique
    // On re-calcule la boite englobante par rapport au dernier déplacement connu;
    if(model_classique_et_choc.nb_infos_comp < 1) return;
    switch(model_classique_et_choc.infos_comp[0])
     {
      case 0: // Maj de la position (translation ou rotation...)
        if(model_classique_et_choc.nb_infos_comp < 2) return;
        switch(model_classique_et_choc.infos_comp[1])
         {
          case 0: // Translation.
            model_boite.x += model_classique_et_choc.position.dx;
            model_boite.y += model_classique_et_choc.position.dy;
          break;
          case 1: // Rotation.
          break;
         }
      break;
      case 1: // Maj des composants, insertion d'un nouveau
       if(L_seg->Taille() > 0)
        {
         it_seg = L_seg->Premier();
         minx = Amin( (it_seg->E()).a.x, (it_seg->E()).b.x);
         maxx = Amax( (it_seg->E()).a.x, (it_seg->E()).b.x);
         miny = Amin( (it_seg->E()).a.y, (it_seg->E()).b.y);
         maxy = Amax( (it_seg->E()).a.y, (it_seg->E()).b.y);
         it_seg_fin = L_seg->Fin();
         it_seg = it_seg->svt;
         for(; it_seg!=it_seg_fin; it_seg = it_seg->svt)
          {
           minx = Amin( Amin( (it_seg->E()).a.x, (it_seg->E()).b.x), minx);
           maxx = Amax( Amax( (it_seg->E()).a.x, (it_seg->E()).b.x), maxx);
           miny = Amin( Amin( (it_seg->E()).a.y, (it_seg->E()).b.y), miny);
           maxy = Amax( Amax( (it_seg->E()).a.y, (it_seg->E()).b.y), maxy);
          }
        }

       model_boite.Construire( minx
                             , maxx
                             , miny
                             , maxy );
      break;
     }
   break;
  }
}


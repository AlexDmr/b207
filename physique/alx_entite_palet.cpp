#include "alx_entite_palet.h"
#include "math_alex.cpp"

void alx_entite_palet::init_models()
{
 model_classique_et_choc.mobilite = Talx_mobile;
 model_classique_et_choc.abonner_a(this, 1);
 model_boite_mouvement.abonner_a(this, 2);
}

/*******************************************************************************/
alx_entite_palet::alx_entite_palet()
{
 init_models();
 alx_point2D pt(0.0, 0.0);
 model_classique_et_choc.L_dep.Ajouter_a_la_fin( alx_deplacement2D(pt, pt, 0.0, 1.0) );
 model_boite_mouvement.Construire(-1.0, 1.0, -1.0, 1.0);

 model_classique_et_choc.Ajouter_cercle2D_phys( alx_cercle2D_physique(1.0, 1.0, 1.0), 1.0 );
}

/*******************************************************************************/
alx_entite_palet::alx_entite_palet( const double rayon, const double masse, const double elasticite
                                  , const alx_point2D &depart, const alx_point2D &arrive
                                  , const double t_deb, const double t_fin)
{
 init_models();
 model_boite_mouvement.Construire( Amin(depart.x, arrive.x) - rayon
                                 , Amax(depart.x, arrive.x) + rayon
                                 , Amin(depart.y, arrive.y) - rayon
                                 , Amax(depart.y, arrive.y) + rayon );

 model_classique_et_choc.vitesse.maj(depart, arrive);
 model_classique_et_choc.L_dep.Ajouter_a_la_fin( alx_deplacement2D(depart, arrive, t_deb, t_fin) );
 model_classique_et_choc.position.maj(arrive.x, arrive.y);

 model_classique_et_choc.Ajouter_cercle2D_phys( alx_cercle2D_physique( masse
                                                                     , elasticite
                                                                     , rayon)
                                              , masse );
 
}




/*******************************************************************************/
/*******************************************************************************/
/*******************************************************************************/
void alx_entite_palet::rationnaliser(int num_model)
{
/* if( model_classique_et_choc.L_dep.empty()
   ||model_classique_et_choc.L_cercle2D_physique.empty() )
  {
   bool a = model_classique_et_choc.L_dep.empty()
      , b = model_classique_et_choc.L_cercle2D_physique.empty();
   return; // Ca sent la mort!!!
  } */

 int sens = model_boite_mouvement.infos_comp[0];
 alx_cercle2D *c;
 c = &( model_classique_et_choc.L_cercle2D_physique.Premier_E() );
 double /*x = model_classique_et_choc.position.dx + c->x
      , y = model_classique_et_choc.position.dy + c->y
      ,*/ r = c->rayon
      , elast;

 alx_deplacement2D *der_dep = &( model_classique_et_choc.L_dep.Dernier_E() );

 alx_segment2D *der_seg = &( der_dep->seg );

 alx_point2D *pt_deb = &(der_seg->a)
           , *pt_fin = &(der_seg->b);

 /*if( (der_dep->dep.dx > 1000.0)||(der_dep->dep.dx < -1000.0)
   ||(der_dep->dep.dy > 1000.0)||(der_dep->dep.dy < -1000.0) )
  {
   int t = model_classique_et_choc.L_dep.size();
   der_dep->dep.dx = (double)t;
  }*/

 switch(num_model)
  {
   case 1: // Alerte du modèle physique
    // On re-calcule la boite englobante par rapport au dernier déplacement connu;
    model_boite_mouvement.Construire( Amin(pt_deb->x, pt_fin->x) - r
                                    , Amax(pt_deb->x, pt_fin->x) + r
                                    , Amin(pt_deb->y, pt_fin->y) - r
                                    , Amax(pt_deb->y, pt_fin->y) + r );
   break;

   case 2: // Alerte du modele pour la boite
     //model_classique_et_choc.position.Soustraire( der_dep->dep );
     elast = model_classique_et_choc.L_cercle2D_physique.Premier_E().elasticite;

     if(sens == 1) // nord-sud
      {// Inversion de la vitesse et retour à la position précédante.
       model_classique_et_choc.position.dy -= der_dep->dep.dy;
       model_classique_et_choc.vitesse.dy = -elast*model_classique_et_choc.vitesse.dy;
       model_classique_et_choc.L_dep.Ajouter_a_la_fin( alx_deplacement2D( *pt_fin
                                                                 , *pt_deb
                                                                 , der_dep->t_fin
                                                                 , der_dep->t_fin) );
      }
      else // est-ouest
       {
        model_classique_et_choc.position.dx -= der_dep->dep.dx;
        model_classique_et_choc.vitesse.dx = -elast*model_classique_et_choc.vitesse.dx;
        model_classique_et_choc.L_dep.Ajouter_a_la_fin( alx_deplacement2D( der_dep->seg.b
                                                                  , der_dep->seg.a
                                                                  , der_dep->t_fin
                                                                  , der_dep->t_fin) );
       }
   break;
  }
}


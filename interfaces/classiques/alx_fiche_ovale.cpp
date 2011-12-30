#include "alx_fiche_ovale.h"

#include <stdio.h>
#include <stdlib.h>
#include "..\..\physique\math_alex.cpp"
#include "..\..\physique\constantes.h"

/*******************************************************************************/
/******************************* Les constructeurs******************************/
/*******************************************************************************/
void alx_fiche_ovale::init()
{
// int i, pos;

 h_base = 0.0;
 mp_entete.abonner_a(this, 1);
 mp_corp.abonner_a(this, 2);
 mp_zone_rot.abonner_a(this, 3);
 mp_zone_redim.abonner_a(this, 4);

 alx_ellipse2D e;

 mp_corp.Ajouter_ellipse2D(e, 1.0);
 mp_entete.Ajouter_ellipse2D(e, 1.0);
 mp_entete.Ajouter_droite2D( alx_droite2D(Pt_nul(), Vect_h()) );
 mp_zone_rot.Ajouter_cercle2D_phys( alx_cercle2D_physique(0.0, 0.0, 1.0, 1.0, 0.0)
                                  , 1.0);
 mp_zone_redim.Ajouter_ellipse2D(e, 1.0);
/*
 mp_corp.position.maj(Longueur_corp()/2, Hauteur_corp()/2);
 mp_entete.position.maj(Longueur_corp()/2, Hauteur_corp()/2);
 mp_zone_rot.position.maj(Longueur_corp()/2, Hauteur_corp()/2);
 mp_zone_redim.position.maj(Longueur_corp()/2, Hauteur_corp()/2);
*/
}

/*******************************************************************************/
void alx_fiche_ovale::maj_mp_redim() // zone redimensionnement
{
// On chope l'ellipse du corp pour la mise à jour de celle de redimensionnement:
 alx_ellipse2D *e_redim = &(mp_zone_redim.L_ellipse2D.Premier_E())
             , *e_corp  = &(mp_corp.L_ellipse2D.Premier_E());
 e_redim->maj(*e_corp);
 e_redim->a += Epaisseur_bordure();
 e_redim->b += Epaisseur_bordure();

//------------------------------------------------
// Mise à jour des informations sur la transformation de scene liée au corp
/*
 alx_vecteur2D *pos = &(mp_corp.position);
 double         rot = mp_corp.rotation;

 mp_zone_redim.position.maj(*pos);
 mp_zone_redim.rotation = rot;
*/
}

void alx_fiche_ovale::maj_mp_zrec() // zone rot, entête, corp
{
 alx_ellipse2D *e = &(mp_corp.L_ellipse2D.Premier_E());
 e->a = Longueur_corp()/2;
 e->b = Hauteur_corp()/2;

 e = &(mp_entete.L_ellipse2D.Premier_E());
 e->a = Longueur_corp()/2+ Epaisseur_bordure() + Hauteur_entete();
 e->b = Hauteur_corp()/2 + Epaisseur_bordure() + Hauteur_entete();

 alx_cercle2D_physique *cercle = &( mp_zone_rot.L_cercle2D_physique.Premier_E() );
 cercle->maj( alx_cercle2D_physique(0, h_base-Hauteur_corp()/2, 1.0, 1.0, Rayon_zone_rotation()) );

 centre_entete.maj(0, h_base+Hauteur_corp()/2+Hauteur_entete()/2);
 centre_corp.maj(0, h_base);
 centre_zone_rot.maj(0, h_base-Hauteur_corp()/2);
}

//_______________________________________________________________________________
alx_fiche_ovale::alx_fiche_ovale() : alx_fenetre()
{
 init();
}

//_______________________________________________________________________________
alx_fiche_ovale::alx_fiche_ovale(const char *n, const double largeur, const double hauteur) : alx_fenetre(n, largeur, hauteur)
{
 init();

 Longueur_corp(largeur);
 Hauteur_corp(hauteur);
 maj_mp_zrec();
 maj_mp_redim();
}

alx_fiche_ovale::alx_fiche_ovale( const char *n, const double largeur, const double hauteur
                    , const double hauteur_entete, const double r_zone_rot) : alx_fenetre(n, largeur, hauteur, largeur, hauteur_entete, r_zone_rot)
{
 init();

 Longueur_corp(largeur);
 Hauteur_corp(hauteur);
 Appliquer_maj();
}

/*******************************************************************************/
/**********************************Les méthodes*********************************/
/*******************************************************************************/
void alx_fiche_ovale::rationnaliser(int num)
{
 alx_model_simulation_physique1 *mp;

 switch(num)
  {
   case 1: mp = &mp_entete; break;
   case 2: mp = &mp_corp; break;
   case 3: mp = &mp_zone_rot; break;
   case 4: mp = &mp_zone_redim; break;
  }

 if( (mp->nb_infos_comp == 4)
   &&(mp->infos_comp[0] == 3)                              //Si c'est une rupture du point de contact
   &&(mp->infos_comp[1] == 0)                              //sur le mp de redimensionnement
   &&(mp->infos_comp[3] == (int)(void*)&mp_zone_redim) )   //alors remettre celui-ci à sa place.
  {
   maj_mp_redim();
   return;
  }

 switch(num)
  {
   case 1: // De l'entête
   case 2: // Du corp
   case 3: // De la zone de rotation
     // Mise à jour de la zone de redimensionnement
/*     if( (mp->nb_infos_comp != 2)
       ||(mp->infos_comp[0] != 0)
       ||(mp->infos_comp[1] != 2) ) return; // on ne traite que les changements de position
*/
     if( (mp->nb_infos_comp != 4)
       ||(mp->infos_comp[0] != 5) ) return; // on ne traite que les changements de position

     maj_mp_redim();

     if(mp->infos_comp[1] == 2)
      {
       double *r      = (double*)(void*)( mp->infos_comp[2] );
       alx_point2D *p = (alx_point2D*)(void*)( mp->infos_comp[3] );
       Bloquer_rationnalisation(true);
         mp_zone_redim.Informer_Reajustement_rotation(r, p);
       Bloquer_rationnalisation(false);
      }

     if(mp->infos_comp[1] == 1)
      {
       vect_dec.maj( *((alx_vecteur2D*)(void*)(mp->infos_comp[2])) );
       Bloquer_rationnalisation(true);
         mp_zone_redim.Informer_Reajustement_translation(&vect_dec, &info_point);
       Bloquer_rationnalisation(false);
      }
   break;
   case 4: // De la zone de redimensionnement
//  6 : Contact avec un objet positionner par une suite de transformation de repère
//     0 : Un point
//        * : adresse du point
//           * : adresse du modèle physique
//              * : adresse d'une liste de transformation de repère list<alx_repere2D*>*
     if( (mp->nb_infos_comp == 5)
       &&(mp->infos_comp[0] == 6)
       &&(mp->infos_comp[1] == 0) )
      {
       l_au_contact = Longueur_corp();
       h_au_contact = Hauteur_corp();
       pt_contact_depart.maj( *((alx_point2D*)(void*)(mp->infos_comp[2])) );
       alx_liste<alx_repere2D*> *liste_repere = (alx_liste<alx_repere2D*>*)(void*)(mp->infos_comp[4]);
       //alx_model_simulation_physique1 *mp2 = (alx_model_simulation_physique1*)(void*)(mp->infos_comp[3]);

       position_initiale.maj(0,0);
       Repere_transformation(pt_contact_depart, *liste_repere);
       //repere->Changer_coordonnees(pt_contact_depart);

       alx_vecteur2D vect(pt_contact_depart);
       //redim_verticale, redim_horizontale

       alx_droite2D droite_centre_contact(Pt_nul(), vect);
       alx_point2D res1, res2, *res;
       alx_ellipse2D *e = &(mp_corp.L_ellipse2D.Premier_E());
       e->intersection(droite_centre_contact, res1, res2);

       if(vect.produit_scalaire(alx_vecteur2D(res1)) > 0) res = &res1;
        else res = &res2;

       alx_vecteur2D vect_centre_bordure( *res );

       eloignement_bordure_pt = alx_vecteur2D(pt_contact_depart).Norme()
                              - vect_centre_bordure.Norme();
       pt_contact_depart.maj(vect_centre_bordure.dx, vect_centre_bordure.dy);

      // Il faut repérer dans quelle partie de l'ellipse se trouve le point de contact
      // pour savoir COMMENT modifier l'ellipse lorsque celui ci se déplacera.
       num_zone_redim = Calculer_num_zone_redim(pt_contact_depart);
      // Selon la zone, on change de repère pour le point de contact, le nouveau repère
      // se trouvant au coin fixe du redimensionnement
       double l = l_au_contact/2
            , h = h_au_contact/2;
       switch(num_zone_redim)
        {
         case 0: pt_contact_depart.Translation( l, 0); break;
         case 1: pt_contact_depart.Translation( l, h); break;
         case 2: pt_contact_depart.Translation( 0, h); break;
         case 3: pt_contact_depart.Translation(-l, h); break;
         case 4: pt_contact_depart.Translation(-l, 0); break;
         case 5: pt_contact_depart.Translation(-l,-h); break;
         case 6: pt_contact_depart.Translation( 0,-h); break;
         case 7: pt_contact_depart.Translation( l,-h); break;
        }

       return;
      }

     if( (mp->nb_infos_comp <= 2)
       ||(mp->infos_comp[0] != 5)
       ||(mp->infos_comp[1] != 1) ) return; // on ne traite que les changements de position

     {
     // Calcul du point de contact
      alx_vecteur2D *deplacement = (alx_vecteur2D*)(void*)(mp->infos_comp[2]);
      alx_point2D   *pt_depart   = (alx_point2D*)(void*)(mp->infos_comp[3]);
      alx_vecteur2D vect( *pt_depart );
      vect.Ajouter( *deplacement );
      vect.Ajouter( *deplacement ); // Pour avoir les coordonnées du points dans le repère précédent
      alx_vecteur2D correction( mp->correction );
      //repere->Changer_coordonnees_prim( &correction );
      vect.Soustraire( correction );

      vect.Normer( vect.Norme()-eloignement_bordure_pt );

      // Changement du repère pour placer l'origine au coin fixe de redimensionnement
       double lg = Longueur_corp()/2
            , ht = Hauteur_corp() /2;
       switch(num_zone_redim)
        {
         case 0: vect.Ajouter( lg, 0 ); break;
         case 1: vect.Ajouter( lg, ht); break;
         case 2: vect.Ajouter( 0 , ht); break;
         case 3: vect.Ajouter(-lg, ht); break;
         case 4: vect.Ajouter(-lg, 0 ); break;
         case 5: vect.Ajouter(-lg,-ht); break;
         case 6: vect.Ajouter( 0 ,-ht); break;
         case 7: vect.Ajouter( lg,-ht); break;
        }

     // Calcul des nouvelles longueurs
      double l, h, dec_x=0, dec_y=0;
      if(pt_contact_depart.x != 0) l = Amax((l_au_contact*vect.dx)/(pt_contact_depart.x), 6*Rayon_zone_rotation()); //Amax((l_au_contact*vect.dx)/pt_contact_depart.x, 6*Rayon_zone_rotation());
       else l = l_au_contact;
      if(pt_contact_depart.y != 0) h = Amax((h_au_contact*vect.dy)/(pt_contact_depart.y), 6*Rayon_zone_rotation());
       else h = h_au_contact;

     // Mise à jour des nouvelles longueurs en fonction de la zone de départ
     // Calcul des décalages
      switch(num_zone_redim)
       {
        case 0:
          dec_x = (l-Longueur_corp());
          Longueur_corp( Longueur_corp()+dec_x );
          dec_x /= 2;
        break;
        case 1:
          dec_x = (l-Longueur_corp());
          dec_y = (h-Hauteur_corp()) ;
          Longueur_corp( Longueur_corp() + dec_x );
          Hauteur_corp(  Hauteur_corp()  + dec_y );
          dec_x /= 2;
          dec_y /= 2;
        break;
        case 2:
          dec_y = (h-Hauteur_corp()) ;
          Hauteur_corp(  Hauteur_corp()  + dec_y );
          dec_y /= 2;
        break;
        case 3:
          dec_x = (l-Longueur_corp());
          dec_y = (h-Hauteur_corp()) ;
          Longueur_corp( Longueur_corp() + dec_x );
          Hauteur_corp(  Hauteur_corp()  + dec_y );
          dec_x /= -2;
          dec_y /= 2;
        break;
        case 4:
          dec_x = (l-Longueur_corp());
          Longueur_corp( Longueur_corp() + dec_x );
          dec_x /= -2;
        break;
        case 5:
          dec_x = (l-Longueur_corp());
          dec_y = (h-Hauteur_corp()) ;
          Longueur_corp( Longueur_corp() + dec_x );
          Hauteur_corp(  Hauteur_corp()  + dec_y );
          dec_x /= -2;
          dec_y /= -2;
        break;
        case 6:
          dec_y = (h-Hauteur_corp()) ;
          Hauteur_corp(  Hauteur_corp()  + dec_y );
          dec_y /= -2;
        break;
        case 7:
          dec_x = (l-Longueur_corp());
          dec_y = (h-Hauteur_corp()) ;
          Longueur_corp( Longueur_corp() + dec_x );
          Hauteur_corp(  Hauteur_corp()  + dec_y );
          dec_x /= 2;
          dec_y /= -2;
        break;
       }

     // Calcul des décalages effectifs et application de ceux ci aux modèle physiques
      vect_dec.maj(dec_x, dec_y);
      vect_dec.Soustraire( *deplacement );
      vect_dec.Ajouter( correction );//vect_dec.Ajouter( mp->correction );
        alx_point2D info_point(0,0);
        {
         alx_vecteur2D v_tmp(vect_dec);
         repere->Changer_coordonnees_prim_inverse( vect_dec );
         repere->Translation( vect_dec );
         vect_dec.maj( v_tmp );
        }

     maj_mp_zrec ();

        Bloquer_rationnalisation(true);
            vect_dep.maj(dec_x, dec_y); // *deplacement );
            //vect_dep.Ajouter( vect_dec );
            bool b_tmp = mp_zone_redim.ne_pas_modifier_la_correction;
              mp_zone_redim.ne_pas_modifier_la_correction = false;
              mp_zone_redim.Informer_Reajustement_translation(&vect_dec, &info_point);
            mp_zone_redim.ne_pas_modifier_la_correction = b_tmp;
            b_tmp = mp_entete.ne_pas_modifier_la_correction;
              mp_entete.ne_pas_modifier_la_correction = true;
              mp_entete.Informer_Reajustement_translation(&vect_dep, &info_point);
            mp_entete.ne_pas_modifier_la_correction = b_tmp;
        Bloquer_rationnalisation(false);
     }
   break;
  }
}

//______________________________________________________________________________
void alx_fiche_ovale::Hauteur_corp(const double v)
{
 Longueur_entete(1.05 * PI
                *sqrt( ( Asqr(v)
                       + Asqr(Longueur_corp())
                       )/8
                     )
                );
 alx_fenetre::Hauteur_corp(v);
}
//______________________________________________________________________________
void alx_fiche_ovale::Longueur_corp(const double v)
{
 double l_e = 1.05 * PI
                *sqrt( ( Asqr(Hauteur_corp() )
                       + Asqr(v)
                       )/8
                     );
 Longueur_entete(l_e);
 alx_fenetre::Longueur_corp(v);
 Longueur_entete(l_e);
}

//______________________________________________________________________________
unsigned int alx_fiche_ovale::Calculer_num_zone_redim(const alx_vecteur2D &pt)
{
 double a = Longueur_corp()/2
      , b = Hauteur_corp()/2;
 double as2 = a/2
      , bs2 = b/2;

 unsigned int rep;
 double rapport_y_x;
 if(absd(pt.dx)<=EPSILON) rapport_y_x = 2*b/as2; // Pour avoir automatiquement rep = 2
  else rapport_y_x = absd(pt.dy/pt.dx);
 if(rapport_y_x < bs2/a) // On est pret de la bande verticale
   rep = 0;
  else
   {
    if(rapport_y_x < b/as2)
      rep = 1;
     else rep = 2;
   }

 // selon le coin on modifie rep...enfin on ne regarde pas le coin haut gauche
 // car la réponse est déja faite pour lui.
  if(pt.dx < 0)
   {
    if(pt.dy>0) rep = 4-rep;
     else rep += 4;
   }
   else if(pt.dy<0) rep = (8-rep)%8;

 return rep;
}




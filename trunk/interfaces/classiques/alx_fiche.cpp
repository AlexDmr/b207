#include "alx_fiche.h"
#include <stdio.h>
#include <stdlib.h>
#include "..\..\physique\math_alex.cpp"
/*******************************************************************************/
/******************************* Les constructeurs******************************/
/*******************************************************************************/
void alx_fiche::init()
{int i, pos;
 h_base = 0.0;

 mp_entete.abonner_a(this, 1);
 mp_corp.abonner_a(this, 2);
 mp_zone_rot.abonner_a(this, 3);
 for(i=0;i<8;i++) mp_zone_redim[i].abonner_a(this, 4+i);

 alx_liste<alx_point2D> L_pt;
 for(i=0;i<4;i++)
   L_pt.Ajouter_a_la_fin( Pt_nul() );

 mp_corp.Ajouter_polygone2D(L_pt, 1.0);
 mp_entete.Ajouter_polygone2D(L_pt, 1.0);
 mp_zone_rot.Ajouter_cercle2D_phys( alx_cercle2D_physique(0.0, 0.0, 1.0, 1.0, 0.0)
                                  , 1.0);

// Mettre à jour les zones de redimensions:
 alx_liste<alx_point2D> L_pt_coin;
 for(i=0;i<6;i++)
   L_pt_coin.Ajouter_a_la_fin( Pt_nul() );

 for(i=0;i<4;i++)
  {pos = i;
   pos = pos<<1;
   mp_zone_redim[pos].Ajouter_polygone2D(L_pt_coin, 1.0);
   mp_zone_redim[pos+1].Ajouter_polygone2D(L_pt, 1.0);}

 maj_mp_zrec();
}

/*******************************************************************************/
void alx_fiche::maj_mp_redim() // zone redimensionnement
{// On chope les coordonnées des sommet du rectangle définissant la zone utile de la fenêtre:
 alx_element_liste<alx_point2D> *it_pt = mp_corp.L_polygone2D.Premier_E().L_points.Premier();
 alx_point2D coin_bg( (it_pt->E()) );
 it_pt = it_pt->svt; alx_point2D coin_bd( (it_pt->E()) );
 it_pt = it_pt->svt; alx_point2D coin_hd( (it_pt->E()) );
 it_pt = it_pt->svt; alx_point2D coin_hg( (it_pt->E()) );
 alx_point2D centre(coin_bg); centre.Translation( Longueur_corp()/2
                                                ,(Hauteur_corp()+Hauteur_entete())/2);

// Mise à jour, à l'aide des données précédentes sur la fenêtre, des bordures de redimmension:
 alx_point2D tmp;
 alx_element_liste<alx_point2D> *it_pt_prcdt;
 alx_polygone2D *poly_coin_bg, *poly_cote_b, *poly_cote_d, *poly_coin_bd;

// coin en bas à gauche
 poly_coin_bg = &(mp_zone_redim[0].L_polygone2D.Premier_E());
 alx_liste<alx_point2D> *L_pt = &(poly_coin_bg->L_points);
 it_pt     = L_pt->Premier();
 ((it_pt->E())).maj(coin_bg); ((it_pt->E())).Translation(-Epaisseur_bordure(), -Epaisseur_bordure()); tmp.maj( (it_pt->E()) ); it_pt = it_pt->svt;
 ((it_pt->E())).maj(tmp); ((it_pt->E())).Translation(Epaisseur_bordure() + Longueur_corp()*0.1, 0); it_pt_prcdt = it_pt; it_pt = it_pt->svt;
 ((it_pt->E())).maj( (it_pt_prcdt->E()) ); ((it_pt->E())).Translation(0, Epaisseur_bordure()); it_pt = it_pt->svt;
 ((it_pt->E())).maj( tmp ); ((it_pt->E())).Translation(Epaisseur_bordure(), Epaisseur_bordure()); it_pt_prcdt = it_pt; it_pt = it_pt->svt;
 ((it_pt->E())).maj( (it_pt_prcdt->E()) ); ((it_pt->E())).Translation(0, Hauteur_corp()*0.1); it_pt_prcdt = it_pt; it_pt = it_pt->svt;
 ((it_pt->E())).maj( (it_pt_prcdt->E()) ); ((it_pt->E())).Translation(-Epaisseur_bordure(), 0);

// Coté du bas
 poly_cote_b = &(mp_zone_redim[1].L_polygone2D.Premier_E());
 L_pt  = &(poly_cote_b->L_points);
 it_pt = L_pt->Premier();
 ((it_pt->E())).maj(coin_bg); ((it_pt->E())).Translation(Longueur_corp()*0.1, -Epaisseur_bordure()); tmp.maj((it_pt->E())); it_pt = it_pt->svt;
 ((it_pt->E())).maj(tmp); ((it_pt->E())).Translation(Longueur_corp()*0.8, 0); it_pt_prcdt = it_pt; it_pt = it_pt->svt;
 ((it_pt->E())).maj( (it_pt_prcdt->E()) ); ((it_pt->E())).Translation(0, Epaisseur_bordure()); it_pt_prcdt = it_pt; it_pt = it_pt->svt;
 ((it_pt->E())).maj( (it_pt_prcdt->E()) ); ((it_pt->E())).Translation(-Longueur_corp()*0.8, 0);

// coin en bas à droite
 alx_droite2D axe(centre, alx_vecteur2D(0, 1));
 poly_coin_bd = &(mp_zone_redim[2].L_polygone2D.Premier_E());
 poly_coin_bd->maj(*poly_coin_bg);
 poly_coin_bd->Symetrique(axe);

// Coté de droite
 poly_cote_d = &(mp_zone_redim[3].L_polygone2D.Premier_E());
 L_pt  = &(poly_cote_d->L_points);
 it_pt = L_pt->Premier();
 ((it_pt->E())).maj(coin_bd); ((it_pt->E())).Translation(Epaisseur_bordure(), Hauteur_corp()*0.1); tmp.maj((it_pt->E())); it_pt = it_pt->svt;
 ((it_pt->E())).maj(tmp); ((it_pt->E())).Translation(0, 0.8*Hauteur_corp() + Hauteur_entete()); it_pt_prcdt = it_pt; it_pt = it_pt->svt;
 ((it_pt->E())).maj((it_pt_prcdt->E())); ((it_pt->E())).Translation(-Epaisseur_bordure(), 0); it_pt_prcdt = it_pt; it_pt = it_pt->svt;
 ((it_pt->E())).maj((it_pt_prcdt->E())); ((it_pt->E())).Translation(0, -0.8*Hauteur_corp() - Hauteur_entete());

// coin en haut à droite
 mp_zone_redim[4].L_polygone2D.Premier_E().maj(*poly_coin_bg);
 mp_zone_redim[4].L_polygone2D.Premier_E().Symetrique(centre);

// Coté du haut
 mp_zone_redim[5].L_polygone2D.Premier_E().maj(*poly_cote_b);
 mp_zone_redim[5].L_polygone2D.Premier_E().Symetrique(centre);

// coin en haut à gauche
 mp_zone_redim[6].L_polygone2D.Premier_E().maj(*poly_coin_bd);
 mp_zone_redim[6].L_polygone2D.Premier_E().Symetrique(centre);

// Coté de gauche
 mp_zone_redim[7].L_polygone2D.Premier_E().maj(*poly_cote_d);
 mp_zone_redim[7].L_polygone2D.Premier_E().Symetrique(centre);

//------------------------------------------------
// Mise à jour des informations sur la transformation de scene liée au corp
/*
 alx_vecteur2D *pos = &(mp_corp.position);
 double         rot = mp_corp.rotation;

 for(int i=0;i<8;i++)
  {
   mp_zone_redim[i].position.maj(*pos);
   mp_zone_redim[i].rotation = rot;
  }
*/
}

void alx_fiche::maj_mp_zrec() // zone rot, entête, corp
{double lc = Longueur_corp()
      , hc = Hauteur_corp()
      , he = Hauteur_entete();

 alx_element_liste<alx_point2D> *it_pt  = mp_corp.L_polygone2D.Premier_E().L_points.Premier()
                              , *it_fin = mp_corp.L_polygone2D.Premier_E().L_points.Fin();
 if (it_pt == it_fin) {printf("OVERFLOW DE LA LISTE DE POINT de mp_corp!!!");}
 ((it_pt->E())).maj(0, h_base); it_pt = it_pt->svt;
 if (it_pt == it_fin) {printf("OVERFLOW DE LA LISTE DE POINT de mp_corp!!!");}
 ((it_pt->E())).maj(lc, 0);     it_pt = it_pt->svt;
 if (it_pt == it_fin) {printf("OVERFLOW DE LA LISTE DE POINT de mp_corp!!!");}
 ((it_pt->E())).maj(lc, hc);    it_pt = it_pt->svt;
 if (it_pt == it_fin) {printf("OVERFLOW DE LA LISTE DE POINT de mp_corp!!!");}
 ((it_pt->E())).maj(0, hc);

 it_pt = mp_entete.L_polygone2D.Premier_E().L_points.Premier();
 it_fin = mp_corp.L_polygone2D.Premier_E().L_points.Fin();
 if (it_pt == it_fin) {printf("OVERFLOW DE LA LISTE DE POINT de mp_entete!!!");}
 ((it_pt->E())).maj(0, hc);     it_pt = it_pt->svt;
 if (it_pt == it_fin) {printf("OVERFLOW DE LA LISTE DE POINT de mp_entete!!!");}
 ((it_pt->E())).maj(lc, hc);    it_pt = it_pt->svt;
 if (it_pt == it_fin) {printf("OVERFLOW DE LA LISTE DE POINT de mp_entete!!!");}
 ((it_pt->E())).maj(lc, hc+he); it_pt = it_pt->svt;
 if (it_pt == it_fin) {printf("OVERFLOW DE LA LISTE DE POINT de mp_entete!!!");}
 ((it_pt->E())).maj(0, hc+he);

 alx_cercle2D_physique *cercle = &( mp_zone_rot.L_cercle2D_physique.Premier_E() );
 cercle->maj( alx_cercle2D_physique(lc/2, h_base, 1.0, 1.0, Rayon_zone_rotation()) );

 double lc_div_2 = lc
      , hcc = hc/(double)2.0
      , hce = hc + he/2.0;
 lc_div_2 /= 2;
 centre_entete.maj(lc_div_2, hce);
 centre_corp.maj(lc_div_2, hcc);
 centre_zone_rot.maj(lc_div_2, 0);
}

/*******************************************************************************/
alx_fiche::alx_fiche() : alx_fenetre()
{init();}

/*******************************************************************************/
alx_fiche::alx_fiche(const char *n, const double largeur, const double hauteur) : alx_fenetre(n, largeur, hauteur)
{init();
 maj_mp_zrec();
 maj_mp_redim();}

alx_fiche::alx_fiche( const char *n, const double largeur, const double hauteur
                    , const double hauteur_entete, const double r_zone_rot) : alx_fenetre(n, largeur, hauteur, largeur, hauteur_entete, r_zone_rot)
{init();
 Appliquer_maj();}

/*******************************************************************************/
/**********************************Les méthodes*********************************/
/*******************************************************************************/
void alx_fiche::rationnaliser(int num)
{alx_model_simulation_physique1 *mp;

 switch(num)
  {case 1: mp = &mp_entete; break;
   case 2: mp = &mp_corp; break;
   case 3: mp = &mp_zone_rot; break;
   default:
    if(num<1 || num>11) return;
    mp = &(mp_zone_redim[num-4]);
   // Gestion des contacts
    if( mp->infos_comp[0]==6)
     {zone_redim_active[num-4] = true;
      depassement[num-4].maj(0,0);}
    if( mp->infos_comp[0] == 3)
      zone_redim_active[num-4] = false;
   break;
  }

 if( (mp->nb_infos_comp == 4)
   &&(mp->infos_comp[0] == 3)                              //Si c'est une rupture du point de contact
   &&(mp->infos_comp[1] == 0)                              //sur le mp de redimensionnement
   &&(mp->infos_comp[3] == (int)(void*)&mp_zone_redim) )   //alors remettre celui-ci à sa place.
  {maj_mp_redim();
   return;}

 //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
 // Contact_avec_point(alx_point2D *pt)
 //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
 if( (mp==&mp_corp)
   &&(mp_corp.infos_comp[0] == 2 || mp_corp.infos_comp[0]==6)
   &&(mp_corp.infos_comp[1] == 0) )
  {Contact_avec_point( (alx_point2D*)(void*)(mp_corp.infos_comp[2]) );}

 if( (mp->nb_infos_comp != 4)
   ||(mp->infos_comp[0] != 5) ) return; // on ne traite que les changements de position

 alx_element_liste<alx_point2D> *it_pt1, *it_pt3;
 alx_point2D centre_redim;//, *coin_bg, *coin_bd, *coin_hd, *coin_hg;
 alx_vecteur2D vect, trans_point;
 double h_base_tmp, h_corp_prcdt, lg_corp_prcdt;
 alx_element_liste<alx_point2D> *it_pt;
 double info_angle, tmp1, tmp2;
 alx_point2D   info_point;
 alx_vecteur2D info_vecteur, correction;
 alx_model_simulation_physique1 *zr;

 switch(num)
  {case 1: // De l'entête
   case 2: // Du corp
   case 3: // De la zone de rotation
     // Mise à jour de la zone de redimensionnement
   // Gestion des déplacements

     if(mp->infos_comp[1] == 2)
      {
       double *r      = (double*)(void*)( mp->infos_comp[2] );
       alx_point2D *p = (alx_point2D*)(void*)( mp->infos_comp[3] );
       Bloquer_rationnalisation(true);
        for(int i=0;i<8;i++)
         mp_zone_redim[i].Informer_Reajustement_rotation(r, p);
       Bloquer_rationnalisation(false);
      }

     if(mp->infos_comp[1] == 1)
      {
       vect_dec.maj( *((alx_vecteur2D*)(void*)(mp->infos_comp[2])) );
       Bloquer_rationnalisation(true);
        for(int i=0;i<8;i++)
         mp_zone_redim[i].Informer_Reajustement_translation(&vect_dec, &info_point);
       Bloquer_rationnalisation(false);
      }
   break;
   default: // De la zone de redimensionnement
    if( num<4 || num>11 || (mp->infos_comp[1] != 1) ) return;

    it_pt = mp->L_polygone2D.Premier_E().L_points.Premier();

    if(num%2 == 0) // On a à faire avec un coin
     {it_pt = it_pt->svt;it_pt = it_pt->svt;it_pt = it_pt->svt; // On sélectionne le quatrième point
     }
     else // On a à faire avec un coté
      {it_pt = it_pt->svt;it_pt = it_pt->svt;it_pt = it_pt->svt; // euh...c'est aussi le quatrième point dans ce cas là....
      }
    centre_redim.maj((it_pt->E()));
    alx_vecteur2D translation_subie( *((alx_vecteur2D*)(void*)(mp->infos_comp[2])) );
    centre_redim.Translation( translation_subie );
    alx_vecteur2D translation_subie_opposee( translation_subie );
    translation_subie_opposee.Oppose();
    //repere->Translation( translation_subie_opposee );

    correction.maj(mp->correction);

    Bloquer_emission_rationnalisation(true);
    h_corp_prcdt  = Hauteur_corp();
    lg_corp_prcdt = Longueur_corp();
    info_vecteur.maj(0,0);
    switch(num)
     {
      case 4: //coin en bas à gauche dans la majorité des cas
        Hauteur_corp( Amax(h_corp_prcdt-centre_redim.y  , 6*Rayon_zone_rotation()) );
        Longueur_corp( Amax(lg_corp_prcdt-centre_redim.x, 6*Rayon_zone_rotation()) );
        tmp1 = Longueur_corp();
        tmp2 = Hauteur_corp();
        info_vecteur.maj(Longueur_corp()-lg_corp_prcdt, Hauteur_corp()-h_corp_prcdt);

       // Mise à jour de la correction qui est pourtant fausse
        depassement[0].Ajouter(translation_subie); depassement[0].Ajouter(info_vecteur);
        if(depassement[0].dx < EPSILON)
          depassement[0].dx = 0;
        if(depassement[0].dy < EPSILON)
          depassement[0].dy = 0;

         trans_point.maj(translation_subie); trans_point.Ajouter(depassement[0]);
         if( (trans_point.dx * translation_subie.dx < 0)
           ||((trans_point.dx * depassement[0].dx > 0)&&(lg_corp_prcdt==tmp1)) )
           trans_point.dx = 0;
         if( (trans_point.dy * translation_subie.dy < 0)
           ||( (trans_point.dy * depassement[0].dy > 0))&&(h_corp_prcdt==tmp2) )
           trans_point.dy = 0;

         if( (depassement[0].dx > 0)
           &&(translation_subie.dx < 0) )
           depassement[0].dx = Amax((double)0, depassement[0].dx+translation_subie.dx);
         if( (depassement[0].dy > 0)
           &&(translation_subie.dy < 0) )
           depassement[0].dy = Amax((double)0, depassement[0].dy+translation_subie.dy);
         if(depassement[0].dx <= EPSILON) depassement[0].dx = 0;
         if(depassement[0].dy <= EPSILON) depassement[0].dy = 0;

        // Calcul des changement de dimensions de la fiche
         centre_redim.maj(0,0); // On replace le coin à sa place d'origine
         centre_redim.Translation( trans_point );
         Hauteur_corp( Amax(h_corp_prcdt-centre_redim.y  , 6*Rayon_zone_rotation()) );
         Longueur_corp( Amax(lg_corp_prcdt-centre_redim.x, 6*Rayon_zone_rotation()) );
         info_vecteur.maj(Longueur_corp()-lg_corp_prcdt, Hauteur_corp()-h_corp_prcdt);

       // Calcul de la translation du repère de la fiche, indépendant des changement de dimensions
        info_point.maj(0,0);
        vect_info[0].maj( translation_subie_opposee );
        vect_info[0].Ajouter( correction );
        vect_info[0].Soustraire( info_vecteur );
        {
         alx_vecteur2D v_tmp(vect_info[0]);
         repere->Changer_coordonnees_prim_inverse( vect_info[0] );
         repere->Translation( vect_info[0] );
         vect_info[0].maj( v_tmp );
        }

        Bloquer_rationnalisation(true);
        mp->Informer_Reajustement_translation(&(vect_info[0]), &info_point);
        Bloquer_rationnalisation(false);
      break;
      case 5: //coté du bas
        Hauteur_corp( Amax(h_corp_prcdt-centre_redim.y  , 6*Rayon_zone_rotation()) );
        //Longueur_corp( Amax(lg_corp_prcdt-centre_redim.x, 6*Rayon_zone_rotation()) );
        tmp1 = Longueur_corp();
        tmp2 = Hauteur_corp();
        info_vecteur.maj(0, Hauteur_corp()-h_corp_prcdt);

       // Mise à jour de la correction qui est pourtant fausse
        depassement[1].Ajouter(translation_subie); depassement[1].Ajouter(info_vecteur);
        if(depassement[1].dy < EPSILON)
          depassement[1].dy = 0;

         trans_point.maj(translation_subie); trans_point.Ajouter(depassement[1]);
         if( (trans_point.dy * translation_subie.dy < 0)
           ||( (trans_point.dy * depassement[1].dy > 0))&&(h_corp_prcdt==tmp2) )
           trans_point.dy = 0;

         if( (depassement[1].dy > 0)
           &&(translation_subie.dy < 0) )
           depassement[1].dy = Amax((double)0, depassement[1].dy+translation_subie.dy);
         if(depassement[1].dy <= EPSILON) depassement[1].dy = 0;

        // Calcul des changement de dimensions de la fiche
         centre_redim.maj(0,0); // On replace le coin à sa place d'origine
         centre_redim.Translation( trans_point );
         Hauteur_corp( Amax(h_corp_prcdt-centre_redim.y  , 6*Rayon_zone_rotation()) );
         info_vecteur.maj(0, Hauteur_corp()-h_corp_prcdt);

       // Calcul de la translation du repère de la fiche, indépendant des changement de dimensions
        info_point.maj(0,0);
        vect_info[1].maj( translation_subie_opposee );
        vect_info[1].Ajouter( correction );
        vect_info[1].Soustraire( info_vecteur );
        {
         alx_vecteur2D v_tmp(vect_info[1]);
         repere->Changer_coordonnees_prim_inverse( vect_info[1] );
         repere->Translation( vect_info[1] );
         vect_info[1].maj( v_tmp );
        }

        Bloquer_rationnalisation(true);
          mp->Informer_Reajustement_translation(&(vect_info[1]), &info_point);
        Bloquer_rationnalisation(false);
      break;
      case 6: //coin en bas à droite
        Hauteur_corp( Amax(h_corp_prcdt-centre_redim.y  , 6*Rayon_zone_rotation()) );
        Longueur_corp( Amax(centre_redim.x, 6*Rayon_zone_rotation()) );
        tmp1 = Longueur_corp();
        tmp2 = Hauteur_corp();
        info_vecteur.maj(Longueur_corp()-lg_corp_prcdt, Hauteur_corp()-h_corp_prcdt);

       // Mise à jour de la correction qui est pourtant fausse
        depassement[2].Ajouter(translation_subie);
        depassement[2].dx -= info_vecteur.dx;
        depassement[2].dy += info_vecteur.dy;
        if(depassement[2].dx > -EPSILON)
          depassement[2].dx = 0;
        if(depassement[2].dy < EPSILON)
          depassement[2].dy = 0;

         trans_point.maj(translation_subie); trans_point.Ajouter(depassement[2]);
         if( (trans_point.dx * translation_subie.dx < 0)
           ||((trans_point.dx * depassement[2].dx > 0)&&(lg_corp_prcdt==tmp1)) )
           trans_point.dx = 0;
         if( (trans_point.dy * translation_subie.dy < 0)
           ||( (trans_point.dy * depassement[2].dy > 0))&&(h_corp_prcdt==tmp2) )
           trans_point.dy = 0;

         if( (depassement[2].dx < 0)
           &&(translation_subie.dx > 0) )
           depassement[2].dx = Amin((double)0, depassement[2].dx+translation_subie.dx);
         if( (depassement[2].dy > 0)
           &&(translation_subie.dy < 0) )
           depassement[2].dy = Amax((double)0, depassement[2].dy+translation_subie.dy);
         if(depassement[2].dx >=-EPSILON) depassement[2].dx = 0;
         if(depassement[2].dy <= EPSILON) depassement[2].dy = 0;

        // Calcul des changement de dimensions de la fiche
         centre_redim.maj((it_pt->E())); // On replace le coin à sa place d'origine
         centre_redim.Translation( trans_point );
         Hauteur_corp( Amax(h_corp_prcdt-centre_redim.y  , 6*Rayon_zone_rotation()) );
         Longueur_corp( Amax(centre_redim.x, 6*Rayon_zone_rotation()) );
         info_vecteur.maj(Longueur_corp()-lg_corp_prcdt, Hauteur_corp()-h_corp_prcdt);

       // Calcul de la translation du repère de la fiche, indépendant des changement de dimensions
        info_point.maj((it_pt->E()));
        vect_info[2].maj( translation_subie_opposee );
        vect_info[2].Ajouter( correction );
        vect_info[2].dy -= info_vecteur.dy;
        {
         alx_vecteur2D v_tmp(vect_info[2]);
         repere->Changer_coordonnees_prim_inverse( vect_info[2] );
         repere->Translation( vect_info[2] );
         vect_info[2].maj( v_tmp );
        }

        Bloquer_rationnalisation(true);
          mp->Informer_Reajustement_translation(&(vect_info[2]), &info_point);
        Bloquer_rationnalisation(false);
      break;
      case 7: //coté de droite
        tmp1 = Longueur_corp();
        Longueur_corp( Amax(centre_redim.x, 6*Rayon_zone_rotation()) );
        info_vecteur.maj(Longueur_corp()-tmp1, 0);

       // Mise à jour de la correction
        depassement[3].Ajouter(translation_subie); depassement[3].Soustraire(info_vecteur);
         if(depassement[3].dx >= 0) depassement[3].dx = 0;
         if(depassement[3].dy >= 0) depassement[3].dy = 0;
         trans_point.maj(translation_subie); trans_point.Ajouter(depassement[3]);
         if(trans_point.dx * translation_subie.dx < 0)
           trans_point.dx = 0;
         if(trans_point.dy * translation_subie.dy < 0)
           trans_point.dy = 0;

         if( (depassement[3].dx < 0)
           &&(translation_subie.dx > 0) )
           depassement[3].dx = Amin((double)0, depassement[3].dx+translation_subie.dx);
         if( (depassement[3].dy < 0)
           &&(translation_subie.dy > 0) )
           depassement[3].dy = Amin((double)0, depassement[3].dy+translation_subie.dy);

         if(depassement[3].dx >= -EPSILON) depassement[3].dx = 0;
         if(depassement[3].dy >= -EPSILON) depassement[3].dy = 0;

        // Calcul des changement de dimensions de la fiche
         centre_redim.Translation_inverse( translation_subie ); // On replace le coin à sa place d'origine
         centre_redim.Translation( trans_point );

         Longueur_corp( Amax(centre_redim.x, 6*Rayon_zone_rotation()) );

       // Calcul de la translation du repère de la fiche, indépendant des changement de dimensions
        info_point.maj(0,0);
        vect_info[3].maj( translation_subie_opposee );
        vect_info[3].Ajouter( correction );
        {
         alx_vecteur2D v_tmp(vect_info[3]);
         repere->Changer_coordonnees_prim_inverse( vect_info[3] );
         repere->Translation( vect_info[3] );
         vect_info[3].maj( v_tmp );
        }

       //repere->Changer_coordonnees_prim_inverse( vect_info );
        Bloquer_rationnalisation(true);
        mp->Informer_Reajustement_translation(&(vect_info[3]), &info_point);
        Bloquer_rationnalisation(false);
      break;
      case 8: //coin en haut à droite
        tmp2 = Hauteur_corp();
        Hauteur_corp( Amax(centre_redim.y-Hauteur_entete(), 6*Rayon_zone_rotation()) );
        tmp1 = Longueur_corp();
        Longueur_corp( Amax(centre_redim.x, 6*Rayon_zone_rotation()) );
        info_vecteur.maj(Longueur_corp()-tmp1, Hauteur_corp()-tmp2);

       // Mise à jour de la correction
        depassement[4].Ajouter(translation_subie); depassement[4].Soustraire(info_vecteur);
         if(depassement[4].dx >= 0) depassement[4].dx = 0;
         if(depassement[4].dy >= 0) depassement[4].dy = 0;
         trans_point.maj(translation_subie); /*trans_point.Ajouter(translation_subie);*/ trans_point.Ajouter(depassement[4]);
         if(trans_point.dx * translation_subie.dx < 0)
           trans_point.dx = 0;
         if(trans_point.dy * translation_subie.dy < 0)
           trans_point.dy = 0;

         if( (depassement[4].dx < 0)
           &&(translation_subie.dx > 0) )
           depassement[4].dx = Amin((double)0, depassement[4].dx+translation_subie.dx);
         if( (depassement[4].dy < 0)
           &&(translation_subie.dy > 0) )
           depassement[4].dy = Amin((double)0, depassement[4].dy+translation_subie.dy);

         if(depassement[4].dx >= -EPSILON) depassement[4].dx = 0;
         if(depassement[4].dy >= -EPSILON) depassement[4].dy = 0;

        // Calcul des changement de dimensions de la fiche
         centre_redim.Translation_inverse( translation_subie ); // On replace le coin à sa place d'origine
         centre_redim.Translation( trans_point );
         Hauteur_corp( Amax(centre_redim.y-Hauteur_entete(), 6*Rayon_zone_rotation()) );
         Longueur_corp( Amax(centre_redim.x, 6*Rayon_zone_rotation()) );

       // Calcul de la translation du repère de la fiche, indépendant des changement de dimensions
        info_point.maj(0,0);
        vect_info[4].maj( translation_subie_opposee );
        vect_info[4].Ajouter( correction );
        {
         alx_vecteur2D v_tmp(vect_info[4]);
         repere->Changer_coordonnees_prim_inverse( vect_info[4] );
         repere->Translation( vect_info[4] );
         vect_info[4].maj( v_tmp );
        }

       //repere->Changer_coordonnees_prim_inverse( vect_info );
        Bloquer_rationnalisation(true);
        mp->Informer_Reajustement_translation(&(vect_info[4]), &info_point);
        Bloquer_rationnalisation(false);
      break;
      case 9: //coté du haut
        tmp2 = Hauteur_corp();
        Hauteur_corp( Amax(centre_redim.y-Hauteur_entete(), 6*Rayon_zone_rotation()) );
        //tmp1 = Longueur_corp();
        //Longueur_corp( Amax(centre_redim.x, 6*Rayon_zone_rotation()) );
        info_vecteur.maj(0/*Longueur_corp()-tmp1*/, Hauteur_corp()-tmp2);

       // Mise à jour de la correction
        depassement[5].Ajouter(translation_subie); depassement[5].Soustraire(info_vecteur);
         if(depassement[5].dx >= 0) depassement[5].dx = 0;
         if(depassement[5].dy >= 0) depassement[5].dy = 0;
         trans_point.maj(translation_subie); /*trans_point.Ajouter(translation_subie);*/ trans_point.Ajouter(depassement[5]);
         if(trans_point.dx * translation_subie.dx < 0)
           trans_point.dx = 0;
         if(trans_point.dy * translation_subie.dy < 0)
           trans_point.dy = 0;

         if( (depassement[5].dx < 0)
           &&(translation_subie.dx > 0) )
           depassement[5].dx = Amin((double)0, depassement[5].dx+translation_subie.dx);
         if( (depassement[5].dy < 0)
           &&(translation_subie.dy > 0) )
           depassement[5].dy = Amin((double)0, depassement[5].dy+translation_subie.dy);

         if(depassement[5].dx >= -EPSILON) depassement[5].dx = 0;
         if(depassement[5].dy >= -EPSILON) depassement[5].dy = 0;

        // Calcul des changement de dimensions de la fiche
         centre_redim.Translation_inverse( translation_subie ); // On replace le coin à sa place d'origine
         centre_redim.Translation( trans_point );
         Hauteur_corp( Amax(centre_redim.y-Hauteur_entete(), 6*Rayon_zone_rotation()) );
         //Longueur_corp( Amax(centre_redim.x, 6*Rayon_zone_rotation()) );

       // Calcul de la translation du repère de la fiche, indépendant des changement de dimensions
        info_point.maj(0,0);
        vect_info[5].maj( translation_subie_opposee );
        vect_info[5].Ajouter( correction );
        {
         alx_vecteur2D v_tmp(vect_info[5]);
         repere->Changer_coordonnees_prim_inverse( vect_info[5] );
         repere->Translation( vect_info[5] );
         vect_info[5].maj( v_tmp );
        }

       //repere->Changer_coordonnees_prim_inverse( vect_info );
        Bloquer_rationnalisation(true);
        mp->Informer_Reajustement_translation(&(vect_info[5]), &info_point);
        Bloquer_rationnalisation(false);
      break;
      case 10: //coin en haut à gauche
        Hauteur_corp( Amax(centre_redim.y-Hauteur_entete()  , 6*Rayon_zone_rotation()) );
        Longueur_corp( Amax(lg_corp_prcdt-centre_redim.x, 6*Rayon_zone_rotation()) );
        tmp1 = Longueur_corp();
        tmp2 = Hauteur_corp();
        info_vecteur.maj(Longueur_corp()-lg_corp_prcdt, Hauteur_corp()-h_corp_prcdt);

       // Mise à jour de la correction qui est pourtant fausse
        depassement[6].Ajouter(translation_subie);
        depassement[6].dx += info_vecteur.dx;
        depassement[6].dy -= info_vecteur.dy;
        if(depassement[6].dx < EPSILON)
          depassement[6].dx = 0;
        if(depassement[6].dy >-EPSILON)
          depassement[6].dy = 0;

         trans_point.maj(translation_subie); trans_point.Ajouter(depassement[6]);
         if( (trans_point.dx * translation_subie.dx < 0)
           ||((trans_point.dx * depassement[6].dx > 0)&&(lg_corp_prcdt==tmp1)) )
           trans_point.dx = 0;
         if( (trans_point.dy * translation_subie.dy < 0)
           ||( (trans_point.dy * depassement[6].dy > 0))&&(h_corp_prcdt==tmp2) )
           trans_point.dy = 0;

         if( (depassement[6].dx > 0)
           &&(translation_subie.dx < 0) )
           depassement[6].dx = Amax((double)0, depassement[6].dx+translation_subie.dx);
         if( (depassement[6].dy < 0)
           &&(translation_subie.dy > 0) )
           depassement[6].dy = Amin((double)0, depassement[6].dy+translation_subie.dy);
         if(depassement[6].dx <= EPSILON) depassement[6].dx = 0;
         if(depassement[6].dy >=-EPSILON) depassement[6].dy = 0;

        // Calcul des changement de dimensions de la fiche
         centre_redim.maj(0, h_corp_prcdt); // On replace le coin à sa place d'origine
         centre_redim.Translation( trans_point );
         Hauteur_corp( Amax(centre_redim.y, 6*Rayon_zone_rotation()) );
         Longueur_corp( Amax(lg_corp_prcdt-centre_redim.x, 6*Rayon_zone_rotation()) );
         info_vecteur.maj(Longueur_corp()-lg_corp_prcdt, Hauteur_corp()-h_corp_prcdt);

       // Calcul de la translation du repère de la fiche, indépendant des changement de dimensions
        info_point.maj(0, 0);
        vect_info[6].maj( translation_subie_opposee );
        vect_info[6].Ajouter( correction );
        vect_info[6].dx -= info_vecteur.dx;
        {
         alx_vecteur2D v_tmp(vect_info[6]);
         repere->Changer_coordonnees_prim_inverse( vect_info[6] );
         repere->Translation( vect_info[6] );
         vect_info[6].maj( v_tmp );
        }

        Bloquer_rationnalisation(true);
          mp->Informer_Reajustement_translation(&(vect_info[6]), &info_point);
        Bloquer_rationnalisation(false);
      break;
      case 11: //coté de gauche
        Longueur_corp( Amax(lg_corp_prcdt-centre_redim.x, 6*Rayon_zone_rotation()) );
        tmp1 = Longueur_corp();
        tmp2 = Hauteur_corp();
        info_vecteur.maj(Longueur_corp()-lg_corp_prcdt, 0);

       // Mise à jour de la correction qui est pourtant fausse
        depassement[7].Ajouter(translation_subie); depassement[7].Ajouter(info_vecteur);
        if(depassement[7].dx < EPSILON)
          depassement[7].dx = 0;

         trans_point.maj(translation_subie); trans_point.Ajouter(depassement[7]);
         if( (trans_point.dx * translation_subie.dx < 0)
           ||((trans_point.dx * depassement[7].dx > 0)&&(lg_corp_prcdt==tmp1)) )
           trans_point.dx = 0;

         if( (depassement[7].dx > 0)
           &&(translation_subie.dx < 0) )
           depassement[7].dx = Amax((double)0, depassement[7].dx+translation_subie.dx);
         if(depassement[7].dx <= EPSILON) depassement[7].dx = 0;

        // Calcul des changement de dimensions de la fiche
         centre_redim.maj(0,0); // On replace le coin à sa place d'origine
         centre_redim.Translation( trans_point );
         Longueur_corp( Amax(lg_corp_prcdt-centre_redim.x, 6*Rayon_zone_rotation()) );
         info_vecteur.maj(Longueur_corp()-lg_corp_prcdt, 0);

       // Calcul de la translation du repère de la fiche, indépendant des changement de dimensions
        info_point.maj(0,0);
        vect_info[7].maj( translation_subie_opposee );
        vect_info[7].Ajouter( correction );
        vect_info[7].Soustraire( info_vecteur );
        {
         alx_vecteur2D v_tmp(vect_info[7]);
         repere->Changer_coordonnees_prim_inverse( vect_info[7] );
         repere->Translation( vect_info[7] );
         vect_info[7].maj( v_tmp );
        }

        Bloquer_rationnalisation(true);
        mp->Informer_Reajustement_translation(&(vect_info[7]), &info_point);
        Bloquer_rationnalisation(false);
      break;
     }
    //alx_vecteur2D v_temp(vect);

    maj_mp_zrec ();
    maj_mp_redim();
    Bloquer_emission_rationnalisation(false); // On débloque l'émission des ordres de rationnalisations (qui portaient sur les grandeurs)
    Hauteur_corp ( Hauteur_corp()  ); // on émet les
    Longueur_corp( Longueur_corp() ); // nouvelles grandeurs



    Bloquer_rationnalisation(true);
      alx_vecteur2D trans_reste( translation_subie );
      trans_reste.Ajouter( vect_info[num-4] );
      trans_reste.Soustraire( correction ); //Soustraire( mp->correction );
      bool b_tmp = mp_entete.ne_pas_modifier_la_correction;
      mp_entete.ne_pas_modifier_la_correction = true;
        mp_entete.Informer_Reajustement_translation(&trans_reste, &info_point);
      mp_entete.ne_pas_modifier_la_correction = b_tmp;
    Bloquer_rationnalisation(false);
  }
}



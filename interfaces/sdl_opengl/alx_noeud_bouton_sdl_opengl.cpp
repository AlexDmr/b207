#include "alx_noeud_bouton_sdl_opengl.h"
#include "..\choses_communes.h"
#include "alx_interface_sdl_opengl_1.h"
#include "alx_noeud_fiche_sdl_opengl.h"
#include "alx_noeud_fiche_ovale_sdl_opengl.h"
#include "../classiques/alx_fiche_ovale_opengl.h"
#include "../classiques/alx_fiche_opengl.h"


void alx_noeud_bouton_sdl_opengl::Creer_fenetre( const double tx, const double ty
                                          , alx_interface_sdl_opengl_1 *interface_m
                                          , int type_de_fenetre )
{
 switch(type_de_fenetre)
  {
   case 1:
     fenetre_bouton = new alx_noeud_fiche_sdl_opengl( new alx_fiche_opengl( new alx_fiche( ""
                                                                                         , tx
                                                                                         , ty, 0, 0)
                                                                          , fonte_Arial()
                                                                          , Matos() )
                                                    , NULL
                                                    , interface_m->Prerendeur()
                                                    , 1
                                                    , NULL //hdc
                                                    , NULL //hglrc
                                                    , ALXGL_RENDU_ECRAN_PUIS_COPIE_DANS_TEXTURE);
   break;
   case 2:
     fenetre_bouton = new alx_noeud_fiche_ovale_sdl_opengl( new alx_fiche_ovale_opengl( new alx_fiche_ovale( ""
                                                                                                           , tx
                                                                                                           , ty)
                                                          , fonte_Arial()
                                                          , Matos() )
                                                    , NULL
                                                    , interface_m->Prerendeur()
                                                    , 1
                                                    , NULL //hdc
                                                    , NULL //hglrc
                                                    , ALXGL_RENDU_ECRAN_PUIS_COPIE_DANS_TEXTURE);
   break;
  }

 fenetre_bouton->Gerer_zone_rot(false);
 fenetre_bouton->Gerer_entete(false);
// fenetre_bouton->Gerer_bordure(false);
 fenetre_bouton->Afficher_zone_rot(false);
 fenetre_bouton->Afficher_entete(false);
 fenetre_bouton->Fenetre()->Hauteur_entete(0);
 fenetre_bouton->Fenetre()->Epaisseur_bordure(5);
 fenetre_bouton->Fenetre()->Appliquer_maj();
 fenetre_bouton->Voir_si_pt_dans_fils(false);
 fenetre_bouton->Noeud_repere_fils()->Noeud_touchable(false);
 alx_model_simulation_physique1 *mp = fenetre_bouton->Fenetre_opengl()->Fenetre()->MP_corps();
  mp->abonner_a(this, (int)(void*)mp );

 etat_bouton       = Bouton_relache;
 //pression_initiale = false;
 fenetre_bouton->Fenetre_opengl()->Couleur_bordure_ext_angle(0,1,0,1);
 fenetre_bouton->Fenetre_opengl()->Couleur_bordure_ext(0,1,0,1);
 fenetre_bouton->Position_dans_liste_des_fils_changeable(false);

 Ajouter_fils( (alx_noeud_scene*)fenetre_bouton );

 Abonner_a(interface_m->lieur_ptr_liant);
}

//______________________________________________________________________________
alx_noeud_bouton_sdl_opengl::alx_noeud_bouton_sdl_opengl( const double tx, const double ty
                                                        , alx_interface_sdl_opengl_1 *interface_m
                                                        , alx_noeud_scene *racine
                                                        , int type_de_fenetre ) : alx_noeud_scene()
{interface_mere = interface_m;
 noeud_racine = racine;
 Creer_fenetre(tx, ty, interface_m, type_de_fenetre);
 Mettre_a_jour();
}

//______________________________________________________________________________
alx_noeud_bouton_sdl_opengl::alx_noeud_bouton_sdl_opengl( alx_image_opengl &imgbt
                                                        , alx_interface_sdl_opengl_1 *interface_m
                                                        , alx_noeud_scene *racine
                                                        , int type_de_fenetre ) : alx_noeud_scene()
{interface_mere = interface_m;
 noeud_racine = racine;
 Creer_fenetre(imgbt.Lg(), imgbt.Ht(), interface_m, type_de_fenetre);
//___________________________________________________________
   img_noeud_tmp = new alx_noeud_image_sdl_opengl( imgbt );
   fenetre_bouton->Ajouter_fils( (alx_noeud_scene*)(img_noeud_tmp) );
   img_noeud_tmp->Noeud_touchable( false );
   //fenetre_bouton->PreRendre();
   //fenetre_bouton->Ne_pas_pre_rendre(true);
//___________________________________________________________
 Mettre_a_jour();
}

//______________________________________________________________________________
/*
alx_noeud_bouton_sdl_opengl::alx_noeud_bouton_sdl_opengl(  alx_noeud_fenetre_sdl_opengl *f ) : alx_noeud_scene()
{
 fenetre_bouton = f;
 fenetre_bouton->Gerer_zone_rot(false);
 fenetre_bouton->Gerer_entete(false);
// fenetre_bouton->Gerer_bordure(false);
 fenetre_bouton->Afficher_zone_rot(false);
 fenetre_bouton->Afficher_entete(false);
 fenetre_bouton->Fenetre()->Hauteur_entete(0);
 fenetre_bouton->Fenetre()->Epaisseur_bordure(5);
 fenetre_bouton->Fenetre()->Appliquer_maj();
 alx_model_simulation_physique1 *mp = fenetre_bouton->Fenetre_opengl()->Fenetre()->MP_corps();
  mp->abonner_a(this, (int)(void*)mp );

 etat_bouton       = Bouton_relache;
 //pression_initiale = false;

 fenetre_bouton->PreRendre();
 fenetre_bouton->Ne_pas_pre_rendre(true);

 fenetre_bouton->Fenetre_opengl()->Couleur_bordure_ext_angle(0,1,0,1);
 fenetre_bouton->Fenetre_opengl()->Couleur_bordure_ext(0,1,0,1);
 fenetre_bouton->Position_dans_liste_des_fils_changeable(false);
 Mettre_a_jour();
// fenetre_bouton->Couleur_bordure_int_angle();
// fenetre_bouton->Couleur_bordure_int();
}*/

alx_noeud_bouton_sdl_opengl::~alx_noeud_bouton_sdl_opengl()
{delete matos;
 delete fenetre_bouton;}

/*info_du_contenant* alx_noeud_bouton_sdl_opengl::Contient(alx_point2D &pt, int action)
{alx_point2D pt2(pt);
 pt2.Translation_inverse(vect_trans);
 if(Rotation()!=0)
   pt2.Pivoter(-Rotation(), Pt_nul());

 info_du_contenant *rep = fenetre_bouton->Contient( pt2
                                                  , ALX_NOEUD_OPERATION_SI_CONTIENT_ALORS_RIEN); //action); //fenetre_bouton->Fenetre_opengl()->Fenetre()->MP_corps()->Contient(pt);
 if( rep )
   return rep;

 return Contenu_dans_fils(pt2, action);
}*/



//______________________________________________________________________________
//___________________Les méthodes plus spécifiques au bouton____________________
//______________________________________________________________________________
void alx_noeud_bouton_sdl_opengl::rationnaliser(int num)
{
 switch(num)
  {
   case -1: // On nous informe que le bouton à été relaché, seul un lieur y aura accès
    {//alx_point2D point( *pt_observe );
     alx_pointeur *ptr = interface_mere->Sim_pointeur()->Pointeur( pt_observe );
     alx_noeud_scene *noeud_sous_ptr = ptr->Noeud_sous_pointeur();
     //rep = ptr->Noeud_pere()->Contient(point, ALX_NOEUD_OPERATION_SI_CONTIENT_ALORS_RIEN);
     if( (noeud_sous_ptr == NULL)
       ||(noeud_sous_ptr != fenetre_bouton) )
       Relacher_bouton();
      else Activer_bouton();
     Arreter_appel_a_info_sur_pt( (alx_point2D*)(void*)(alx_classe_base_model::infos_comp[0])
                                , alx_pointeur_relache);
    }
   break;

   default: // Un message du modèle physique d'un corp de bouton. (on peut en avoir plusieurs de corp, un peu des vues multiples finalement
    // On ne traite que la détection d'un point de contact:
     alx_model_simulation_physique1 *mp_corp = (alx_model_simulation_physique1*)(void*)(num); //fenetre_bouton->Fenetre_opengl()->Fenetre()->MP_corps();
     if( ( (mp_corp->infos_comp[0] != 2)&&(mp_corp->infos_comp[0] != 3)&&(mp_corp->infos_comp[0]!=6) )
       ||(mp_corp->infos_comp[1] != 0) ) return; // Ce message n'est pas le bon
      {
       if( ((mp_corp->infos_comp[0]==2)||(mp_corp->infos_comp[0]==6))
         &&(etat_bouton == Bouton_relache) )
        {
         Enfoncer_bouton();
         pt_observe = (alx_point2D*)(void*)(mp_corp->infos_comp[2]);
         Appel_a_info_sur_pt(pt_observe, alx_pointeur_relache);
        }
      }

   break;
  }
}



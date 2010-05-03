#include "alx_noeud_fiche_ovale_sdl_opengl.h"
#include "../../opengl/config_opengl.h"
//#include "../../utilitaires/alx_arbre.cpp"
#include "..\choses_communes.h"

#include "alx_interface_sdl_opengl_1.h"
#include "../classiques/alx_fiche_ovale_opengl.h"


//------------------------------------------------------------------------------
void alx_noeud_fiche_ovale_sdl_opengl::init()
{int i;

 rationnalise_fo = false;

 fenetre_opengl = (alx_fenetre_opengl*)fiche_gl;
 fenetre        = (alx_fenetre*)fiche;

 coord_x_prcdt = coord_y_prcdt = -1;
 //rendre_fond = true;

 fiche->abonner_a((alx_classe_base_liant*)this, 1);
 fiche->Longueur_corp( fiche->Longueur_corp() );
 fiche->Hauteur_corp(  fiche->Hauteur_corp()  );

 lg_deb = fiche->Longueur_corp()/2;
 ht_deb = fiche->Hauteur_corp()/2;
 Origine(lg_deb, ht_deb);
// zdr_corp.repere.origine.maj(lg_deb, ht_deb);

 A_corp.e       = &(fiche->mp_corp);
 A_entete.e     = &(fiche->mp_entete);
 A_zone_rot.e   = &(fiche->mp_zone_rot);
 A_zone_redim.e = &(fiche->mp_zone_redim);

 A_corp.Ajouter_fils(&A_zone_rot);
 A_corp.Ajouter_fils(&A_zone_redim);
 A_corp.Ajouter_fils(&A_entete);

 if(Sim_contact())
  {
   ens_contact_fiche1       = Sim_contact()->Ajouter_ensemble_contact();
   ens_contact_fiche1_redim = Sim_contact()->Ajouter_ensemble_contact();

   mod_contact_entete = Sim_contact()->Ajouter_modele( ens_contact_fiche1
                                                   , &(fiche->mp_entete)
                                                   , this
                                                   , alx_point_contact_capacite_translation
                                                    |alx_point_contact_capacite_rapporteur );
   mod_contact_corp = Sim_contact()->Ajouter_modele( ens_contact_fiche1
                              , &(fiche->mp_corp)
                              , this
                              , alx_point_contact_capacite_translation
                               |alx_point_contact_capacite_rotation );
   mod_contact_zone_rot = Sim_contact()->Ajouter_modele( ens_contact_fiche1
                                                     , &(fiche->mp_zone_rot)
                                                     , this
                                                     , alx_point_contact_capacite_rotation );

   mod_contact_zone_redim = Sim_contact()->Ajouter_modele( ens_contact_fiche1_redim
                                                       , &(fiche->mp_zone_redim)
                                                       , this
                                                       , alx_point_contact_capacite_translation
                                                        |alx_point_contact_capacite_rapporteur );

   Sim_contact()->Fixer_point_translation_defaut( ens_contact_fiche1
                                              , mod_contact_entete
                                              , &(fiche->centre_entete) );
   Sim_contact()->Fixer_point_rotation_defaut( ens_contact_fiche1
                                           , mod_contact_zone_rot
                                           , &(fiche->centre_corp) );
   ens_contact_fiche1->pt_rot_defaut.Lier  ( &(fiche->mp_corp) );
   ens_contact_fiche1->pt_trans_defaut.Lier( &(fiche->mp_entete) );
  }

 Origine(0, 0);
 nfo_rappel_chgmnt_dim = FONCTION_DE_RAPPEL( alx_noeud_fiche_ovale_sdl_opengl
                                           ,&alx_noeud_fiche_ovale_sdl_opengl::Rationnaliser_chgmnt_dim);
 fiche->abonner_a_dimension( nfo_rappel_chgmnt_dim );
 fiche_gl->Mettre_a_jour();
}

//------------------------------------------------------------------------------
alx_noeud_fiche_ovale_sdl_opengl::alx_noeud_fiche_ovale_sdl_opengl( const char *nom, const double tx, const double ty
                                                                  , alx_interface_sdl_opengl_1 *interface_m) : alx_noeud_fenetre_sdl_opengl( nom, tx, ty, interface_m )
                                                                                                             , origine_interne_logique(0,0)
{fiche    = new alx_fiche_ovale(nom, tx, ty, 25, 15);
 fiche_gl = new alx_fiche_ovale_opengl(fiche, fonte_Arial(), Matos());
 fiche->repere = (alx_repere2D*)this;
 fiche_gl->Zdr_entete(&zdr_entete);
 init();
 noeud_entete.Maj_fenetre( (alx_fenetre_opengl*)fiche_gl );
 PreRendreEntete();
 //Serialiser_type();
}

alx_noeud_fiche_ovale_sdl_opengl::alx_noeud_fiche_ovale_sdl_opengl( alx_fiche_ovale_opengl *f_opengl
                                                      , alx_interface_sdl_opengl_1 *interface_m
                                                      , alx_prerendeur *prerendeur
                                                      , double nb_pixels_par_unite
                                                      , HDC hdc, HGLRC hglrc
                                                      , ALXGL_TYPE_RENDU type_rendu) : alx_noeud_fenetre_sdl_opengl( f_opengl
                                                                                                                   , interface_m
                                                                                                                   , prerendeur
                                                                                                                   , nb_pixels_par_unite
                                                                                                                   , hdc, hglrc
                                                                                                                   , type_rendu)
                                                                                      , origine_interne_logique(0,0)
{fiche_gl = f_opengl;
 fiche    = f_opengl->fiche;
 fiche->repere = (alx_repere2D*)this;
 init();
 //Serialiser_type();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void alx_noeud_fiche_ovale_sdl_opengl::Titre(const char *t)
{alx_noeud_fenetre_sdl_opengl::Titre(t);
 fiche->Nom(t);
 Forcer_pre_rendu_entete(true);
//   PreRendreEntete();
// Forcer_pre_rendu_entete(false);
}
void alx_noeud_fiche_ovale_sdl_opengl::Titre(const alx_chaine_char &t)
{Titre( t.Texte() );}

//______________________________________________________________________________
void alx_noeud_fiche_ovale_sdl_opengl::Rationnaliser_chgmnt_dim(void *param)
{if(rationnalise_fo) return;
 rationnalise_fo = true;
 fiche_gl->Mettre_a_jour();
 rationnalise_fo = false;
 }

//______________________________________________________________________________
void alx_noeud_fiche_ovale_sdl_opengl::Dessin_noeud()
{if(!Texture_tempon_prete())
   return;

 double coord_x, coord_y;
 glPushMatrix();
 glPushAttrib(GL_ENABLE_BIT);

 fiche_gl->maj_fact();
 fiche_gl->Etirement_commun();
 if( Bord_translucide() )
  {
   glEnable(GL_BLEND);
   glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  }

 if(Noeud_puis_fils())
  {if(Afficher_entete())   fiche_gl->afficher_entete();
   if(Afficher_bordure())  fiche_gl->afficher_bordure();
   if(Afficher_zone_rot()) fiche_gl->afficher_zone_rot();}

     if(Afficher_corps())
      {
       glEnable(GL_TEXTURE_2D);
       glBindTexture(GL_TEXTURE_2D, zdr_corp.Infos_texture_tempon(0)->id_texture/*infos_texture_tempon.id_texture*/);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
       if( Lisser() )
        {glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);}
        else
         {glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);}
       glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

       if( Corp_translucide() )
        {
         glEnable(GL_BLEND);
         glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
         glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        }
        else glDisable(GL_BLEND);

       coord_x = zdr_corp.Nb_unite_x()*zdr_corp.Nb_pixels_par_unite()/zdr_corp.Infos_texture_tempon(0)->tx;
       coord_y = zdr_corp.Nb_unite_y()*zdr_corp.Nb_pixels_par_unite()/zdr_corp.Infos_texture_tempon(0)->ty;

       if( (coord_x_prcdt != coord_x)
         ||(coord_y_prcdt != coord_y) )
        {
         fiche_gl->Maj_coord_text(coord_x, coord_y);
         coord_x_prcdt = coord_x;
         coord_y_prcdt = coord_y;
        }

       //glColor4f(1,1,1,1);
       fiche_gl->afficher_corp();
      }

 if(!Noeud_puis_fils())
  {if(Afficher_entete())   fiche_gl->afficher_entete();
   if(Afficher_bordure())  fiche_gl->afficher_bordure();
   if(Afficher_zone_rot()) fiche_gl->afficher_zone_rot();}

 glPopAttrib();
 glPopMatrix();
}

//------------------------------------------------------------------------------
// Renvoi un pointeur sur le modèle physique qui contient la primitive, NULL si aucun.
bool Zone_pointee(alx_model_simulation_physique1 **e, void *info_sup)
{alx_point2D                    *pt = (alx_point2D*)info_sup;
 alx_model_simulation_physique1 *mp = *e;

 if( mp->Contient(*pt) )
   return true;

 return false;
}

//------------------------------------------------------------------------------
info_du_contenant* alx_noeud_fiche_ovale_sdl_opengl::Noeud_contient(const alx_point2D &pt, int action)
{Contact_avec_corp( false );
 A_pt_tmp.maj(pt);

 if( !boite_noeud.Contient(A_pt_tmp) )
   return (info_du_contenant*)NULL;

 info_du_contenant *rep;
 alx_model_simulation_physique1 **mp_rep;
// alx_prerendeur *rendeur = Prerendeur_externe();

 if( (mp_rep = A_corp.Rechercher_et_traiter(Zone_pointee, &A_pt_tmp)) != NULL)
 {if( *mp_rep != &(fiche->mp_corp) )
   {// Eliminer les cas dont on ne veut pas...
    if( (!Gerer_entete())
      &&(*mp_rep == &(fiche->mp_entete)) ) return Rep_puit();
    if( (!Gerer_bordure())
      &&(*mp_rep == &(fiche->mp_zone_redim)) ) return Rep_puit();
    if( (!Gerer_zone_rot())
      &&(*mp_rep == &(fiche->mp_zone_rot)) ) return Rep_puit();

    rep = new info_du_contenant();
    rep->mp    = *mp_rep;
    rep->noeud = (alx_noeud_scene*)this;
    rep->Point_au_contact(pt);
    return rep;
   }

   Contact_avec_corp( true );
   if(!Fond_touchable()) {return NULL;}
   if(!Gerer_corps())    {return Rep_puit();}
   rep = new info_du_contenant;
   rep->mp    = *mp_rep;
   rep->noeud = (alx_noeud_scene*)this;
   rep->Point_au_contact(A_pt_tmp);
   return rep;
  }

 // On est dans le cas où le point n'est pas dans la zone restrictive de la fenêtre.
 return NULL;
}

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//______________________________________________________________________________
void alx_noeud_fiche_ovale_sdl_opengl::PreRendre()
{if( !LR_Avant_pre_rendu.Vide() ) {LR_Avant_pre_rendu.Rappeler( this );}
 PreRendreEntete();

// Gestion de la boîte englobante
 pt_tmp_boite.maj( Longueur_corp()/2
                 , Hauteur_corp() /2 );
 boite_noeud.maj(-pt_tmp_boite.X()-Epaisseur_bordure()                , -pt_tmp_boite.Y()-Epaisseur_bordure()
                ,-pt_tmp_boite.X()+Longueur_corp()+Epaisseur_bordure(), -pt_tmp_boite.Y()+Hauteur_corp()+Hauteur_entete()+Epaisseur_bordure() );
 tenir_compte_des_boites_des_fils = false;
//   PreRendre_fils();
 // Mise à jour des coordonnées de la boîte englobante dans les repères des pères.
  boite_noeud_et_fils.maj( boite_noeud );
  boite_noeud_et_fils_glob.maj( boite_noeud_et_fils );
  boite_noeud_et_fils_glob.Prendre_en_compte( *this );

// Le prérendu proprement dit du corp
// alx_noeud_fenetre_sdl_opengl::PreRendre(); // Pour le calul de la zone englobante

 //Prerendeur_fiche()->simuler();
 zdr_corp.Nb_unite_x( fiche->Longueur_corp() );
 zdr_corp.Nb_unite_y( fiche->Hauteur_corp()  );
 zdr_corp.Nb_pixels_par_unite( Nb_pixels_par_unite() );
 zdr_corp.PreRendre();
 Texture_tempon_prete(true);

 if( !LR_Apres_pre_rendu.Vide() ) {LR_Apres_pre_rendu.Rappeler( this );}
}



// Les codes
// 0 : Changement de dimension
//   0 : En longueur
//     *a : ancienne longueur (double)
//     *n : nouvelle longueur (double)
//   1 : En hauteur
//     *a : ancienne hauteur  (double)
//     *n : nouvelle hauteur  (double)
void alx_noeud_fiche_ovale_sdl_opengl::rationnaliser(int num)
{
 switch(num)
  {
   case 1: // La fiche ovale
    if(fiche->infos_comp[0] == 0 && fiche->nb_infos_comp == 4)
     {
      double l, h;

      if(fiche->infos_comp[1]==0)
       {l  = *((double*)(void*)(fiche->infos_comp[3]));
        h  = fiche->Hauteur_corp();}
      if(fiche->infos_comp[1]==1)
       {l  = fiche->Longueur_corp();
        h  = *((double*)(void*)(fiche->infos_comp[3]));}

      zdr_corp.Origine(l/2, h/2);
      Origine_du_contenu( origine_interne_logique );
     }
   break;
  }
}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
void alx_noeud_fiche_ovale_sdl_opengl::abonner_fiche_a( alx_classe_base_liant *liant
                                                      , int n)
{fiche->abonner_a(liant, n);}

void alx_noeud_fiche_ovale_sdl_opengl::Origine_du_contenu(const alx_point2D &pt)
{origine_interne_logique.maj(pt);
 alx_point2D tmp(pt);
 tmp.Translation(-fiche->Longueur_corp()/2, -fiche->Hauteur_corp()/2);
 alx_noeud_fenetre_sdl_opengl::Origine_du_contenu( tmp );}

const alx_point2D& alx_noeud_fiche_ovale_sdl_opengl::Position_bg() const
{origine_logique.maj( alx_noeud_scene::Origine() );
 origine_logique.Translation(-fiche->Longueur_corp()/2, -fiche->Hauteur_corp()/2);
 return origine_logique;}

void alx_noeud_fiche_ovale_sdl_opengl::Position_bg(const alx_point2D &p)
{Position_bg(p.x, p.y);}

void alx_noeud_fiche_ovale_sdl_opengl::Position_bg(const double x, const double y)
{alx_noeud_scene::Origine(x, y);
 Translation(fiche->Longueur_corp()/2, fiche->Hauteur_corp()/2);}

const alx_point2D& alx_noeud_fiche_ovale_sdl_opengl::Origine() const
{origine_logique.maj( alx_noeud_scene::Origine() );
 return origine_logique;}

void alx_noeud_fiche_ovale_sdl_opengl::Origine(const alx_point2D &pt)
{alx_noeud_scene::Origine(pt);}

void alx_noeud_fiche_ovale_sdl_opengl::Origine(const double x, const double y)
{alx_noeud_scene::Origine(x, y);}

const double alx_noeud_fiche_ovale_sdl_opengl::Longueur_totale() const
{return Longueur_corp() + Longueur_entete() + 2*(Epaisseur_bordure()+Hauteur_entete());}


//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
void alx_noeud_fiche_ovale_sdl_opengl::Calculer_boite_noeud()
{// Gestion de la boîte englobante
 double demi_tx = Longueur_corp() / 2
      , demi_ty = Hauteur_corp()  / 2
      , he = Hauteur_entete()
      , eb = Epaisseur_bordure();
 boite_noeud.maj(-he-demi_tx-eb,-demi_ty-eb
                , demi_tx+eb+he, demi_ty+he+eb );
 tenir_compte_des_boites_des_fils = false;
 // Mise à jour des coordonnées de la boîte englobante dans les repères des pères.
  boite_noeud_et_fils.maj( boite_noeud );
  boite_noeud_et_fils_glob.maj( boite_noeud_et_fils );
  boite_noeud_et_fils_glob.Prendre_en_compte( *this );
}
//______________________________________________________________________________
void alx_noeud_fiche_ovale_sdl_opengl::Calculer_boite_fils()
{}


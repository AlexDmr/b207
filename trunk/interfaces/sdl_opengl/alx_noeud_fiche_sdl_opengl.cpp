#include "alx_noeud_fiche_sdl_opengl.h"

#include "../classiques/alx_fiche_opengl.h"
#include "../../opengl/config_opengl.h"
#include "..\choses_communes.h"

//------------------------------------------------------------------------------
void alx_noeud_fiche_sdl_opengl::init()
{int i;

 fenetre_opengl = (alx_fenetre_opengl*)fiche_gl;
 fenetre        = (alx_fenetre*)fiche;

 A_corp.e       = &(fiche->mp_corp);
 A_entete.e     = &(fiche->mp_entete);
 A_zone_rot.e   = &(fiche->mp_zone_rot);
 for(i=0;i<8;i++)
   A_zone_redim[i].e = &(fiche->mp_zone_redim[i]);

 A_corp.Ajouter_fils(&A_entete);
 A_corp.Ajouter_fils(&A_zone_rot);
 for(i=0;i<8;i++) A_corp.Ajouter_fils(&(A_zone_redim[i]));

 if(Sim_contact())
  {
   ens_contact_fiche1       = Sim_contact()->Ajouter_ensemble_contact();
   for(i=0;i<8;i++)
     ens_contact_fiche1_redim[i] = Sim_contact()->Ajouter_ensemble_contact();

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

   for(i=0;i<8;i++)
     Sim_contact()->Ajouter_modele( ens_contact_fiche1_redim[i]
                                , &(fiche->mp_zone_redim[i])
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

 nf_rappel_chgmnt_dim = FONCTION_DE_RAPPEL( alx_noeud_fiche_sdl_opengl
                                          ,&alx_noeud_fiche_sdl_opengl::Rationnaliser_chgmnt_dim);
 fiche->abonner_a_dimension( nf_rappel_chgmnt_dim );
 fiche_gl->Mettre_a_jour();
}

void alx_noeud_fiche_sdl_opengl::Rationnaliser_chgmnt_dim(void *param)
{fiche_gl->Mettre_a_jour();}

//------------------------------------------------------------------------------
alx_noeud_fiche_sdl_opengl::alx_noeud_fiche_sdl_opengl( const char *nom, const double tx, const double ty
                                                      , alx_interface_sdl_opengl_1 *interface_m) : alx_noeud_fenetre_sdl_opengl( nom, tx, ty, interface_m )
{fiche    = new alx_fiche(nom, tx, ty, 25, 15);
 fiche_gl = new alx_fiche_opengl(fiche, fonte_Arial(), Matos());
 fiche->repere = (alx_repere2D*)this;
 fiche_gl->Zdr_entete(&zdr_entete);
 init();
 noeud_entete.Maj_fenetre( (alx_fenetre_opengl*)fiche_gl );
 PreRendreEntete();
 //Serialiser_type();
}
//______________________________________________________________________________
alx_noeud_fiche_sdl_opengl::alx_noeud_fiche_sdl_opengl( alx_fiche_opengl *f_opengl
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
{//init_default();
 fiche_gl = f_opengl;
 fiche    = f_opengl->fiche;
 fiche->repere = (alx_repere2D*)this;

 init();
 //Serialiser_type();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void alx_noeud_fiche_sdl_opengl::Titre(const char *t)
{alx_noeud_fenetre_sdl_opengl::Titre(t);
 fiche->Nom(t);
 Forcer_pre_rendu_entete(true);
//   PreRendreEntete();
// Forcer_pre_rendu_entete(false);
}
void alx_noeud_fiche_sdl_opengl::Titre(const alx_chaine_char &t)
{Titre( t.Texte() );}

//______________________________________________________________________________
void alx_noeud_fiche_sdl_opengl::Dessin_noeud()
{if( !Texture_tempon_prete()
   &&!Rendu_ecran_direct() ) return;

// Préparation de l'affichage proprement dit...
 double coord_x, coord_y;
 alx_sommet_opengl *ptr_corp = fiche_gl->Pointeur_sur_coord_corp()->tab_sommets;


    glPushAttrib(GL_ENABLE_BIT);

    if( Bord_translucide() )
     {glEnable(GL_BLEND);
      glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
     }

     if(Noeud_puis_fils())
      {if(Afficher_entete())   fiche_gl->afficher_entete();
       if(Afficher_bordure())  fiche_gl->afficher_bordure();
       if(Afficher_zone_rot()) fiche_gl->afficher_zone_rot();}

     if(Rendu_ecran_direct())
      {glDisable(GL_BLEND);
       Afficher_fils(0);
      }
      else if(Afficher_corps())
      {glEnable(GL_TEXTURE_2D);
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
       //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

       if( Corp_translucide() )
        {glEnable(GL_BLEND);
         glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
         glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        }
        else glDisable(GL_BLEND);

       coord_x = zdr_corp.Nb_unite_x()*zdr_corp.Nb_pixels_par_unite()/zdr_corp.Infos_texture_tempon(0)->tx;
       coord_y = zdr_corp.Nb_unite_y()*zdr_corp.Nb_pixels_par_unite()/zdr_corp.Infos_texture_tempon(0)->ty;
       ptr_corp[0].coord_texture[0] = 0.0    ; ptr_corp[0].coord_texture[1] = 0.0;
       ptr_corp[1].coord_texture[0] = coord_x; ptr_corp[1].coord_texture[1] = 0.0;
       ptr_corp[2].coord_texture[0] = coord_x; ptr_corp[2].coord_texture[1] = coord_y;
       ptr_corp[3].coord_texture[0] = 0.0    ; ptr_corp[3].coord_texture[1] = coord_y;
       glColor4dv( fiche_gl->Couleur_masque_corp() );
       fiche_gl->afficher_corp();
      }

 if(!Noeud_puis_fils())
  {if(Afficher_entete())   fiche_gl->afficher_entete  ();
   if(Afficher_bordure())  fiche_gl->afficher_bordure ();
   if(Afficher_zone_rot()) fiche_gl->afficher_zone_rot();}

 glPopAttrib();
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
info_du_contenant* alx_noeud_fiche_sdl_opengl::Noeud_contient(const alx_point2D &pt, int action)
{Contact_avec_corp( false );
 if( !boite_noeud.Contient(pt) )
   {return (info_du_contenant*)NULL;
   }

 info_du_contenant *rep;
 alx_model_simulation_physique1 **mp_rep;
 //alx_prerendeur *rendeur = Prerendeur_externe();

 A_pt_tmp.maj(pt);
 if( (mp_rep = A_corp.Rechercher_et_traiter(Zone_pointee, &A_pt_tmp)) != NULL)
  {if( *mp_rep != &(fiche->mp_corp) )
    {if( (!Gerer_entete())
       &&(*mp_rep == &(fiche->mp_entete)) ) {return Rep_puit();
                                            }
     if( (!Gerer_zone_rot())
       &&(*mp_rep == &(fiche->mp_zone_rot)) ) {return Rep_puit();
                                              }
     if(!Gerer_bordure())
      {for(int i=0;i<8;i++)
        {if(*mp_rep == &(fiche->mp_zone_redim[i])) {return Rep_puit();
                                                   }
        }
      }
     rep = new info_du_contenant();
     rep->mp    = *mp_rep;
     rep->noeud = (alx_noeud_scene*)this;
     rep->Point_au_contact(pt);
     return rep;
    }

  // Si on est sur le corp, on vérifie d'abord que le point n'appartient pas à un fils
  // On n'oublie donc pas de passer dans le repère mp_corp de la fiche
  // D'ailleur on le défini ce repère nom de nom!
   Contact_avec_corp( true );
   if(!Fond_touchable()) {return NULL;      }
   if(!Gerer_corps())    {return Rep_puit();}
   rep = new info_du_contenant();
   rep->mp    = *mp_rep;
   rep->noeud = (alx_noeud_scene*)this;
   rep->Point_au_contact(pt);
   return rep;
  }

 // On est dans le cas où le point n'est pas dans la zone restrictive de la fenêtre.
 return NULL;
}

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//______________________________________________________________________________
void alx_noeud_fiche_sdl_opengl::PreRendre()
{if( !LR_Avant_pre_rendu.Vide() ) {LR_Avant_pre_rendu.Rappeler( this );}
// const bool a_changer_tmp = A_changer();
   PreRendreEntete();

   PreRendre_fils();
  // Le prérendu proprement dit du corp
   alx_noeud_fenetre_sdl_opengl::PreRendre(); // Pour le calul de la zone englobante

   if( !Rendu_ecran_direct())
    {//Prerendeur_fiche()->simuler();
     zdr_corp.Nb_unite_x( fiche->Longueur_corp() );
     zdr_corp.Nb_unite_y( fiche->Hauteur_corp()  );
     zdr_corp.Nb_pixels_par_unite( Nb_pixels_par_unite() );
     zdr_corp.PreRendre();
     Texture_tempon_prete(true);
    }

// A_changer( a_changer_tmp );
 if( !LR_Apres_pre_rendu.Vide() ) {LR_Apres_pre_rendu.Rappeler( this );}
}

//______________________________________________________________________________
void alx_noeud_fiche_sdl_opengl::abonner_fiche_a( alx_classe_base_liant *liant
                                                , int n)
{fiche->abonner_a(liant, n);}


//______________________________________________________________________________
//_______________________________ La sérialisation______________________________
//______________________________________________________________________________
//________________________________ La différence _______________________________
const alx_chaine_char& alx_noeud_fiche_sdl_opengl::Serialiser_difference_type(const alx_chaine_char &diff_contenu) const
{if(diff_contenu.Taille()==0)
  {serialisation_differences_type_fr.Maj("",0,0);
   return serialisation_differences_type_fr;}
 serialisation_differences_type_fr.Maj("<fiche_r >", 0, 10);
   serialisation_differences_type_fr += diff_contenu;
 serialisation_differences_type_fr.Ajouter("</fiche_r>", 0, 10);

 return serialisation_differences_type_fr;
}
//______________________________________________________________________________
const alx_chaine_char& alx_noeud_fiche_sdl_opengl::Serialiser_difference_contenu() const
{return alx_noeud_fenetre_sdl_opengl::Serialiser_difference_type();}

//______________________________________________________________________________
//_______________________________ La sérialisation______________________________
const alx_chaine_char& alx_noeud_fiche_sdl_opengl::Serialiser_type(const alx_chaine_char &contenu) const
{serialisation_type_fr.Maj("<fiche_r >", 0, 10);
   serialisation_type_fr += contenu;
 serialisation_type_fr.Ajouter("</fiche_r>", 0, 10);
 return serialisation_type_fr;
}
//______________________________________________________________________________
const alx_chaine_char& alx_noeud_fiche_sdl_opengl::Serialiser_contenu() const
{return alx_noeud_fenetre_sdl_opengl::Serialiser_type();}

//______________________________________________________________________________
//______________________________ La désérialisation_____________________________
void alx_noeud_fiche_sdl_opengl::Deserialiser_type(const alx_chaine_char &txt, unsigned int &pos)
{pos += 10; // On passe la balise de début
   alx_noeud_fenetre_sdl_opengl::Deserialiser_type(txt, pos);
 pos += 10; // On passe la balise de fin
}
//______________________________________________________________________________
void alx_noeud_fiche_sdl_opengl::Deserialiser(const alx_chaine_char &txt, unsigned int &pos)
{alx_noeud_fenetre_sdl_opengl::Deserialiser(txt, pos);}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
void alx_noeud_fiche_sdl_opengl::Calculer_boite_noeud()
{// Gestion de la boîte englobante
 boite_noeud.maj(-Epaisseur_bordure(),-Epaisseur_bordure()
                , Longueur_corp()+Epaisseur_bordure(), Hauteur_corp()+Hauteur_entete()+Epaisseur_bordure() );
 tenir_compte_des_boites_des_fils = false;
 boite_fils.maj(0, 0, Longueur_corp(), Hauteur_corp());
 // Mise à jour des coordonnées de la boîte englobante dans les repères des pères.
  boite_noeud_et_fils.maj( boite_noeud );
  boite_noeud_et_fils_glob.maj( boite_noeud_et_fils );
  boite_noeud_et_fils_glob.Prendre_en_compte( *this );
}
//______________________________________________________________________________
void alx_noeud_fiche_sdl_opengl::Calculer_boite_fils()
{}


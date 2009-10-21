#include "alx_noeud_editeur_olfa_comet.h"
#include "../fontes.h"
#include "../choses_communes.h"

void alx_noeud_editeur_olfa_comet::init_interface()
{alx_chaine_char nom_fiche = "Interface de ";
 nom_fiche += comet->Nom_usage();
 fiche = new alx_noeud_fiche_sdl_opengl( nom_fiche.Texte(), 320, 480, interface_mere );

 fiche->Translucidite_globale(0.6);
// fiche->Fenetre_translucide( true );
 z_texte = new alx_zone_texte_opengl( fiche->Fenetre()->Longueur_corp(), fiche->Fenetre()->Hauteur_corp()
                                    , fonte_Arial(), interface_mere->Sim_dispo_saisi());
 z_texte->Zoom(20);
 zone_texte = new alx_noeud_zone_texte_interpreteur_tcl_sdl_opengl(z_texte, Olfa_comet()->Interpreteur());
 fiche->Ajouter_fils( (alx_noeud_scene*)zone_texte );
 zone_texte->abonner_a_caractere_tape( FONCTION_DE_RAPPEL( alx_noeud_editeur_olfa_comet
                                                         ,&alx_noeud_editeur_olfa_comet::Rationnaliser_chgmnt_dim_fiche) );
 z_texte->abonner_a_chgmnt_pos_curseur( FONCTION_DE_RAPPEL( alx_noeud_editeur_olfa_comet
                                                          ,&alx_noeud_editeur_olfa_comet::Rationnaliser_chgmnt_dim_fiche) );

 this->Ajouter_fils( (alx_noeud_scene*)fiche );
 this->Position_dans_liste_des_fils_changeable( true );

 fiche->Triangle_control(false);
 fiche->Fenetre()->abonner_a_dimension( FONCTION_DE_RAPPEL( alx_noeud_editeur_olfa_comet
                                                          , &alx_noeud_editeur_olfa_comet::Rationnaliser_chgmnt_dim_fiche) );
}

alx_noeud_editeur_olfa_comet::~alx_noeud_editeur_olfa_comet()
{delete z_texte;
 delete zone_texte;
 delete fiche;}

void alx_noeud_editeur_olfa_comet::Rationnaliser_chgmnt_dim_fiche(void *param)
{z_texte->Hauteur( fiche->Fenetre()->Hauteur_corp()  );
 z_texte->Largeur( fiche->Fenetre()->Longueur_corp() );
 alx_noeud_scene::Ne_pas_pre_rendre(false);}

void alx_noeud_editeur_olfa_comet::PreRendre()
{alx_noeud_scene::PreRendre();
 alx_noeud_scene::Ne_pas_pre_rendre(true);}


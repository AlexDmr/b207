#include "alx_telec_camnote_cpp_bigre.h"
#include "..\interfaces\fontes.h"
#include "..\interfaces\choses_communes.h"
#include "..\physique\math_alex.cpp"

alx_telec_camnote_cpp_bigre::alx_telec_camnote_cpp_bigre( char *nom
                                          //, cogitant::Environment *e
                                          , alx_interpreteur_tcl  *interp
                                          , alx_interface_sdl_opengl_1 *i_mere
                                          , alx_noeud_scene *n_pere
                                          , const INFOS_TEXTURE *text_cam
                                          , const unsigned int nb_octets_par_pix
                                          , const int ordre_couleur  ) : alx_telec_camnote_cpp(nom, /*e,*/ interp)
                                                                            , olfa_comet( "CamTele"
                                                                                        //, e
                                                                                        , interp )
                                                                            , alx_noeud_fiche_sdl_opengl("Télécommande"
                                                                                                        , 320, 520
                                                                                                        , i_mere )
{Nom_IU("Telecommande");
 interface_mere = i_mere;
 texture_camera = *text_cam;

 if(n_pere)
   n_pere->Ajouter_fils( (alx_noeud_scene*)this );

// Interfaçage avec TCL
 Enregistrer_CamNote_Tele((olfa_comet*)this, this);

 fct_rappel_clavier = FONCTION_DE_RAPPEL( alx_telec_camnote_cpp_bigre
                                        ,&alx_telec_camnote_cpp_bigre::Rationnaliser_clavier);

 visu_cam = new alx_noeud_image_sdl_opengl(*text_cam, 320, 240);
   visu_cam->Nom_IU("Image\nCamera");
// this->Gerer_bordure(false);
 this->Gerer_zone_rot(false);
 this->Afficher_zone_rot(false);
  //this->Gerer_corps(false);
 this->Epaisseur_bordure(5);
 this->Gerer_bordure(false);

 fiche_num_diapo = new alx_noeud_fiche_sdl_opengl("Informations", 320, 240, interface_mere);
   fiche_num_diapo->Nom_IU("Pilotage");
 fiche_num_diapo->Fenetre()->Epaisseur_bordure(1);
 fiche_num_diapo->Lisser(false);
 fiche_num_diapo->Gerer_bordure(false);
 fiche_num_diapo->Afficher_zone_rot(false);
 fiche_num_diapo->Hauteur_entete(0);
 fiche_num_diapo->Afficher_bordure( false );
 fiche_num_diapo->Mettre_a_jour();
 fiche_num_diapo->Position_bg(0, 240);
 fiche_num_diapo->Compter_pre_rendu(false);
 this->Ajouter_fils( (alx_noeud_scene*)fiche_num_diapo );
 fiche_num_diapo->Fenetre_translucide( true );
 fiche_num_diapo->Translucidite_globale(0.65);
 fiche_num_diapo->Couleur_fond(0,0,0,0);
 fiche_num_diapo->Gerer_corps(true);
 fiche_num_diapo->Fond_touchable(false);

// La zone d'édition de notes :
 z_texte = new alx_zone_texte_opengl(320, 240, fonte_Arial(), interface_mere->Sim_dispo_saisi());
 z_texte->Zoom(20);
 zone_texte = new alx_noeud_zone_texte_sdl_opengl(z_texte);
 zone_texte->Origine(0, 0/*240+fiche_num_diapo->Hauteur_totale()*/);
   zone_texte->Nom_IU("Notes");
 this->Ajouter_fils( (alx_noeud_scene*)zone_texte );

// A l'intérieur de la sous fenêtre de pilotage :
 potentiometre_translucidite = new alx_noeud_potentiometre_sdl_opengl(interface_mere, 244, 32);
   potentiometre_translucidite->Nom_IU("Potentiomètre\ntranslucidité");
 potentiometre_translucidite->Epaisseur_bordure( 3 );
 potentiometre_translucidite->Origine(3, 43);
 fiche_num_diapo->Ajouter_fils( (alx_noeud_scene*)potentiometre_translucidite );

 z_texte_nom_diapo = new alx_zone_texte_opengl( 100, 30, fonte_Arial()
                                              , interface_mere->Sim_dispo_saisi());

 z_texte_nom_diapo->Zoom(25);
 z_texte_nom_diapo->Maj_texte("alex ");
 alx_telec_camnote_cpp::Nom_de_transparent( z_texte_nom_diapo->Chaine_char() );

 zone_texte_nom_diapo = new alx_noeud_zone_texte_sdl_opengl(z_texte_nom_diapo);
   zone_texte_nom_diapo->Nom_IU("Nom de la\ndiapositive");
 zone_texte_nom_diapo->Origine(0, 110);
 fiche_num_diapo->Ajouter_fils( (alx_noeud_scene*)zone_texte_nom_diapo );


 char tmp[16];
 z_texte_numero = new alx_zone_texte_opengl(90, 30, fonte_Arial(), interface_mere->Sim_dispo_saisi());
 z_texte_numero->Zoom(25);
 _itoa_s(alx_telec_cpp::Numero_transparent(), tmp, 10); z_texte_numero->Maj_texte( tmp );
 zone_texte_numero = new alx_noeud_zone_texte_sdl_opengl(z_texte_numero);
   zone_texte_numero->Nom_IU("Numéro de\ndiapositive");
 zone_texte_numero->Origine(115, 211);
 fiche_num_diapo->Ajouter_fils( (alx_noeud_scene*)zone_texte_numero );

 //Charger_presentation( z_texte_nom_diapo->texte.Texte() );
// Les boutons__________________________________________________________________
 Initialiser_boutons();
 bouton_prcdt       = new alx_noeud_bouton_sdl_opengl( *Image_bouton_dec() , interface_mere, interface_mere->Noeud_scene(), 2);
   bouton_prcdt->Nom_IU("Bouton\nprécédent");
 bouton_svt         = new alx_noeud_bouton_sdl_opengl( *Image_bouton_inc() , interface_mere, interface_mere->Noeud_scene(), 2);
   bouton_svt->Nom_IU("Bouton\nsuivant");
 bouton_aller_a     = new alx_noeud_bouton_sdl_opengl( *Image_bouton_rect(), interface_mere, interface_mere->Noeud_scene(), 1);
   bouton_aller_a->Nom_IU("Bouton\naller à");
 bouton_pix_mirroir = new alx_noeud_bouton_sdl_opengl( *Image_bouton_rect(), interface_mere, interface_mere->Noeud_scene(), 1);
   bouton_pix_mirroir->Nom_IU("Bouton\npixels m.");

 bouton_prcdt->Longueur(100); bouton_prcdt->Hauteur(64);             bouton_prcdt->Etirement_du_contenu(100.0/16, 64.0/16);       bouton_prcdt->Lisser(true);       bouton_prcdt->Mettre_a_jour();
 bouton_svt->Longueur(100);   bouton_svt->Hauteur(64);               bouton_svt->Etirement_du_contenu(100.0/16, 64.0/16);         bouton_svt->Lisser(true);         bouton_svt->Mettre_a_jour();
 bouton_aller_a->Longueur(90); bouton_aller_a->Hauteur(30);          bouton_aller_a->Etirement_du_contenu(90.0/16, 30.0/16);      bouton_aller_a->Lisser(true);     bouton_aller_a->Mettre_a_jour();
 bouton_pix_mirroir->Longueur(310); bouton_pix_mirroir->Hauteur(30); bouton_pix_mirroir->Etirement_du_contenu(310.0/16, 30.0/16); bouton_pix_mirroir->Lisser(true); bouton_pix_mirroir->Mettre_a_jour();



 bouton_prcdt->Position_bg(5, 171);
 bouton_svt->Position_bg(215, 171);
 bouton_pix_mirroir->Position_bg(5, 5);
 bouton_aller_a->Position_bg(115, 171);

 fiche_num_diapo->Ajouter_fils( (alx_noeud_scene*)bouton_prcdt );
 fiche_num_diapo->Ajouter_fils( (alx_noeud_scene*)bouton_svt );
 fiche_num_diapo->Ajouter_fils( (alx_noeud_scene*)bouton_aller_a );
 fiche_num_diapo->Ajouter_fils( (alx_noeud_scene*)bouton_pix_mirroir );

 if(visu_cam)
  {visu_cam->Origine(0, 240);
   this->Ajouter_fils( (alx_noeud_scene*)visu_cam); //(&translation_visu_cam) );
//     translation_visu_cam.Coord(0,240/*fiche_num_diapo->Hauteur_totale()*/,0);
//     translation_visu_cam.Ajouter_fils( (alx_noeud_scene*)visu_cam );//(&etirement_telec) );
//       etirement_telec.Ajouter_fils( (alx_noeud_scene*)visu_cam );
  }

 // Redimensionnement de la télécommande :
 this->Hauteur_corp( /*fiche_num_diapo->Hauteur_totale()+*/240+240 );
 this->Mettre_a_jour();
 // Liens de rappels
 potentiometre_translucidite->abonner_a_valeur( FONCTION_DE_RAPPEL( alx_telec_camnote_cpp_bigre
                                                                  ,&alx_telec_camnote_cpp_bigre::Rationnaliser_translucidite_pixels_mirroirs) );
 bouton_prcdt->abonner_a_activation( FONCTION_DE_RAPPEL( alx_telec_camnote_cpp_bigre
                                                       ,&alx_telec_camnote_cpp_bigre::Rationnaliser_bouton_prcdt) );
 bouton_svt->abonner_a_activation(FONCTION_DE_RAPPEL( alx_telec_camnote_cpp_bigre
                                                    ,&alx_telec_camnote_cpp_bigre::Rationnaliser_bouton_svt) );
 bouton_aller_a->abonner_a_activation(FONCTION_DE_RAPPEL( alx_telec_camnote_cpp_bigre
                                                        ,&alx_telec_camnote_cpp_bigre::Rationnaliser_bouton_aller_a) );
 bouton_pix_mirroir->abonner_a_activation(FONCTION_DE_RAPPEL( alx_telec_camnote_cpp_bigre
                                                            ,&alx_telec_camnote_cpp_bigre::Rationnaliser_activation_pixels_mirroirs) );
}

void alx_telec_camnote_cpp_bigre::Rationnaliser_activation_pixels_mirroirs(void *param)
{Activation_pix_mirroir( !Activation_pix_mirroir() );}

void alx_telec_camnote_cpp_bigre::Rationnaliser_translucidite_pixels_mirroirs(void *param)
{alx_telec_camnote_cpp::Intensite_pixels_miroirs( ( (float)(potentiometre_translucidite->Valeur())
                                                / (float)(potentiometre_translucidite->Valeur_max()) ) );}

void alx_telec_camnote_cpp_bigre::Rationnaliser_bouton_prcdt(void *param)
{Numero_transparent( Amax(alx_telec_cpp::Numero_transparent()-1, (unsigned int)1) );}

void alx_telec_camnote_cpp_bigre::Rationnaliser_bouton_svt(void *param)
{Numero_transparent( Amax(alx_telec_cpp::Numero_transparent()+1, (unsigned int)1) );}

void alx_telec_camnote_cpp_bigre::Rationnaliser_bouton_aller_a(void *param)
{Numero_transparent( Amax( atoi(z_texte_numero->Chaine_char().Texte()), 1) );}

void alx_telec_camnote_cpp_bigre::Numero_transparent(const unsigned int num)
{static char nom_diapo[256];
 if(alx_telec_cpp::Numero_transparent() == num) return;
 // Sauvegarde des notes:
 alx_telec_cpp::Commentaire(alx_telec_cpp::Numero_transparent(), z_texte->Chaine_char());
 // Changement de transparent:
 alx_telec_cpp::Numero_transparent(num);
 // Chargement des notes du nouveau transparent:
 z_texte->Maj_texte( alx_telec_cpp::Commentaire(alx_telec_cpp::Numero_transparent()) );
 z_texte->Position_curseur(0);
 // Mise à jour de l'affichage du numéro de transparent
 _itoa_s(alx_telec_cpp::Numero_transparent(), nom_diapo, 10); z_texte_numero->Maj_texte( nom_diapo );
}

// Version clavier
void alx_telec_camnote_cpp_bigre::Rationnaliser_clavier(void *param)
{if( interface_mere->Sim_dispo_saisi()->L_evt.Vide() )
   return;
 alx_evennement_dispositif_saisie *evt = &( interface_mere->Sim_dispo_saisi()->L_evt.Premier_E() );
 if(evt->Type_evt() != alx_dispositif_saisie_appuie )
   return;
 char s[2];
 s[0] = evt->Info_comp().caractere_saisie;
 s[1] = 0;
 if( (s[0] == ' ')
   ||(evt->Info_comp().caractere_saisie == SDLK_RIGHT) )
  {Rationnaliser_bouton_svt(NULL);}
 if( (evt->Info_comp().caractere_saisie == SDLK_LEFT) )
  {Rationnaliser_bouton_prcdt(NULL);}

 if(s[0]=='p' || s[0]=='P')
  {Activation_pix_mirroir( !Activation_pix_mirroir() );}
}

//______________________________________________________________________________
/*info_du_contenant* alx_telec_camnote_cpp_bigre::Contient(alx_point2D &pt, int action)
{info_du_contenant *rep = alx_noeud_fiche_sdl_opengl::Contient(pt, action);
 if( rep )
  { // Voir si on ne se trouve pas au dessus de l'image
    // auquel cas on augmente l'opacité des boutons de navigation.
   info_du_contenant *rep2;
   pt_tmp.maj(pt);
   this->Changer_coordonnees( pt_tmp );
   this->repere_des_fils.Changer_coordonnees( pt_tmp );
   rep2 = fiche_num_diapo->Contient(pt_tmp, ALX_NOEUD_OPERATION_SI_CONTIENT_ALORS_RIEN);
   if( rep2 )
    {fiche_num_diapo->Translucidite_globale( Amin(0.7, fiche_num_diapo->Translucidite_corp()+1.0/256) );
     if(rep2 != Rep_puit())
       delete rep2;
    }
    else fiche_num_diapo->Translucidite_globale( Amax(0.1, fiche_num_diapo->Translucidite_corp()-1.0/256) );
  }

 return rep;
}
*/
//______________________________________________________________________________
void alx_telec_camnote_cpp_bigre::Afficher()
{alx_noeud_fiche_sdl_opengl::Afficher();}


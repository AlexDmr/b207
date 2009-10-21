#include "alx_visu_image_camnote_cpp_bigre.h"
#include "..\interfaces\fontes.h"
#include "..\interfaces\choses_communes.h"
#include "..\physique\math_alex.cpp"

alx_visu_image_camnote_cpp_bigre::alx_visu_image_camnote_cpp_bigre( char *nom
                                                              //, cogitant::Environment *e
                                                              , alx_interpreteur_tcl  *interp
                                                              , alx_interface_sdl_opengl_1 *i_mere
                                                              , alx_noeud_scene *n_pere
                                                              , const INFOS_TEXTURE *text_cam
                                                              , const unsigned int nb_octets_par_pix
                                                              , const int ordre_couleur) : alx_visu_image_camnote_cpp(nom, /*e,*/ interp)
                                                                                                , olfa_comet( "CamVisu"
                                                                                                            //, e
                                                                                                            , interp )
                                                                                                , alx_noeud_fiche_sdl_opengl("Visualisateur de diapositives"
                                                                                                                            , 320, 240
                                                                                                                            , i_mere )
{Nom_IU("Visualisateur");
 interface_mere = i_mere;
 if(n_pere)
   n_pere->Ajouter_fils( (alx_noeud_scene*)this );

// Interfaçage avec TCL
 Enregistrer_CamNote_Visu((olfa_comet*)this, this);

// mise_a_jour_a_faire = true;
 intensite_pixels_miroirs = 0;
 Initialiser_matos();

 this->Rendre_fond(false);
 this->Rendu_ecran_direct(true);

 diapo    = new alx_noeud_image_sdl_opengl("Brectangle.bmp");
 diapo->Img().Etirement(1,1);
 diapo->Img().Couleur_def(true);
 diapo->Img().Lisser(true);
 diapo->Img().Inverser_y(true);
 diapo->Noeud_touchable( false );
   diapo->Nom_IU("Diapositive");
 visu_cam = new alx_noeud_image_sdl_opengl(*text_cam, 320, 240);
 visu_cam->Nb_octets_par_pixel     ( nb_octets_par_pix );
 visu_cam->Ordonnancement_couleurs ( ordre_couleur );
 visu_cam->Img().Etirement(1,1);
 visu_cam->Img().Inverser_x(true);
 visu_cam->Img().Lisser(true);
   visu_cam->Nom_IU("Pixels\nmirroir");
// p_translucidite.param = GL_BLEND;
// p_translucidite.facteur_s = GL_ONE_MINUS_DST_ALPHA;
// p_translucidite.facteur_d = GL_DST_COLOR; //GL_DST_COLOR;
 p_translucidite.couleur[0] = 1;
 p_translucidite.couleur[1] = 1;
 p_translucidite.couleur[2] = 1;
 p_translucidite.couleur[3] = 1-intensite_pixels_miroirs;
 noeud_translucidite.Param1(&p_translucidite);

 Activation_pix_mirroir(false);

// couleur_diapo.Coord(0.5,0.5,0.5,0.5);

// Construction du graphe de scène :
   this->Ajouter_fils( (alx_noeud_scene*)(&barriere_avec_pixmiroirs) ); //etirement_visu_cam) );
     barriere_avec_pixmiroirs.Ajouter_fils( (alx_noeud_scene*)(&noeud_translucidite) ); //etirement_visu_cam) );
       noeud_translucidite.Ajouter_fils( (alx_noeud_scene*)diapo );
     barriere_avec_pixmiroirs.Ajouter_fils( (alx_noeud_scene*) visu_cam);
   this->Ajouter_fils( (alx_noeud_scene*)(&barriere_sans_pixmiroirs) ); //etirement_visu_cam) );
     barriere_sans_pixmiroirs.Ajouter_fils( (alx_noeud_scene*)diapo );

 this->Etirement_du_contenu(320, 240);
 this->Mettre_a_jour();

// On fait les liens sur les évennements, notemment le redimensionnement :
 this->Voir_si_pt_dans_fils(false);
 this->Fenetre()->abonner_a_dimension( FONCTION_DE_RAPPEL( alx_visu_image_camnote_cpp_bigre
                                                         ,&alx_visu_image_camnote_cpp_bigre::Rationnaliser_fenetre_diapo) );
}

void alx_visu_image_camnote_cpp_bigre::Rationnaliser_fenetre_diapo(void *param)
{double tx = this->Fenetre()->Longueur_corp()
      , ty = this->Fenetre()->Hauteur_corp();

 this->Etirement_du_contenu(tx, ty);
}

void alx_visu_image_camnote_cpp_bigre::Charger_image(const alx_chaine_char &nom)
{//if(boost::filesystem::exists( nom.Texte() ))
   ((alx_image_opengl*)diapo)->maj( nom.Texte() );
 // else ((alx_image_opengl*)diapo)->maj( "Brectangle.bmp" );

 diapo->Img().Etirement(1,1);
}

void alx_visu_image_camnote_cpp_bigre::Activation_pix_mirroir(const bool b)
{diapo->Couleur_def(!b);
 barriere_avec_pixmiroirs.Bloquer(!b);
 barriere_sans_pixmiroirs.Bloquer(b);

 alx_visu_image_camnote_cpp::Activation_pix_mirroir(b);
}

void alx_visu_image_camnote_cpp_bigre::Intensite_pixels_miroirs(const float i)
{
 intensite_pixels_miroirs = (GLfloat)i;
 p_translucidite.couleur[3] = 1-intensite_pixels_miroirs;
 noeud_translucidite.Param1(&p_translucidite);
 alx_visu_image_camnote_cpp::Intensite_pixels_miroirs(i);
}



#include "principal.h"
#include "../interfaces/acces_interface.h"

#include <glew.h>
#include <gl.h>
#include <glu.h>

#include "..\physique\alx_model_simulation_physique1.h"
//#include "..\interfaces\classiques\alx_zone_texte_opengl.h"
#include "..\interfaces\interface.h"
#include "..\interfaces\fontes.h"
#include "..\Route 66\route66\includes\route66.h"

#include <il.h>
#include <time.h>

//#define ALX_DEBUG_PRINCIPAL 0

//using namespace cogitant;
//#include <windows.h>
//Tcl_Interp *interpreteur_tcl;

principal::~principal()
{
// delete sim_pointeurs;
// delete sim_souris;
	
// interpreteur_tcl = Tcl_CreateInterp();
}

principal::principal(const char *nom_exec, alx_interpreteur_tcl *environnement_tcl)
{angle_camera = 90;
 //sim_choc2D = new alx_simulateur_choc_2D;
 this->environnement_tcl = environnement_tcl;
}

principal::principal(const char *nom_exec)
{angle_camera = 90;
 //sim_choc2D = new alx_simulateur_choc_2D;
 this->environnement_tcl = (alx_interpreteur_tcl*)NULL;
 cc_nom_exec = nom_exec;
}

void principal::Initialisation()
{
 int error_code;
 alx_chaine_char IP_tmp = slimGetLocalIp();
 IP_tmp += "_";
 IP_tmp += (unsigned long int)time(NULL);

 IP_locale( IP_tmp );
 route66 = new Route66("224.0.0.80", 1500);
 error_code = route66->getLastError();
 if(error_code != 0)
   printf("route66 error nb: %d\n", error_code);
  else printf("Route 66 à bien démarrer sur:\n  IP   : %s\n  PORT : %s\n", route66->IP(), route66->PORT());

 // Interpréteur TCL
   if(!environnement_tcl)
     environnement_tcl = new Environnement_comet_olfa( cc_nom_exec.Texte() );
   Initialisation_commandes_tcl_interfaces( environnement_tcl );
 Enregistrer_alx_interpreteur_tcl(environnement_tcl, environnement_tcl, "interp_tcl");
 interface_1 = new alx_interface_sdl_opengl_1( ecran_x, ecran_y, angle_camera
                                             , route66
                                             , environnement_tcl );
 Interface_mere(interface_1);
}

int principal::demarrer( Uint32 propriete
                       , unsigned int tx
                       , unsigned int ty)
{ecran_x = tx;
 ecran_y = ty;

// DevIL
 ilInit();
 ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
 ilEnable(IL_ORIGIN_SET);

// SDL et OpenGL
 //putenv("SDL_VIDEODRIVER=windib");
 if( SDL_Init(SDL_INIT_VIDEO) < 0) return -1;
 SDL_ShowCursor(SDL_DISABLE);
 SDL_WM_GrabInput(SDL_GRAB_ON);
 //SDL_WM_GrabInput(SDL_GRAB_OFF);

// SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
// SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

 SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
 SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
 SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
 SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );
 SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 32 );
 SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8);
 SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
 if ( (ecran=SDL_SetVideoMode( ecran_x, ecran_y, 32
                             , SDL_OPENGL | SDL_GL_ACCELERATED_VISUAL | propriete /*| SDL_FULLSCREEN*/)) == NULL )
  return -2;

/*XXX DEBUG Still necessary?
 if(extgl_Initialize() != 0)
  {exit(0);}
*/
 SDL_WM_SetCaption("The future desktop for Model Driven Plasticity", "The future desktop for Model Driven Plasticity");

// initialiser les éléments
 Initialiser_fontes();
 Initialisation();

 // GLEW
 glewInit();

// Def de la caméra:
 glMatrixMode(GL_PROJECTION); // on rend active la matrice de projection
 glLoadIdentity(); // on reinitialise la matrice de projection
 gluPerspective(angle_camera,(double)(ecran_x)/(double)(ecran_y),0.0,10000.0);

 glMatrixMode(GL_MODELVIEW);

 ///*
 //glDepthFunc(GL_LEQUAL);
 //XXXglEnable(GL_TEXTURE_2D);						// Enable Texture Mapping
 glShadeModel(GL_SMOOTH);						// Enable Smooth Shading

 //HDC   hdc   = wglGetCurrentDC();
 //HGLRC hglrc = wglGetCurrentContext();

//------------------------------------------------------------------------------

// jeu_casse_brique = new alx_jeu_casse_bille( sim_choc2D
//                                           , ecran_x, ecran_y, angle_camera
//                                           , interface_1->Sim_pointeur()
//                                           , NULL //interface_1->Sim_couche()
//                                           , NULL ); //interface_1->Sim_points_contacts() );

//------------------------------------------------------------------------------

 bool fin = false;
 //int pointeur_x, pointeur_y;

 glClearStencil(0);
// Uint8 *etat_clavier;


 glEnable(GL_SCISSOR_TEST);
 glDisable(GL_STENCIL_TEST);
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_DEPTH_TEST);
 glEnable(GL_ALPHA_TEST);
 glAlphaFunc(GL_GREATER, 0);

 glEnable(GL_LINE_SMOOTH);    glHint(GL_LINE_SMOOTH_HINT   , GL_NICEST);
// glEnable(GL_POLYGON_SMOOTH); glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
//glHint(GL_PERSPECTIVE_CORRECTION, GL_NICEST);

//------------------------------------------------------------------------------
// Lecture musique:
 interface_1->Init_son();
 interface_1->Init_film();

// FSOUND_STREAM *musique = FSOUND_Stream_OpenFile( "02-Warrior of Ice.mp3"
//                                                , FSOUND_LOOP_NORMAL
//                                                , 0 );
 //FSOUND_Stream_Play(1, musique);
//----------------------------------------------------------------------------*/
//__________________________________________________

// L'éditeur TCL, de base
 interface_1->Arbre_scene().Nom_IU("PC Root");
 olfa_comet comet_essai(IP_locale().Texte(), environnement_tcl);
 alx_noeud_editeur_olfa_comet editeur_comet_essai(interface_1, &comet_essai);
 editeur_comet_essai.Nom_IU("TCL editor");
 editeur_comet_essai.Fenetre()->Nom_IU("TCL editor\nwindow");
 editeur_comet_essai.Editeur()->Nom_IU("TCL editor\ninput");
 interface_1->Arbre_scene().Ajouter_fils( (alx_noeud_scene*)(&editeur_comet_essai) );
 Enregistrer_noeud_bigre(environnement_tcl, &editeur_comet_essai, "noeud_editeur_tcl");
 Enregistrer_fenetre_bigre(environnement_tcl, editeur_comet_essai.Fenetre(), "fen_editeur_tcl");
 Enregistrer_zone_texte_bigre(environnement_tcl, editeur_comet_essai.Editeur(), "editeur_tcl");
//___________________________________________________
 //const char *rep = Tcl_PkgRequire(environnement_tcl->Interpreteur_tcl(), "Tcldot", "2", 0);
 //environnement_tcl->Evaluer( "source test_usb_mouse.tcl" );
 //printf("Nombre de souris : %i", SDL_GetNumMice());x

//___________________________________________________
 //#include <config_opengl.h>
 glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_ALPHA_ARB , GL_texture());

// interface_1->Translation( alx_vecteur2D(0, 200) );
// int nb_car_ecrit;

 bool bascule = true
    , enfonce = false;
 //HDC hdc = wglGetCurrentDC();
 //TCHAR text[ ] = "Defenestration can be hazardous";
// int nb_car;
 if(glClientActiveTextureARB == NULL) {
	 printf("\n!ERROR! glClientActiveTextureARB is NULL !\n");
	}
 while(!fin)
  {//etat_clavier = SDL_GetKeyState(NULL);
   #ifdef ALX_DEBUG_PRINCIPAL
     printf("R66..");
   #endif
   interface_1->Demande_acces();
     route66->Manage_callbacks();
   interface_1->Liberer_acces();
   #ifdef ALX_DEBUG_PRINCIPAL
     printf(".");
   #endif

   interface_1->Demande_acces();
     #ifdef ALX_DEBUG_PRINCIPAL
       printf("Simuler...");
     #endif
     interface_1->Simuler();
   interface_1->Liberer_acces();

   // Emettre les changements de l'interface sur les bus dédiés
   interface_1->Demande_acces(); //SDL_CreateMutex();
     #ifdef ALX_DEBUG_PRINCIPAL
       printf("Emettre_changements...");
     #endif
     interface_1->Emettre_changements( true );
   interface_1->Liberer_acces();

   interface_1->Demande_acces();
     #ifdef ALX_DEBUG_PRINCIPAL
       printf("PreAff...");
     #endif
     interface_1->Pre_afficher();
   interface_1->Liberer_acces();

   glClearColor(0.0, 0.0, 0.0, 0.0);
   glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   Initialiser_tempon();

   interface_1->Demande_acces();
     #ifdef ALX_DEBUG_PRINCIPAL
       printf("Aff...");
     #endif
     interface_1->Afficher();
   interface_1->Liberer_acces();

   glFlush();
   #ifdef ALX_DEBUG_PRINCIPAL
     printf("Swap...");
   #endif
   SDL_GL_SwapBuffers();
   //printf("END\n");
  }

 interface_1->Fin_son();
 SDL_Quit();

 return 0;
}


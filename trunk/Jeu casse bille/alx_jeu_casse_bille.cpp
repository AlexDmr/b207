#include "alx_jeu_casse_bille.h"

//extern alx_vecteur2D vect_h, vect_v;

/******************************************************************************/
/************************** Les constructeurs *********************************/
/******************************************************************************/
alx_jeu_casse_bille::alx_jeu_casse_bille( alx_simulateur_choc_2D *sc
                                        , int ex, int ey, double angle
                                        , alx_simulateur_pointeurs *sp
                                        , alx_simulateur_couche    *scouche
                                        , alx_simulateur_points_contact    *pc)
{
 ecran_x = ex;
 ecran_y = ey;
 angle_camera = angle;

 coin_bg.maj(0.0, 0.0);
 vect_h.maj( (double)ecran_x, 0.0);
 vect_v.maj(0.0, (double)ecran_y);
 trans.maj(0.0, 0.0);
 rotation = 0.0;

 if(sp == NULL)
  {
   sim_pointeurs  = new alx_simulateur_pointeurs();
   sim_ptr_souris = new alx_simulateur_pointeurs_souris( &ecran_x, &ecran_y
                                                       , &coin_bg
                                                       , &vect_h, &vect_v);
   id_souris = sim_ptr_souris->Ajouter_rapporteur(sim_pointeurs);                                                    
  }
  else
   {
    sim_pointeurs = sp;
   }

 if(pc == NULL) sim_contact = new alx_simulateur_points_contact();
  else sim_contact = pc;
 if(scouche == NULL) sim_couche  = new alx_simulateur_couche(sim_pointeurs);
  else sim_couche = scouche;
 liant_ptr_contact = new alx_liant_ptr_contact(sim_pointeurs, sim_contact);

 if(scouche == NULL) sim_couche->A_racine = &A_scene;
  else sim_couche->A_racine->Ajouter_fils( &A_scene );

 sim_choc = sc;

 Initialiser_jeu();
}

alx_jeu_casse_bille::~alx_jeu_casse_bille()
{

}

/******************************************************************************/
/************************ Simulation et affichage *****************************/
/******************************************************************************/
void alx_jeu_casse_bille::Simuler(double dt)
{
 if(sim_ptr_souris)
  {
   sim_ptr_souris->simuler();
   sim_pointeurs->simuler();
  }

 sim_init_newton.simuler();
 sim_champs.simuler();
 sim_cadre.simuler();
 sim_mouvement_newton.simuler(t_cour, dt);
 sim_choc->simuler();
 t_cour += dt;
}

GLfloat pos;
void alx_jeu_casse_bille::Afficher()
{

// Texture de fond:
 glMatrixMode( GL_PROJECTION );
 glLoadIdentity();
 gluPerspective(66.666, 800.0/600.0, 0.0, 10000.0);

 glMatrixMode( GL_MODELVIEW );
 glLoadIdentity();
/*
 glDisable(GL_LIGHTING);

 glEnable(GL_TEXTURE_2D);
 glEnable(GL_BLEND);
 glBindTexture(GL_TEXTURE_2D, num_texture_fond);
 glBegin(GL_QUADS);
   glColor4f(1.0f,1.0f,1.0f,1.0f);		// Full Brightness, 50% Alpha ( NEW )
   glNormal3f( 0.0f, 0.0f, 1.0f);
   glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -1.0, 0.0);
   glTexCoord2f(0.0, 4.6875); glVertex3f(-1.0,  1.0, 0.0);
   glTexCoord2f(6.25, 4.6875); glVertex3f( 1.0,  1.0, 0.0);
   glTexCoord2f(6.25, 0.0); glVertex3f( 1.0, -1.0, 0.0);
 glEnd();
 glDisable(GL_BLEND);
 glDisable(GL_TEXTURE_2D);
*/
//----------------------------- Lumières ---------------------------------------
        GLfloat specular  [] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess [] = { 100.0 };
	GLfloat position  [] = { -1.0, -1.0, -1.0, 0.0 };

        glDisable(GL_BLEND);
	// Set the shading model
	glShadeModel(GL_SMOOTH);

	// Enable depth testing for hidden line removal
	glDisable(GL_DEPTH_TEST);
        //iglDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
	// Define material properties of specular color and degree of
	// shininess.  Since this is only done once in this particular 
	// example, it applies to all objects.  Material properties can
	// be set for individual objects, individual faces of the objects,
	// individual vertices of the faces, etc... 
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

	// Set the GL_AMBIENT_AND_DIFFUSE color state variable to be the
	// one referred to by all following calls to glColor
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	// Create a Directional Light Source
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

        glMatrixMode(GL_MODELVIEW);
          glPushMatrix();
            glLoadIdentity();
            pos += 0.01;
            if(pos > 3.2 || pos < 0) pos = 0;
            glTranslated(-1.6 + pos, 1, -3.0);
            glColor4f(0, 1, 1, 1);

            glutSolidSphere(1, 80, 80);
            glTranslated(3.2-2*pos, 0, 0);
            glColor3f(1, 0, 0);
            glutWireSphere(1, 80, 80);
          glPopMatrix();
//        glDisable(GL_LIGHTING);

//------------------------------------------------------------------------------



// La scène
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity(); // on reinitialise la matrice de projection
 gluPerspective(angle_camera,(double)(ecran_x)/(double)(ecran_y),0.0,10000.0);
 camera_pour_ecran(coin_bg, vect_h, vect_v.norme(), angle_camera, (double)(ecran_y)/(double)(ecran_x));
   glMatrixMode( GL_MODELVIEW );
   A_scene.e->Afficher();
 glPopMatrix(); // Pour le camera_pour_ecran.

 glDisable(GL_LIGHTING); //Et on éteind la lumière
 glDisable(GL_DEPTH_TEST);
 glDisable(GL_CULL_FACE);
 glDisable(GL_BLEND);
}

/******************************************************************************/
/******************** modifications de l'interface ****************************/
/******************************************************************************/
void alx_jeu_casse_bille::Translation(const alx_vecteur2D &t)
{
 trans.Ajouter(t);
 coin_bg.maj(trans.dx, trans.dy);
}

/******************************************************************************/
void alx_jeu_casse_bille::Rotation(const double r)
{
 rotation += r;
 vect_h.maj((double)ecran_x, 0.0);
 vect_v.maj(0.0, (double)ecran_y);

 vect_h.Pivoter( rotation );
 vect_v.Pivoter( rotation );
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------- Les fonctions du jeu -------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void alx_jeu_casse_bille::Initialiser_jeu()
{
// initialisation de la texture:
 texture_fond = new alx_image_32(/*"texture_1.bmp"*/"Texture_multicolor_009.bmp");
// img->fixer_alpha("texture_1.bmp", 50, 2);


 //glDepthFunc(GL_LEQUAL);
 glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
 glShadeModel(GL_SMOOTH);							// Enable Smooth Shading

 glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
 glEnable(GL_BLEND);

 glGenTextures(1, &num_texture_fond);
 glBindTexture(GL_TEXTURE_2D, num_texture_fond);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

 glTexImage2D( GL_TEXTURE_2D
             , 0                 // niveau de résolution (pour les mipmap)
             , GL_RGBA           // Le format interne, i.e. ce que l'on souhaite utiliser.
             , texture_fond->L(), texture_fond->H()
             , 0                 // La largeur du bord
             , GL_RGBA           // Le format externe de l'image
             , GL_UNSIGNED_BYTE  // Le type des données
             , texture_fond->TEMPON() );
 
 glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
 glDisable(GL_BLEND);

// Initialisation de la physique
 sim_cadre.est   = 0.0;
 sim_cadre.ouest = (double)ecran_x;
 sim_cadre.nord  = 0.0;
 sim_cadre.sud   = (double)ecran_y;

 sim_champs.champs.maj(0.0, -10.0);

 tapis = new alx_noeud_tapis((double)ecran_x, (double)ecran_y);
 A_scene.Ajouter_fils( (alx_noeud_scene*)tapis );
 tapis->abonner_a(this, 1);
}

void alx_jeu_casse_bille::Lacher_bille(alx_point2D &pt)
{
 alx_noeud_bille *nouvelle_bille = new alx_noeud_bille( 30.0, 1.0, 1.0
                                                      , pt, pt
                                                      , 0.0, 0.0
                                                      , 0.0, 1.0, 0.0, 1.0 );
 tapis->Ajouter_fils( (alx_noeud_scene*)nouvelle_bille );

 alx_model_simulation_physique1 *mp = &(nouvelle_bille->palet->model_classique_et_choc);

 sim_init_newton.inscrire     ( mp );
 sim_champs.inscrire          ( mp );
 sim_mouvement_newton.inscrire( mp );
 sim_cadre.inscrire           ( &(nouvelle_bille->palet->model_boite_mouvement) );
 sim_choc->inscrire           ( new models_utils_choc_2D( &(nouvelle_bille->palet->model_boite_mouvement)
                                                        , mp) );
}

void alx_jeu_casse_bille::rationnaliser(int num)
{
 switch(num)
  {
   case 1: // un message du tapis
    {
     int *infos = tapis->infos_comp;
     if( (tapis->nb_infos_comp == 3)
       &&(infos[0] == 0)
       &&(infos[1] == 0) )
      { // Contact avec un point
       Lacher_bille( *( (alx_point2D*)(void*)(infos[2]) ) );
      }
    }
   break;
  }
}



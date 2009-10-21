#include "alx_noeud_visualisation_camera.h"
#include "..\..\opengl\config_opengl.h"
#include "..\..\physique\math_alex.cpp"

#include "../../opengl/alx_sommet_opengl.h"

//---------------------------------------------------------------------------------------------------------------------------------
//--------------------------------- Les constructeurs -----------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
int fonction_de_rappel(GMLGrabber *g, void * params)
{((alx_noeud_visualisation_camera*)params)->Nouvelle_image(true);
 return 0;
}

void alx_noeud_visualisation_camera::init()
{acquisiteur = gmlCreateGrabber(0, 1);
 gmlStartGrabber(acquisiteur);
 gmlSetCallback(acquisiteur, fonction_de_rappel, this);
 nouvelle_image = true;
 //couleur_libre = false;
 Nb_octets_par_pixel( gmlGetDepth(acquisiteur) >> 3 );
 if(Nb_octets_par_pixel() == 4)
  {Ordonnancement_couleurs( GL_bvra() );
  } else {Ordonnancement_couleurs( GL_bvr() );}
 Ordre_couleur_texture       ( GL_bvr() );
 Nb_octets_par_pixels_texture(3);
}

alx_noeud_visualisation_camera::alx_noeud_visualisation_camera() : alx_noeud_image_sdl_opengl()
{init();

 alx_noeud_image_sdl_opengl::maj_raw( gmlGetWidth (acquisiteur)
                                    , gmlGetHeight(acquisiteur)
                                    , Ordonnancement_couleurs()
                                    , Nb_octets_par_pixel()
                                    , NULL);
 nouvelle_image = true;
 PreRendre();
}

alx_noeud_visualisation_camera::~alx_noeud_visualisation_camera()
{gmlStopGrabber   (acquisiteur);
 gmlDestroyGrabber(acquisiteur);
};

//---------------------------------------------------------------------------------------------------------------------------------
//------------------------------------ Les méthodes -------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
void alx_noeud_visualisation_camera::PreRendre()
{if(nouvelle_image)
  {char *tempon = (char*)gmlLockFrame(acquisiteur);
     alx_noeud_image_sdl_opengl::maj_raw( Lg()
                                        , Ht()
                                        , Ordonnancement_couleurs()
                                        , Nb_octets_par_pixel()
                                        , tempon );
   gmlUnlockFrame(acquisiteur);
  }
 nouvelle_image = false;
}



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
 threaded_mode = false;
}

//---------------------------------------------------------------------------------------------------------------------------------
alx_noeud_visualisation_camera::alx_noeud_visualisation_camera() : alx_noeud_image_sdl_opengl()
{init();

 Resize_camera_image_buffer();

 nouvelle_image = true;
 PreRendre();
}

//---------------------------------------------------------------------------------------------------------------------------------
alx_noeud_visualisation_camera::~alx_noeud_visualisation_camera()
{gmlStopGrabber   (acquisiteur);
 gmlDestroyGrabber(acquisiteur);
};

//---------------------------------------------------------------------------------------------------------------------------------
void alx_noeud_visualisation_camera::Resize_camera_image_buffer()
{alx_noeud_image_sdl_opengl::maj_raw( gmlGetWidth (acquisiteur)
                                    , gmlGetHeight(acquisiteur)
                                    , Ordonnancement_couleurs()
                                    , Nb_octets_par_pixel()
                                    , NULL);
}

//---------------------------------------------------------------------------------------------------------------------------------
//------------------------------------ Les méthodes -------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
void alx_noeud_visualisation_camera::PreRendre()
{if(nouvelle_image)
  {char *tempon = (char*)gmlLockFrame(acquisiteur);
  if(threaded_mode) {
    alx_noeud_image_sdl_opengl::Threaded_maj( (int)Lg()
                                                    , (int)Ht()
                                                    , Ordonnancement_couleurs()
                                                    , gmlGetDepth(acquisiteur) >> 3
                                                    , Ordre_couleur_texture()
                                                    , Nb_octets_par_pixels_texture()
                                                    , tempon );
  } else {
          alx_noeud_image_sdl_opengl::maj_raw_with_transfo( (int)Lg()
                                                          , (int)Ht()
                                                          , Ordonnancement_couleurs()
                                                          , gmlGetDepth(acquisiteur) >> 3
                                                          , Ordre_couleur_texture()
                                                          , Nb_octets_par_pixels_texture()
                                                          , tempon );
         }
/*     alx_noeud_image_sdl_opengl::maj_raw( Lg()
                                        , Ht()
                                        , Ordonnancement_couleurs()
                                        , Nb_octets_par_pixel()
                                        , tempon );*/
   gmlUnlockFrame(acquisiteur);
  }
 nouvelle_image = false;

 alx_noeud_image_sdl_opengl::PreRendre();
}



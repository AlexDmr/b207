#include "alx_noeud_visualisation_camera.h"
#include "..\..\opengl\config_opengl.h"
#include "..\..\physique\math_alex.cpp"

#include "../../opengl/alx_sommet_opengl.h"


//---------------------------------------------------------------------------------------------------------------------------------
//--------------------------------- Les constructeurs -----------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
/* OLDint fonction_de_rappel(GMLGrabber *g, void * params)
{((alx_noeud_visualisation_camera*)params)->Nouvelle_image(true);
 return 0;
}


void alx_noeud_visualisation_camera::init(const int device_index)
{acquisiteur = gmlCreateGrabber(device_index, 1);
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
*/


void alx_noeud_visualisation_camera::init(const int device_index) {
	this->has_been_init = false;
	this->device_index = device_index;
	VI.setupDevice(device_index, 640, 480, VI_COMPOSITE);
	//VI.setUseCallback(true);
	nouvelle_image = false;
	Ordonnancement_couleurs( GL_bvr() ); Nb_octets_par_pixel(3);
	Ordre_couleur_texture  ( GL_bvr() ); Nb_octets_par_pixels_texture(3);
	threaded_mode = false;
}

//---------------------------------------------------------------------------------------------------------------------------------
alx_noeud_visualisation_camera::alx_noeud_visualisation_camera(const int device_index) : alx_noeud_image_sdl_opengl()
{init(device_index);

// Resize_camera_image_buffer();

 nouvelle_image = false;
 //PreRendre();
}

//---------------------------------------------------------------------------------------------------------------------------------
alx_noeud_visualisation_camera::~alx_noeud_visualisation_camera()
{/*OLD gmlStopGrabber   (acquisiteur);
 gmlDestroyGrabber(acquisiteur);*/
	VI.stopDevice(device_index);
};

//---------------------------------------------------------------------------------------------------------------------------------
void alx_noeud_visualisation_camera::Resize_camera_image_buffer()
{alx_noeud_image_sdl_opengl::maj_raw( VI.getWidth (device_index)
                                    , VI.getHeight(device_index)
                                    , Ordonnancement_couleurs()
                                    , Nb_octets_par_pixel()
                                    , NULL);
}

//---------------------------------------------------------------------------------------------------------------------------------
//------------------------------------ Les méthodes -------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
void alx_noeud_visualisation_camera::PreRendre()
{if(VI.isFrameNew(device_index) /* VI.isReadyToCapture(device_index)*/ && VI.isDeviceSetup(device_index))
  {if( !this->has_been_init ) {printf("Camera resized to %dx%d ...", VI.getWidth(device_index), VI.getHeight(device_index)); Resize_camera_image_buffer(); printf("done\n"); this->has_been_init = true; }
   if(threaded_mode) {
    alx_noeud_image_sdl_opengl::Threaded_maj( (int)Lg()
                                            , (int)Ht()
                                            , Ordonnancement_couleurs()
                                            , 3
                                            , Ordre_couleur_texture()
                                            , Nb_octets_par_pixels_texture() 
                                            , (char*)VI.getPixels(device_index, false, false) );
  } else {
          alx_noeud_image_sdl_opengl::maj_raw_with_transfo( (int)Lg()
                                                          , (int)Ht()
                                                          , Ordonnancement_couleurs()
                                                          , 3
                                                          , Ordre_couleur_texture()
                                                          , Nb_octets_par_pixels_texture()
                                                          , VI.getPixels(device_index, false, false) );
         }
  }
 nouvelle_image = false;

 alx_noeud_image_sdl_opengl::PreRendre();
}
/* OLD
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
   gmlUnlockFrame(acquisiteur);
  }
 nouvelle_image = false;

 alx_noeud_image_sdl_opengl::PreRendre();
}
*/

//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
videoInput alx_noeud_visualisation_camera::VI;

//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
#include <tcl.h>
Tcl_DString tcl_res;
const char* Lister_cameras() {
		Tcl_DStringInit(&tcl_res);
		Tcl_DStringAppend(&tcl_res, "{", 1);
	for (int wIndex = 0; wIndex < alx_noeud_visualisation_camera::VI.devicesFound; wIndex++) 
	{
		 char tmp[1024];
		 char *name = alx_noeud_visualisation_camera::VI.getDeviceName(wIndex);
		 if (name) {
			 _itoa_s(wIndex, tmp, 10)						 ; Tcl_DStringAppendElement(&tcl_res, tmp);	
			 Tcl_DStringAppendElement(&tcl_res, name);
			}
		}

	return Tcl_DStringAppend(&tcl_res, "}", 1);
}
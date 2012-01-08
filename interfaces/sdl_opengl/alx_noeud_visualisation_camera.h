#ifndef __ALX_NOEUD_ACQUISITION_CAMERA_H__
#define __ALX_NOEUD_ACQUISITION_CAMERA_H__

//#include "../classiques/alx_fiche_opengl.h"
//#include "../../physique/
//#include "../../utilitaires/alx_arbre.h"
//#include "../../physique/alx_simulateur_points_contact.h"
//#include "../opengl/alx_texture_rendu.h"
//#include "alx_prerendeur.h"

#include "alx_noeud_image_sdl_opengl.h"
#include "../../opengl/config_opengl.h"
//OLD #include "../../../GMLGrabber1.0/include/GMLGrabber.h"
#include "videoInput.h"

class alx_polygone_opengl;

class alx_noeud_visualisation_camera : public alx_noeud_image_sdl_opengl
{
 private:
   bool nouvelle_image, threaded_mode/*, couleur_libre*/;
//   alx_model_simulation_physique1 mp;
//   alx_polygone_opengl            *polygone_opengl;
   //OLD GMLGrabber                     *acquisiteur;
   int device_index;
   bool has_been_init;

//   int ordonnancement_couleurs_camera;
//   unsigned int nb_octets_par_pixel;
//   INFOS_TEXTURE *info_texture;

   void init(const int device_index);

 protected:
   void Resize_camera_image_buffer();

 public :
   static videoInput VI;

  // Les constructeurs
   alx_noeud_visualisation_camera(const int device_index = 0);
  ~alx_noeud_visualisation_camera();

   virtual const char* Real_class_cmd() {return "Void_vers_camera";}

  // Les méthodes
//   inline const unsigned int Nb_octets_par_pixel()  const {return nb_octets_par_pixel;}
//   inline const int Ordonnancement_couleurs_camera()       const {return ordonnancement_couleurs;}
//   inline void Ordonnancement_couleurs_camera(const int o)       {ordonnancement_couleurs = o;}
//   inline const bool Couleur_libre()                const {return couleur_libre;}
//   inline void       Couleur_libre(const bool b)          {couleur_libre = b;}
//   inline const INFOS_TEXTURE* Info_texture()       const {return info_texture;}
   inline bool Nouvelle_image()                     const {return nouvelle_image;}
   inline void Nouvelle_image(const bool b)               {nouvelle_image = b;}

   inline const bool EstPret() const {return has_been_init;}

   inline bool Threaded_mode()                     const {return threaded_mode;}
   inline void Threaded_mode(const bool b)               {threaded_mode = b;}

   inline void Stop () {VI.stopDevice(device_index);}
   inline void Start() {VI.restartDevice(device_index);}

   inline const int get_resolution_x() const {return VI.getWidth (device_index);}
   inline const int get_resolution_y() const {return VI.getHeight(device_index);}
   inline void set_resolution(const int x, const int y) {
     Stop();
     VI.setupDevice(device_index, x, y, VI_COMPOSITE);
     Resize_camera_image_buffer();
     Start();
    }

   inline const char* get_description() const {return VI.getDeviceName(device_index);}
   inline const int   Display_DialogFormat() {VI.showSettingsWindow(device_index);
											  return 1;
                                             }

   inline const int Frequence() const {return 0;}
   inline int  Frequence(const int f) {VI.setIdealFramerate(device_index, f); return f;}
//   void Afficher();
//   info_du_contenant* Contient(alx_point2D &pt, int action); // Renvoi un pointeur sur le modèle physique qui contient la primitive, NULL si aucun.
   void PreRendre();
};

typedef alx_noeud_visualisation_camera* P_alx_noeud_visualisation_camera;
inline alx_noeud_visualisation_camera* Void_vers_camera(void *p) {return (alx_noeud_visualisation_camera*)p;}

const char* Lister_cameras();

#endif

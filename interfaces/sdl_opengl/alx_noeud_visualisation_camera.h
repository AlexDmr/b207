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
#include "../../../GMLGrabber1.0/include/GMLGrabber.h"

class alx_polygone_opengl;

class alx_noeud_visualisation_camera : public alx_noeud_image_sdl_opengl
{
 private:
   bool nouvelle_image, threaded_mode/*, couleur_libre*/;
//   alx_model_simulation_physique1 mp;
//   alx_polygone_opengl            *polygone_opengl;
   GMLGrabber                     *acquisiteur;
//   int ordonnancement_couleurs_camera;
//   unsigned int nb_octets_par_pixel;
//   INFOS_TEXTURE *info_texture;

   void init();

 protected:
   void Resize_camera_image_buffer();

 public :
  // Les constructeurs
   alx_noeud_visualisation_camera();
  ~alx_noeud_visualisation_camera();

   virtual const char* Real_class_cmd() {return "Void_vers_camera";}

  // Les m�thodes
//   inline const unsigned int Nb_octets_par_pixel()  const {return nb_octets_par_pixel;}
//   inline const int Ordonnancement_couleurs_camera()       const {return ordonnancement_couleurs;}
//   inline void Ordonnancement_couleurs_camera(const int o)       {ordonnancement_couleurs = o;}
//   inline const bool Couleur_libre()                const {return couleur_libre;}
//   inline void       Couleur_libre(const bool b)          {couleur_libre = b;}
//   inline const INFOS_TEXTURE* Info_texture()       const {return info_texture;}
   inline bool Nouvelle_image()                     const {return nouvelle_image;}
   inline void Nouvelle_image(const bool b)               {nouvelle_image = b;}

   inline bool Threaded_mode()                     const {return threaded_mode;}
   inline void Threaded_mode(const bool b)               {threaded_mode = b;}

   inline void Stop () {gmlStopGrabber (acquisiteur);}
   inline void Start() {gmlStartGrabber(acquisiteur);}

   inline const int get_resolution_x() const {return gmlGetWidth (acquisiteur);}
   inline const int get_resolution_y() const {return gmlGetHeight(acquisiteur);}
   inline void set_resolution(const int x, const int y) {
     Stop();
     gmlSetResolution(acquisiteur, x, y);
     Resize_camera_image_buffer();
     Start();
    }

   inline const char* get_description() const {return gmlGetDescription(acquisiteur);}
   inline const int   Display_DialogFormat() {int rep = gmlDisplayDialogFormat(acquisiteur);
                                              if(rep == 2) {
                                                 Resize_camera_image_buffer();
                                                }
                                              return rep;
                                             }

   inline const int Frequence() const {return gmlGetFrameRate(acquisiteur);}
   inline int  Frequence(const int f) {if(acquisiteur)
                                         return gmlSetFrameRate(acquisiteur, f);
                                        else return 0;}
//   void Afficher();
//   info_du_contenant* Contient(alx_point2D &pt, int action); // Renvoi un pointeur sur le mod�le physique qui contient la primitive, NULL si aucun.
   void PreRendre();
};

typedef alx_noeud_visualisation_camera* P_alx_noeud_visualisation_camera;
inline alx_noeud_visualisation_camera* Void_vers_camera(void *p) {return (alx_noeud_visualisation_camera*)p;}

#endif
#ifndef __ALX_NOEUD_ZONE_TEXTE_INTERPRETEUR_TCL_SDL_OPENGL_H__
#define __ALX_NOEUD_ZONE_TEXTE_INTERPRETEUR_TCL_SDL_OPENGL_H__

#include "alx_noeud_zone_texte_sdl_opengl.h"

class alx_interpreteur_tcl;

class alx_noeud_zone_texte_interpreteur_tcl_sdl_opengl : public alx_noeud_zone_texte_sdl_opengl
{
 private:
   alx_interpreteur_tcl *interpreteur_tcl;
   Liste_de_rappel       L_rappel_car_tape;

 public :
   alx_noeud_zone_texte_interpreteur_tcl_sdl_opengl( alx_zone_texte_opengl *zgl
                                                   , alx_interpreteur_tcl *interpreteur );

   virtual const char* Real_class_cmd() {return "Void_vers_zone_txt_interp_tcl";}
   //void Afficher ();
  // Renvoi un pointeur sur le modèle physique qui contient la primitive, NULL si aucun.
   //info_du_contenant* Contient(alx_point2D &point, int action);
   void Caractere_tape(void *param);

   const alx_interpreteur_tcl* Interpreteur_tcl() const {return interpreteur_tcl;}

  inline void abonner_a_caractere_tape(alx_methode_rappel *m)
   {L_rappel_car_tape.Ajouter_methode_a_la_fin(m);}
  inline void Informer_que_caractere_tape(int c)
   {L_rappel_car_tape.Rappeler(&c);}
};
typedef alx_noeud_zone_texte_interpreteur_tcl_sdl_opengl* P_alx_noeud_zone_texte_interpreteur_tcl_sdl_opengl;
alx_noeud_zone_texte_interpreteur_tcl_sdl_opengl* Void_vers_zone_txt_interp_tcl(void *p) {return (alx_noeud_zone_texte_interpreteur_tcl_sdl_opengl*)p;}

#endif



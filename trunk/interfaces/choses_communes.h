#ifndef __ALX_CHOSES_COMMUNES_H__
#define __ALX_CHOSES_COMMUNES_H__

#include "..\opengl\alx_materiel.h"  // 2,56s
#include "..\opengl\alx_image_opengl.h" // 1,31s
#include "fontes.h"                     // 0,36s
#include <sdl.h>

void Initialiser_boutons();

alx_image_opengl* Image_bouton_dec();
alx_image_opengl* Image_bouton_inc();
alx_image_opengl* Image_bouton_rect();

alx_materiel* Matos();
void Initialiser_matos();

void Initialiser_boutons();
alx_image_opengl* Image_bouton_dec();
alx_image_opengl* Image_bouton_inc();
alx_image_opengl* Image_bouton_rect();

class alx_prerendeur;
alx_prerendeur* Prerendeur_defaut();

inline int  Void_vers_int (void *p) {return *(int *)p;}
inline char Void_vers_char(void *p) {return *(char*)p;}


inline int SDSK_UP        () {return SDLK_UP;}
inline int SDSK_DOWN      () {return SDLK_DOWN;}
inline int SDSK_LEFT      () {return SDLK_LEFT;}
inline int SDSK_RIGHT     () {return SDLK_RIGHT;}
inline int SDSK_BACKSPACE () {return SDLK_BACKSPACE;}
inline int SDSK_TAB       () {return SDLK_TAB;}
inline int SDSK_CLEAR     () {return SDLK_CLEAR;}
inline int SDSK_RETURN    () {return SDLK_RETURN;}
inline int SDSK_PAUSE     () {return SDLK_PAUSE;}
inline int SDSK_ESCAPE    () {return SDLK_ESCAPE;}
inline int SDSK_HOME      () {return SDLK_HOME;}
inline int SDSK_END       () {return SDLK_END;}
inline int SDSK_PAGEUP    () {return SDLK_PAGEUP;}
inline int SDSK_PAGEDOWN  () {return SDLK_PAGEDOWN;}

#endif

#ifndef __ALX_NOEUD_ENTETE_H__
#define __ALX_NOEUD_ENTETE_H__

#include "..\alx_noeud_scene.h"
#include "..\classiques\alx_fenetre_opengl.h"

class alx_noeud_entete_sdl_opengl : public alx_noeud_scene
{
 private:
   alx_fenetre_opengl *fenetre_gl;
   alx_fenetre        *fenetre;
   double couleur[4];

 public :
   void Maj_fenetre(alx_fenetre_opengl *f)
    {fenetre_gl = f;
     if(fenetre_gl)
       fenetre = f->Fenetre();
      else fenetre = (alx_fenetre*)NULL;}

   alx_noeud_entete_sdl_opengl(alx_fenetre_opengl *f) : alx_noeud_scene()
    {Maj_fenetre(f);
     Couleur(1,1,1,1); }

   virtual const char* Void_vers_editeur_olfa_comet() {return "Void_vers_entete";}

   inline void Couleur(const double r, const double v, const double b, const double a)
    {couleur[0] = r;
     couleur[1] = v;
     couleur[2] = b;
     couleur[3] = a; }

   void Afficher();
};

typedef alx_noeud_entete_sdl_opengl* P_alx_noeud_entete_sdl_opengl;
inline alx_noeud_entete_sdl_opengl* Void_vers_entete(void *p) {return (alx_noeud_entete_sdl_opengl*)p;}

#endif

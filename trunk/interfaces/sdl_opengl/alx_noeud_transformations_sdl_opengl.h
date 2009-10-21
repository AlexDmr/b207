#ifndef __ALX_NOEUD_TRANSFORMATION_SDL_OPENGL_H__
#define __ALX_NOEUD_TRANSFORMATION_SDL_OPENGL_H__

#include "..\alx_noeud_scene.h"
#include "..\..\opengl\alx_sommet_opengl.h"
#include "..\..\opengl\config_opengl.h"

template <void (*f1)(void *p1), void (*f2)(void *p2)>
class alx_noeud_transformations_sdl_opengl : public alx_noeud_scene
{private:
   void *param1, *param2;

 public :
   alx_noeud_transformations_sdl_opengl() {param1 = param2 = NULL;};

   inline void  Param1(void *p) {param1 = p;}
   inline void  Param2(void *p) {param2 = p;}
   inline void* Param1()        {return param1;}
   inline void* Param2()        {return param2;}

   inline void Afficher()
    {glPushMatrix();
       f1(param1);
       Afficher_fils(0);
       f2(param2);
     glPopMatrix();
    }
};

class param_translucidite
{public:
   double couleur[4];
   GLint param; // GL_MODULATE, GL_DECAL, ou GL_BLEND.
   GLenum facteur_s  // source      // GL_ZERO, GL_ONE, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA
        , facteur_d; // destination// GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA, and GL_SRC_ALPHA_SATURATE

   param_translucidite()
    {couleur[0] = couleur[1] = couleur[2] = 1;
     couleur[3] = 1;
     param = GL_MODULATE;
     facteur_s = GL_SRC_ALPHA;
     facteur_d = GL_ONE_MINUS_SRC_ALPHA;}
};
typedef param_translucidite* P_param_translucidite;

inline void translucidite(void *param)
{glPushAttrib(GL_ENABLE_BIT);
   glEnable(GL_BLEND);
   param_translucidite *p = (param_translucidite*)param;
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, p->param);
    glBlendFunc(p->facteur_s, p->facteur_d );
    glColor4dv(p->couleur); //[0], p->couleur[1], p->couleur[2], p->couleur[3]);
    //glColor4f(1,1,1,1);
}

inline void depile_bit_de_controle(void *p)
{glPopAttrib();}

class alx_noeud_Barriere_booleenne : public alx_noeud_scene
{private:
   bool bloquer;
 public:
   alx_noeud_Barriere_booleenne() : alx_noeud_scene() {bloquer=false;}

   inline void Bloquer(const bool b) {bloquer = b;}
   inline bool Bloquer() const       {return bloquer;}

   inline void Afficher()
    {if(!bloquer) Afficher_fils(0);}

   info_du_contenant* Noeud_contient(const alx_point2D &pt, int action)
    {if(!bloquer)
       return alx_noeud_scene::Contenu_dans_fils(pt, action);
     return NULL;}
};
typedef alx_noeud_Barriere_booleenne* P_alx_noeud_Barriere_booleenne;

class alx_noeud_couleur_sdl_opengl : public alx_noeud_scene
{private:
   double t[4];

 public :
   alx_noeud_couleur_sdl_opengl() : alx_noeud_scene() {}

   inline const double* Coord() const {return t;}
   inline void Coord(const double coord[4]) {Copie_couleur(coord, t);}
   inline void Coord( const double r
                    , const double v
                    , const double b
                    , const double a ) {t[0]=r; t[1]=v; t[2]=b; t[3]=a;}

   inline void Afficher()
    {glPushAttrib(GL_CURRENT_BIT);
       glColor4dv(t);
       Afficher_fils(0);
     glPopAttrib();}
  // Renvoi un pointeur sur le modèle physique qui contient la primitive, NULL si aucun.
   //info_du_contenant* Contient(alx_point2D &pt, int action);
};
typedef alx_noeud_couleur_sdl_opengl* P_alx_noeud_couleur_sdl_opengl;

//______________________________________________________________________________
/*
class alx_noeud_repere2D : public alx_noeud_scene
{public:
   alx_noeud_repere2D() : alx_noeud_scene() {}
   alx_point2D pt2;

   inline void Afficher()
    {alx_noeud_scene::Afficher();}

   info_du_contenant* Contient(alx_point2D &pt, int action)
    {return alx_noeud_scene::Contient(pt, action);}
};
typedef alx_noeud_repere2D* P_alx_noeud_repere2D;
*/
#endif


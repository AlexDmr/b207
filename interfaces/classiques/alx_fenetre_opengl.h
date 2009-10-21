#ifndef __ALX_FENETRE_OPENGL_H__
#define __ALX_FENETRE_OPENGL_H__

#include "alx_fenetre.h"
#include "..\..\opengl\alx_sommet_opengl.h"
#include "..\..\opengl\alx_materiel.h"


class gl_fonte;
//#include "..\..\opengl\alx_image.h"

//#include "..\opengl\alx_texture_rendu.h"
#include "..\sdl_opengl\alx_noeud_zone_rendu_sdl_opengl.h"

class alx_fenetre_opengl : public alx_classe_base_liant
{
 private:
   alx_fenetre *fenetre;
   gl_fonte *fonte_entete;
   alx_noeud_zone_rendu_sdl_opengl *zdr_entete;

   double couleur_corps[4]
         , couleur_bordure_ext_angle[4]
         , couleur_bordure_ext[4]
         , couleur_bordure_int_angle[4]
         , couleur_bordure_int[4]
         , couleur_entete[4][4] // pour les quatres coins
         , couleur_masque_corp[4]
         , couleur_masque_bord[4];

 public :
   alx_fenetre_opengl(alx_fenetre *f, gl_fonte *fonte);
   alx_fenetre* Fenetre() {return fenetre;}

   inline const double* Couleur_corps()             const {return couleur_corps;}
   inline const double* Couleur_bordure_ext_angle() const {return couleur_bordure_ext_angle;}
   inline const double* Couleur_bordure_ext()       const {return couleur_bordure_ext;}
   inline const double* Couleur_bordure_int_angle() const {return couleur_bordure_int_angle;}
   inline const double* Couleur_bordure_int()       const {return couleur_bordure_int;}
   inline const double* Couleur_entete(const int n) const {return couleur_entete[n];}
   inline const double* Couleur_masque_corp()       const {return couleur_masque_corp;}
   inline const double* Couleur_masque_bord()       const {return couleur_masque_bord;}


   inline void Zdr_entete(alx_noeud_zone_rendu_sdl_opengl *z) {zdr_entete = z;}
   inline alx_noeud_zone_rendu_sdl_opengl* Zdr_entete() {return zdr_entete;}
   //___________________________________________________________________________
   inline void Fonte_entete(gl_fonte *f) {fonte_entete = f;}
   inline gl_fonte* Fonte_entete() {return fonte_entete;}
   //___________________________________________________________________________
   inline void Couleur_corps(const double r, const double v, const double b, const double a)
    {couleur_corps[0]=r; couleur_corps[1]=v; couleur_corps[2]=b; couleur_corps[3]=a;}
   //___________________________________________________________________________
   inline void Couleur_bordure_ext_angle(const double r, const double v, const double b, const double a)
    {couleur_bordure_ext_angle[0]=r; couleur_bordure_ext_angle[1]=v; couleur_bordure_ext_angle[2]=b; couleur_bordure_ext_angle[3]=a;}
   //___________________________________________________________________________
   inline void Couleur_bordure_ext(const double r, const double v, const double b, const double a)
    {couleur_bordure_ext[0]=r; couleur_bordure_ext[1]=v; couleur_bordure_ext[2]=b; couleur_bordure_ext[3]=a;}
   //___________________________________________________________________________
   inline void Couleur_bordure_int_angle(const double r, const double v, const double b, const double a)
    {couleur_bordure_int_angle[0]=r; couleur_bordure_int_angle[1]=v; couleur_bordure_int_angle[2]=b; couleur_bordure_int_angle[3]=a;}
   //___________________________________________________________________________
   inline void Couleur_bordure_int(const double r, const double v, const double b, const double a)
    {couleur_bordure_int[0]=r; couleur_bordure_int[1]=v; couleur_bordure_int[2]=b; couleur_bordure_int[3]=a;}
   //___________________________________________________________________________
   inline void Couleur_entete(const int num_coin, const double r, const double v, const double b, const double a)
    {couleur_entete[num_coin][0]=r; couleur_entete[num_coin][1]=v; couleur_entete[num_coin][2]=b; couleur_entete[num_coin][3]=a;}
   //___________________________________________________________________________
   inline void Couleur_masque_corp(const double r, const double v, const double b, const double a)
    {couleur_masque_corp[0]=r; couleur_masque_corp[1]=v; couleur_masque_corp[2]=b; couleur_masque_corp[3]=a;}
   //___________________________________________________________________________
   inline void Couleur_masque_bord(const double r, const double v, const double b, const double a)
    {couleur_masque_bord[0]=r; couleur_masque_bord[1]=v; couleur_masque_bord[2]=b; couleur_masque_bord[3]=a;}
   //___________________________________________________________________________

   virtual inline void afficher()      {};
   virtual inline void Transfo_commune() {};
   virtual inline void Fin_Transfo_commune() {};
   virtual inline void afficher_fond() {};
   virtual inline void afficher_entete() {};
   virtual inline void afficher_zone_rot() {};
   virtual inline void afficher_bordure() {};

   virtual inline void Mettre_a_jour() {};

   //___________________________________________________________________________
   inline double* P_couleur_corps(const unsigned int i)                        {return &(couleur_corps[i]);}
   inline double* P_couleur_bordure_ext_angle(const unsigned int i)            {return &(couleur_bordure_ext_angle[i]);}
   inline double* P_couleur_bordure_ext(const unsigned int i)                  {return &(couleur_bordure_ext[i]);}
   inline double* P_couleur_bordure_int_angle(const unsigned int i)            {return &(couleur_bordure_int_angle[i]);}
   inline double* P_couleur_bordure_int(const unsigned int i)                  {return &(couleur_bordure_int[i]);}
   inline double* P_couleur_entete(const unsigned int n, const unsigned int i) {return &(couleur_entete[n][i]);}
   inline double* P_couleur_masque_corp(const unsigned int i)                  {return &(couleur_masque_corp[i]);}
   inline double* P_couleur_masque_bord(const unsigned int i)                  {return &(couleur_masque_bord[i]);}
};

#endif

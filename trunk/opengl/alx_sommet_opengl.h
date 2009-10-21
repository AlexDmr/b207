#ifndef __ALX_SOMMET_OPENGL_H__
#define __ALX_SOMMET_OPENGL_H__

#include "../physique/alx_geometrie2D.h"
#include "..\utilitaires\alx_liste.h"

#include <extgl.h>


class alx_sommet_opengl;
#include "../physique/alx_polygone2D.h"

class alx_sommet_opengl
{public:
   double coord_espace[3];
   double coord_texture[2];
   double coord_couleur[4];
};

inline void Combiner_couleurs( const double *coul1, const double coef1
                             , const double *coul2, const double coef2
                             , double *dest)
{dest[0] = coef1*coul1[0] + coef2*coul2[0];
 dest[1] = coef1*coul1[1] + coef2*coul2[1];
 dest[2] = coef1*coul1[2] + coef2*coul2[2];
 dest[3] = coef1*coul1[3] + coef2*coul2[3];}

inline void Copie_couleur(const double *source, double *dest)
{dest[0] = source[0];
 dest[1] = source[1];
 dest[2] = source[2];
 dest[3] = source[3];}

inline void Copie_position(const double *source, double *dest)
{dest[0] = source[0];
 dest[1] = source[1];
 dest[2] = source[2];}

class alx_polygone_opengl
{private:
   void Raffraichir_polygl();
   void Rendu_couleur() const;
   void Rendu_texture() const;
   void Rendu_couleur_et_texture() const;
   void Rendu_couleur_direct() const;
   void Rendu_texture_direct() const;
   void Rendu_couleur_et_texture_direct() const;

 public:
  // Les constituants
   alx_liste<int>    L_ordre_affichage;
   alx_polygone2D    *poly2D;
   alx_sommet_opengl *tab_sommets;
   unsigned int nb_sommets;
   unsigned int *ordre_affichage;

  // Les constructeurs
   alx_polygone_opengl()
    {nb_sommets = 0;
     ordre_affichage = (unsigned int *)NULL;
     tab_sommets = (alx_sommet_opengl*)NULL;
     poly2D = (alx_polygone2D*)NULL;}
   alx_polygone_opengl(alx_polygone2D *poly, const alx_liste<int> &L_aff) : L_ordre_affichage(L_aff)
    {poly2D = poly;
     nb_sommets  = poly->L_points.Taille();
     tab_sommets     = new alx_sommet_opengl[nb_sommets];
     ordre_affichage = new unsigned int[nb_sommets];
     for(unsigned int i=0;i<nb_sommets;i++) tab_sommets[i].coord_espace[2] = 0;
    }
   alx_polygone_opengl(int nb)
    {nb_sommets = nb;
     tab_sommets = new alx_sommet_opengl[nb];
     poly2D = (alx_polygone2D*)NULL;
     ordre_affichage = new unsigned int[nb];}
   alx_polygone_opengl(alx_sommet_opengl *tab, unsigned int nb)
    {nb_sommets = nb; tab_sommets = tab; poly2D = (alx_polygone2D*)NULL;
     ordre_affichage = new unsigned int[nb];}
   ~alx_polygone_opengl()
    {if(tab_sommets) delete[] tab_sommets;
     if(ordre_affichage) delete[] ordre_affichage;}

  // Les méthodes :
   void Raffraichir_polygl(const alx_polygone2D &poly);
   void maj(const unsigned int nb_s);
   void maj(alx_polygone2D *poly, const alx_liste<int> &L_aff);

   void Affichage_du_polygone2D_par_triangles(GLenum mode, int type_rendu); //1:couleur, 2:texture, 3:les deux.
   void Etirer_coordonneees_texture(alx_vecteur2D &v, alx_point2D &centre);

   void Affichage_liste_triangle_directe(GLenum mode, int type_rendu); //1:couleur, 2:texture, 3:les deux.
};

alx_polygone_opengl* Generer_poly_gl( const alx_polygone2D &poly2D, const double profondeur
                                    , const double r, const double v, const double b, const double a // couleur de base
                                    , const double deb_x, const double taille_x    // pour les coordonnées textures
                                    , const double deb_y, const double taille_y ); // pour les coordonnées textures
void tracer_polygone_texture_couleur(alx_sommet_opengl *tab, int nb_sommets);
void tracer_polygone_texture(alx_sommet_opengl *tab, int nb_sommets);
void tracer_polygone_couleur(alx_sommet_opengl *tab, int nb_sommets);

#endif

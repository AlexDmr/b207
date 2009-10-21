#ifndef __ALX_REPERE2D_LENTILLE_BASE_RECTANGLE_H__
#define __ALX_REPERE2D_LENTILLE_BASE_RECTANGLE_H__

#include "alx_repere2D.h"
//#include <mem.h>

typedef void (*f_ajustement )(alx_point2D &pt, void *param);

// La courbe unite définie la forme de la coupole de la lentille, elle
//        est exprimée par une suite de point ordonnés par abscisses croissantes
//        allant de 0 à 1. 0 étant le CENTRE de la lentille, 1 un bord.
// La bordure est une suite de point

// Le tableau des niveaux de détail exprime, pour chaque niveau, jusqu'à quelle
// coordonnées (exprimé en niveau 1) le ième niveau de détail est valide.
// De base, on a un seul niveau de détail, valide jusqu'à 1.

class info_courbe_lentille
{private:
   double *tab;
   unsigned int nb_points, real_nb;

 public:
   info_courbe_lentille(double *t, const unsigned int nb) {tab=t; nb_points=nb; real_nb = nb;}
   info_courbe_lentille() {tab = (double*)NULL; nb_points=0; real_nb = 0;}
   const double* Tab() const {return tab;}
   void Tab(double *t) {tab=t;}
   const unsigned int Nb() const {return nb_points;}
   void Nb(const unsigned int nb) {nb_points=nb;}

  // Pour en créer une de toute pièce
   void Allouer(const unsigned int nb) {if(real_nb<nb) {if(tab) delete[] tab; tab = new double [2*nb]; real_nb = nb;}; nb_points = nb;}
   void Affecter(const unsigned int num, const double x, const double y) {if(num >= nb_points) {return;}; tab[2*num]=x   ; tab[2*num+1]=y;   }
   void Affecter(const unsigned int num, const double *v)                {if(num >= nb_points) {return;}; tab[2*num]=v[0]; tab[2*num+1]=v[1];}
   void Affecter_x(const unsigned int num, const double v) {if(num >= nb_points) {return;}; tab[2*num]   = v;}
   void Affecter_y(const unsigned int num, const double v) {if(num >= nb_points) {return;}; tab[2*num+1] = v;}
   const double Consulter_x(const unsigned int num) const {if(num >= real_nb) {return ~0;}; return tab[2*num];}
   const double Consulter_y(const unsigned int num) const {if(num >= real_nb) {return ~0;}; return tab[2*num+1];}
   void Copier(const double *t, const unsigned int nb) {Allouer(nb); memcpy(tab, t, 2*nb*sizeof(double));}
   void maj(const info_courbe_lentille icl) {Copier(icl.Tab(), icl.Nb());}
};

void Initialiser_courbes_lentilles();
void Courbe_lentille_plate(info_courbe_lentille &rep);
void Courbe_lentille_bombe(info_courbe_lentille &rep);
void Courbe_lentille_test(info_courbe_lentille &rep);

double* Tab_niveaux_details(unsigned int n);

class alx_repere2D_lentille : public alx_repere2D
{private:
   static bool lentilles_initialisees;
   mutable bool sabordage;
   f_ajustement fct;
   void *param;
   info_courbe_lentille icl;

 protected:
   double text_x, text_y                                    // Les dimensions effective des textures utilisées
        , taille_x, taille_y
        , nb_pix_par_unite;
   unsigned int nb_niveaux_details; double *niveaux_details; // Le nombre et coordonnées de départ des niveaux de détails.
   unsigned int *nb_couche_par_niveau;
   unsigned int nb_pt_courbe
              , nb_pt_bordure;
   double *courbe_unite
        , *bordure
        , **lentille_sommets // 4 coords (x, y, tx, ty), modélise la structure spatiale et les coordonénes de plaquage des textures de la lentille
        , **lentille_tex_coords            // 1 tableau par niveau de détail, à cause des coordonnées de textures qui changent.
        , centre[4];  // 4 coords (x, y, tx, ty)

   unsigned int *ordres_affichage_ext
              , *ordres_affichage_int; // Tableau de tableaux indiquant les ordres d'affichages.
                                 // Sachant que la couche la plus centré se fait par un affichage en rayon. (décrit en 0)
                                // Les autres couches se font par liste de triangles. (décrit en 1)

   void Tranche( const double *P
               , const double *C                  // Le centre
               , const unsigned int dec_bord      // Pour faire la différence en CE et CT pour la bordure
               , unsigned int &p1, unsigned int &p2 ) const;
   void Triangle_contenant( const double *P
                          , const double *C
                          , const unsigned int dec_bord
                          , const unsigned int p1, const unsigned int p2
                          , const double **tab_tab
                          , unsigned int &niv
                          , unsigned int &T1, unsigned int &T2, unsigned int &T3 ) const;
 public:
  // Les constructeurs
   alx_repere2D_lentille();
  ~alx_repere2D_lentille();

   inline void Fonction_ajustement_point(f_ajustement f, void *p)
    {fct = f;
     param = p;}

   inline const bool Sabordage() const {return sabordage;}

  // Les méthodes surcharges de alx_repere2D
   virtual void Changer_coordonnees_inverse( alx_point2D &pt ) const;
   virtual void Changer_coordonnees( alx_point2D &pt ) const;

  // Les méthodes propres
   const info_courbe_lentille ICL() const {return icl;}
   void maj_lentille(const info_courbe_lentille &i) {Courbe_unite(i.Tab(), i.Nb());}
   void Courbe_unite(const double *C, const unsigned int nb);
   inline const double* Courbe_unite() const {return courbe_unite;}
   inline const unsigned int Nb_point_courbe_unite() const {return nb_pt_courbe;}

   void Bordure(const double *B, const unsigned int nb);
   inline const double* Bordure() const {return bordure;}
   inline const unsigned int Nb_point_bordure() const {return nb_pt_bordure;}

   void Centre(const double *c) {memcpy(centre, c, 4*sizeof(double));}
   const double* Centre() const {return centre;}

   void Niveaux_details(const double *niv, const unsigned int nb);

  // Gestion des points définnissant la lentille
   void Effacer_lentille();
   void Allouer_lentille();
   void Generer_lentille();
  // Gestion de l'ordre d'affichage de ces points
   void Effacer_ordre_affichage();
   void Allouer_ordre_affichage();
   void Generer_ordre_affichage();
  // Fonctions d'accès aux données
   inline const unsigned int* Ordre_couche_niveau  () const {return ordres_affichage_ext;}
   inline const unsigned int* Ordre_couche_centrale() const {return ordres_affichage_int;}

   inline const double* Couche_sommets_niveau(const unsigned int i)   const {return lentille_sommets[i];}
   inline const double* Couche_texcoords_niveau(const unsigned int i) const {return lentille_tex_coords[i];}
   inline const double* Couche_sommets_centrale()                     const {return lentille_sommets[nb_niveaux_details];}
   inline const double* Couche_texcoords_centrale()                   const {return lentille_tex_coords[nb_niveaux_details];}

   inline const unsigned int Nb_ordre_couche_niveau(const unsigned int i) const {return /*2+2*nb_pt_bordure*(nb_couche_par_niveau[i]-1);}*/2+(nb_couche_par_niveau[i]-1)*(8*(nb_pt_bordure-1)+8) - 4;} //-5 car on a pas à se recaller pour la dernière couche.
   inline const unsigned int Nb_ordre_couche_centrale()                   const {return 2+nb_pt_bordure;}

   inline const double Nb_pix_par_unite() const {return nb_pix_par_unite;}
   inline void Nb_pix_par_unite(const double n) {nb_pix_par_unite = n;}

   inline void Taille_effective_texture(const double tx, const double ty) {taille_x=tx; taille_y=ty;}
   inline const double Taille_x() const {return taille_x;}
   inline const double Taille_y() const {return taille_y;}
   inline void Dimensions_effectives_texture(const double x, const double y) {text_x=x; text_y=y;}
   inline const double Text_x() const {return text_x;}
   inline const double Text_y() const {return text_y;}

   inline const unsigned int Obtenir_couche_externe(const double p)
    {unsigned int res=0;
     while(courbe_unite[res<<1] < p) res++;
     return res-1;}
   void Changer_coord( alx_point2D &pt
                     , const double *P1, const double *T1
                     , const double *P2, const double *T2
                     , const double *P3, const double *T3) const;
};


#endif

#ifndef __ALX_EVENNEMENT_POINTEUR_H__
#define __ALX_EVENNEMENT_POINTEUR_H__

//#include "../../physique/alx_classes_bases.h"
#include "../physique/alx_geometrie2D.h"  // point, vecteur, droite, segment...
#include "../utilitaires/alx_chaine_char.h"

#define alx_pointeur_enfonce         1
#define alx_pointeur_relache         2
#define alx_pointeur_clique          4
#define alx_pointeur_double_clique   8
#define alx_pointeur_apparition     16
#define alx_pointeur_disparition    32
#define alx_pointeur_mouvement      64

typedef unsigned int alx_pointeur_type_evennement;

const alx_pointeur_type_evennement ALX_pointeur_enfonce    () {return alx_pointeur_enfonce;}
const alx_pointeur_type_evennement ALX_pointeur_relache    () {return alx_pointeur_relache;}
const alx_pointeur_type_evennement ALX_pointeur_apparition () {return alx_pointeur_apparition;}
const alx_pointeur_type_evennement ALX_pointeur_disparition() {return alx_pointeur_disparition;}
const alx_pointeur_type_evennement ALX_pointeur_mouvement  () {return alx_pointeur_mouvement;}
const alx_pointeur_type_evennement ALX_pointeur_ALL        () {return 255;}

class alx_evennement_info_point_et_vect
{public:
   double x, y;

   alx_evennement_info_point_et_vect &operator =(const alx_evennement_info_point_et_vect &pv)    // Surcharge du  =
    {x = pv.x;
     y = pv.y;
     return *this;}
};

class alx_pointeur;
class info_comp_evennement
{public:
   //alx_evennement_info_point_et_vect coord, dep;
   alx_point2D coord, dep;
   int type_appuie;
   alx_chaine_char comp_txt;

   inline void Coord(const double x, const double y) {coord.maj(x,y);}
   inline void Dep  (const double x, const double y) {dep.maj(x,y);}
   inline void Coord(const alx_point2D &pt) {coord.maj(pt);}
   inline void Dep  (const alx_point2D &pt) {dep.maj(pt);}
   inline void Type_appuie(const int v)              {type_appuie = v;}

   info_comp_evennement() {}
   info_comp_evennement &operator =(const info_comp_evennement &c)    // Surcharge du  =
    {coord.maj(c.coord);
     dep.maj(c.dep);
     type_appuie = c.type_appuie;
     comp_txt = c.comp_txt;
     return *this;}
};

class alx_noeud_scene;
class alx_evennement_pointeur
{private:
   int identifiant;
   alx_pointeur_type_evennement type_evt;
   info_comp_evennement         info_comp;
   alx_noeud_scene *racine;

 public :
   alx_pointeur *ptr;

   alx_evennement_pointeur() : identifiant(-1), racine(NULL) {}
   alx_evennement_pointeur( alx_evennement_pointeur *evt );
   alx_evennement_pointeur( const int id
                          , const alx_pointeur_type_evennement t_evt
                          , const info_comp_evennement        &comp
                          , alx_noeud_scene *r);


   inline       void                         Identifiant(const int i) {identifiant = i;}
   inline const int                          Identifiant()const {return identifiant;}
   inline const alx_pointeur_type_evennement Type_evt() const {return type_evt;   }
   inline void Type_evt(const alx_pointeur_type_evennement e) {type_evt = e;}
   inline const info_comp_evennement         Info_comp()  const {return info_comp;  }
   inline       alx_noeud_scene*             Racine()                   {return racine;}
   inline       void                         Racine(alx_noeud_scene *r) {racine = r;}
   inline       alx_pointeur* Ptr() {return ptr;}
   inline       void Ptr(alx_pointeur *p) {ptr = p;}
   alx_noeud_scene* Noeud_sous_pointeur();
   void             Noeud_sous_pointeur(alx_noeud_scene *n);

   void maj(const alx_evennement_pointeur &evt)
    {type_evt    = evt.type_evt;
     info_comp   = evt.info_comp;
     identifiant = evt.identifiant;
     ptr         = evt.ptr;
     racine      = evt.racine;}

   void maj( const int id
           , const alx_pointeur_type_evennement t_evt
           , const info_comp_evennement        &comp
           , alx_noeud_scene *r);

   alx_evennement_pointeur &operator =(const alx_evennement_pointeur &evt)    // Surcharge du  =
    {maj(evt);
     return *this;}
};

#endif

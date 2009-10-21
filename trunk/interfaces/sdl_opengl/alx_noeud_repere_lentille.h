#ifndef __ALX_NOEUD_REPERE_LENTILLE_H__
#define __ALX_NOEUD_REPERE_LENTILLE_H__

#include "../alx_noeud_scene.h"
#include "../../physique/alx_repere2D_lentille.h"

class alx_noeud_repere_lentille : public alx_noeud_scene
{private:
   bool deja_teste;
   alx_point2D pt2_tmp;
 public:
   alx_repere2D_lentille rep_lentille;

   alx_noeud_repere_lentille() : alx_noeud_scene(), deja_teste( false )
     {Nom_IU("Repère lentille");}

   virtual const char* Real_class_cmd() {return "Void_vers_rep_lentille";}

   inline void Fonction_ajustement_point(f_ajustement f, void *p)
    {rep_lentille.Fonction_ajustement_point(f, p);}

   virtual inline info_du_contenant* Contient(const alx_point2D &pt, int action)
    {if(deja_teste) return NULL;
     deja_teste = true;
       info_du_contenant *rep = alx_noeud_scene::Contient(pt, action);
     deja_teste = false;
     return rep;
    }

   virtual inline void Changer_coordonnees_prim( alx_primitive2D *prim )
    {rep_lentille.Changer_coordonnees_prim(prim);}

   virtual inline void Changer_coordonnees_prim_inverse( alx_primitive2D &prim )
    {rep_lentille.Changer_coordonnees_prim_inverse(prim);}

   virtual inline void Changer_coordonnees_inverse( alx_point2D &pt ) const
    {rep_lentille.Changer_coordonnees_inverse(pt);}

   virtual inline void Changer_coordonnees( alx_point2D &pt ) const
    {rep_lentille.Changer_coordonnees(pt);}

  virtual void Changer_coordonnees_inverse(double *T, const unsigned int nb) const;
  virtual void Changer_coordonnees        (double *T, const unsigned int nb) const;
};

alx_noeud_repere_lentille* Void_vers_rep_lentille(void *p) {return (alx_noeud_repere_lentille*)p;}
#endif

#ifndef __ALX_LIEUR_PTR_LIANT_H__
#define __ALX_LIEUR_PTR_LIANT_H__

#include "../physique/alx_classes_bases.h"
//#include "alx_evennement_pointeur.h"
#include "alx_simulateur_pointeurs.h"
#include "../physique/alx_simulateur_points_contact.h"

#include "../physique/table_hachage.h"

// On suppose que l'identifiant de ceux qui s'abonne est l'adresse de leur partie liant.
// Seuls des liants devrait pouvoir s'abonner ici.

// Ce liant répond à des appels d'autres liants qui veulent s'informer sur l'état
// d'un point en particulier. Voici de code de l'appel
// -1 : On veut une info
//    0 : sur un point
//      X : le type d'info voulu.
//        * : adresse du point
//          X : code à envoyer lors de l'appel à Propager (identifie la réponse)
// -2 : On ne veut plus d'info
//    0 : sur un point
//      X : le type d'info qu'on ne veut plus.
//        * : adresse du point
//          X : code d'envoi lors de l'appel à Propager (identifie la réponse)

class interet_liant
{public:
   alx_point2D *pt;
   alx_classe_base_liant *liant, *lieur;
   int code_appel;
   int interet_porte;

   interet_liant() {liant=NULL; lieur=NULL;}
   interet_liant( alx_classe_base_liant *l_demandeur, alx_classe_base_liant *l
                , int code, int interet, alx_point2D *p)
    {pt    = p;
     liant = l_demandeur;
     lieur = l;
     code_appel    = code;
     interet_porte = interet;}

   bool prevenir(int interet)
    {if( (interet&interet_porte) == 0 ) return false; // rien à faire
     liant->nb_infos_comp = 1;
     liant->infos_comp[0] = (int)(void*)pt;
     // liant->rationnaliser(code_appel); // Pas ici car on peut le tuer après appel au liant
     return true;
    }
};

//------------------------------------------------------------------------------
class correspondance_ptr_liant
{public:
   alx_pointeur *ptr;
   alx_point2D  *pt;
   alx_classe_base_liant *lieur;
   alx_liste<interet_liant> L_interet;

   correspondance_ptr_liant()
    { ptr=NULL; lieur=NULL; }

   correspondance_ptr_liant(alx_pointeur *p, alx_classe_base_liant *l)
    {ptr=p; lieur=l;}

   bool prevenir( alx_pointeur_type_evennement type_evt )  // Si s'il a effectivement pu prévenir quelqu'un
    {alx_element_liste<interet_liant> *it     = L_interet.Premier()
                                    , *it_fin = L_interet.Fin()
                                    , *it_tmp;
     bool rep = false;

     while(it!=L_interet.Fin())
      {it_tmp = it; it_tmp = it_tmp->svt;
       if( (it->E()).prevenir(type_evt) )
        {// on y va
         (it->E()).liant->rationnaliser( (it->E()).code_appel );
         rep = true;
        }
       it = it_tmp;
      }
     return rep;
    }

   inline correspondance_ptr_liant &operator =(const correspondance_ptr_liant &cpl)
    {pt        = cpl.pt;
     ptr       = cpl.ptr;
     lieur     = cpl.lieur;
     L_interet = cpl.L_interet;
     return *this;}
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
class alx_lieur_ptr_liant : public alx_classe_base_liant
{
 private:
   alx_simulateur_pointeurs *sim_pointeurs;
   table_hachage<correspondance_ptr_liant> table_correspondance;

   int Trouver_ou_creer_correspondant( alx_pointeur *ptr
                                     , alx_point2D *pt
                                     , int interet
                                     , int code
                                     , alx_classe_base_liant *l_demandeur ); // 0:trouvé, 1:créé.
   bool Supprimer_interet( alx_pointeur *ptr
                         , int interet
                         , int code
                         , alx_classe_base_liant *l_demandeur );

   correspondance_ptr_liant* trouver_cpl( alx_pointeur *ptr );

 public :
   alx_lieur_ptr_liant(alx_simulateur_pointeurs *sp)
    {sim_pointeurs = sp;
     sim_pointeurs->abonner_a(this, 0); // Pour que le sim_ptr prévienne.
    }

  // Les méthodes
   //void simuler();
   void rationnaliser(int num);
};

#endif

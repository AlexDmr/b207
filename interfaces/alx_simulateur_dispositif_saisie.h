#ifndef __ALX_SIMULATEUR_DISPOSITIF_SAISIE_H__
#define __ALX_SIMULATEUR_DISPOSITIF_SAISIE_H__

#include "../physique/alx_classes_bases.h"
#include "../physique/alx_model_simulation_physique1.h"
#include "alx_evennement_dispositif_saisie.h"
//#include <list.h>
//#include <vector.h>
#include "../physique/table_hachage.h"
//#include "alx_simulateur_couche.h"

// On doit gérer ici des pointeurs provenant de sources multiples
// qui ne sont pas forcéments connus. Ces sources fournirons des évennements
// que ce simulateurs devra répercuter aux intéressés. Il faut bien sur pouvoir
// identifier les pointeurs.
class alx_dispositif_saisie : public alx_classe_base_model
{
 private:
   int id;
   int *etat;

 public :

  // Les constructeurs
   alx_dispositif_saisie()      {id=-1; etat=(int*)NULL;}
   alx_dispositif_saisie(int i) {id=i ; etat=(int*)NULL;}

  // Les méthodes
   inline bool   id_valant(int i) const {return id==i;}
   inline int  Etat(int i) const {return etat[i];}
   inline bool Etat(int *e) {return etat==e;}
   inline int* Etat() {return etat;}
};
/*
class association_texte_physique
{
 public:
   alx_model_simulation_physique1 *mp;
   alx_classe_base_liant          *liant;
};

class association_texte_pointeur
{
 public:
   alx_classe_base_liant          *liant;
   alx_
};
*/
class alx_simulateur_dispositif_saisie : public alx_classe_base_liant
{
 private:
   alx_liste<alx_dispositif_saisie> L_dispositifs_saisies; // Liste tous les dispositifs déja utilisés;
   //table_hachage<association_texte_physique> table_correspondance;
   alx_liste<lien_interet> V_destinataires_precis;

   void maj_dispositif(alx_evennement_dispositif_saisie &e);
   void maj_dispositifs_saisies(alx_evennement_dispositif_saisie &e);

   alx_liste<Liste_de_rappel> L_rappels;  // un ancien vector
   Liste_de_rappel L_rappel_globale;

 public :
   alx_liste<alx_evennement_dispositif_saisie> L_evt;

   alx_simulateur_dispositif_saisie();

   int Ajouter_dispositif_saisie();       // renvoi le numéro l'identifiant.
   int Retirer_dispositif_saisie(int id); // retire le pointeur id, renvoi un code d'erreur.

   alx_dispositif_saisie* Dispositif_saisie(int id);
   alx_dispositif_saisie* Dispositif_saisie(int *ds);

   void abonner_a_evennements(const int id, alx_methode_rappel *m)    {alx_element_liste<Liste_de_rappel> *it = L_rappels.Premier();
                                                                       for(int i=0;i<id;i++) it=it->svt;
                                                                       (it->E()).Ajouter_methode_a_la_fin(m);}
   void desabonner_a_evennements(const int id, alx_methode_rappel *m) {alx_element_liste<Liste_de_rappel> *it = L_rappels.Premier();
                                                                       for(int i=0;i<id;i++) it=it->svt;
                                                                       (it->E()).Retirer_methode(m);} //L_rappels[id].Retirer_methode(m);}
   void abonner_a_tout_evennement_courant(alx_methode_rappel *m)
    {L_rappel_globale.Ajouter_methode_a_la_fin(m);}
   void desabonner_a_tout_evennement_courant(alx_methode_rappel *m)
    {L_rappel_globale.Retirer_methode(m);}


   // A FAIRE : Gérer plusieurs dispositifs dans cette fonction
   void Prendre_evennements_lies_a( alx_point2D *pt
                                  , alx_classe_base_liant *liant
                                  , int num = 2); // Numéro d'abonnement!
   void simuler();

   alx_simulateur_dispositif_saisie &operator =(const alx_simulateur_dispositif_saisie &sds)
    {
     L_dispositifs_saisies.maj(sds.L_dispositifs_saisies);
     L_evt.maj(sds.L_evt);
     L_rappels.maj(sds.L_rappels);

     return *this;
    }

};

#endif

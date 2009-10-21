#ifndef __ALX_CLASSE_BASE_MODELE_H__
#define __ALX_CLASSE_BASE_MODELE_H__

//#include <list.h>
#include "..\utilitaires\alx_liste.h"
#include "alx_lien_interet.h"
#include "alx_methode_rappel.h"
//#include <vector.h>

// CONSEIL :
// Abonner le modele au liant "direct" de l'entité afin qu'il soit appellé en dernier
// (car on place les nouveaux abonnés en tête de liste), ce afin que la rationnalisation
// de l'entité se fasse après tous les changements (sauf demande explcicte avant).

#define nb_max_infos_comp 10

class lien_interet;

class alx_classe_base_model
{
 private:
   alx_classe_base_liant *pivot;
   bool bloquer_emission_rationnalisation;
   bool est_deja_abonner( lien_interet &l );

 public:
  alx_liste<lien_interet> L_interessees;

  int nb_infos_comp;
  int infos_comp[nb_max_infos_comp]; // info complémentaire, utiles lorsqu'on doit mettre à jour les autres modèles

  inline alx_classe_base_model()
   {bloquer_emission_rationnalisation = false;
    pivot = (alx_classe_base_liant *)0;
    nb_infos_comp = 0;}

  void maj(const alx_classe_base_model &m)
   {bloquer_emission_rationnalisation = m.bloquer_emission_rationnalisation;
    L_interessees.maj(m.L_interessees);
    nb_infos_comp = m.nb_infos_comp;
    pivot = m.pivot;
    for(int i=0;i<nb_infos_comp;i++)
      infos_comp[i] = m.infos_comp[i];
   }

  alx_classe_base_model &operator =(const alx_classe_base_model &m)
   {maj(m);
    return *this;}

 /*****************************************************************************/
  inline virtual void simuler() {}

  virtual void abonner_a( lien_interet &l );
  virtual void abonner_en_dernier_a( lien_interet &l );

  inline virtual void abonner_en_dernier_a( alx_classe_base_liant *liant
                                   , int n)
   {lien_interet l(liant, n);
    abonner_en_dernier_a(l);}

  inline virtual void abonner_a( alx_classe_base_liant *liant
                        , int n)
   {lien_interet l(liant, n);
    abonner_a(l);}

  virtual void Oyez_oyez();
  virtual void Colporter(alx_classe_base_liant *demandeur);

  inline const bool Bloquer_emission_rationnalisation() const {return bloquer_emission_rationnalisation;}
  inline void Bloquer_emission_rationnalisation(const bool b) {bloquer_emission_rationnalisation = b;}
  inline bool* Ptr_emission_rationnalisation() {return &bloquer_emission_rationnalisation;}
};

typedef alx_classe_base_model* P_alx_classe_base_model;
#endif


#ifndef __TELECOMMANDE_CAMNOTE_H__
#define __TELECOMMANDE_CAMNOTE_H__

#include "..\physique\alx_classes_bases.h"
#include "..\Comet Olfa\olfa_comet.h"
#include "..\utilitaires\alx_chaine_char.h"

// Evennements
// Changement de numéro :
//   X : adresse sur un tableau de 2 unsigned int [ancien num, nouveau num]
//______________________________________________________________________________
// Rationnalisation :
// 0 : Changement de numero
//   X : ancien numéro
//     X : nouveau numéro
class alx_telec_cpp : public alx_classe_base_liant
{
 private:
   Liste_de_rappel     L_rappel_numero;
   alx_chaine_char T_commentaires[200];
   alx_chaine_char nom;
   unsigned int num_trans, ancien_nouveau[2];
   void init();
   
 protected:
   inline void Changement_numero(const unsigned int a_num, const unsigned int n_num)
    {ancien_nouveau[0] = a_num; ancien_nouveau[1] = n_num;
     L_rappel_numero.Rappeler(ancien_nouveau);
     alx_classe_base_model::nb_infos_comp = 3;
     alx_classe_base_model::infos_comp[0] = 0; // Changement de numero
     alx_classe_base_model::infos_comp[1] = (int)a_num; // l'ancien numéro
     alx_classe_base_model::infos_comp[2] = (int)n_num; // le nouveau numéro
     alx_classe_base_model::Oyez_oyez();}

 public :
   alx_telec_cpp( char *nom
                //, cogitant::Environment *e
                , alx_interpreteur_tcl  *interp );

   void Commentaire(const unsigned int num, const alx_chaine_char com)
    {if(num >= 200) return;
      //{T_commentaires.resize(num+10);}
     T_commentaires[num] = com;}
   inline const alx_chaine_char Commentaire(const unsigned int num) const
     {if(num<200) return T_commentaires[num]; else return T_commentaires[0];}

   inline void abonner_a_numero(alx_methode_rappel *m) {L_rappel_numero.Ajouter_methode_a_la_fin(m);}
   virtual const unsigned int Numero_transparent() const ;
   virtual void Numero_transparent(const unsigned int num);
};

#endif

#ifndef __OLFA_COMET_H__
#define __OLFA_COMET_H__

//#include <cogitant.h>
#include <tcl.h>
#include "..\utilitaires\alx_chaine_char.h"
#include "..\tcl\alx_interpreteur_tcl.h"

//______________________________________________________________________________
//__Ceux qui hériteront de cette classe devront penser à s'enregistrer dans TCL_
//______________________________________________________________________________
class olfa_comet
{
 private:
   //cogitant::iSet description_semantique;
   //cogitant::Environment *env;
   alx_interpreteur_tcl  *interpreteur;
   alx_chaine_char erreur, nom_usage, classe_comet, programme_tcl;
   static unsigned int nb_comets;
   char numero[256];
   bool graphe_charge
      , evoluer;

  // Les méthodes dont on a honte :
   //void Creer_objet_tcl();

 protected:
   alx_chaine_char nom_interne;
   void Commande(const char *c);

 public :
  // Les constructeurs
   olfa_comet() {interpreteur = (alx_interpreteur_tcl*)NULL;}
   olfa_comet( const char *nom
             //, cogitant::Environment *e
             , alx_interpreteur_tcl  *interp);

  void Enregistrer_dans_TCL();
 // Les méthodes pour manipulation de la comet dans TCL:
  inline const alx_chaine_char& Nom_usage() const {return nom_usage;}
  inline void Nom_usage(const alx_chaine_char n)
   {static alx_chaine_char s_cmd;
    static char c_cmd[256];
    s_cmd  = "set ";
    s_cmd += n; s_cmd += " ";
    s_cmd += Nom_interne();
	strcpy_s(c_cmd, s_cmd.Texte());
    //strcpy(c_cmd, s_cmd.Texte());
    Interpreteur()->Evaluer(c_cmd);
    nom_usage = n;
   }

  inline void Evoluer(const bool b) {evoluer = b;}
  inline const bool Evoluer() const {return evoluer;}
  inline const alx_chaine_char& Classe_comet() const {return classe_comet;}
  inline void Nom_interne(const char *n) {nom_interne = n;}
  inline const alx_chaine_char& Nom_interne() const {return nom_interne;}
  inline alx_chaine_char& Programme_tcl() {return programme_tcl;}
  inline void                  Interpreteur( alx_interpreteur_tcl *interp) {interpreteur = interp;}
  inline alx_interpreteur_tcl* Interpreteur()                              {return interpreteur;}

 // Les méthodes liées aux graphes conceptuels
  /*bool Charger_graphe_description_semantique(const char *nom_fichier);
  bool Charger_graphe_description_semantique(const cogitant::iSet &g);
  bool Decharger_graphe_description();

  inline void  Environnement(cogitant::Environment *e) {env = e;}
  inline const cogitant::Environment* Environnement()  {return env;}

  inline cogitant::iSet* Description_semantique() {return &description_semantique;}
  */
  inline void olfa_comet::Proposer_evolutions   () {Commande("Proposer");}
  inline void olfa_comet::Analyser_propositions () {Commande("Analyser");}
  inline void olfa_comet::Effectuer_propositions() {Commande("Effectuer");}
  inline void olfa_comet::Excecuter_propositions() {Commande("Excecuter");}
  inline void olfa_comet::Lister_conseils       () {Commande("Lister_conseils");}
  inline void olfa_comet::Lister_deconseils     () {Commande("Lister_deconseils");}

 // Les méthodes informatives sur l'état des opérations
  inline const char* Information_erreur() const {return erreur.Texte();}

 // Les méthodes définies par Olfa
  // A tout niveau
          // Concepts
          // Tâches
          // Utilisateur
          // Plate-forme
          // Environnement
          // Evolution
          // Transition
  // Définit pour la comet en entier
          // EstPlastique(Contexte c[], Propriété p[])
  // Définit pour les IU concrètes
          // Style
          // Usage
  // Définit pour les IU finales
          // Propriétés
};

class Environnement_comet_olfa : public alx_interpreteur_tcl
{
 private:

 public :
  // Constructeurs
   Environnement_comet_olfa(const char *nom_exec);
};

#endif

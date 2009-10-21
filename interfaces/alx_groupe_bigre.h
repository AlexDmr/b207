#ifndef __ALX_GROUPE_BIGRE_H__
#define __ALX_GROUPE_BIGRE_H__

#include "..\utilitaires\alx_chaine_char.h"
#include "..\physique\table_hachage.h"
#include "..\horloge\alx_horloge_vectorielle.h"

/* Les messages et les réponses attendus:
   ^LISTER_GROUPES EMETTEUR=(.*)$
     ^INFO_LISTE_GROUPE NOM=(.*); NUM=([0-9]*); IP=(.*)$  //IP du gars qui fait parti du groupe
   ^JOINDRE_GROUPE EMETTEUR=(.*); GROUPE=([0-9]*)$
     ^INFO_NOEUDS_GROUPE NUM=([0-9]*); (.*)$
   ^QUITTER_GROUPE EMETTEUR=(.*); GROUPE=([0-9]*)$
*/

class alx_noeud_scene;

class alx_groupe_bigre
{private:
  // Les noeuds locaux des pointeurs distants
   alx_liste<alx_noeud_scene *> liste_des_pointeurs_distants;

  // Identification du groupe :
   alx_chaine_char nom;
   unsigned int numero;
  // Identification des noeuds :
   alx_noeud_scene *noeud_modele;
   table_hachage<alx_noeud_scene*> Table_nom;
   mutable alx_noeud_scene *racine;
  // Identification des membres :
   class alx_membre_bigre
    {private:
       alx_chaine_char ip;
     public :
       alx_membre_bigre(const alx_chaine_char &cc) {ip = cc;}
       const alx_chaine_char& IP() const {return ip;}
    };
   unsigned int nb_membres;
  // Marqueurs pour le mentient de la cohérence avec l'extérieur
   bool tout_reemettre
      , emettre_IP_horloge
      , groupe_coherent
      , emettre_pointeurs
      , visualiser_pointeurs;
   void init();

 public :
  // Horloge connue:
   alx_horloge_vectorielle horloge;
   alx_horloge_vectorielle& Horloge() {return horloge;}

  // Les constructeurs :
   alx_groupe_bigre() {init(); horloge.Ordre(1);}
   alx_groupe_bigre(const unsigned int nb, const char *nom)
    {init(); this->nom=nom; Numero(nb); horloge.Ordre(nb);}
   alx_groupe_bigre(const unsigned int nb, const alx_chaine_char &nom)
    {init(); this->nom=nom; Numero(nb); horloge.Ordre(nb);}
  //~alx_groupe_bigre() {}

 // Les méthodes :
  // Le nom
   inline void Nom(const alx_chaine_char &n) {nom = n;}
   inline const alx_chaine_char& Nom() const {return nom;}
  // Le numéro
   inline const unsigned int Numero() const {return numero;}
   inline void Numero(const unsigned int n) {numero = n;}
  // La racine
   inline void Racine(alx_noeud_scene *r) {racine = r;}
   inline alx_noeud_scene* Racine() const {return racine;}
  // Gestion, Emission/visualisation des pointeurs?
   inline void Enregistrer_noeud_pointeur(alx_noeud_scene *noeud_ptr)              {liste_des_pointeurs_distants.Ajouter_a_la_fin(noeud_ptr);}
   inline const alx_liste<alx_noeud_scene *>& Liste_des_pointeurs_distants() const {return liste_des_pointeurs_distants;}

   inline const bool Emettre_pointeurs() const {return emettre_pointeurs;}
   inline void Emettre_pointeurs(const bool b) {emettre_pointeurs = b;}
   inline const bool Visualiser_pointeurs() const {return visualiser_pointeurs;}
   inline void Visualiser_pointeurs(const bool b) {visualiser_pointeurs = b;}

  // La table de hachage
   inline const table_hachage<alx_noeud_scene*>& Table_Nom() const {return Table_nom;}
   void Enregistrer_noeud(const alx_noeud_scene *n);
   alx_noeud_scene* Adresse_noeud(const alx_chaine_char &nom);
  // Les marqueurs pour la ré-émission
   inline void Tout_reemettre(const bool b) {tout_reemettre = b;}
   inline const bool Tout_reemettre() const {return tout_reemettre;}
   inline void Emettre_IP_horloge(const bool b) {emettre_IP_horloge = b;}
   inline const bool Emettre_IP_horloge() const {return emettre_IP_horloge;}
  // Le marqueur de cohérence du groupe
   void Groupe_coherent(const bool b) {groupe_coherent = b;}
   const bool Groupe_coherent() const {return groupe_coherent;}

  // Les surcharges d'opérateurs
   const alx_groupe_bigre& operator=(const alx_groupe_bigre &gb);
};

#endif

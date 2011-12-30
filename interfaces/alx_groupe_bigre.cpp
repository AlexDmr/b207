#include "alx_groupe_bigre.h"
#include "alx_noeud_scene.h"

/* Dans un groupe il nous faut retenir les liens entre les noeuds répliqués en local
   et leurs noms, afin de pouvoir y accéder rapidement.
   Si un tel noeud n'éxiste pas déja il faut le créer.
   Il faut aussi maintenir la connaissance des ordinateurs qui participent au groupe.
   Cela afin de les classer entre eux selon leur adresse IP, de sorte que ce classement soit
   le même sur chaque ordinateur.
   On a un noeud en local qui est le père du graphe répliqué
    
   Soit NP le noeud_ père,
   La sérialisation de l'interface se fait à partir du noeud père.
   On ne tiendra compte, pour le moment, que de sa liste de fils lors de la désérialisation.
   Pour ce qui est e l'association NOM<->@locale, utiliser une table de hachage.
*/

void alx_groupe_bigre::init()
{tout_reemettre     = false;
 emettre_IP_horloge = false;
 groupe_coherent    = false;
 Emettre_pointeurs   (true);
 Visualiser_pointeurs(true);
 noeud_modele = new alx_noeud_scene();}
//______________________________________________________________________________
//__________________Fonction aidant à l'insertion dans la table_________________
//______________________________________________________________________________
const int Valuer_chaine(const alx_chaine_char &cc, const unsigned int nb)
{unsigned int rep = cc.Taille();
 const char *texte = cc.Texte();
 for(unsigned int i=0; i<cc.Taille(); i++)
   rep += texte[i];
 return (int)(rep%nb);}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
bool f_recherche_noeud(alx_noeud_scene **modele, alx_noeud_scene **cour)
{return (*modele)->Nom() == (*cour)->Nom();}

//______________________________________________________________________________
void alx_groupe_bigre::Enregistrer_noeud(const alx_noeud_scene *n)
{Table_nom.inserer( (alx_noeud_scene * const)n
                  , Valuer_chaine(n->Nom(), Table_nom.Nb_hachage()) );
}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
alx_noeud_scene* alx_groupe_bigre::Adresse_noeud(const alx_chaine_char &nom)
{alx_noeud_scene **rep;
 noeud_modele->Nom( nom );
 if( Table_nom.trouver( Valuer_chaine(nom, Table_nom.Nb_hachage())
                      , &rep
                      , f_recherche_noeud
                      , &noeud_modele) )
   return *rep;
 else return (alx_noeud_scene*)NULL;
}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
const alx_groupe_bigre& alx_groupe_bigre::operator=(const alx_groupe_bigre &gb)
{Nom( gb.Nom() );
 Numero( gb.Numero() );
 Table_nom = gb.Table_Nom();
 Racine( gb.Racine() );
 horloge = gb.horloge;
 groupe_coherent = gb.Groupe_coherent();
 Emettre_pointeurs( gb.Emettre_pointeurs() );
 Visualiser_pointeurs( gb.Visualiser_pointeurs() );
 return *this;
}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________


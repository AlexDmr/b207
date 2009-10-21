#ifndef __ALX_SIMULATEUR_POINTEURS_H__
#define __ALX_SIMULATEUR_POINTEURS_H__

#include "..\physique/alx_classes_bases.h"
#include "..\physique/alx_model_simulation_physique1.h"
#include "alx_evennement_pointeur.h"
#include "..\utilitaires\alx_liste.h"
#include "..\utilitaires\alx_chaine_char.h"
#include "..\utilitaires\MetaData.h"

// On doit gérer ici des pointeurs provenant de sources multiples
// qui ne sont pas forcéments connus. Ces sources fournirons des évennements
// que ce simulateurs devra répercuter aux intéressés. Il faut bien sur pouvoir
// identifier les pointeurs.
class alx_simulateur_pointeurs_generique;
class alx_noeud_scene;
class alx_pointeur : public alx_classe_base_model
{private:
   int id;
   alx_chaine_char alx_chaine_info, nom_ptr;
   mutable alx_chaine_char serialisation;
   mutable alx_point2D pt_tmp;
   bool est_une_replique;
   alx_noeud_scene *pere, *representation, *noeud_sous_pointeur;
   Liste_de_rappel L_rappel_changement;
   alx_liste<alx_repere2D*> reperes;
   MetaData metaData;

 public :
   alx_point2D pt;
   int appuie; // Chaque bit pour un bouton!
  // Les constructeurs
   alx_pointeur()                          {Est_une_replique(false); id=-1; appuie=0; pere = NULL; representation = noeud_sous_pointeur = NULL; L_rappel_changement.Param(this);}
   alx_pointeur(alx_noeud_scene *p, int i) {Est_une_replique(false); id=i;  appuie=0; pere = p;    representation = noeud_sous_pointeur = NULL; L_rappel_changement.Param(this);}
  ~alx_pointeur() {L_rappel_changement.Vider();}

  // Les méthodes
   inline alx_noeud_scene* Noeud_pere()                   {return pere;}
   inline void             Noeud_pere(alx_noeud_scene *n) {pere = n;}
   inline alx_noeud_scene* Noeud_representation()                   {return representation;}
   void                    Noeud_representation(alx_noeud_scene *n);
   inline alx_noeud_scene* Noeud_sous_pointeur()                   {return noeud_sous_pointeur;}
   inline void             Noeud_sous_pointeur(alx_noeud_scene *n) {noeud_sous_pointeur = n;}

   inline const alx_liste<alx_repere2D*>& L_reperes() const {return reperes;}
   inline void L_reperes(const alx_liste<alx_repere2D*> &L) {reperes.maj(L);}
   inline void Vider_L_reperes()                            {reperes.Vider();}
   inline void Add_to_L_reperes(alx_repere2D *r)            {reperes.Ajouter_a_la_fin(r);}

   inline void Nom(const alx_chaine_char &n) {nom_ptr = n;}
   inline void Nom(const char *n)            {nom_ptr = n;}
   inline const alx_chaine_char& Nom() const {return nom_ptr;}

   inline const bool Est_une_replique() const {return est_une_replique;}
   inline void Est_une_replique(const bool b) {est_une_replique = b;}

   inline const int Id() const {return id;}
   inline const bool id_valant(int i)              const {return id==i;}

   inline const int Appuie() const {return appuie;}
   inline void Appuie(const int i) {appuie = i;}

   inline alx_point2D* P_Point() {return &pt;}
   inline const alx_point2D& Point() const {return pt;}

   inline const double X() const {return pt.X();}
   inline void X(const double v) {pt.X(v);}
   inline void Y(const double v) {pt.Y(v);}
   inline const double Y() const {return pt.Y();}
   inline void Coord(const double x, const double y) {pt.maj(x, y);}
   inline void Coord(const alx_point2D &p)           {pt.maj(p);}

   inline const bool Add_MetaData(const char *var, const char *val) {return metaData.Ajouter(var, val);}
   inline const bool Sub_MetaData(const char *var)                  {return metaData.Retirer(var);}
   inline const char* Val_MetaData(const char *var) const           {return metaData.Val(var);}
   inline const char* get_MetaData()                const           {return metaData.get_texte();}
   inline void        Vider_MetaData()                              {metaData.Vider();}

   inline const bool adresse_pt_en(alx_point2D *p) const {return &pt==p;}

  // La gestion des listes de rappels
   inline void abonner_a_changement    (alx_methode_rappel *m) {L_rappel_changement.Ajouter_methode_a_la_fin(m);}
   inline void desabonner_de_changement(alx_methode_rappel *m) {L_rappel_changement.Retirer_methode(m);}
   inline void Rappel_changement       ()                      {L_rappel_changement.Rappeler(this);}

   const bool operator!=(const alx_pointeur &p) const;  // Surcharge du  ==
   const bool operator==(const alx_pointeur &p) const;

   inline const alx_chaine_char& Serialiser(const alx_liste<alx_repere2D*> *L_rep) const
    {serialisation = Id();
       serialisation.Ajouter("_",0,1);
     pt_tmp.maj( pt );
     if(L_rep) Repere_transformation(pt_tmp, *L_rep);
     serialisation += pt_tmp.x;
       serialisation.Ajouter("_",0,1);
     serialisation += pt_tmp.y;
       serialisation.Ajouter("_",0,1);
     serialisation += appuie;
     return serialisation;
    }
};
typedef alx_pointeur*        P_alx_pointeur;
typedef const alx_pointeur* CP_alx_pointeur;
alx_pointeur* Void_vers_alx_pointeur(void *p) {return (alx_pointeur*)p;}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
class alx_simulateur_pointeurs : public alx_classe_base_liant
{private:
   class Pipo_pointeur_detruit
    {public:
       int id;
       double x, y;
       Pipo_pointeur_detruit() {}
       Pipo_pointeur_detruit(int ID, double X, double Y) {x = X; y = Y; id = ID;}
       const Pipo_pointeur_detruit &operator =(const Pipo_pointeur_detruit &ptr) {id = ptr.id; x = ptr.x; y = ptr.y; return *this;}
    };
   unsigned int nb_info_ptr;
   alx_liste<alx_pointeur> L_pointeurs; // Liste tous les identifiant déja utilisés;
   mutable alx_liste<Pipo_pointeur_detruit> L_pointeurs_detruits;
   alx_pointeur* maj_pointeurs(alx_evennement_pointeur &e);
   mutable alx_chaine_char serialisation;
   alx_point2D pt_tmp;
   CP_alx_pointeur TP_ptr[256];
   alx_evennement_pointeur evt_tmp;

   bool *T_free_id;
   unsigned int nb_T_free_id;
   const unsigned int get_a_point_id();
   void release_a_point_id(const unsigned int id);

 public :
   alx_liste<alx_evennement_pointeur> L_evt;

   alx_simulateur_pointeurs();

   int Ajouter_pointeur(const char *n, alx_noeud_scene *pere, const unsigned int nb = 1);       // renvoi le numéro l'identifiant.
   int Retirer_pointeur(int id); // retire le pointeur id, renvoi un code d'erreur.

   alx_pointeur* Pointeur_id(int id) {return Pointeur(id);}
   alx_pointeur* Pointeur(int id);
   alx_pointeur* Pointeur(alx_point2D *pt);
   alx_pointeur* Pointeur(const alx_chaine_char &n);
   const unsigned int Nb_pointeurs() {return L_pointeurs.Taille();}

   const unsigned int Simuler();
   void Simuler(alx_evennement_pointeur &evt);

   const alx_chaine_char& Serialiser(const alx_liste<alx_repere2D*> *L_rep) const;
   const alx_chaine_char& Serialiser_diff(const alx_liste<alx_repere2D*> *L_rep) const;

   alx_simulateur_pointeurs &operator =(const alx_simulateur_pointeurs &sp)
    {L_pointeurs.maj(sp.L_pointeurs);
     L_evt.maj(sp.L_evt);
     return *this;}

   alx_liste<alx_pointeur>& L_ptr() {return L_pointeurs;}
   void Ajouter_evt(const alx_evennement_pointeur &evt) {L_evt.Ajouter_a_la_fin( evt );}
};

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
class alx_pointeur_rapporteur
{public:
   int                      id;
   alx_simulateur_pointeurs *sim_ptr;

   alx_pointeur_rapporteur &operator =(const alx_pointeur_rapporteur &r)
    {id      = r.id;
     sim_ptr = r.sim_ptr;
     return *this;}
};

#endif

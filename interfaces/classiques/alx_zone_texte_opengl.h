#ifndef __ALX_ZONE_TEXTE_OPENGL_H__
#define __ALX_ZONE_TEXTE_OPENGL_H__

#include "..\..\utilitaires\alx_chaine_char.h"

class alx_model_simulation_physique1;
class gl_fonte;
//#include "../../opengl/texte.h"
//#include "../../opengl/alx_image.h"
//#include "../../opengl/alx_materiel.h"

#include "../alx_simulateur_dispositif_saisie.h"
#include "../alx_quemendeur_de_pointeur.h"

class alx_zone_texte_opengl : public alx_quemendeur_de_pointeur
{private:
   mutable bool seri_diff_Texte_a_ete_change;

 protected:
   Liste_de_rappel L_rappel_car_tape, L_rappel_chgmnt_pos_curseur;
   char *tmp;

   alx_point2D *pt_de_curseur;
   double hauteur, largeur, zoom
        , l_zoom, h_zoom
        , x_max, y_max; // Bordure maximum du texte effectif, utils pour déterminer les dimansions au plus juste
   double  decalage_x, decalage_y
        , optimal_tx, optimal_ty;
   bool afficher_fond, editable, lisser, mode_selection, mode_copie_colle
      , a_changer;

   gl_fonte *fonte;
   double couleur_texte[4], couleur_curseur[4], couleur_fond[4];
   alx_model_simulation_physique1 *mp_corp;

   alx_simulateur_dispositif_saisie *sim_dispo_saisis;

  // Les données relative à la position du curseur dans le texte:
   unsigned int V_position_debut_des_lignes[1024];
   int position_affichage_debut
     , position_curseur
     , ligne_courante, derniere_ligne_ecrite, ligne_curseur
     , ligne_de_depart
     , debut_selection, fin_selection;
   int nb_ligne_cour
     , nb_ligne_max;
  // Les méthodes cachées
   void afficher_ligne(int deb);
   void afficher_curseur(int deb);
   void init_mp_corp();
   void maj_mp_corp();
   void maj_curseur();
   void Placer_curseur(const alx_point2D &);

   void Monter_ligne(int pos);
   void Descendre_ligne(int pos);
   void Debut_ligne(int pos);
   void Fin_ligne(int pos);
   void Monter_page(int pos);
   void Descendre_page(int pos);
   void inserer(const char *c);
   void inserer(const alx_chaine_char &s);

   char c_texte[65536];

   alx_chaine_char texte;

 public :

  alx_repere2D *repere;
  void          Repere(alx_repere2D *rep) {repere = rep;}
  alx_repere2D* Repere()                  {return repere;}

 // Les constructeurs
  alx_zone_texte_opengl( const double l, const double h, gl_fonte *f
                       , alx_simulateur_dispositif_saisie *sds);
 ~alx_zone_texte_opengl();

 //____________________________________________________________________________
  const bool Seri_diff_Texte_a_ete_change(const alx_chaine_char **) const;

 // Les méthodes
  inline const gl_fonte* Fonte() const {return fonte;}
  inline alx_model_simulation_physique1* Mp_corp() {return mp_corp;}

  inline alx_chaine_char* Texte() {return &texte;}
  inline const alx_chaine_char& Chaine_char()     {return texte;}
  inline void Maj_texte(const char *t)            {seri_diff_Texte_a_ete_change = true; A_changer(true); texte = t;}
  inline void Maj_texte(const alx_chaine_char &t) {seri_diff_Texte_a_ete_change = true; A_changer(true); texte = t;}
  inline void Ecrire(const char *t)               {seri_diff_Texte_a_ete_change = true; A_changer(true); texte += t;}
  inline void Ecrire(const alx_chaine_char &t)    {seri_diff_Texte_a_ete_change = true; A_changer(true); texte += t;}

  inline const double Hauteur() const {return hauteur;}
  inline const double Largeur() const {return largeur;}
  inline const double Zoom   () const {return zoom;}
  inline void   Hauteur(const double h) {hauteur = h; h_zoom = h*zoom; maj_mp_corp(); maj_curseur(); A_changer(true); }
  inline void   Largeur(const double l) {largeur = l; l_zoom = l*zoom; maj_mp_corp(); maj_curseur(); A_changer(true); }
  inline void   Zoom   (const double z)
   {zoom   = z;
    h_zoom = hauteur/z;
    l_zoom = largeur/z;
    maj_curseur();
    A_changer(true);
   }

 // Les méthodes:
  void Optimiser_dimensions();
  void Calculer_dimensions_optimales();
  const double Optimal_tx() const {return optimal_tx;}
  void Optimal_tx(const double v) {optimal_tx = v;}
  const double Optimal_ty() const {return optimal_ty;}
  void Optimal_ty(const double v) {optimal_ty = v;}

  inline void Afficher_fond(const bool b) {afficher_fond = b; A_changer(true);}
  inline const bool Afficher_fond() const {return afficher_fond;}

  inline void A_changer(const bool b) {a_changer = b;}
  inline const bool A_changer() const {return a_changer;}

  inline void Editable(const bool e) {editable = e; A_changer(true);}
  inline const bool Editable() const {return editable;}
  inline void Couleur_texte(const double *c) {memcpy(couleur_texte, c, sizeof(double)<<2); A_changer(true);}
  inline void Couleur_texte(const double r, const double v, const double b, const double a)
   {couleur_texte[0]=r; couleur_texte[1]=v; couleur_texte[2]=b; couleur_texte[3]=a; A_changer(true);}
  inline const double Couleur_texte(const unsigned int canal) {return couleur_texte[canal];}
  inline void Couleur_texte(const unsigned int canal, const double v)
   {couleur_texte[canal] = v; A_changer(true);}
  inline const double* Couleur_texte() const {return couleur_texte;}


  inline void Couleur_curseur(const double r, const double v, const double b, const double a)
   {couleur_curseur[0]=r; couleur_curseur[1]=v; couleur_curseur[2]=b; couleur_curseur[3]=a; A_changer(true);}
  inline void Couleur_curseur(const double *c) {memcpy(couleur_curseur, c, sizeof(double)<<2);}
  inline const double* Couleur_curseur() const {return couleur_curseur;}
  inline const bool Lisser() const {return lisser;}
  inline void Lisser(const bool b) {lisser = b; A_changer(true);}

  inline void Couleur_fond(const double r, const double v, const double b, const double a)
   {couleur_fond[0]=r; couleur_fond[1]=v; couleur_fond[2]=b; couleur_fond[3]=a; A_changer(true);}
  inline void Couleur_fond(const double *c) {memcpy(couleur_fond, c, sizeof(double)<<2); A_changer(true);}
  inline const double* Couleur_fond() const {return couleur_fond;}

  inline void Ligne_de_depart(const int l) {ligne_de_depart = l; A_changer(true);}
  inline const int Ligne_de_depart() const {return ligne_de_depart;}

  void rationnaliser(int num);
  void afficher();

  inline void Position_curseur(const unsigned int p) {position_curseur = p; A_changer(true);}
  inline const unsigned int Position_curseur() const {return position_curseur;}
  char* Derniere_phrase();
  inline void Positionner_curseur_a_la_fin()
   {position_curseur = texte.Taille(); A_changer(true);}

  inline void abonner_a_caractere_tape(alx_methode_rappel *m)
   {L_rappel_car_tape.Ajouter_methode_a_la_fin(m);}
  inline void Informer_que_caractere_tape(int c)
   {L_rappel_car_tape.Rappeler(&c);}

  inline void abonner_a_chgmnt_pos_curseur(alx_methode_rappel *m)
   {L_rappel_chgmnt_pos_curseur.Ajouter_methode_a_la_fin(m);}
  inline void Informer_que_pos_curseur_change(int pos)
   {L_rappel_chgmnt_pos_curseur.Rappeler(&pos);}

};

#endif

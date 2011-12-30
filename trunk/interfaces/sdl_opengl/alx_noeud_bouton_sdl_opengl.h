#ifndef __ALX_NOEUD_BOUTON_SDL_OPENGL_H__
#define __ALX_NOEUD_BOUTON_SDL_OPENGL_H__

class alx_interface_sdl_opengl_1;

#include "alx_noeud_fenetre_sdl_opengl.h"

//#include "../../physique/alx_classes_bases.h"
//#include "../classiques/alx_bouton_opengl.h"

#include "../alx_evennement_pointeur.h"
#include "../alx_quemendeur_de_pointeur.h"

#include "..\..\physique\math_alex.h"
#include "..\..\physique\alx_geometrie2D.h"
//#include "alx_noeud_transformations_sdl_opengl.h"

#include "alx_noeud_image_sdl_opengl.h"
//#include "..\..\opengl\alx_image_opengl.h"

#include "..\fontes.h"


typedef enum {Bouton_enfonce, Bouton_relache} Etat_bouton;

class alx_noeud_bouton_sdl_opengl :  public alx_noeud_scene, public alx_quemendeur_de_pointeur //alx_noeud_fenetre_sdl_opengl
{
 private:
   Liste_de_rappel L_rappel_enfonce, L_rappel_relache, L_rappel_actionne;
   alx_materiel *matos;
   alx_noeud_fenetre_sdl_opengl *fenetre_bouton;
   Etat_bouton etat_bouton;
   alx_vecteur2D vect_trans;
   bool affichage_principal;//, pression_initiale;
   alx_point2D *pt_observe;
   alx_noeud_image_sdl_opengl *img_noeud_tmp;
   alx_interface_sdl_opengl_1 *interface_mere;
   alx_noeud_scene *noeud_racine;

 protected:
   void Creer_fenetre( const double tx, const double ty
                     , alx_interface_sdl_opengl_1 *interface_m
                     , int type_de_fenetre );

 public :
   alx_noeud_bouton_sdl_opengl( alx_image_opengl &
                              , alx_interface_sdl_opengl_1 *interface_m
                              , alx_noeud_scene *racine
                              , int type_de_fenetre );
   alx_noeud_bouton_sdl_opengl( const double tx, const double ty
                              , alx_interface_sdl_opengl_1 *interface_m
                              , alx_noeud_scene *racine
                              , int type_de_fenetre );
   ~alx_noeud_bouton_sdl_opengl();

   virtual const char* Real_class_cmd() {return "Void_vers_bouton";}

   inline alx_noeud_fenetre_sdl_opengl* Noeud_fen() {return fenetre_bouton;}
   inline void   Hauteur(const double h)  {fenetre_bouton->Fenetre()->Hauteur_corp(h);}
   inline void   Longueur(const double l) {fenetre_bouton->Fenetre()->Longueur_corp(l);}
   inline double Hauteur()  const {return fenetre_bouton->Fenetre()->Hauteur_corp();}
   inline double Longueur() const {return fenetre_bouton->Fenetre()->Longueur_corp();}
   inline double Epaisseur_bordure() const {return fenetre_bouton->Fenetre()->Epaisseur_bordure();}

   inline void Etirement_du_contenu(const double e) {fenetre_bouton->Etirement_du_contenu(e);}
   inline void Etirement_du_contenu(const double ex, const double ey) {fenetre_bouton->Etirement_du_contenu(ex, ey);}
   inline void Etirement_du_contenu(const alx_vecteur2D &ve) {fenetre_bouton->Etirement_du_contenu(ve);}
   inline const alx_vecteur2D Etirement_du_contenu() const {return fenetre_bouton->Etirement_du_contenu();}
   inline void Epaisseur_bordure(const double e) {fenetre_bouton->Fenetre()->Epaisseur_bordure(e);}

   inline const double* Couleur_bordure_int() {return fenetre_bouton->Fenetre_opengl()->Couleur_bordure_int();}
   inline void Couleur_bordure_int(const double r, const double v, const double b, const double a)
    {fenetre_bouton->Fenetre_opengl()->Couleur_bordure_int(r,v,b,a);
     fenetre_bouton->Fenetre_opengl()->Couleur_bordure_int_angle(r,v,b,a);}
   inline const double* Couleur_bordure_ext() {return fenetre_bouton->Fenetre_opengl()->Couleur_bordure_ext();}
   inline void Couleur_bordure_ext(const double r, const double v, const double b, const double a)
    {fenetre_bouton->Fenetre_opengl()->Couleur_bordure_ext(r,v,b,a);
     fenetre_bouton->Fenetre_opengl()->Couleur_bordure_ext_angle(r,v,b,a);}

   inline void Lisser(const bool b) {fenetre_bouton->Lisser(b);}
   inline const bool Lisser() const {return fenetre_bouton->Lisser();}

   inline virtual const char* Nom_IU() const               {return alx_noeud_scene::Nom_IU();}
   inline virtual void Nom_IU(const alx_chaine_char &cc)   {alx_noeud_scene::Nom_IU("Gestion\nbouton"); fenetre_bouton->Nom_IU(cc);}
   inline virtual void Nom_IU(const char *n)               {alx_noeud_scene::Nom_IU("Gestion\nbouton"); fenetre_bouton->Nom_IU(n);}

   inline void Mettre_a_jour()
    {fenetre_bouton->Fenetre()->Appliquer_maj();
     fenetre_bouton->Fenetre_opengl()->Mettre_a_jour();
     fenetre_bouton->Pre_rendu_exeptionnel(true);
     //fenetre_bouton->Ne_pas_pre_rendre(false);
     //fenetre_bouton->PreRendre();
     //fenetre_bouton->Ne_pas_pre_rendre(true);
    }

   inline void Ne_pas_pre_rendre(const bool b) {fenetre_bouton->Ne_pas_pre_rendre(b);}
   inline const bool Ne_pas_pre_rendre() const {return fenetre_bouton->Ne_pas_pre_rendre();}

/*   inline void Afficher()
    {glPushMatrix();
       glTranslated(Origine().x, Origine().y, 0);
       if(Rotation()!=0)
         glRotated(Rotation()*180/PI, 0, 0, 1);
       glScaled(Etirement().dx, Etirement().dy, 1);
       fenetre_bouton->Afficher();
     glPopMatrix();}                 */

  // Renvoi un pointeur sur le modèle physique qui contient la primitive, NULL si aucun.
   //info_du_contenant* Contient(alx_point2D &pt, int action);

   void rationnaliser(int num);
   inline void Abonner_a(alx_classe_base_liant *liant)
     {abonner_a(liant, (int)(void*)(alx_classe_base_liant*)this); }
   inline void Abonner_a(alx_classe_base_liant *liant, int num)
     {abonner_a(liant, num); }


   inline virtual void Position_bg(const alx_point2D &p)
    {Position_bg(p.x, p.y);}
   inline virtual void Position_bg(const double x, const double y)
    {fenetre_bouton->Position_bg(x, y);}

   inline const alx_point2D& Origine() const {return alx_noeud_scene::Origine();}
   inline void Origine(const alx_point2D &pt) {alx_noeud_scene::Origine(pt);            vect_trans.maj( Origine() );}
   inline void Origine(const double x, const double y) {alx_noeud_scene::Origine(x, y); vect_trans.maj( Origine() );}

   inline virtual void Translation(const alx_vecteur2D &v)   {fenetre_bouton->Translation(v);      vect_trans.maj( Origine() );}
   inline void Translation(const double dx, const double dy) {fenetre_bouton->Translation(dx, dy); vect_trans.maj( Origine() );}
   inline const alx_vecteur2D& Translation() const {return vect_trans;}

  // Il faudra penser à surcharger les Ajouter_fils et compagnie!!!

  // Les états du bouton
   inline void abonner_a_enfonce   (alx_methode_rappel *m) {L_rappel_enfonce.Ajouter_methode_a_la_fin(m);}
   inline void desabonner_a_enfonce(alx_methode_rappel *m) {L_rappel_enfonce.Retirer_methode(m);}
   inline void Enfoncer_bouton()
    {if(etat_bouton == Bouton_enfonce) return;
     //pression_initiale = true;
     etat_bouton = Bouton_enfonce;

     fenetre_bouton->Fenetre_opengl()->Couleur_bordure_int_angle(0,1,0,1);
     fenetre_bouton->Fenetre_opengl()->Couleur_bordure_int(0,1,0,1);
     fenetre_bouton->Fenetre_opengl()->Couleur_bordure_ext_angle(0,0,0,1);
     fenetre_bouton->Fenetre_opengl()->Couleur_bordure_ext(0,0,0,1);
     fenetre_bouton->Fenetre()->Appliquer_maj();
     fenetre_bouton->Fenetre_opengl()->Mettre_a_jour();

     L_rappel_enfonce.Rappeler(this);
     alx_classe_base_model::nb_infos_comp = 2;
     alx_classe_base_model::infos_comp[0] = 0; // Changement d'état
     alx_classe_base_model::infos_comp[1] = 0; // on enfonce
     alx_classe_base_model::Oyez_oyez();
    }
//______________________________________________________________________________
   inline void abonner_a_relache   (alx_methode_rappel *m) {L_rappel_relache.Ajouter_methode_a_la_fin(m);}
   inline void desabonner_a_relache(alx_methode_rappel *m) {L_rappel_relache.Retirer_methode(m);}
   inline void Relacher_bouton()
    {if(etat_bouton == Bouton_relache) return;
     //pression_initiale = false;
     etat_bouton = Bouton_relache;

     fenetre_bouton->Fenetre_opengl()->Couleur_bordure_int_angle(0,0,0,1);
     fenetre_bouton->Fenetre_opengl()->Couleur_bordure_int(0,0,0,1);
     fenetre_bouton->Fenetre_opengl()->Couleur_bordure_ext_angle(0,1,0,1);
     fenetre_bouton->Fenetre_opengl()->Couleur_bordure_ext(0,1,0,1);
     fenetre_bouton->Fenetre()->Appliquer_maj();
     fenetre_bouton->Fenetre_opengl()->Mettre_a_jour();

     L_rappel_relache.Rappeler(this);
     alx_classe_base_model::nb_infos_comp = 2;
     alx_classe_base_model::infos_comp[0] = 0;
     alx_classe_base_model::infos_comp[1] = 1; // on relache
     alx_classe_base_model::Oyez_oyez();
    }
//______________________________________________________________________________
   inline void abonner_a_activation    (alx_methode_rappel *m) {L_rappel_actionne.Ajouter_methode_a_la_fin(m);}
   inline void desabonner_de_activation(alx_methode_rappel *m) {L_rappel_relache.Retirer_methode(m);}
   inline void Activer_bouton()
    {if(etat_bouton == Bouton_relache) return;
     //pression_initiale = false;
     etat_bouton = Bouton_relache;

     fenetre_bouton->Fenetre_opengl()->Couleur_bordure_int_angle(0,0,0,1);
     fenetre_bouton->Fenetre_opengl()->Couleur_bordure_int(0,0,0,1);
     fenetre_bouton->Fenetre_opengl()->Couleur_bordure_ext_angle(0,1,0,1);
     fenetre_bouton->Fenetre_opengl()->Couleur_bordure_ext(0,1,0,1);
     fenetre_bouton->Fenetre()->Appliquer_maj();
     fenetre_bouton->Fenetre_opengl()->Mettre_a_jour();

     L_rappel_actionne.Rappeler(this);
     alx_classe_base_model::nb_infos_comp = 2;
     alx_classe_base_model::infos_comp[0] = 0;
     alx_classe_base_model::infos_comp[1] = 2; // on active
     alx_classe_base_model::Oyez_oyez();
    }
};

typedef alx_noeud_bouton_sdl_opengl* P_alx_noeud_bouton_sdl_opengl;
inline alx_noeud_bouton_sdl_opengl* Void_vers_bouton (void *p) {return (alx_noeud_bouton_sdl_opengl*)p;}

#endif

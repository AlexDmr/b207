#ifndef __ALX_FICHE_OPENGL_H__
#define __ALX_FICHE_OPENGL_H__

#include "alx_fiche.h"
#include "alx_fenetre_opengl.h"

#include "..\..\opengl\alx_sommet_opengl.h"

class alx_materiel;

const int nb_point_cercle = 16;

class alx_fiche_opengl : public alx_fenetre_opengl
{
 private:
  //------DEBUG-------
   //int angle;
  //------------------
   void init();
   void Maj_sommets();
   //void rafraichir(int val_stencil, GLenum op1, GLenum op2);

   //Gluint liste_corp, liste_entete, liste_zone_redim;
   alx_materiel *materiel_de_fond;
   
   alx_polygone_opengl corp
                     , entete
                     , zone_rot, zone_rot_base // Pour ne pas recalculer les cos sin à chaque fois
                     , zone_redim[8];
 public :
   alx_fiche *fiche;

  // Les constructeurs:
   alx_fiche_opengl( alx_fiche *f
                   , gl_fonte *fonte_pour_entete
                   , alx_materiel *matos);
   
  // Les méthodes:
   //void rationnaliser(int num);
   void Transfo_commune(); 
   void Fin_Transfo_commune();
   void afficher_fond() ;
   void afficher_corp() ;
   void afficher_entete();
   void afficher_zone_rot();
   void afficher_bordure();

   void Mettre_a_jour();

   void Etirer_texture_fond(alx_vecteur2D &v, alx_point2D &centre);

   alx_polygone_opengl* Pointeur_sur_coord_corp() {return &corp;}
//   void afficher_zone_dessin_des_fils();
};


#endif
 
#ifndef __ALX_FICHE_OVALE_OPENGL_H__
#define __ALX_FICHE_OVALE_OPENGL_H__

#include "alx_fiche_ovale.h"
#include "alx_fenetre_opengl.h"


class alx_fiche_ovale_opengl : public alx_fenetre_opengl
{private:
   void init();
   void Maj_sommets();
   void rafraichir(int val_stencil, GLenum op1, GLenum op2);

   double liste_corp, liste_entete, liste_zone_redim
         , fact_x, fact_y;
   double *abscisse, *ordonnee;

   unsigned int nb_sommets;
   alx_materiel *materiel_de_fond;
   gl_fonte *fonte_entete;
   alx_polygone_opengl corp
                     , entete
                     , zone_rot, zone_rot_base // Pour ne pas recalculer les cos sin à chaque fois
                     , zone_redim;
 public :
   alx_fiche_ovale *fiche;

  // Les constructeurs:
   alx_fiche_ovale_opengl( alx_fiche_ovale *f
                         , gl_fonte *fonte_pour_entete
                         , alx_materiel *matos);

  // Les méthodes:
   //void rationnaliser(int num);
   const unsigned int Nb_sommets() const {return nb_sommets;}
   void Nb_sommets(const unsigned int nb)
    {nb_sommets = nb;
     if(abscisse) delete abscisse;
     if(ordonnee) delete ordonnee;
     abscisse = new double[nb_sommets];
     ordonnee = new double[nb_sommets];
    // Calcul des coordonnées des points
     double angle, angle_base = 2*PI/nb_sommets;
     for(unsigned int i=0;i<nb_sommets;i++)
      {angle = i*angle_base;
       abscisse[i] = cos(angle);
       ordonnee[i] = sin(angle);}
     Maj_sommets();
    }

   void Transfo_commune();
   void Etirement_commun();
   void Fin_Transfo_commune();
   void afficher_fond() ;
   void afficher_corp() ;
   void afficher_entete();
   void afficher_zone_rot();
   void afficher_bordure();

   void Mettre_a_jour();
   void maj_fact();

   void Maj_coord_text_entete(double coord_x, double coord_y);
   void Maj_coord_text(double coord_x, double coord_y);
   void Etirer_texture_fond(alx_vecteur2D &v, alx_point2D &centre);

   alx_polygone_opengl* Pointeur_sur_coord_corp() {return &corp;}
};

#endif

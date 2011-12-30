#include "alx_fiche_ovale_opengl.h"
#include "../../physique/math_alex.h"

#include "..\..\opengl\alx_image.h"
#include "..\..\opengl\config_opengl.h"
#include <glut.h>

/*******************************************************************************/
/******************************* Les constructeurs******************************/
/*******************************************************************************/
void alx_fiche_ovale_opengl::Maj_coord_text(double coord_x, double coord_y)
{
// double a = fiche->Longueur_corp()/2
//      , b = fiche->Hauteur_corp()/2;
 double *tab_text
       , *tab_esp;
 alx_sommet_opengl *tab = corp.tab_sommets;

 tab[0].coord_texture[0] = coord_x/2;
 tab[0].coord_texture[1] = coord_y/2;
 for(unsigned int i=0;i<nb_sommets;i++)
  {
   tab_text = tab[1+i].coord_texture;
   tab_esp  = tab[1+i].coord_espace;
   tab_text[0] = (0.5+tab_esp[0]/2)*coord_x;
   tab_text[1] = (0.5+tab_esp[1]/2)*coord_y;
  }
 tab_text = tab[1+nb_sommets].coord_texture;
 tab_esp  = tab[1+nb_sommets].coord_espace;
 tab_text[0] = (0.5+tab_esp[0]/2)*coord_x;
 tab_text[1] = (0.5+tab_esp[1]/2)*coord_y;
}

void alx_fiche_ovale_opengl::Mettre_a_jour()
{Maj_sommets();}

void alx_fiche_ovale_opengl::Maj_sommets()
{
 unsigned int i, position, pos_seixieme_cercle, pos_huitieme_cercle, pos_quart_cercle;

// glBegin(GL_TRIANGLE_FAN);
   if((nb_sommets+2) != corp.nb_sommets)
    {corp.maj(nb_sommets+2);
     zone_redim.maj((nb_sommets+1)*2);
     entete.maj(nb_sommets+2);
     }
   alx_sommet_opengl *tab        = corp.tab_sommets
                   , *tab_zr     = zone_redim.tab_sommets
                   , *tab_entete = entete.tab_sommets;
   double rayon_ext_abs, rayon_ext_ord;

   if(fiche->Longueur_corp() == 0) rayon_ext_abs = 1;
    else rayon_ext_abs = 1+(2*fiche->Epaisseur_bordure()/fiche->Longueur_corp());
   if(fiche->Hauteur_corp() == 0) rayon_ext_ord = 1;
    else rayon_ext_ord = 1+(2*fiche->Epaisseur_bordure()/fiche->Hauteur_corp());

   tab[0].coord_espace[0] = 0;
   tab[0].coord_espace[1] = 0;
   Copie_couleur(Couleur_corps(), tab[0].coord_couleur);
   //tab[0].coord_texture[0] = 0.5;
   //tab[0].coord_texture[1] = 0.5;
   pos_quart_cercle    = nb_sommets>>2;
   pos_huitieme_cercle = nb_sommets>>3;
   pos_seixieme_cercle = nb_sommets>>4;
   for(i=0;i<nb_sommets;i++)
    {
     position = 1+i;
     tab[position].coord_espace[0] = abscisse[i];
     tab[position].coord_espace[1] = ordonnee[i];
     Copie_couleur(Couleur_corps(), tab[position].coord_couleur);

     position = 2*i;
     tab_zr[position].coord_espace[0] = abscisse[i];
     tab_zr[position].coord_espace[1] = ordonnee[i];
     if((i+pos_seixieme_cercle)%pos_quart_cercle <= pos_huitieme_cercle)
       Copie_couleur(Couleur_bordure_int(), tab_zr[position].coord_couleur);
      else Copie_couleur(Couleur_bordure_int_angle(), tab_zr[position].coord_couleur);
     tab_zr[position].coord_couleur[3] = Couleur_masque_bord()[3];
     position++;
     tab_zr[position].coord_espace[0] = abscisse[i]*rayon_ext_abs;
     tab_zr[position].coord_espace[1] = ordonnee[i]*rayon_ext_ord;
     if((i+pos_seixieme_cercle)%pos_quart_cercle <= pos_huitieme_cercle)
       Copie_couleur(Couleur_bordure_ext(), tab_zr[position].coord_couleur);
      else Copie_couleur(Couleur_bordure_ext_angle(), tab_zr[position].coord_couleur);
     tab_zr[position].coord_couleur[3] = Couleur_masque_bord()[3];
    }
   tab[1+nb_sommets].coord_espace[0] = abscisse[0];
   tab[1+nb_sommets].coord_espace[1] = ordonnee[0];
   Copie_couleur(Couleur_corps(), tab[1+nb_sommets].coord_couleur);
     position = 2*nb_sommets;
     tab_zr[position].coord_espace[0] = abscisse[0];
     tab_zr[position].coord_espace[1] = ordonnee[0];
     Copie_couleur(Couleur_bordure_int(), tab_zr[position].coord_couleur);
     tab_zr[position].coord_couleur[3] = Couleur_masque_bord()[3];
     position++;
     tab_zr[position].coord_espace[0] = abscisse[0]*rayon_ext_abs;
     tab_zr[position].coord_espace[1] = ordonnee[0]*rayon_ext_ord;
     Copie_couleur(Couleur_bordure_ext(), tab_zr[position].coord_couleur);
     tab_zr[position].coord_couleur[3] = Couleur_masque_bord()[3];

// On passe à l'entête
 unsigned int limite = nb_sommets/2;
 double h, l, coef;

 if(fiche->Longueur_corp() == 0) l = 1;
  else l = 2*fiche->Hauteur_entete()/fiche->Longueur_corp();
 if(fiche->Hauteur_corp() == 0) h = 1;
  else h = 2*fiche->Hauteur_entete()/fiche->Hauteur_corp();


 position = 0;

 for(i=0;i<=limite;i++)
  {coef = 1 - ((double)i/(double)limite);

   Copie_position(tab_zr[position+1].coord_espace, tab_entete[position].coord_espace);
   Combiner_couleurs( Couleur_entete(1), coef
                    , Couleur_entete(0), 1-coef
                    , tab_entete[position].coord_couleur);
   tab_entete[position].coord_texture[0] = coef;
   tab_entete[position].coord_texture[1] = 0;
   position++;

   Copie_position(tab_zr[position].coord_espace, tab_entete[position].coord_espace);
   tab_entete[position].coord_espace[0] += l*abscisse[i];
   tab_entete[position].coord_espace[1] += h*ordonnee[i];
   Combiner_couleurs( Couleur_entete(2), coef
                    , Couleur_entete(3), 1-coef
                    , tab_entete[position].coord_couleur);
   tab_entete[position].coord_texture[0] = coef;
   tab_entete[position].coord_texture[1] = 1;
   position++;
  }

}

//______________________________________________________________________________
void alx_fiche_ovale_opengl::init()
{abscisse = ordonnee = NULL;
 Nb_sommets(64);}

alx_fiche_ovale_opengl::alx_fiche_ovale_opengl( alx_fiche_ovale *f
                                              , gl_fonte *fonte_pour_entete
                                              , alx_materiel *matos) : alx_fenetre_opengl(f, fonte_pour_entete)
{fiche = f;
 //fonte_entete = fonte_pour_entete;
 materiel_de_fond = matos; // 1:couleur simple, 2:texteure, 3:les deux.
 init();}

/*******************************************************************************/
/**********************************Les méthodes*********************************/
/*******************************************************************************/
void alx_fiche_ovale_opengl::Etirer_texture_fond(alx_vecteur2D &v, alx_point2D &centre)
{}

void alx_fiche_ovale_opengl::maj_fact()
{fact_x = fiche->Longueur_corp()/2; // On divise par 2 car le cercle à un RAYON de 1
 fact_y = fiche->Hauteur_corp()/2; // or il faut qu'il ait un DIAMETRE de 1
}

void alx_fiche_ovale_opengl::Transfo_commune()
{
// Appliquer les transformations:
 alx_vecteur2D /*trans_retour
             , */trans2;
// double angle_radian;

 //Maj_sommets();
 maj_fact();
 glPushMatrix();
   Changement_repere( *(fiche->repere) );
}

void alx_fiche_ovale_opengl::Etirement_commun()
{glScalef(fact_x, fact_y, 1);}

void alx_fiche_ovale_opengl::Fin_Transfo_commune()
{glPopMatrix();}

void alx_fiche_ovale_opengl::Maj_coord_text_entete(double coord_x, double coord_y)
{
 int limite = nb_sommets/2;
 double coef, l
       , lg_cour = 0;
 alx_sommet_opengl *tab_entete = entete.tab_sommets
                 , *sommet;
 double a = fiche->Longueur_corp()/2
      , b = fiche->Hauteur_corp() /2;
 alx_point2D prcdt
           , cour;

 l = 0;
 for(int i=limite;i>=0;i--)
  {prcdt.maj(cour);
   cour.maj(a+a*abscisse[i], b*ordonnee[i]);
   l += cour.distance(prcdt);}

// fiche->Longueur_entete(l*1.005);
 cour.maj(0,0);
 int position = 2*limite+1;
 for(int i=limite;i>=0;i--)
  {prcdt.maj(cour);
   cour.maj(a+a*abscisse[i], b*ordonnee[i]);
   lg_cour += cour.distance(prcdt);
   coef = ((double)lg_cour/l)*coord_x;

   sommet = &(tab_entete[position]);
   sommet->coord_texture[0] = coef;
   sommet->coord_texture[1] = coord_y;
   position--;

   sommet = &(tab_entete[position]);
   sommet->coord_texture[0] = coef;
   sommet->coord_texture[1] = 0;
   position--;
  }
}

void alx_fiche_ovale_opengl::afficher_entete()
{
 if( Zdr_entete()==NULL
   ||Zdr_entete()->Infos_texture_tempon(0)->tx==0
   ||Zdr_entete()->Infos_texture_tempon(0)->ty==0 )
   return;

 //float etirement_texte;
 double coord_x, coord_y;

 // Affichage proprement dit de l'entête:
 glPushAttrib(GL_ENABLE_BIT);
  glEnable(GL_TEXTURE_2D);
       glBindTexture(GL_TEXTURE_2D, Zdr_entete()->Infos_texture_tempon(0)->id_texture);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
       coord_x = Zdr_entete()->Nb_unite_x()*Zdr_entete()->Nb_pixels_par_unite()/Zdr_entete()->Infos_texture_tempon(0)->tx;
       coord_y = Zdr_entete()->Nb_unite_y()*Zdr_entete()->Nb_pixels_par_unite()/Zdr_entete()->Infos_texture_tempon(0)->ty;
       Maj_coord_text_entete(coord_x, coord_y);
       glColor4dv( alx_fenetre_opengl::Couleur_masque_bord() );
       entete.Affichage_liste_triangle_directe(GL_TRIANGLE_STRIP, 2);//tracer_polygone_couleur(entete, 4);
 glPopAttrib();
}

void alx_fiche_ovale_opengl::afficher_zone_rot()
{if( (fact_x==0)
   ||(fact_y==0)) return;
 alx_cercle2D_physique *cercle = &( fiche->mp_zone_rot.L_cercle2D_physique.Premier_E() );
 double centre_x = cercle->x
      , centre_y = cercle->y;

 glPushMatrix();
 glScalef(1/fact_x, 1/fact_y, 1);
   glTranslated(centre_x, centre_y, 0.0);
// Affichage proprement dit de la zone de rotation:
   glColor4d(0.3, 1.0, 0.3, Couleur_masque_bord()[3]);
   //glutSolidTorus(fiche->Rayon_zone_rotation()*0.2, fiche->Rayon_zone_rotation()*0.8, 2, 16);
   glTranslated(-centre_x, -centre_y, 0.0);
 glPopMatrix();
}

void alx_fiche_ovale_opengl::afficher_bordure()
{zone_redim.Affichage_liste_triangle_directe(GL_TRIANGLE_STRIP, 1);}

void alx_fiche_ovale_opengl::afficher_fond()
{corp.Affichage_liste_triangle_directe(GL_TRIANGLE_FAN, 1); //Triangles n-1, n, n+1; mode couleur simple
 }

void alx_fiche_ovale_opengl::afficher_corp()
{glColor4dv( Couleur_masque_corp() );
 corp.Affichage_liste_triangle_directe(GL_TRIANGLE_FAN, 2); //Triangles n-1, n, n+1; mode texture simple
 }



#include "alx_fiche_opengl.h"
#include "../../physique/math_alex.h"

#include "..\..\opengl\alx_image.h"
#include "..\..\opengl\config_opengl.h"

#include <glut.h>


/*******************************************************************************/
/******************************* Les constructeurs******************************/
/*******************************************************************************/
void alx_fiche_opengl::init()
{corp.maj(4);   corp.ordre_affichage[0] = 1; corp.ordre_affichage[1] = 0; corp.ordre_affichage[2] = 2; corp.ordre_affichage[3] = 3;
 entete.maj(4); entete.ordre_affichage[0] = 1; entete.ordre_affichage[1] = 0; entete.ordre_affichage[2] = 2; entete.ordre_affichage[3] = 3;
 int pos=0;
 for(int i=0;i<4;i++)
  {zone_redim[pos].maj(6);
   zone_redim[pos].ordre_affichage[0] = 1; zone_redim[pos].ordre_affichage[1] = 2; zone_redim[pos].ordre_affichage[2] = 0;
   zone_redim[pos].ordre_affichage[3] = 3; zone_redim[pos].ordre_affichage[4] = 5; zone_redim[pos].ordre_affichage[5] = 4;
   pos++;
   zone_redim[pos].maj(4);
   zone_redim[pos].ordre_affichage[0] = 1; zone_redim[pos].ordre_affichage[1] = 0; zone_redim[pos].ordre_affichage[2] = 2; zone_redim[pos].ordre_affichage[3] = 3;
   pos++;
  }
 Mettre_a_jour();
}

void alx_fiche_opengl::Mettre_a_jour()
{int i, pos = 0;

// Mise à jour des dimensions des polygones OpenGL:
// Corp et entete :
 //alx_polygone2D pol_tmp;

 corp.Raffraichir_polygl  ( fiche->mp_corp.L_polygone2D.Premier_E()   );
 entete.Raffraichir_polygl( fiche->mp_entete.L_polygone2D.Premier_E() );
// Pas de polygone pour la zone_rot, on utilise un disque
// Bordure :
 for(i=0;i<4;i++)
  {zone_redim[pos].Raffraichir_polygl( fiche->mp_zone_redim[pos].L_polygone2D.Premier_E() ); pos++;
   zone_redim[pos].Raffraichir_polygl( fiche->mp_zone_redim[pos].L_polygone2D.Premier_E() ); pos++; }

// Mise à jour des couleurs :
 for(i=0;i<4;i++)
  {Copie_couleur(Couleur_corps() , corp.tab_sommets[i].coord_couleur);
   Copie_couleur(Couleur_entete(i), entete.tab_sommets[i].coord_couleur); }

 double *tab;
 for(i=0;i<4;i++)
  {pos = i; // Les coins
   pos=pos<<1;
   tab = zone_redim[pos].tab_sommets[0].coord_couleur;
      Copie_couleur(Couleur_bordure_ext_angle() , tab);
      tab[3] = Couleur_masque_bord()[3];
   tab = zone_redim[pos].tab_sommets[1].coord_couleur;
     Copie_couleur(Couleur_bordure_ext() , tab);
     tab[3] = Couleur_masque_bord()[3];
   tab = zone_redim[pos].tab_sommets[2].coord_couleur;
     Copie_couleur(Couleur_bordure_int() , tab);
     tab[3] = Couleur_masque_bord()[3];
   tab = zone_redim[pos].tab_sommets[3].coord_couleur;
     Copie_couleur(Couleur_bordure_int_angle() , tab);
     tab[3] = Couleur_masque_bord()[3];
   tab = zone_redim[pos].tab_sommets[4].coord_couleur;
     Copie_couleur(Couleur_bordure_int() , tab);
     tab[3] = Couleur_masque_bord()[3];
   tab = zone_redim[pos].tab_sommets[5].coord_couleur;
     Copie_couleur(Couleur_bordure_ext() , tab);
     tab[3] = Couleur_masque_bord()[3];

   pos++; // Les cotés
   tab = zone_redim[pos].tab_sommets[0].coord_couleur;
     Copie_couleur(Couleur_bordure_ext() , tab);
     tab[3] = Couleur_masque_bord()[3];
   tab = zone_redim[pos].tab_sommets[1].coord_couleur;
     Copie_couleur(Couleur_bordure_ext() , tab);
     tab[3] = Couleur_masque_bord()[3];
   tab = zone_redim[pos].tab_sommets[2].coord_couleur;
     Copie_couleur(Couleur_bordure_int() , tab);
     tab[3] = Couleur_masque_bord()[3];
   tab = zone_redim[pos].tab_sommets[3].coord_couleur;
     Copie_couleur(Couleur_bordure_int() , tab);
     tab[3] = Couleur_masque_bord()[3];
  }
}

alx_fiche_opengl::alx_fiche_opengl( alx_fiche *f
                                  , gl_fonte *fonte_pour_entete
                                  , alx_materiel *matos) : alx_fenetre_opengl(f, fonte_pour_entete)
{fiche = f;
 materiel_de_fond = matos; // 1:couleur simple, 2:texteure, 3:les deux.
 init();}

/*******************************************************************************/
/**********************************Les méthodes*********************************/
/*******************************************************************************/
void alx_fiche_opengl::Etirer_texture_fond(alx_vecteur2D &v, alx_point2D &centre)
{

}

void alx_fiche_opengl::afficher_fond()
{// Affichage du polygone qui tapisse le fond
 materiel_de_fond->Activer();
 corp.Affichage_liste_triangle_directe(GL_TRIANGLE_STRIP, 1); //Triangles n-1, n, n+1; mode couleur simple
}

void alx_fiche_opengl::afficher_corp()
{// Affichage proprement dit du corp:
 corp.Affichage_liste_triangle_directe(GL_TRIANGLE_STRIP, 2); //Triangles n-1, n, n+1; mode couleur simple
}

void alx_fiche_opengl::Transfo_commune()
{double angle_radian;
// alx_vecteur2D trans_retour
//             , trans2;
 glPushMatrix();
   Changement_repere( *(fiche->repere) );
}

void alx_fiche_opengl::Fin_Transfo_commune()
{glPopMatrix();}

void alx_fiche_opengl::afficher_entete()
{if( Zdr_entete()==NULL
   ||Zdr_entete()->Infos_texture_tempon(0)->tx==0
   ||Zdr_entete()->Infos_texture_tempon(0)->ty==0 )
   return;

 //float etirement_texte;
 double coord_x, coord_y;
 alx_sommet_opengl *ptr_corp = entete.tab_sommets;

 glPushAttrib(GL_ENABLE_BIT);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glEnable(GL_TEXTURE_2D);
       glBindTexture(GL_TEXTURE_2D, Zdr_entete()->Infos_texture_tempon(0)->id_texture);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
       //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
       coord_x = Zdr_entete()->Nb_unite_x()*Zdr_entete()->Nb_pixels_par_unite()/Zdr_entete()->Infos_texture_tempon(0)->tx;
       coord_y = Zdr_entete()->Nb_unite_y()*Zdr_entete()->Nb_pixels_par_unite()/Zdr_entete()->Infos_texture_tempon(0)->ty;
       ptr_corp[0].coord_texture[0] = 0.0    ; ptr_corp[0].coord_texture[1] = 0.0;
       ptr_corp[1].coord_texture[0] = coord_x; ptr_corp[1].coord_texture[1] = 0.0;
       ptr_corp[2].coord_texture[0] = coord_x; ptr_corp[2].coord_texture[1] = coord_y;
       ptr_corp[3].coord_texture[0] = 0.0    ; ptr_corp[3].coord_texture[1] = coord_y;
       glColor4dv( alx_fenetre_opengl::Couleur_masque_bord() );
       entete.Affichage_liste_triangle_directe(GL_TRIANGLE_STRIP, 2);//tracer_polygone_couleur(entete, 4);
 glPopAttrib();
}

void alx_fiche_opengl::afficher_zone_rot()
{alx_cercle2D_physique *cercle = &( fiche->mp_zone_rot.L_cercle2D_physique.Premier_E() );
 double centre_x = cercle->x
      , centre_y = cercle->y;


 glTranslated(centre_x, centre_y, 0);
  glColor4d(0.3, 1.0, 0.3, Couleur_masque_bord()[3]);
  //glutSolidTorus(fiche->Rayon_zone_rotation()*0.2, fiche->Rayon_zone_rotation()*0.8, 2, 16);
 glTranslated(-centre_x, -centre_y, 0);
}

void alx_fiche_opengl::afficher_bordure()
{for(int i=0;i<8;i++)
  zone_redim[i].Affichage_liste_triangle_directe(GL_TRIANGLE_STRIP, 1); //tracer_polygone_couleur(zone_redim, 4);
}



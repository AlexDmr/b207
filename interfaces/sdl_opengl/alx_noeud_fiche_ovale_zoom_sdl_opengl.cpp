#include "alx_noeud_fiche_ovale_zoom_sdl_opengl.h"
#include "alx_interface_sdl_opengl_1.h"

#include "..\classiques\alx_fiche_ovale_opengl.h"

//______________________________________________________________________________
void fct_ajust(alx_point2D &pt, void *param)
{alx_noeud_fiche_ovale_zoom_sdl_opengl *nfo = (alx_noeud_fiche_ovale_zoom_sdl_opengl*)param;
 pt.x -= nfo->Longueur_corp() / 2;
 pt.y -= nfo->Hauteur_corp() / 2;
}

//______________________________________________________________________________
void alx_noeud_fiche_ovale_zoom_sdl_opengl::Rationnaliser_buffer_reperes(void *param)
{Maj_buffer_reperes();
}
//______________________________________________________________________________
alx_noeud_fiche_ovale_zoom_sdl_opengl::alx_noeud_fiche_ovale_zoom_sdl_opengl( const char *titre, const double tx, const double ty
                                                                , alx_interface_sdl_opengl_1 *interface_m) : alx_noeud_fiche_ovale_sdl_opengl( titre, tx, ty
                                                                                                                                             , interface_m )
{Init_buffer_reperes();

 interface_m->abonner_a_simulateur( FONCTION_DE_RAPPEL( alx_noeud_fiche_ovale_zoom_sdl_opengl
                                                      ,&alx_noeud_fiche_ovale_zoom_sdl_opengl::Rationnaliser_buffer_reperes)
                                  );

 Position_dans_liste_des_fils_changeable( alx_noeud_scene::Position_dans_liste_des_fils_changeable() );
 Compter_affichage(false);
 Compter_pre_rendu(true);
 Nb_max_pre_rendu(1);
 //Nb_max_affichage(1);
 Gerer_corps(false); Afficher_corps(false);
 Voir_si_pt_dans_fils(false);
 Bord_translucide( true );
 Translucidite_bord( 0.4 );
 Forcer_a_ne_plus_empiler_contenant( true );
 Mettre_a_jour();

 reallouer_lentille    = true;
 recalculer_coord_text = true;
 rationnalise_foz = false;

 alx_noeud_scene::Vider_fils();

 alx_noeud_scene::Ajouter_fils( (alx_noeud_scene*)(&noeud_rep_lentille) );
 noeud_rep_lentille.Ajouter_fils( (alx_noeud_scene*)(&repere_des_fils) );
 rep_lentille = &(noeud_rep_lentille.rep_lentille);

 nb_pt_bord = 0;
 bordure    = (double*)NULL;

 noeud_rep_lentille.Fonction_ajustement_point(fct_ajust, this);

 Afficher_lignes(false);

// On donne une des courbes de lentilles pré-fournie
 info_courbe_lentille info_lentille;
 Courbe_lentille_bombe( info_lentille );
 rep_lentille->Courbe_unite( info_lentille.Tab()
                           , info_lentille.Nb() );
 alx_noeud_fiche_ovale_zoom_sdl_opengl::maj_lentille();

 fiche->desabonner_de_dimension( nfo_rappel_chgmnt_dim );
 fiche->abonner_a_dimension( FONCTION_DE_RAPPEL( alx_noeud_fiche_ovale_zoom_sdl_opengl
                                               ,&alx_noeud_fiche_ovale_zoom_sdl_opengl::Rationnaliser_chgmnt_dim) );
 Mettre_a_jour();
}

//______________________________________________________________________________
void alx_noeud_fiche_ovale_zoom_sdl_opengl::Rationnaliser_chgmnt_dim(void *param)
{if(rationnalise_foz) return;
 rationnalise_foz = true;
   alx_noeud_fiche_ovale_sdl_opengl::Rationnaliser_chgmnt_dim(param);
   recalculer_coord_text = true;
 rationnalise_foz = false;
 }

//______________________________________________________________________________
alx_noeud_fiche_ovale_zoom_sdl_opengl::~alx_noeud_fiche_ovale_zoom_sdl_opengl()
{}

//______________________________________________________________________________
info_du_contenant* alx_noeud_fiche_ovale_zoom_sdl_opengl::Noeud_contient(const alx_point2D &pt, int action)
{if( Appartenance_deja_testee()
   ||(zdr_corp.Infos_texture_tempon(0)->tx == 0)
   ||(zdr_corp.Infos_texture_tempon(0)->ty == 0) ) return (info_du_contenant*)NULL;

 alx_noeud_scene::Position_dans_liste_des_fils_changeable( Position_dans_liste_des_fils_changeable() );
 if( !Texture_tempon_prete()
   &&!Rendu_ecran_direct() ) return NULL;

 //Noeud_de_coupe(true);

 A_pt_tmp.maj(pt);
 info_du_contenant *rep = alx_noeud_fiche_ovale_sdl_opengl::Noeud_contient(A_pt_tmp, action);
 if( (!Contact_avec_corp()) || (rep==(info_du_contenant*)NULL))
  {Appartenance_deja_testee(false);
   return rep;}

 return rep;
}

//______________________________________________________________________________
info_du_contenant* alx_noeud_fiche_ovale_zoom_sdl_opengl::Contenu_dans_fils(alx_point2D &pt, int action)
{if( Appartenance_deja_testee() )
   return (info_du_contenant*)NULL;
 Appartenance_deja_testee(true);
 //Limite( (alx_noeud_scene*)this );
 //Maj_buffer_reperes();
   if(Est_modifier_action_type()) action=Modification_action_type();
   info_du_contenant *rep2 = alx_noeud_fiche_ovale_sdl_opengl::Contenu_dans_fils(pt, action);
 //Limite( (alx_noeud_scene*)NULL );

 Appartenance_deja_testee(false);

 return rep2;
}

//______________________________________________________________________________
void alx_noeud_fiche_ovale_zoom_sdl_opengl::Init_buffer_reperes()
{for(unsigned int i=0; i<NB_MAX_FILS; i++)
  {buffer_reperes[i][0] = (alx_noeud_scene*)NULL;
   buffer_reperes[i][1] = (alx_noeud_scene*)NULL;
  }
}
//______________________________________________________________________________
void alx_noeud_fiche_ovale_zoom_sdl_opengl::Maj_buffer_reperes()
{for(unsigned int i=0; i<NB_MAX_FILS; i++)
  {if(buffer_reperes[i][0] && buffer_reperes[i][1]) {
     buffer_reperes[i][1]->maj( *(buffer_reperes[i][0]) );
     buffer_reperes[i][1]->Inverser_coordonnees_brut();
     buffer_reperes[i][1]->Ordre_rendu(1);
     buffer_reperes[i][1]->A_changer(false);
    }
  }
}

//______________________________________________________________________________
void alx_noeud_fiche_ovale_zoom_sdl_opengl::PreRendre()
{// On a à faire à l'un des fils, c'est à dire la scène en dessous, il ne faut
 // donc pas passer au premier plan, quoi qu'il arrive :
 alx_noeud_scene::Position_dans_liste_des_fils_changeable( false );
 alx_repere2D *rep_contenu = Repere_du_contenu();
 rep_contenu->maj( *this );
 rep_contenu->Inverser_coordonnees_brut();
 rep_contenu->Ordre_rendu(1);

 Maj_buffer_reperes();
 Afficher_noeud(false);
   Limite( (alx_noeud_scene*)this );
   //Limite_nb_recursions( 1 );
     alx_noeud_fiche_ovale_sdl_opengl::PreRendre();
     Avorter(false);
   Limite( (alx_noeud_scene*)NULL );
 Afficher_noeud(true);

 if( zdr_corp.Texture_recree()
   ||recalculer_coord_text )
  {recalculer_coord_text = false;
   maj_lentille();
   Mettre_a_jour();}
}

//______________________________________________________________________________
void alx_noeud_fiche_ovale_zoom_sdl_opengl::maj_lentille()
{// On redéfinie la lentille à partir des informations de la fenêtre
 A_changer(true);
 if( (zdr_corp.Infos_texture_tempon(0)->tx == 0)
   ||(zdr_corp.Infos_texture_tempon(0)->ty == 0) ) return;

 double centre[4]
      , coord_x = zdr_corp.Nb_unite_x()*zdr_corp.Nb_pixels_par_unite()/zdr_corp.Infos_texture_tempon(0)->tx
      , coord_y = zdr_corp.Nb_unite_y()*zdr_corp.Nb_pixels_par_unite()/zdr_corp.Infos_texture_tempon(0)->ty
      , l = Longueur_corp()
      , h = Hauteur_corp();

 // On fixe les niveaux de détail.
 rep_lentille->Niveaux_details(Tab_niveaux_details(1), 1);

 rep_lentille->Nb_pix_par_unite( Nb_pixels_par_unite() );
 // On fixe les coordonnées effectives des texture utilisées:
 rep_lentille->Dimensions_effectives_texture(coord_x, coord_y);
 rep_lentille->Taille_effective_texture( zdr_corp.Infos_texture_tempon(0)->tx
                                       , zdr_corp.Infos_texture_tempon(0)->ty);

 // Définition du centre
 centre[0] = 0;
 centre[1] = 0;
 centre[2] = coord_x/2;
 centre[3] = coord_y/2;
 rep_lentille->Centre( centre );

 // Définission de la bordure
 if(nb_pt_bord != fiche_gl->Nb_sommets())
  {if(bordure)
    {delete bordure;}
   bordure = new double[4*fiche_gl->Nb_sommets()];
   nb_pt_bord = fiche_gl->Nb_sommets();
  }

 fiche_gl->Maj_coord_text(coord_x, coord_y);
 unsigned int pos = 0;
 double tx = l / 2
      , ty = h / 2;
 alx_sommet_opengl *tab = fiche_gl->Pointeur_sur_coord_corp()->tab_sommets;
 for(unsigned int i=1; i<=nb_pt_bord; i++)
  {bordure[pos++] = tab[i].coord_espace[0] * tx;
   bordure[pos++] = tab[i].coord_espace[1] * ty;
   bordure[pos++] = tab[i].coord_texture[0];
   bordure[pos++] = tab[i].coord_texture[1];
  }
/* bordure[0] =0; bordure[1] =0; bordure[2] =0;       bordure[3] =0;
 bordure[4] =l; bordure[5] =0; bordure[6] =coord_x; bordure[7] =0;
 bordure[8] =l; bordure[9] =h; bordure[10]=coord_x; bordure[11]=coord_y;
 bordure[12]=0; bordure[13]=h; bordure[14]=0;       bordure[15]=coord_y;  */
 rep_lentille->Bordure(bordure, nb_pt_bord);

 // Finalement on re-génère la lentille:
// if(reallouer_lentille)
  {rep_lentille->Effacer_lentille();
   rep_lentille->Allouer_lentille();
   rep_lentille->Effacer_ordre_affichage();
   rep_lentille->Allouer_ordre_affichage();
//   reallouer_lentille = false;
   rep_lentille->Generer_ordre_affichage();
   }

 rep_lentille->Generer_lentille();
}

//______________________________________________________________________________
void alx_noeud_fiche_ovale_zoom_sdl_opengl::Dessin_noeud()
{if(!Texture_tempon_prete()) return;
 glPushMatrix();
   alx_noeud_fiche_ovale_sdl_opengl::Dessin_noeud();
 glPopMatrix();

     const double *tab_sommets   = rep_lentille->Couche_sommets_centrale()
                , *tab_texcoords = rep_lentille->Couche_texcoords_centrale();
     const unsigned int *ordre_int = rep_lentille->Ordre_couche_centrale()
                      , *ordre_ext = rep_lentille->Ordre_couche_niveau();


 glColor4f(1,1,1,1);
 glPushAttrib(GL_ENABLE_BIT);

// glPushMatrix();
 //glLoadIdentity();
   // Affichage du corp de la lentille :
   unsigned int i;
   glEnable(GL_TEXTURE_2D);

       if( Corp_translucide() )
        {glEnable(GL_BLEND);
         glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
         glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);}
        else glDisable(GL_BLEND);

   glEnableClientState( GL_VERTEX_ARRAY );
   glEnableClientState( GL_TEXTURE_COORD_ARRAY );

   for(i=0;i<zdr_corp.Nb_zooms();i++)
    {glBindTexture(GL_TEXTURE_2D, zdr_corp.Infos_texture_tempon(0)->id_texture);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
     if( Lisser() )
      {glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);}
      else
       {glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);}
     glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
     glVertexPointer  (2, GL_DOUBLE, 0, rep_lentille->Couche_sommets_niveau(i) );
     glTexCoordPointer(2, GL_DOUBLE, 0, rep_lentille->Couche_texcoords_niveau(i) );

     glDrawElements( GL_TRIANGLE_STRIP
                   , rep_lentille->Nb_ordre_couche_niveau(i)
                   , GL_UNSIGNED_INT
                   , ordre_ext);
     if(Afficher_lignes())
      {glPushAttrib(GL_POLYGON_BIT);
       glPushAttrib(GL_CURRENT_BIT);
       glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
       glColor4f(0,0,0,0);
       glDisableClientState( GL_TEXTURE_COORD_ARRAY );
       glDrawElements( GL_TRIANGLE_STRIP
                     , rep_lentille->Nb_ordre_couche_niveau(i)
                     , GL_UNSIGNED_INT
                     , ordre_ext);
       glEnableClientState( GL_TEXTURE_COORD_ARRAY );
       glPopAttrib();
       glPopAttrib();
      }
    }

           /*glDisable(GL_TEXTURE_2D);
           glColor4f(0,0,0,0);
           glDisableClientState( GL_TEXTURE_COORD_ARRAY );
             glDrawElements( GL_TRIANGLE_STRIP
                           , rep_lentille->Nb_ordre_couche_niveau(i-1)
                           , GL_UNSIGNED_INT
                           , ordre_ext);
           glEnable(GL_TEXTURE_2D);
           glEnableClientState( GL_TEXTURE_COORD_ARRAY );
           glColor4f(1,1,1,1);   */

   glVertexPointer  (2, GL_DOUBLE, 0, tab_sommets );
   glTexCoordPointer(2, GL_DOUBLE, 0, tab_texcoords );
   glDrawElements( GL_TRIANGLE_FAN
                 , rep_lentille->Nb_ordre_couche_centrale()
                 , GL_UNSIGNED_INT
                 , ordre_int );

   glDisableClientState( GL_VERTEX_ARRAY | GL_TEXTURE_COORD_ARRAY );

 glPopAttrib();
}

//______________________________________________________________________________
const int alx_noeud_fiche_ovale_zoom_sdl_opengl::Trouver_buffer_fils(alx_noeud_scene *n) {
 for(unsigned int i=0; i<NB_MAX_FILS; i++)
  {if(buffer_reperes[i][0] == n) {return i;}
  }
 return -1;
}
//______________________________________________________________________________
const int alx_noeud_fiche_ovale_zoom_sdl_opengl::Desintercaller_noeud_zoom(alx_noeud_scene *e)
{int pos = Trouver_buffer_fils(e);
 if(pos == -1) return pos;
   buffer_reperes[pos][1]->Retirer_et_relier();
   buffer_reperes[pos][0] = (alx_noeud_scene*)NULL;
 return pos;
}
//______________________________________________________________________________
const int alx_noeud_fiche_ovale_zoom_sdl_opengl::Intercaller_noeud_zoom(alx_noeud_scene *e)
{int pos = Trouver_buffer_fils(NULL);
 if(pos == -1) return pos;
 if(buffer_reperes[pos][1] == (alx_noeud_scene*)NULL) {buffer_reperes[pos][1] = new alx_noeud_scene();}
 buffer_reperes[pos][1]->Intercaler_noeud_entre(Noeud_repere_fils(), e);
 buffer_reperes[pos][0] = e;
 return pos;
}
//______________________________________________________________________________
// Les surchgarges de la classe alx_arbre
bool alx_noeud_fiche_ovale_zoom_sdl_opengl::A_pour_fils(alx_noeud_scene *e)
{for(unsigned int i=0; i<NB_MAX_FILS; i++)
  {if(buffer_reperes[i][0] == e) return true;}
 return false;
}
//______________________________________________________________________________
void alx_noeud_fiche_ovale_zoom_sdl_opengl::Ajouter_fils_au_debut(alx_noeud_scene *e)
{alx_noeud_fiche_ovale_sdl_opengl::Ajouter_fils_au_debut(e);
 Intercaller_noeud_zoom(e);
}
//______________________________________________________________________________
void alx_noeud_fiche_ovale_zoom_sdl_opengl::Ajouter_fils_a_la_fin(alx_noeud_scene *e)
{alx_noeud_fiche_ovale_sdl_opengl::Ajouter_fils_a_la_fin(e);
 Intercaller_noeud_zoom(e);
}
//______________________________________________________________________________
void alx_noeud_fiche_ovale_zoom_sdl_opengl::Ajouter_fils_replique(alx_noeud_scene *e)
{alx_noeud_fiche_ovale_sdl_opengl::Ajouter_fils_replique(e);
 Intercaller_noeud_zoom(e);
}
//______________________________________________________________________________
void alx_noeud_fiche_ovale_zoom_sdl_opengl::Ajouter_fils(alx_noeud_scene *e)
{alx_noeud_fiche_ovale_sdl_opengl::Ajouter_fils(e);
 Intercaller_noeud_zoom(e);
}
//______________________________________________________________________________
void alx_noeud_fiche_ovale_zoom_sdl_opengl::Ajouter_fils_avant(alx_noeud_scene *noeud, alx_noeud_scene *noeud_fils)
{alx_noeud_fiche_ovale_sdl_opengl::Ajouter_fils_avant(noeud, noeud_fils);
 Intercaller_noeud_zoom(noeud);
}
//______________________________________________________________________________
void alx_noeud_fiche_ovale_zoom_sdl_opengl::Ajouter_fils(alx_noeud_scene *e, alx_element_liste<alx_arbre<alx_noeud_scene*>*> &it)
{alx_noeud_fiche_ovale_sdl_opengl::Ajouter_fils_it(e, it);
 Intercaller_noeud_zoom(e);
}
//______________________________________________________________________________
void alx_noeud_fiche_ovale_zoom_sdl_opengl::Retirer_fils(alx_noeud_scene *e)
{if(e) Desintercaller_noeud_zoom(e);
 alx_noeud_fiche_ovale_sdl_opengl::Retirer_fils(e);
}
//______________________________________________________________________________
void alx_noeud_fiche_ovale_zoom_sdl_opengl::Retirer_fils(alx_element_liste<alx_arbre<alx_noeud_scene*>*> &it)
{Desintercaller_noeud_zoom(it.E()->e);
 alx_noeud_fiche_ovale_sdl_opengl::Retirer_fils_it(it);
}
//______________________________________________________________________________
void alx_noeud_fiche_ovale_zoom_sdl_opengl::Vider_fils()
{for(unsigned int i=0; i<NB_MAX_FILS; i++)
  {if(buffer_reperes[i][1]) {buffer_reperes[i][1]->Vider_fils();}
   buffer_reperes[i][0] = (alx_noeud_scene*)NULL;
  }
 alx_noeud_fiche_ovale_sdl_opengl::Vider_fils();
}

/*
info_du_contenant *rep

class alx_sommet_opengl
{
 public:
   GLfloat coord_espace[3];
   GLfloat coord_texture[2];
   GLfloat coord_couleur[4];
};



// Calcul des points de la matrice pour le cas d'un rectangle déformé
nd      : niveaux de détails
finesse : la finesse de calcul de l'interpollation de la courbe déformante
 OU Finesse[nd] : pareil mais avec des différences pour chaque niveau de détails
                  util car sur les bords on en aura besoin de moins.
tx, ty  : dimensions du rectangle
c       : le centre du rectangle
p       : points en cour de calcul, intersection de la droite partant du centre
        , d'angle alpha, avec la courbe
e.[l|h] : les dimensions (type rayon) de la pseudo-ellipse.

matrice[4*nd*finesse] : Contient tous les points
                      , ainsi que leurs couleurs et coordonnées de texture
Finasserie_x[nd*finesse] : les valeurs associées aux balayage par angle
                           pour l'axe des abscisses
Finasserie_y[nd*finesse] : les valeurs associées aux balayage par angle
                           pour l'axe des ordonnées

Pour i allant de 0 à nd*finesse
  d = droite partnat de c et d'angle i*PI/finesse avec l'axe de abscisses.
  p = intersection de d avec l'ellpise e, ordonné positive
  Finasserie_x[i] = p.x
  Finasserie_y[i] = p.x*h/l

// Gestion des niveaux de détails, racalibrage des textures:
// Les frontières peuvent être placées soit :
//   - aux multiples de nd dans les Finasseries, c'est faciles!
//   - à partir du moment où la texture en cours d'utilisations n'est plus valide
//     on passe alors à la texture plus générale, plus dur!!!!

// Notons les coordonnées des textures de détails par rapport à la taille originale
// Niveau ; Intervalle
//      1 ; [0    ; 1    ]
//      2 ; [0,25 ; 0,75 ]
//      3 ; [0,375; 0,625]
//.....
//      n ; [0,5-(0,5)^n; 0,5+(0,5)^n]

// Mise en correspondance des coordonnées:
//   On se base sur les coordonnées de niveau 1 pour la correspondance
//   et la détection des frontières.
//   le découpage doit alors être au minimum fait par pas de
//      (0,5)^n = 1/(2^n) soit 2^n
//   La finesse de découpage entre chaque tranche peut être multipliée par 2
//   à chaque étape, par exemple.
*/


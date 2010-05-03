#include "alx_noeud_fiche_zoom_sdl_opengl.h"
#include "alx_interface_sdl_opengl_1.h"
#include "..\classiques\alx_fiche.h"

alx_noeud_fiche_zoom_sdl_opengl::alx_noeud_fiche_zoom_sdl_opengl( const char *titre, const double tx, const double ty
                                                                , alx_interface_sdl_opengl_1 *interface_m) : alx_noeud_fiche_sdl_opengl( titre, tx, ty
                                                                                                                                       , interface_m )
{Position_dans_liste_des_fils_changeable( alx_noeud_scene::Position_dans_liste_des_fils_changeable() );
 Compter_affichage(false);
 Compter_pre_rendu(true);
 Nb_max_pre_rendu(1);
 //Nb_max_affichage(1);
 Gerer_corps(false); Afficher_corps(false);
 Bord_translucide( true );
 Translucidite_bord( 0.4 );
 Mettre_a_jour();

 rationnalise_fz = false;
 reallouer_lentille = true;

 alx_noeud_scene::Vider_fils();

 alx_noeud_scene::Ajouter_fils( (alx_noeud_scene*)(&noeud_rep_lentille) );
 noeud_rep_lentille.Ajouter_fils( (alx_noeud_scene*)(&repere_des_fils) );
 rep_lentille = &(noeud_rep_lentille.rep_lentille);

// noeud_rep_lentille.Fonction_ajustement_point(fct_ajust, this);

 recalculer_coord_text = true;
 maj_lentille();
 fiche->desabonner_de_dimension( nf_rappel_chgmnt_dim );
 fiche->abonner_a_dimension( FONCTION_DE_RAPPEL( alx_noeud_fiche_zoom_sdl_opengl
                                               ,&alx_noeud_fiche_zoom_sdl_opengl::Rationnaliser_chgmnt_dim) );
}

//______________________________________________________________________________
void alx_noeud_fiche_zoom_sdl_opengl::Rationnaliser_chgmnt_dim(void *param)
{if(rationnalise_fz) return;
 rationnalise_fz = true;
   alx_noeud_fiche_sdl_opengl::Rationnaliser_chgmnt_dim(param);
   recalculer_coord_text = true;
 rationnalise_fz = false;
 }

//______________________________________________________________________________
alx_noeud_fiche_zoom_sdl_opengl::~alx_noeud_fiche_zoom_sdl_opengl()
{}

//______________________________________________________________________________
info_du_contenant* alx_noeud_fiche_zoom_sdl_opengl::Contient(alx_point2D &pt, int action)
{if( Appartenance_deja_testee() ) return NULL;
 alx_noeud_scene::Position_dans_liste_des_fils_changeable( Position_dans_liste_des_fils_changeable() );
 if( !Texture_tempon_prete()
   &&!Rendu_ecran_direct() ) return NULL;
 Appartenance_deja_testee(true);

 Noeud_de_coupe(true);

 info_du_contenant *rep = alx_noeud_fiche_sdl_opengl::Contient(pt, action);
 if( (!Contact_avec_corp()) || (rep==NULL))
  {Appartenance_deja_testee(false);
   return rep;}

 // On a à faire à l'un des fils, c'est à dire la scène en dessous, il ne faut
 // donc pas passer au premier plan, quoi qu'il arrive :
 alx_noeud_scene::Position_dans_liste_des_fils_changeable( false );
 // On a eut un contact avec le corp, on peut chercher à l'intérieur peut être?
 // Allé hop on va voir du coté du fiston...
 A_pt_tmp.maj(pt); //alx_point2D pt2(pt);
 Changer_coordonnees(A_pt_tmp);//pt2);
 Limite( (alx_noeud_scene*)this );
 if(Est_modifier_action_type()) action=Modification_action_type();
 info_du_contenant *rep2 = noeud_rep_lentille.Contient(/*pt2*/A_pt_tmp, action);
 Limite( (alx_noeud_scene*)NULL );

 Appartenance_deja_testee(false);
 if(rep2)
  {if(rep && (rep != Rep_puit()))
     delete rep;
   rep2->L_reperes.Ajouter_au_debut( (alx_repere2D*)this );
   //Ajouter_appels(rep2, 2, L_rappel_pointeur_detecte); //XXX
   return rep2;}

 return rep;
}

//______________________________________________________________________________
void alx_noeud_fiche_zoom_sdl_opengl::PreRendre()
{Rotation_du_contenu( -Rotation() );
 A_pt_tmp.maj(-Origine().x, -Origine().y); //alx_point2D pt(-Origine().x, -Origine().y);
 /*pt*/A_pt_tmp.Pivoter( -Rotation() );
 Origine_du_contenu( A_pt_tmp/*pt*/ );

// IMPORTE DE FICHE_OVALE
// On fixe l'origine :
// Origine_du_contenu( A_pt_tmp ); //pt );
 if( !Noeud_repere_fils()->L_fils.Vide() ) // On a un fils, on applique la transfo du fils
  {alx_repere2D *nf = Noeud_repere_fils()->L_fils.Premier_E()->e
              , *ni = Repere_du_contenu();
   const alx_point2D &pof = nf->Origine();  // Point Origine du Fils
   ni->Rotation( ni->Rotation()-nf->Rotation() );
   ni->Translation_interne(-pof.x, -pof.y);
  }
//________________________

 Afficher_noeud(false);
   Limite( (alx_noeud_scene*)this );
     alx_noeud_fiche_sdl_opengl::PreRendre();
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
void alx_noeud_fiche_zoom_sdl_opengl::maj_lentille()
{// On redéfinie la lentille à partir des informations de la fenêtre
 double centre[4]
      , coord_x = 0
      , coord_y = 0
      , bordure[4*4] // 4 points de 4 coordonnées chacun
      , l = Longueur_corp()
      , h = Hauteur_corp();

 if (  zdr_corp.Infos_texture_tempon(0)->tx > 0
    && zdr_corp.Infos_texture_tempon(0)->ty > 0) {
   coord_x = zdr_corp.Nb_unite_x()*zdr_corp.Nb_pixels_par_unite()/zdr_corp.Infos_texture_tempon(0)->tx;
   coord_y = zdr_corp.Nb_unite_y()*zdr_corp.Nb_pixels_par_unite()/zdr_corp.Infos_texture_tempon(0)->ty;
  }
 // On fixe les niveaux de détail.
 if(Tab_niveaux_details(1))
   rep_lentille->Niveaux_details(Tab_niveaux_details(1), 1);

 // On fixe les coordonnées effectives des texture utilisées:
 rep_lentille->Dimensions_effectives_texture(coord_x, coord_y);
 rep_lentille->Taille_effective_texture( zdr_corp.Infos_texture_tempon(0)->tx
                                       , zdr_corp.Infos_texture_tempon(0)->ty);

 // Définition du centre
 centre[0] = l/2;
 centre[1] = h/2;
 centre[2] = coord_x/2;
 centre[3] = coord_y/2;
 rep_lentille->Centre( centre );

 // Définission de la bordure
 bordure[0] =0; bordure[1] =0; bordure[2] =0;       bordure[3] =0;
 bordure[4] =l; bordure[5] =0; bordure[6] =coord_x; bordure[7] =0;
 bordure[8] =l; bordure[9] =h; bordure[10]=coord_x; bordure[11]=coord_y;
 bordure[12]=0; bordure[13]=h; bordure[14]=0;       bordure[15]=coord_y;
 rep_lentille->Bordure(bordure, 4);

 // On donne une des courbes de lentilles pré-fournie
 info_courbe_lentille info_lentille;
 Courbe_lentille_bombe( info_lentille );
 rep_lentille->Courbe_unite( info_lentille.Tab()
                           , info_lentille.Nb() );

 // Finalement on re-génère la lentille:
 if(reallouer_lentille)
  {rep_lentille->Effacer_lentille();
   rep_lentille->Allouer_lentille();
   rep_lentille->Effacer_ordre_affichage();
   rep_lentille->Allouer_ordre_affichage();
   reallouer_lentille = false;
   rep_lentille->Generer_ordre_affichage();
   }

 rep_lentille->Generer_lentille();
}

//______________________________________________________________________________
void alx_noeud_fiche_zoom_sdl_opengl::Dessin_noeud()
{if(!Texture_tempon_prete()) return;
 alx_noeud_fiche_sdl_opengl::Dessin_noeud();

     const double *tab_sommets   = rep_lentille->Couche_sommets_centrale()
                , *tab_texcoords = rep_lentille->Couche_texcoords_centrale();
     const unsigned int *ordre_int = rep_lentille->Ordre_couche_centrale()
                      , *ordre_ext = rep_lentille->Ordre_couche_niveau();


 glColor4f(1,1,1,1);
 glPushAttrib(GL_ENABLE_BIT);

   // Affichage du corp de la lentille :
   unsigned int i;
   glEnable(GL_TEXTURE_2D);
   glEnableClientState( GL_VERTEX_ARRAY );
   glEnableClientState( GL_TEXTURE_COORD_ARRAY );


//   glLineWidth(2);
   for(i=0;i<zdr_corp.Nb_zooms();i++)
    {glBindTexture(GL_TEXTURE_2D, zdr_corp.Infos_texture_tempon(0)->id_texture);
     glVertexPointer  (2, GL_DOUBLE, 0, rep_lentille->Couche_sommets_niveau(i) );
     glTexCoordPointer(2, GL_DOUBLE, 0, rep_lentille->Couche_texcoords_niveau(i) );
     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
     glDrawElements( GL_TRIANGLE_STRIP
                   , rep_lentille->Nb_ordre_couche_niveau(i)
                   , GL_UNSIGNED_INT
                   , ordre_ext);
     /*glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
     glDrawElements( GL_TRIANGLE_STRIP
                   , rep_lentille->Nb_ordre_couche_niveau(i)
                   , GL_UNSIGNED_INT
                   , ordre_ext);*/
    }
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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


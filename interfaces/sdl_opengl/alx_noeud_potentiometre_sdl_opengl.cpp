#include "alx_noeud_potentiometre_sdl_opengl.h"

#include "alx_interface_sdl_opengl_1.h"
#include "../../opengl/config_opengl.h"
#include "../choses_communes.h"
#include "../../physique/math_alex.cpp"

const unsigned int alx_noeud_potentiometre_sdl_opengl::NB_ELEMENTS = 4;

void alx_noeud_potentiometre_sdl_opengl::maj_interne()
{
// Mise à jour des polygones
 // Polygone du fond
 alx_element_liste<alx_point2D> *it_pt;
 alx_liste<alx_point2D> *L_pt = &(mp_zone_fond.L_polygone2D.Premier_E().L_points);
 it_pt = L_pt->Premier();

 double dec_x = Longueur_boutons_controles() / 2
      , x = Longueur() + Longueur_boutons_controles() + bouton_reculer->Epaisseur_bordure() + bouton_avancer->Epaisseur_bordure()
      , y = Hauteur();
 (it_pt->E()).maj(dec_x    , 0); it_pt=it_pt->svt;
 (it_pt->E()).maj(dec_x + x, 0); it_pt=it_pt->svt;
 (it_pt->E()).maj(dec_x + x, y); it_pt=it_pt->svt;
 (it_pt->E()).maj(dec_x    , y);

 // Polygone pour le réglage
 L_pt = &(mp_zone_reglage.L_polygone2D.Premier_E().L_points);
 it_pt = L_pt->Premier();
 x  = Longueur() / Amin(NbDivisions(), 20);
 double lg = Longueur() - x;
 dec_x = Longueur_boutons_controles() + bouton_reculer->Epaisseur_bordure() + lg * Valeur() / NbDivisions();
 (it_pt->E()).maj(dec_x    , 0.0); it_pt=it_pt->svt;
 (it_pt->E()).maj(dec_x + x, 0.0); it_pt=it_pt->svt;
 (it_pt->E()).maj(dec_x + x, y);   it_pt=it_pt->svt;
 (it_pt->E()).maj(dec_x    , y);

// Redimensionnement des boutons
 double ah = bouton_reculer->Hauteur()
      , al = bouton_reculer->Longueur();
 bouton_reculer->Hauteur ( Hauteur() );
 bouton_reculer->Longueur( Longueur_boutons_controles() );
 bouton_avancer->Hauteur ( Hauteur() );
 bouton_avancer->Longueur( Longueur_boutons_controles() );
 double ex = bouton_reculer->Etirement_du_contenu().dx*bouton_reculer->Longueur()/al
      , ey = bouton_reculer->Etirement_du_contenu().dy*bouton_reculer->Hauteur()/ah;
 bouton_reculer->Etirement_du_contenu(ex, ey);
 bouton_avancer->Etirement_du_contenu(ex, ey);

// Déplacement des boutons de navigation
 bouton_avancer->Origine( Longueur()
                        + bouton_reculer->Epaisseur_bordure()
                        + bouton_avancer->Epaisseur_bordure()
                        + Longueur_boutons_controles(), 0);

// Mise à jour de l'intérieur des boutons
 bouton_reculer->Mettre_a_jour();
 bouton_avancer->Mettre_a_jour();
}

void alx_noeud_potentiometre_sdl_opengl::init()
{
// Construction des polygones
 alx_liste<alx_point2D> L_point;

 for(int i=0;i<4;i++) L_point.Ajouter_a_la_fin( Pt_nul() );

 mp_zone_reglage.Ajouter_polygone2D(L_point, 1.0);
 mp_zone_fond.Ajouter_polygone2D(L_point, 1.0);

 for(int i=0;i<4;i++)
  {gl_zone_reglage[i].coord_espace[2] = 0;
   gl_zone_fond[i].coord_espace[2]    = 0;}

// Définition des zones de contact
 alx_simulateur_points_contact *sim_contact = interface_mere->sim_contact;

 ens_contact_zone_redim   = sim_contact->Ajouter_ensemble_contact();
 Ajouter_fils( &noeud_zone_reglage );
 mod_contact_zone_reglage = sim_contact->Ajouter_modele( ens_contact_zone_redim
                                                       , &mp_zone_reglage
                                                       , &noeud_zone_reglage
                                                       , alx_point_contact_capacite_translation
                                                        |alx_point_contact_capacite_rapporteur );
// mod_contact_zone_reglage->noeud_du_modele = NULL;
// mod_contact_zone_reglage->modele_physique = &mp_zone_reglage;

// Liaison des objets pour la rationnalisation
 bouton_reculer->Abonner_a((alx_classe_base_liant*)this, 0);
 bouton_avancer->Abonner_a((alx_classe_base_liant*)this, 1);
 mp_zone_fond.abonner_a((alx_classe_base_liant*)this, 2);
 mp_zone_reglage.abonner_a((alx_classe_base_liant*)this, 3);

 T_modeles_bases = new P_alx_classe_base_model[NB_ELEMENTS];
 T_modeles_bases[0] = (P_alx_classe_base_model)(bouton_reculer);
 T_modeles_bases[1] = (P_alx_classe_base_model)(bouton_avancer);
 T_modeles_bases[2] = (P_alx_classe_base_model)(&mp_zone_fond);
 T_modeles_bases[3] = (P_alx_classe_base_model)(&mp_zone_reglage);

 T_rationnalisation_global = new pf[4];
 T_rationnalisation_global[0]   = &alx_noeud_potentiometre_sdl_opengl::Rationnaliser_Bouton_prcdt;
 T_rationnalisation_global[1]   = &alx_noeud_potentiometre_sdl_opengl::Rationnaliser_Bouton_svt;
 T_rationnalisation_global[2]   = &alx_noeud_potentiometre_sdl_opengl::Rationnaliser_zone_fond;
 T_rationnalisation_global[3]   = &alx_noeud_potentiometre_sdl_opengl::Rationnaliser_zone_reglage;

// Mise à jour interne
 maj_interne();
}

alx_noeud_potentiometre_sdl_opengl::alx_noeud_potentiometre_sdl_opengl( alx_interface_sdl_opengl_1 *interface_mere
                                                                      , const double lg, const double ht
                                                                      , const double lg_boutons_controles
                                                                      , const unsigned int type_bouton) : alx_noeud_scene()
{
 this->interface_mere = interface_mere;

 if(lg_boutons_controles < 0)
   Longueur_boutons_controles(ht);
  else Longueur_boutons_controles(lg_boutons_controles);
 Longueur(lg);
 Hauteur (ht);
 Valeur(0);
 Valeur_min(0);
 Valeur_max(100);
 Valeur_saut(10);
 Couleur_zone_reglage(0.6, 1, 0.8, 1);
 Couleur_fond(0.6, 0.55, 0.2, 1);

// Création des boutons de navigation
 Initialiser_boutons();
 bouton_reculer = new alx_noeud_bouton_sdl_opengl(*Image_bouton_dec(), interface_mere, interface_mere->Noeud_scene(), type_bouton);
 bouton_avancer = new alx_noeud_bouton_sdl_opengl(*Image_bouton_inc(), interface_mere, interface_mere->Noeud_scene(), type_bouton);

 Ajouter_fils( (alx_noeud_scene*)bouton_reculer );
 Ajouter_fils( (alx_noeud_scene*)bouton_avancer );

 init();
}

alx_noeud_potentiometre_sdl_opengl::~alx_noeud_potentiometre_sdl_opengl()
{delete bouton_reculer;
 delete bouton_avancer;}

//______________________________________________________________________________
//______________________________Les méthodes____________________________________
//______________________________________________________________________________
void alx_noeud_potentiometre_sdl_opengl::Dessin_noeud()
{// Se placer dans le repère du potentiomètre:
// glPushMatrix();
// Changement_repere( *this );

 // Afficher le fond
 alx_element_liste<alx_point2D> *it_pt_corp;
 int i;

 it_pt_corp   = mp_zone_fond.L_polygone2D.Premier_E().L_points.Premier();
 for(i=0;i<4;i++)
  {gl_zone_fond[i].coord_espace[0] = (it_pt_corp->E()).x;
   gl_zone_fond[i].coord_espace[1] = (it_pt_corp->E()).y;
   it_pt_corp = it_pt_corp->svt;}

 tracer_polygone_couleur(gl_zone_fond, 4);
 double epaisseur = bouton_reculer->Epaisseur_bordure()
       , lg  = Longueur()
       , lgb = Longueur_boutons_controles();

 glBegin(GL_TRIANGLE_STRIP);
   glColor4dv( bouton_reculer->Couleur_bordure_ext() );
   glVertex2d(lgb*0.5, 0);     glVertex2d(lgb*0.5, -epaisseur);
   glColor4dv( bouton_avancer->Couleur_bordure_ext() );
   glVertex2d(1.5*lgb+lg, 0); glVertex2d(1.5*lgb+lg, -epaisseur);
 glEnd();
 glTranslated(0, Hauteur()+epaisseur, 0);
 glBegin(GL_TRIANGLE_STRIP);
   glColor4dv( bouton_reculer->Couleur_bordure_ext() );
   glVertex2d(lgb*0.5, 0);     glVertex2d(lgb*0.5, -epaisseur);
   glColor4dv( bouton_avancer->Couleur_bordure_ext() );
   glVertex2d(1.5*lgb+lg, 0); glVertex2d(1.5*lgb+lg, -epaisseur);
 glEnd();
 glTranslated(0, -Hauteur()-epaisseur, 0);

 // Afficher les boutons
// bouton_reculer->Afficher();
// bouton_avancer->Afficher();

 // Afficher le régleur
 it_pt_corp   = mp_zone_reglage.L_polygone2D.Premier_E().L_points.Premier();
 for(i=0;i<4;i++)
  {gl_zone_reglage[i].coord_espace[0] = (it_pt_corp->E()).x;
   gl_zone_reglage[i].coord_espace[1] = (it_pt_corp->E()).y;
   it_pt_corp = it_pt_corp->svt;}

 tracer_polygone_couleur(gl_zone_reglage, 4);

 // Dépiler le repère du potentiomètre
// glPopMatrix();
}

//______________________________________________________________________________
info_du_contenant* alx_noeud_potentiometre_sdl_opengl::Noeud_contient(const alx_point2D &pt, int action)
{//alx_point2D pt2(pt);
 pt2.maj(pt);
// Changer_coordonnees(pt2);

 if( (pt2.x >= 0) && (pt2.x <= (Longueur() + 2*Longueur_boutons_controles()+bouton_reculer->Epaisseur_bordure()+bouton_avancer->Epaisseur_bordure()))
   &&(pt2.y >= -bouton_reculer->Epaisseur_bordure()) && (pt2.y <= Hauteur()+bouton_reculer->Epaisseur_bordure()) )
  {
   info_du_contenant *rep = (info_du_contenant*)NULL;
  // Tester les boutons
   rep = bouton_reculer->Contient(pt2, action);
   if(rep) {rep->L_reperes.Ajouter_au_debut( (alx_repere2D*)this ); return rep;}
   rep = bouton_avancer->Contient(pt2, action);
   if(rep) {rep->L_reperes.Ajouter_au_debut( (alx_repere2D*)this ); return rep;}

  // Tester la zone de réglage
   if( mp_zone_reglage.Contient(pt2) )
    {rep = new info_du_contenant;
     rep->mp = &mp_zone_reglage;
     rep->noeud = (alx_noeud_scene*)(&noeud_zone_reglage);
     rep->Point_au_contact(pt2);
     rep->L_reperes.Ajouter_au_debut( (alx_repere2D*)this );
     return rep;
    }

  // Tester la zone de fond
   if( mp_zone_fond.Contient(pt2) )
    {rep = new info_du_contenant;
     rep->mp = &mp_zone_fond;
     rep->noeud = (alx_noeud_scene*)this;
     rep->Point_au_contact(pt2);
     rep->L_reperes.Ajouter_au_debut( (alx_repere2D*)this );
     return rep;
    }

  // Si rien (possible si les boutons sont ovales) alors NULL
   if((pt2.x >= (Longueur_boutons_controles()/2)) && (pt2.x <= (Longueur_boutons_controles()+Longueur())))
    {return Rep_puit();}
   return NULL;
  }

 return NULL;
}


//______________________________________________________________________________
//_________________Les méthodes liées à la rationnalisation_____________________
//______________________________________________________________________________
void alx_noeud_potentiometre_sdl_opengl::Rationnaliser_Bouton_prcdt(const int *infos)
{if( infos[0]!=0
   ||infos[1]!=2 )
  return;
 Valeur( Amax(Valeur()-1, Valeur_min()) );
 maj_interne();
}

void alx_noeud_potentiometre_sdl_opengl::Rationnaliser_Bouton_svt(const int *infos)
{if( infos[0]!=0
   ||infos[1]!=2 )
  return;
 Valeur( Amin(Valeur()+1, Valeur_max()) );
 maj_interne();
}

//______________________________________________________________________________
void alx_noeud_potentiometre_sdl_opengl::Rationnaliser_zone_fond(const int *infos)
{if( infos[0]!=6
   ||infos[1]!=0 ) return;

 alx_point2D pt( *((alx_point2D*)(void*)(infos[2])) );
 alx_liste<alx_repere2D*> *L_rep = (alx_liste<alx_repere2D*>*)(void*)(infos[4]);
 Repere_transformation(pt, *L_rep);
 Changer_coordonnees(pt);
 pt.Translation(-Longueur_boutons_controles()-bouton_reculer->Epaisseur_bordure(), 0);
 int x = (int)( Valeur_min() + pt.x * NbDivisions() / (Longueur() - Longueur() / Amin(NbDivisions(), 20)) );
 if(x<=Valeur()) Valeur( Amax(Valeur()-Valeur_saut(), Valeur_min()) );
  else Valeur( Amin(Valeur()+Valeur_saut(), Valeur_max()) );
 maj_interne();
}

//______________________________________________________________________________
void alx_noeud_potentiometre_sdl_opengl::Rationnaliser_zone_reglage(const int *infos)
{if( infos[0]==6
   &&infos[1]==0 )
  {
   alx_point2D pt( *((alx_point2D*)(void*)(infos[2])) );
   alx_liste<alx_repere2D*> *L_rep = (alx_liste<alx_repere2D*>*)(void*)(infos[4]);
   Repere_transformation(pt, *L_rep);
   decalage_initiale = pt.x
                     - Longueur_boutons_controles() - bouton_reculer->Epaisseur_bordure()
                     - (Longueur() - Longueur() / Amin(NbDivisions(), 20)) * Valeur() / NbDivisions();
  }

 if( infos[0]==3
   &&infos[1]==0 )
   noeud_zone_reglage.Origine(0,0);

 if( infos[0]!=5  // Changement dans l'objet
   ||infos[1]!=1)// Une translation
  return;

 alx_vecteur2D *info_vecteur = (alx_vecteur2D*)(void*)(infos[2]);
 alx_point2D   *info_point( (alx_point2D*)(void*)(infos[3]) )
           ,   pt_arrive( *info_point );
 pt_arrive.Translation(*info_vecteur);

 noeud_zone_reglage.Changer_coordonnees_inverse(pt_arrive);
 pt_arrive.x -= Longueur_boutons_controles()
               +bouton_reculer->Epaisseur_bordure()
               +decalage_initiale;
 int nouvelle_valeur;
 nouvelle_valeur = Amax( Valeur_min()
                       , Amin( Valeur_max()
                             , (int)(Valeur_min()
                             + pt_arrive.x * NbDivisions() / (Longueur() - Longueur() / Amin(NbDivisions(), 20))) ));
 Valeur( nouvelle_valeur );
 maj_interne();
}


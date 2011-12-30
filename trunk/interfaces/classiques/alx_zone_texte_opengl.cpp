#include "alx_zone_texte_opengl.h"

//#include <extgl.h>
#include "../../physique/math_alex.cpp"

#include "../../physique/alx_model_simulation_physique1.h"
#include "../../opengl/alx_sommet_opengl.h"
#include "../../opengl/config_opengl.h"
#include "../../opengl/texte.h"
#include <sdl.h>

void alx_zone_texte_opengl::init_mp_corp()
{alx_liste<alx_point2D> L_pt;
 alx_point2D pt_tmp(0.0, 0.0);
 for(int i=0;i<4;i++) L_pt.Ajouter_a_la_fin( pt_tmp );
 mp_corp->Ajouter_polygone2D(L_pt, 1.0);
}

void alx_zone_texte_opengl::maj_mp_corp()
{alx_element_liste<alx_point2D> *it_pt;
 alx_liste<alx_point2D> *L_pt = &(mp_corp->L_polygone2D.Premier()->E().L_points);
 it_pt     = L_pt->Premier();

 it_pt = it_pt->svt; // Le premier point est toujours en (0, 0), on tourne en sens trigo
 ((it_pt->E())).maj(largeur, 0.0); it_pt = it_pt->svt;
 ((it_pt->E())).maj(largeur, hauteur); it_pt = it_pt->svt;
 ((it_pt->E())).maj(0.0, hauteur);
}

//______________________________________________________________________________
alx_zone_texte_opengl::~alx_zone_texte_opengl()
{delete mp_corp;}

//______________________________________________________________________________
alx_zone_texte_opengl::alx_zone_texte_opengl( const double l, const double h, gl_fonte *f
                                            , alx_simulateur_dispositif_saisie *sds ) : tmp( (char*)NULL )
{repere = NULL;
 mp_corp = new alx_model_simulation_physique1;

 mode_copie_colle = mode_selection = false;
 seri_diff_Texte_a_ete_change = false;

 largeur = l_zoom = l;
 hauteur = h_zoom = h;
 zoom    = 1;
 fonte   = f;
 decalage_x = decalage_y = 0;
 position_affichage_debut = 0;
 Position_curseur(0);
 sim_dispo_saisis = sds;

 mp_corp->abonner_a( (alx_classe_base_liant*)this
                   , 1 ); // Numéro attribué
 afficher_fond = false;
 init_mp_corp();
 maj_mp_corp();
 maj_curseur();

 editable = true;
 couleur_texte  [0] = 1; couleur_texte  [1] = 1; couleur_texte  [2] = 1  ; couleur_texte  [3] = 1  ;
 couleur_curseur[0] = 0; couleur_curseur[1] = 1; couleur_curseur[2] = 0.1; couleur_curseur[3] = 0.5;
 couleur_fond   [0] = 0; couleur_fond   [1] = 0; couleur_fond   [2] = 0  ; couleur_fond   [3] = 1  ;
}

//______________________________________________________________________________
const bool alx_zone_texte_opengl::Seri_diff_Texte_a_ete_change(const alx_chaine_char **ptr_cc) const
{*ptr_cc = &texte;
 const bool rep = seri_diff_Texte_a_ete_change;
   seri_diff_Texte_a_ete_change = false;
 return rep;
}

//______________________________________________________________________________
void alx_zone_texte_opengl::afficher_curseur(int deb)
{if(!editable)
   return; //Pas de curseur! Punition!

 char c = c_texte[Position_curseur()];
 int nbcar;
 double largeur_equivalente = largeur/zoom;
 double lg
     , l = fonte->HAUTEUR() / 4
     , h = fonte->HAUTEUR();
 ligne_curseur = ligne_courante-1;
 c_texte[Position_curseur()] = 0;
 lg = fonte->longueur_effective( nbcar
                               , largeur_equivalente //- decalage_x
                               , (char *)&(c_texte[deb]) );
 c_texte[Position_curseur()] = c;
 glPushAttrib(GL_CURRENT_BIT);
 glPushAttrib(GL_COLOR_BUFFER_BIT);
 glEnable(GL_BLEND);
   glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
 glColor4dv(couleur_curseur);
 glPushMatrix();
   glTranslated(lg-decalage_x, -h/4, 0.00000f);
   glBegin(GL_QUADS);
    glVertex3d(0, 0, 0);
    glVertex3d(l, 0, 0);
    glVertex3d(l, h, 0);
    glVertex3d(0, h, 0);
   glEnd();
   //glDisable(GL_BLEND);
 glPopMatrix();
 glPopAttrib();
 glPopAttrib();
}

//______________________________________________________________________________
void alx_zone_texte_opengl::Optimiser_dimensions()
{Calculer_dimensions_optimales();
 Hauteur( Optimal_ty() );
 Largeur( Optimal_tx() );
}

//______________________________________________________________________________
void alx_zone_texte_opengl::Calculer_dimensions_optimales()
{double tx = 0, ty = 0, tx_tmp = 0;
 unsigned int i = 0;
 char c;
 while(i<=texte.Taille()) {
   c = texte.Char(i);
   switch(c)
    {case '\r': if(texte.Char(i+1) != '\n') {break;} else {i++;}
     case 0:
     case '\n':
       if(tx_tmp > tx) {tx = tx_tmp;}
       tx_tmp = 0;
       ty += fonte->HAUTEUR();
     break;
     default:
       tx_tmp += fonte->Largeur_char(c);
     break;
    }
   i++;
  }
  Optimal_tx( tx*Zoom()+0.01 );
  Optimal_ty( ty*Zoom()+0.01 );
}

//______________________________________________________________________________
void alx_zone_texte_opengl::afficher_ligne(int deb)
{V_position_debut_des_lignes[ligne_courante] = deb;
 ligne_courante++;

 int total = (int)strlen( &(c_texte[deb]) );
 if(total==0 && deb==Position_curseur())
  {afficher_curseur(deb);
   decalage_y+= fonte->HAUTEUR();
   glTranslated( 0, -fonte->HAUTEUR(), 0.0f);
   return;}

 bool premier_coup = true;
 int NbCarEcritsTotal = 0
   , NbCarEcrits = 0;

 decalage_x = 0;
 double largeur_equivalente = largeur/zoom;
 while( (NbCarEcritsTotal < total)
      &&(nb_ligne_cour < nb_ligne_max) )
  {
   if(!premier_coup)
    {
     V_position_debut_des_lignes[ligne_courante] = deb;
     ligne_courante++;
    } else premier_coup = false;

   decalage_x += fonte->ecrire( NbCarEcrits
                              , largeur_equivalente - decalage_x
                              , (char *)&(c_texte[deb]) );
   if(x_max<decalage_x) {x_max = decalage_x;}

   char car = c_texte[Position_curseur()];
   if( (Position_curseur() >= deb)
     &&( (Position_curseur() <  deb+NbCarEcrits)
       ||(Position_curseur() == texte.Taille() && Position_curseur() <=  deb+NbCarEcrits)
       ||((car==0||car=='\n') && Position_curseur() == deb+NbCarEcrits) ) )
    {
     afficher_curseur(deb);
    }
   deb += NbCarEcrits;
   NbCarEcritsTotal += NbCarEcrits;
   glTranslated( -decalage_x, -fonte->HAUTEUR(), 0.0f);
   decalage_x = 0;
   decalage_y+= fonte->HAUTEUR();
   y_max = decalage_y;
   if(NbCarEcritsTotal < total) nb_ligne_cour++;
  }

 if(premier_coup)
  {
   decalage_y+= fonte->HAUTEUR();
   y_max = decalage_y;
   glTranslated( 0, -fonte->HAUTEUR(), 0.0f);
  }

}

//______________________________________________________________________________
// But : Annalyser la zone de texte pour détecter les retour à la ligne.
void alx_zone_texte_opengl::afficher()
{// Copie préliminaire ddu texte:
 strcpy_s(c_texte, texte.Texte());
 x_max = y_max = 0;

// On passe aux choses sérieuses:
 int pos = 0
   , deb = 0
   , m = fonte->Mode();

 fonte->Mode(0);     // Pas d'annalyse des %
 ligne_courante = 0; // Ligne courante au début de l'afficheur.
 if(afficher_fond)
  {double dt = Fonte()->HAUTEUR()*Zoom()/5.0;
   glPushAttrib(GL_CURRENT_BIT);
   glColor4dv(couleur_fond);
   if(couleur_fond[3]<1)
    {glPushAttrib(GL_ENABLE_BIT);
     glEnable(GL_BLEND);}
   glBegin(GL_QUADS);
     glVertex2d(       -dt,        -dt);
     glVertex2d(largeur+dt,        -dt);
     glVertex2d(largeur+dt, hauteur);
     glVertex2d(       -dt, hauteur);
   glEnd();
   if(couleur_fond[3]<1)
     glPopAttrib ();
   glPopAttrib ();}

 if(Lisser())
  {glPushAttrib( GL_COLOR_BUFFER_BIT );
   glPushAttrib( GL_ENABLE_BIT );
   glEnable( GL_POLYGON_SMOOTH | GL_LINE_SMOOTH );
   glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
   glEnable( GL_BLEND );}

 glPushAttrib(GL_CURRENT_BIT);
 glColor4dv(couleur_texte);
 if(couleur_texte[3] != 1)
   glEnable(GL_BLEND);
 glPushMatrix();
 decalage_x = 0;
 decalage_y = fonte->HAUTEUR();
 glTranslated(0, hauteur - decalage_y*zoom, 0); // On multiplie par le zoom pour tenir compte du changement
 glScaled(zoom, zoom, 1);                      // d'échelle du glScalef!

 nb_ligne_cour = 0;
 nb_ligne_max  = (int)(hauteur / (zoom*fonte->HAUTEUR()));
 while( (c_texte[pos]!=0)
      &&(nb_ligne_cour < nb_ligne_max) )
  {
   switch(c_texte[pos])
    {case '\r':
       if(c_texte[pos+1]!='\n') break;
       c_texte[pos] = 0;
       afficher_ligne(deb); nb_ligne_cour++;
       c_texte[pos] = '\r';
       pos+= 2;
       deb = pos;
     break;
     case '\n':
       c_texte[pos] = 0;
       afficher_ligne(deb); nb_ligne_cour++;
       c_texte[pos] = '\n';
       pos++;
       deb = pos;
     break;
     default: pos++;
    }
  }
 if( (hauteur == -1)
   ||(nb_ligne_cour < nb_ligne_max) )
   afficher_ligne(deb);

 if(Lisser())
  {glPopAttrib();
   glPopAttrib();}

 glPopMatrix();
 glPopAttrib (); // Couleur du c_texte

 fonte->Mode(m); // retour au mode d'annalyse des % précédents.
 derniere_ligne_ecrite = ligne_courante-1;

}

//______________________________________________________________________________
void alx_zone_texte_opengl::Placer_curseur(const alx_point2D &p)
{
 ligne_curseur = Amax(0, Amin((int)(((hauteur-p.y) / (zoom*fonte->HAUTEUR()))), derniere_ligne_ecrite));
 Position_curseur( V_position_debut_des_lignes[ligne_curseur] );

 unsigned int pos_der_car_ligne;
 if(ligne_curseur==derniere_ligne_ecrite)
   pos_der_car_ligne = texte.Taille();
  else pos_der_car_ligne = V_position_debut_des_lignes[ligne_curseur+1] - 1;
 Position_curseur( Amin( pos_der_car_ligne
                       , Position_curseur() + fonte->Position_lettre(p.x/zoom, (unsigned char *)&(c_texte[Position_curseur()]))) );
}

//______________________________________________________________________________
void alx_zone_texte_opengl::rationnaliser(int num)
{
 switch(num)
  {
   case -1: //// On nous informe que le bouton à été relaché, seul un lieur y aura accès
     Arreter_appel_a_info_sur_pt( pt_de_curseur//(alx_point2D*)(void*)(alx_classe_base_model::infos_comp[0])
                                , alx_pointeur_relache);
   break;
   case 1: // Un message du modèle physique...
     if(!editable)
       return; //On s'en tape de tes messages!
     if( ( (mp_corp->infos_comp[0] != 2)&&(mp_corp->infos_comp[0] != 3)&&(mp_corp->infos_comp[0]!=6) )
       ||(mp_corp->infos_comp[1] != 0) ) return; // Ce message n'est pas le bon
      {
//  2 : Contact avec un objet
//     0 : Un point
//        * : adresse du point
//           * : adresse du modèle physique
//  6 : Contact avec un objet positionner par une suite de transformation de repère
//     0 : Un point
//        * : adresse du point
//           * : adresse du modèle physique
//              * : adresse d'une liste de transformation de repère list<alx_repere2D*>*
       A_changer(true);
       if( (mp_corp->infos_comp[0]==2)||(mp_corp->infos_comp[0]==6) )
        {
         pt_de_curseur = (alx_point2D*)(void*)(mp_corp->infos_comp[2]);
         sim_dispo_saisis->Prendre_evennements_lies_a( pt_de_curseur
                                                     , (alx_classe_base_liant*)this
                                                     , 2 ); // Numéro d'abonnement!
         Appel_a_info_sur_pt(pt_de_curseur, alx_pointeur_relache);

         // Obtenir les coordonnées du clic dans la zone d'édition, en déduire le caractère visé.
         // A FAIRE!!!
         alx_point2D         pt( *( (alx_point2D*)((void*)(mp_corp->infos_comp[2])) ) );
         alx_liste<alx_repere2D*> *l_transfo = (alx_liste<alx_repere2D*>*)((void*)(mp_corp->infos_comp[4]));
         Repere_transformation(pt, *l_transfo);
         if(repere)
           repere->Changer_coordonnees(pt);
         mp_corp->Appliquer_deplacement_inverse_a(pt);
         pt.y += fonte->HAUTEUR()/4;
         Placer_curseur( pt );
         Informer_que_pos_curseur_change( Position_curseur() );
        }

//   3 : Rupture de contact avec un objet
//     0 : Un point
//        * : adresse du point
//           * : adresse du modèle physique
       if(mp_corp->infos_comp[0]==3)
        {

        }
      }
   break;
   case 2: // Les évennements qui proviennent de dispositifs de saisie.
    {
     alx_evennement_dispositif_saisie *evt = &( sim_dispo_saisis->L_evt.Premier_E() );
     unsigned char car = evt->Info_comp().caractere_saisie;

     if(evt->Type_evt() == alx_dispositif_saisie_relache )
      {
       switch( evt->Info_comp().caractere_saisie )
        {
         case SDLK_LCTRL:
         case SDLK_RCTRL:
           mode_copie_colle = false;
         break;
         case SDLK_LSHIFT:
         case SDLK_RSHIFT:
           mode_selection = false;
           fin_selection = Position_curseur();
         break;
        }
      }

     if(evt->Type_evt() == alx_dispositif_saisie_appuie )
      {
       if( !mode_selection )
        {
         switch( evt->Info_comp().caractere_saisie )
          {
           case SDLK_UP         :
           case SDLK_DOWN       :
           case SDLK_RIGHT      :
           case SDLK_LEFT       :
           case SDLK_HOME       :
           case SDLK_END        :
           case SDLK_PAGEUP     :
           case SDLK_PAGEDOWN   :
           case SDLK_LCTRL      :
           case SDLK_RCTRL      :
           case SDLK_LSHIFT     :
           case SDLK_RSHIFT     : // Tous les cas où on ne détruit pas la sélection
             {fin_selection = debut_selection = Position_curseur();}
           break;
           case SDLK_c: // On copie
           break;
           case SDLK_v: // On colle
           break;
           default:
           break;
          }
        }
       if( mode_copie_colle )
        {

        }
       if(evt->Info_comp().est_un_caractere)
        {
         switch( car )
          {
           //case 0: break;
           case SDLK_TAB      : inserer( alx_chaine_char("    ") );      break;
           case SDLK_BACKSPACE:
              if(!mode_selection && (debut_selection != fin_selection))
                {Position_curseur( Amin(fin_selection, debut_selection) );
                 texte.Effacer(Position_curseur(), absd(fin_selection-debut_selection) );
                } else if(Position_curseur()>0) {texte.Effacer(Position_curseur()-1, 1); Position_curseur(Position_curseur()-1);}
               fin_selection = debut_selection = Position_curseur();
           break;
           case SDLK_DELETE:
               if(!mode_selection && (debut_selection != fin_selection))
                {Position_curseur( Amin(fin_selection, debut_selection) );
                 texte.Effacer(Position_curseur(), absd(fin_selection-debut_selection) );
                } else if(Position_curseur()<texte.Taille()) texte.Effacer(Position_curseur(), 1);
               fin_selection = debut_selection = Position_curseur();
           break;
           case SDLK_KP_ENTER :
           case SDLK_RETURN   : inserer("\n");                  break;
           default:
            char s[2];
            s[0] = evt->Info_comp().caractere_saisie;
            s[1] = 0;
            inserer(s);
          }
        }
        else
         {
          // A voir, gestion des flèches, des page haut/bas, des débuts/fin de ligne
          switch( evt->Info_comp().caractere_saisie )
           {
            case SDLK_KP0: inserer("0"); break;
            case SDLK_KP1: inserer("1"); break;
            case SDLK_KP2: inserer("2"); break;
            case SDLK_KP3: inserer("3"); break;
            case SDLK_KP4: inserer("4"); break;
            case SDLK_KP5: inserer("5"); break;
            case SDLK_KP6: inserer("6"); break;
            case SDLK_KP7: inserer("7"); break;
            case SDLK_KP8: inserer("8"); break;
            case SDLK_KP9: inserer("9"); break;
            case SDLK_KP_PERIOD  : inserer("."); break;
            case SDLK_KP_DIVIDE  : inserer("/"); break;
            case SDLK_KP_MULTIPLY: inserer("*"); break;
            case SDLK_KP_MINUS   : inserer("-"); break;
            case SDLK_KP_PLUS    : inserer("+"); break;
            case SDLK_KP_ENTER   : inserer("\n"); break;
            case SDLK_KP_EQUALS  : inserer("="); break;

            case SDLK_LCTRL:
            case SDLK_RCTRL:
              mode_copie_colle = true;
            break;
            case SDLK_LSHIFT:
            case SDLK_RSHIFT:
              mode_selection = true;
              debut_selection = Position_curseur();
            break;

           // Pour se déplacer dans le texte:
            case SDLK_UP         : Monter_ligne(Position_curseur()); break;
            case SDLK_DOWN       : Descendre_ligne(Position_curseur()); break;
            case SDLK_RIGHT      : Position_curseur( Amin( (unsigned int)(Position_curseur()+1), texte.Taille()) ); break;
            case SDLK_LEFT       : if(Position_curseur()>0) Position_curseur( Amax(Position_curseur()-1, (unsigned int)0) ); break;
            case SDLK_HOME       : Debut_ligne(Position_curseur()); break;
            case SDLK_END        : Fin_ligne(Position_curseur()); break;
            case SDLK_PAGEUP     : Monter_page(Position_curseur()); break;
            case SDLK_PAGEDOWN   : Descendre_page(Position_curseur()); break;
           }
         }
       Informer_que_caractere_tape( evt->Info_comp().caractere_saisie );
       if( !mode_selection ) {fin_selection = debut_selection = Position_curseur();}
      }
    }
   break;
  }
}

void alx_zone_texte_opengl::maj_curseur()
{
 //V_position_debut_des_lignes.resize( (unsigned int)(2*hauteur/(zoom*fonte->HAUTEUR())) ); // Le 2* au cas où....
}

void alx_zone_texte_opengl::Monter_ligne(int pos)
{
/* int t = V_position_debut_des_lignes.size();
 double h = fonte->HAUTEUR();   */
 if(ligne_curseur==0) return;

 int deb = V_position_debut_des_lignes[ligne_curseur];
 int dec = Position_curseur() - deb;
// int pour_info_deb_cour  = V_position_debut_des_lignes[ligne_curseur]
//   , pour_info_deb_prcdt = V_position_debut_des_lignes[ligne_curseur-1];

 ligne_curseur--;
 Position_curseur( V_position_debut_des_lignes[ligne_curseur] + Amin( dec
                                                                    , (int)(deb - V_position_debut_des_lignes[ligne_curseur] -1)) );
 ligne_curseur++;
}

void alx_zone_texte_opengl::Descendre_ligne(int pos)
{
 if(ligne_curseur == derniere_ligne_ecrite) return;

 int deb = V_position_debut_des_lignes[ligne_curseur];
 int dec = Position_curseur() - deb;
// int pourinfo = V_position_debut_des_lignes.size();

 ligne_curseur++;
 if(ligne_curseur == derniere_ligne_ecrite)
   Position_curseur( V_position_debut_des_lignes[ligne_curseur] + Amin( dec
                                                                      , (int)(texte.Taille() - V_position_debut_des_lignes[ligne_curseur] -1)) );
  else
    Position_curseur( V_position_debut_des_lignes[ligne_curseur] + Amin( dec
                                                                       , (int)(V_position_debut_des_lignes[ligne_curseur+1] - V_position_debut_des_lignes[ligne_curseur] -1)) );
 ligne_curseur--;
}

void alx_zone_texte_opengl::Debut_ligne(int pos)
{Position_curseur( V_position_debut_des_lignes[ligne_curseur] );}

void alx_zone_texte_opengl::Fin_ligne(int pos)
{if(ligne_curseur == derniere_ligne_ecrite)
   Position_curseur( texte.Taille() );
  else Position_curseur( V_position_debut_des_lignes[ligne_curseur+1] - 1 );
}

void alx_zone_texte_opengl::Monter_page(int pos)
{}

void alx_zone_texte_opengl::Descendre_page(int pos)
{}

inline void alx_zone_texte_opengl::inserer(const char *c)
{texte.Inserer(Position_curseur(), c);
 Position_curseur( Position_curseur()+1 );
 A_changer(true);
}

inline void alx_zone_texte_opengl::inserer(const alx_chaine_char &s)
{texte.Inserer(Position_curseur(), s);
 Position_curseur( Position_curseur()+s.Taille() );
 A_changer(true);
}

char* alx_zone_texte_opengl::Derniere_phrase()
{if(tmp)
   delete tmp;

// Recherche, à partir de la fin, du premier retour à la ligne ou du début de texte
 unsigned int position = texte.Taille()
            , taille = 0;
 const char *car_pos = (texte.Texte() + position - 1);

 do
  {position--; taille++; car_pos--;}
  while( (position > 0)
       &&(*car_pos != '\n') );
 car_pos++;

 tmp = new char [2*taille];
 strcpy_s(tmp, 2*taille, car_pos);
 tmp[taille-1] = 0;

 return tmp;
}


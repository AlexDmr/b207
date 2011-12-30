#include "alx_noeud_scene.h"
#include "../physique/alx_model_simulation_physique1.h"
#include "sdl_opengl/alx_prerendeur.h"

#include <glew.h>
#include "..\physique\math_alex.cpp"
#include "acces_interface.h"

unsigned int alx_noeud_scene::marqueur_pour_parcours_recursif = 1;

extern "C" {
//#include <glu.h>
//#include <glut.h>
}

const alx_pointeur* info_du_contenant::Ptr() const
 {return evt_ptr->Ptr();}

alx_noeud_scene *alx_noeud_scene::noeud_courant;
//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
void boite_englobante::Afficher() const
{glPushAttrib(GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT);
  if(Alpha() < 1) {glEnable(GL_BLEND);}
   glColor4fv( Tab_couleur() );
   glBegin(GL_LINE_LOOP);
     glVertex2d(p1.X(), p1.Y());
     glVertex2d(p2.X(), p1.Y());
     glVertex2d(p2.X(), p2.Y());
     glVertex2d(p1.X(), p2.Y());
   glEnd();
 glPopAttrib();
}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
alx_chaine_char ip_locale;
void IP_locale(const alx_chaine_char &a)
 {ip_locale = a;}
void IP_locale(const char *a)
 {ip_locale = a;}
const alx_chaine_char& IP_locale()
 {return ip_locale;}


char tmp[16];

void alx_noeud_scene::Suffixer_nom(const char *t)
{nom = alx_chaine_char("n_") + nb/*itoa_s(nb, tmp, 10)*/ + "_" + IP_locale() + "_" + t;}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
alx_noeud_scene::alx_noeud_scene(const bool est_texture_optimisable) : alx_arbre<alx_noeud_scene*>(this)//, etirement_absolu(1,1)
{// Generate names
 nb = nb_noeud++;
 nom = "n_";
 nom+= nb; //_itoa_s(nb, tmp, 10);
 nom.Ajouter("_", 0, 1);
 nom+= IP_locale();

 Contenant_courant           ( (info_du_contenant *)NULL );
 Rap_pre_traitement_contient ( (alx_methode_rappel*)NULL );
 Rap_post_traitement_contient( (alx_methode_rappel*)NULL );

 if(est_texture_optimisable)
  {zdr = new alx_noeud_zone_rendu_sdl_opengl( NULL
                                            , 1
                                            , NULL, NULL
                                            , ALXGL_RENDU_ECRAN_PUIS_COPIE_DANS_TEXTURE );
   zdr->Noeud_a_rendre( NULL );
   zdr->Ajouter_fils( this );

   cc_tmp  = "Info_texture_scene_de_";
   cc_tmp += nom;
   Info_texture_scene()->Nom_CC(cc_tmp); cc_tmp = "";
  } else {zdr = (alx_noeud_zone_rendu_sdl_opengl*)NULL;}

 MP( &Intrinseque_MP() );

 parcours_changement = false;
 Forcer_pre_rendu_noeud(false);

 noeud_a_changer = serialisation_a_changer = true;
 fils_a_subi_deseri_diff = false;
 fils_a_subi_seri_diff   = false;
/* tailles_locales[0]   = 0;
   positions_locales[0] = 0;
   nb_elements_locaux  = 0;
 tailles_globales[0] = 0;
   positions_globales[0] = 0;
   nb_elements_globaux = 0;
 tailles_meta_data[0] = 0;
   positions_meta_data[0] = 0;
   nb_elements_meta_data = 0;
*/

 noeud_pour_seri_fils = this;

 Nom_classe("noeud_bigre");

 nb_ptr_present_ds_noeud = nb_ptr_present_ds_noeud_prcdt = 0;
 nb_ptr_present_ds_fils  = nb_ptr_present_ds_fils_prcdt  = 0;

 Marqueur_attributs(M_tout, false);
 deja_prerendu = false;
 avorter = false;
 maj_boites_recursif = false;
 rationnalisation_en_cours = false;
 replication_juste_effectuee = false;
 Est_une_replique(false);
 Pre_rendu_exeptionnel(false);
 Noeud_touchable(true);
 Noeud_de_coupe(false);
 Noeud_puis_fils(true);
 tenir_compte_des_boites_des_fils = true;
 Afficher_boites(false);
 Empiler_contenant(false);
 Forcer_a_ne_plus_empiler_contenant( false );

 pipoter_affichage = heritage_de_pipoter_affichage = false;
/* ne_pas_afficher_fils = ne_pas_pre_rendre_fils = ne_pas_pre_rendre = deja_prerendu = false;
 compter_affichage = compter_pre_rendu = false;
 appartenance_deja_testee = false;
 position_dans_liste_des_fils_changeable = false;
 toujours_au_premier_plan                = false;
 rationnalisation_en_cours               = false;
 avorter                                 = false;
 verouiller                              = false;
 est_une_replique                        = false;
 deja_serialise                          = false; */

 Marqueur_attributs( M_position_dans_liste_des_fils_changeable|M_position_des_fils_changeable|M_afficher|M_gerer_contacts
                   , true );
 Contenu_dans_fils_ssi_pas_contenu_dans_noeud(false);
 Contenu_dans_fils_ssi_contenu_dans_noeud    (false);
 Mode_texture( false );
 Texture_translucide( true );
 Couleur_fond_texture(0, 0, 0, 0);
 Couleur_masque_texture(1, 1, 1, 1);

 Compter_pre_rendu( true );
/* position_des_fils_changeable            = true;
 afficher = gerer_contacts               = true;*/
 nb_max_pre_rendu = nb_max_affichage = 1;
 marqueur_de_parcours = 0;
 Limite_nb_recursions( 0 );
 nb_pre_rendu = nb_affichage = 0;
 prerendeur = (alx_prerendeur*)NULL;
 limite = limite_svg = (alx_noeud_scene*)NULL;
 val_stencil = 0;
 operation_si_appuie  = ALX_NOEUD_OPERATION_SI_CONTIENT_ALORS_PREMIER_PLAN;
 operation_si_relache = ALX_NOEUD_OPERATION_SI_CONTIENT_ALORS_RIEN;
 operation_si_passage = ALX_NOEUD_OPERATION_SI_CONTIENT_ALORS_RIEN;
 init_ptr();
 chaine_cour = (alx_chaine_char*)NULL;

 Nb_pixels_par_unite( 1 );
// Serialiser_type();

}

void alx_noeud_scene::init_ptr()
{f_ptr[0] = &alx_noeud_scene::Deserialiser_bool;
 f_ptr[1] = &alx_noeud_scene::Deserialiser_int;
 f_ptr[2] = &alx_noeud_scene::Deserialiser_double;
 f_ptr[3] = &alx_noeud_scene::Deserialiser_repere;
 f_ptr[4] = &alx_noeud_scene::Deserialiser_chaine;
 f_ptr[5] = &alx_noeud_scene::Deserialiser_Lfils;

 a_ptr[0]  = (alx_repere2D*)this;
 a_ptr[1]  = &nom_iu; //NULL;//&(nom);
 a_ptr[2]  = &(nb);
 a_ptr[3]  = &COPIE_nb_pix_par_unite;
 a_ptr[4]  = &(M_attrib);

 a_ptr[5]  = NULL; //&(deja_prerendu);
 a_ptr[6]  = NULL; //&(position_dans_liste_des_fils_changeable);
 a_ptr[7]  = NULL; //&(ne_pas_pre_rendre_fils);
 a_ptr[8]  = NULL; //&(ne_pas_afficher_fils);
 a_ptr[9]  = NULL; //&(gerer_contacts);
 a_ptr[10] = NULL; //&(afficher);
 a_ptr[11] = NULL; //&(toujours_au_premier_plan);
 a_ptr[12] = NULL; //&(compter_affichage);
 a_ptr[13] = NULL; //&(compter_pre_rendu);
 a_ptr[14] = NULL; //&(appartenance_deja_testee);
 a_ptr[15] = NULL; //&(rationnalisation_en_cours);
 a_ptr[16] = NULL; //&(avorter);
 a_ptr[17] = NULL; //&(position_des_fils_changeable);
 a_ptr[18] = NULL; //&(verouiller);
 a_ptr[19] = &(nb_max_pre_rendu);

 a_ptr[20] = NULL; //&(nb_pre_rendu);
 a_ptr[21] = NULL; //&(nb_affichage);
 a_ptr[22] = &(nb_max_affichage);
 a_ptr[23] = &(limite_nb_recursions);
 a_ptr[24] =  NULL; //&(val_stencil);
 a_ptr[25] = &(operation_si_appuie);
 a_ptr[26] = &(operation_si_relache);
 a_ptr[27] = &(operation_si_passage);
 a_ptr[28] = NULL; //&(est_une_replique);
 a_ptr[29] = NULL; //&(deja_serialise); */
 a_ptr[30] = &(L_fils);
 a_ptr[31] = &(action_transmise);
}

//______________________________________________________________________________
void alx_noeud_scene::maj_noeud (const alx_noeud_scene &n)
{alx_repere2D::maj(n);}

//______________________________________________________________________________
void alx_noeud_scene::maj_repere(const alx_repere2D &r)
{alx_repere2D::maj(r);}

//______________________________________________________________________________
//________________________ A propos de la sérialisation ________________________
//______________________________________________________________________________
const alx_chaine_char& alx_noeud_scene::Serialiser_type(const alx_chaine_char &contenu) const
{serialisation_type.Maj("<alx_noeud_scene nom=",0,21);
 serialisation_type += Nom();
 serialisation_type.Ajouter(";>",0,2);
 serialisation_type += contenu;
 serialisation_type.Ajouter("</alx_noeud_scene>",0,18);
 return serialisation_type;}

//______________________________________________________________________________
void alx_noeud_scene::Deserialiser_bool(unsigned int & pos, void *res)
{*((bool*)res) = chaine_cour->Lire_bool(pos);}
//______________________________________________________________________________
void alx_noeud_scene::Deserialiser_int(unsigned int & pos, void *res)
{*((int*)res) = chaine_cour->Lire_entier(pos);}
//______________________________________________________________________________
void alx_noeud_scene::Deserialiser_double(unsigned int & pos, void *res)
{*((double*)res) = chaine_cour->Lire_double(pos);}
//______________________________________________________________________________
void alx_noeud_scene::Deserialiser_repere(unsigned int & pos, void *res)
{((alx_repere2D*)res)->alx_repere2D::Deserialiser_contenu(*chaine_cour, pos);}
//______________________________________________________________________________
void alx_noeud_scene::Deserialiser_chaine(unsigned int & pos, void *res)
{const char *texte = chaine_cour->Texte();
// Lire la taille
 unsigned int nbc = chaine_cour->Lire_entier(pos);
// Passer le séparateur '_'
 pos ++;
// Lire le texte
 ((alx_chaine_char*)res)->Maj(&(texte[pos]), nbc);
// Mettre la position à la fin du texte
 pos += nbc;
}
//______________________________________________________________________________
void alx_noeud_scene::Deserialiser_Lfils (unsigned int & pos, void *res)
{fils_a_subi_deseri_diff = true;
 const char *texte = chaine_cour->Texte();
 if( texte[pos] != '(' )
    return; ///ARGHHHHH!!!!
 pos++; // On passe la (
 unsigned int deb, deb_tout = pos;
 L_fils_seri.Vider();
 while(texte[pos] != ')')
  {deb = pos;
   pos = chaine_cour->Position_du_prochain(';', deb);
   nom_f_tmp.Maj(texte, deb, pos-deb);
   L_fils_seri.Ajouter_a_la_fin( nom_f_tmp );
   pos++; // On passe le ;
  }

 serialisation_Lfils.Maj(&(texte[deb]), pos-deb_tout);
 pos++; // on passe la )
}

//______________________________________________________________________________
const alx_chaine_char& alx_noeud_scene::Serialiser_difference_fils() const
{fils_a_subi_seri_diff = false;
 #ifdef __ALX_DEBUG__
   printf("    Serialiser diff fils\n");
 #endif
 if(noeud_pour_seri_fils->L_fils.Taille() != L_fils_seri.Taille())
 {
  #ifdef __ALX_DEBUG__
    printf("    FIN DE Serialiser diff fils\n");
  #endif
  fils_a_subi_seri_diff = true; return Serialiser_fils();}
 alx_element_liste<alx_arbre<alx_noeud_scene*>*> *it     = noeud_pour_seri_fils->L_fils.Premier()
                                               , *it_fin = noeud_pour_seri_fils->L_fils.Fin();
 alx_element_liste<alx_chaine_char> *its     = L_fils_seri.Premier()
                                  , *its_fin = L_fils_seri.Fin();
 for(; (it!=it_fin)&&(its!=its_fin); it=it->svt)
  {if( it->E()->e->Nom() == its->E() )
     its = its->svt;
    else break;
  }
 if( (it  != it_fin ) // On régénère tout!
   ||(its != its_fin) )
  {changement_dans_liste_fils = false;
  #ifdef __ALX_DEBUG__
    printf("    FIN DE Serialiser diff fils\n");
  #endif
   fils_a_subi_seri_diff = true; return Serialiser_fils();
  }
 changement_dans_liste_fils = true;
 #ifdef __ALX_DEBUG__
   printf("    FIN DE Serialiser diff fils\n");
 #endif
 return Chaine_vide();
}

//______________________________________________________________________________
const alx_chaine_char& alx_noeud_scene::Serialiser_fils() const
{serialisation_fils.Maj("(",0,1);
 alx_noeud_scene *noeud;
 alx_element_liste<alx_arbre<alx_noeud_scene*>*> *it     = noeud_pour_seri_fils->L_fils.Premier()
                                               , *it_fin = noeud_pour_seri_fils->L_fils.Fin();
 L_fils_seri.Vider();
 for(;it!=it_fin;it=it->svt)
  {noeud = (it->E())->e;
   L_fils_seri.Ajouter_a_la_fin( noeud->Nom() );
   serialisation_fils += noeud->Nom()+";";}

 serialisation_fils.Ajouter(")",0,1);
 return serialisation_fils;
}

//______________________________________________________________________________
const alx_chaine_char& alx_noeud_scene::Serialiser_contenu() const
{//serialisation_contenu = "(alx_noeud_scene)(";
// const char *txt_seri = serialisation_contenu.Texte();
   serialisation_contenu  = "0,3="+alx_repere2D::Serialiser_contenu()+";";
   serialisation_contenu += "1,4=";
      serialisation_contenu += nom_iu.Taille();
      serialisation_contenu += "_";
      serialisation_contenu += nom_iu;
      serialisation_contenu += ";";
   serialisation_contenu += alx_chaine_char("3,2=")+COPIE_nb_pix_par_unite+";";
   serialisation_contenu += alx_chaine_char("2,1=")+nb+";";
   serialisation_contenu += alx_chaine_char("4,1=")+M_attrib+";";
   serialisation_contenu += alx_chaine_char("19,1=")+nb_max_pre_rendu+";";
//   serialisation_contenu += alx_chaine_char("20,1=")+nb_pre_rendu+";";
//   serialisation_contenu += alx_chaine_char("21,1=")+nb_affichage+";";
   serialisation_contenu += alx_chaine_char("22,1=")+nb_max_affichage+";";
   serialisation_contenu += alx_chaine_char("23,1=")+limite_nb_recursions+";";
//   serialisation_contenu += alx_chaine_char("24,1=")+val_stencil+";";
   serialisation_contenu += alx_chaine_char("25,1=")+operation_si_appuie+";";
   serialisation_contenu += alx_chaine_char("26,1=")+operation_si_relache+";";
   serialisation_contenu += alx_chaine_char("27,1=")+operation_si_passage+";";
   serialisation_contenu += "30,5=" + Serialiser_fils() + ";";
   serialisation_contenu += alx_chaine_char("31,1=") + action_transmise  + ";";
/*     alx_noeud_scene *noeud;
     alx_element_liste<alx_arbre<alx_noeud_scene*>*> *it     = L_fils.Premier()
                                                   , *it_fin = L_fils.Fin();
     for(;it!=it_fin;it=it->svt)
      {noeud = (it->E())->e;
       serialisation_contenu += noeud->Nom()+";";}
   serialisation_contenu += ");"; */
 return serialisation_contenu;}

//______________________________________________________________________________
//___________________ Sérialisation des différences survenues___________________
//______________________________________________________________________________
const alx_chaine_char& alx_noeud_scene::Serialiser_difference_contenu() const
{
 #ifdef __ALX_DEBUG__
   printf("  Serialiser diff contenu\n");
 #endif
 if(serialisation_contenu.Taille()==0)
  {Serialiser_contenu();
   serialisation_differences_contenu = serialisation_contenu;
   #ifdef __ALX_DEBUG__
     printf("  FIN DE Serialiser diff contenu\n");
   #endif
   return serialisation_differences_contenu;}

 const char *txt_seri = serialisation_contenu.Texte()
          , *txt_diff = serialisation_differences_contenu.Texte();

 serialisation_differences_contenu.Maj("", 0, 0);
 // On passe au crible toutes les variables et on sérialise si elles sont différentes,
 // en oubliant pas de mettre à jour serialisation_contenu !
// Parcours de la chaine serialisation_contenu, on construit en même temps sa nouvelle valeur

 unsigned int pos = 0, dep, i_tmp, dep_info, fin_info;
 // Le repère :_________________________________________________________________
   const alx_chaine_char &ccs_rep = alx_repere2D::Serialiser_difference_contenu();
   if( ccs_rep.Taille() != 0 )
    {serialisation_differences_contenu.Maj("0,3=", 0, 4);
     serialisation_differences_contenu += ccs_rep;
     serialisation_differences_contenu.Ajouter(";", 0, 1);}
   nouv_contenu.Maj("0,3=", 0, 4);
   nouv_contenu += alx_repere2D::Serialisation_contenu();
   nouv_contenu.Ajouter(";", 0, 1);
 // Le numéro :_________________________________________________________________
 // L'attribut binaire condensé :_______________________________________________
 // Nombre max de pré-rendu :___________________________________________________
 // Nombre max d'affichages :___________________________________________________
 // Nombre limite de récursions :_______________________________________________
 // Opération si appui :________________________________________________________
 // Opération si relache :______________________________________________________
 // Opération si passage :______________________________________________________
   unsigned int T_val[9];
   T_val[0] = nb; T_val[1] = M_attrib; T_val[2] = nb_max_pre_rendu; T_val[3] = nb_max_affichage;
   T_val[4] = limite_nb_recursions; T_val[5] = operation_si_appuie; T_val[6] = operation_si_relache;
   T_val[7] = operation_si_passage;

   pos = 4;
   for(unsigned int i=0; i<4; i++)
    {pos = serialisation_contenu.Position_du_prochain(')', pos); pos++;}
   pos++; // On passe le ; qui suit la description du repère.

  // Le nom de l'IU
   unsigned int pos_deb_txt;
   dep_info = pos;
   pos = serialisation_contenu.Position_du_prochain('=', pos); pos++; // le =
     dep = pos;
   i_tmp = serialisation_contenu.Lire_entier(pos);             pos++; // le _
     pos_deb_txt = pos;
   pos += i_tmp;
   serialisation_contenu.Remplacer_char(pos, (char)0);

   if( (nom_iu.Taille() != i_tmp)
     ||(nom_iu != &(txt_seri[pos_deb_txt])) ) {// Mise à jour de la différence
                                               serialisation_differences_contenu.Ajouter(txt_seri, dep_info, dep-dep_info);
                                               serialisation_differences_contenu += nom_iu.Taille();
                                               serialisation_differences_contenu.Ajouter("_", 0, 1);
                                               serialisation_differences_contenu += nom_iu;
                                               serialisation_differences_contenu.Ajouter(";", 0, 1);
                                               // Mise à jour du nouveau contenu
                                               nouv_contenu.Ajouter(txt_seri, dep_info, dep-dep_info);
                                               nouv_contenu += nom_iu.Taille();
                                               nouv_contenu.Ajouter("_", 0, 1);
                                               nouv_contenu += nom_iu;
                                               nouv_contenu.Ajouter(";", 0, 1);
                                              } else {nouv_contenu.Ajouter(txt_seri, dep_info, pos-dep_info);
                                                      nouv_contenu.Ajouter(";", 0, 1);}
   serialisation_contenu.Remplacer_char(pos, ';');
   pos++;

  // Codage des doubles
  double d_temp;
  dep_info = dep = pos;
  pos = serialisation_contenu.Position_du_prochain('=', pos); pos++;
  fin_info = pos;
  nouv_contenu.Ajouter(serialisation_contenu.Texte(), dep, pos-dep);
  dep = pos; d_temp = serialisation_contenu.Lire_double(pos);
  if(COPIE_nb_pix_par_unite != d_temp)
   {cc_tmp = COPIE_nb_pix_par_unite;
    nouv_contenu += cc_tmp;
    serialisation_differences_contenu.Ajouter(serialisation_contenu.Texte(), dep_info, fin_info-dep_info);
    serialisation_differences_contenu += cc_tmp;
    serialisation_differences_contenu.Ajouter(";", 0, 1);
   } else {nouv_contenu.Ajouter(serialisation_contenu.Texte(), dep, pos-dep);}
   nouv_contenu.Ajouter(";", 0, 1);
   pos++;

  // Les codages à partir d'entiers
   for(unsigned int i=0; i<8; i++)
    {dep_info = dep = pos;
     pos = serialisation_contenu.Position_du_prochain('=', pos); pos++;
     fin_info = pos;
     //cc_tmp.Maj(serialisation_contenu.Texte(), dep, pos-dep);
     nouv_contenu.Ajouter(serialisation_contenu.Texte(), dep, pos-dep);
     dep = pos; i_tmp = serialisation_contenu.Lire_entier(pos);
     if(i_tmp != T_val[i])
      {cc_tmp = T_val[i];
       nouv_contenu += cc_tmp;
       serialisation_differences_contenu.Ajouter(serialisation_contenu.Texte(), dep_info, fin_info-dep_info);
       serialisation_differences_contenu += cc_tmp;
       serialisation_differences_contenu.Ajouter(";", 0, 1);}
      else nouv_contenu.Ajouter(serialisation_contenu.Texte(), dep, pos-dep);
     nouv_contenu.Ajouter(";", 0, 1);
     pos++;
    }

 // Les fistons : A FAIRE A PART !!!____________________________________________
  dep  = pos;
  pos += 6; // 30,5=(
  pos  = serialisation_contenu.Position_du_prochain(')', pos);
  pos += 2; // );
  const alx_chaine_char &sdf = Serialiser_difference_fils();
  if(sdf.Taille() != 0)
   {serialisation_differences_contenu.Ajouter("30,5=",0,5); nouv_contenu.Ajouter("30,5=",0,5);
    serialisation_differences_contenu += sdf;               nouv_contenu += sdf;
    serialisation_differences_contenu.Ajouter(";",0,1);     nouv_contenu.Ajouter(";",0,1);
    }
    else {nouv_contenu.Ajouter(serialisation_contenu.Texte(), dep, pos-dep);}

// Ajout de l'action en cas de modification lors de la transmition
 dep = pos;
 pos = serialisation_contenu.Position_du_prochain('=', pos); pos++;
 fin_info = pos;
 nouv_contenu.Ajouter(serialisation_contenu.Texte(), dep, pos-dep);
 dep = pos; i_tmp = serialisation_contenu.Lire_entier(pos);
 if(i_tmp != (unsigned int)action_transmise)
  {cc_tmp = action_transmise;
   nouv_contenu += cc_tmp;
   serialisation_differences_contenu.Ajouter(serialisation_contenu.Texte(), dep_info, fin_info-dep_info);
   serialisation_differences_contenu += cc_tmp;
   serialisation_differences_contenu.Ajouter(";", 0, 1);
  } else nouv_contenu.Ajouter(serialisation_contenu.Texte(), dep, pos-dep);
 nouv_contenu.Ajouter(";", 0, 1);

// A la fin on affecte le nouveau contenu de serialisation_contenu et on le renvoi :
 serialisation_contenu = nouv_contenu;

#ifdef __ALX_DEBUG__
  printf("  FIN DE Serialiser diff contenu\n");
#endif

 return serialisation_differences_contenu;
}
//______________________________________________________________________________
const alx_chaine_char& alx_noeud_scene::Serialiser_difference_type(const alx_chaine_char &diff_contenu) const
{
 #ifdef __ALX_DEBUG__
   printf("Serialiser diff type\n");
 #endif
 if(serialisation_type.Taille()==0)
  {Serialiser_type();
   serialisation_differences_type = serialisation_type;
   #ifdef __ALX_DEBUG__
     printf("FIN DE Serialiser diff type\n");
   #endif
   return serialisation_differences_type;}

 // Rien de spécifique à mettre ici
 // si le contenu est nul on se barre!
 if(diff_contenu.Taille()==0)
   serialisation_differences_type.Maj("", 0, 0);
  else
   {serialisation_differences_type.Maj("<alx_noeud_scene nom=", 0, 21);
    serialisation_differences_type += Nom();
    serialisation_differences_type.Ajouter(";>", 0, 2);
    serialisation_differences_type += diff_contenu;
    serialisation_differences_type.Ajouter("</alx_noeud_scene>", 0, 18);}
    #ifdef __ALX_DEBUG__
      printf("FIN DE Serialiser diff type\n");
    #endif
 return serialisation_differences_type;
}

//______________________________________________________________________________
//______________________________ Désérialisation________________________________
//______________________________________________________________________________
void alx_noeud_scene::Deserialiser_type(const alx_chaine_char &txt, unsigned int &pos)
{pos = txt.Position_du_prochain('>', pos); pos++;
 alx_noeud_scene::Deserialiser(txt, pos);
 pos += 18; // </alx_noeud_scene>
}

//______________________________________________________________________________
void alx_noeud_scene::Deserialiser(const alx_chaine_char &txt, unsigned int &pos)
{fils_a_subi_deseri_diff = false; // Pour le moment...

 unsigned int num_f, num_a;
 const char *texte = txt.Texte();
 f_deserialisation_scene f;

 chaine_cour = &txt;
 //pos+=18; // on passe (alx_noeud_scene)(
 while( (texte[pos]<='9')
      &&(texte[pos]>='0') )
  {num_a = txt.Lire_entier(pos);
   pos++; // On passe la ,
   num_f = txt.Lire_entier(pos);
   f = f_ptr[num_f];
   pos++; // On passe le =

   (this->*f)(pos, a_ptr[num_a]);
   pos++; // On passe le ;
  }
 //pos++; // On passe la )

// Horrible maj du nb de pixels par unité...
 if(Nb_pixels_par_unite() != COPIE_nb_pix_par_unite) {Nb_pixels_par_unite(COPIE_nb_pix_par_unite);}
}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
unsigned int alx_noeud_scene::nb_noeud = 0;

alx_model_simulation_physique1 mp_puit;
alx_noeud_scene noeud_puit;
info_du_contenant rep_puit;

alx_model_simulation_physique1* Mp_puit() {return &mp_puit;}
alx_noeud_scene* Noeud_puit() {return &noeud_puit;}
info_du_contenant* Rep_puit() {return &rep_puit;}

void Initialisation_etats_puits()
{rep_puit.noeud = &noeud_puit;
 rep_puit.mp    = &mp_puit;}

Liste_de_rappel
              ;
void alx_noeud_scene::Vider_Listes()
{LR_Avant_pre_rendu_fils.Vider();
 LR_Apres_pre_rendu_fils.Vider();
 LR_Avant_affichage_fils.Vider();
 LR_Apres_affichage_fils.Vider();
 LR_Avant_contenu_dans_fils.Vider();
 LR_Apres_contenu_dans_fils.Vider();
 LR_Avant_affichage.Vider();
 LR_Apres_affichage.Vider();
 LR_Avant_pre_rendu.Vider();
 LR_Apres_pre_rendu.Vider();
 L_rappel_pointeur_detecte.Vider();
 L_rappel_effective_pointeur_detecte.Vider();
}

//______________________________________________________________________________
//______________________________ Gestion texture _______________________________
//______________________________________________________________________________
INFOS_TEXTURE* alx_noeud_scene::Info_texture_scene() const {return zdr->Infos_texture_tempon();}
const double* alx_noeud_scene::Couleur_fond_texture() const {if(zdr) {return zdr->Couleur();} else {return NULL;}}
const double alx_noeud_scene::Composante_couleur_fond_texture(const unsigned int i)
 {if(zdr) {return zdr->Composante_couleur(i);} else {return -1;}}
void alx_noeud_scene::Composante_couleur_fond_texture(const unsigned int i, const double v)
 {if (zdr) {A_changer(true); zdr->Composante_couleur(i, v);}}
void alx_noeud_scene::Couleur_fond_texture(const double r, const double v, const double b, const double a)
 {if (zdr) {A_changer(true); zdr->Couleur(r, v, b, a);}}
void alx_noeud_scene::Couleur_fond_texture(const double *c)
 {if (zdr) {A_changer(true); zdr->Couleur(c);}}
double* alx_noeud_scene::P_couleur_fond_texture(const unsigned int i) {if (zdr) {return zdr->P_couleur(i);} else {return NULL;}}

void alx_noeud_scene::Nb_pixels_par_unite(const double n) {if(zdr) {zdr->Nb_pixels_par_unite(n); COPIE_nb_pix_par_unite = n; A_changer(true); Maj_texture(true);}}
const double alx_noeud_scene::Nb_pixels_par_unite() const {if(zdr) {return zdr->Nb_pixels_par_unite();} else {return 0;}}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
info_du_contenant* alx_noeud_scene::Creer_contenant()
{info_du_contenant *rep = new info_du_contenant();
 // Si c'est bon, renvoyer la réponse.
 rep->mp    = &(MP());
 rep->noeud = (alx_noeud_scene*)this;
 return rep;
}

//______________________________________________________________________________
info_du_contenant* alx_noeud_scene::Noeud_contient(const alx_point2D &pt, int action)
{return (info_du_contenant*)NULL;}

//______________________________________________________________________________
info_du_contenant* alx_noeud_scene::Resultat_contient(info_du_contenant *rep)
{
 if(rep)
   rep->L_reperes.Ajouter_au_debut( (alx_repere2D*)this );
 return rep;
}
//______________________________________________________________________________
info_du_contenant* alx_noeud_scene::Contient(const alx_point2D &pt, int action)
{if(!Noeud_touchable())
   return (info_du_contenant*)NULL;
 info_du_contenant *rep, *rep_fils;

 pt_pour_contient.maj(pt);
 if(Prendre_en_compte_repere())
  {this->Changer_coordonnees( pt_pour_contient );}

// A-t-on cliqué sur le noeud et doit on renvoyer ce résultat?
 if( Rap_pre_traitement_contient() )
  {Rap_pre_traitement_contient()->Rappel( this );
  } else {Pre_traitement_contient( true );}
 if(Pre_traitement_contient())
   rep = this->Noeud_contient(pt_pour_contient, action);
  else {rep = (info_du_contenant*)NULL;}
 if( (Noeud_de_coupe())
   ||( (rep != (info_du_contenant*)NULL)
     &&(!Noeud_puis_fils())) )
   {this->Resultat_contient(rep);
   } else {// Tous les cas où le résultats des fils dépend de celui du père :
           if( (Contenu_dans_fils_ssi_contenu_dans_noeud()     && Contenu_dans_fils_ssi_pas_contenu_dans_noeud())
             ||(Contenu_dans_fils_ssi_contenu_dans_noeud()     && (rep == (info_du_contenant*)NULL))
             ||(Contenu_dans_fils_ssi_pas_contenu_dans_noeud() && (rep != (info_du_contenant*)NULL))             )
            {this->Resultat_contient(rep);
            } else {// Si je suis ici c'est que les fils sont prioritaire, si j'ai cliqué sur l'un d'eux c'est lui que je renverrais
                    rep_fils = this->Contenu_dans_fils(pt_pour_contient, action);
                    if(rep_fils)
                     {if(rep && (rep != Rep_puit())) {delete rep;}
                      rep = this->Resultat_contient(rep_fils);
                     }
                   }
          }

 if( Rap_post_traitement_contient() )
  {bool nouvelle_rep;
   Contenant_courant( rep );
   Rap_post_traitement_contient()->Params( this );
   Rap_post_traitement_contient()->Rappel( this );
   if(rep != Contenant_courant())                 {nouvelle_rep = true;} else {nouvelle_rep = false;}
   if(rep && (rep != Rep_puit()) && nouvelle_rep) {delete rep;}
   rep = Contenant_courant();
   if( (rep !=  (info_du_contenant*)NULL) && nouvelle_rep)
    {rep->Point_au_contact( pt_pour_contient );
     this->Resultat_contient(rep);
    }
  }
 return rep;
}

//______________________________________________________________________________
void Reconstruire_rappels(info_du_contenant *rep, const alx_liste<alx_repere2D*> &L_rep)
{alx_noeud_scene *noeud;
 alx_element_liste<alx_repere2D*> *it     = L_rep.Dernier()
                                , *it_deb = L_rep.Debut();

 for(; it!=it_deb; it=it->prcdt)
  {noeud = static_cast<alx_noeud_scene*>(it->E());
   Liste_de_rappel *L = new Liste_de_rappel();
   noeud->Ajouter_appels(rep, noeud, *L);
  }

}

//______________________________________________________________________________
void alx_noeud_scene::Ajouter_appels(info_du_contenant *rep, alx_noeud_scene *noeud_contenant, Liste_de_rappel &L_rappel_effective)
{alx_interface_sdl_opengl_1 *interface_mere = Interface_mere();
 if( (interface_mere == (alx_interface_sdl_opengl_1*)NULL )
   ||(noeud_contenant->L_rappel_pointeur_detecte.Vide()) )
   return;
 const unsigned int type = (unsigned int)interface_mere->Sim_couche()->Evt_courant()->Type_evt();
 mr_cour = noeud_contenant->L_rappel_pointeur_detecte.Liste()->Premier(); ep_cour = noeud_contenant->L_etat_pointeur.Premier();
 mr_fin  = noeud_contenant->L_rappel_pointeur_detecte.Liste()->Fin()    ; ep_fin  = noeud_contenant->L_etat_pointeur.Fin();
 unsigned int ui_cour;
 for(;mr_cour!=mr_fin; mr_cour=mr_cour->svt)
  {ui_cour = ep_cour->E();
   if( (ui_cour & type)
     &&( ((ui_cour & Pas_rappel_si_noeud_contient())==0) || (noeud_contenant!=rep->NOEUD()) )
     &&( ((ui_cour & Pas_rappel_si_fils_contient() )==0) || !rep->L_REPERES().Contient( noeud_contenant ) )
     &&( ((ui_cour & Rappel_Seulement_si_noeud_contient())==0) || noeud_contenant==rep->NOEUD() )
     &&( ((ui_cour & Rappel_Seulement_si_fils_contient())==0 ) || (rep->L_REPERES().Contient( noeud_contenant ) && rep->NOEUD() != noeud_contenant) )
     )
    {L_rappel_effective.Ajouter_methode_a_la_fin( mr_cour->E() );}
   ep_cour = ep_cour->svt;
  }
 if( L_rappel_effective.Vide() )
     return;
 if(rep->LL_RAPPEL() == NULL)
   rep->LL_RAPPEL(new alx_liste<Liste_de_rappel*>());
 i_si_contient.info            = rep;
 i_si_contient.noeud_contenant = noeud_contenant;
 i_si_contient.L_reperes = rep->L_reperes;
 //Lister_partie(rep->L_reperes, rep->L_reperes.Premier(), rep->L_reperes.Fin(), i_si_contient.L_reperes);
// if(type==1)
//   Lister_partie(rep->L_reperes, rep->L_reperes.Premier()->svt, rep->L_reperes.Fin(), i_si_contient.L_reperes);
//  else Lister_partie(rep->L_reperes, rep->L_reperes.Premier()->svt->svt, rep->L_reperes.Fin(), i_si_contient.L_reperes);
 L_rappel_effective.Param(&i_si_contient);
 rep->LL_RAPPEL()->Ajouter_a_la_fin( &L_rappel_effective );
}

//______________________________________________________________________________
void alx_noeud_scene::Ajouter_appels(info_du_contenant *rep, const unsigned int type, Liste_de_rappel &L_rappel)
{alx_interface_sdl_opengl_1 *interface_mere = Interface_mere();
 if( interface_mere )
   if( (type!=(unsigned int)interface_mere->Sim_couche()->Evt_courant()->Type_evt())
     ||(L_rappel.Liste()->Vide()) )
     return;
 if(rep->LL_RAPPEL() == NULL)
   rep->LL_RAPPEL(new alx_liste<Liste_de_rappel*>);
 i_si_contient.info            = rep;
 i_si_contient.noeud_contenant = this;
 if(type==1)
   Lister_partie(rep->L_reperes, rep->L_reperes.Premier()->svt, rep->L_reperes.Fin(), i_si_contient.L_reperes);
  else {if(rep->L_reperes.Taille() >= 1)
         {Lister_partie(rep->L_reperes, rep->L_reperes.Premier()->svt->svt, rep->L_reperes.Fin(), i_si_contient.L_reperes);
         } else {printf("PB dans alx_noeud_scene::Ajouter_appels(info_du_contenant *rep, const unsigned int type, Liste_de_rappel &L_rappel)\n");}
       }
 L_rappel.Param(&i_si_contient);
 rep->LL_RAPPEL()->Ajouter_a_la_fin( &L_rappel );
}

//______________________________________________________________________________
info_du_contenant* alx_noeud_scene::Contenu_dans_fils(const alx_point2D &pt, int action)
    {if( !LR_Avant_contenu_dans_fils.Vide() ) {LR_Avant_contenu_dans_fils.Rappeler( this );}

     if(!Noeud_touchable()
       ||Ne_pas_afficher_fils() ) {if( !LR_Apres_contenu_dans_fils.Vide() ) {LR_Apres_contenu_dans_fils.Rappeler( this );}
                                   return NULL;
                                  }

     info_du_contenant *rep_deja_donne = (info_du_contenant*)NULL;

     if(Est_modifier_action_type()) action=action_transmise;
     alx_element_liste<alx_arbre<alx_noeud_scene*>*> *it     = L_fils.Premier()
                                                   , *it_fin = L_fils.Fin()
                                                   , *it_svg;
     info_du_contenant *rep;
     alx_noeud_scene *noeud;

     for(; it!=it_fin; it=it->svt)
     {noeud = (it->E())->e;                     // on définit le noeud fils
//      while(noeud->Est_verouille());
      if( !noeud->Gerer_contacts()
        ||!noeud->Noeud_touchable() ) continue;  // si il ne gère pas le contact on passe au suivant
      (it->E())->pere = this;
      if(noeud==Limite())                       // Si c'est la limite de parcours alors on avorte celui ci.
       {Avorter(true);
        if( !LR_Apres_contenu_dans_fils.Vide() ) {LR_Apres_contenu_dans_fils.Rappeler( this );}
        return NULL;
       }
      noeud->Limite_svg( noeud->Limite() );
      noeud->Limite( Limite() );
      alx_noeud_scene::Noeud_courant(noeud);
      if( (rep=noeud->Contient(pt, action)) != NULL )
       {it_svg = it->svt;
        // Si le fils est une réplique, on considère qu'on a cliqué sur un puit !
//        if( noeud->Est_une_replique() )
//         {rep = Rep_puit();
//          return rep;}
        // Gestion des méthodes de rappel de détection d'activité d'un pointeur :
        noeud->L_rappel_effective_pointeur_detecte.Vider();
        Ajouter_appels(rep, noeud, noeud->L_rappel_effective_pointeur_detecte);
        // Gestion du parcours
        noeud->Limite( noeud->Limite_svg() );
        if( (action&ALX_NOEUD_OPERATION_SI_CONTIENT_ALORS_PREMIER_PLAN)
          &&(noeud->Position_dans_liste_des_fils_changeable())
          &&(Position_des_fils_changeable())
          &&(rep != Rep_puit())
          &&(rep_deja_donne == NULL) )
         {L_fils.Ajouter_au_debut( it->E() );
          L_fils.Retirer( it );
          A_changer(true);
         }
        if(noeud->Avorter())                      // Si le fils avorte, on avorte aussi!
         {Avorter(true);
          noeud->Avorter(false);}                 // Le fils a fait son boulot, il oublie maintenant...

        // Si le noeud pointé dans la réponse à la propriété de cumulation des réponses
        // On continue le parcours en empilant les résultats. A chaque empilement,
        // on remplace le noeud par le dernier à avoir confirmé qu'il contenait le point considéré.
        if(rep_deja_donne) {// Fusionner la liste de rappel de rep_deja_donne avec celle de la nouvelle réponse
                            if(rep != Rep_puit())
                             {rep_deja_donne->L_Reponses_cumulees().Ajouter_a_la_fin( rep );
                              rep_deja_donne->Empiler_contenant( rep->NOEUD()->Empiler_contenant() );}
                           } else {if(rep->NOEUD() == noeud)
                                     rep->Empiler_contenant( rep->NOEUD()->Empiler_contenant() );
                                   if(this->Forcer_a_ne_plus_empiler_contenant())
                                      rep->Empiler_contenant( false );
                                   rep_deja_donne = rep;}
        if( rep_deja_donne->Empiler_contenant()
          ||(action&Op_Contient_alors_accumuler()) )
         {it = it_svg->prcdt;}
         else {if( !LR_Apres_contenu_dans_fils.Vide() ) {LR_Apres_contenu_dans_fils.Rappeler( this );}
               alx_noeud_scene::Noeud_courant(this);
               return rep_deja_donne;  // On retourne le résultat tout de suite si le noeud n'autorise pas à emplier les résultats.
              }
       }
      alx_noeud_scene::Noeud_courant(this);
      noeud->Limite( noeud->Limite_svg() );
      if(noeud->Avorter())                      // Si le fils avorte, on avorte aussi!
       {Avorter(true);
        noeud->Avorter(false);                  // Le fils a fait son boulot, il oublie maintenant...
        if( !LR_Apres_contenu_dans_fils.Vide() ) {LR_Apres_contenu_dans_fils.Rappeler( this );}
        return NULL;}                           // Bon...on se casse...
     }

 alx_noeud_scene::Noeud_courant(this);
 if( !LR_Apres_contenu_dans_fils.Vide() ) {LR_Apres_contenu_dans_fils.Rappeler( this );}
 return rep_deja_donne; // NULL si on a rien vu passer, une réponse sinon.
}

//______________________________________________________________________________
void alx_noeud_scene::Calculer_boite_noeud()
{// Mise à jour des coordonnées de la boîte englobante dans les repères des pères.
 boite_noeud_et_fils.maj( boite_fils );
 boite_noeud_et_fils_glob.maj( boite_noeud_et_fils );
 boite_noeud_et_fils_glob.Prendre_en_compte( *this );
}
//______________________________________________________________________________
void alx_noeud_scene::Calculer_boite_fils()
{alx_element_liste<alx_arbre<alx_noeud_scene*>*> *it     = L_fils.Dernier()
                                               , *it_deb = L_fils.Debut();
 alx_noeud_scene *noeud;
 boite_fils.Est_vide(true);
 for(; it!=it_deb; it=it->prcdt)
  {noeud = it->E()->e;
   if(noeud->Afficher_noeud()) boite_fils.Inclure_boite( noeud->Boite_noeud_et_fils_glob() );}
}

//______________________________________________________________________________
void alx_noeud_scene::Transfo_repere()
{
  if(Prendre_en_compte_repere())
   {glPushMatrix();
      switch(Ordre_rendu())
       {case 0: Changement_repere( *this );               break;
        case 1: Changement_repere_ordre_inverse( *this ); break;
       }
   }
}

//______________________________________________________________________________
void alx_noeud_scene::Depiler_transfo_repere()
{if(Prendre_en_compte_repere())
   glPopMatrix();
}

//______________________________________________________________________________
/*
void alx_noeud_scene::Afficher()
{if(Nb_affichage() > Nb_max_affichage()) return;
 Propager_contexte();
 if( !LR_Avant_affichage.Vide() ) {LR_Avant_affichage.Rappeler( this );}

 alx_noeud_scene::Noeud_courant(this);
   this->Transfo_repere();
   if(Noeud_puis_fils())
    {this->Dessin_noeud();
     this->Afficher_fils(0);
    } else {this->Afficher_fils(0);
            this->Dessin_noeud();}
   this->Depiler_transfo_repere();

// Mise à jour des coordonnées de la boîte englobante dans les repères des pères.
 Calculer_boite_noeud();
 if( !LR_Apres_affichage.Vide() ) {LR_Apres_affichage.Rappeler( this );}
}
*/
//______________________________________________________________________________
void alx_noeud_scene::Dessin()
{
 if(Mode_texture()) {this->Dessin_texture_noeud();} else {this->Dessin_noeud();}
}

//______________________________________________________________________________
void alx_noeud_scene::Dessin_noeud()
{}

//______________________________________________________________________________
const boite_englobante& alx_noeud_scene::get_boite_pour_texture()
{if(Boite_noeud_pour_texture()) {return Boite_noeud();}
 if(Mode_texture_fils()) {return Boite_noeud_et_fils();}
 return Boite_noeud();

//Boite_noeud()
//Boite_fils()
//Boite_noeud_et_fils()
}

//______________________________________________________________________________
void alx_noeud_scene::Maj_boites_recursif()
{if(maj_boites_recursif) return;

 alx_element_liste<alx_arbre<alx_noeud_scene*>*> *it     = L_fils.Premier()
                                               , *it_fin = L_fils.Fin();
 maj_boites_recursif = true;
   for(; it!=it_fin; it=it->svt)
    {it->E()->e->Maj_boites_recursif();
    }
   Calculer_boites();
 maj_boites_recursif = false;
}

//______________________________________________________________________________
const bool alx_noeud_scene::Changement()
{if(parcours_changement) return false;
 if(A_changer()) return true;
 bool rep = false;
 alx_element_liste<alx_arbre<alx_noeud_scene*>*> *it     = L_fils.Premier()
                                               , *it_fin = L_fils.Fin();
 parcours_changement = true;
   for(; it!=it_fin; it=it->svt)
    {if(it->E()->e->Changement()) {rep = true; break;}
    }
 Calculer_boites();
 parcours_changement = false;
 return rep;
}

//______________________________________________________________________________
void alx_noeud_scene::Dessin_texture_noeud()
{if(zdr == NULL) {
   return;
  }

 glPushAttrib(GL_ENABLE_BIT);
 if(Texture_translucide()) {glEnable(GL_BLEND);
                            glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
                            //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
                           }

 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, zdr->Infos_texture_tempon(0)->id_texture);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
 if(Lisser_texture())
  {glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   } else {glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
          }
 glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
 //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

 //XXX
 //  Activer_stencil(GL_lequal(), GL_incr(), Tempon());
 //  Incr_tempon();
 //XXX

 glBegin(GL_TRIANGLE_STRIP);
   Maj_boites_recursif();
   const boite_englobante &boite_pour_texture = get_boite_pour_texture();
   double V_tmp[2]
        , coord_x = zdr->Nb_unite_x()*zdr->Nb_pixels_par_unite()/zdr->Infos_texture_tempon(0)->tx
        , coord_y = zdr->Nb_unite_y()*zdr->Nb_pixels_par_unite()/zdr->Infos_texture_tempon(0)->ty
        , px = boite_pour_texture.BG_X()
        , py = boite_pour_texture.BG_Y()
        , tx = boite_pour_texture.Tx()
        , ty = boite_pour_texture.Ty();
   glColor4dv( Couleur_masque_texture() );
   V_tmp[0]=0; V_tmp[1]=0; glTexCoord2d(0      , 0      ); glVertex2d(px   , py   );
   V_tmp[0]=0; V_tmp[1]=0; glTexCoord2d(coord_x, 0      ); glVertex2d(px+tx, py   );
   V_tmp[0]=0; V_tmp[1]=0; glTexCoord2d(0      , coord_y); glVertex2d(px   , py+ty);
   V_tmp[0]=0; V_tmp[1]=0; glTexCoord2d(coord_x, coord_y); glVertex2d(px+tx, py+ty);
 glEnd();

 // XXX
 //glDisable(GL_TEXTURE_2D);
 //Op_stencil(GL_keep(), GL_keep());
 //Fct_stencil(GL_lequal(), Tempon());
 //  this->Afficher_fils(0);
 //Depiler_stencil();
 // XXX


glPopAttrib();
}

//______________________________________________________________________________
const bool alx_noeud_scene::Pipoter_affichage()
{if(pipoter_affichage) {
   heritage_de_pipoter_affichage = true;
  } else {if(Pere()) heritage_de_pipoter_affichage = Pere()->heritage_de_pipoter_affichage; else heritage_de_pipoter_affichage = false;
         }
 return heritage_de_pipoter_affichage;
}

//______________________________________________________________________________
void alx_noeud_scene::Afficher()
{if(Nb_affichage() > Nb_max_affichage()) return;
 if( !LR_Avant_affichage.Vide() ) {LR_Avant_affichage.Rappeler( this );}

 alx_noeud_scene::Noeud_courant(this);
 if(!Pipoter_affichage())
  {
   this->Transfo_repere();
   if(Noeud_puis_fils())
   {if(Mode_texture()) {this->Dessin_texture_noeud();
                        if(!Mode_texture_fils()) {this->Afficher_fils(0);}
                        } else {this->Dessin_noeud();
                                this->Afficher_fils(0);
                               }
    } else {if(Mode_texture()) {
              if(!Mode_texture_fils()) {this->Afficher_fils(0);}
              this->Dessin_texture_noeud();
             } else {this->Afficher_fils(0);
                     this->Dessin_noeud();
                    }
           }
   this->Depiler_transfo_repere();
  } else {this->Afficher_fils(0);}

// Mise à jour des coordonnées de la boîte englobante dans les repères des pères.
 Calculer_boite_noeud();
 if( !LR_Apres_affichage.Vide() ) {LR_Apres_affichage.Rappeler( this );}
}
//------------------------------------------------------------------------------
void alx_noeud_scene::Afficher_fils(int add_val_stencil)
{if( !LR_Avant_affichage_fils.Vide() ) {LR_Avant_affichage_fils.Rappeler( this );}

 if( Ne_pas_afficher_fils() )
  {if( !LR_Apres_affichage_fils.Vide() ) {LR_Apres_affichage_fils.Rappeler( this );}
   return;
  }

 alx_element_liste<alx_arbre<alx_noeud_scene*>*> *it     = L_fils.Dernier()
                                               , *it_deb = L_fils.Debut();
 alx_noeud_scene *noeud;
 int nv_stencil = val_stencil + add_val_stencil;
 //boite_fils.Est_vide(true);
 for(; it!=it_deb; it=it->prcdt)
  {noeud = (it->E())->e;
//   while(noeud->Est_verouille());
   if( noeud==Limite() )
    {if( noeud->Limite_nb_recursions() > 0 )
      {noeud->Limite_nb_recursions( noeud->Limite_nb_recursions()-1 );}
      else
       {Avorter(true);
        if( !LR_Apres_affichage_fils.Vide() ) {LR_Apres_affichage_fils.Rappeler( this );}
        return;
       }
    }
   if( !noeud->Afficher_noeud()
     ||(noeud->Nb_affichage() > noeud->Nb_max_affichage()) ) continue;
   if( noeud->Compter_affichage() )
     noeud->Nb_affichage( noeud->Nb_affichage()+1 );
   noeud->pere = this;
   noeud->val_stencil = nv_stencil;

   noeud->Limite_svg( noeud->Limite() );
   noeud->Limite( Limite() );
   noeud->Calculer_boites();
     noeud->Afficher();
     if(tenir_compte_des_boites_des_fils)
      {boite_fils.Inclure_boite( noeud->Boite_noeud_et_fils_glob() );
       if( noeud->Boite_noeud_et_fils_glob().Est_a_afficher()
         ||noeud->Afficher_boite_noeud()
         ||Afficher_boites_fils())
         noeud->Boite_noeud_et_fils_glob().Afficher();
      }
   noeud->Limite( noeud->Limite_svg() );
   if(noeud->Avorter())                      // Si le fils avorte, on avorte aussi!
    {Avorter(true);
     noeud->Avorter(false);                  // Le fils a fait son boulot, il oublie maintenant...
     break ;}                               // Bon...on se casse...
  }

 if( !LR_Apres_affichage_fils.Vide() ) {LR_Apres_affichage_fils.Rappeler( this );}
 if(Afficher_boite_noeud())
   Boite_noeud_et_fils().Afficher();
 if(boite_fils.Est_a_afficher())
   {boite_fils.Afficher();}

}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
void alx_noeud_scene::Vider_peres() {
 alx_arbre<alx_noeud_scene*>::Vider_peres();
}
//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
/*
void alx_noeud_scene::PreRendre()
{if( Ne_pas_pre_rendre()
   ||(Nb_pre_rendu() >= Nb_max_pre_rendu())) return;
 PreRendre_fils();
 if( L_fils.Vide() )
  {boite_noeud_et_fils_glob.Est_vide(true);
   return;}
}
*/
//______________________________________________________________________________
const bool alx_noeud_scene::Maj_texture()
{if(maj_texture) return true;

 bool changer = A_changer(), rep;
 A_changer(false);
   parcours_changement = false;
   rep = Changement();
 A_changer(changer);
 return rep;
}

//______________________________________________________________________________
void alx_noeud_scene::PreRendre()
{if( Ne_pas_pre_rendre()
   ||(Nb_pre_rendu() > Nb_max_pre_rendu())) return;

 if( !LR_Avant_pre_rendu.Vide() ) {LR_Avant_pre_rendu.Rappeler( this );}

if(!Pipoter_affichage())
{
 if( Mode_texture() )
  {const bool pre_rendu_exeptionnel_SVG = Pre_rendu_exeptionnel();
   PreRendre_fils();
   Pre_rendu_exeptionnel( pre_rendu_exeptionnel_SVG );
   if (Forcer_pre_rendu_noeud() || Pre_rendu_exeptionnel() || Maj_texture())
    {Forcer_pre_rendu_noeud(false); Pre_rendu_exeptionnel(false);
     /*DEBUG*/Maj_boites_recursif();
     const boite_englobante &boite_pour_texture = get_boite_pour_texture();
     zdr->Nb_unite_x( boite_pour_texture.Tx() );
     zdr->Nb_unite_y( boite_pour_texture.Ty() );
     Mode_texture(false);
       repere_tmp.maj( *this );
       alx_repere2D::maj(-boite_pour_texture.BG_X(), -boite_pour_texture.BG_Y(), 0, 1, 1);
         //Ne_pas_pre_rendre(true);

         const bool b_pas_aff_fils = Ne_pas_afficher_fils();
         if(!Mode_texture_fils()) {Ne_pas_afficher_fils(true);}
           if( !zdr->A_pour_fils( this ) ) {
             zdr->Ajouter_fils( this );
            }
           zdr->PreRendre();
           //zdr->Infos_texture_tempon()->taille_reelle[0] = zdr->Nb_unite_x()*zdr->Nb_pixels_par_unite();
           //zdr->Infos_texture_tempon()->taille_reelle[1] = zdr->Nb_unite_y()*zdr->Nb_pixels_par_unite();
         if(!Mode_texture_fils()) {Ne_pas_afficher_fils(b_pas_aff_fils);}
         //Ne_pas_pre_rendre(false);
       alx_repere2D::maj( repere_tmp );
     Mode_texture(true);
     maj_texture = false;
     A_changer(true);
     Forcer_pre_rendu_noeud(false);
    }
  } else {PreRendre_fils();
         }
} else {PreRendre_fils();}

 if( !LR_Apres_pre_rendu.Vide() ) {LR_Apres_pre_rendu.Rappeler( this );}
}

//______________________________________________________________________________
void alx_noeud_scene::PreRendre_fils()
{if( !LR_Avant_pre_rendu_fils.Vide() ) {LR_Avant_pre_rendu_fils.Rappeler( this );}
 if( Ne_pas_pre_rendre_fils() )
  {if( !LR_Apres_pre_rendu_fils.Vide() ) {LR_Apres_pre_rendu_fils.Rappeler( this );}
   return;
  }


// On parcours...
 alx_element_liste<alx_arbre<alx_noeud_scene*>*> *it     = L_fils.Dernier()
                                               , *it_deb = L_fils.Debut();
 alx_noeud_scene *noeud;
 //boite_fils.Est_vide(true);
 for(; it!=it_deb; it=it->prcdt)
  {noeud = (it->E())->e;

//   while(noeud->Est_verouille());
   if( noeud==Limite() )
    {if( noeud->Limite_nb_recursions() > 0 )
      {noeud->Limite_nb_recursions( noeud->Limite_nb_recursions()-1 );}
      else
       {Avorter(true);
        break;}
    }
   if( ( noeud->Ne_pas_pre_rendre()
       ||(noeud->Nb_pre_rendu() > noeud->Nb_max_pre_rendu()))
     &&(!noeud->Pre_rendu_exeptionnel()) ) continue;
   if( noeud->Compter_pre_rendu() )
     noeud->Nb_pre_rendu( noeud->Nb_pre_rendu()+1 );
   noeud->pere = this;

   noeud->Limite_svg( noeud->Limite() );
   noeud->Limite( Limite() );
     alx_noeud_scene::Noeud_courant(noeud);
     noeud->PreRendre();
     A_changer( A_changer() || noeud->A_changer() );
     alx_noeud_scene::Noeud_courant(this);
   noeud->Limite( noeud->Limite_svg() );
   Pre_rendu_exeptionnel(false);
   if(noeud->Avorter())                      // Si le fils avorte, on avorte aussi!
    {Avorter(true);
     noeud->Avorter(false);                  // Le fils a fait son boulot, il oublie maintenant...
     break ;}                               // Bon...on se casse...
  }
 if( !LR_Apres_pre_rendu_fils.Vide() ) {LR_Apres_pre_rendu_fils.Rappeler( this );}
}

//------------------------------------------------------------------------------
void alx_noeud_scene::Transformer_point_depuis_racine(alx_point2D &pt)
{alx_liste<alx_repere2D*> L_rep;
 Donner_liste_repere( L_rep );

 alx_element_liste<alx_repere2D*> *it     = L_rep.Premier()
                                , *it_fin = L_rep.Fin();
 for(;it!=it_fin; it++)
   (it->E())->Changer_coordonnees(pt);
}

//------------------------------------------------------------------------------
void alx_noeud_scene::Donner_liste_repere(alx_liste<alx_repere2D*> &L_repere)
{L_repere.Ajouter_au_debut( this );
 Donner_liste_repere_moins_courant(L_repere);}

//------------------------------------------------------------------------------
void alx_noeud_scene::Transformer_point_depuis_racine(alx_point2D &pt, alx_noeud_scene *racine)
{alx_liste<alx_repere2D*> L_rep;
 Donner_liste_repere(L_rep, racine);

 alx_element_liste<alx_repere2D*> *it     = L_rep.Premier()
                                , *it_fin = L_rep.Fin();
 for(;it!=it_fin; it=it->svt)
   (it->E())->Changer_coordonnees(pt);
}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
bool alx_noeud_scene::A_pour_L_fils(const alx_liste<alx_arbre<alx_noeud_scene*>*> &L)
{alx_element_liste<alx_arbre<alx_noeud_scene*>*> *it     = L.Premier()
                                               , *it_fin = L.Fin();
 for(; it!=it_fin; it=it->svt)
  {if(!this->A_pour_fils( (alx_noeud_scene*)it->E() ) )
     return false;
  }
 return true;
}
//______________________________________________________________________________
void alx_noeud_scene::Ajouter_L_fils_au_debut(const alx_liste<alx_arbre<alx_noeud_scene*>*> &L)
{alx_element_liste<alx_arbre<alx_noeud_scene*>*> *it     = L.Dernier()
                                               , *it_deb = L.Debut();
 for(; it!=it_deb; it=it->prcdt)
  {this->Ajouter_fils_au_debut( (alx_noeud_scene*)it->E() );
  }
}
//______________________________________________________________________________
void alx_noeud_scene::Ajouter_L_fils_a_la_fin(const alx_liste<alx_arbre<alx_noeud_scene*>*> &L)
{alx_element_liste<alx_arbre<alx_noeud_scene*>*> *it     = L.Premier()
                                               , *it_fin = L.Fin();
 for(; it!=it_fin; it=it->svt)
  {this->Ajouter_fils_a_la_fin( (alx_noeud_scene*)it->E() );
  }
}
//______________________________________________________________________________
void alx_noeud_scene::Ajouter_L_fils_replique(const alx_liste<alx_arbre<alx_noeud_scene*>*> &L)
{alx_element_liste<alx_arbre<alx_noeud_scene*>*> *it     = L.Premier()
                                               , *it_fin = L.Fin();
 for(; it!=it_fin; it=it->svt)
  {this->Ajouter_fils_replique( (alx_noeud_scene*)it->E() );
  }
}
//______________________________________________________________________________
void alx_noeud_scene::Ajouter_L_fils(const alx_liste<alx_arbre<alx_noeud_scene*>*> &L)
{alx_element_liste<alx_arbre<alx_noeud_scene*>*> *it     = L.Premier()
                                               , *it_fin = L.Fin();
 for(; it!=it_fin; it=it->svt)
  {this->Ajouter_fils( (alx_noeud_scene*)it->E() );
  }
}
//______________________________________________________________________________
void alx_noeud_scene::Ajouter_L_fils( const alx_liste<alx_arbre<alx_noeud_scene*>*> &L
                                  , alx_element_liste<alx_arbre<alx_noeud_scene*>*> &it_pos)
{alx_element_liste<alx_arbre<alx_noeud_scene*>*> *it     = L.Premier()
                                               , *it_fin = L.Fin();
 for(; it!=it_fin; it=it->svt)
  {this->Ajouter_fils_it( (alx_noeud_scene*)it->E(), it_pos);
  }
}

//______________________________________________________________________________
void alx_noeud_scene::Retirer_L_fils(const alx_liste<alx_arbre<alx_noeud_scene*>*> &L)
{alx_element_liste<alx_arbre<alx_noeud_scene*>*> *it     = L.Dernier()
                                               , *it_deb = L.Debut();
 for(; it!=it_deb; it=it->prcdt)
  {this->Ajouter_fils_au_debut( (alx_noeud_scene*)it->E() );
  }
}


//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
void alx_noeud_scene::Maj_etirement_absolu()
{alx_liste<alx_repere2D*> L_repere;
 Donner_liste_repere(L_repere);

 alx_element_liste<alx_repere2D*> *it     = L_repere.Premier()
                                , *it_fin = L_repere.Fin();
 Etirement(1,1); // Par convention l'étirement au départ est de 1 pour chaque axe
 for(;it!=it_fin;it=it->svt)
  {Etirer_de( (it->E())->Etirement() );}
}

void alx_noeud_scene::Donner_liste_repere(alx_liste<alx_repere2D*> &L_repere, alx_noeud_scene *racine)
{alx_noeud_scene *cour = this;
 L_repere.Ajouter_au_debut( this );

 while( (cour->pere != NULL)
      &&(cour != racine) )
  {// Changer de noeud:
   cour = cour->pere->e;
   // Ajouter le repère:
   L_repere.Ajouter_au_debut( (alx_repere2D*)cour );
  }
}

void alx_noeud_scene::Donner_liste_repere_moins_courant(alx_liste<alx_repere2D*> &L_repere)
{alx_noeud_scene *cour = this;

 while( cour->pere != NULL )
  {// Changer de noeud:
   cour = cour->pere->e;
   // Ajouter le repère:
   L_repere.Ajouter_au_debut( (alx_repere2D*)cour );}
}

//______________________________________________________________________________
void alx_noeud_scene::Transformer_point_depuis_racine_sauf_noeud_courant(alx_point2D &pt)
{alx_liste<alx_repere2D*> L_rep;
 Donner_liste_repere_moins_courant(L_rep);

 alx_element_liste<alx_repere2D*> *it     = L_rep.Premier()
                                , *it_fin = L_rep.Fin();
 it_fin--;
 for(;it!=it_fin; it=it->svt)
   (it->E())->Changer_coordonnees(pt);
}

//______________________________________________________________________________
void alx_noeud_scene::Transformer_point_depuis_racine_sauf_noeud_courant(alx_point2D &pt, alx_noeud_scene *racine)
{alx_liste<alx_repere2D*> L_rep;
 Donner_liste_repere(L_rep, racine);

 alx_element_liste<alx_repere2D*> *it     = L_rep.Premier()
                                , *it_fin = L_rep.Fin();
 it_fin--;
 for(;it!=it_fin; it=it->svt)
   (it->E())->Changer_coordonnees(pt);
}

//______________________________________________________________________________
void alx_noeud_scene::Intercaler_noeud(alx_noeud_scene *noeud)
{alx_element_liste<alx_arbre<alx_noeud_scene*>*> *it     = L_fils.Premier()
                                               , *it_fin = L_fils.Fin();
 for(; it!=it_fin; it=it->svt)
  {noeud->Ajouter_fils_a_la_fin( it->E()->E() );}
 Vider_fils();
 Ajouter_fils( noeud );
}

//______________________________________________________________________________
void alx_noeud_scene::Ajouter_fils_avant(alx_noeud_scene *noeud, alx_noeud_scene *noeud_fils)
{alx_noeud_scene *n_tmp;
 alx_element_liste<alx_arbre<alx_noeud_scene*>*> *it     = L_fils.Premier()
                                               , *it_fin = L_fils.Fin();
 for(; it!=it_fin; it=it->svt)
  {n_tmp = it->E()->e;
   if(n_tmp == noeud_fils) {
     Ajouter_fils_it(noeud, *it);
     return;
    }
  }
}

//______________________________________________________________________________
void alx_noeud_scene::Intercaler_noeud_entre(alx_noeud_scene *noeud_pere, alx_noeud_scene *noeud_fils)
{Vider_peres();
 Vider_fils();
 noeud_pere->Ajouter_fils_avant(this, noeud_fils);
 noeud_pere->Retirer_fils( noeud_fils );
 this->Ajouter_fils(noeud_fils);
}

//______________________________________________________________________________
void alx_noeud_scene::Retirer_et_relier()
{alx_noeud_scene *n_tmp;
 alx_element_liste<alx_arbre<alx_noeud_scene*>*> *it     = L_peres.Premier()
                                               , *it_fin = L_peres.Fin();
 alx_element_liste<alx_arbre<alx_noeud_scene*>*> *it_p
                                               , *it_p_fin;
 for(; it!=it_fin; it=it->svt)
  {n_tmp = it->E()->e;
   it_p     = n_tmp->L_fils.Premier();
   it_p_fin = n_tmp->L_fils.Fin();
   for(; it_p!=it_p_fin; it_p=it_p->svt)
    {n_tmp->Ajouter_fils_avant(it_p->E()->e, this);
    }
  }
 Vider_peres();
 Vider_fils();
}

//______________________________________________________________________________
const unsigned int alx_noeud_scene::Chemin_ancetre( const alx_noeud_scene *racine
                                                  , alx_liste<const alx_noeud_scene*> &L_rep)
{
 L_rep.Ajouter_au_debut(this);
 if (this == racine)     return 1;
 if (this->pere == NULL) return 0;
 return this->pere->e->Chemin_ancetre(racine, L_rep);
}

//______________________________________________________________________________
void alx_noeud_scene::maj_etirement_courant() const
{
 if(pere) {
   alx_repere2D::Etirement_courant( pere->e->etirement_noeud_svg );
  }
 alx_repere2D::maj_etirement_courant();
 etirement_noeud_svg.maj( alx_repere2D::Etirement_courant() );
}
//______________________________________________________________________________
//______________________________ Les opérateurs ________________________________
//______________________________________________________________________________
alx_noeud_scene& alx_noeud_scene::operator+=(const alx_liste<alx_arbre<alx_noeud_scene*>*> &l)  // Surcharge du  +=
{L_fils.Ajouter_au_debut( l );
 return *this;}

alx_noeud_scene& alx_noeud_scene::operator+=(alx_noeud_scene *e)  // Surcharge du  +=
{L_fils.Ajouter_au_debut(e);
 return *this;}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
const char* alx_noeud_scene::CC_Val(const alx_chaine_char &var) const
{const char *rep = M_contexte_local.Val_CC( var );
 if ( rep == (const char*)NULL && Pere_const() ) {
   return Pere_const()->CC_Val( var );
  } else {return rep;}
}

//______________________________________________________________________________
const char* alx_noeud_scene::Val(const char *var) const
{cc_var_tmp = var;
 return CC_Val( cc_var_tmp );
}




/*unsigned int pos_liste_noeuds = 0;
alx_liste<alx_arbre<alx_noeud_scene*>*> Tab_liste_noeuds[32];
inline const unsigned int Pos_liste_noeuds()
{unsigned int rep = pos_liste_noeuds;
 pos_liste_noeuds = (++pos_liste_noeuds) % 32;
 return rep;}
//______________________________________________________________________________

alx_liste<alx_arbre<alx_noeud_scene*>*>& operator+(alx_noeud_scene *a, alx_noeud_scene *b)
{Tab_liste_noeuds[pos_liste_noeuds].Vider();
 Tab_liste_noeuds[pos_liste_noeuds].Ajouter_a_la_fin(&a);
 Tab_liste_noeuds[pos_liste_noeuds].Ajouter_a_la_fin(&b);
 return Tab_liste_noeuds[Pos_liste_noeuds()];}

//______________________________________________________________________________
alx_liste<alx_arbre<alx_noeud_scene*>*>& operator+(alx_noeud_scene *a, alx_liste<alx_arbre<alx_noeud_scene*>*> &l)
{l.Ajouter_au_debut(&a);
 return l;}

//______________________________________________________________________________
alx_liste<alx_arbre<alx_noeud_scene*>*>& operator+(alx_liste<alx_arbre<alx_noeud_scene*>*> &l, alx_noeud_scene *b)
{l.Ajouter_a_la_fin(&b);
 return l;}

//______________________________________________________________________________
alx_liste<alx_arbre<alx_noeud_scene*>*>& operator+(const alx_liste<alx_arbre<alx_noeud_scene*>*> &l1, const alx_liste<alx_arbre<alx_noeud_scene*>*> &l2)
{Tab_liste_noeuds[pos_liste_noeuds].Vider();
 Tab_liste_noeuds[pos_liste_noeuds].Ajouter_au_debut(l1);
 Tab_liste_noeuds[pos_liste_noeuds].Ajouter_a_la_fin(l2);
 return Tab_liste_noeuds[Pos_liste_noeuds()];}

*/

//______________________________________________________________________________
void Ajouter_noeud_en_debut_de_liste_rep( alx_liste<alx_repere2D*> &L_rep
                                        , alx_noeud_scene *noeud )
{L_rep.Ajouter_au_debut( noeud );
}

//______________________________________________________________________________
void Ajouter_noeud_en_fin_de_liste_rep( alx_liste<alx_repere2D*> &L_rep
                                        , alx_noeud_scene *noeud )
{L_rep.Ajouter_a_la_fin( noeud );
}

//______________________________________________________________________________
void Lister_partie_index( const alx_liste<alx_repere2D*>         &L_rep
                        , const unsigned int prem
                        , const unsigned int der
                        , alx_liste<alx_repere2D*> &L_res)
{L_res.Vider();
 alx_element_liste<alx_repere2D*> *it_p = L_rep.It(prem)
                                , *it_d = L_rep.It(der);
 Lister_partie(L_rep, it_p, it_d, L_res);
}

//______________________________________________________________________________
void Lister_partie_noeud( const alx_liste<alx_repere2D*>         &L_rep
                        , alx_noeud_scene* premier
                        , alx_noeud_scene* dernier
                        , alx_liste<alx_repere2D*> &L_res)
{L_res.Vider();
 alx_element_liste<alx_repere2D*> *it_p = L_rep.It(premier)
                                , *it_d = L_rep.It(dernier);
 Lister_partie(L_rep, it_p, it_d, L_res);
}


//______________________________________________________________________________
bool Prise_en_compte_des_reperes_ou_pas(alx_noeud_scene **e, void *info_comp)
{unsigned int m = *((unsigned int *)info_comp);
 if( m == (*e)->Marqueur_de_parcours() ) return false;

 (*e)->Marqueur_de_parcours( m );

 const bool a_changer_tmp = (*e)->A_changer();
 if( (*e)->Est_entite() ) {
   (*e)->Prendre_en_compte_repere(false);
  }

 (*e)->A_changer( a_changer_tmp );

 return true;
}


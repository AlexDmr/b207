#include "alx_noeud_zone_texte_sdl_opengl.h"
#include <glew.h>
#include "../../opengl/config_opengl.h"
#include "../fontes.h"

alx_noeud_zone_texte_sdl_opengl::alx_noeud_zone_texte_sdl_opengl( const double tx, const double ty
                                                                , const double taille_lettres
                                                                , gl_fonte *fonte
                                                                , alx_simulateur_dispositif_saisie *sds ) : classe_serialisation( this
                                                                                                                     , &serialisation_type_zt
                                                                                                                     , &nouv_serialisation_type
                                                                                                                     , &serialisation_differences_type_zt )
{zone_gl = new alx_zone_texte_opengl(tx, ty, fonte, sds);
 zone_gl->Zoom(taille_lettres);
 zone_gl->repere = (alx_repere2D*)this;
 Nom_classe("zone_txt_bigre");
 Nom_IU("zone de texte");
 init_ptr();

 boite_noeud.Est_vide( false );
 boite_fils.Est_vide( false );
 boite_noeud_et_fils.Est_vide( false );
 boite_noeud_et_fils_glob.Est_vide( false );

 MP( zone_gl->Mp_corp() );
}

//______________________________________________________________________________
alx_noeud_zone_texte_sdl_opengl::alx_noeud_zone_texte_sdl_opengl( alx_zone_texte_opengl *zgl ) : classe_serialisation( this
                                                                                                                     , &serialisation_type_zt
                                                                                                                     , &nouv_serialisation_type
                                                                                                                     , &serialisation_differences_type_zt )
{zone_gl = zgl;
 zone_gl->repere = (alx_repere2D*)this;
 Nom_classe("zone_txt_bigre");
 init_ptr();

 boite_noeud.Est_vide( false );
 boite_fils.Est_vide( false );
 boite_noeud_et_fils.Est_vide( false );
 boite_noeud_et_fils_glob.Est_vide( false );

 MP( zone_gl->Mp_corp() );
}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
void alx_noeud_zone_texte_sdl_opengl::Couleur_fond(const double r, const double v, const double b, const double a)
{zone_gl->Couleur_fond(r,v,b,a);
 zdr->Couleur(r,v,b,a);
 A_changer(true);
 Maj_texture(true);
}

//______________________________________________________________________________
void alx_noeud_zone_texte_sdl_opengl::Couleur_fond(const double *c)
{zone_gl->Couleur_fond(c);
 zdr->Couleur(c);
 A_changer(true);
 Maj_texture(true);
}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
/*
void alx_noeud_zone_texte_sdl_opengl::PreRendre()
{PreRendre_fils();
 if (Mode_texture() && Maj_texture())
  {zdr->Nb_unite_x( Largeur() );
   zdr->Nb_unite_y( Hauteur() );
   zdr->Nb_pixels_par_unite( 1 );
   Mode_texture(false);
     bool b = this->Prendre_en_compte_repere();
     this->Prendre_en_compte_repere(false);
     zdr->PreRendre();
     this->Prendre_en_compte_repere(b);
   Mode_texture(true);
   Maj_texture( false );
  }
}
*/
//______________________________________________________________________________
void alx_noeud_zone_texte_sdl_opengl::Dessin_noeud()
{zone_gl->afficher();
}

//______________________________________________________________________________
// Renvoi un pointeur sur le modèle physique qui contient la primitive, NULL si aucun.
//______________________________________________________________________________
info_du_contenant* alx_noeud_zone_texte_sdl_opengl::Noeud_contient(const alx_point2D &point, int action)
{if(boite_noeud.Contient(point) )
  {info_du_contenant *rep = new info_du_contenant();
   rep->mp    = zone_gl->Mp_corp();
   rep->noeud = (alx_noeud_scene*)this;
   rep->Point_au_contact(point);
   return rep;
  }
 return (info_du_contenant*)NULL;
}

//______________________________________________________________________________
//_________________________ Les calculs des boites _____________________________
//______________________________________________________________________________
void alx_noeud_zone_texte_sdl_opengl::Calculer_boite_noeud()
{// Mise à jour des coordonnées de la boîte englobante dans les repères des pères.
 double dt = zone_gl->Fonte()->HAUTEUR()*zone_gl->Zoom()/5;
 boite_noeud.maj(-dt, -dt, Largeur()+dt, Hauteur());

 boite_noeud_et_fils.maj( boite_noeud );
 boite_noeud_et_fils.Inclure_boite( boite_fils );
 boite_noeud_et_fils_glob.maj(boite_noeud_et_fils);
 boite_noeud_et_fils_glob.Prendre_en_compte( *this );
}

//______________________________________________________________________________
//___________________ Les surcharges pour la sérialisation _____________________
//______________________________________________________________________________
void alx_noeud_zone_texte_sdl_opengl::init_ptr()
{P_void *f_diff   = classe_serialisation.f_diff
      , *f_deseri = classe_serialisation.f_deseri;

 mode_texture = maj_texture = false;
 Couleur_fond(0,0,0,0);
 Translucide(true);



 classe_serialisation.fcc[0]   = &alx_noeud_zone_texte_sdl_opengl::Maj_texte;                    f_deseri[0] = &(classe_serialisation.fcc[0]);
 classe_serialisation.fsd_cc[0]= &alx_noeud_zone_texte_sdl_opengl::Seri_diff_Texte_a_ete_change; f_diff[0]   = &(classe_serialisation.fsd_cc[0]);

 classe_serialisation.fd1[0]   = &alx_noeud_zone_texte_sdl_opengl::Hauteur;    f_deseri[1] = &(classe_serialisation.fd1[0]);
 classe_serialisation.fsd_d1[0]= &alx_noeud_zone_texte_sdl_opengl::Hauteur;    f_diff[1]   = &(classe_serialisation.fsd_d1[0]);
 classe_serialisation.fd1[1]   = &alx_noeud_zone_texte_sdl_opengl::Largeur;    f_deseri[2] = &(classe_serialisation.fd1[1]);
 classe_serialisation.fsd_d1[1]= &alx_noeud_zone_texte_sdl_opengl::Largeur;    f_diff[2]   = &(classe_serialisation.fsd_d1[1]);
 classe_serialisation.fd1[2]   = &alx_noeud_zone_texte_sdl_opengl::Zoom;       f_deseri[3] = &(classe_serialisation.fd1[2]);
 classe_serialisation.fsd_d1[2]= &alx_noeud_zone_texte_sdl_opengl::Zoom;       f_diff[3]   = &(classe_serialisation.fsd_d1[2]);

 classe_serialisation.fb1[0]   = &alx_noeud_zone_texte_sdl_opengl::Afficher_fond; f_deseri[4] = &(classe_serialisation.fb1[0]);
 classe_serialisation.fsd_b1[0]= &alx_noeud_zone_texte_sdl_opengl::Afficher_fond; f_diff[4]   = &(classe_serialisation.fsd_b1[0]);
 classe_serialisation.fb1[1]   = &alx_noeud_zone_texte_sdl_opengl::Editable; f_deseri[5] = &(classe_serialisation.fb1[1]);
 classe_serialisation.fsd_b1[1]= &alx_noeud_zone_texte_sdl_opengl::Editable; f_diff[5]   = &(classe_serialisation.fsd_b1[1]);
 classe_serialisation.fb1[2]   = &alx_noeud_zone_texte_sdl_opengl::Lisser; f_deseri[6] = &(classe_serialisation.fb1[2]);
 classe_serialisation.fsd_b1[2]= &alx_noeud_zone_texte_sdl_opengl::Lisser; f_diff[6]   = &(classe_serialisation.fsd_b1[2]);

 classe_serialisation.fi1[0]   = &alx_noeud_zone_texte_sdl_opengl::Position_curseur;f_deseri[7] = &(classe_serialisation.fi1[0]);
 classe_serialisation.fsd_i1[0]= &alx_noeud_zone_texte_sdl_opengl::Position_curseur;f_diff[7] = &(classe_serialisation.fsd_i1[0]);
 classe_serialisation.fi1[1]   = &alx_noeud_zone_texte_sdl_opengl::Position_curseur;f_deseri[8] = &(classe_serialisation.fi1[1]);
 classe_serialisation.fsd_i1[1]= &alx_noeud_zone_texte_sdl_opengl::Position_curseur;f_diff[8] = &(classe_serialisation.fsd_i1[1]);

 classe_serialisation.fd4[0]   = &alx_noeud_zone_texte_sdl_opengl::Couleur_texte; f_deseri[9] = &(classe_serialisation.fd4[0]);
 classe_serialisation.fsd_d4[0]= &alx_noeud_zone_texte_sdl_opengl::Couleur_texte; f_diff[9]   = &(classe_serialisation.fsd_d4[0]);
 classe_serialisation.fd4[1]   = &alx_noeud_zone_texte_sdl_opengl::Couleur_curseur; f_deseri[10] = &(classe_serialisation.fd4[1]);
 classe_serialisation.fsd_d4[1]= &alx_noeud_zone_texte_sdl_opengl::Couleur_curseur; f_diff[10]   = &(classe_serialisation.fsd_d4[1]);
}
//______________________________________________________________________________
//_____________________________ La différence __________________________________
//______________________________________________________________________________
const bool alx_noeud_zone_texte_sdl_opengl::Seri_diff_Texte_a_ete_change(const alx_chaine_char **ptr_cc) const
{return zone_gl->Seri_diff_Texte_a_ete_change(ptr_cc);
}

//______________________________________________________________________________
const alx_chaine_char& alx_noeud_zone_texte_sdl_opengl::Serialiser_difference_type(const alx_chaine_char &diff_contenu) const
{if(serialisation_type_zt.Taille()==0)
  {Serialiser_type();
   serialisation_differences_type_zt = serialisation_type_zt;
   return serialisation_differences_type_zt;}

 serialisation_differences_type_zt.Maj("",0,0);
 nouv_serialisation_type.Maj("",0,0);
   unsigned int pos = Nom_classe().Taille()+2;
   pos = serialisation_type_zt.Position_du_prochain(';', pos); pos++; //On passe nom=...;
   seri_diff_type.Maj("",0,0);
   classe_serialisation.chaine_cour_ecriture_diff = &seri_diff_type;
   classe_serialisation.Serialiser_difference_type(pos);
   if( (classe_serialisation.chaine_cour_ecriture_diff->Taille() > 0)
     ||(diff_contenu.Taille() > 0) )
    {serialisation_differences_type_zt.Maj("<",0,1);
     serialisation_differences_type_zt += Nom_classe();
     serialisation_differences_type_zt.Ajouter(" nom=",0,5);
     serialisation_differences_type_zt += Nom();
     serialisation_differences_type_zt.Ajouter(";",0,1);
     serialisation_differences_type_zt += seri_diff_type;
     serialisation_differences_type_zt.Ajouter(">",0,1);
     serialisation_differences_type_zt += diff_contenu;
     serialisation_differences_type_zt.Ajouter("</",0,2);
     serialisation_differences_type_zt += Nom_classe();
     serialisation_differences_type_zt.Ajouter(">",0,1);}
 serialisation_type_zt.Maj("<",0,1);
 serialisation_type_zt += Nom_classe();
 serialisation_type_zt.Ajouter(" nom=",0,5);
 serialisation_type_zt += Nom();
 serialisation_type_zt.Ajouter(";",0,1);
 serialisation_type_zt += nouv_serialisation_type;
 serialisation_type_zt.Ajouter(">",0,1);
 serialisation_type_zt += alx_noeud_scene::Serialisation_type();
 serialisation_type_zt.Ajouter("</",0,2);
 serialisation_type_zt += Nom_classe();
 serialisation_type_zt.Ajouter(">",0,1);

 return serialisation_differences_type_zt;
}
//______________________________________________________________________________
const alx_chaine_char& alx_noeud_zone_texte_sdl_opengl::Serialiser_difference_contenu() const
{return alx_noeud_scene::Serialiser_difference_type();}
//______________________________________________________________________________
//___________________________ La sérialisation _________________________________
const alx_chaine_char& alx_noeud_zone_texte_sdl_opengl::Serialiser_type(const alx_chaine_char &contenu) const
{const double *tab;
 alx_chaine_char cc_tmp;
 serialisation_type_zt = "<"+Nom_classe()+" nom="+Nom()+";";
   cc_tmp = "4,0="; serialisation_type_zt += cc_tmp + Chaine_char().Taille() + "_" + Chaine_char() + ";";
   cc_tmp = "1,1="; serialisation_type_zt += cc_tmp + Hauteur() + ";";
   cc_tmp = "1,2="; serialisation_type_zt += cc_tmp + Largeur() + ";";
   cc_tmp = "1,3="; serialisation_type_zt += cc_tmp + Zoom() + ";";
   cc_tmp = "0,4="; serialisation_type_zt += cc_tmp + Afficher_fond() + ";";
   cc_tmp = "0,5="; serialisation_type_zt += cc_tmp + Editable() + ";";
   cc_tmp = "0,6="; serialisation_type_zt += cc_tmp + Lisser() + ";";
   cc_tmp = "6,7="; serialisation_type_zt += cc_tmp + Position_curseur() + ";";
   cc_tmp = "6,8="; serialisation_type_zt += cc_tmp + Ligne_de_depart() + ";";
   tab = Couleur_texte();
   cc_tmp = "3,9="; serialisation_type_zt += cc_tmp +tab[0]+"_"+tab[1]+"_"+tab[2]+"_"+tab[3]+";";
   tab = Couleur_curseur();
   cc_tmp = "3,10="; serialisation_type_zt += cc_tmp +tab[0]+"_"+tab[1]+"_"+tab[2]+"_"+tab[3]+";";
 serialisation_type_zt += ">"+contenu+"</"+Nom_classe()+">";

 return serialisation_type_zt;}

//______________________________________________________________________________
const alx_chaine_char& alx_noeud_zone_texte_sdl_opengl::Serialiser_contenu() const
{return alx_noeud_scene::Serialiser_type();}
//______________________________________________________________________________
//__________________________ La désérialisation ________________________________
void alx_noeud_zone_texte_sdl_opengl::Deserialiser_type(const alx_chaine_char &txt, unsigned int &pos)
{//unsigned int num_f, num_a;
 const char *texte = txt.Texte();


 classe_serialisation.chaine_cour = &txt;
 if(texte[pos] == '<') pos+= Nom_classe().Taille()+2; // on passe "<alx_noeud_fenetre_sdl_opengl "
 pos = txt.Position_du_prochain(';', pos); pos++; //On passe nom=...;

 classe_serialisation.Deserialiser_type(txt, pos);
 const bool a_changer   = A_changer()
          , maj_texture = Maj_texture();
   pos++; // On passe le >
   if( (texte[pos]   == '<')
     &&(texte[pos+1] != '/') )
    Deserialiser(txt, pos);
 A_changer  (a_changer);
 Maj_texture(maj_texture);
 pos += Nom_classe().Taille()+3; // On passe la balise de fin "</alx_noeud_fenetre_sdl_opengl>"
}
//______________________________________________________________________________
void alx_noeud_zone_texte_sdl_opengl::Deserialiser(const alx_chaine_char &txt, unsigned int &pos)
{alx_noeud_scene::Deserialiser_type(txt, pos);}



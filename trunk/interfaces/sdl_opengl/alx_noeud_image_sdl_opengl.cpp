#include "alx_noeud_image_sdl_opengl.h"

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
void alx_noeud_image_sdl_opengl::init()
{Nom_classe("alx_noeud_image_sdl_opengl");
 init_ptr();
 Position_dans_liste_des_fils_changeable(true);
 boite_noeud.Est_vide( false );
 boite_fils.Est_vide( false );
 boite_noeud_et_fils.Est_vide( false );
 boite_noeud_et_fils_glob.Est_vide( false );
 Etirement_x(0); Etirement_y(0);

 // Nom de la texture affichée par l'image...
 cc_tmp  = "Image_de_";
 cc_tmp += Nom();
 Info_texture().Nom_CC( cc_tmp );
}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
alx_noeud_image_sdl_opengl::alx_noeud_image_sdl_opengl(const bool mipmap) : alx_noeud_scene()
                                                         , alx_image_opengl(mipmap)
                                                         , classe_serialisation( this
                                                                               , &serialisation_type_img
                                                                               , &nouv_serialisation_type_img
                                                                               , &serialisation_differences_type_img )
{init();
 MipMap( mipmap );
}

//______________________________________________________________________________
alx_noeud_image_sdl_opengl::alx_noeud_image_sdl_opengl( const char *n
                                                      , const bool mipmap) : alx_noeud_scene()
                                                                      , alx_image_opengl(n, mipmap)
                                                                      , classe_serialisation( this
                                                                                            , &serialisation_type_img
                                                                                            , &nouv_serialisation_type_img
                                                                                            , &serialisation_differences_type_img )
{init();
 MipMap( mipmap );
 L_rap_fin_maj_data_texture.Rappeler(this);
}

//______________________________________________________________________________
alx_noeud_image_sdl_opengl::alx_noeud_image_sdl_opengl( const INFOS_TEXTURE &info_text
                                                      , const double taille_x
                                                      , const double taille_y ) : alx_noeud_scene()
                                                                                , alx_image_opengl(info_text, taille_x, taille_y)
                                                                                , classe_serialisation( this
                                                                                                      , &serialisation_type_img
                                                                                                      , &nouv_serialisation_type_img
                                                                                                      , &serialisation_differences_type_img )
{init();
}

//______________________________________________________________________________
alx_noeud_image_sdl_opengl::alx_noeud_image_sdl_opengl(const alx_image_opengl &img) : alx_noeud_scene()
                                                                                    , alx_image_opengl(img)
                                                                                    , classe_serialisation( this
                                                                                                          , &serialisation_type_img
                                                                                                          , &nouv_serialisation_type_img
                                                                                                          , &serialisation_differences_type_img )
{init();
 L_rap_fin_maj_data_texture.Rappeler(this);
}

//______________________________________________________________________________
alx_noeud_image_sdl_opengl::~alx_noeud_image_sdl_opengl()
{  Lock_mutex_tempon();
 UnLock_mutex_tempon();
}


//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
void alx_noeud_image_sdl_opengl::PreRendre()
{if(image_processed_by_thread || force_update_after_threaded_maj) {
   // Load the texture in OpenGL
   image_processed_by_thread = force_update_after_threaded_maj = false;
   maj_tempon();
  }
  alx_noeud_scene::PreRendre();
}

//______________________________________________________________________________
void alx_noeud_image_sdl_opengl::Dessin_noeud()
{alx_image_opengl::afficher();}
//______________________________________________________________________________
/*void alx_noeud_image_sdl_opengl::Afficher()
{//Calculer_boite_noeud();
 if( !LR_Avant_affichage.Vide() ) {LR_Avant_affichage.Rappeler( this );}
 glPushMatrix();
   Changement_repere( *this );
   if(Noeud_puis_fils())
    {alx_image_opengl::afficher();
     Afficher_fils(0);}
    else
    {Afficher_fils(0);
     alx_image_opengl::afficher();}
 glPopMatrix();
 if( !LR_Apres_affichage.Vide() ) {LR_Apres_affichage.Rappeler( this );}
}
*/
//______________________________________________________________________________
info_du_contenant* alx_noeud_image_sdl_opengl::Noeud_contient(const alx_point2D &pt, int action)
{// Transformer les coordonnées
 info_du_contenant *rep;
// alx_model_simulation_physique1 **mp_rep;

 // Vérifier que le point est bien dans le rectangle de l'image
 if( (pt.x < 0)
   ||(pt.y < 0)
   ||(pt.x > Lg())
   ||(pt.y > Ht()) ) {return (info_du_contenant*)NULL;}

 // Si l'image à un tempon en mémoire centrale et que la translucidité est activée
 // , vérifier que le pixel sélectionner n'est pas transparent
 unsigned int x, y;
 if(Inverser_x()) x = (unsigned int)(Lg()-pt.x);
  else x = (unsigned int)pt.x;
 if(Inverser_y()) y = (unsigned int)(Ht()-pt.y);
  else y = (unsigned int)pt.y;

 if( (Translucidite()) // Cas où on peut cliquer au travers de l'image, dans la zone translucide
   &&(TEMPON())
   &&(Alpha(x, y) == 0) )
   return (info_du_contenant*)NULL;

 rep = new info_du_contenant();
 // Si c'est bon, renvoyer la réponse.
 rep->mp    = &(MP());
 rep->noeud = (alx_noeud_scene*)this;
 rep->Point_au_contact(pt);

 return rep;
}
//______________________________________________________________________________
//___________________ Les surcharges pour la sérialisation _____________________
//______________________________________________________________________________
void alx_noeud_image_sdl_opengl::init_ptr()
{P_void *f_diff   = classe_serialisation.f_diff
      , *f_deseri = classe_serialisation.f_deseri;

 classe_serialisation.fd1[0]   = &alx_noeud_image_sdl_opengl::Etirement_x;    f_deseri[0] = &(classe_serialisation.fd1[0]);
 classe_serialisation.fsd_d1[0]= &alx_noeud_image_sdl_opengl::Etirement_x;    f_diff[0]   = &(classe_serialisation.fsd_d1[0]);
 classe_serialisation.fd1[1]   = &alx_noeud_image_sdl_opengl::Etirement_y;    f_deseri[1] = &(classe_serialisation.fd1[1]);
 classe_serialisation.fsd_d1[1]= &alx_noeud_image_sdl_opengl::Etirement_y;    f_diff[1]   = &(classe_serialisation.fsd_d1[1]);
 classe_serialisation.fd1[2]   = &alx_noeud_image_sdl_opengl::Lg;       f_deseri[2] = &(classe_serialisation.fd1[2]);
 classe_serialisation.fsd_d1[2]= &alx_noeud_image_sdl_opengl::Lg;       f_diff[2]   = &(classe_serialisation.fsd_d1[2]);
 classe_serialisation.fd1[3]   = &alx_noeud_image_sdl_opengl::Ht;       f_deseri[3] = &(classe_serialisation.fd1[3]);
 classe_serialisation.fsd_d1[3]= &alx_noeud_image_sdl_opengl::Ht;       f_diff[3]   = &(classe_serialisation.fsd_d1[3]);

 classe_serialisation.fb1   [0] = &alx_noeud_image_sdl_opengl::Image_creer_en_interne; f_deseri[4] = &(classe_serialisation.fb1[0]);
 classe_serialisation.fsd_b1[0] = &alx_noeud_image_sdl_opengl::Image_creer_en_interne; f_diff  [4] = &(classe_serialisation.fsd_b1[0]);
 classe_serialisation.fb1   [1] = &alx_noeud_image_sdl_opengl::Couleur_def           ; f_deseri[5] = &(classe_serialisation.fb1[1]);
 classe_serialisation.fsd_b1[1] = &alx_noeud_image_sdl_opengl::Couleur_def           ; f_diff  [5] = &(classe_serialisation.fsd_b1[1]);
 classe_serialisation.fb1   [2] = &alx_noeud_image_sdl_opengl::Lisser                ; f_deseri[6] = &(classe_serialisation.fb1[2]);
 classe_serialisation.fsd_b1[2] = &alx_noeud_image_sdl_opengl::Lisser                ; f_diff  [6] = &(classe_serialisation.fsd_b1[2]);

 classe_serialisation.fd4[0]   = &alx_noeud_image_sdl_opengl::Couleur; f_deseri[7] = &(classe_serialisation.fd4[0]);
 classe_serialisation.fsd_d4[0]= &alx_noeud_image_sdl_opengl::Couleur; f_diff[7]   = &(classe_serialisation.fsd_d4[0]);

 classe_serialisation.fcc[0]   = &alx_noeud_image_sdl_opengl::Deseri_info_texture;               ; f_deseri[8] = &(classe_serialisation.fcc[0]);
 classe_serialisation.fsd_cc[0]= &alx_noeud_image_sdl_opengl::Seri_diff_info_texture_a_ete_change; f_diff[8]   = &(classe_serialisation.fsd_cc[0]);

 classe_serialisation.fi1   [0] = &alx_noeud_image_sdl_opengl::Ordonnancement_couleurs; f_deseri[9] = &(classe_serialisation.fi1[0]);
 classe_serialisation.fsd_i1[0] = &alx_noeud_image_sdl_opengl::Ordonnancement_couleurs; f_diff  [9] = &(classe_serialisation.fsd_i1[0]);
 classe_serialisation.fi1   [1] = &alx_noeud_image_sdl_opengl::Nb_octets_par_pixel    ; f_deseri[10] = &(classe_serialisation.fi1[1]);
 classe_serialisation.fsd_i1[1] = &alx_noeud_image_sdl_opengl::Nb_octets_par_pixel    ; f_diff  [10] = &(classe_serialisation.fsd_i1[1]);

 classe_serialisation.fb1   [3] = &alx_noeud_image_sdl_opengl::Translucidite         ; f_deseri[11] = &(classe_serialisation.fb1[3]);
 classe_serialisation.fsd_b1[3] = &alx_noeud_image_sdl_opengl::Translucidite         ; f_diff  [11] = &(classe_serialisation.fsd_b1[3]);
 classe_serialisation.fb1   [4] = &alx_noeud_image_sdl_opengl::Inverser_x            ; f_deseri[12] = &(classe_serialisation.fb1[4]);
 classe_serialisation.fsd_b1[4] = &alx_noeud_image_sdl_opengl::Inverser_x            ; f_diff  [12] = &(classe_serialisation.fsd_b1[4]);
 classe_serialisation.fb1   [5] = &alx_noeud_image_sdl_opengl::Inverser_y            ; f_deseri[13] = &(classe_serialisation.fb1[5]);
 classe_serialisation.fsd_b1[5] = &alx_noeud_image_sdl_opengl::Inverser_y            ; f_diff  [13] = &(classe_serialisation.fsd_b1[5]);

 classe_serialisation.fi1   [2] = &alx_noeud_image_sdl_opengl::Nb_octets_par_pixels_texture; f_deseri[14] = &(classe_serialisation.fi1[2]);
 classe_serialisation.fsd_i1[2] = &alx_noeud_image_sdl_opengl::Nb_octets_par_pixels_texture; f_diff  [14] = &(classe_serialisation.fsd_i1[2]);
 classe_serialisation.fi1   [3] = &alx_noeud_image_sdl_opengl::Ordre_couleur_texture    ; f_deseri[15] = &(classe_serialisation.fi1[3]);
 classe_serialisation.fsd_i1[3] = &alx_noeud_image_sdl_opengl::Ordre_couleur_texture    ; f_diff  [15] = &(classe_serialisation.fsd_i1[3]);
}

//______________________________________________________________________________
//_____________________________ La différence __________________________________
//______________________________________________________________________________
void alx_noeud_image_sdl_opengl::Deseri_info_texture(const alx_chaine_char &t)
{Info_texture().Nom_CC( t );
 L_rap_maj_Info_texture.Rappeler( this );
}

//______________________________________________________________________________
const bool alx_noeud_image_sdl_opengl::Seri_diff_info_texture_a_ete_change(const alx_chaine_char **ptr_cc) const
{*ptr_cc = Info_texture_const().Ptr_Nom_const();
 const bool rep = Info_texture_const().Data_modifiees();
   Info_texture_const().Data_modifiees(false);
 return rep;
}

//______________________________________________________________________________
const alx_chaine_char& alx_noeud_image_sdl_opengl::Serialiser_difference_type(const alx_chaine_char &diff_contenu) const
{if(serialisation_type_img.Taille() == 0) {
   serialisation_differences_type_img = Serialiser_type();
   return serialisation_differences_type_img;
  }

 serialisation_differences_type_img.Maj("",0,0);
 nouv_serialisation_type_img.Maj("",0,0);
   unsigned int pos = Nom_classe().Taille()+2;
   pos = serialisation_type_img.Position_du_prochain(';', pos); pos++; //On passe nom=...;
   seri_diff_type_img.Maj("",0,0);
   classe_serialisation.chaine_cour_ecriture_diff = &seri_diff_type_img;
   classe_serialisation.Serialiser_difference_type(pos);
   if( (classe_serialisation.chaine_cour_ecriture_diff->Taille() > 0)
     ||(diff_contenu.Taille() > 0) )
    {serialisation_differences_type_img.Maj("<",0,1);
     serialisation_differences_type_img += Nom_classe();
     serialisation_differences_type_img.Ajouter(" nom=",0,5);
     serialisation_differences_type_img += Nom();
     serialisation_differences_type_img.Ajouter(";",0,1);
     serialisation_differences_type_img += seri_diff_type_img;
     serialisation_differences_type_img.Ajouter(">",0,1);
     serialisation_differences_type_img += diff_contenu;
     serialisation_differences_type_img.Ajouter("</",0,2);
     serialisation_differences_type_img += Nom_classe();
     serialisation_differences_type_img.Ajouter(">",0,1);}
 serialisation_type_img.Maj("<",0,1);
 serialisation_type_img += Nom_classe();
 serialisation_type_img.Ajouter(" nom=",0,5);
 serialisation_type_img += Nom();
 serialisation_type_img.Ajouter(";",0,1);
 serialisation_type_img += nouv_serialisation_type_img;
 serialisation_type_img.Ajouter(">",0,1);
 serialisation_type_img += alx_noeud_scene::Serialisation_type();
 serialisation_type_img.Ajouter("</",0,2);
 serialisation_type_img += Nom_classe();
 serialisation_type_img.Ajouter(">",0,1);

 return serialisation_differences_type_img;
}
//______________________________________________________________________________
const alx_chaine_char& alx_noeud_image_sdl_opengl::Serialiser_difference_contenu() const
{return alx_noeud_scene::Serialiser_difference_type();}
//______________________________________________________________________________
//___________________________ La sérialisation _________________________________
const alx_chaine_char& alx_noeud_image_sdl_opengl::Serialiser_type(const alx_chaine_char &contenu) const
{const double *tab;
 //alx_chaine_char cc_tmp;
 serialisation_type_img = "<"+Nom_classe()+" nom="+Nom()+";";
   cc_tmp = "1,0="; serialisation_type_img += cc_tmp + Etirement_x() + ";";
   cc_tmp = "1,1="; serialisation_type_img += cc_tmp + Etirement_y() + ";";
   cc_tmp = "1,2="; serialisation_type_img += cc_tmp + Lg() + ";";
   cc_tmp = "1,3="; serialisation_type_img += cc_tmp + Ht() + ";";
   cc_tmp = "0,4="; serialisation_type_img += cc_tmp + Image_creer_en_interne() + ";";
   cc_tmp = "0,5="; serialisation_type_img += cc_tmp + Couleur_def() + ";";
   cc_tmp = "0,6="; serialisation_type_img += cc_tmp + Lisser() + ";";
   tab = Couleur();
   cc_tmp = "3,7="; serialisation_type_img += cc_tmp +tab[0]+"_"+tab[1]+"_"+tab[2]+"_"+tab[3]+";";
   serialisation_type_img += "4,8=";
     serialisation_type_img += Info_texture_const().Nom().Taille();
     serialisation_type_img.Ajouter("|",0,1);
     serialisation_type_img += Info_texture_const().Nom();
     serialisation_type_img.Ajouter(";",0,1);
   serialisation_type_img += "6,9=" ; serialisation_type_img += Ordonnancement_couleurs()     ; serialisation_type_img.Ajouter(";",0,1);
   serialisation_type_img += "6,10="; serialisation_type_img += Nb_octets_par_pixel()         ; serialisation_type_img.Ajouter(";",0,1);
   serialisation_type_img += "0,11="; serialisation_type_img += Translucidite()               ; serialisation_type_img.Ajouter(";",0,1);
   serialisation_type_img += "0,12="; serialisation_type_img += Inverser_x()                  ; serialisation_type_img.Ajouter(";",0,1);
   serialisation_type_img += "0,13="; serialisation_type_img += Inverser_y()                  ; serialisation_type_img.Ajouter(";",0,1);
   serialisation_type_img += "6,14="; serialisation_type_img += Nb_octets_par_pixels_texture(); serialisation_type_img.Ajouter(";",0,1);
   serialisation_type_img += "6,15="; serialisation_type_img += Ordre_couleur_texture()       ; serialisation_type_img.Ajouter(";",0,1);
 serialisation_type_img += ">"+contenu+"</"+Nom_classe()+">";

 return serialisation_type_img;}

//______________________________________________________________________________
const alx_chaine_char& alx_noeud_image_sdl_opengl::Serialiser_contenu() const
{return alx_noeud_scene::Serialiser_type();}
//______________________________________________________________________________
//__________________________ La désérialisation ________________________________
void alx_noeud_image_sdl_opengl::Deserialiser_type(const alx_chaine_char &txt, unsigned int &pos) 
{//unsigned int num_f, num_a;
 const char *texte = txt.Texte();

 classe_serialisation.chaine_cour = &txt;
 if(texte[pos] == '<') pos+= Nom_classe().Taille()+2; // on passe "<alx_noeud_fenetre_sdl_opengl "
 pos = txt.Position_du_prochain(';', pos); pos++; //On passe nom=...;

 classe_serialisation.Deserialiser_type(txt, pos);
 pos++; // On passe le >
 Deserialiser(txt, pos);

 pos += Nom_classe().Taille()+3; // On passe la balise de fin "</alx_noeud_fenetre_sdl_opengl>"
}
//______________________________________________________________________________
void alx_noeud_image_sdl_opengl::Deserialiser(const alx_chaine_char &txt, unsigned int &pos)
{alx_noeud_scene::Deserialiser_type(txt, pos);}

//______________________________________________________________________________
void alx_noeud_image_sdl_opengl::Calculer_boite_noeud()
{// Mise à jour des coordonnées de la boîte englobante dans les repères des pères.
 boite_noeud.maj(0,0,Lg(),Ht());
 boite_noeud_et_fils.maj( boite_noeud );
 boite_noeud_et_fils.Inclure_boite( boite_fils );
 boite_noeud_et_fils_glob.maj(boite_noeud_et_fils);
 boite_noeud_et_fils_glob.Prendre_en_compte( *this );
}


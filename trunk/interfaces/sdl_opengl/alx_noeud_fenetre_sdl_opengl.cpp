#include "alx_noeud_fenetre_sdl_opengl.h"

#include "alx_interface_sdl_opengl_1.h"
#include "..\..\physique\alx_simulateur_points_contact.h"
#include "..\classiques\alx_fenetre_opengl.h"
#include "..\..\physique\math_alex.cpp"

//______________________________________________________________________________
//____________________________ La sérialisation ________________________________
//______________________________________________________________________________
void alx_noeud_fenetre_sdl_opengl::Serialiser_diff_4_doubles(unsigned int &pos, void *fptr) const
{unsigned int pos_fin_amorce = pos;
 double T1[4];
 T1[0] = chaine_cour_diff_seri_fen->Lire_double(pos); pos++;
 T1[1] = chaine_cour_diff_seri_fen->Lire_double(pos); pos++;
 T1[2] = chaine_cour_diff_seri_fen->Lire_double(pos); pos++;
 T1[3] = chaine_cour_diff_seri_fen->Lire_double(pos);
 f_fen_consultation_4_doubles f4 = *((f_fen_consultation_4_doubles*)fptr);
 const double *T = (this->*f4)();
 bool diff = false;
 for(unsigned int i=0;i<4;i++)
   if( absd(T[i]-T1[i]) > EPSILON ) {diff=true; break;}
 if(diff)
  {chaine_tmp.Maj(chaine_cour_diff_seri_fen->Texte(), dep_ecriture, pos_fin_amorce-dep_ecriture);
   chaine_tmp += T[0]; chaine_tmp.Ajouter("_",0,1);
   chaine_tmp += T[1]; chaine_tmp.Ajouter("_",0,1);
   chaine_tmp += T[2]; chaine_tmp.Ajouter("_",0,1);
   chaine_tmp += T[3]; chaine_tmp.Ajouter(";",0,1);
   *chaine_cour_ecriture_diff_fen += chaine_tmp;
   *chaine_cour_ecriture_seri_fen += chaine_tmp;} else {chaine_cour_ecriture_seri_fen->Ajouter(chaine_cour_diff_seri_fen->Texte(), dep_ecriture, pos-dep_ecriture+1);}
}

void alx_noeud_fenetre_sdl_opengl::Serialiser_diff_2_doubles(unsigned int &pos, void *fptr) const
{/*unsigned int pos_fin_amorce = pos;*/}

void alx_noeud_fenetre_sdl_opengl::Serialiser_diff_1_double (unsigned int &pos, void *fptr) const
{unsigned int pos_fin_amorce = pos;
 double a, ap;
 a  = chaine_cour_diff_seri_fen->Lire_double(pos);
 ap = (this->**((f_fen_consultation_1_double*)fptr))();
 if( absd(a-ap) > EPSILON )
  {chaine_tmp.Maj(chaine_cour_diff_seri_fen->Texte(), dep_ecriture, pos_fin_amorce-dep_ecriture); chaine_tmp+=ap; chaine_tmp.Ajouter(";",0,1);
   *chaine_cour_ecriture_diff_fen += chaine_tmp;
   *chaine_cour_ecriture_seri_fen += chaine_tmp;} else {chaine_cour_ecriture_seri_fen->Ajouter(chaine_cour_diff_seri_fen->Texte(), dep_ecriture, pos-dep_ecriture+1);}
}

//______________________________________________________________________________
void alx_noeud_fenetre_sdl_opengl::Serialiser_diff_1_bool   (unsigned int &pos, void *f) const
{unsigned int pos_fin_amorce = pos;
 bool a, ap;
 a  = chaine_cour_diff_seri_fen->Lire_bool(pos);
 ap = (this->**((f_fen_consultation_1_bool*)f))();
 if(a != ap)
  {chaine_tmp.Maj(chaine_cour_diff_seri_fen->Texte(), dep_ecriture, pos_fin_amorce-dep_ecriture); chaine_tmp+=ap; chaine_tmp.Ajouter(";",0,1);
   *chaine_cour_ecriture_diff_fen += chaine_tmp;
   *chaine_cour_ecriture_seri_fen += chaine_tmp;} else {chaine_cour_ecriture_seri_fen->Ajouter(chaine_cour_diff_seri_fen->Texte(), dep_ecriture, pos-dep_ecriture+1);}
}
//______________________________________________________________________________
void alx_noeud_fenetre_sdl_opengl::Serialiser_diff_chaine_char   (unsigned int &pos, void *f) const
{unsigned int pos_fin_amorce = pos;
  // Calcul de la chaine déja sérialisée:
   unsigned int dep_cc1 = pos;
   const char *txt = chaine_cour_diff_seri_fen->Texte();
   while( txt[pos] != ';') pos++;
   cc1.Maj(txt, dep_cc1, pos-dep_cc1);
 const alx_chaine_char &cc2 = (this->**((f_fen_consultation_chaine_char*)f))();
 if(cc1 != cc2)
  {chaine_tmp.Maj(chaine_cour_diff_seri_fen->Texte(), dep_ecriture, pos_fin_amorce-dep_ecriture); chaine_tmp+= cc2; chaine_tmp.Ajouter(";",0,1);
   *chaine_cour_ecriture_diff_fen += chaine_tmp;
   *chaine_cour_ecriture_seri_fen += chaine_tmp;} else {chaine_cour_ecriture_seri_fen->Ajouter(chaine_cour_diff_seri_fen->Texte(), dep_ecriture, pos-dep_ecriture+1);}
}
//______________________________________________________________________________
void alx_noeud_fenetre_sdl_opengl::Serialiser_diff_4_doubles_i(unsigned int &pos, void *fptr) const
{unsigned int pos_fin_amorce = pos, n, dep_contenu;
 double T1[4];
 bool amorce_ecrite      = false
    , amorce_diff_ecrite = false;

 f_fen_consultation_4_doubles_i f4 = *((f_fen_consultation_4_doubles_i*)fptr);
 for(unsigned int j=0; j<4; j++)
  {dep_contenu = pos;
   n     = chaine_cour_diff_seri_fen->Lire_entier(pos); pos++;
   T1[0] = chaine_cour_diff_seri_fen->Lire_double(pos); pos++;
   T1[1] = chaine_cour_diff_seri_fen->Lire_double(pos); pos++;
   T1[2] = chaine_cour_diff_seri_fen->Lire_double(pos); pos++;
   T1[3] = chaine_cour_diff_seri_fen->Lire_double(pos); pos++;
   const double *T = (this->*f4)(j);
   bool diff = false;
   for(unsigned int i=0;i<4;i++)
     if( absd(T[i]-T1[i]) > EPSILON ) {diff=true; break;}
   if(diff)
    {chaine_tmp.Maj("",0,0);
     if(!amorce_ecrite)
      {chaine_tmp.Maj(chaine_cour_diff_seri_fen->Texte(), dep_ecriture, pos_fin_amorce-dep_ecriture);
       amorce_ecrite = true;}
     if(!amorce_diff_ecrite)
      {chaine_cour_ecriture_diff_fen->Ajouter(chaine_cour_diff_seri_fen->Texte(), dep_ecriture, pos_fin_amorce-dep_ecriture);
       amorce_diff_ecrite = true;}
      else chaine_cour_ecriture_diff_fen->Ajouter("_",0,1);
     chaine_tmp += n;    chaine_tmp.Ajouter("_",0,1);
     chaine_tmp += T[0]; chaine_tmp.Ajouter("_",0,1);
     chaine_tmp += T[1]; chaine_tmp.Ajouter("_",0,1);
     chaine_tmp += T[2]; chaine_tmp.Ajouter("_",0,1);
     chaine_tmp += T[3];
     *chaine_cour_ecriture_diff_fen += chaine_tmp;
     *chaine_cour_ecriture_seri_fen += chaine_tmp;
     if(j<3) chaine_cour_ecriture_seri_fen->Ajouter("_",0,1);
      else chaine_cour_ecriture_seri_fen->Ajouter(";",0,1);
     }
    else
     {if(!amorce_ecrite)
       {chaine_cour_ecriture_seri_fen->Ajouter(chaine_cour_diff_seri_fen->Texte(), dep_ecriture, pos_fin_amorce-dep_ecriture);
        amorce_ecrite = true;}
      chaine_cour_ecriture_seri_fen->Ajouter(chaine_cour_diff_seri_fen->Texte(), dep_contenu, pos-dep_contenu);
     }
  }

 if(amorce_diff_ecrite)
   chaine_cour_ecriture_diff_fen->Ajouter(";",0,1);
 // On a avancé un coup de trop, on revient sur le ;
 pos--;
}

//______________________________________________________________________________
void alx_noeud_fenetre_sdl_opengl::Serialiser_diff_repere     (unsigned int &pos, void *fptr) const
{unsigned int pos_fin_amorce = pos;
 f_fen_consultation_repere f = *((f_fen_consultation_repere*)fptr);
 const alx_repere2D *repere = (this->*f)();
 const alx_chaine_char &cc_diff = repere->alx_repere2D::Serialiser_difference_contenu();

 for(unsigned int i=0; i<11; i++)
  {pos = chaine_cour_diff_seri_fen->Position_du_prochain(';',pos);
   pos++;}
 pos--; // On revient au niveau du dernier ;

 if(cc_diff.Taille() != 0)
  {// Mise à jour de la différence.
   chaine_cour_ecriture_diff_fen->Ajouter(chaine_cour_diff_seri_fen->Texte(), dep_ecriture, pos_fin_amorce-dep_ecriture);
   *chaine_cour_ecriture_diff_fen += cc_diff; chaine_cour_ecriture_diff_fen->Ajouter(";",0,1);
  // Mise à jour de la sérialisation
   chaine_cour_ecriture_seri_fen->Ajouter(chaine_cour_diff_seri_fen->Texte(), dep_ecriture, pos_fin_amorce-dep_ecriture);
   *chaine_cour_ecriture_seri_fen += repere->alx_repere2D::Serialisation_contenu(); chaine_cour_ecriture_seri_fen->Ajouter(";",0,1);
  }
  else
   {chaine_cour_ecriture_seri_fen->Ajouter(chaine_cour_diff_seri_fen->Texte(), dep_ecriture, pos-dep_ecriture+1);}
}
//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
void alx_noeud_fenetre_sdl_opengl::Deserialiser_repere      (unsigned int &pos, void *fptr)
{f_fen_deserialisation_repere f = *((f_fen_deserialisation_repere*)fptr);
 alx_repere2D *repere = (this->*f)();
 repere->alx_repere2D::Deserialiser_contenu(*chaine_cour_fen, pos);
}

//______________________________________________________________________________
void alx_noeud_fenetre_sdl_opengl::Deserialiser_4_doubles_i (unsigned int &pos, void *fptr)
{double a, b, c, d;
 int i;
 const char *txt = chaine_cour_fen->Texte();
 f_fen_deserialisation_int_4_doubles f4d_i = *((f_fen_deserialisation_int_4_doubles*)fptr);
 while(txt[pos] != ';')
  {i = chaine_cour_fen->Lire_entier(pos); pos++;
   a = chaine_cour_fen->Lire_double(pos); pos++;
   b = chaine_cour_fen->Lire_double(pos); pos++;
   c = chaine_cour_fen->Lire_double(pos); pos++;
   d = chaine_cour_fen->Lire_double(pos);
   if(txt[pos]=='_') pos++;
   (this->*f4d_i)(i,a,b,c,d);
  }
}
//______________________________________________________________________________
void alx_noeud_fenetre_sdl_opengl::Deserialiser_4_doubles(unsigned int &pos, void *fptr)
{double a, b, c, d;
 a = chaine_cour_fen->Lire_double(pos); pos++;
 b = chaine_cour_fen->Lire_double(pos); pos++;
 c = chaine_cour_fen->Lire_double(pos); pos++;
 d = chaine_cour_fen->Lire_double(pos);
 f_fen_deserialisation_4_doubles f4 = *((f_fen_deserialisation_4_doubles*)fptr);
 (this->*f4)(a,b,c,d);}
//______________________________________________________________________________
void alx_noeud_fenetre_sdl_opengl::Deserialiser_2_doubles(unsigned int &pos, void *fptr)
{double a, b;
 a = chaine_cour_fen->Lire_double(pos); pos++;
 b = chaine_cour_fen->Lire_double(pos);
 (this->**((f_fen_deserialisation_2_doubles*)fptr))(a,b);}
//______________________________________________________________________________
void alx_noeud_fenetre_sdl_opengl::Deserialiser_1_double (unsigned int &pos, void *fptr)
{double a;
 a = chaine_cour_fen->Lire_double(pos);
 (this->**((f_fen_deserialisation_1_double*)fptr))(a);}
//______________________________________________________________________________
void alx_noeud_fenetre_sdl_opengl::Deserialiser_1_bool   (unsigned int &pos, void *fptr)
{bool a;
 a = chaine_cour_fen->Lire_bool(pos);
 (this->**((f_fen_deserialisation_1_bool*)fptr))(a);}
//______________________________________________________________________________
void alx_noeud_fenetre_sdl_opengl::Deserialiser_chaine_char   (unsigned int &pos, void *fptr)
{unsigned int pos_dep = pos;
 pos = chaine_cour_fen->Position_du_prochain(';', pos);
 alx_chaine_char tmp(pos-pos_dep+1);
 tmp.Maj(chaine_cour_fen->Texte(), pos_dep, pos-pos_dep);
 (this->**((f_fen_deserialisation_chaine_char*)fptr))(tmp);}

//______________________________________________________________________________
// On suppose être placé sur le premier argument
// On désérialise ici les attributs qui ne sont accessibles que par des méthodes.
void alx_noeud_fenetre_sdl_opengl::Deserialiser_type(const alx_chaine_char &txt, unsigned int &pos)
{unsigned int num_f, num_a;
 const char *texte = txt.Texte();
 f_fen_deserialisation f;

 chaine_cour_fen = &txt;
 if(texte[pos] == '<') pos+= 30; // on passe "<alx_noeud_fenetre_sdl_opengl "
 pos = txt.Position_du_prochain(';', pos); pos++; //On passe nom=...;

// On cherche d'abord le début de la sérialisation du noeud
 unsigned int pos_noeud = pos;
 while( texte[pos_noeud++]  != '>' );
 Deserialiser(txt, pos_noeud);

// On refixe les fonctions de rappel, pour assurer la désérialisation:
 f_ptr_fen[6] = &(alx_noeud_fenetre_sdl_opengl::Deserialiser_repere);
 f_ptr_fen[5] = &(alx_noeud_fenetre_sdl_opengl::Deserialiser_4_doubles_i);
 f_ptr_fen[4] = &(alx_noeud_fenetre_sdl_opengl::Deserialiser_chaine_char);
 f_ptr_fen[3] = &(alx_noeud_fenetre_sdl_opengl::Deserialiser_4_doubles);
 f_ptr_fen[2] = &(alx_noeud_fenetre_sdl_opengl::Deserialiser_2_doubles);
 f_ptr_fen[1] = &(alx_noeud_fenetre_sdl_opengl::Deserialiser_1_double);
 f_ptr_fen[0] = &(alx_noeud_fenetre_sdl_opengl::Deserialiser_1_bool);

// Puis on désérialise la partie spécifique à la fenêtre
 while( texte[pos] != '>' )
  {num_f = txt.Lire_entier(pos);
   pos++; // On passe la ,
   num_a = txt.Lire_entier(pos);
   pos++; // On passe le =
   f = f_ptr_fen[num_f];
   (this->*f)(pos, f_fen[num_a]);
   pos++; // On passe le ;
  }

 pos = pos_noeud; // On se recalle à al fin du noeud.
 pos += 31; // On passe la balise de fin "</alx_noeud_fenetre_sdl_opengl>"
 Mettre_a_jour();
}

//______________________________________________________________________________
void alx_noeud_fenetre_sdl_opengl::Deserialiser(const alx_chaine_char &txt, unsigned int &pos)
{unsigned int num_f, num_a;
 const char *texte = txt.Texte();
 f_deserialisation_scene f;

 chaine_cour = chaine_cour_fen = &txt;
 //pos+=31; // on passe (alx_noeud_fenetre_sdl_opengl)(
 while( (texte[pos]<='9')
      &&(texte[pos]>='0') )
  {num_a = txt.Lire_entier(pos);
   pos++; // On passe la ,
   num_f = txt.Lire_entier(pos);
   f = f_ptr[num_f];
   pos++; // On passe le =
   (this->*f)(pos, a_ptr_fen[num_a]);
   pos++; // On passe le ;
  }
// A-t-on la description du noeud ou bien est ce la fin?
 if( (texte[pos]   == '<')
   &&(texte[pos+1] == '/') )   // </alx_noeud_scene_sdl_opengl>
  {/*pos+=31; */return;}
 alx_noeud_scene::Deserialiser_type(txt, pos);
 //pos++; // On passe la )
 //pos+=31; // </alx_noeud_fenetre_sdl_opengl>
}


//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
const alx_chaine_char& alx_noeud_fenetre_sdl_opengl::Serialiser_difference_contenu() const
{if(serialisation_contenu_fen.Taille()==0)
  {alx_noeud_fenetre_sdl_opengl::Serialiser_contenu();
   serialisation_differences_contenu_fen = serialisation_contenu_fen;
   return serialisation_differences_contenu_fen;}

 serialisation_differences_contenu_fen.Maj("",0,0);
 // Variables temporaires pour la comparaison :
 double d_tmp;
 int    i_tmp;
 // Parcours des attributs
 const char *txt = serialisation_contenu_fen.Texte();
 unsigned int pos = 4, dep = 0;
 acc_tmp.Maj("",0,0);
 if( nb_pixels_par_unite != serialisation_contenu_fen.Lire_double(pos) )
  {diff_tmp.Maj("0,2=",0,4);
   diff_tmp += nb_pixels_par_unite;
   diff_tmp.Ajouter(";",0,1);
   acc_tmp                               += diff_tmp;
   serialisation_differences_contenu_fen += diff_tmp;} else {acc_tmp.Ajouter(txt, dep, pos-dep+1);}
 dep=pos+1; pos+=5; //;2,1=
 if( M_attrib_fen != (unsigned int)serialisation_contenu_fen.Lire_entier(pos) )
  {diff_tmp.Maj("2,1=",0,4);
   diff_tmp += M_attrib_fen;
   diff_tmp.Ajouter(";",0,1);
   acc_tmp                               += diff_tmp;
   serialisation_differences_contenu_fen += diff_tmp;} else {acc_tmp.Ajouter(txt, dep, pos-dep+1);}

/* const alx_chaine_char &rep = ((alx_repere2D*)(&repere_des_fils))->alx_repere2D::Serialiser_difference_contenu();
 if(rep.Taille() > 0)
  {diff_tmp.Maj("18,3=",0,5);
   diff_tmp += rep;
   diff_tmp.Ajouter(";",0,1);
   acc_tmp                               += diff_tmp;
   serialisation_differences_contenu_fen += diff_tmp;} else {acc_tmp.Ajouter("18,3=",0,5);
                                                             acc_tmp += ((alx_repere2D*)(&repere_des_fils))->alx_repere2D::Serialisation_contenu();
                                                             acc_tmp.Ajouter(";",0,1);}    */

 // Ajout des différences des attributs du noeud :
 serialisation_differences_contenu_fen += alx_noeud_scene::Serialiser_difference_type();

 serialisation_contenu_fen  = acc_tmp;
 serialisation_contenu_fen += alx_noeud_scene::Serialisation_type();
 return serialisation_differences_contenu_fen;
}

//______________________________________________________________________________
const alx_chaine_char& alx_noeud_fenetre_sdl_opengl::Serialiser_difference_type(const alx_chaine_char &diff_contenu) const
{if(serialisation_type_fen.Taille()==0)
  {alx_noeud_fenetre_sdl_opengl::Serialiser_type();
   serialisation_differences_type_fen = serialisation_type_fen;
   return serialisation_differences_type_fen;}

 unsigned int num_f, num_a, pos = 0;
 const char *texte = serialisation_type_fen.Texte();
 f_fen_seri_diff f;

 diff_type_tmp.Maj("",0,0);

 seri_tmp.Maj("",0,0);
 chaine_cour_diff_seri_fen     = &serialisation_type_fen;
 chaine_cour_ecriture_diff_fen = &diff_type_tmp;
 chaine_cour_ecriture_seri_fen = &seri_tmp;
 if(texte[pos] == '<') pos+= 30; // on passe "<alx_noeud_fenetre_sdl_opengl "
 pos = serialisation_type_fen.Position_du_prochain(';', pos); pos++; //On passe nom=...;

// On refixe les fonctions de rappel, pour assurer la désérialisation:
 f_ptr_seri_diff_fen[6] = &(alx_noeud_fenetre_sdl_opengl::Serialiser_diff_repere);
 f_ptr_seri_diff_fen[5] = &(alx_noeud_fenetre_sdl_opengl::Serialiser_diff_4_doubles_i);
 f_ptr_seri_diff_fen[4] = &(alx_noeud_fenetre_sdl_opengl::Serialiser_diff_chaine_char);
 f_ptr_seri_diff_fen[3] = &(alx_noeud_fenetre_sdl_opengl::Serialiser_diff_4_doubles);
 f_ptr_seri_diff_fen[2] = &(alx_noeud_fenetre_sdl_opengl::Serialiser_diff_2_doubles);
 f_ptr_seri_diff_fen[1] = &(alx_noeud_fenetre_sdl_opengl::Serialiser_diff_1_double);
 f_ptr_seri_diff_fen[0] = &(alx_noeud_fenetre_sdl_opengl::Serialiser_diff_1_bool);

// Puis on désérialise la partie spécifique à la fenêtre
 while( texte[pos] != '>' )
  {dep_ecriture = pos;
   num_f = serialisation_type_fen.Lire_entier(pos);
   pos++; // On passe la ,
   num_a = serialisation_type_fen.Lire_entier(pos);
   pos++; // On passe le =
   f = f_ptr_seri_diff_fen[num_f];
   (this->*f)(pos, f_fen_diff[num_a]);
   pos++; // On passe le ;
  }

// Mise à jour des variables exprimant la sérialisation:
 if( diff_contenu.Taille()>0
   ||chaine_cour_ecriture_diff_fen->Taille()>0 )
  {serialisation_differences_type_fen.Maj("<alx_noeud_fenetre_sdl_opengl nom=",0,34);
   serialisation_differences_type_fen += Nom();
   serialisation_differences_type_fen.Ajouter(";",0,1);
   serialisation_differences_type_fen += *chaine_cour_ecriture_diff_fen;
   serialisation_differences_type_fen.Ajouter(">",0,1);
   serialisation_differences_type_fen += diff_contenu;
   serialisation_differences_type_fen.Ajouter("</alx_noeud_fenetre_sdl_opengl>",0,31);}
  else serialisation_differences_type_fen.Maj("",0,0);

 serialisation_type_fen.Maj("<alx_noeud_fenetre_sdl_opengl nom=",0,34);
 serialisation_type_fen += Nom();
 serialisation_type_fen.Ajouter(";",0,1);
 serialisation_type_fen += seri_tmp;
 serialisation_type_fen.Ajouter(">",0,1);
 serialisation_type_fen += Serialisation_contenu();
 serialisation_type_fen.Ajouter("</alx_noeud_fenetre_sdl_opengl>",0,31);

 return serialisation_differences_type_fen;
}

//______________________________________________________________________________
const alx_chaine_char& alx_noeud_fenetre_sdl_opengl::Serialiser_type(const alx_chaine_char &contenu) const
{const double *tab;
 serialisation_type_fen  = "<alx_noeud_fenetre_sdl_opengl nom="+Nom()+";";
   tab = Couleur_fond();
   serialisation_type_fen += alx_chaine_char("3,0=")  + tab[0]+"_"+tab[1]+"_"+tab[2]+"_"+tab[3]+";";
   serialisation_type_fen += alx_chaine_char("1,1=")  + Epaisseur_bordure()+";";
   serialisation_type_fen += alx_chaine_char("1,2=")  + Hauteur_entete()+";";
   serialisation_type_fen += alx_chaine_char("1,3=")  + Hauteur_corp()+";";
   //serialisation_type_fen += alx_chaine_char("1,4=")  + Longueur_entete()+";";
   serialisation_type_fen += alx_chaine_char("1,5=")  + Longueur_corp()+";";
   //serialisation_type_fen += alx_chaine_char("2,6=")  + Dimension_navigation().Dx()+"_"+Dimension_navigation().Dy() + ";";
   serialisation_type_fen += alx_chaine_char("0,7=")  + Triangle_control() + ";";
   serialisation_type_fen += alx_chaine_char("0,8=")  + Rendre_fond() + ";";
   serialisation_type_fen += alx_chaine_char("0,9=")  + Top_classos() + ";";
   serialisation_type_fen += alx_chaine_char("1,10=") + Nb_pixels_par_unite() + ";";
   tab = Couleur_masque_corp();
   serialisation_type_fen += alx_chaine_char("3,11=") + tab[0]+"_"+tab[1]+"_"+tab[2]+"_"+tab[3] + ";";
   tab = Couleur_masque_bord();
   serialisation_type_fen += alx_chaine_char("3,12=") + tab[0]+"_"+tab[1]+"_"+tab[2]+"_"+tab[3] + ";";
   serialisation_type_fen += alx_chaine_char("4,13=")+Titre()+";";

   tab = Couleur_corps();
   serialisation_type_fen += alx_chaine_char("3,14=") + tab[0]+"_"+tab[1]+"_"+tab[2]+"_"+tab[3] + ";";
   tab = Couleur_bordure_ext_angle();
   serialisation_type_fen += alx_chaine_char("3,15=") + tab[0]+"_"+tab[1]+"_"+tab[2]+"_"+tab[3] + ";";
   tab = Couleur_bordure_ext();
   serialisation_type_fen += alx_chaine_char("3,16=") + tab[0]+"_"+tab[1]+"_"+tab[2]+"_"+tab[3] + ";";
   tab = Couleur_bordure_int_angle();
   serialisation_type_fen += alx_chaine_char("3,17=") + tab[0]+"_"+tab[1]+"_"+tab[2]+"_"+tab[3] + ";";
   tab = Couleur_bordure_int();
   serialisation_type_fen += alx_chaine_char("3,18=") + tab[0]+"_"+tab[1]+"_"+tab[2]+"_"+tab[3] + ";";

   serialisation_type_fen += alx_chaine_char("6,19=") + repere_des_fils.alx_repere2D::Serialiser_contenu() + ";";

   serialisation_type_fen.Ajouter("5,20=",0,5);
   for(unsigned int i=0; i<4; i++)
    {tab = Couleur_entete(i);
     serialisation_type_fen += i+alx_chaine_char("_")+tab[0]+"_"+tab[1]+"_"+tab[2]+"_"+tab[3];
     if(i<3) serialisation_type_fen.Ajouter("_",0,1);
      else serialisation_type_fen.Ajouter(";",0,1);
    }

 serialisation_type_fen += ">"+contenu+"</alx_noeud_fenetre_sdl_opengl>";
 return serialisation_type_fen;}

//______________________________________________________________________________
const alx_chaine_char& alx_noeud_fenetre_sdl_opengl::Serialiser_contenu() const
{serialisation_contenu_fen  = "";
   serialisation_contenu_fen += alx_chaine_char("0,2=")  + nb_pixels_par_unite                  + ";";
   serialisation_contenu_fen += alx_chaine_char("2,1=")  + M_attrib_fen                         + ";";
   //serialisation_contenu_fen += alx_chaine_char("17,4=") + titre                                + ";";
   //serialisation_contenu_fen += alx_chaine_char("18,3=") + ((alx_repere2D*)(&repere_des_fils))->alx_repere2D::Serialiser_contenu() + ";";

   serialisation_contenu_fen += alx_noeud_scene::Serialiser_type( alx_noeud_scene::Serialiser_contenu() );
 return serialisation_contenu_fen;
}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
void alx_noeud_fenetre_sdl_opengl::init_default()
{Origine(0,0);
 Marqueur_attributs_fen(M_fen_tout, false);
 fen_texture_tempon_prete = false;
/* rendu_ecran_direct   = false;
 bord_translucide  = false; corp_translucide  = false;
 */
// ne_pas_pre_rendre false;
 forcer_pre_rendu_entete = false;
 Marqueur_attributs_fen( M_fen_lisser|M_fen_gerer_corps|M_fen_afficher_corps|M_fen_fond_touchable
                        |M_fen_gerer_entete|M_fen_afficher_entete|M_fen_gerer_bordure
                        |M_fen_afficher_bordure|M_fen_gerer_zone_rot|M_fen_afficher_zone_rot
                        |M_fen_voir_si_pt_dans_fils
                       , true );

 boite_noeud.Est_vide( false );
 boite_fils.Est_vide( false );
 boite_noeud_et_fils.Est_vide( false );
 boite_noeud_et_fils_glob.Est_vide( false );

 entete_prcdt[0] = entete_prcdt[1] = -10;

 Position_dans_liste_des_fils_changeable(true);
 Noeud_de_coupe(true);

 alx_noeud_scene::Ajouter_fils( (alx_noeud_scene*)(&repere_des_fils) );

 sim_contact    = (alx_simulateur_points_contact*)NULL;
 interface_mere = (alx_interface_sdl_opengl_1*)NULL;
 fenetre_opengl = (alx_fenetre_opengl*)NULL;
 fenetre        = (alx_fenetre*)NULL;
 prerendeur_fiche = prerendeur_entete = prerendeur_externe = (alx_prerendeur*)NULL;

 Compter_affichage( false );
 Compter_pre_rendu( true );
 Rendre_fond( true );
 init_ptr();
}

//______________________________________________________________________________
void alx_noeud_fenetre_sdl_opengl::init_ptr()
{// Les attributs traitables directements
 memset(a_ptr_fen , 0, 50*sizeof(void*));
 memset(f_fen     , 0, 50*sizeof(void*));
 memset(f_fen_diff, 0, 50*sizeof(void*));

 a_ptr_fen[0]  = &(nb_pixels_par_unite);
 a_ptr_fen[2]  = &(M_attrib_fen);

 a_ptr_fen[3]  = NULL; //&(afficher_corps);
 a_ptr_fen[4]  = NULL; //&(gerer_entete);
 a_ptr_fen[5]  = NULL; //&(afficher_entete);
 a_ptr_fen[6]  = NULL; //&(afficher_bordure);
 a_ptr_fen[7]  = NULL; //&(gerer_bordure);
 a_ptr_fen[8]  = NULL; //&(afficher_zone_rot);
 a_ptr_fen[9]  = NULL; //&(gerer_zone_rot);
 a_ptr_fen[10] = NULL; //&(texture_tempon_prete);
 a_ptr_fen[11] = NULL; //&(rendu_ecran_direct);
 a_ptr_fen[12] = NULL; //&(corp_translucide);
 a_ptr_fen[13] = NULL; //&(bord_translucide);
 a_ptr_fen[14] = NULL; //&(lisser);
 a_ptr_fen[15] = NULL; //&(navigation_interne);
 a_ptr_fen[16] = NULL; //&(contact_avec_corp);
 a_ptr_fen[17] = NULL; //&(titre);
 a_ptr_fen[18] = NULL; //(alx_repere2D*)(&repere_des_fils); */

 a_ptr_fen[40] = NULL; //&(entete_prcdt[0]);
 a_ptr_fen[41] = NULL; //&(entete_prcdt[1]);

// Ce qui devra être modifié par appel de fonctions
 f_ptr_fen[6] = &(alx_noeud_fenetre_sdl_opengl::Deserialiser_repere);
 f_ptr_fen[5] = &(alx_noeud_fenetre_sdl_opengl::Deserialiser_4_doubles_i);
 f_ptr_fen[4] = &(alx_noeud_fenetre_sdl_opengl::Deserialiser_chaine_char);
 f_ptr_fen[3] = &(alx_noeud_fenetre_sdl_opengl::Deserialiser_4_doubles);
 f_ptr_fen[2] = &(alx_noeud_fenetre_sdl_opengl::Deserialiser_2_doubles);
 f_ptr_fen[1] = &(alx_noeud_fenetre_sdl_opengl::Deserialiser_1_double);
 f_ptr_fen[0] = &(alx_noeud_fenetre_sdl_opengl::Deserialiser_1_bool);

 fd1[0]    = &(alx_noeud_fenetre_sdl_opengl::Epaisseur_bordure);    f_fen[1]       = &(fd1[0]);
 fsd_d1[0] = &(alx_noeud_fenetre_sdl_opengl::Epaisseur_bordure);    f_fen_diff[1]  = &(fsd_d1[0]);
 fd1[1]    = &(alx_noeud_fenetre_sdl_opengl::Hauteur_entete);       f_fen[2]       = &(fd1[1]);
 fsd_d1[1] = &(alx_noeud_fenetre_sdl_opengl::Hauteur_entete);       f_fen_diff[2]  = &(fsd_d1[1]);
 fd1[2]    = &(alx_noeud_fenetre_sdl_opengl::Hauteur_corp);         f_fen[3]       = &(fd1[2]);
 fsd_d1[2] = &(alx_noeud_fenetre_sdl_opengl::Hauteur_corp);         f_fen_diff[3]  = &(fsd_d1[2]);
 fd1[3]    = &(alx_noeud_fenetre_sdl_opengl::Longueur_entete);      f_fen[4]       = &(fd1[3]);
 fsd_d1[3] = &(alx_noeud_fenetre_sdl_opengl::Longueur_entete);      f_fen_diff[4]  = &(fsd_d1[3]);
 fd1[4]    = &(alx_noeud_fenetre_sdl_opengl::Longueur_corp);        f_fen[5]       = &(fd1[4]);
 fsd_d1[4] = &(alx_noeud_fenetre_sdl_opengl::Longueur_corp);        f_fen_diff[5]  = &(fsd_d1[4]);
 fd2[0]    = &(alx_noeud_fenetre_sdl_opengl::Dimension_navigation); f_fen[6]       = &(fd2[0]);

 fb1[0]    = &(alx_noeud_fenetre_sdl_opengl::Triangle_control);     f_fen[7]       = &(fb1[0]);
 fsd_b1[0] = &(alx_noeud_fenetre_sdl_opengl::Triangle_control);     f_fen_diff[7]  = &(fsd_b1[0]);
 fb1[1]    = &(alx_noeud_fenetre_sdl_opengl::Rendre_fond);          f_fen[8]       = &(fb1[1]);
 fsd_b1[1] = &(alx_noeud_fenetre_sdl_opengl::Rendre_fond);          f_fen_diff[8]  = &(fsd_b1[1]);
 fb1[2]    = &(alx_noeud_fenetre_sdl_opengl::Top_classos);          f_fen[9]       = &(fb1[2]);
 fsd_b1[2] = &(alx_noeud_fenetre_sdl_opengl::Top_classos);          f_fen_diff[9]  = &(fsd_b1[2]);
 fd1[5]    = &(alx_noeud_fenetre_sdl_opengl::Nb_pixels_par_unite);  f_fen[10]      = &(fd1[5]);
 fsd_d1[5] = &(alx_noeud_fenetre_sdl_opengl::Nb_pixels_par_unite);  f_fen_diff[10] = &(fsd_d1[5]);

 fcc[0]    = &(alx_noeud_fenetre_sdl_opengl::Titre);             ;  f_fen[13]      = &(fcc[0]);
 fsd_cc[0] = &(alx_noeud_fenetre_sdl_opengl::Titre);             ;  f_fen_diff[13] = &(fsd_cc[0]);

 fd4[0]    = &(alx_noeud_fenetre_sdl_opengl::Couleur_fond);              f_fen[0]       = &(fd4[0]);
 fsd_d4[0] = &(alx_noeud_fenetre_sdl_opengl::Couleur_fond);              f_fen_diff[0]  = &(fsd_d4[0]);
 fd4[1]    = &(alx_noeud_fenetre_sdl_opengl::Couleur_masque_corp);       f_fen[11]      = &(fd4[1]);
 fsd_d4[1] = &(alx_noeud_fenetre_sdl_opengl::Couleur_masque_corp);       f_fen_diff[11] = &(fsd_d4[1]);
 fd4[2]    = &(alx_noeud_fenetre_sdl_opengl::Couleur_masque_bord);       f_fen[12]      = &(fd4[2]);
 fsd_d4[2] = &(alx_noeud_fenetre_sdl_opengl::Couleur_masque_bord);       f_fen_diff[12] = &(fsd_d4[2]);
 fd4[3]    = &(alx_noeud_fenetre_sdl_opengl::Couleur_corps);             f_fen[14]      = &(fd4[3]);
 fsd_d4[3] = &(alx_noeud_fenetre_sdl_opengl::Couleur_corps);             f_fen_diff[14] = &(fsd_d4[3]);
 fd4[4]    = &(alx_noeud_fenetre_sdl_opengl::Couleur_bordure_ext_angle); f_fen[15]      = &(fd4[4]);
 fsd_d4[4] = &(alx_noeud_fenetre_sdl_opengl::Couleur_bordure_ext_angle); f_fen_diff[15] = &(fsd_d4[4]);
 fd4[5]    = &(alx_noeud_fenetre_sdl_opengl::Couleur_bordure_ext);       f_fen[16]      = &(fd4[5]);
 fsd_d4[5] = &(alx_noeud_fenetre_sdl_opengl::Couleur_bordure_ext);       f_fen_diff[16] = &(fsd_d4[5]);
 fd4[6]    = &(alx_noeud_fenetre_sdl_opengl::Couleur_bordure_int_angle); f_fen[17]      = &(fd4[6]);
 fsd_d4[6] = &(alx_noeud_fenetre_sdl_opengl::Couleur_bordure_int_angle); f_fen_diff[17] = &(fsd_d4[6]);
 fd4[7]    = &(alx_noeud_fenetre_sdl_opengl::Couleur_bordure_int);       f_fen[18]      = &(fd4[7]);
 fsd_d4[7] = &(alx_noeud_fenetre_sdl_opengl::Couleur_bordure_int);       f_fen_diff[18] = &(fsd_d4[7]);

 frep[0]   = &(alx_noeud_fenetre_sdl_opengl::Repere_des_fils_pour_modif); f_fen[19]      = &(frep[0]);
 fsd_rep[0]= &(alx_noeud_fenetre_sdl_opengl::Repere_des_fils);            f_fen_diff[19] = &(fsd_rep[0]);

 fd4_i[0]    = &(alx_noeud_fenetre_sdl_opengl::Couleur_entete);           f_fen[20]      = &(fd4_i[0]);
 fsd_d4_i[0] = &(alx_noeud_fenetre_sdl_opengl::Couleur_entete);           f_fen_diff[20] = &(fsd_d4_i[0]);
}

//______________________________________________________________________________
void alx_noeud_fenetre_sdl_opengl::init( alx_fenetre_opengl *f_opengl, const double tx, const double ty
                                       , alx_interface_sdl_opengl_1 *interface_m
                                       , alx_prerendeur *prerendeur
                                       , const double nb_pix_par_unite
                                       , HDC hdc, HGLRC hglrc
                                       , ALXGL_TYPE_RENDU type_rendu )
{init_default();

 //alx_fenetre_opengl *fiche_gl = f_opengl;
 fenetre_opengl = f_opengl;
 alx_fenetre *fiche;
 if(f_opengl)
  {fiche = f_opengl->Fenetre();
   titre = (char*)(fiche->Nom());}

 if(f_opengl)
   fenetre = f_opengl->Fenetre();
  else fenetre = NULL;
 if(interface_m)
  {this->interface_mere = interface_m;
   this->sim_contact = interface_mere->sim_contact;}
 prerendeur_externe = prerendeur;

 Etirement_du_contenu(1);
 nb_pixels_par_unite = nb_pix_par_unite;
 nb_pixels_par_unite_entete = 1;

 alx_noeud_scene::Prerendeur(prerendeur);
 //prerendeur->Prendre_en_charge( (alx_noeud_scene*)this, 1, ACTION_DU_RENDU_rendre_toujours, 'p');
 Prerendeur_externe(prerendeur);
/* Prerendeur_fiche( new alx_prerendeur( this
                                     , prerendeur->Ecran_x()
                                     , prerendeur->Ecran_y()
                                     , prerendeur->Angle_camera()) );
 Prerendeur_fiche()->Vider();*/

 if(f_opengl)
   zdr_corp.Nb_unite_x( fiche->Longueur_corp() * nb_pixels_par_unite );
  else zdr_corp.Nb_unite_x( tx * nb_pixels_par_unite );
 if(f_opengl)
   zdr_corp.Nb_unite_y( fiche->Hauteur_corp()  * nb_pixels_par_unite );
  else zdr_corp.Nb_unite_y( ty  * nb_pixels_par_unite );
 zdr_corp.Noeud_a_rendre( (alx_noeud_scene*)this );

 zdr_entete.Effacer_fond(false);
 noeud_pere_entete.Ajouter_fils( (alx_noeud_scene*)(&noeud_entete) );
 if(f_opengl)
   zdr_entete.Nb_unite_x( fiche->Longueur_entete() );
  else zdr_entete.Nb_unite_x( tx );
 if(f_opengl)
   zdr_entete.Nb_unite_y( fiche->Hauteur_entete());
  else zdr_entete.Nb_unite_y( 64 );
 zdr_entete.Noeud_a_rendre( (alx_noeud_scene*)(&noeud_pere_entete) );
 zdr_entete.PreRendre();

 if(f_opengl)
   f_opengl->Zdr_entete(&zdr_entete);

/* Prerendeur_entete( new alx_prerendeur( &noeud_pere_entete
                                      , prerendeur->Ecran_x()
                                      , prerendeur->Ecran_y()
                                      , prerendeur->Angle_camera()) );
 Prerendeur_entete()->Vider();*/
}

//______________________________________________________________________________
alx_noeud_fenetre_sdl_opengl::alx_noeud_fenetre_sdl_opengl( const char *titre, const double tx, const double ty
                                                          , alx_interface_sdl_opengl_1 *interface_m ) : alx_noeud_scene()
                                                                                            , zdr_corp( interface_m->Prerendeur()
                                                                                                      , 1
                                                                                                      , NULL, NULL
                                                                                                      , ALXGL_RENDU_ECRAN_PUIS_COPIE_DANS_TEXTURE )
                                                                                            , zdr_entete( interface_m->Prerendeur()
                                                                                                        , 1
                                                                                                        , NULL, NULL
                                                                                                        , ALXGL_RENDU_ECRAN_PUIS_COPIE_DANS_TEXTURE )
                                                                                            , noeud_entete(NULL)
{Navigation_interne(false);
 this->titre = titre;
 init(NULL, tx, ty, interface_m, interface_m->Prerendeur(), 1, NULL, NULL, ALXGL_RENDU_ECRAN_PUIS_COPIE_DANS_TEXTURE);
 Nom("f_"+Nom());
 noeud_pour_seri_fils = &repere_des_fils;
 Contenu_dans_fils_ssi_contenu_dans_noeud(true);
}

//______________________________________________________________________________

alx_noeud_fenetre_sdl_opengl::~alx_noeud_fenetre_sdl_opengl()
{delete Prerendeur_fiche();
 delete Prerendeur_entete();
}

//______________________________________________________________________________
alx_noeud_fenetre_sdl_opengl::alx_noeud_fenetre_sdl_opengl( alx_fenetre_opengl *f_opengl
                                                          , alx_interface_sdl_opengl_1 *interface_m
                                                          , alx_prerendeur *prerendeur
                                                          , const double nb_pix_par_unite ) : alx_noeud_scene()
                                                                                            , zdr_corp( prerendeur
                                                                                                      , nb_pix_par_unite
                                                                                                      , NULL, NULL
                                                                                                      , ALXGL_RENDU_ECRAN_PUIS_COPIE_DANS_TEXTURE )
                                                                                            , zdr_entete( prerendeur
                                                                                                        , nb_pix_par_unite
                                                                                                        , NULL, NULL
                                                                                                        , ALXGL_RENDU_ECRAN_PUIS_COPIE_DANS_TEXTURE )
                                                                                            , noeud_entete(f_opengl)
{Navigation_interne(false);
 init(f_opengl, -1, -1, interface_m, prerendeur, nb_pix_par_unite, NULL, NULL, ALXGL_RENDU_ECRAN_PUIS_COPIE_DANS_TEXTURE);
 Nom("f_"+Nom());
 noeud_pour_seri_fils = &repere_des_fils;
}

//______________________________________________________________________________
alx_noeud_fenetre_sdl_opengl::alx_noeud_fenetre_sdl_opengl( alx_fenetre_opengl *f_opengl
                                                          , alx_interface_sdl_opengl_1 *interface_m
                                                          , alx_prerendeur *prerendeur
                                                          , const double nb_pix_par_unite
                                                          , HDC hdc, HGLRC hglrc
                                                          , ALXGL_TYPE_RENDU type_rendu) : alx_noeud_scene()
                                                                                         , zdr_corp( prerendeur
                                                                                                   , nb_pix_par_unite
                                                                                                   , hdc, hglrc
                                                                                                   , type_rendu )
                                                                                         , zdr_entete( prerendeur
                                                                                                     , nb_pix_par_unite
                                                                                                     , hdc, hglrc
                                                                                                     , type_rendu )
                                                                                         , noeud_entete(f_opengl)
{Navigation_interne(false);
 init(f_opengl, -1, -1, interface_m, prerendeur, nb_pix_par_unite, hdc, hglrc, type_rendu);
 Nom("f_"+Nom());
 noeud_pour_seri_fils = &repere_des_fils;
}

//______________________________________________________________________________
void alx_noeud_fenetre_sdl_opengl::Afficher()
{if(Nb_affichage() > Nb_max_affichage()) return;
 if( !LR_Avant_affichage.Vide() ) {LR_Avant_affichage.Rappeler( this );}

   this->Transfo_repere();
   if(Noeud_puis_fils())
    {this->Dessin_noeud();
    } else {this->Dessin_noeud();}
   this->Depiler_transfo_repere();

// Mise à jour des coordonnées de la boîte englobante dans les repères des pères.
 Calculer_boite_noeud();
 if( !LR_Apres_affichage.Vide() ) {LR_Apres_affichage.Rappeler( this );}
}

//______________________________________________________________________________
void alx_noeud_fenetre_sdl_opengl::PreRendre()
{}

//______________________________________________________________________________
void alx_noeud_fenetre_sdl_opengl::PreRendreEntete()
{if(!Fenetre()) return;
 if( ( (entete_prcdt[0]==Fenetre()->Longueur_entete())
     &&(entete_prcdt[1]==Fenetre()->Hauteur_entete()) )
     &&( zdr_entete.Nb_pixels_par_unite() == Nb_pixels_par_unite_entete())
   &&(!Forcer_pre_rendu_entete()) )
   return;
 zdr_entete.Nb_unite_x( entete_prcdt[0]=Fenetre()->Longueur_entete());
 zdr_entete.Nb_unite_y( entete_prcdt[1]=Fenetre()->Hauteur_entete() );
 zdr_entete.Nb_pixels_par_unite( Nb_pixels_par_unite_entete() );
 zdr_entete.PreRendre();
 Forcer_pre_rendu_entete(false);
}

//______________________________________________________________________________
//______________________________________________________________________________
info_du_contenant* alx_noeud_fenetre_sdl_opengl::Contient(const alx_point2D &pt, int action)
{// Tenir compte du fait que le point peut être contenu dans le corp.
 if(!Noeud_touchable())
   return (info_du_contenant*)NULL;
 info_du_contenant *rep, *rep_fils;

 pt_pour_contient_fen.maj(pt);
 if(Prendre_en_compte_repere())
  {this->Changer_coordonnees( pt_pour_contient_fen );}

// A-t-on cliqué sur le noeud et doit on renvoyer ce résultat?
 rep = this->Noeud_contient(pt_pour_contient_fen, action);
 if( (!Contact_avec_corp())
   &&( (Noeud_de_coupe())
     ||( (rep != (info_du_contenant*)NULL)
       &&(!Noeud_puis_fils()))) ) return this->Resultat_contient(rep);

// Tous les cas où le résultats des fils dépend de celui du père :
 if(Contenu_dans_fils_ssi_contenu_dans_noeud()     && Contenu_dans_fils_ssi_pas_contenu_dans_noeud())              {return this->Resultat_contient(rep);}
 if(Contenu_dans_fils_ssi_contenu_dans_noeud()     && (!Contact_avec_corp()) && (rep == (info_du_contenant*)NULL)) {return this->Resultat_contient(rep);}
 if(Contenu_dans_fils_ssi_pas_contenu_dans_noeud() && (Contact_avec_corp())  && (rep != (info_du_contenant*)NULL)) {return this->Resultat_contient(rep);}

// Si je suis ici c'est que les fils sont prioritaire, si j'ai cliqué sur l'un d'eux c'est lui que je renverrais
 rep_fils = this->Contenu_dans_fils(pt_pour_contient_fen, action);
 if(rep_fils)
  {if(rep && (rep != Rep_puit())) {delete rep;}
   return this->Resultat_contient(rep_fils);
  }
 return this->Resultat_contient(rep);
}

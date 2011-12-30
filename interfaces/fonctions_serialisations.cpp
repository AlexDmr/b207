#ifndef __ALX_FONCTIONS_SERIALISATIONS_CPP__
#define __ALX_FONCTIONS_SERIALISATIONS_CPP__

#include "fonctions_serialisations.h"
#include "..\utilitaires\alx_chaine_char.h"
#include "..\physique\math_alex.cpp"

template<class C> void CFSD<C>::Serialiser_diff_4_doubles(unsigned int &pos, void *fptr) const
{unsigned int pos_fin_amorce = pos;
 double T1[4];
 T1[0] = chaine_cour_diff_seri->Lire_double(pos); pos++;
 T1[1] = chaine_cour_diff_seri->Lire_double(pos); pos++;
 T1[2] = chaine_cour_diff_seri->Lire_double(pos); pos++;
 T1[3] = chaine_cour_diff_seri->Lire_double(pos);
 f_consultation_4_doubles f4 = *((f_consultation_4_doubles*)fptr);
 const double *T = (instanceC->*f4)();
 bool diff = false;
 for(unsigned int i=0;i<4;i++)
   if( absd(T[i]-T1[i]) > EPSILON ) {diff=true; break;}
 if(diff)
  {chaine_tmp.Maj(chaine_cour_diff_seri->Texte(), dep_ecriture, pos_fin_amorce-dep_ecriture);
   chaine_tmp += T[0]; chaine_tmp.Ajouter("_",0,1);
   chaine_tmp += T[1]; chaine_tmp.Ajouter("_",0,1);
   chaine_tmp += T[2]; chaine_tmp.Ajouter("_",0,1);
   chaine_tmp += T[3]; chaine_tmp.Ajouter(";",0,1);
   *chaine_cour_ecriture_diff += chaine_tmp;
   *chaine_cour_ecriture_seri += chaine_tmp;} else {chaine_cour_ecriture_seri->Ajouter(chaine_cour_diff_seri->Texte(), dep_ecriture, pos-dep_ecriture+1);}
}

template<class C> void CFSD<C>::Serialiser_diff_2_doubles(unsigned int &pos, void *fptr) const
{}

template<class C> void CFSD<C>::Serialiser_diff_1_double (unsigned int &pos, void *fptr) const
{unsigned int pos_fin_amorce = pos;
 double a, ap;
 a  = chaine_cour_diff_seri->Lire_double(pos);
 ap = (instanceC->**((f_consultation_1_double*)fptr))();
 if( absd(a-ap) > EPSILON )
  {chaine_tmp.Maj(chaine_cour_diff_seri->Texte(), dep_ecriture, pos_fin_amorce-dep_ecriture); chaine_tmp+=ap; chaine_tmp.Ajouter(";",0,1);
   *chaine_cour_ecriture_diff += chaine_tmp;
   *chaine_cour_ecriture_seri += chaine_tmp;} else {chaine_cour_ecriture_seri->Ajouter(chaine_cour_diff_seri->Texte(), dep_ecriture, pos-dep_ecriture+1);}
}

//______________________________________________________________________________
template<class C> void CFSD<C>::Serialiser_diff_1_int    (unsigned int &pos, void *fptr) const
{unsigned int pos_fin_amorce = pos;
 int a, ap;
 a  = chaine_cour_diff_seri->Lire_entier(pos);
 ap = (instanceC->**((f_consultation_1_int*)fptr))();
 if(a != ap)
  {chaine_tmp.Maj(chaine_cour_diff_seri->Texte(), dep_ecriture, pos_fin_amorce-dep_ecriture); chaine_tmp+=ap; chaine_tmp.Ajouter(";",0,1);
   *chaine_cour_ecriture_diff += chaine_tmp;
   *chaine_cour_ecriture_seri += chaine_tmp;} else {chaine_cour_ecriture_seri->Ajouter(chaine_cour_diff_seri->Texte(), dep_ecriture, pos-dep_ecriture+1);}
}

//______________________________________________________________________________
template<class C> void CFSD<C>::Serialiser_diff_1_bool   (unsigned int &pos, void *f) const
{unsigned int pos_fin_amorce = pos;
 bool a, ap;
 a  = chaine_cour_diff_seri->Lire_bool(pos);
 ap = (instanceC->**((f_consultation_1_bool*)f))();
 if(a != ap)
  {chaine_tmp.Maj(chaine_cour_diff_seri->Texte(), dep_ecriture, pos_fin_amorce-dep_ecriture); chaine_tmp+=ap; chaine_tmp.Ajouter(";",0,1);
   *chaine_cour_ecriture_diff += chaine_tmp;
   *chaine_cour_ecriture_seri += chaine_tmp;} else {chaine_cour_ecriture_seri->Ajouter(chaine_cour_diff_seri->Texte(), dep_ecriture, pos-dep_ecriture+1);}
}

//______________________________________________________________________________
// Structure de la sérialisation d'un polygone
// 1) taille de la sérialisation (moins elle même et le séparateur nécessaire, ex 4_coco
// 2) nbcontours_(trou_nbvertexs_(x'y_)*|)*
template<class C> void CFSD<C>::Serialiser_diff_polygone(unsigned int &pos, void *fptr) const
{unsigned int pos_fin_amorce = pos;
// double val_d_tmp;
 gpc_polygon *poly = (gpc_polygon *)NULL;
 f_consultation_polygone fct = *((f_consultation_polygone*)fptr);
 const bool poly_a_change = (instanceC->*fct)( &poly );
 unsigned int taille_serialisation = chaine_cour_diff_seri->Lire_entier(pos); pos++;
 pos += taille_serialisation;
 if(poly_a_change)
  {// Sérialisation du polygone
   cc_poly_tmp = poly->num_contours;
   cc_poly_tmp.Ajouter("_",0,1);
   for(unsigned int i=0; i<(unsigned int)(poly->num_contours); i++)
    {gpc_vertex_list *VL = &(poly->contour[i]);
     cc_poly_tmp += poly->hole[i];
     cc_poly_tmp.Ajouter("_",0,1);
     cc_poly_tmp += VL->num_vertices;
     cc_poly_tmp.Ajouter("_",0,1);
     for(unsigned int j=0; j<(unsigned int)(VL->num_vertices); j++)
      {cc_poly_tmp += VL->vertex[j].x;
       cc_poly_tmp.Ajouter("'",0,1);
       cc_poly_tmp += VL->vertex[j].y;
       cc_poly_tmp.Ajouter("_",0,1);}
     cc_poly_tmp.Ajouter("|",0,1);
    }
   chaine_cour_ecriture_diff->Ajouter(chaine_cour_diff_seri->Texte(), dep_ecriture, pos_fin_amorce-dep_ecriture);
     *chaine_cour_ecriture_diff += cc_poly_tmp.Taille();
      chaine_cour_ecriture_diff->Ajouter("|",0,1);
     *chaine_cour_ecriture_diff += cc_poly_tmp;
      chaine_cour_ecriture_diff->Ajouter(";",0,1);
   chaine_cour_ecriture_seri->Ajouter(chaine_cour_diff_seri->Texte(), dep_ecriture, pos_fin_amorce-dep_ecriture);
     *chaine_cour_ecriture_seri += cc_poly_tmp.Taille();
      chaine_cour_ecriture_seri->Ajouter("|",0,1);
     *chaine_cour_ecriture_seri += cc_poly_tmp;
      chaine_cour_ecriture_seri->Ajouter(";",0,1);
  } else {chaine_cour_ecriture_seri->Ajouter(chaine_cour_diff_seri->Texte(), dep_ecriture, pos-dep_ecriture+1);}
}

//______________________________________________________________________________
template<class C> void CFSD<C>::Serialiser_diff_chaine_char   (unsigned int &pos, void *fptr) const
{unsigned int pos_fin_amorce = pos;
  // Calcul de la chaine déja sérialisée:
   //unsigned int dep_cc1 = pos;
   const alx_chaine_char *cc = (alx_chaine_char*)NULL;
   f_consultation_chaine_char fct = *((f_consultation_chaine_char*)fptr);
     const bool cc_a_change = (instanceC->*fct)( &cc );
     unsigned int taille_serialisation = chaine_cour_diff_seri->Lire_entier(pos); pos++;
     pos += taille_serialisation;
   if(cc_a_change)
   {chaine_cour_ecriture_diff->Ajouter(chaine_cour_diff_seri->Texte(), dep_ecriture, pos_fin_amorce-dep_ecriture);
     if(cc)
      {*chaine_cour_ecriture_diff += cc->Taille();
        chaine_cour_ecriture_diff->Ajouter("|",0,1);
       *chaine_cour_ecriture_diff += *cc;
        chaine_cour_ecriture_diff->Ajouter(";",0,1);
      } else {chaine_cour_ecriture_diff->Ajouter("0|;",0,3);}
    chaine_cour_ecriture_seri->Ajouter(chaine_cour_diff_seri->Texte(), dep_ecriture, pos_fin_amorce-dep_ecriture);
     if(cc)
      {*chaine_cour_ecriture_seri += cc->Taille();
        chaine_cour_ecriture_seri->Ajouter("|",0,1);
       *chaine_cour_ecriture_seri += *cc;
        chaine_cour_ecriture_seri->Ajouter(";",0,1);
      } else {chaine_cour_ecriture_seri->Ajouter("0|;",0,3);}
    } else {chaine_cour_ecriture_seri->Ajouter(chaine_cour_diff_seri->Texte(), dep_ecriture, pos-dep_ecriture+1);}
}

/*template<class C> void CFSD<C>::Serialiser_diff_chaine_char   (unsigned int &pos, void *f) const
{bool difference;
 unsigned int pos_fin_amorce = pos, taille;
  // Calcul de la chaine déja sérialisée:
   //unsigned int dep_cc1 = pos;
   const char *txt = chaine_cour_diff_seri->Texte();
   taille = chaine_cour_diff_seri->Lire_entier(pos); pos++; // On lie la taille et on passe le séparateur.
   const alx_chaine_char &cc2 = (instanceC->**((f_consultation_chaine_char*)f))();
   if(taille == cc2.Taille())
    {unsigned int i = 0;
     const char *txt2 = cc2.Texte();
     while( (i<taille)
          &&(txt[pos+i] == txt2[i]) ) i++;
     difference = (i<taille);
    }
    else difference = true;
 pos += taille;
 if(difference)
  {chaine_tmp.Maj(chaine_cour_diff_seri->Texte(), dep_ecriture, pos_fin_amorce-dep_ecriture);
   chaine_tmp += cc2.Taille(); chaine_tmp.Ajouter("_",0,1);
   chaine_tmp += cc2; chaine_tmp.Ajouter(";",0,1);
   *chaine_cour_ecriture_diff += chaine_tmp;
   *chaine_cour_ecriture_seri += chaine_tmp;} else {chaine_cour_ecriture_seri->Ajouter(chaine_cour_diff_seri->Texte(), dep_ecriture, pos-dep_ecriture+1);}
}
*/
//______________________________________________________________________________
template<class C> void CFSD<C>::Serialiser_diff_4_doubles_i(unsigned int &pos, void *fptr) const
{unsigned int pos_fin_amorce = pos, n, dep_contenu;
 double T1[4];
 bool amorce_ecrite      = false
    , amorce_diff_ecrite = false;

 f_consultation_4_doubles_i f4 = *((f_consultation_4_doubles_i*)fptr);
 for(unsigned int j=0; j<4; j++)
  {dep_contenu = pos;
   n     = chaine_cour_diff_seri->Lire_entier(pos); pos++;
   T1[0] = chaine_cour_diff_seri->Lire_double(pos); pos++;
   T1[1] = chaine_cour_diff_seri->Lire_double(pos); pos++;
   T1[2] = chaine_cour_diff_seri->Lire_double(pos); pos++;
   T1[3] = chaine_cour_diff_seri->Lire_double(pos); pos++;
   const double *T = (instanceC->*f4)(j);
   bool diff = false;
   for(unsigned int i=0;i<4;i++)
     if( absd(T[i]-T1[i]) > EPSILON ) {diff=true; break;}
   if(diff)
    {chaine_tmp.Maj("",0,0);
     if(!amorce_ecrite)
      {chaine_tmp.Maj(chaine_cour_diff_seri->Texte(), dep_ecriture, pos_fin_amorce-dep_ecriture);
       amorce_ecrite = true;}
     if(!amorce_diff_ecrite)
      {chaine_cour_ecriture_diff->Ajouter(chaine_cour_diff_seri->Texte(), dep_ecriture, pos_fin_amorce-dep_ecriture);
       amorce_diff_ecrite = true;}
      else chaine_cour_ecriture_diff->Ajouter("_",0,1);
     chaine_tmp += n;    chaine_tmp.Ajouter("_",0,1);
     chaine_tmp += T[0]; chaine_tmp.Ajouter("_",0,1);
     chaine_tmp += T[1]; chaine_tmp.Ajouter("_",0,1);
     chaine_tmp += T[2]; chaine_tmp.Ajouter("_",0,1);
     chaine_tmp += T[3];
     *chaine_cour_ecriture_diff += chaine_tmp;
     *chaine_cour_ecriture_seri += chaine_tmp;
     if(j<3) chaine_cour_ecriture_seri->Ajouter("_",0,1);
      else chaine_cour_ecriture_seri->Ajouter(";",0,1);
     }
    else
     {if(!amorce_ecrite)
       {chaine_cour_ecriture_seri->Ajouter(chaine_cour_diff_seri->Texte(), dep_ecriture, pos_fin_amorce-dep_ecriture);
        amorce_ecrite = true;}
      chaine_cour_ecriture_seri->Ajouter(chaine_cour_diff_seri->Texte(), dep_contenu, pos-dep_contenu);
     }
  }

 if(amorce_diff_ecrite)
   chaine_cour_ecriture_diff->Ajouter(";",0,1);
 // On a avancé un coup de trop, on revient sur le ;
 pos--;
}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
// tailleserie_nbcontours_(trou_nbvertexs_(x'y_)*|)*
template<class C> void CFSD<C>::Deserialiser_polygone(unsigned int &pos, void *fptr)
{double tab_double_tmp[65536];
 gpc_vertex_list VL;
 gpc_free_polygon(&poly_tmp);
   poly_tmp.contour = NULL;
   poly_tmp.hole    = NULL;
 unsigned int taille_serialisation = chaine_cour->Lire_entier(pos); pos++;
 unsigned int nb_contours          = chaine_cour->Lire_entier(pos); pos++;

 for(unsigned int i=0; i<nb_contours; i++)
  {unsigned int trou     = chaine_cour->Lire_entier(pos); pos++;
   unsigned int pos_tab  = 0;

   VL.num_vertices = chaine_cour->Lire_entier(pos); pos++;
   VL.vertex       = (gpc_vertex*)(void*)tab_double_tmp;
   for(unsigned int j=0; j<(unsigned int)VL.num_vertices; j++)
    {tab_double_tmp[pos_tab] = chaine_cour->Lire_double(pos); pos++; pos_tab++;
     tab_double_tmp[pos_tab] = chaine_cour->Lire_double(pos); pos++; pos_tab++;}
   pos++; // le '|'
   gpc_add_contour( &poly_tmp
                  , &VL
                  , trou);
   VL.vertex       = (gpc_vertex*)NULL;
   VL.num_vertices = 0;
  }

 poly_reshaped = true;
 //f_deserialisation_polygone f_maj_poly = *((f_deserialisation_polygone*)fptr);
 //(instanceC->*f_maj_poly)( &poly_tmp );
}
//______________________________________________________________________________
template<class C> void CFSD<C>::Deserialiser_4_doubles_i (unsigned int &pos, void *fptr)
{double a, b, c, d;
 int i;
 const char *txt = chaine_cour->Texte();
 f_deserialisation_int_4_doubles f4d_i = *((f_deserialisation_int_4_doubles*)fptr);
 while(txt[pos] != ';')
  {i = chaine_cour->Lire_entier(pos); pos++;
   a = chaine_cour->Lire_double(pos); pos++;
   b = chaine_cour->Lire_double(pos); pos++;
   c = chaine_cour->Lire_double(pos); pos++;
   d = chaine_cour->Lire_double(pos);
   if(txt[pos]=='_') pos++;
   (instanceC->*f4d_i)(i,a,b,c,d);
  }
}
//______________________________________________________________________________
template<class C> void CFSD<C>::Deserialiser_4_doubles(unsigned int &pos, void *fptr)
{double a, b, c, d;
 a = chaine_cour->Lire_double(pos); pos++;
 b = chaine_cour->Lire_double(pos); pos++;
 c = chaine_cour->Lire_double(pos); pos++;
 d = chaine_cour->Lire_double(pos);
 f_deserialisation_4_doubles f4 = *((f_deserialisation_4_doubles*)fptr);
 (instanceC->*f4)(a,b,c,d);}
//______________________________________________________________________________
template<class C> void CFSD<C>::Deserialiser_2_doubles(unsigned int &pos, void *fptr)
{double a, b;
 a = chaine_cour->Lire_double(pos); pos++;
 b = chaine_cour->Lire_double(pos);
 (instanceC->**((f_deserialisation_2_doubles*)fptr))(a,b);}
//______________________________________________________________________________
template<class C> void CFSD<C>::Deserialiser_1_double (unsigned int &pos, void *fptr)
{double a;
 a = chaine_cour->Lire_double(pos);
 (instanceC->**((f_deserialisation_1_double*)fptr))(a);}

//______________________________________________________________________________
template<class C> void CFSD<C>::Deserialiser_1_int    (unsigned int &pos, void *fptr)
{int a;
 a = chaine_cour->Lire_entier(pos);
 (instanceC->**((f_deserialisation_1_int*)fptr))(a);
}

//______________________________________________________________________________
template<class C> void CFSD<C>::Deserialiser_1_bool   (unsigned int &pos, void *fptr)
{bool a;
 a = chaine_cour->Lire_bool(pos);
 (instanceC->**((f_deserialisation_1_bool*)fptr))(a);}
//______________________________________________________________________________
template<class C> void CFSD<C>::Deserialiser_chaine_char   (unsigned int &pos, void *fptr)
{unsigned int taille;

 taille = chaine_cour->Lire_entier(pos); pos++;
 cc_tmp.Maj(chaine_cour->Texte(), pos, taille);
 pos += taille;
 (instanceC->**((f_deserialisation_chaine_char*)fptr))(cc_tmp);
}

//______________________________________________________________________________
// On suppose être placé sur le premier argument
// On désérialise ici les attributs qui ne sont accessibles que par des méthodes.
template<class C> void CFSD<C>::Deserialiser_type(const alx_chaine_char &txt, unsigned int &pos)
{unsigned int num_f, num_a;
 const char *texte = txt.Texte();
 f_deserialisation f;

 chaine_cour = &txt;

// On refixe les fonctions de rappel, pour assurer la désérialisation:
 f_ptr[7] = &CFSD<C>::Deserialiser_polygone;
 f_ptr[6] = &CFSD<C>::Deserialiser_1_int;
 f_ptr[5] = &CFSD<C>::Deserialiser_4_doubles_i;
 f_ptr[4] = &CFSD<C>::Deserialiser_chaine_char;
 f_ptr[3] = &CFSD<C>::Deserialiser_4_doubles;
 f_ptr[2] = &CFSD<C>::Deserialiser_2_doubles;
 f_ptr[1] = &CFSD<C>::Deserialiser_1_double;
 f_ptr[0] = &CFSD<C>::Deserialiser_1_bool;

 poly_reshaped = false;
// Puis on désérialise la partie spécifique à la fenêtre
 while( texte[pos] != '>' )
  {num_f = txt.Lire_entier(pos);
   pos++; // On passe la ,
   num_a = txt.Lire_entier(pos);
   pos++; // On passe le =
   f = f_ptr[num_f];
   (this->*f)(pos, f_deseri[num_a]);
   pos++; // On passe le ;
  }
}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
template<class C> const alx_chaine_char& CFSD<C>::Serialiser_difference_type(unsigned int &pos) const
{unsigned int num_f, num_a;
 f_seri_diff f;

// chaine_cour_diff_seri     = &serialisation_type;
// chaine_cour_ecriture_diff = &diff_type_tmp;
// chaine_cour_ecriture_seri = &seri_tmp;

// On refixe les fonctions de rappel, pour assurer la désérialisation:
 f_ptr_seri_diff[7] = &CFSD<C>::Serialiser_diff_polygone;
 f_ptr_seri_diff[6] = &CFSD<C>::Serialiser_diff_1_int;
 f_ptr_seri_diff[5] = &CFSD<C>::Serialiser_diff_4_doubles_i;
 f_ptr_seri_diff[4] = &CFSD<C>::Serialiser_diff_chaine_char;
 f_ptr_seri_diff[3] = &CFSD<C>::Serialiser_diff_4_doubles;
 f_ptr_seri_diff[2] = &CFSD<C>::Serialiser_diff_2_doubles;
 f_ptr_seri_diff[1] = &CFSD<C>::Serialiser_diff_1_double;
 f_ptr_seri_diff[0] = &CFSD<C>::Serialiser_diff_1_bool;

// Puis on désérialise la partie spécifique à la fenêtre
 const char *texte = chaine_cour_diff_seri->Texte();
 while( texte[pos] != '>' )
  {dep_ecriture = pos;
   num_f = chaine_cour_diff_seri->Lire_entier(pos);
   pos++; // On passe la ,
   num_a = chaine_cour_diff_seri->Lire_entier(pos);
   pos++; // On passe le =
   f = f_ptr_seri_diff[num_f];
   (this->*f)(pos, f_diff[num_a]);
   pos++; // On passe le ;
  }

 return *chaine_cour_ecriture_diff;
}


#endif


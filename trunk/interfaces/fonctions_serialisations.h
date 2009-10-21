#ifndef __ALX_FONCTIONS_SERIALISATIONS_H__
#define __ALX_FONCTIONS_SERIALISATIONS_H__

#include "alx_noeud_scene.h"
#include "..\utilitaires\alx_chaine_char.h"
#include "../GPC/gpc.h"
// fonctions_serialisations

template <class C> class CFSD // Classe de Fonctions de Sérialisation / Désérialisation
{private:
   mutable alx_chaine_char chaine_tmp, cc1, cc2, cc_tmp, cc_poly_tmp;
   mutable gpc_polygon poly_tmp;
   bool poly_reshaped;

  // Gestion des tableau de tableau de doubles
   typedef void (CFSD<C>::*f_deserialisation)(unsigned int &pos, void *fptr);

   typedef void (C::*f_deserialisation_chaine_char)   (const alx_chaine_char &t);
   typedef void (C::*f_deserialisation_polygone)      (gpc_polygon *poly);
   typedef void (C::*f_deserialisation_int_4_doubles) (const int n, const double r, const double v, const double b, const double a);
   typedef void (C::*f_deserialisation_4_doubles)     (const double r, const double v, const double b, const double a);
   typedef void (C::*f_deserialisation_2_doubles)     (const double x, const double y);
   typedef void (C::*f_deserialisation_1_double)      (const double x);
   typedef void (C::*f_deserialisation_1_int)         (const int b);
   typedef void (C::*f_deserialisation_1_bool)        (const bool b);

    typedef void (CFSD<C>::*f_seri_diff)(unsigned int &pos, void *fptr) const;
    typedef const bool             (C::*f_consultation_polygone)   (gpc_polygon **)     const;
    typedef const bool             (C::*f_consultation_chaine_char)(const alx_chaine_char **) const;
    typedef const double*          (C::*f_consultation_4_doubles_i)(const int i) const;
    typedef const double*          (C::*f_consultation_4_doubles)  ()            const;
    typedef const double           (C::*f_consultation_1_double)   ()            const;
    typedef const int              (C::*f_consultation_1_int)      ()            const;
    typedef const bool             (C::*f_consultation_1_bool)     ()            const;

    mutable f_deserialisation f_ptr[8];
    void Deserialiser_polygone    (unsigned int &pos, void *fptr);
    void Deserialiser_1_int       (unsigned int &pos, void *fptr);
    void Deserialiser_4_doubles_i (unsigned int &pos, void *fptr);
    void Deserialiser_4_doubles   (unsigned int &pos, void *fptr);
    void Deserialiser_2_doubles   (unsigned int &pos, void *fptr);
    void Deserialiser_1_double    (unsigned int &pos, void *fptr);
    void Deserialiser_1_bool      (unsigned int &pos, void *fptr);
    void Deserialiser_chaine_char (unsigned int &pos, void *fptr);

    mutable f_seri_diff f_ptr_seri_diff[8];
    void Serialiser_diff_polygone    (unsigned int &pos, void *fptr) const;
    void Serialiser_diff_1_int       (unsigned int &pos, void *fptr) const;
    void Serialiser_diff_4_doubles_i (unsigned int &pos, void *fptr) const;
    void Serialiser_diff_4_doubles   (unsigned int &pos, void *fptr) const;
    void Serialiser_diff_2_doubles   (unsigned int &pos, void *fptr) const;
    void Serialiser_diff_1_double    (unsigned int &pos, void *fptr) const;
    void Serialiser_diff_1_bool      (unsigned int &pos, void *fptr) const;
    void Serialiser_diff_chaine_char (unsigned int &pos, void *fptr) const;

    C *instanceC;
 public :
    mutable unsigned int dep_ecriture;
    const   alx_chaine_char *chaine_cour;
    mutable alx_chaine_char *chaine_cour_diff_seri;
    mutable alx_chaine_char *chaine_cour_ecriture_diff
                          , *chaine_cour_ecriture_seri;

   P_void a_ptr[50]; // pointeurs sur les attributs
   P_void f_deseri[50]
        , f_diff[50];

   f_deserialisation_chaine_char   fcc[50];
   f_deserialisation_polygone      fd_poly[50];
   f_deserialisation_int_4_doubles fd4_i[50];
   f_deserialisation_4_doubles     fd4[50];
   f_deserialisation_2_doubles     fd2[50];
   f_deserialisation_1_double      fd1[50];
   f_deserialisation_1_bool        fb1[50];
   f_deserialisation_1_int         fi1[50];

   f_consultation_chaine_char fsd_cc[50];
   f_consultation_polygone    fsd_poly[50];
   f_consultation_4_doubles_i fsd_d4_i[50];
   f_consultation_4_doubles   fsd_d4[50];
   f_consultation_1_double    fsd_d1[50];
   f_consultation_1_bool      fsd_b1[50];
   f_consultation_1_int       fsd_i1[50];

   CFSD( C *instanceC
       , alx_chaine_char *cc_consulte_seri
       , alx_chaine_char *cc_ecriture_nouv_seri
       , alx_chaine_char *cc_ecriture_diff) {this->instanceC = instanceC;
                                             chaine_cour_diff_seri     = cc_consulte_seri;
                                             chaine_cour_ecriture_seri = cc_ecriture_nouv_seri;
                                             chaine_cour_ecriture_diff = cc_ecriture_diff;
                                             poly_tmp.num_contours = 0;
                                               poly_tmp.contour = NULL;
                                               poly_tmp.hole    = NULL;
                                            }

  // Les méthodes
   void Deserialiser_type(const alx_chaine_char &txt, unsigned int &pos);
   const alx_chaine_char& Serialiser_difference_type(unsigned int &pos) const;

   gpc_polygon* get_poly_tmp() {return &poly_tmp;};
   const bool Recharger_polygone() const {return poly_reshaped;}
};

#endif


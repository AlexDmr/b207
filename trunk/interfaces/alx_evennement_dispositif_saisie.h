#ifndef __ALX_EVENNEMENT_DISPOSITIF_SAISIE_H__
#define __ALX_EVENNEMENT_DISPOSITIF_SAISIE_H__

//#include "../../physique/alx_classes_bases.h"
//#include "../../physique/alx_geometrie2D.h"  // point, vecteur, droite, segment...

#define alx_dispositif_saisie_apparition      1
#define alx_dispositif_saisie_disparition     2
#define alx_dispositif_saisie_appuie          4
#define alx_dispositif_saisie_relache         8

typedef int alx_dispositif_saisie_type_evennement;

class info_comp_evennement_dispositif_saisie
{
 public:
   //alx_evennement_info_point_et_vect coord, dep;
   int   caractere_saisie;
   int   type_appuie;
   bool  est_un_caractere;
   
   info_comp_evennement_dispositif_saisie() {}
   info_comp_evennement_dispositif_saisie &operator =(const info_comp_evennement_dispositif_saisie &c)    // Surcharge du  =
    {
     caractere_saisie = c.caractere_saisie;
     type_appuie      = c.type_appuie;
     est_un_caractere = c.est_un_caractere;
     return *this;
    }
};

class alx_evennement_dispositif_saisie
{
 private:
   int                                    identifiant;
   alx_dispositif_saisie_type_evennement  type_evt;
   info_comp_evennement_dispositif_saisie info_comp;

 public :
   alx_evennement_dispositif_saisie() : identifiant(-1) {}
   alx_evennement_dispositif_saisie( alx_evennement_dispositif_saisie *evt );
   alx_evennement_dispositif_saisie( int id
                                   , alx_dispositif_saisie_type_evennement  t_evt
                                   , info_comp_evennement_dispositif_saisie comp);


   inline void                                         Identifiant(const int i) {identifiant = i;}
   inline const int                                    Identifiant()const {return identifiant;}
   inline const alx_dispositif_saisie_type_evennement  Type_evt()   const {return type_evt;   }
   inline const info_comp_evennement_dispositif_saisie Info_comp()  const {return info_comp;  }

   void maj(const alx_evennement_dispositif_saisie &evt)
    {
     type_evt    = evt.type_evt;
     info_comp   = evt.info_comp;
     identifiant = evt.identifiant;
    }

   alx_evennement_dispositif_saisie &operator =(const alx_evennement_dispositif_saisie &evt)    // Surcharge du  =
    {
     maj(evt);

     return *this;
    }
};

#endif

 
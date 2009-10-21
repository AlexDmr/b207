#ifndef __ALX_METHODE_RAPPEL_H__
#define __ALX_METHODE_RAPPEL_H__

#include "..\utilitaires\alx_liste.h"

class alx_methode_rappel
{protected:
   void *params;

 public :
   virtual void Rappel(void *params)=0;
   virtual void Rappel()=0;
   void  Params(void *p) {params = p;}
   void* Params()        {return params;}
   void  Param (void *p) {params = p;}
   void* Param ()        {return params;}
};

template <class T> class alx_methode_rappel_de : public alx_methode_rappel
{protected:
   T *object;
   typedef void ( T::*fonction__de_rappel )(void *params);
   fonction__de_rappel fct;
   void *params;

 public:
   alx_methode_rappel_de(T* o, fonction__de_rappel f) : object (o), fct(f)
    {params = 0;}
   void Rappel()
    {(object->*fct)(Params());}
   void Rappel(void *param)
    {void *param_svg = Param();
     Param( param );
       (object->*fct)(param);
     Param( param_svg );
    }
// raccourci d'ecriture à la Ivy
#define FONCTION_DE_RAPPEL(cl, m) new alx_methode_rappel_de<cl>( this, m )
};

class Liste_de_rappel
{private:
   alx_liste<alx_methode_rappel*> L, L_tmp;
   bool bloquer_emission_rationnalisation_interne
      ,*bloquer_emission_rationnalisation_externe;
   void* param;

 public :
  // Les constructeurs
   Liste_de_rappel()        {bloquer_emission_rationnalisation_interne = false;
                             bloquer_emission_rationnalisation_externe = &bloquer_emission_rationnalisation_interne;}
   Liste_de_rappel(bool *b) {bloquer_emission_rationnalisation_externe = b;}
  ~Liste_de_rappel()
   {alx_element_liste<alx_methode_rappel*> *it     = L.Premier()
                                         , *it_fin = L.Fin();
    for(;it!=it_fin;it=it->svt)
     {delete it->E();
     }
   }

  // Les méthodes
   inline void Bloquer_emission_rationnalisation(const bool b)
    {bloquer_emission_rationnalisation_interne = b;
     bloquer_emission_rationnalisation_externe = &bloquer_emission_rationnalisation_interne;}
   inline const bool Bloquer_emission_rationnalisation() const {return *bloquer_emission_rationnalisation_externe;}
   inline void Bloquer_emission_rationnalisation(bool *b) {bloquer_emission_rationnalisation_externe = b;}
   inline void Ajouter_methode_a_la_fin(alx_methode_rappel *m) {L.Ajouter_a_la_fin(m);}
   inline void Ajouter_methode_au_debut(alx_methode_rappel *m) {L.Ajouter_au_debut(m);}

   inline const unsigned int Index(alx_methode_rappel *m) const {return L.Index(m);}
   inline const bool Vide() const {return L.Vide();}
   inline void Vider() {L.Vider();}

   inline bool Retirer_methode(alx_methode_rappel *m)
    {return L.Retirer(m);}

   void Rappeler(void *param)
    {if( Bloquer_emission_rationnalisation() ) return;
     L_tmp = L;
     alx_element_liste<alx_methode_rappel*> *it     = L_tmp.Premier()
                                          , *it_fin = L_tmp.Fin()
                                          , *it_svt;
     Param(param);
     while(it!=it_fin)
      {it_svt = it->svt;
       (it->E())->Rappel(param);
       it = it_svt;
      }

    }

   void  Param(void *p) {param = p;}
   void* Param()        {return param;}
   void Rappeler()
    {if( Bloquer_emission_rationnalisation() ) return;
     L_tmp = L;
     alx_element_liste<alx_methode_rappel*> *it     = L_tmp.Premier()
                                          , *it_fin = L_tmp.Fin()
                                          , *it_svt;
      if(Param())
       {while(it!=it_fin)
          {it_svt = it->svt;
           (it->E())->Rappel(Param());
           it = it_svt;
          }
       } else {while(it!=it_fin)
                {it_svt = it->svt;
                 (it->E())->Rappel();
                 it = it_svt;
                }
              }
    }
   alx_liste<alx_methode_rappel*>* Liste() {return &L;}

   const Liste_de_rappel &operator =(const Liste_de_rappel &Liste_R)
    {L = Liste_R.L;
     bloquer_emission_rationnalisation_interne = Liste_R.bloquer_emission_rationnalisation_interne;
     if( Liste_R.bloquer_emission_rationnalisation_externe == &(Liste_R.bloquer_emission_rationnalisation_interne) )
       bloquer_emission_rationnalisation_externe = &bloquer_emission_rationnalisation_interne;
      else bloquer_emission_rationnalisation_externe = Liste_R.bloquer_emission_rationnalisation_externe;
     param = Liste_R.param;
     return *this;}
};

#endif

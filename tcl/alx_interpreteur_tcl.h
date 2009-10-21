#ifndef __ALX_INTERPRETEUR_TCL_H__
#define __ALX_INTERPRETEUR_TCL_H__

#include <tcl.h>
#include <tk.h>
#include "..\utilitaires\alx_chaine_char.h"

class resultat_interpreteur_tcl
{public:
   const char *reponse_texte;
   int  reponse_int;
   resultat_interpreteur_tcl(const char *t, const int r)
    {reponse_texte = t; reponse_int = r;}
};

class alx_interpreteur_tcl
{protected:
   Tcl_Interp  *interpreteur;
   alx_chaine_char *texte_edition;
   resultat_interpreteur_tcl dernier_resultat;

 public :
   alx_interpreteur_tcl(const char *nom_exec);

  resultat_interpreteur_tcl Evaluer(const char *commande);
  resultat_interpreteur_tcl Evaluer(const alx_chaine_char &commande);

  inline Tcl_Interp* Interpreteur_tcl() {return interpreteur;}
  inline void               Texte_edition(alx_chaine_char *t) {texte_edition = t;}
  inline const alx_chaine_char* Texte_edition()         const {return texte_edition;}
  inline void Ecrire(const alx_chaine_char &t) {*texte_edition += t;}
  inline void Ecrire(const char* t)            {*texte_edition += t;}
  inline void Effacer_texte()                  {*texte_edition = "";}

  inline resultat_interpreteur_tcl& Dernier_resultat() {return dernier_resultat;}
};

class alx_methode_rappel;

class Appel_TCL
{private:
   alx_interpreteur_tcl *interp;
   alx_chaine_char texte;
   alx_methode_rappel *m_rappel;
   void init();
   void F_rappel(void *param);
   void *param;
   Tcl_Obj *tcl_obj;

 public :
   Appel_TCL(alx_interpreteur_tcl *i);
   Appel_TCL(alx_interpreteur_tcl *i, const char *txt);

   inline void  Param(void *p) {param = p;}
   inline void* Param()        {return param;}
   void  Texte(const char *t);
   inline const char* Texte() const        {return texte.Texte();}
   inline       void             Texte_cc(const alx_chaine_char &t) {texte = t;}
   inline const alx_chaine_char& Texte_cc() const                   {return texte;}
   inline alx_methode_rappel* Rappel() {return m_rappel;}
};

#endif

#include "alx_classe_base_modele.h"
#include "alx_classe_base_liant.h"

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
void alx_classe_base_model::Oyez_oyez()
 {
  if( Bloquer_emission_rationnalisation() ) return;
  alx_element_liste<lien_interet> *it, *it_fin = L_interessees.Fin();

  for(it=L_interessees.Premier(); it!=it_fin; it=it->svt)
   {pivot = (it->E()).interessee;
    if(pivot->Bloquer_rationnalisation()) continue;
    pivot->rationnaliser( (it->E()).num );
   }
 }

//______________________________________________________________________________
void alx_classe_base_model::Colporter(alx_classe_base_liant *demandeur)
 {
  if( Bloquer_emission_rationnalisation() ) return;
  alx_element_liste<lien_interet> *it, *it_fin = L_interessees.Fin();

  for(it=L_interessees.Premier(); it!=it_fin; it=it->svt)
   {pivot = (it->E()).interessee;
    if(pivot->Bloquer_rationnalisation()) continue;
    if( (it->E()).interessee != demandeur )
      pivot->rationnaliser( (it->E()).num );
   }
 }

//______________________________________________________________________________
//______________________________________________________________________________
bool alx_classe_base_model::est_deja_abonner( lien_interet &l )
{alx_element_liste<lien_interet> *it     = L_interessees.Premier()
                               , *it_fin = L_interessees.Fin();
 for(; it!=it_fin; it=it->svt)
  {if( it->E().interessee == l.interessee
     &&it->E().num        == l.num )
    {return true;}
  }
 return false;
}

//______________________________________________________________________________
void alx_classe_base_model::abonner_a( lien_interet &l )
{if(est_deja_abonner(l) )
  return;
 L_interessees.Ajouter_au_debut(l);
}

//______________________________________________________________________________
void alx_classe_base_model::abonner_en_dernier_a( lien_interet &l )
{if(est_deja_abonner(l) )
  return;
 L_interessees.Ajouter_a_la_fin(l);
}


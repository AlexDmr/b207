#include "alx_simulateur_dispositif_saisie.h"

alx_simulateur_dispositif_saisie::alx_simulateur_dispositif_saisie()
{}

//------------------------------------------------------------------------------
int alx_simulateur_dispositif_saisie::Ajouter_dispositif_saisie()       // renvoi le numéro l'identifiant.
{alx_element_liste<alx_dispositif_saisie> *it     = L_dispositifs_saisies.Premier()
                                        , *it_fin = L_dispositifs_saisies.Fin();
 int der_id = 0;

 while( ( it != it_fin)
      &&(it->E()).id_valant(++der_id ) ) it=it->svt;

 if(it==it_fin)
   V_destinataires_precis.Ajouter_avant(lien_interet(NULL, 0), V_destinataires_precis.Fin()); // NULL car personne ne prends pour le moment les évennements

 L_dispositifs_saisies.Ajouter_avant(alx_dispositif_saisie(der_id), it);

// Poster un évennement relatif à l'apparition du pointeur.
 L_evt.Ajouter_a_la_fin( alx_evennement_dispositif_saisie( der_id
                                                         , alx_dispositif_saisie_apparition
                                                         , info_comp_evennement_dispositif_saisie()) );
 return der_id;
}

//------------------------------------------------------------------------------
// Codes de renvois:
// 0 : Trouvé et supprimé
//-1 : pas trouvé.
int alx_simulateur_dispositif_saisie::Retirer_dispositif_saisie(int id) // retire le pointeur id, renvoi un code d'erreur.
{alx_element_liste<alx_dispositif_saisie> *it     = L_dispositifs_saisies.Premier()
                                        , *it_fin = L_dispositifs_saisies.Fin();
 while( ( it != it_fin)
      &&(!(it->E()).id_valant(id) ) ) it=it->svt;

 if(it == it_fin) return -1; // Pas trouvé.
 L_dispositifs_saisies.Retirer(it);
 V_destinataires_precis.E(id).interessee = NULL; // Celui qui était intéressé ne l'est plus...

// Poster un évennement relatif à la disparition du pointeur.
 L_evt.Ajouter_a_la_fin( alx_evennement_dispositif_saisie( id
                                                         , alx_dispositif_saisie_disparition
                                                         , info_comp_evennement_dispositif_saisie()) );
 return 0; // tout va bien
}

//------------------------------------------------------------------------------
alx_dispositif_saisie* alx_simulateur_dispositif_saisie::Dispositif_saisie(int id)
{alx_element_liste<alx_dispositif_saisie> *it_ptr     = L_dispositifs_saisies.Premier()
                                        , *it_fin_ptr = L_dispositifs_saisies.Fin();

 for(; (it_ptr!=it_fin_ptr)&&(!(it_ptr->E()).id_valant(id)); it_ptr=it_ptr->svt);

 if(it_ptr==it_fin_ptr) return NULL;
 return &(it_ptr->E());
}

//------------------------------------------------------------------------------
alx_dispositif_saisie* alx_simulateur_dispositif_saisie::Dispositif_saisie(int *ds)
{alx_element_liste<alx_dispositif_saisie> *it_ptr     = L_dispositifs_saisies.Premier()
                                        , *it_fin_ptr = L_dispositifs_saisies.Fin();

 for(; (it_ptr!=it_fin_ptr)&&(!(it_ptr->E()).Etat(ds)); it_ptr=it_ptr->svt);

 if(it_ptr==it_fin_ptr) return NULL;
 return &(it_ptr->E());
}

//------------------------------------------------------------------------------
void alx_simulateur_dispositif_saisie::maj_dispositifs_saisies(alx_evennement_dispositif_saisie &e)
{}

//------------------------------------------------------------------------------
void alx_simulateur_dispositif_saisie::simuler()
{// On envoi les évennements aux intéressés jusqu'à ce qu'il n'y en ait plus.
 // Si personne ne traite un évennement, celui-ci est retiré de la liste.
 alx_element_liste<alx_evennement_dispositif_saisie> *L_evt_it;
 alx_element_liste<lien_interet>                     *it, *it_fin = L_interessees.Fin();
 alx_evennement_dispositif_saisie *eds;
 lien_interet *lien;
 int /*i,*/ taille = L_rappels.Taille();

 alx_element_liste<Liste_de_rappel> *it_rappel;

 while(!L_evt.Vide())
  {
   L_evt_it = L_evt.Premier();
  // Prendre en compte le message en interne.
   eds = &(L_evt_it->E());
   maj_dispositifs_saisies(*eds);

  // Faire suivre aux intéressés.
   L_rappel_globale.Rappeler(eds);

   it_rappel = L_rappels.Premier();
   for(int i=0;i<taille;i++)
    {(it_rappel->E()).Rappeler(eds);
     it_rappel=it_rappel->svt;}
   //ex for(i=0;i<taille;i++) L_rappels[i].Rappeler(eds);

   for(it=L_interessees.Premier(); (it!=it_fin)&&(!L_evt.Vide())&&(L_evt_it==L_evt.Premier()); it=it->svt)
     (it->E()).interessee->rationnaliser( (it->E()).num );

  // Faire suivre au liant qui est intéressé par le message de CE dispositif
   lien = &(V_destinataires_precis.E(eds->Identifiant()));
   if(lien->interessee) lien->interessee->rationnaliser(lien->num);

  // Supprimer le message s'il y a lieu
   if( (!L_evt.Vide())&&(L_evt_it==L_evt.Premier()) )
     L_evt.Retirer_premier(); // Personne n'à supprimer le message, on le fait.
  }

}

//------------------------------------------------------------------------------
void alx_simulateur_dispositif_saisie::Prendre_evennements_lies_a( alx_point2D *pt
                                                                 , alx_classe_base_liant *liant
                                                                 , int num) // Numéro d'abonnement!
{// Déterminer à quel pointeur ptr appartient le point pt
 // Déterminer quel dispositif de saisie est associé à ce pointeur
 //

 // Pour le moment on se contente d'un seul dispositifs d'entrée par pointage
 // Celui qui prend les évennements les prends tous et est le seul à les prendres
 alx_element_liste<lien_interet> *it     = V_destinataires_precis.Premier()
                               , *it_fin = V_destinataires_precis.Fin();
 for(;it!=it_fin;it=it->svt)
  {(it->E()).interessee = liant;
   (it->E()).num        = num;}

// int t = V_destinataires_precis.Taille();
// for(int i=0;i<t;i++)
//  {V_destinataires_precis[i].interessee = liant;
//   V_destinataires_precis[i].num        = num;
//  }
}


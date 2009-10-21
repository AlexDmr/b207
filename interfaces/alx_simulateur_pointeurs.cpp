#include "alx_simulateur_pointeurs.h"
#include "alx_noeud_scene.h"
#include <stdio.h>

alx_simulateur_pointeurs::alx_simulateur_pointeurs()
{nb_T_free_id = 32; T_free_id = new bool [32]; for(int i=31; i>=0; i--) {T_free_id[i] = true;}
}

//______________________________________________________________________________
const bool alx_pointeur::operator!=(const alx_pointeur &p) const
{return p.Id() != Id();}

//______________________________________________________________________________
const bool alx_pointeur::operator==(const alx_pointeur &p) const
{return p.Id() == Id();}

//______________________________________________________________________________
void alx_pointeur::Noeud_representation(alx_noeud_scene *n)
{pere->Retirer_fils( representation );
 representation = n;
 pere->Ajouter_fils_au_debut( n );
}

//______________________________________________________________________________
const unsigned int alx_simulateur_pointeurs::get_a_point_id()
{for(unsigned int i=0; i<nb_T_free_id; i++)
  {if(T_free_id[i]) {
     T_free_id[i] = false;
     return i;
    }
  }
// Not enough id...realloc
 bool *tmp = new bool[2*nb_T_free_id];
 for(unsigned int i=0; i<=nb_T_free_id; i++)               {tmp[i] = false;}
 for(unsigned int i=nb_T_free_id+1; i<2*nb_T_free_id; i++) {tmp[i] = true;}
// Update attributs
 delete[] T_free_id; T_free_id = tmp;
 unsigned int rep = nb_T_free_id;
 nb_T_free_id *= 2;

 return rep;
}

//______________________________________________________________________________
void alx_simulateur_pointeurs::release_a_point_id(const unsigned int id)
{T_free_id[id] = true;
}

//______________________________________________________________________________
int alx_simulateur_pointeurs::Ajouter_pointeur(const char *n, alx_noeud_scene *p, const unsigned int nb)       // renvoi le numéro l'identifiant.
{unsigned int /*der_id=-1, */rep = get_a_point_id();
 release_a_point_id(rep);
 for(unsigned int i=0; i<nb; i++)
  {alx_pointeur ptr(p, /*der_id=*/get_a_point_id());
   ptr.Nom(n);
   L_pointeurs.Ajouter_au_debut(ptr);
  // Poster un évennement relatif à l'apparition du pointeur.
   evt_tmp.maj( ptr.Id()
              , alx_pointeur_apparition
              , info_comp_evennement()
              , NULL);

   L_evt.Ajouter_a_la_fin( evt_tmp );
  }

 return rep;
}

//______________________________________________________________________________
// Codes de renvois:
// 0 : Trouvé et supprimé
//-1 : pas trouvé.
//______________________________________________________________________________
int alx_simulateur_pointeurs::Retirer_pointeur(int id) // retire le pointeur id, renvoi un code d'erreur.
{alx_element_liste<alx_pointeur> *it     = L_pointeurs.Premier()
                               , *it_fin = L_pointeurs.Fin();

 while( ( it != it_fin)
      &&(!(it->E()).id_valant(id) ) ) it=it->svt;

 if(it == it_fin) return -1; // Pas trouvé.
 if(L_pointeurs.Vide())
   printf("alx_simulateur_pointeurs::Retirer_pointeur DEBUG");

 alx_noeud_scene *pere_ptr, *preso_ptr;
 alx_pointeur *ptr = &(it->E());
 ptr->Add_MetaData("alx_simulateur_pointeurs::Retirer_pointeur", "1");
 pere_ptr  = ptr->Noeud_pere();
 preso_ptr = ptr->Noeud_representation();
 if(pere_ptr && preso_ptr) {/*pere_ptr->Retirer_fils( preso_ptr );*/}

 //L_pointeurs.Retirer(it);
 //return 0;

// Poster un évennement relatif à la disparition du pointeur.
 info_comp_evennement info_tmp;
   info_tmp.Coord( ptr->Point() );
 evt_tmp.maj( id
            , alx_pointeur_disparition
            , info_tmp
            , pere_ptr);
//alx_evennement_pointeur *evt = new alx_evennement_pointeur( id
//                                                          , alx_pointeur_disparition
//                                                          , info_comp_evennement()
//                                                          , NULL);
 L_evt.Ajouter_a_la_fin( evt_tmp );
 return 0; // tout va bien
}

//______________________________________________________________________________
alx_pointeur* alx_simulateur_pointeurs::maj_pointeurs(alx_evennement_pointeur &e)
{int id_ptr = e.Identifiant();

// repérer le pointeur dont il est question dans la liste:
 alx_element_liste<alx_pointeur> *it_ptr     = L_pointeurs.Premier()
                               , *it_fin_ptr = L_pointeurs.Fin();

 for(; (it_ptr!=it_fin_ptr)&&(!(it_ptr->E()).id_valant(id_ptr)); it_ptr=it_ptr->svt);

 if(it_ptr==it_fin_ptr) return (alx_pointeur*)NULL;

// Traiter le changement à l'origine de l'évennement.
 switch(e.Type_evt())
  {case alx_pointeur_enfonce:
     (it_ptr->E()).appuie |= e.Info_comp().type_appuie;
   break;
   case alx_pointeur_relache:
     (it_ptr->E()).appuie &= ~(e.Info_comp().type_appuie);
   break;
   case alx_pointeur_mouvement:
     alx_noeud_scene *n = it_ptr->E().Noeud_representation();
     (it_ptr->E()).pt.maj( e.Info_comp().coord );
     if (n) n->Origine( (it_ptr->E()).pt );
   break;
  }

 return &(it_ptr->E());
}

//______________________________________________________________________________
void alx_simulateur_pointeurs::Simuler(alx_evennement_pointeur &evt)
{alx_element_liste<alx_evennement_pointeur> *L_evt_it;
 L_evt.Ajouter_au_debut( &evt );
 L_evt_it = L_evt.Premier();
 alx_element_liste<lien_interet> *it, *it_fin = L_interessees.Fin();

 for(it=L_interessees.Premier(); (it!=it_fin)&&(!L_evt.Vide())&&(L_evt_it==L_evt.Premier()); it=it->svt)
   (it->E()).interessee->rationnaliser( (it->E()).num );
 if(L_evt_it==L_evt.Premier())
  {if(L_evt.Vide())
     printf("DEBUG");
   L_evt.Retirer_premier();}
}

//______________________________________________________________________________
const unsigned int alx_simulateur_pointeurs::Simuler()
{nb_info_ptr = 0;
 alx_pointeur *ptr;
 unsigned int i;
 // On envoi les évennements aux intéressés jusqu'à ce qu'il n'y en ait plus.
 // Si personne ne traite un évennement, celui-ci est retiré de la liste.
 alx_element_liste<alx_evennement_pointeur> *L_evt_it;
 alx_element_liste<lien_interet> *it, *it_fin = L_interessees.Fin();

 //serialisation.Maj("(",0,1);
 bool to_be_delete;
 while(!L_evt.Vide())
  {L_evt_it = L_evt.Premier();
  // Prendre en compte le message en interne.
   ptr = maj_pointeurs( L_evt_it->E() );

  // XXX Cas d'un retrait de pointeur
   if (L_evt_it->E().Type_evt() == ALX_pointeur_disparition())
    {to_be_delete = true;} else {to_be_delete = false;}
  // Prévenir ceux qui se sont abonné à ce pointeur en particulier
   if( !ptr->Est_une_replique() )
    {i = 0;
     while( (i<nb_info_ptr)
          &&(TP_ptr[i] != ptr) ) i++;
     if(i == nb_info_ptr)
      {TP_ptr[nb_info_ptr] = ptr;
       nb_info_ptr++;}
    }
   ptr->Rappel_changement();

  // Faire suivre aux intéressés.
   L_evt_it->E().ptr = ptr;
   for(it=L_interessees.Premier(); (it!=it_fin)&&(!L_evt.Vide())&&(L_evt_it==L_evt.Premier()); it=it->svt)
     (it->E()).interessee->rationnaliser( (it->E()).num );

   if( (!L_evt.Vide())&&(L_evt_it==L_evt.Premier()) )
     L_evt.Retirer_premier(); // Personne n'à traité le message, on le retire.

  // Doit-on maintenant retirer le pointeur de la liste?
   if(to_be_delete) {
     if(ptr->Noeud_representation()) {ptr->Noeud_representation()->Vider_peres();        }
     if( !ptr->Est_une_replique()  ) {L_pointeurs_detruits.Ajouter_a_la_fin( Pipo_pointeur_detruit(ptr->Id(), ptr->X(), ptr->Y()) );}
       ptr->Noeud_pere( NULL );
       release_a_point_id( ptr->Id() );
       L_pointeurs.Retirer( *ptr );
    }
  }
 //serialisation.Ajouter(")",0,1);

 return nb_info_ptr;
}


//______________________________________________________________________________
alx_pointeur* alx_simulateur_pointeurs::Pointeur(int id)
{alx_element_liste<alx_pointeur> *it_ptr     = L_pointeurs.Premier()
                               , *it_fin_ptr = L_pointeurs.Fin();

 for(; (it_ptr!=it_fin_ptr)&&(!(it_ptr->E()).id_valant(id)); it_ptr=it_ptr->svt);

 if(it_ptr==it_fin_ptr) return (alx_pointeur*)NULL;
 return &(it_ptr->E());
}

//______________________________________________________________________________
alx_pointeur* alx_simulateur_pointeurs::Pointeur(alx_point2D *pt)
{alx_element_liste<alx_pointeur> *it_ptr     = L_pointeurs.Premier()
                               , *it_fin_ptr = L_pointeurs.Fin();

 for(; (it_ptr!=it_fin_ptr)&&(!(it_ptr->E()).adresse_pt_en(pt)); it_ptr=it_ptr->svt);

 if(it_ptr==it_fin_ptr) return (alx_pointeur*)NULL;
 return &(it_ptr->E());
}

//______________________________________________________________________________
alx_pointeur* alx_simulateur_pointeurs::Pointeur(const alx_chaine_char &n)
{alx_element_liste<alx_pointeur> *it_ptr     = L_pointeurs.Premier()
                               , *it_fin_ptr = L_pointeurs.Fin();

 for(; (it_ptr!=it_fin_ptr)&&((it_ptr->E()).Nom() != n); it_ptr=it_ptr->svt);

 if(it_ptr==it_fin_ptr) return (alx_pointeur*)NULL;
 return &(it_ptr->E());
}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
const alx_chaine_char& alx_simulateur_pointeurs::Serialiser(const alx_liste<alx_repere2D*> *L_rep) const
{if(L_pointeurs.Vide())
  {serialisation.Maj("",0,0);
   return serialisation;}
 alx_element_liste<alx_pointeur> *it_ptr     = L_pointeurs.Premier()
                               , *it_fin_ptr = L_pointeurs.Fin();
 serialisation.Maj("(",0,1);
 for(; (it_ptr!=it_fin_ptr); it_ptr=it_ptr->svt)
  {if( !it_ptr->E().Est_une_replique() )
    {serialisation += it_ptr->E().Serialiser( L_rep );
     serialisation.Ajouter(",",0,1);}
  }
// Lister les pointeurs détruits
 alx_element_liste<Pipo_pointeur_detruit> *it_id     = L_pointeurs_detruits.Premier()
                                        , *it_fin_id = L_pointeurs_detruits.Fin();
 for(; it_id!=it_fin_id; it_id=it_id->svt)
  {const Pipo_pointeur_detruit &pipo_ptr = it_id->E();
   serialisation += pipo_ptr.id; serialisation.Ajouter("_", 0, 1);
   serialisation += pipo_ptr.x ; serialisation.Ajouter("_", 0, 1);
   serialisation += pipo_ptr.y ; serialisation.Ajouter("_-1,", 0, 4);
  }
 L_pointeurs_detruits.Vider();

 serialisation.Ajouter(")",0,1);
 return serialisation;
}

//______________________________________________________________________________
const alx_chaine_char& alx_simulateur_pointeurs::Serialiser_diff(const alx_liste<alx_repere2D*> *L_rep) const
{return Serialiser(L_rep);
/*if(nb_info_ptr == 0)
  {serialisation.Maj("",0,0);
   return serialisation;}
 serialisation.Maj("(",0,1);
 for(unsigned int i=0;i<nb_info_ptr;i++)
  {serialisation += TP_ptr[i]->Serialiser( L_rep );
   serialisation.Ajouter(",",0,1);}
 serialisation.Ajouter(")",0,1);
 return serialisation;
*/
}




#include "alx_horloge_vectorielle.h"
#include <stdlib.h>
//#include <mem.h>
#include "..\physique\math_alex.cpp"

//______________________________________________________________________________
//____________________________ Initialisation __________________________________
//______________________________________________________________________________
void alx_horloge_vectorielle::init()
{ordre   = 0;
 proprio =-1; // Inconnu!
 H     = (unsigned int *)NULL;
 TabIP = (alx_chaine_char *)NULL;
 Tab_connaissance = (alx_liste<alx_chaine_char>*)NULL;}

//______________________________________________________________________________
//________________________________Constructeurs_________________________________
//______________________________________________________________________________
alx_horloge_vectorielle::alx_horloge_vectorielle()
{init();}

//______________________________________________________________________________
alx_horloge_vectorielle::alx_horloge_vectorielle(const unsigned int n)
{init();
 ordre = n;
 H     = new unsigned int[ordre];
 TabIP = new alx_chaine_char[ordre];
 for(unsigned int i=0; i<n; i++)
  {H[i] = 0;}
}

//______________________________________________________________________________
alx_horloge_vectorielle::alx_horloge_vectorielle(const unsigned int n, const int p)
{init();
 ordre = n;
 H = new unsigned int[ordre];
 TabIP = new alx_chaine_char[ordre];
 for(unsigned int i=0; i<n; i++)
   H[i] = 0;
 proprio = p;}

//______________________________________________________________________________
alx_horloge_vectorielle::~alx_horloge_vectorielle()
{if(H)
   delete [] H;
 if(TabIP)
   delete [] TabIP;
}

//______________________________________________________________________________
//______________________________________ Méthodes ______________________________
//______________________________________________________________________________
void alx_horloge_vectorielle::Ordre(const unsigned int o)
{if(Ordre() == o) return;
 unsigned int    *nH = new unsigned int[o];
 alx_chaine_char *nT = new alx_chaine_char[o];
 alx_liste<alx_chaine_char> *nTC = new alx_liste<alx_chaine_char>[o];

 if(H)                  // Les valeurs dans l'horloge
  {memcpy(nH, H    , Amin(o, ordre)*sizeof(unsigned int));
   delete [] H;}
 H = nH;
 nH[o-1]=0;
 if(TabIP)              // Les IPs associées aux valeurs
  {for(unsigned int i=0; i<Amin(o, ordre); i++)
     nT[i] = TabIP[i];
   delete [] TabIP;}
 TabIP = nT;

 if(Tab_connaissance)   // La connaissance sur la structures des horloges associées aux IPs
  {for(unsigned int i=0; i<Amin(o, ordre); i++)
     nTC[i] = Tab_connaissance[i];
   delete [] Tab_connaissance;}
 Tab_connaissance = nTC;

 // Mise à jour de l'ordre
 ordre = o;}

//______________________________________________________________________________
void alx_horloge_vectorielle::Maj(const alx_horloge_vectorielle &h)
{Ordre( h.Ordre() );
 for(unsigned int i=0; i<ordre; i++)
  {H[i]     = h.Horloge()[i];
   TabIP[i] = h.IP(i);
   Tab_connaissance[i] = h.Connaissance(i);}
 Proprietaire( h.Proprietaire() );
}

/*void alx_horloge_vectorielle::Prendre_en_compte_horloge_differente(const alx_horloge_vectorielle &h)
{// Ne comparer que les valeurs identiques
 // On suppose qu'il y a des insertion entre les IP mais pas de permutations
 unsigned int j = 0;
 const unsigned int *H2 = h.Horloge();
 for(unsigned int i=0; i<Ordre(); i++)
  {while( (j < h.Ordre())
        &&(TabIP[i] != h.IP(j)) ) j++;
   if(j == h.Ordre()) break;
   H[i] = Amax(H[i], H2[j]);
  }
}*/
//______________________________________________________________________________
void alx_horloge_vectorielle::Reduire_a(const alx_horloge_vectorielle &h)
{unsigned int i = 0
            , j
            , dep = 0;
 while(i<Ordre())
  {j=dep;
   while( (j < h.Ordre())
        &&(TabIP[i] != h.IP(j)) ) j++;
   if(j == h.Ordre()) // Pas trouvé, on supprime et on reste donc sur place
     Supprimer(i);
    else {i++; dep = j+1;}// On garde, on marque et on avance.
  }
}

//______________________________________________________________________________
void alx_horloge_vectorielle::Prendre_en_compte(const alx_chaine_char &h_seri)
{H[Proprietaire()]++;
 unsigned int pos = 1; // la (
   unsigned int ordre_seri = h_seri.Lire_entier(pos);
   if(Ordre() != ordre_seri)
     return;
   pos++; // le ;
   unsigned int proprio_seri = h_seri.Lire_entier(pos)
              , val_h_tmp;
   for(unsigned int i=0; i<Ordre(); i++)
    {val_h_tmp = h_seri.Lire_entier(pos);
     if(i == proprio_seri) {H[proprio_seri] = val_h_tmp;
                            break;}
     pos++; // le ;
    }
}

//______________________________________________________________________________
void alx_horloge_vectorielle::Prendre_en_compte(const alx_horloge_vectorielle &h)
{// On prend prend la valeur du proprio de h.
 const unsigned int *H2 = h.Horloge();
 H[Proprietaire()]++;
 H[h.Proprietaire()] = H2[h.Proprietaire()];
}

//______________________________________________________________________________
const unsigned int alx_horloge_vectorielle::Incrementer()
{return ++H[proprio];}

//______________________________________________________________________________
const unsigned int alx_horloge_vectorielle::Inserer(const unsigned int v, const alx_chaine_char &ip)
{// On cherche à faire l'insertion selon l'ordre alphabétique sur les adresse IP.
 unsigned int pos = 0;
 while( (pos < Ordre())
      &&(TabIP[pos] < ip) ) pos++;
 if(pos == Ordre()) {return Inserer(v, pos, ip);}
 if(TabIP[pos] == ip) return pos; // On l'a déja insérer!
 return Inserer(v, pos, ip); // insertion effective!
}

//______________________________________________________________________________
const unsigned int alx_horloge_vectorielle::Inserer(const unsigned int v, const unsigned int p, const alx_chaine_char &ip)
{// Insertion dans les compteurs
 unsigned int *nH = new unsigned int[Ordre()+1];
 if(H)
  {memcpy(nH, H, p*sizeof(unsigned int)); nH[p] = v;
   memcpy(&(nH[p+1]), &(H[p]), (Ordre()-p)*sizeof(unsigned int));
   delete [] H;}

 // Insertion dans les IP.
 alx_chaine_char *nT = new alx_chaine_char[Ordre()+1];
 if(TabIP)
  {for(unsigned int i=0; i<p; i++)
     nT[i] = TabIP[i];
   nT[p] = ip;
   for(unsigned int i=p; i<Ordre(); i++)
     nT[i+1] = TabIP[i];
   delete [] TabIP;}

 // Insertion dans les connaissances
 alx_liste<alx_chaine_char> *nTC = new alx_liste<alx_chaine_char>[Ordre()+1];
 if(Tab_connaissance)
  {for(unsigned int i=0; i<p; i++)
     nTC[i] = Tab_connaissance[i];
   for(unsigned int i=p; i<Ordre(); i++)
     nTC[i+1] = Tab_connaissance[i];
   delete [] Tab_connaissance;
  }

 // Mise à jour des attributs
 ordre++;
 H = nH; TabIP = nT; Tab_connaissance = nTC;
 if(p <= Proprietaire())
   Proprietaire( Proprietaire()+1 );

 return p;
}

//______________________________________________________________________________
const int alx_horloge_vectorielle::Position_dans_horloge(const alx_chaine_char &ip) const
{int pos=0;
 while( (pos < (int)Ordre())
      &&(TabIP[pos] != ip) ) pos++;
 if(pos >= (int)Ordre()) pos=-1;
 return pos;
}

//______________________________________________________________________________
void alx_horloge_vectorielle::Supprimer(const alx_chaine_char &ip)
{unsigned int pos=0;
 while( (pos < Ordre())
      &&(TabIP[pos] != ip) ) pos++;
 Supprimer( pos );
}

//______________________________________________________________________________
void alx_horloge_vectorielle::Supprimer(const unsigned int p)
{if( p >= Ordre() ) return;
 // Suppression dans les compteurs
 unsigned int *nH = new unsigned int[Ordre()-1];
 if(H)
  {memcpy(nH, H, p*sizeof(unsigned int));
   memcpy(&(nH[p]), &(H[p+1]), (Ordre()-p-1)*sizeof(unsigned int));
   delete [] H;}

 // Suppression dans les IP
 alx_chaine_char *nT = new alx_chaine_char[Ordre()-1];
 if(TabIP)
  {for(unsigned int i=0; i<p; i++)
     nT[i] = TabIP[i];
   for(unsigned int i=p+1; i<Ordre(); i++)
     nT[i-1] = TabIP[i];
   delete [] TabIP;}

 // Insertion dans les connaissances
 alx_liste<alx_chaine_char> *nTC = new alx_liste<alx_chaine_char>[Ordre()+1];
 if(Tab_connaissance)
  {for(unsigned int i=0; i<p; i++)
     nTC[i] = Tab_connaissance[i];
   for(unsigned int i=p+1; i<Ordre(); i++)
     nTC[i-1] = Tab_connaissance[i];
   delete [] Tab_connaissance;
  }

 // Mise à jour des attributs
 ordre--;
 H = nH; TabIP = nT; Tab_connaissance = nTC;
}

//______________________________________________________________________________
//______________ Gestion de la connaissance des autres horloges_________________
//______________________________________________________________________________
const bool alx_horloge_vectorielle::Connaissance(const alx_chaine_char &ip, const char *L_IP)
{// Cherchons l'IP, sa position dans notre liste
 bool insertion_faite = false;
 unsigned int pos_ip = 0;
 while( (pos_ip < Ordre())
      &&(TabIP[pos_ip] != ip) ) pos_ip++;
 if(pos_ip == Ordre()) // on ne l'a pas trouvé.... IL FAUT l'AJOUTER!!!
  {pos_ip = Inserer(0, ip);
   insertion_faite = true;}

 // On a trouvé l'IP.
 Connaissance(pos_ip, L_IP);
 return insertion_faite;
}

//______________________________________________________________________________
void alx_horloge_vectorielle::Connaissance(const unsigned int pos, const char *L_IP)
{// On parcours L_IP pour insérer les IP dans la liste de connaissance correspondant à pos
 unsigned int i=1, pos_dep;
 Tab_connaissance[pos].Vider();

 while((L_IP[i] != 0)&&(L_IP[i] != ')'))
  {pos_dep = i;
   while(L_IP[i] != ';') i++;
   A_cc_tmp1.Maj(L_IP, pos_dep, i);
   Tab_connaissance[pos].Ajouter_a_la_fin(A_cc_tmp1);
   i++; // On passe le ;
  }
}

const bool alx_horloge_vectorielle::Inferieur_dans_ordre_total(const alx_horloge_vectorielle &h)
{if(*this < h) return true;
  else
   {if(!(*this>h))
      return TabIP[Proprietaire()] < h.IP( h.Proprietaire() );
    return false;
   }
}

//______________________________________________________________________________
//__________________________________ Opérateurs ________________________________
//______________________________________________________________________________
const alx_horloge_vectorielle &alx_horloge_vectorielle::operator=(const alx_horloge_vectorielle &h)
{Maj(h);
 return *this;}

//______________________________________________________________________________
const bool alx_horloge_vectorielle::operator==(const alx_horloge_vectorielle &h) const
{if(Ordre() != h.Ordre()) return false;
 const unsigned int *t1 = Horloge()
                  , *t2 = h.Horloge();
 for(unsigned int i=0; i<Ordre(); i++)
   if(t1[i] != t2[i]) return false;
 return true;
}

//______________________________________________________________________________
const bool operator<=(const alx_horloge_vectorielle &h1, const alx_horloge_vectorielle &h2)
{if(h1.Ordre() != h2.Ordre()) return false;
 const unsigned int *t1 = h1.Horloge()
                  , *t2 = h2.Horloge();
 for(unsigned int i=0; i<h1.Ordre(); i++)
   if(t1[i] > t2[i]) return false;
 return true;
}

//______________________________________________________________________________
const bool operator>=(const alx_horloge_vectorielle &h1, const alx_horloge_vectorielle &h2)
{return h2<=h1;}

//______________________________________________________________________________
const bool operator<(const alx_horloge_vectorielle &h1, const alx_horloge_vectorielle &h2)
{if(h1.Ordre() != h2.Ordre()) return false;
 const unsigned int *t1 = h1.Horloge()
                  , *t2 = h2.Horloge();
 bool rep = false;
 for(unsigned int i=0; i<h1.Ordre(); i++)
  {if(t1[i] > t2[i]) return false;
   if(t1[i] < t2[i]) rep = true;}
 return rep;
}

//______________________________________________________________________________
const bool operator>(const alx_horloge_vectorielle &h1, const alx_horloge_vectorielle &h2)
{return h2<h1;}

//______________________________________________________________________________
const bool operator| (const alx_horloge_vectorielle &h1, const alx_horloge_vectorielle &h2)
{if( (!(h1<h2))
   &&(!(h2<h1)) ) return true;
 return false;
}

//______________________________________________________________________________
//_________________________ Sérialisation / désérialisation ____________________
//______________________________________________________________________________
const alx_chaine_char& alx_horloge_vectorielle::Serialiser_IPs() const
{serialisation_IPs.Maj("(",0,1);
   for(unsigned int i=0; i<Ordre(); i++)
    {serialisation_IPs += TabIP[i];
     serialisation_IPs.Ajouter(";",0,1);}
 serialisation_IPs.Ajouter(")",0,1);
 return serialisation_IPs;
}

//______________________________________________________________________________
const alx_chaine_char& alx_horloge_vectorielle::Serialiser() const
{serialisation.Maj("(",0,1);
 serialisation += Ordre();
 serialisation.Ajouter(";",0,1);
 serialisation += Proprietaire();
 serialisation.Ajouter(";(",0,2);
   for(unsigned int i=0; i<Ordre(); i++)
    {serialisation += H[i];
     serialisation.Ajouter(";", 0, 1);}
 serialisation.Ajouter("))",0,2);
 return serialisation;
}

//______________________________________________________________________________
void alx_horloge_vectorielle::Deserialiser(const alx_chaine_char &cc, unsigned int &pos)
{pos++; // la (
   Ordre( cc.Lire_entier(pos) );
   pos++; // le ;
   Proprietaire( cc.Lire_entier(pos) );
   pos += 2; // les ;(
   for(unsigned int i=0; i<Ordre(); i++)
    {H[i] = cc.Lire_entier(pos);
     pos++; // le ;
    }
 pos+=2; //les ))
}


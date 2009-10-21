#include "MetaData.h"

//_____________________________________________________________________________
//_____________________________________________________________________________
//_____________________________________________________________________________
void MetaData::init()
{nb_elements = 0;
 nb_reel     = 0;
 tailles     = (unsigned int *)NULL;
 positions   = (unsigned int *)NULL;
}

//_____________________________________________________________________________
//_____________________________________________________________________________
//_____________________________________________________________________________
MetaData::MetaData() : alx_chaine_char()
{init();}

//_____________________________________________________________________________
MetaData::~MetaData()
{nb_elements = 0;
 nb_reel     = 0;
 if(tailles)   {delete[] tailles;   tailles   = (unsigned int *)NULL;}
 if(positions) {delete[] positions; positions = (unsigned int *)NULL;}
 Allouer(16);
}

//_____________________________________________________________________________
//_____________________________________________________________________________
//_____________________________________________________________________________
void MetaData::Allouer(unsigned int nb)
{if(nb >= nb_reel)
  {unsigned int *new_tailles   = new unsigned int [2*nb]
              , *new_positions = new unsigned int [2*nb];
   nb_reel = 2*nb;
   if(tailles && positions)
    {for(int i=(int)nb_elements; i>=0; i--)
      {new_tailles  [i] = tailles  [i];
       new_positions[i] = positions[i];
      }
     if (tailles)   {delete[] tailles;   tailles   = new_tailles;}
     if (positions) {delete[] positions; positions = new_positions;}
    }
   tailles   = new_tailles;
   positions = new_positions; positions[0] = 0;
  }
}

//_____________________________________________________________________________
const bool MetaData::Ajouter(const char *var, const char *val)
{bool rep = false;
 if( nb_elements>0 && Retirer(var) ) {rep = true;}
 Allouer(nb_elements+2);
 tailles[nb_elements] = strlen(var);
 nb_elements++;
 *this += var;
 alx_chaine_char::Ajouter("=",0,1);
 *this += val;
 alx_chaine_char::Ajouter(";",0,1);
 positions[nb_elements] = Taille();

 return rep;
}

//_____________________________________________________________________________
const bool MetaData::Retirer(const char *var)
{cc_tmp = var;
 return Retirer(cc_tmp);
}

//_____________________________________________________________________________
const bool MetaData::Retirer(const alx_chaine_char &var)
{const int pos = Chercher(var, nb_elements);
 if(pos == -1)
   return false;
 const unsigned int deb = positions[pos];
 const unsigned int t   = positions[pos+1] - deb;

 this->Effacer(deb, t);
// Mise à jour des indexes:
 for(unsigned int i=pos; i<nb_elements; i++)
  {tailles[i]   = tailles[i+1];
   positions[i] = positions[i+1] - t;}
 nb_elements--;
 return true;
}

//_____________________________________________________________________________
const int MetaData::Chercher(const alx_chaine_char &var, unsigned int nb) const
{if( nb_elements==0 ) return -1;

 const unsigned int t = var.Taille();
 const char *texte = Texte();
 int i;
 int rep = -1;

 for(i=nb-1; i>=0; i--)
  {if(tailles[i] != t) continue;
   if(var.Prefixe_de( &(texte[positions[i]]) )) {rep = i; break;}
  }

 return rep;
}

//_____________________________________________________________________________
const char* MetaData::Val(const char *var) const
{cc_tmp = var;
 return Val_CC(cc_tmp);
}

//_____________________________________________________________________________
const char* MetaData::Val_CC(const alx_chaine_char &var) const
{const int pos = Chercher(var, nb_elements);
 if(pos == -1) {return (char*)NULL;
  } else {
    const unsigned int deb = positions[pos] + var.Taille() + 1;
    const unsigned int   t = positions[pos+1] - deb - 1;
    reponse.Maj(Texte(), deb, t);
    return reponse.Texte();
   }
}

//_____________________________________________________________________________
void MetaData::maj( const MetaData &m )
{nb_elements = m.nb_elements;
 Allouer(nb_elements);
 for(int i = (int)nb_elements; i>=0; i--)
  {tailles  [i] = m.tailles[i];
   positions[i] = m.positions[i];
  }
}

//_____________________________________________________________________________
void MetaData::Vider()
{nb_elements=0;
 Maj("",0,0);
 if(positions) positions[0]=0;
}

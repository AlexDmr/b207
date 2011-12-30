#include "alx_chaine_char.h"
#include "..\physique\math_alex.cpp"
//#include <mem.h>
#include <stdio.h>

alx_chaine_char chaine_vide;
const alx_chaine_char& Chaine_vide() {return chaine_vide;}

unsigned int pos_chaine = 0;
char tempon_char_256[256];

void alx_chaine_char::Ecrire_dans(const char *nom, const unsigned int nb)
{FILE *f;
 fopen_s(&f, nom, "w");
   fwrite(texte, nb, 1, f);
 fclose(f);
}

void alx_chaine_char::Lire_dans(const char *nom)
{char tmp[65536];
 FILE *f;
 fopen_s(&f, nom, "r");
   fread(tmp, 65536, 1, f);
   *this = tmp;
 fclose(f);
}

//______________________________________________________________________________
inline const unsigned int Pos_svt()
{unsigned int rep = pos_chaine;
 pos_chaine = (pos_chaine+1) % 32;
 return rep;}

//______________________________________________________________________________
void alx_chaine_char::init()
{texte = tmp = (char*)NULL;
 taille_reelle = taille = taille_tmp = 0;}

//______________________________________________________________________________
//_____________________________ Les constructeurs ______________________________
//______________________________________________________________________________
alx_chaine_char::alx_chaine_char()
{init();
 texte = new char[32];
 tmp   = new char[32];
 taille = 0;
 taille_reelle = taille_tmp = 32;
 texte[0] = 0;}

alx_chaine_char Tab_chaines[32];

//______________________________________________________________________________
alx_chaine_char::alx_chaine_char(const alx_chaine_char &c)
{init();
 *this = c;
}
//______________________________________________________________________________
alx_chaine_char::alx_chaine_char(const unsigned int n)
{init();
 Reallouer_texte(n);
 texte[0] = 0;
}
//______________________________________________________________________________
alx_chaine_char::alx_chaine_char(const char *txt)
{init();
 *this = txt;
}

//______________________________________________________________________________
alx_chaine_char::alx_chaine_char(const char *txt, const unsigned int n)
{init();
 Maj(txt, 0,  n);
 /*taille = n;
 taille_reelle = Puissance_2_sup(taille+1, 32);
 texte = new char[taille_reelle]; // +1 pour le 0 de fin.
 memcpy(texte, txt, taille+1);*/
 }
//______________________________________________________________________________
alx_chaine_char::~alx_chaine_char()
{if(texte)
   delete[] texte;
 if(tmp)
   delete[] tmp;
 texte = tmp = (char*)NULL;
}

//______________________________________________________________________________
//_____________________________ Les méthodes ___________________________________
//______________________________________________________________________________
const bool alx_chaine_char::Prefixe_de(const char *txt) const
{unsigned int i;

 for(i=0; i<taille; i++)
   if(txt[i] != texte[i]) break;

 return i==Taille();
}

//______________________________________________________________________________
const bool   alx_chaine_char::Lire_bool(unsigned int & pos) const
{if(texte[pos++] == '0') return false;
 return true;}

//______________________________________________________________________________
const double alx_chaine_char::Lire_double(unsigned int & pos) const
{double rep;
 pos += sscanf_s(&(texte[pos]), "%lf", &rep);
 while(( (texte[pos]>='0')
       &&(texte[pos]<='9'))
      ||(texte[pos]=='.') ) pos++;
/* while( (texte[pos] != ';')
      &&(texte[pos] != '_')
      &&(texte[pos] != ')') ) pos++;*/
 return rep;
/*double unites, decimales;
 unsigned int nb_decimales, pos_deb_decimales;

 unites = Lire_entier(pos);
 if(texte[pos] != '.') return unites;
 pos++; // On passe le .
 pos_deb_decimales = pos;
 decimales = Lire_entier(pos);
 nb_decimales = pos - pos_deb_decimales;
 return unites + decimales/pow(10, nb_decimales); */
}

//______________________________________________________________________________
const int alx_chaine_char::Lire_entier(unsigned int &pos) const
{int rep=0, signe=1;
 if(texte[pos]=='-') {signe=-1; pos++;}
 while( (texte[pos]>='0')
      &&(texte[pos]<='9') )
   rep = 10*rep + (texte[pos++]-'0');

 return signe*rep;
}

const unsigned int alx_chaine_char::Remplacer_char_par(const char c1, const char c2)
{unsigned int nb=0;
 for(unsigned int i=0; i<Taille(); i++)
  {if(texte[i] == c1) {texte[i] = c2; nb++;}
  }
 return nb;
}

//______________________________________________________________________________
const unsigned int alx_chaine_char::Position_du_prochain(const char c, const unsigned int p) const
{unsigned int rep = p;
 while( (rep < Taille()+1)
      &&(texte[rep] != c) ) rep++;
 return rep;
}

//______________________________________________________________________________
void alx_chaine_char::Effacer(const unsigned int dep, const unsigned int nb)
{if(dep > Taille()) return;
 unsigned int bout = Amin(Taille(), dep+nb)
            , t    = Taille() - bout;
 Reallouer_tmp(t+1);
 memcpy(tmp, &(texte[bout]), t+1);
 memcpy(&(texte[dep]), tmp, t+1);
 if(taille > dep+nb)
   taille -= nb;
  else taille = dep;
}

//______________________________________________________________________________
void alx_chaine_char::Inserer(const unsigned int pos, const alx_chaine_char &c)
{unsigned int t = c.Taille()
            , r = Taille()+1-pos;
 Reallouer_tmp(r);
 memcpy(tmp, &(texte[pos]), r);
 Reallouer_texte( Taille()+t+1 );
 memcpy(&(texte[pos]), c.Texte(), t);
 memcpy(&(texte[pos+t]), tmp, r);
 taille += t;
}

//______________________________________________________________________________
void alx_chaine_char::Inserer(const unsigned int pos, const char *c)
{unsigned int t = strlen(c)
            , r = Taille()+1-pos;
 Reallouer_tmp(r);
 memcpy(tmp, &(texte[pos]), r);
 Reallouer_texte( Taille()+t+1 );
 memcpy(&(texte[pos]), c, t);
 memcpy(&(texte[pos+t]), tmp, r);
 taille += t;
}

//______________________________________________________________________________
void alx_chaine_char::Reallouer_tmp(const unsigned int t)
{if(taille_tmp >= t) return;
 unsigned int nt = Puissance_2_sup(t, Amax(taille_tmp+1, (unsigned int)32));
 char *n_txt = new char[nt];
 taille_tmp = nt;
 if(tmp)
   delete[] tmp;
 tmp = n_txt;
}

//______________________________________________________________________________
void alx_chaine_char::Reallouer_texte(const unsigned int t)
{if(taille_reelle >= t) return;
 unsigned int nt = Puissance_2_sup(t, Amax(taille_reelle+1, (unsigned int)32));
 char *n_txt = new char[nt];
 if(Texte())
   memcpy(n_txt, Texte(), Taille()+1);
 taille_reelle = nt;
 if(Texte())
   delete[] texte;
 texte = n_txt;
}

//______________________________________________________________________________
void alx_chaine_char::Fixer_taille(const unsigned int t)
{Reallouer_texte(t+1);
 taille = t;}

//______________________________________________________________________________
void alx_chaine_char::Ajouter(const char *txt, const unsigned int dep, const unsigned int nb_char)
{Reallouer_texte(Taille()+nb_char+1);
 memcpy(&(texte[taille]), &(txt[dep]), nb_char);
 taille += nb_char;
 texte[Taille()] = 0;}

//______________________________________________________________________________
void alx_chaine_char::Maj(const char *txt, const unsigned int dep, const unsigned int nb_char)
{Reallouer_texte(nb_char+1);
 memcpy(texte, &(txt[dep]), nb_char);
 taille = nb_char;
 texte[nb_char] = 0;}

//______________________________________________________________________________
void alx_chaine_char::Maj(const char *txt, const unsigned int nb_char)
{Maj(txt, 0, nb_char);}

//______________________________________________________________________________
const alx_chaine_char &alx_chaine_char::operator+=(const alx_chaine_char &c)  // Surcharge du  +=
{Reallouer_texte( Taille()+c.Taille()+1 );
 memcpy(&(texte[taille]), c.Texte(), c.Taille()+1);
 taille += c.Taille();
 return *this;}

//______________________________________________________________________________
const alx_chaine_char &alx_chaine_char::operator+=(const char *c)  // Surcharge du  +=
{unsigned int t = strlen(c);
 Reallouer_texte( Taille()+t+1 );
 memcpy(&(texte[taille]), c, t+1);
 taille += t;
 return *this;}

//______________________________________________________________________________
const alx_chaine_char &alx_chaine_char::operator =(const alx_chaine_char &c)  // Surcharge du  +=
{Maj(c.Texte(), 0, c.Taille());
 return *this;}

//______________________________________________________________________________
const alx_chaine_char &alx_chaine_char::operator =(const char *c)
{unsigned int t = strlen(c);
 Reallouer_texte( t+1 );
 memcpy(texte, c, t+1);
 taille = t;
 return *this;}

//______________________________________________________________________________
const alx_chaine_char &alx_chaine_char::operator =(const bool b)
{if(b) this->Maj("1",0,1);
 else this->Maj("0",0,1);
 return *this;}

//______________________________________________________________________________
const alx_chaine_char &alx_chaine_char::operator =(const int i)
{int nb = sprintf_s(tempon_char_256, 256, "%d", i);
 this->Maj(tempon_char_256,0,nb);
 return *this;}

//______________________________________________________________________________
const alx_chaine_char &alx_chaine_char::operator =(const unsigned long int i)
{int nb = sprintf_s(tempon_char_256, 256, "%lu", i);
 this->Maj(tempon_char_256,0,nb);
 return *this;}

//______________________________________________________________________________
const alx_chaine_char &alx_chaine_char::operator =(const unsigned int i)
{int nb = sprintf_s(tempon_char_256, 256, "%u", i);
 this->Maj(tempon_char_256,0,nb);
 return *this;}

//______________________________________________________________________________
const alx_chaine_char &alx_chaine_char::operator =(const double v)
{double d;
 if(absd(v) < 0.000001) {d = 0;} else {d = v;}
 int nb = sprintf_s(tempon_char_256, 256, "%lf", d);
 if(nb == 0) return *this;
 while(tempon_char_256[nb-1] == '0') nb--;
 if(tempon_char_256[nb-1] == '.') nb--;
 this->Maj(tempon_char_256, 0, nb);
 return *this;}

//______________________________________________________________________________
const alx_chaine_char &alx_chaine_char::operator+=(const bool b)
{Reallouer_texte( Taille()+2 );
 if(b) texte[Taille()] = '1';
  else texte[Taille()] = '0';
 taille++;
 texte[taille] = 0;
 return *this;}

//______________________________________________________________________________
const alx_chaine_char &alx_chaine_char::operator+=(const int i)
{int nb = sprintf_s(tempon_char_256, 256, "%d", i);
 this->Ajouter(tempon_char_256, 0, nb);
 return *this;}

//______________________________________________________________________________
const alx_chaine_char &alx_chaine_char::operator+=(const unsigned long  i)
{int nb = sprintf_s(tempon_char_256, 256, "%lu", i);
 this->Ajouter(tempon_char_256, 0, nb);
 return *this;}

//______________________________________________________________________________
const alx_chaine_char &alx_chaine_char::operator+=(const unsigned int i)
{int nb = sprintf_s(tempon_char_256, 256, "%u", i);
 this->Ajouter(tempon_char_256, 0, nb);
 return *this;}

//______________________________________________________________________________
const alx_chaine_char &alx_chaine_char::operator+=(const double v)
{double d;
 if(absd(v) < 0.000001) {d = 0;} else {d = v;}
 int nb = sprintf_s(tempon_char_256, 256, "%f", d);
 if(nb == 0) return *this;
 while(tempon_char_256[nb-1] == '0') nb--;
 if(tempon_char_256[nb-1] == '.') nb--;
 this->Ajouter(tempon_char_256, 0, nb);
 return *this;}

//______________________________________________________________________________
const bool alx_chaine_char::operator!=(const alx_chaine_char &c) const
{return !(*this==c);}

//______________________________________________________________________________
const bool alx_chaine_char::operator==(const alx_chaine_char &c) const // Surcharge du  ==
{if( Taille() != c.Taille() ) return false;
 const char *t1 = Texte()
          , *t2 = c.Texte();
 for(int i=Taille();i>=0;i--)
   if(t1[i]!=t2[i]) return false;
 return true;
}

//______________________________________________________________________________
const bool alx_chaine_char::operator!=(const char *s) const  // Surcharge du  ==
{return !(*this==s);}

//______________________________________________________________________________
const bool alx_chaine_char::operator==(const char *s) const  // Surcharge du  ==
{const char *t = Texte();
 unsigned int i = 0;

 while( (s[i] != 0)
      &&(i < Taille())
      &&(s[i] == t[i]) ) i++;
 return (i==Taille())&&(s[i]==0);
}

//______________________________________________________________________________
//_________________________ Les opérateurs infixés______________________________
//______________________________________________________________________________
const alx_chaine_char& operator+(const char *cc, const alx_chaine_char &ca)
{Tab_chaines[pos_chaine] = cc;
 Tab_chaines[pos_chaine]+= ca;
 return Tab_chaines[Pos_svt()];}
//______________________________________________________________________________
const alx_chaine_char& operator+(const alx_chaine_char &ca, const char *cc)
{Tab_chaines[pos_chaine] = ca;
 Tab_chaines[pos_chaine]+= cc;
 return Tab_chaines[Pos_svt()];}
//______________________________________________________________________________
const alx_chaine_char& operator+(const alx_chaine_char &ca1, const alx_chaine_char &ca2)
{Tab_chaines[pos_chaine] = ca1;
 Tab_chaines[pos_chaine]+= ca2;
 return Tab_chaines[Pos_svt()];}

//______________________________________________________________________________
const alx_chaine_char& operator+(const int v, const alx_chaine_char &cc)
{Tab_chaines[pos_chaine] = v;
 Tab_chaines[pos_chaine]+= cc;
 return Tab_chaines[Pos_svt()];}
//______________________________________________________________________________
const alx_chaine_char& operator+(const alx_chaine_char &cc, const int v)
{Tab_chaines[pos_chaine] = cc;
 Tab_chaines[pos_chaine]+= v;
 return Tab_chaines[Pos_svt()];}
//______________________________________________________________________________
const alx_chaine_char& operator+(const unsigned int v, const alx_chaine_char &cc)
{Tab_chaines[pos_chaine] = v;
 Tab_chaines[pos_chaine]+= cc;
 return Tab_chaines[Pos_svt()];}
//______________________________________________________________________________
const alx_chaine_char& operator+(const alx_chaine_char &cc, const unsigned int v)
{Tab_chaines[pos_chaine] = cc;
 Tab_chaines[pos_chaine]+= v;
 return Tab_chaines[Pos_svt()];}
//______________________________________________________________________________
const alx_chaine_char& operator+(const double v, const alx_chaine_char &cc)
{Tab_chaines[pos_chaine] = v;
 Tab_chaines[pos_chaine]+= cc;
 return Tab_chaines[Pos_svt()];}
//______________________________________________________________________________
const alx_chaine_char& operator+(const alx_chaine_char &cc, const double v)
{Tab_chaines[pos_chaine] = cc;
 Tab_chaines[pos_chaine]+= v;
 return Tab_chaines[Pos_svt()];}
//______________________________________________________________________________
const alx_chaine_char& operator+(const bool v, const alx_chaine_char &cc)
{Tab_chaines[pos_chaine] = v;
 Tab_chaines[pos_chaine]+= cc;
 return Tab_chaines[Pos_svt()];}
//______________________________________________________________________________
const alx_chaine_char& operator+(const alx_chaine_char &cc, const bool v)
{Tab_chaines[pos_chaine] = cc;
 Tab_chaines[pos_chaine]+= v;
 return Tab_chaines[Pos_svt()];}

//______________________________________________________________________________
const bool alx_chaine_char::Egal_a(const char *txt, const unsigned int premier, const unsigned int dernier) const
{if(premier > Taille() || dernier > Taille()) {return false;}
 const char *T = Texte();
 for(unsigned int i=premier; i<=dernier; i++)
  {if(T[i] != txt[i-premier]) {return false;}}
 return true;
}

//______________________________________________________________________________
const bool alx_chaine_char::operator<(const alx_chaine_char& cc) const
{unsigned int tmin = Amin(Taille(), cc.Taille());
 const char *T1 = Texte()
          , *T2 = cc.Texte();
 for(unsigned int i=0; i<tmin; i++)
  {if(T1[i]<T2[i]) return true;
   if(T1[i]>T2[i]) return false;}
 if(Taille() >= cc.Taille())
   return false;
 return true;
}

//______________________________________________________________________________
const bool alx_chaine_char::operator>(const alx_chaine_char& cc) const
{return cc<*this;}

//______________________________________________________________________________
/*const bool operator<(const alx_chaine_char &cc1, const alx_chaine_char &cc2)
{unsigned int tmin = Amin(cc1.Taille(), cc2.Taille());
 const char *T1 = cc1.Texte()
          , *T2 = cc2.Texte();
 for(unsigned int i=0; i<tmin; i++)
  {if(T1[i]<T2[i]) return true;
   if(T1[i]>T2[i]) return false;}
 if(cc1.Taille() == tmin)
   return true;
 return false;
}*/
//______________________________________________________________________________
const bool operator>(const alx_chaine_char &cc1, const alx_chaine_char &cc2)
{return cc2<cc1;}



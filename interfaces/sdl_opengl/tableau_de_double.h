#ifndef __ALX_NOEUD_TAB_DOUBLE_H__
#define __ALX_NOEUD_TAB_DOUBLE_H__

class Tab_double
{private:
  double       *tab;
  unsigned int nb_elements;

 public:
   Tab_double() {
     tab         = (double*)NULL;
     nb_elements = 0;}
  Tab_double(const unsigned int n) {
    tab         = (double*)NULL;
    nb_elements = 0;
    Reserver_place(n);}
  ~Tab_double() {if(tab) delete[] tab; tab = (double*)NULL;}
 //_________________________________________
  void Reserver_place(const unsigned int n) {if(tab) delete[] tab; tab = (double*)NULL;
                                             tab = new double[n];
                                             nb_elements = n;}
  const double Consulter(const unsigned int pos) {if(pos>=nb_elements) return 0;
                                                  return tab[pos];}
  const bool Ajouter(const unsigned int pos, const double v) {if(pos>=nb_elements) return false;
                                                              tab[pos] = v; return true;}
  const bool Ajouter(const unsigned int pos, const double v1, const double v2) {if(pos+2>nb_elements) return false;
                                                                                tab[pos] = v1; tab[pos+1] = v2; return true;}
  const bool Ajouter( const unsigned int pos
                    , const double v1, const double v2
                    , const double v3, const double v4) {if(pos+4>nb_elements) return false;
                                                         tab[pos]   = v1;
                                                         tab[pos+1] = v2;
                                                         tab[pos+2] = v3;
                                                         tab[pos+3] = v4;
                                                         return true;}

  const double *Tab() const {return tab;}
  const unsigned int Taille() const {return nb_elements;}
};

#endif

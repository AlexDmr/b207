
#ifndef __ALX_REPERE_2D_H__
#define __ALX_REPERE_2D_H__

#include "alx_geometrie2D.h"
//#include "alx_point2D.h"
//#include "alx_vecteur2D.h"
#include "..\utilitaires\alx_liste.h" //<list.h>
#include "constantes.h"
#include "..\utilitaires\alx_chaine_char.h"

//class alx_point2D;

class alx_repere2D : public alx_primitive2D
{private:
   bool identite;
   bool prendre_en_compte_repere;
   unsigned int ordre_rendu;
   mutable alx_chaine_char serialisation_rep, serialisation_differences_rep;
   mutable alx_point2D   pt_tmp;
   mutable alx_vecteur2D v_tmp;
   static alx_vecteur2D etirement_courant;
   void init_repere() {
     is_inside_A_changer = is_inside_Prendre_en_compte_repere = false;
    }

 protected:
   mutable alx_vecteur2D etirement_noeud_svg;
   bool changement_repere, changer_serialisation;
   mutable bool is_inside_A_changer, is_inside_Prendre_en_compte_repere;

 public :
   alx_point2D origine; // En coordonnée du repère courant
   double rotation;
   alx_vecteur2D etirement, glissement;

   alx_repere2D() : origine(0,0), etirement(1,1), glissement(0,0), prendre_en_compte_repere(true)
    {init_repere();
     rotation = 0.0;
     ordre_rendu = 0;
     changement_repere = changer_serialisation = true;
     Identite( false );
     Prendre_en_compte_repere( true );}

   alx_repere2D(const alx_repere2D &rep)
    {init_repere();
     changer_serialisation = true;
     alx_repere2D::A_changer( true );
     maj(rep);}

   alx_repere2D( const alx_point2D &o
               , const double rot ) : etirement(1,1), glissement(0,0)
    {init_repere();
     Origine(o);
     Rotation(rot);
     Identite( false );
     ordre_rendu = 0;
     Prendre_en_compte_repere( false );
     changer_serialisation = true;
     alx_repere2D::A_changer( true );
    }

   alx_repere2D( const alx_point2D &o
               , const double rot
               , const alx_vecteur2D &e ) : glissement(0,0) {init_repere();
                                                             ordre_rendu = 0;
                                                             changer_serialisation = true;
                                                             alx_repere2D::A_changer( true );
                                                             maj(o, rot, e);}

 // La sérialisation:
  virtual const alx_chaine_char& Serialisation_contenu()     const {return serialisation_rep;}
  virtual const alx_chaine_char& Serialisation_differences() const {return serialisation_differences_rep;}
  virtual const alx_chaine_char& Serialiser_contenu() const;
  virtual void Deserialiser_contenu(const alx_chaine_char &txt, unsigned int &pos);
  virtual const alx_chaine_char& Serialiser_difference_contenu() const;


 //_____________________________________________________________________________
  inline virtual const bool A_changer() const {return changement_repere;}
  inline virtual void A_changer(const bool b) {is_inside_A_changer = true;
                                               if(b!=changement_repere && !Prendre_en_compte_repere())
                                                {changement_repere = b;
                                                 if (b && !is_inside_Prendre_en_compte_repere) Prendre_en_compte_repere(true);
                                                } else {changement_repere = b;
                                                       }
                                               is_inside_A_changer = false;
                                              }

 //_____________________________________________________________________________
  inline void Ordre_rendu(const unsigned int i) {ordre_rendu = i;}
  inline const unsigned int Ordre_rendu() const {return ordre_rendu;}

 //_____________________________________________________________________________
 // Les surcharges de alx_primitive2D
  inline virtual void Px(const double v) {origine.X(v)    ; alx_repere2D::A_changer( true );}
  inline virtual void Py(const double v) {origine.Y(v)    ; alx_repere2D::A_changer( true );}
  inline virtual void Ex(const double v) {etirement.Dx(v) ; alx_repere2D::A_changer( true );}
  inline virtual void Ey(const double v) {etirement.Dy(v) ; alx_repere2D::A_changer( true );}
  inline virtual void Gx(const double v) {glissement.Dx(v); alx_repere2D::A_changer( true );}
  inline virtual void Gy(const double v) {glissement.Dy(v); alx_repere2D::A_changer( true );}
  inline virtual void R (const double v) {Rotation(v);}
  inline virtual const double         Px()        const {return origine.X();}
  inline virtual const double         Py()        const {return origine.Y();}
  inline virtual const double         Ex()        const {return etirement.Dx();}
  inline virtual const double         Ey()        const {return etirement.Dy();}
  inline virtual const double         Gx()        const {return glissement.Dx();}
  inline virtual const double         Gy()        const {return glissement.Dy();}
  inline virtual const double         R ()        const {return rotation;}
  inline virtual const alx_point2D&   Origine()   const {return origine;}
  inline virtual const alx_vecteur2D& Glissement()const {return glissement;}
  inline virtual const alx_vecteur2D& Etirement() const {return etirement;}
  inline virtual const double         Rotation()  const {return rotation;}

//_____________________________________________________________________________
  const bool Est_entite() const;

//_____________________________________________________________________________
  virtual void maj_etirement_courant() const;
  inline void Etirement_courant(const double x, const double y) {etirement_courant.maj(x, y);}
  inline void Etirement_courant(const alx_vecteur2D &e) {etirement_courant.maj(e);}
  inline const alx_vecteur2D& Etirement_courant() const {return etirement_courant;}
 //_____________________________________________________________________________
  inline const bool Prendre_en_compte_repere() const {return prendre_en_compte_repere;}
  inline void Prendre_en_compte_repere(const bool b) {is_inside_Prendre_en_compte_repere = true;
                                                        prendre_en_compte_repere = b;
                                                        if( !alx_repere2D::A_changer()
                                                          &&!is_inside_A_changer ) alx_repere2D::A_changer( true );
                                                      is_inside_Prendre_en_compte_repere = false;
                                                     }
  inline void Identite(const bool b) {identite = b; alx_repere2D::A_changer( true );}
  inline const bool Identite() const {return identite;}

  inline void maj( const double x, const double y
                 , const double r
                 , const double ex  , const double ey
                 , const double gx=0, const double gy=0)
   {Origine(x,y); Rotation(r); Etirement(ex,ey); Glissement(gx, gy);}
  inline void maj(const alx_repere2D &r)
   {Origine(r.Origine()); Rotation(r.Rotation()); Etirement(r.Etirement()); Glissement(r.Glissement()); Ordre_rendu(r.Ordre_rendu());}
  inline void maj(const alx_point2D &o, const double r, const alx_vecteur2D &e)
   {Origine(o); Rotation(r); Etirement(e);}
  inline void maj(const alx_point2D &o, const double r, const alx_vecteur2D &e, const alx_vecteur2D &g)
   {Origine(o); Rotation(r); Etirement(e); Glissement(g);}

 //_____________________________________________________________________________
  void Inverser_coordonnees();      // Tiens compte de la rotation et de l'étirement pour le calcul de l'origine.
  void Inverser_coordonnees_brut(); // Inverse toutes les coordonnées.
 //_____________________________________________________________________________
  virtual inline void Changer_coordonnees_prim( alx_primitive2D *prim )
   {if( Identite()
      ||!Prendre_en_compte_repere() ) return;
    if(Rotation() != 0)
      prim->Pivoter( -Rotation(), Origine() );
    //alx_vecteur2D trans( Origine() );
    prim->Translation( Origine() );
    //trans.Oppose();
    if( Etirement().dx != 0 && Etirement().dy != 0 )
      prim->Etirer(1/Etirement().dx, 1/Etirement().dy);
   }

  virtual inline void Changer_coordonnees_prim_inverse( alx_primitive2D &prim )
   {if( Identite()
      ||!Prendre_en_compte_repere() ) return;
    prim.Etirer(Etirement().dx, Etirement().dy);
    if(Rotation() != 0)
      prim.Pivoter(Rotation(), Pt_nul());
    prim.Translation( Origine() );
   }

  virtual void Changer_coordonnees_inverse( alx_point2D &pt ) const;
  virtual void Changer_coordonnees        ( alx_point2D &pt ) const;
  virtual void Changer_coordonnees_inverse(double *T, const unsigned int nb) const;
  virtual void Changer_coordonnees        (double *T, const unsigned int nb) const;

  // Ré-exprimer le repère par rapport à un autre:
  void Exprimer_dans(const alx_repere2D &R);
  void Exprimer_par_rapport_a(const alx_repere2D &R);

  inline virtual void Origine(const alx_point2D &pt)          {origine.maj(pt)  ; alx_repere2D::A_changer(true);}
  inline virtual void Origine(const double x, const double y) {origine.maj(x, y); alx_repere2D::A_changer(true);}

  inline virtual void Translation(const double dx, const double dy)  {origine.Translation(dx, dy); alx_repere2D::A_changer(true);}
  inline virtual void Translation(const alx_vecteur2D &v)            {origine.Translation(v); alx_repere2D::A_changer(true);}

  virtual void Translation_interne(const double dx, const double dy);
  inline virtual void Translation_interne(const alx_vecteur2D &v) {Translation_interne(v.dx, v.dy);}
  virtual void Pivoter_interne    (const double r, const alx_point2D &p);
  inline virtual void Pivoter_interne    (const double r, const double cx, const double cy) {Pivoter_interne(r, alx_point2D(cx, cy));}

  virtual void Etirement_interne    (const double ex, const double ey, const double cx, const double cy);

  inline virtual void Pivoter    (const double r, const alx_point2D &p)
   {origine.Pivoter(r, p);
    rotation += r;
    alx_repere2D::A_changer(true);}
  inline virtual void Pivoter    (const double r, const double cx, const double cy)
   {origine.Pivoter(r, alx_point2D(cx, cy));
    rotation += r;
    alx_repere2D::A_changer(true);}
  inline virtual void Rotation   (const double r)                    {rotation = r; alx_repere2D::A_changer(true);}

  virtual void Symetrique (const alx_point2D &);
  virtual void Symetrique (const alx_droite2D &);

  inline virtual void Etirement  (const alx_vecteur2D &v)            {etirement.maj(v); alx_repere2D::A_changer(true);}
  inline virtual void Etirement  (const double x, const double y)    {etirement.maj(x, y); alx_repere2D::A_changer(true);}
  inline virtual void Etirer_de  (const alx_vecteur2D &v)            {etirement.Etirer(v); alx_repere2D::A_changer(true);}
  inline virtual void Etirer_de  (const double x, const double y)    {etirement.Etirer(x, y); alx_repere2D::A_changer(true);}

  inline virtual void Glissement  (const alx_vecteur2D &v)            {glissement.maj(v); alx_repere2D::A_changer(true);}
  inline virtual void Glissement  (const double x, const double y)    {glissement.maj(x, y); alx_repere2D::A_changer(true);}
  inline virtual void Glisser_de  (const alx_vecteur2D &v)            {glissement.Etirer(v); alx_repere2D::A_changer(true);}
  inline virtual void Glisser_de  (const double x, const double y)    {glissement.Etirer(x, y); alx_repere2D::A_changer(true);}

 // les méthodes renvoyant un pointeur sur un nouvel objet
virtual alx_repere2D& alx_repere2D::symetrique(const alx_point2D &pivot) const;
virtual alx_repere2D& alx_repere2D::symetrique(const alx_droite2D &axe) const;
virtual alx_repere2D& alx_repere2D::etirer(const alx_vecteur2D &) const;
};
typedef alx_repere2D* P_alx_repere2D;

// Quelques fonctions bien utiles
void Repere_transformation_inverse( alx_point2D &p
                                  , const alx_liste<alx_repere2D*> &L_reperes);
void Repere_transformation(alx_point2D &p, const alx_liste<alx_repere2D*> &L_reperes);
void Repere_transformation_inverse_sauf_dernier( alx_point2D &p
                                               , const alx_liste<alx_repere2D*> &L_reperes);
void Repere_transformation_sauf_dernier(alx_point2D &p, const alx_liste<alx_repere2D*> &L_reperes);

void Lister_partie( const alx_liste<alx_repere2D*>         &L_rep
                  , alx_element_liste<alx_repere2D*> *premier
                  , alx_element_liste<alx_repere2D*> *dernier
                  , alx_liste<alx_repere2D*> &L_res);
void Lister_partie_rep( const alx_liste<alx_repere2D*>         &L_rep
                      , alx_repere2D* premier
                      , alx_repere2D* dernier
                      , alx_liste<alx_repere2D*> &L_res);

void Condenser_reperes( alx_liste<alx_repere2D*> &L_rep
                      , alx_repere2D &res
                      , const unsigned int deb=0, const unsigned int fin=999999);

#endif



#include "alx_repere2D.h"
#include "math_alex.cpp"
#include "alx_geometrie2D.h"

alx_vecteur2D alx_repere2D::etirement_courant;

void alx_repere2D::Deserialiser_contenu(const alx_chaine_char &txt, unsigned int &pos)
{pos++; //on passe la (
 origine.Deserialiser_contenu(txt, pos);
 pos++; // on passe le ;
 rotation = txt.Lire_double(pos);
 pos++; // on passe le ;
 etirement.Deserialiser_contenu(txt, pos);
 pos++; // on passe le ;
 glissement.Deserialiser_contenu(txt, pos);
 pos++; // on passe le ;
 identite = txt.Lire_bool(pos);
 pos++; // on passe le ;
 prendre_en_compte_repere = txt.Lire_bool(pos);
 pos++; // on passe le ;
 ordre_rendu = txt.Lire_entier(pos);
 pos++; // on passe le ;
 pos++; // on passe la )
}

//______________________________________________________________________________
const alx_chaine_char& alx_repere2D::Serialiser_contenu() const
{serialisation_rep.Maj("(",0,1);                            // (
 serialisation_rep += origine.Serialiser_contenu();         //  origine
 serialisation_rep.Ajouter(";",0,1);                        // ,
 serialisation_rep += rotation;                             //  rotation
 serialisation_rep.Ajouter(";",0,1);                        // ,
 serialisation_rep += etirement.Serialiser_contenu();       //  etirement
 serialisation_rep.Ajouter(";",0,1);                        // ,
 serialisation_rep += glissement.Serialiser_contenu();      //  glissement
 serialisation_rep.Ajouter(";",0,1);                        // ,
 serialisation_rep += identite;                             //  identite
 serialisation_rep.Ajouter(";",0,1);                        // ,
 serialisation_rep += prendre_en_compte_repere;             //  prendre_en_compte_repere
 serialisation_rep.Ajouter(";",0,1);                        // ,
 serialisation_rep += ordre_rendu;                          //  ordre_rendu
 serialisation_rep.Ajouter(";)",0,2);                       // ;)
 return serialisation_rep;
}

//______________________________________________________________________________
const alx_chaine_char& alx_repere2D::Serialiser_difference_contenu() const
{bool b_tmp;
 double d_tmp;
 int i_tmp;
 unsigned int pos = 0;

 //serialisation_differences_rep.Maj("",0,0);// = "";
 pos++; //on passe la (
 pt_tmp.Deserialiser_contenu(serialisation_rep, pos);
   if( !pt_tmp.equivalent(origine, EPSILON) )  {serialisation_differences_rep = alx_repere2D::Serialiser_contenu(); return serialisation_differences_rep;}
 pos++; // on passe le ;
 d_tmp = serialisation_rep.Lire_double(pos);
   if( absd(rotation-d_tmp) > EPSILON )        {serialisation_differences_rep = alx_repere2D::Serialiser_contenu(); return serialisation_differences_rep;}
 pos++; // on passe le ;
 v_tmp.Deserialiser_contenu(serialisation_rep, pos);
   if( !v_tmp.equivalent(etirement, EPSILON) ) {serialisation_differences_rep  = alx_repere2D::Serialiser_contenu(); return serialisation_differences_rep;}
 pos++; // on passe le ;
 v_tmp.Deserialiser_contenu(serialisation_rep, pos);
   if( !v_tmp.equivalent(glissement, EPSILON) ) {serialisation_differences_rep = alx_repere2D::Serialiser_contenu(); return serialisation_differences_rep;}
 pos++; // on passe le ;
 b_tmp = serialisation_rep.Lire_bool(pos);
   if( b_tmp!=identite )                       {serialisation_differences_rep = alx_repere2D::Serialiser_contenu(); return serialisation_differences_rep;}
 pos++; // on passe le ;
 b_tmp = serialisation_rep.Lire_bool(pos);
   if( b_tmp!=prendre_en_compte_repere )       {serialisation_differences_rep = alx_repere2D::Serialiser_contenu(); return serialisation_differences_rep;}
 pos++; // on passe le ;
 i_tmp = serialisation_rep.Lire_entier(pos);
   if( i_tmp!=(int)ordre_rendu )               {serialisation_differences_rep = alx_repere2D::Serialiser_contenu(); return serialisation_differences_rep;}
// pos++; // on passe le ;
// pos++; // on passe la )
 serialisation_differences_rep.Maj("",0,0);
 return serialisation_differences_rep;
}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
void alx_repere2D::maj_etirement_courant() const {etirement_courant.dx *= Ex();
                                                  etirement_courant.dy *= Ey();}
//______________________________________________________________________________
void alx_repere2D::Translation_interne(const double dx, const double dy)
{alx_vecteur2D trans_x( Vect_h() );
 trans_x.Pivoter( rotation );
 alx_vecteur2D trans_y( trans_x );
 trans_y.Perpendiculaire_g();
 trans_x.Alonger(dx);
 trans_y.Alonger(dy);

 trans_x.Ajouter( trans_y );
 Translation( trans_x );
}

//______________________________________________________________________________
void alx_repere2D::Pivoter_interne    (const double r, const alx_point2D &p)
{alx_point2D tmp( Origine() );
 Translation_interne(p.x, p.y);
 alx_point2D centre_rot( Origine() );
 Origine( tmp );

 Pivoter(r, centre_rot);
}

//______________________________________________________________________________
void alx_repere2D::Etirement_interne    (const double ex, const double ey, const double cx, const double cy)
{Translation_interne( -Ex()*cx*(ex-1)
                    , -Ey()*cy*(ey-1));
 Etirer_de(ex, ey);
 }

// Les surcharges de alx_primitive2D
//______________________________________________________________________________
void alx_repere2D::Symetrique (const alx_point2D &p)
{origine.Symetrique(p);
 rotation += PI;
 alx_repere2D::A_changer( true );
}

//______________________________________________________________________________
void alx_repere2D::Symetrique (const alx_droite2D &d)
{alx_point2D unite_abs(origine);
 unite_abs.x += 1;
 unite_abs.Pivoter(rotation, origine);
 unite_abs.Symetrique(d);
 origine.Symetrique(d);

 alx_vecteur2D vect(origine, unite_abs);
 rotation = vect.angle_trigo(Vect_h());
 etirement.dy = -etirement.dy;
 alx_repere2D::A_changer( true );
}

//______________________________________________________________________________
 // les méthodes renvoyant un pointeur sur un nouvel objet

alx_repere2D& alx_repere2D::symetrique(const alx_point2D &pivot) const
{alx_repere2D *rep = new alx_repere2D(*this);
 rep->Symetrique(pivot);
 return *rep;}

alx_repere2D& alx_repere2D::symetrique(const alx_droite2D &axe) const
{alx_repere2D *rep = new alx_repere2D(*this);
 rep->Symetrique(axe);
 return *rep;}

alx_repere2D& alx_repere2D::etirer(const alx_vecteur2D &e) const
{alx_repere2D *rep = new alx_repere2D(*this);
 rep->Etirer(e);
 return *rep;}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
void alx_repere2D::Exprimer_dans(const alx_repere2D &R)
{R.Changer_coordonnees( origine );
 rotation -= R.Rotation();
 etirement.dx  /= R.etirement.dx;
 etirement.dy  /= R.etirement.dy;
 glissement.dx -= R.glissement.dx;
 glissement.dy -= R.glissement.dy;
 alx_repere2D::A_changer( true );
}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
void alx_repere2D::Exprimer_par_rapport_a(const alx_repere2D &R)
{const alx_vecteur2D &E_R = R.Etirement()
                   , &G_R = R.Glissement();
 v_tmp.maj(R.Origine());
 v_tmp.Oppose();
 v_tmp.Pivoter( -R.Rotation() );
 v_tmp.Etirer(1/E_R.Dx(), 1/E_R.Dy());
 v_tmp.Soustraire( G_R.Dx() * v_tmp.Dy()
                 , G_R.Dy() * v_tmp.Dx());
 Origine(v_tmp.Dx(), v_tmp.Dy());
 //R.Changer_coordonnees( origine );

 rotation      -= R.Rotation();
 etirement.dx  /= R.etirement.dx;
 etirement.dy  /= R.etirement.dy;
 glissement.dx -= R.glissement.dx;
 glissement.dy -= R.glissement.dy;
 alx_repere2D::A_changer( true );
}

//______________________________________________________________________________
const bool alx_repere2D::Est_entite() const
{return !Prendre_en_compte_repere()
        ||
     (  Px()==0
     && Py()==0
     && R ()==0
     && Ex()==1
     && Ey()==1
     && Gx()==0
     && Gy()==0 );
}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
void alx_repere2D::Inverser_coordonnees_brut()
{Glissement(-glissement.Dx(), -glissement.Dy());
 Etirement(1/etirement.Dx(), 1/etirement.Dy());
 Rotation(-Rotation());
 Origine(-origine.X(), -origine.Y());
}
//______________________________________________________________________________
void alx_repere2D::Inverser_coordonnees()
{Glissement(-glissement.Dx(), -glissement.Dy());
 Etirement(1/etirement.Dx(), 1/etirement.Dy());
 Rotation(-Rotation());
 Origine(-origine.X()*etirement.Dx(), -origine.Y()*etirement.Dy());
 origine.Pivoter( Rotation() );
}
//______________________________________________________________________________
void Repere_transformation_inverse( alx_point2D &p
                                  , const alx_liste<alx_repere2D*> &L_reperes)
{alx_element_liste<alx_repere2D*> *it     = L_reperes.Dernier()
                                , *it_deb = L_reperes.Debut();
 for(;it!=it_deb;it=it->prcdt)
   (it->E())->Changer_coordonnees_inverse(p);
}

//______________________________________________________________________________
void Repere_transformation(alx_point2D &p, const alx_liste<alx_repere2D*> &L_reperes)
{alx_element_liste<alx_repere2D*> *it     = L_reperes.Premier()
                                , *it_fin = L_reperes.Fin();
 for(;it!=it_fin;it=it->svt)
   (it->E())->Changer_coordonnees(p);
}

//______________________________________________________________________________
void Repere_transformation_inverse_sauf_dernier( alx_point2D &p
                                               , const alx_liste<alx_repere2D*> &L_reperes)
{if(L_reperes.Taille() == 0)
   return;
 alx_element_liste<alx_repere2D*> *it     = L_reperes.Dernier()
                                , *it_deb = L_reperes.Premier();
 for(;it!=it_deb;it=it->prcdt)
   (it->E())->Changer_coordonnees_inverse(p);
}
//______________________________________________________________________________
void Repere_transformation_sauf_dernier(alx_point2D &p, const alx_liste<alx_repere2D*> &L_reperes)
{if(L_reperes.Taille() == 0)
   return;
 alx_element_liste<alx_repere2D*> *it     = L_reperes.Premier()
                                , *it_fin = L_reperes.Dernier();
 for(;it!=it_fin;it=it->svt)
   (it->E())->Changer_coordonnees(p);
}
//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
void Lister_partie( const alx_liste<alx_repere2D*>         &L_rep
                  , alx_element_liste<alx_repere2D*> *premier
                  , alx_element_liste<alx_repere2D*> *dernier
                  , alx_liste<alx_repere2D*> &L_res)
{L_res.Vider();
 if(dernier!=L_rep.Fin()) {dernier = dernier->svt;}
 L_res.Ajouter_a_la_fin(L_rep, premier, dernier);
}

//______________________________________________________________________________
void Lister_partie_rep( const alx_liste<alx_repere2D*>         &L_rep
                      , alx_repere2D* premier
                      , alx_repere2D* dernier
                      , alx_liste<alx_repere2D*> &L_res)
{L_res.Vider();
 alx_element_liste<alx_repere2D*> *it_p = L_rep.It(premier)
                                , *it_d = L_rep.It(dernier);
 Lister_partie(L_rep, it_p, it_d, L_res);
}

//______________________________________________________________________________
void Condenser_reperes( alx_liste<alx_repere2D*> &L_rep
                      , alx_repere2D &res
                      , const unsigned int deb, const unsigned int fin)
{if(L_rep.Taille()<=deb)
  {res.maj(0,0,0,1,1);
   return;}

 alx_element_liste<alx_repere2D*> *it, *it_deb;
 if(deb==0)              {it_deb = L_rep.Debut();}   else {it_deb = L_rep.It(deb-1);}
 if(fin>=L_rep.Taille()) {it     = L_rep.Dernier();} else {it     = L_rep.It(fin);}

 alx_repere2D *cour = it->E();
 res.maj( *cour );
 it = it->prcdt;
 for(;it!=it_deb;it=it->prcdt)
  {cour = it->E();
   cour->Changer_coordonnees_inverse(res.origine);
   res.Glissement( res.Glissement().Dx()+cour->Glissement().Dx()
                 , res.Glissement().Dy()+cour->Glissement().Dy() );
   res.Etirement( res.Etirement().Dx()*cour->Etirement().Dx()
                , res.Etirement().Dy()*cour->Etirement().Dy() );
   res.Rotation( res.Rotation()+cour->Rotation() );
  }
}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
void alx_repere2D::Changer_coordonnees_inverse(double *T, const unsigned int nb) const
{if( Identite()
   ||!Prendre_en_compte_repere() ) return;
 unsigned int pos_x = 0
            , pos_y = 1;
 double tmp_x, tmp_y
      , cos_r = cos( Rotation() )
      , sin_r = sin( Rotation() );

 switch(Ordre_rendu())
  {case 0:
     // Faire rotation inverse et translation inverse.
     for(unsigned int i=0; i<nb; i++)
      {tmp_x=T[pos_x]; tmp_y=T[pos_y];
      // Glissement
       T[pos_x] += tmp_y*Gx();
       T[pos_y] += tmp_x*Gy();
      // Etirement
       T[pos_x] *= Etirement().Dx();
       T[pos_y] *= Etirement().Dy();
      // Rotation
       if(Rotation() != 0)
        {tmp_x=T[pos_x]; tmp_y=T[pos_y];
         T[pos_x] = tmp_x*cos_r - tmp_y*sin_r;
         T[pos_y] = tmp_x*sin_r + tmp_y*cos_r;}
      // Translation
       T[pos_x] += Origine().X();
       T[pos_y] += Origine().Y();
      // Prochain point !
       pos_x += 2;
       pos_y += 2;
      }
   break;
   case 1:
     for(unsigned int i=0; i<nb; i++)
      {// Translation
       T[pos_x] += Origine().X();
       T[pos_y] += Origine().Y();
      // Rotation
       if(Rotation() != 0)
        {tmp_x=T[pos_x]; tmp_y=T[pos_y];
         T[pos_x] = tmp_x*cos_r - tmp_y*sin_r;
         T[pos_y] = tmp_x*sin_r + tmp_y*cos_r;}
      // Etirement
       T[pos_x] *= Etirement().Dx();
       T[pos_y] *= Etirement().Dy();
      // Glissement
       tmp_x=T[pos_x]; tmp_y=T[pos_y];
       T[pos_x] += tmp_y*Gx();
       T[pos_y] += tmp_x*Gy();
      // Prochain point !
       pos_x += 2;
       pos_y += 2;
      }
   break;
  }
}

//______________________________________________________________________________
void alx_repere2D::Changer_coordonnees_inverse( alx_point2D &pt ) const
   {if( Identite()
      ||!Prendre_en_compte_repere() ) return;
    switch(Ordre_rendu())
     {case 0:
        // Faire rotation inverse et translation inverse.
        pt.maj( pt.X() + pt.Y()*Gx()
              , pt.Y() + pt.X()*Gy() );
        pt.Etirer(Etirement().dx, Etirement().dy);
        if(Rotation() != 0) pt.Pivoter( Rotation() );
        pt.Translation( Origine() );
      break;
      case 1:
        pt.Translation( Origine() );
        if(Rotation() != 0) pt.Pivoter( Rotation() );
        pt.Etirer(Etirement().dx, Etirement().dy);
        pt.maj( pt.X() + pt.Y()*Gx()
              , pt.Y() + pt.X()*Gy() );
      break;
     }
   }

//______________________________________________________________________________
void alx_repere2D::Changer_coordonnees(double *T, const unsigned int nb) const
{if( Identite()
   ||!Prendre_en_compte_repere() ) return;
 unsigned int pos_x = 0
            , pos_y = 1;
 double tmp_x, tmp_y
      , cos_r = cos( -Rotation() )
      , sin_r = sin( -Rotation() );
 if(Gx()*Gy()==1 || Etirement().Dx()==0 || Etirement().Dy()==0)
  {for(unsigned int i=0; i<nb; i++)
    {T[pos_x] = T[pos_y] = 0xAFFFFFFF;
     pos_x += 2;
     pos_y += 2;
    }
  }
 double det;
 switch(Ordre_rendu())
  {case 0:
     // Faire rotation inverse et translation inverse.
     for(unsigned int i=0; i<nb; i++)
      {// Translation inverse
       T[pos_x] -= Origine().X();
       T[pos_y] -= Origine().Y();
      // Rotation
       if( Rotation() != 0)
       {tmp_x=T[pos_x]; tmp_y=T[pos_y];
        T[pos_x] = tmp_x*cos_r - tmp_y*sin_r;
        T[pos_y] = tmp_x*sin_r + tmp_y*cos_r;}
      // Etirement
       T[pos_x] /= Etirement().Dx();
       T[pos_y] /= Etirement().Dy();
      // Glissement
       tmp_x=T[pos_x]; tmp_y=T[pos_y];
       T[pos_x] -= tmp_y*Gx();
       T[pos_y] -= tmp_x*Gy();
         det=1/(1-Gx()*Gy());
         T[pos_x] *= det;
         T[pos_y] *= det;
      // Prochain point !
       pos_x += 2;
       pos_y += 2;
      }
   break;
   case 1:
     for(unsigned int i=0; i<nb; i++)
      {// Glissement
       tmp_x=T[pos_x]; tmp_y=T[pos_y];
       T[pos_x] -= tmp_y*Gx();
       T[pos_y] -= tmp_x*Gy();
       if(Gx()*Gy() != 0)
         det=1/(1-Gx()*Gy());
         T[pos_x] *= det;
         T[pos_y] *= det;
      // Etirement
       T[pos_x] /= Etirement().Dx();
       T[pos_y] /= Etirement().Dy();
      // Rotation inverse
       if( Rotation() != 0)
       {tmp_x=T[pos_x]; tmp_y=T[pos_y];
        T[pos_x] = tmp_x*cos_r - tmp_y*sin_r;
        T[pos_y] = tmp_x*sin_r + tmp_y*cos_r;}
      // Translation inverse
       T[pos_x] -= Origine().X();
       T[pos_y] -= Origine().Y();
      // Prochain point !
       pos_x += 2;
       pos_y += 2;
      }
   break;
  }
}
//______________________________________________________________________________
void alx_repere2D::Changer_coordonnees( alx_point2D &pt ) const
   {if( Identite()
      ||!Prendre_en_compte_repere() ) return;
    if(Gx()*Gy() == 1 || this->Etirement().dx == 0 || this->Etirement().dy == 0) {pt.maj(0xAFFFFFFF, 0xAFFFFFFF); return;}

    double x, y, det;
    switch(Ordre_rendu())
     {case 0:
        // Faire rotation inverse et translation inverse.
       if( this->Rotation() != 0)
        {pt.Pivoter( -this->Rotation(), this->Origine() );}
       pt.Translation_inverse( this->Origine() );
       pt.Etirer(1/this->Etirement().dx, 1/this->Etirement().dy);

       x = pt.X(); y = pt.Y();
       pt.maj( x-y*Gx()
             , y-x*Gy() );

         det=1/(1-Gx()*Gy());
         pt.Etirer(det, det);

      break;
      case 1:
       x = pt.X(); y = pt.Y();
       pt.maj( x-y*Gx()
             , y-x*Gy() );

         det=1/(1-Gx()*Gy());
         pt.Etirer(det, det);

       pt.Etirer(1/this->Etirement().dx, 1/this->Etirement().dy);

       if( this->Rotation() != 0) pt.Pivoter( -this->Rotation() );
       pt.Translation_inverse( this->Origine() );
      break;
     }
   }


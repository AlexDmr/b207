#include "alx_sommet_opengl.h"

void alx_polygone_opengl::Etirer_coordonneees_texture(alx_vecteur2D &v, alx_point2D &centre)
{alx_element_liste<alx_point2D> *it_pt     = poly2D->L_points.Premier()
                              , *it_pt_fin = poly2D->L_points.Fin();
 for(;it_pt!=it_pt_fin;it_pt=it_pt->svt)
  {}
}

//______________________________________________________________________________
void alx_polygone_opengl::Raffraichir_polygl()
{alx_element_liste<alx_point2D> *it_pt = poly2D->L_points.Premier();
 alx_point2D *pt;
 for(unsigned int i=0;i<nb_sommets;i++)
  {pt = &(it_pt->E());
   tab_sommets[i].coord_espace[0] = pt->x;
   tab_sommets[i].coord_espace[1] = pt->y;
   it_pt = it_pt->svt;}
}

//______________________________________________________________________________
void alx_polygone_opengl::Raffraichir_polygl(const alx_polygone2D &poly)
{alx_element_liste<alx_point2D> *it_pt = poly.L_points.Premier();
 alx_point2D *pt;
 for(unsigned int i=0;i<nb_sommets;i++)
  {pt = &(it_pt->E());
   tab_sommets[i].coord_espace[0] = pt->x;
   tab_sommets[i].coord_espace[1] = pt->y;
   it_pt = it_pt->svt;}
}

//______________________________________________________________________________
void alx_polygone_opengl::Affichage_liste_triangle_directe(GLenum mode, int type_rendu)
{if(poly2D) Raffraichir_polygl();
 glBegin(mode);
   switch(type_rendu)
    {case 1: Rendu_couleur_direct(); break;
     case 2: Rendu_texture_direct(); break;
     case 3: Rendu_couleur_et_texture_direct(); break;}
 glEnd();
}

void alx_polygone_opengl::Rendu_couleur_direct() const
{alx_sommet_opengl *sommet;
 double *couleur, *coord;

 for(unsigned int i=0; i<nb_sommets; i++)
  {sommet = &(tab_sommets[ ordre_affichage[i] ]);
   couleur = sommet->coord_couleur;
   coord   = sommet->coord_espace;
   glColor4dv(couleur);
   glVertex3dv(coord);}
}

void alx_polygone_opengl::Rendu_texture_direct() const
{alx_sommet_opengl *sommet;
 double *text, *coord;

 for(unsigned int i=0; i<nb_sommets; i++)
  {sommet = &(tab_sommets[ ordre_affichage[i] ]);
   text   = sommet->coord_texture;
   coord  = sommet->coord_espace;
   glTexCoord2dv(text);
   glVertex3dv(coord);}
}

void alx_polygone_opengl::Rendu_couleur_et_texture_direct() const
{alx_sommet_opengl *sommet;
 double *couleur, *text, *coord;

 for(unsigned int i=0; i<nb_sommets; i++)
  {sommet = &(tab_sommets[ ordre_affichage[i] ]);
   couleur = sommet->coord_couleur;
   coord   = sommet->coord_espace;
   text    = sommet->coord_texture;
   glColor4dv(couleur);
   glTexCoord2dv(text);
   glVertex3dv(coord);}
}

//______________________________________________________________________________
void alx_polygone_opengl::Rendu_couleur() const
{alx_element_liste<int> *it_num = L_ordre_affichage.Premier();
 int nb_num = L_ordre_affichage.Taille(), pos;
 alx_sommet_opengl *sommet;
 double *couleur, *coord;

 for(int i=0; i<nb_num; i++)
  {pos = it_num->E();
   sommet = &(tab_sommets[pos]);
   couleur = sommet->coord_couleur;
   coord   = sommet->coord_espace;
   glColor4dv(couleur);
   glVertex3dv(coord);
   it_num = it_num->svt;}
}

void alx_polygone_opengl::Rendu_texture() const
{alx_element_liste<int> *it_num = L_ordre_affichage.Premier();
 int nb_num = L_ordre_affichage.Taille();
 alx_sommet_opengl *sommet;
 double *text, *coord;

 for(int i=0; i<nb_num; i++)
  {sommet = &(tab_sommets[it_num->E()]);
   text   = sommet->coord_texture;
   coord  = sommet->coord_espace;
   glTexCoord2dv(text);
   glVertex3dv(coord);
   it_num = it_num->svt;}
}

void alx_polygone_opengl::Rendu_couleur_et_texture() const
{alx_element_liste<int> *it_num = L_ordre_affichage.Premier();
 int nb_num = L_ordre_affichage.Taille();
 alx_sommet_opengl *sommet;
 double *couleur, *text, *coord;

 for(int i=0; i<nb_num; i++)
  {sommet = &(tab_sommets[it_num->E()]);
   couleur = sommet->coord_couleur;
   coord   = sommet->coord_espace;
   text   = sommet->coord_texture;
   glColor4dv(couleur);
   glTexCoord2dv(text);
   glVertex3dv(coord);
   it_num = it_num->svt;}
}

void alx_polygone_opengl::maj(alx_polygone2D *poly, const alx_liste<int> &L_aff)
{poly2D = poly;
 L_ordre_affichage = L_aff;
 nb_sommets  = poly->L_points.Taille();
 if(tab_sommets)     delete[] tab_sommets;
 if(ordre_affichage) delete[] ordre_affichage;

 tab_sommets     = new alx_sommet_opengl[nb_sommets];
 ordre_affichage = new unsigned int[nb_sommets];
 for(unsigned int i=0;i<nb_sommets;i++)
  {tab_sommets[i].coord_espace[2] = 0;
   ordre_affichage[i] = i;}
}

void alx_polygone_opengl::maj(const unsigned int nb_s)
{nb_sommets = nb_s;
 if(tab_sommets)     delete[] tab_sommets;
 if(ordre_affichage) delete[] ordre_affichage;
 tab_sommets = new alx_sommet_opengl[nb_sommets];
 ordre_affichage = new unsigned int[nb_sommets];
 for(unsigned int i=0;i<nb_sommets;i++)
  {tab_sommets[i].coord_espace[2] = 0;
   ordre_affichage[i] = i;
   }
}

//------------------------------------------------------------------------------
void alx_polygone_opengl::Affichage_du_polygone2D_par_triangles(GLenum mode, int type_rendu)
{if(poly2D) Raffraichir_polygl();
 glBegin(mode);
   switch(type_rendu)
    {case 1: Rendu_couleur(); break;
     case 2: Rendu_texture(); break;
     case 3: Rendu_couleur_et_texture(); break;}
 glEnd();
}


//------------------------------------------------------------------------------
void tracer_polygone_texture_couleur(alx_sommet_opengl *tab, int nb_sommets)
{glBegin (GL_POLYGON);
  for(int i=0;i<nb_sommets;i++)
   {glTexCoord2dv( tab[i].coord_texture );
    glColor4dv( tab[i].coord_couleur );
    glVertex3dv( tab[i].coord_espace );}
 glEnd();
}

//------------------------------------------------------------------------------
void tracer_polygone_texture(alx_sommet_opengl *tab, int nb_sommets)
{glBegin (GL_POLYGON);
  for(int i=0;i<nb_sommets;i++)
   {glTexCoord2dv( tab[i].coord_texture );
    glVertex3dv( tab[i].coord_espace );}
 glEnd();
}

//------------------------------------------------------------------------------
void tracer_polygone_couleur(alx_sommet_opengl *tab, int nb_sommets)
{glBegin (GL_POLYGON);
  for(int i=0;i<nb_sommets;i++)
   {glColor4dv( tab[i].coord_couleur );
    glVertex3dv( tab[i].coord_espace );}
 glEnd();
}

//------------------------------------------------------------------------------
alx_polygone_opengl* Generer_poly_gl( const alx_polygone2D &poly2D, const double profondeur
                                    , const double r, const double v, const double b, const double a // couleur de base
                                    , const double deb_x, const double taille_x    // pour les coordonnées textures
                                    , const double deb_y, const double taille_y )  // pour les coordonnées textures
{alx_element_liste<alx_point2D> *it_pt = poly2D.L_points.Premier();
 int nb_sommets = poly2D.L_points.Taille();
 alx_polygone_opengl *rep = new alx_polygone_opengl(nb_sommets);
 alx_sommet_opengl *tab = rep->tab_sommets;
 double x, y;

 for(int i=0;i<nb_sommets;i++)
  {tab[i].coord_espace[0] = x = (it_pt->E()).x;
   tab[i].coord_espace[1] = y = (it_pt->E()).y;
   tab[i].coord_espace[2] = profondeur;

   tab[i].coord_couleur[0] = r;
   tab[i].coord_couleur[1] = v;
   tab[i].coord_couleur[2] = b;
   tab[i].coord_couleur[3] = a;

   tab[i].coord_texture[0] = (x-deb_x)/taille_x;
   tab[i].coord_texture[1] = (y-deb_y)/taille_y;

   it_pt = it_pt->svt;
  }

 return rep;
}



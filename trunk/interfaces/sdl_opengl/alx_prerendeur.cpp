#include "alx_prerendeur.h"
#include "../../opengl/config_opengl.h"
#include "../../physique/math_alex.cpp"
#include "../alx_noeud_scene.h"

#include <extgl.h>
#include <glu.h>

//******************************************************************************
//-----------------------------------------------------------------------------*
//----------------------------- alx_prerendeur --------------------------------*
//-----------------------------------------------------------------------------*
//******************************************************************************
void pre_rendu::init()
{accumulateur = 0;}

pre_rendu::pre_rendu()
{init();
 noeud = NULL;
 priorite = -1;}

//------------------------------------------------------------------------------
pre_rendu::pre_rendu(alx_noeud_scene *noeud, const float priorite, const action_du_rendu action)
{init();
 this->noeud    = noeud;
 this->priorite = priorite;
 this->action   = action;}

//------------------------------------------------------------------------------
void pre_rendu::maj(const pre_rendu &pr)
{accumulateur = pr.accumulateur;
 noeud        = pr.noeud;
 priorite     = pr.priorite;
 action       = pr.action;}

//------------------------------------------------------------------------------
pre_rendu &pre_rendu::operator =(const pre_rendu &pr)  // Surcharge du  =
{maj(pr);
 return *this;}

//******************************************************************************
//-----------------------------------------------------------------------------*
//----------------------------- alx_prerendeur --------------------------------*
//-----------------------------------------------------------------------------*
//******************************************************************************

//------------------------------------------------------------------------------
//--------------------------- Les constructeurs --------------------------------
//------------------------------------------------------------------------------
alx_prerendeur::alx_prerendeur( alx_noeud_scene *noeud_a_rendre
                              , unsigned int ecran_x, unsigned int ecran_y, double angle_camera )
{this->noeud_a_rendre = noeud_a_rendre;
 maj_info_ecran(ecran_x, ecran_y, angle_camera);}

//------------------------------------------------------------------------------
//------------------------------ Les méthodes ----------------------------------
//------------------------------------------------------------------------------
void alx_prerendeur::Vider()
{L_pre_rendu.Vider();}

//------------------------------------------------------------------------------
void alx_prerendeur::Multiplier_priorite_par(const float f)
{alx_element_liste<pre_rendu> *it     = L_pre_rendu.Premier()
                            , *it_fin = L_pre_rendu.Fin();
 for(;it!=it_fin;it=it->svt)
  {(it->E()).priorite *= f;}
}

//------------------------------------------------------------------------------
void alx_prerendeur::simuler()
{
 if( noeud_a_rendre->Ne_pas_pre_rendre_fils()
   ||noeud_a_rendre->Ne_pas_pre_rendre() )
   return;
 noeud_a_rendre->PreRendre_fils();

// A la fin, retour à la vue ecran:
 glViewport(0, 0, ecran_x, ecran_y);
 glMatrixMode(GL_PROJECTION); // on rend active la matrice de projection
 glLoadIdentity(); // on reinitialise la matrice de projection
 gluPerspective(angle_camera,(double)(ecran_x)/(double)(ecran_y),0.0,10000.0);
 glMatrixMode(GL_MODELVIEW);
}

//------------------------------------------------------------------------------
void alx_prerendeur::maj_info_ecran(unsigned int ecran_x, unsigned int ecran_y, double angle_camera)
{
 this->ecran_x      = ecran_x;
 this->ecran_y      = ecran_y;
 this->angle_camera = angle_camera;
}



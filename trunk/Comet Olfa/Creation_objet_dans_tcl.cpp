#include <winsock.h>
#include "..\interfaces\interface.h"
#include "..\CamNote\CamNote.h"
#include "Creation_objet_dans_tcl.h"
#include "..\interfaces\classiques\alx_zone_texte.h"

#include "..\CamNote\CamNote.h"
#include "..\interfaces\classiques\alx_fiche_opengl.h"
#include "..\interfaces\classiques\alx_fiche.h"
#include "..\interfaces\classiques\alx_fiche_ovale_opengl.h"
#include "..\interfaces\classiques\alx_fiche_ovale.h"
#include "..\interfaces\sdl_opengl\alx_noeud_visualisation_camera.h"
#include "..\utilitaires\MetaData.h""
#include "..\interfaces\alx_simulateur_pointeurs_TCL.h"
#include "..\SWIG_TCL\alx_noeud_sdl_opengl_wrap.cxx"

static bool initialisation_faite = false;
alx_interpreteur_tcl *alx_interpreteur_cache;
Tcl_Interp *interpreteur_cache;

void Initialisation_commandes_tcl_interfaces(alx_interpreteur_tcl *interp)
{alx_interpreteur_cache = interp;
 interpreteur_cache = interp->Interpreteur_tcl();
 SWIG_init(interpreteur_cache);
 initialisation_faite = true;
}
//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
void Enregistrer(alx_interpreteur_tcl *interp, const char *nom_classe, const char *nom_var, const char *var_interne, const char *methode_acces)
{alx_chaine_char s_commande;
 s_commande  = nom_classe;
 s_commande += " ";
 s_commande += nom_var;
 resultat_interpreteur_tcl res_alx = interp->Evaluer(s_commande.Texte());

 s_commande  = "method ";
 s_commande += nom_classe;
 s_commande += " ";
 s_commande += methode_acces;
 s_commande += " {} {return ";
 s_commande += var_interne;
 s_commande += "}";
 res_alx = interp->Evaluer(s_commande.Texte());
}

//______________________________________________________________________________
void Constructeur_tcl(alx_interpreteur_tcl *interp, const char *c_spec, const char *param, const char *supl)
{
 alx_chaine_char s_commande;
 s_commande  = "method ";
 s_commande += c_spec;
 s_commande += " constructor { ";
 s_commande += param;
 s_commande += "} {this inherited; ";
 s_commande += supl;
 s_commande += "}";
 resultat_interpreteur_tcl res_alx = interp->Evaluer(s_commande.Texte());
 res_alx.reponse_int = 10;
}

//______________________________________________________________________________
void Enregistrer_interface(olfa_comet *comet, alx_interface_sdl_opengl_1 *i)
{alx_interpreteur_tcl *interp = comet->Interpreteur();
 if(interp)
  {//Constructeur_tcl(interp, (camnote->Classe_comet()).Texte(), "", "");
   Tcl_SetObjResult(interp->Interpreteur_tcl(),SWIG_Tcl_NewInstanceObj( interp->Interpreteur_tcl()
                                                                  , (void *)i
                                                                  , SWIGTYPE_p_alx_interface_sdl_opengl_1
                                                                  ,0) );
   const char *texte_res = Tcl_GetStringResult(interp->Interpreteur_tcl());
   alx_chaine_char str_tmp(texte_res);
  // Mise à jour du constructeur pour prendre en compte l'héritage
   Constructeur_tcl( interp
                   , (comet->Classe_comet()).Texte()
                   , "", "");
  // Création de l'objet gml qui, en outre, permettra l'accès à l'objet SWIG
   comet->Enregistrer_dans_TCL();
   Enregistrer_accesseur( interp
                        , (comet->Classe_comet()).Texte()
                        , (comet->Nom_usage()).Texte()
                        , str_tmp.Texte(), "Noeud_interface");
  }
}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
void Enregistre_dans(alx_interpreteur_tcl *interp, const alx_chaine_char &nom)
{alx_chaine_char str_tmp( Tcl_GetStringResult(interp->Interpreteur_tcl()) )
               , cmd("set "+nom+" "+str_tmp);
 interp->Evaluer(cmd);
}

//______________________________________________________________________________
void Enregistrer_alx_interpreteur_tcl(alx_interpreteur_tcl *interp_pere, alx_interpreteur_tcl *interp, const alx_chaine_char &nom)
{Tcl_SetObjResult(interp_pere->Interpreteur_tcl(),SWIG_Tcl_NewInstanceObj(interp_pere->Interpreteur_tcl(), (void *)interp, SWIGTYPE_p_alx_interpreteur_tcl,0));
 Enregistre_dans(interp, nom);}

//______________________________________________________________________________
void Enregistrer_image_opengl(alx_interpreteur_tcl *interp, const alx_image_opengl *img, const alx_chaine_char &nom)
{Tcl_SetObjResult(interp->Interpreteur_tcl(),SWIG_Tcl_NewInstanceObj(interp->Interpreteur_tcl(), (void *)img, SWIGTYPE_p_alx_image_opengl,0));
 Enregistre_dans(interp, nom);}

//______________________________________________________________________________
void Enregistrer_noeud_bigre(alx_interpreteur_tcl *interp, const alx_noeud_scene *noeud, const alx_chaine_char &nom)
{Tcl_SetObjResult(interp->Interpreteur_tcl(),SWIG_Tcl_NewInstanceObj(interp->Interpreteur_tcl(), (void *)noeud, SWIGTYPE_p_alx_noeud_scene,0));
 Enregistre_dans(interp, nom);}

//______________________________________________________________________________
void Enregistrer_fenetre_bigre(alx_interpreteur_tcl *interp, const alx_noeud_fenetre_sdl_opengl *fen, const alx_chaine_char &nom)
{Tcl_SetObjResult(interp->Interpreteur_tcl(),SWIG_Tcl_NewInstanceObj(interp->Interpreteur_tcl(), (void *)fen, SWIGTYPE_p_alx_noeud_fenetre_sdl_opengl,0));
 Enregistre_dans(interp, nom);}

//______________________________________________________________________________
void Enregistrer_zone_texte_bigre(alx_interpreteur_tcl *interp, const alx_noeud_zone_texte_sdl_opengl *zt, const alx_chaine_char &nom)
{Tcl_SetObjResult(interp->Interpreteur_tcl(),SWIG_Tcl_NewInstanceObj(interp->Interpreteur_tcl(), (void *)zt, SWIGTYPE_p_alx_noeud_zone_texte_sdl_opengl,0));
 Enregistre_dans(interp, nom);}
//______________________________________________________________________________
void Enregistrer_image_bigre(alx_interpreteur_tcl *interp, const alx_noeud_image_sdl_opengl *img, const alx_chaine_char &nom)
{Tcl_SetObjResult(interp->Interpreteur_tcl(),SWIG_Tcl_NewInstanceObj(interp->Interpreteur_tcl(), (void *)img, SWIGTYPE_p_alx_noeud_image_sdl_opengl,0));
 Enregistre_dans(interp, nom);}

//______________________________________________________________________________
void Enregistrer_visualisation_camera(alx_interpreteur_tcl *interp, const alx_noeud_visualisation_camera *cam, const alx_chaine_char &nom)
{Tcl_SetObjResult(interp->Interpreteur_tcl(),SWIG_Tcl_NewInstanceObj(interp->Interpreteur_tcl(), (void *)cam, SWIGTYPE_p_alx_noeud_visualisation_camera,0));
 Enregistre_dans(interp, nom);}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
void Enregistrer_accesseur( alx_interpreteur_tcl *interp
                          , const char *nom_classe
                          , const char *nom_var
                          , const char *nom_object_accede
                          , const char *nom_methode_acces)
{// Enregistrer les méthodes de modifications de l'objet accédé
 alx_chaine_char s_commande;
   s_commande  = "method "; s_commande += nom_classe; s_commande += " get_";
   s_commande += nom_methode_acces; s_commande += " {} {return $this(objet_accede)}";
 resultat_interpreteur_tcl res_alx = interp->Evaluer(s_commande.Texte());
   s_commande  = "method "; s_commande += nom_classe; s_commande += " set_";
   s_commande += nom_methode_acces; s_commande += " {o} {set this(objet_accede) $o}";
 res_alx = interp->Evaluer(s_commande.Texte());
   s_commande  = nom_var; s_commande += " set_"; s_commande += nom_methode_acces; s_commande += " "; s_commande += nom_object_accede;
 res_alx = interp->Evaluer(s_commande.Texte());
}

//______________________________________________________________________________
void Enregistrer_CamNote(olfa_comet *comet, CamNote *camnote)
{alx_interpreteur_tcl *interp = comet->Interpreteur();
 if(interp)
  {Tcl_SetObjResult(interp->Interpreteur_tcl(),SWIG_Tcl_NewInstanceObj(interp->Interpreteur_tcl(), (void *)camnote, SWIGTYPE_p_CamNote,0));
   const char *texte_res = Tcl_GetStringResult(interp->Interpreteur_tcl());
   alx_chaine_char str_tmp(texte_res);
  // Mise à jour du constructeur pour prendre en compte l'héritage
   Constructeur_tcl( interp
                   , (comet->Classe_comet()).Texte()
                   , "", "");
  // Création de l'objet gml qui, en outre, permettra l'accès à l'objet SWIG
   comet->Enregistrer_dans_TCL();
   Enregistrer_accesseur( interp
                        , (comet->Classe_comet()).Texte()
                        , (comet->Nom_usage()).Texte()
                        , str_tmp.Texte(), "CamNote_bigre");
  }
}

//______________________________________________________________________________
void Enregistrer_CamNote_Visu(olfa_comet *comet, alx_visu_image_camnote_cpp_bigre *visu)
{alx_interpreteur_tcl *interp = comet->Interpreteur();
 if(interp)
  {
   //Constructeur_tcl(interp, (camnote->Classe_comet()).Texte(), "", "");
   Tcl_SetObjResult(interp->Interpreteur_tcl(),SWIG_Tcl_NewInstanceObj(interp->Interpreteur_tcl(), (void *)visu, SWIGTYPE_p_alx_visu_image_camnote_cpp_bigre,0));
   const char *texte_res = Tcl_GetStringResult(interp->Interpreteur_tcl());
   alx_chaine_char str_tmp(texte_res);
  // Mise à jour du constructeur pour prendre en compte l'héritage
   Constructeur_tcl( interp
                   , (comet->Classe_comet()).Texte()
                   , "", "");
  // Création de l'objet gml qui, en outre, permettra l'accès à l'objet SWIG
   comet->Enregistrer_dans_TCL();
   Enregistrer_accesseur( interp
                        , (comet->Classe_comet()).Texte()
                        , (comet->Nom_usage()).Texte()
                        , str_tmp.Texte(), "CamVisu_bigre");
  }
}

//______________________________________________________________________________
void Enregistrer_CamNote_Tele(olfa_comet *comet, alx_telec_camnote_cpp_bigre *tele)
{alx_interpreteur_tcl *interp = comet->Interpreteur();
 if(interp)
  {
   //Constructeur_tcl(interp, (camnote->Classe_comet()).Texte(), "", "");
   Tcl_SetObjResult(interp->Interpreteur_tcl(),SWIG_Tcl_NewInstanceObj(interp->Interpreteur_tcl(), (void *)tele, SWIGTYPE_p_alx_telec_camnote_cpp_bigre,0));
   const char *texte_res = Tcl_GetStringResult(interp->Interpreteur_tcl());
   alx_chaine_char str_tmp(texte_res);
  // Mise à jour du constructeur pour prendre en compte l'héritage
   Constructeur_tcl( interp
                   , (comet->Classe_comet()).Texte()
                   , "", "");
  // Création de l'objet gml qui, en outre, permettra l'accès à l'objet SWIG
   comet->Enregistrer_dans_TCL();
   Enregistrer_accesseur( interp
                        , (comet->Classe_comet()).Texte()
                        , (comet->Nom_usage()).Texte()
                        , str_tmp.Texte(), "CamTele_bigre");
  }
}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
comet_noeud_scene::comet_noeud_scene( const char *nom_usage
                                    /*, cogitant::Environment *e*/) : olfa_comet( nom_usage
                                                                  //               , e
                                                                                 , alx_interpreteur_cache)
{
 if( !initialisation_faite || !interpreteur_cache) return ;
 Constructeur_tcl(alx_interpreteur_cache, Classe_comet().Texte(), "", "");
 Tcl_SetObjResult(interpreteur_cache,SWIG_Tcl_NewInstanceObj(interpreteur_cache, (void *)this, SWIGTYPE_p_comet_noeud_scene,0));
 const char *texte_res = Tcl_GetStringResult(interpreteur_cache);
 nom_interne = texte_res;
 Enregistrer(alx_interpreteur_cache, Classe_comet().Texte(), Nom_usage().Texte(), nom_interne.Texte(), "Noeud_scene");
}




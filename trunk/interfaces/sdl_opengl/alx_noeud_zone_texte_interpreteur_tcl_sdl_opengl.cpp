#include "alx_noeud_zone_texte_interpreteur_tcl_sdl_opengl.h"
#include <sdl.h>
#include "..\..\tcl\alx_interpreteur_tcl.h"

alx_noeud_zone_texte_interpreteur_tcl_sdl_opengl::alx_noeud_zone_texte_interpreteur_tcl_sdl_opengl( alx_zone_texte_opengl *zgl
                                                                                                  , alx_interpreteur_tcl *interpreteur) : alx_noeud_zone_texte_sdl_opengl(zgl)
{interpreteur_tcl = interpreteur;
 interpreteur_tcl->Texte_edition( this->Texte() );
 zgl->abonner_a_caractere_tape( FONCTION_DE_RAPPEL( alx_noeud_zone_texte_interpreteur_tcl_sdl_opengl
                                                  , &alx_noeud_zone_texte_interpreteur_tcl_sdl_opengl::Caractere_tape) );
}

void alx_noeud_zone_texte_interpreteur_tcl_sdl_opengl::Caractere_tape(void *param)
{//const alx_chaine_char &txt = Chaine_char();
 int car = *( (int*)param );

 switch( (char)car )
  {case SDLK_KP_ENTER:
   case SDLK_RETURN:
     const char *commande = Zone_texte_gl()->Derniere_phrase();
    // On regarde si ça n'est pas une de mes commandes de la mort
     if( !strcmp("cls", commande) )
      {Maj_texte("");
       Zone_texte_gl()->Positionner_curseur_a_la_fin();
       break;}

     if( !strcmp("Charger_prop", commande) )
      {//*(Texte()) = "";
       interpreteur_tcl->Ecrire( (interpreteur_tcl->Evaluer("source $chemin_propositions")).reponse_texte );
       break;}

    // Si c'est pas le cas on file ça à l'interpréteur TCL
     resultat_interpreteur_tcl res( interpreteur_tcl->Evaluer( commande ) );
    Ecrire(res.reponse_texte);
    Ecrire("\n");
    Zone_texte_gl()->Positionner_curseur_a_la_fin();
   break;
  }

 Informer_que_caractere_tape(car);
}


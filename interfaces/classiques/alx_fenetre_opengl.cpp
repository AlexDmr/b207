#include "alx_fenetre_opengl.h"
#include "..\..\opengl\config_opengl.h"
#include "..\..\opengl\texte.h"
#include <glew.h>

alx_fenetre_opengl::alx_fenetre_opengl(alx_fenetre *f, gl_fonte *fonte)
{
 fenetre      = f;
 fonte_entete = fonte;
 zdr_entete = NULL;

 couleur_corps[0] = 1;
 couleur_corps[0] = 1;
 couleur_corps[0] = 1;
 couleur_corps[0] = 1;

 couleur_bordure_ext_angle[0] = 0;
 couleur_bordure_ext_angle[1] = 1;
 couleur_bordure_ext_angle[2] = 1;
 couleur_bordure_ext_angle[3] = 1;

 couleur_bordure_ext[0] = 1;
 couleur_bordure_ext[1] = 1;
 couleur_bordure_ext[2] = 1;
 couleur_bordure_ext[3] = 1;

 couleur_bordure_int_angle[0] = 0;
 couleur_bordure_int_angle[1] = 0;
 couleur_bordure_int_angle[2] = 0;
 couleur_bordure_int_angle[3] = 1;

 couleur_bordure_int[0] = 0;
 couleur_bordure_int[1] = 0;
 couleur_bordure_int[2] = 0;
 couleur_bordure_int[3] = 1;

 couleur_entete[0][0] = 0;
 couleur_entete[0][1] = 0.5;
 couleur_entete[0][2] = 0.5;
 couleur_entete[0][3] = 1;
 Copie_couleur(couleur_entete[0], couleur_entete[3]);
 couleur_entete[1][0] = 0;
 couleur_entete[1][1] = 1;
 couleur_entete[1][2] = 1;
 couleur_entete[1][3] = 1;
 Copie_couleur(couleur_entete[1], couleur_entete[2]);

 Couleur_masque_bord(1,1,1,1);
 Couleur_masque_corp(1,1,1,1);
}


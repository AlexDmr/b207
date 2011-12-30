
#include "alx_noeud_polygone_sdl_opengl.h"
//#include "../../GPC/gpc.cpp"
#include "../../opengl/config_opengl.h"
//#include <glut.h>
#include <glew.h>
#include "../../physique/math_alex.cpp"
extern "C" {
 #include <glu.h>
// #include <glut.h>
}

unsigned int  alx_noeud_polygone_sdl_opengl::nb_elements_ordre_affichage = 1;
unsigned int* alx_noeud_polygone_sdl_opengl::ordre_affichage_poly        = new unsigned int [1];

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
void CALLBACK vertexTexture2Callback(GLvoid *vertex)  // COORD[3]TEXTURE[2]TEXTURE[2]
{GLdouble *ptr;
 ptr = (GLdouble *) vertex;
 glMultiTexCoord2dvARB(GL_TEXTURE0_ARB, &(ptr[3]));
 glMultiTexCoord2dvARB(GL_TEXTURE1_ARB, &(ptr[5]));
 glVertex2dv( ptr);
}
//______________________________________________________________________________
void CALLBACK vertexTextureCallback(GLvoid *vertex)  // COORD[3]TEXTURE[2]
{GLdouble *ptr;
 ptr = (GLdouble *) vertex;
 glTexCoord2dv( &(ptr[3]) );
 glVertex2dv( ptr);
}
//______________________________________________________________________________
void CALLBACK vertexCouleurCallback(GLvoid *vertex)  // COORD[3]TEXTURE[2]
{GLdouble *ptr;
 ptr = (GLdouble *) vertex;
 glColor4dv( &(ptr[3]) );
 glVertex2dv( ptr);
}
//______________________________________________________________________________
void CALLBACK vertexCouleurTexture2Callback(GLvoid *vertex)  // COORD[3]TEXTURE[2]
{GLdouble *ptr;
 ptr = (GLdouble *) vertex;
 glColor4dv( &(ptr[5]) );
 glMultiTexCoord2dvARB(GL_TEXTURE0_ARB, &(ptr[3]));
 glMultiTexCoord2dvARB(GL_TEXTURE1_ARB, &(ptr[9]));
 glVertex2dv( ptr);
}
//______________________________________________________________________________
void CALLBACK vertexCouleurTextureCallback(GLvoid *vertex)  // COORD[3]TEXTURE[2]
{GLdouble *ptr;
 ptr = (GLdouble *) vertex;
 glColor4dv( &(ptr[5]) );
 glTexCoord2dv( &(ptr[3]) );
 glVertex2dv( ptr);
}
//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
void CALLBACK combineCouleurCallback(GLdouble coords[3], GLdouble *vertex_data[4], GLfloat weight[4], GLdouble **dataOut) // COORD[3]TEXTURE[2]
{GLdouble *vertex;
 vertex = new GLdouble[7];
 vertex[0] = coords[0];
 vertex[1] = coords[1];
 vertex[2] = coords[2];
 //printf("vertexTextureCallback( coord(%f, %f, %f), vertex_data(%h, %h, %h, %h)\n", coords[0], coords[1], coords[2], vertex_data[0], vertex_data[1], vertex_data[2], vertex_data[3]);
 for (int i = 3; i < 7; i++) // Pour chaque canal couleur
  {vertex[i] = 0;
   for(unsigned int j=0; j<4; j++)
     if(weight[j] > 0) vertex[i] += weight[j] * vertex_data[j][i];
  }

 *dataOut = vertex;
}

//______________________________________________________________________________
void CALLBACK combineCouleurTexture2Callback(GLdouble coords[3], GLdouble *vertex_data[4], GLfloat weight[4], GLdouble **dataOut) // COORD[3]TEXTURE[2]
{GLdouble *vertex;
 vertex =  new GLdouble[11];
 vertex[0] = coords[0];
 vertex[1] = coords[1];
 vertex[2] = coords[2];
 //printf("vertexTextureCallback( coord(%f, %f, %f), vertex_data(%h, %h, %h, %h)\n", coords[0], coords[1], coords[2], vertex_data[0], vertex_data[1], vertex_data[2], vertex_data[3]);
 for (int i = 3; i < 11; i++) // Pour chaque canal couleur et chaque canal texture
  {vertex[i] = 0;
   for(unsigned int j=0; j<4; j++)
     if(weight[j] > 0) vertex[i] += weight[j] * vertex_data[j][i];
  }

 *dataOut = vertex;
}
//______________________________________________________________________________
void CALLBACK combineCouleurTextureCallback(GLdouble coords[3], GLdouble *vertex_data[4], GLfloat weight[4], GLdouble **dataOut) // COORD[3]TEXTURE[2]
{GLdouble *vertex;
 vertex =  new GLdouble[9];
 vertex[0] = coords[0];
 vertex[1] = coords[1];
 vertex[2] = coords[2];
 //printf("vertexTextureCallback( coord(%f, %f, %f), vertex_data(%h, %h, %h, %h)\n", coords[0], coords[1], coords[2], vertex_data[0], vertex_data[1], vertex_data[2], vertex_data[3]);
 for (int i = 3; i < 9; i++) // Pour chaque canal couleur et chaque canal texture
  {vertex[i] = 0;
   for(unsigned int j=0; j<4; j++)
     if(weight[j] > 0) vertex[i] += weight[j] * vertex_data[j][i];
  }

 *dataOut = vertex;
}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
void CALLBACK combineCallback(GLdouble coords[3], GLdouble *vertex_data[4], GLfloat weight[4], GLdouble **dataOut)
{GLdouble *vertex;
 vertex =  new GLdouble[3];
 vertex[0] = coords[0];
 vertex[1] = coords[1];
 vertex[2] = coords[2];
// vertex[2] = coords[2];
// printf("Ajout du point (%f, %f)", vertex[0], vertex[1]);
 *dataOut = vertex;
}

//______________________________________________________________________________
void CALLBACK combineTexture2Callback(GLdouble coords[3], GLdouble *vertex_data[4], GLfloat weight[4], GLdouble **dataOut) // COORD[3]TEXTURE[2]
{GLdouble *vertex;
 vertex =  new GLdouble[7];
 vertex[0] = coords[0];
 vertex[1] = coords[1];
 vertex[2] = coords[2];
 //printf("vertexTextureCallback( coord(%f, %f, %f), vertex_data(%h, %h, %h, %h)\n", coords[0], coords[1], coords[2], vertex_data[0], vertex_data[1], vertex_data[2], vertex_data[3]);
 for (int i = 3; i < 7; i++)
  {vertex[i] = 0;
   for(unsigned int j=0; j<4; j++)
     if(weight[j] > 0) vertex[i] += weight[j] * vertex_data[j][i];
  }
// vertex[2] = coords[2];
// printf("Ajout du point (%f, %f)", vertex[0], vertex[1]);
 *dataOut = vertex;
}

//______________________________________________________________________________
void CALLBACK combineTextureCallback(GLdouble coords[3], GLdouble *vertex_data[4], GLfloat weight[4], GLdouble **dataOut) // COORD[3]TEXTURE[2]
{GLdouble *vertex;
 vertex =  new GLdouble[5];
 vertex[0] = coords[0];
 vertex[1] = coords[1];
 vertex[2] = coords[2];
 //printf("vertexTextureCallback( coord(%f, %f, %f), vertex_data(%h, %h, %h, %h)\n", coords[0], coords[1], coords[2], vertex_data[0], vertex_data[1], vertex_data[2], vertex_data[3]);
 for (int i = 3; i < 5; i++)
  {vertex[i] = 0;
   for(unsigned int j=0; j<4; j++)
     if(weight[j] > 0) vertex[i] += weight[j] * vertex_data[j][i];
  }
// vertex[2] = coords[2];
// printf("Ajout du point (%f, %f)", vertex[0], vertex[1]);
 *dataOut = vertex;
}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
void alx_noeud_polygone_sdl_opengl::init_ptr()
{P_void *f_diff   = classe_seri_poly.f_diff
      , *f_deseri = classe_seri_poly.f_deseri;

/* classe_seri_poly.fd1[0]   = &(alx_noeud_polygone_sdl_opengl::Hauteur);    f_deseri[1] = &(classe_seri_poly.fd1[0]);
 classe_seri_poly.fsd_d1[0]= &(alx_noeud_polygone_sdl_opengl::Hauteur);    f_diff[1]   = &(classe_seri_poly.fsd_d1[0]);
 classe_seri_poly.fd1[1]   = &(alx_noeud_polygone_sdl_opengl::Largeur);    f_deseri[2] = &(classe_seri_poly.fd1[1]);
 classe_seri_poly.fsd_d1[1]= &(alx_noeud_polygone_sdl_opengl::Largeur);    f_diff[2]   = &(classe_seri_poly.fsd_d1[1]);
 classe_seri_poly.fd1[2]   = &(alx_noeud_polygone_sdl_opengl::Zoom);       f_deseri[3] = &(classe_seri_poly.fd1[2]);
 classe_seri_poly.fsd_d1[2]= &(alx_noeud_polygone_sdl_opengl::Zoom);       f_diff[3]   = &(classe_seri_poly.fsd_d1[2]);*/

/* classe_seri_poly.fb1[0]   = &(alx_noeud_polygone_sdl_opengl::Translucidite); f_deseri[4] = &(classe_seri_poly.fb1[0]);
 classe_seri_poly.fsd_b1[0]= &(alx_noeud_polygone_sdl_opengl::Translucidite); f_diff[4]   = &(classe_seri_poly.fsd_b1[0]);
 classe_seri_poly.fb1[1]   = &(alx_noeud_polygone_sdl_opengl::Translucidite_perimetre); f_deseri[5] = &(classe_seri_poly.fb1[1]);
 classe_seri_poly.fsd_b1[1]= &(alx_noeud_polygone_sdl_opengl::Translucidite_perimetre); f_diff[5]   = &(classe_seri_poly.fsd_b1[1]);
 classe_seri_poly.fb1[2]   = &(alx_noeud_polygone_sdl_opengl::Lisser); f_deseri[6] = &(classe_seri_poly.fb1[2]);
 classe_seri_poly.fsd_b1[2]= &(alx_noeud_polygone_sdl_opengl::Lisser); f_diff[6]   = &(classe_seri_poly.fsd_b1[2]);
 classe_seri_poly.fb1[3]   = &(alx_noeud_polygone_sdl_opengl::Polygone_a_afficher); f_deseri[7] = &(classe_seri_poly.fb1[3]);
 classe_seri_poly.fsd_b1[3]= &(alx_noeud_polygone_sdl_opengl::Polygone_a_afficher); f_diff[7]   = &(classe_seri_poly.fsd_b1[3]);
 classe_seri_poly.fb1[4]   = &(alx_noeud_polygone_sdl_opengl::Perimetre_a_afficher); f_deseri[8] = &(classe_seri_poly.fb1[4]);
 classe_seri_poly.fsd_b1[4]= &(alx_noeud_polygone_sdl_opengl::Perimetre_a_afficher); f_diff[8]   = &(classe_seri_poly.fsd_b1[4]);
*/
 classe_seri_poly.fcc[0]   = &alx_noeud_polygone_sdl_opengl::Deseri_info_texture_1;               ; f_deseri[6] = &(classe_seri_poly.fcc[0]);
 classe_seri_poly.fsd_cc[0]= &alx_noeud_polygone_sdl_opengl::Seri_diff_info_texture_1_a_ete_change; f_diff[6]   = &(classe_seri_poly.fsd_cc[0]);
 classe_seri_poly.fcc[1]   = &alx_noeud_polygone_sdl_opengl::Deseri_info_texture_2;               ; f_deseri[7] = &(classe_seri_poly.fcc[1]);
 classe_seri_poly.fsd_cc[1]= &alx_noeud_polygone_sdl_opengl::Seri_diff_info_texture_2_a_ete_change; f_diff[7]   = &(classe_seri_poly.fsd_cc[1]);

 classe_seri_poly.fi1[0]    = &alx_noeud_polygone_sdl_opengl::Marqueur_attributs_poly; f_deseri[8] = &(classe_seri_poly.fi1[0]);
 classe_seri_poly.fsd_i1[0] = &alx_noeud_polygone_sdl_opengl::Marqueur_attributs_poly; f_diff[8]   = &(classe_seri_poly.fsd_i1[0]);

 classe_seri_poly.fd4[0]   = &alx_noeud_polygone_sdl_opengl::Couleur;           f_deseri[9] = &(classe_seri_poly.fd4[0]);
 classe_seri_poly.fsd_d4[0]= &alx_noeud_polygone_sdl_opengl::Couleur;           f_diff[9]   = &(classe_seri_poly.fsd_d4[0]);
 classe_seri_poly.fd4[1]   = &alx_noeud_polygone_sdl_opengl::Couleur_perimetre; f_deseri[10] = &(classe_seri_poly.fd4[1]);
 classe_seri_poly.fsd_d4[1]= &alx_noeud_polygone_sdl_opengl::Couleur_perimetre; f_diff[10]   = &(classe_seri_poly.fsd_d4[1]);

 classe_seri_poly.fd_poly [0] = &alx_noeud_polygone_sdl_opengl::Maj_poly_gpc;          f_deseri[11] = &(classe_seri_poly.fd_poly [0]);
 classe_seri_poly.fsd_poly[0] = &alx_noeud_polygone_sdl_opengl::A_ete_modifie;         f_diff  [11] = &(classe_seri_poly.fsd_poly[0]);

 classe_seri_poly.fd4[2]   = &alx_noeud_polygone_sdl_opengl::Const_couleur_d; f_deseri[12] = &(classe_seri_poly.fd4[2]);
 classe_seri_poly.fsd_d4[2]= &alx_noeud_polygone_sdl_opengl::Const_couleur_d; f_diff[12]   = &(classe_seri_poly.fsd_d4[2]);

 classe_seri_poly.fd1[0]   = &alx_noeud_polygone_sdl_opengl::Alpha_scale_d; f_deseri[13] = &(classe_seri_poly.fd1[0]);
 classe_seri_poly.fsd_d1[0]= &alx_noeud_polygone_sdl_opengl::Alpha_scale_d; f_diff[13]   = &(classe_seri_poly.fsd_d1[0]);
 classe_seri_poly.fd1[1]   = &alx_noeud_polygone_sdl_opengl::RGB_scale_d  ; f_deseri[14] = &(classe_seri_poly.fd1[1]);
 classe_seri_poly.fsd_d1[1]= &alx_noeud_polygone_sdl_opengl::RGB_scale_d  ; f_diff[14]   = &(classe_seri_poly.fsd_d1[1]);
 classe_seri_poly.fd1[2]   = &alx_noeud_polygone_sdl_opengl::Constante_combinaison_couleurs_d  ; f_deseri[15] = &(classe_seri_poly.fd1[2]);
 classe_seri_poly.fsd_d1[2]= &alx_noeud_polygone_sdl_opengl::Constante_combinaison_couleurs_d  ; f_diff[15]   = &(classe_seri_poly.fsd_d1[2]);
 classe_seri_poly.fd1[3]   = &alx_noeud_polygone_sdl_opengl::Constante_combinaison_textures_d  ; f_deseri[16] = &(classe_seri_poly.fd1[3]);
 classe_seri_poly.fsd_d1[3]= &alx_noeud_polygone_sdl_opengl::Constante_combinaison_textures_d  ; f_diff[16]   = &(classe_seri_poly.fsd_d1[3]);

 classe_seri_poly.fi1[1]    = &alx_noeud_polygone_sdl_opengl::Combinaison_couleurs_source_arg2 ; f_deseri[17] = &(classe_seri_poly.fi1[1]);
 classe_seri_poly.fsd_i1[1] = &alx_noeud_polygone_sdl_opengl::Combinaison_couleurs_source_arg2 ; f_diff[17]   = &(classe_seri_poly.fsd_i1[1]);
 classe_seri_poly.fi1[2]    = &alx_noeud_polygone_sdl_opengl::Combinaison_couleurs_operande2   ; f_deseri[18] = &(classe_seri_poly.fi1[2]);
 classe_seri_poly.fsd_i1[2] = &alx_noeud_polygone_sdl_opengl::Combinaison_couleurs_operande2   ; f_diff[18]   = &(classe_seri_poly.fsd_i1[2]);
 classe_seri_poly.fi1[3]    = &alx_noeud_polygone_sdl_opengl::Combinaison_textures_source_arg2 ; f_deseri[19] = &(classe_seri_poly.fi1[3]);
 classe_seri_poly.fsd_i1[3] = &alx_noeud_polygone_sdl_opengl::Combinaison_textures_source_arg2 ; f_diff[19]   = &(classe_seri_poly.fsd_i1[3]);
 classe_seri_poly.fi1[4]    = &alx_noeud_polygone_sdl_opengl::Combinaison_textures_operande2   ; f_deseri[20] = &(classe_seri_poly.fi1[4]);
 classe_seri_poly.fsd_i1[4] = &alx_noeud_polygone_sdl_opengl::Combinaison_textures_operande2   ; f_diff[20]   = &(classe_seri_poly.fsd_i1[4]);
 classe_seri_poly.fi1[5]    = &alx_noeud_polygone_sdl_opengl::Ordre_application_textures_rgb   ; f_deseri[21] = &(classe_seri_poly.fi1[5]);
 classe_seri_poly.fsd_i1[5] = &alx_noeud_polygone_sdl_opengl::Ordre_application_textures_rgb   ; f_diff[21] = &(classe_seri_poly.fsd_i1[5]);
 classe_seri_poly.fi1[6]    = &alx_noeud_polygone_sdl_opengl::Ordre_application_textures_alpha ; f_deseri[22] = &(classe_seri_poly.fi1[6]);
 classe_seri_poly.fsd_i1[6] = &alx_noeud_polygone_sdl_opengl::Ordre_application_textures_alpha ; f_diff[22] = &(classe_seri_poly.fsd_i1[6]);
 classe_seri_poly.fi1[7]    = &alx_noeud_polygone_sdl_opengl::Combinaison_textures_rgb         ; f_deseri[23] = &(classe_seri_poly.fi1[7]);
 classe_seri_poly.fsd_i1[7] = &alx_noeud_polygone_sdl_opengl::Combinaison_textures_rgb         ; f_diff[23] = &(classe_seri_poly.fsd_i1[7]);
 classe_seri_poly.fi1[8]    = &alx_noeud_polygone_sdl_opengl::Combinaison_textures_alpha       ; f_deseri[24] = &(classe_seri_poly.fi1[8]);
 classe_seri_poly.fsd_i1[8] = &alx_noeud_polygone_sdl_opengl::Combinaison_textures_alpha       ; f_diff[24] = &(classe_seri_poly.fsd_i1[8]);
 classe_seri_poly.fi1[9]    = &alx_noeud_polygone_sdl_opengl::Ordre_application_couleurs_rgb   ; f_deseri[25] = &(classe_seri_poly.fi1[9]);
 classe_seri_poly.fsd_i1[9] = &alx_noeud_polygone_sdl_opengl::Ordre_application_couleurs_rgb   ; f_diff[25] = &(classe_seri_poly.fsd_i1[9]);
 classe_seri_poly.fi1[10]    = &alx_noeud_polygone_sdl_opengl::Ordre_application_couleurs_alpha; f_deseri[26] = &(classe_seri_poly.fi1[10]);
 classe_seri_poly.fsd_i1[10] = &alx_noeud_polygone_sdl_opengl::Ordre_application_couleurs_alpha; f_diff[26] = &(classe_seri_poly.fsd_i1[10]);
 classe_seri_poly.fi1[11]    = &alx_noeud_polygone_sdl_opengl::Combinaison_couleurs_rgb        ; f_deseri[27] = &(classe_seri_poly.fi1[11]);
 classe_seri_poly.fsd_i1[11] = &alx_noeud_polygone_sdl_opengl::Combinaison_couleurs_rgb        ; f_diff[27] = &(classe_seri_poly.fsd_i1[11]);
 classe_seri_poly.fi1[12]    = &alx_noeud_polygone_sdl_opengl::Combinaison_couleurs_alpha      ; f_deseri[28] = &(classe_seri_poly.fi1[12]);
 classe_seri_poly.fsd_i1[12] = &alx_noeud_polygone_sdl_opengl::Combinaison_couleurs_alpha      ; f_diff[28] = &(classe_seri_poly.fsd_i1[12]);

 classe_seri_poly.fi1[13]    = &alx_noeud_polygone_sdl_opengl::Combinaison_couleurs_source_arg0; f_deseri[29] = &(classe_seri_poly.fi1[13]);
 classe_seri_poly.fsd_i1[13] = &alx_noeud_polygone_sdl_opengl::Combinaison_couleurs_source_arg0; f_diff[29]   = &(classe_seri_poly.fsd_i1[13]);
 classe_seri_poly.fi1[14]    = &alx_noeud_polygone_sdl_opengl::Combinaison_couleurs_operande0; f_deseri[30] = &(classe_seri_poly.fi1[14]);
 classe_seri_poly.fsd_i1[14] = &alx_noeud_polygone_sdl_opengl::Combinaison_couleurs_operande0; f_diff[30]   = &(classe_seri_poly.fsd_i1[14]);
 classe_seri_poly.fi1[15]    = &alx_noeud_polygone_sdl_opengl::Combinaison_couleurs_source_arg1; f_deseri[31] = &(classe_seri_poly.fi1[15]);
 classe_seri_poly.fsd_i1[15] = &alx_noeud_polygone_sdl_opengl::Combinaison_couleurs_source_arg1; f_diff[31]   = &(classe_seri_poly.fsd_i1[15]);
 classe_seri_poly.fi1[16]    = &alx_noeud_polygone_sdl_opengl::Combinaison_couleurs_operande1; f_deseri[32] = &(classe_seri_poly.fi1[16]);
 classe_seri_poly.fsd_i1[16] = &alx_noeud_polygone_sdl_opengl::Combinaison_couleurs_operande1; f_diff[32]   = &(classe_seri_poly.fsd_i1[16]);
 classe_seri_poly.fi1[17]    = &alx_noeud_polygone_sdl_opengl::Combinaison_couleurs_source_arg2; f_deseri[33] = &(classe_seri_poly.fi1[17]);
 classe_seri_poly.fsd_i1[17] = &alx_noeud_polygone_sdl_opengl::Combinaison_couleurs_source_arg2; f_diff[33]   = &(classe_seri_poly.fsd_i1[17]);
 classe_seri_poly.fi1[18]    = &alx_noeud_polygone_sdl_opengl::Combinaison_couleurs_operande2; f_deseri[34] = &(classe_seri_poly.fi1[18]);
 classe_seri_poly.fsd_i1[18] = &alx_noeud_polygone_sdl_opengl::Combinaison_couleurs_operande2; f_diff[34]   = &(classe_seri_poly.fsd_i1[18]);
 classe_seri_poly.fi1[19]    = &alx_noeud_polygone_sdl_opengl::Combinaison_couleurs_source_arg0_Alpha; f_deseri[35] = &(classe_seri_poly.fi1[19]);
 classe_seri_poly.fsd_i1[19] = &alx_noeud_polygone_sdl_opengl::Combinaison_couleurs_source_arg0_Alpha; f_diff[35]   = &(classe_seri_poly.fsd_i1[19]);
 classe_seri_poly.fi1[20]    = &alx_noeud_polygone_sdl_opengl::Combinaison_couleurs_operande0_Alpha; f_deseri[36] = &(classe_seri_poly.fi1[20]);
 classe_seri_poly.fsd_i1[20] = &alx_noeud_polygone_sdl_opengl::Combinaison_couleurs_operande0_Alpha; f_diff[36]   = &(classe_seri_poly.fsd_i1[20]);
 classe_seri_poly.fi1[21]    = &alx_noeud_polygone_sdl_opengl::Combinaison_couleurs_source_arg1_Alpha; f_deseri[37] = &(classe_seri_poly.fi1[21]);
 classe_seri_poly.fsd_i1[21] = &alx_noeud_polygone_sdl_opengl::Combinaison_couleurs_source_arg1_Alpha; f_diff[37]   = &(classe_seri_poly.fsd_i1[21]);
 classe_seri_poly.fi1[22]    = &alx_noeud_polygone_sdl_opengl::Combinaison_couleurs_operande1_Alpha; f_deseri[38] = &(classe_seri_poly.fi1[22]);
 classe_seri_poly.fsd_i1[22] = &alx_noeud_polygone_sdl_opengl::Combinaison_couleurs_operande1_Alpha; f_diff[38]   = &(classe_seri_poly.fsd_i1[22]);
 classe_seri_poly.fi1[23]    = &alx_noeud_polygone_sdl_opengl::Combinaison_textures_source_arg0; f_deseri[39] = &(classe_seri_poly.fi1[23]);
 classe_seri_poly.fsd_i1[23] = &alx_noeud_polygone_sdl_opengl::Combinaison_textures_source_arg0; f_diff[39]   = &(classe_seri_poly.fsd_i1[23]);
 classe_seri_poly.fi1[24]    = &alx_noeud_polygone_sdl_opengl::Combinaison_textures_operande0; f_deseri[40] = &(classe_seri_poly.fi1[24]);
 classe_seri_poly.fsd_i1[24] = &alx_noeud_polygone_sdl_opengl::Combinaison_textures_operande0; f_diff[40]   = &(classe_seri_poly.fsd_i1[24]);
 classe_seri_poly.fi1[25]    = &alx_noeud_polygone_sdl_opengl::Combinaison_textures_source_arg1; f_deseri[41] = &(classe_seri_poly.fi1[25]);
 classe_seri_poly.fsd_i1[25] = &alx_noeud_polygone_sdl_opengl::Combinaison_textures_source_arg1; f_diff[41]   = &(classe_seri_poly.fsd_i1[25]);
 classe_seri_poly.fi1[26]    = &alx_noeud_polygone_sdl_opengl::Combinaison_textures_operande1; f_deseri[42] = &(classe_seri_poly.fi1[26]);
 classe_seri_poly.fsd_i1[26] = &alx_noeud_polygone_sdl_opengl::Combinaison_textures_operande1; f_diff[42]   = &(classe_seri_poly.fsd_i1[26]);
 classe_seri_poly.fi1[27]    = &alx_noeud_polygone_sdl_opengl::Combinaison_textures_source_arg2; f_deseri[43] = &(classe_seri_poly.fi1[27]);
 classe_seri_poly.fsd_i1[27] = &alx_noeud_polygone_sdl_opengl::Combinaison_textures_source_arg2; f_diff[43]   = &(classe_seri_poly.fsd_i1[27]);
 classe_seri_poly.fi1[28]    = &alx_noeud_polygone_sdl_opengl::Combinaison_textures_operande2; f_deseri[44] = &(classe_seri_poly.fi1[28]);
 classe_seri_poly.fsd_i1[28] = &alx_noeud_polygone_sdl_opengl::Combinaison_textures_operande2; f_diff[44]   = &(classe_seri_poly.fsd_i1[28]);
 classe_seri_poly.fi1[29]    = &alx_noeud_polygone_sdl_opengl::Combinaison_textures_source_arg0_Alpha; f_deseri[45] = &(classe_seri_poly.fi1[29]);
 classe_seri_poly.fsd_i1[29] = &alx_noeud_polygone_sdl_opengl::Combinaison_textures_source_arg0_Alpha; f_diff[45]   = &(classe_seri_poly.fsd_i1[29]);
 classe_seri_poly.fi1[30]    = &alx_noeud_polygone_sdl_opengl::Combinaison_textures_operande0_Alpha; f_deseri[46] = &(classe_seri_poly.fi1[30]);
 classe_seri_poly.fsd_i1[30] = &alx_noeud_polygone_sdl_opengl::Combinaison_textures_operande0_Alpha; f_diff[46]   = &(classe_seri_poly.fsd_i1[30]);
 classe_seri_poly.fi1[31]    = &alx_noeud_polygone_sdl_opengl::Combinaison_textures_source_arg1_Alpha; f_deseri[47] = &(classe_seri_poly.fi1[31]);
 classe_seri_poly.fsd_i1[31] = &alx_noeud_polygone_sdl_opengl::Combinaison_textures_source_arg1_Alpha; f_diff[47]   = &(classe_seri_poly.fsd_i1[31]);
 classe_seri_poly.fi1[32]    = &alx_noeud_polygone_sdl_opengl::Combinaison_textures_operande1_Alpha; f_deseri[48] = &(classe_seri_poly.fi1[32]);
 classe_seri_poly.fsd_i1[32] = &alx_noeud_polygone_sdl_opengl::Combinaison_textures_operande1_Alpha; f_diff[48]   = &(classe_seri_poly.fsd_i1[32]);

}

//______________________________________________________________________________
void alx_noeud_polygone_sdl_opengl::init()
{Nom_classe("polygon");
 init_ptr();

 ligne_originale        = (double*)NULL;
 taille_relle_ligne_originale = taille_ligne_originale = 0;

 info_texture2 = info_texture = (INFOS_TEXTURE*)NULL;

 pre_rendu_en_cours = false;
 Marqueur_attributs_poly(0);
 Position_dans_liste_des_fils_changeable(true);
 boite_noeud.Est_vide( false );
 boite_fils.Est_vide( false );
 boite_noeud_et_fils.Est_vide( false );
 boite_noeud_et_fils_glob.Est_vide( false );

 gpc_poly.num_contours         = 0;
   gpc_poly.contour = NULL;
   gpc_poly.hole    = NULL;
 gpc_poly_tmp.num_contours     = 0;
   gpc_poly_tmp.contour = NULL;
   gpc_poly_tmp.hole    = NULL;
 gpc_poly_add_sub.num_contours = 0;
   gpc_poly_add_sub.contour = NULL;
   gpc_poly_add_sub.hole    = NULL;

 Translucidite(true);
 Lisser( true );
 Lisser_texture(true);
 Polygone_a_afficher( true  );
 Perimetre_a_afficher( true );
 Couleur(1, 1, 1, 1);
 Couleur_perimetre(0,0,0,1);
 Const_couleur(0,0,0,0);

 Coords_couleurs          ( NULL );
 Coords_textures          ( NULL );
 Coords_textures2         ( NULL );
 Coords_couleurs_perimetre( NULL );
 Coords_textures_perimetre( NULL );

 Combinaison_textures_source_arg0_Alpha( 0 ); Combinaison_textures_operande0_Alpha( 0 );
 Combinaison_couleurs_source_arg0_Alpha( 0 ); Combinaison_couleurs_operande0_Alpha( 0 );
 Combinaison_textures_source_arg1_Alpha( 0 ); Combinaison_textures_operande1_Alpha( 0 );
 Combinaison_couleurs_source_arg1_Alpha( 0 ); Combinaison_couleurs_operande1_Alpha( 0 );
 Combinaison_textures_source_arg2_Alpha( 0 ); Combinaison_textures_operande2_Alpha( 0 );
 Combinaison_couleurs_source_arg2_Alpha( 0 ); Combinaison_couleurs_operande2_Alpha( 0 );
 Combinaison_textures_source_arg0      ( 0 ); Combinaison_textures_operande0      ( 0 );
 Combinaison_couleurs_source_arg0      ( 0 ); Combinaison_couleurs_operande0      ( 0 );
 Combinaison_textures_source_arg1      ( 0 ); Combinaison_textures_operande1      ( 0 );
 Combinaison_couleurs_source_arg1      ( 0 ); Combinaison_couleurs_operande1      ( 0 );
 Combinaison_textures_source_arg2      ( 0 ); Combinaison_textures_operande2      ( 0 );
 Combinaison_couleurs_source_arg2      ( 0 ); Combinaison_couleurs_operande2      ( 0 );

/* DEBUG OPTIMISATION
 Combinaison_textures_source_arg0_Alpha( GL_TEXTURE      ); Combinaison_textures_operande0_Alpha( GL_SRC_ALPHA );
 Combinaison_couleurs_source_arg0_Alpha( GL_TEXTURE      ); Combinaison_couleurs_operande0_Alpha( GL_SRC_ALPHA );
 Combinaison_textures_source_arg1_Alpha( GL_PRIMARY_COLOR_ARB ); Combinaison_textures_operande1_Alpha( GL_SRC_ALPHA );
 Combinaison_couleurs_source_arg1_Alpha( GL_PRIMARY_COLOR_ARB ); Combinaison_couleurs_operande1_Alpha( GL_SRC_ALPHA );
 Combinaison_textures_source_arg2_Alpha( GL_PRIMARY_COLOR_ARB ); Combinaison_textures_operande2_Alpha( GL_SRC_ALPHA );
 Combinaison_couleurs_source_arg2_Alpha( GL_PRIMARY_COLOR_ARB ); Combinaison_couleurs_operande2_Alpha( GL_SRC_ALPHA );
 Combinaison_textures_source_arg0      ( GL_TEXTURE      ); Combinaison_textures_operande0      ( GL_SRC_COLOR );
 Combinaison_couleurs_source_arg0      ( GL_TEXTURE      ); Combinaison_couleurs_operande0      ( GL_SRC_COLOR );
 Combinaison_textures_source_arg1      ( GL_PRIMARY_COLOR_ARB ); Combinaison_textures_operande1      ( GL_SRC_COLOR );
 Combinaison_couleurs_source_arg1      ( GL_PRIMARY_COLOR_ARB ); Combinaison_couleurs_operande1      ( GL_SRC_COLOR );
 Combinaison_textures_source_arg2      ( GL_PRIMARY_COLOR_ARB ); Combinaison_textures_operande2      ( GL_SRC_COLOR );
 Combinaison_couleurs_source_arg2      ( GL_PRIMARY_COLOR_ARB ); Combinaison_couleurs_operande2      ( GL_SRC_COLOR );
*/
 num_liste               = -1;
 num_liste_perimetre     = -1;
 num_liste_mode_texture2 = -1;
 poly_ok     = &gpc_poly;
 poly_calcul = &gpc_poly_tmp;
 boite_noeud_deja_calculees = false;
 info_texture = NULL;

 Nom_IU("Polygone");
 a_ete_modifie = false;
 texture_1_modif = texture_2_modif = false;

 interpollateur = new Interpollateur_couleur_polygone();
 interpollateur->Fct( Fonction_interpolation_poly_boite_englobante );
 interpollateur_perimetre = new Interpollateur_couleur_polygone();
 interpollateur_perimetre->Fct( Fonction_interpolation_poly_boite_englobante );

 Combinaison_textures( GL_modulate() );
 Combinaison_couleurs( GL_modulate() );
 Alpha_scale(1); RGB_scale(1);
 Ordre_application_couleurs(0); Ordre_application_textures(0);
 Constante_combinaison_textures(0.5);
 Constante_combinaison_couleurs(0.5);
/* DEBUG OPTIMISATION
 Combinaison_couleurs_source_arg2(GL_TEXTURE);
 Combinaison_couleurs_operande2  (GL_SRC_ALPHA);
 Combinaison_textures_source_arg2(GL_TEXTURE);
 Combinaison_textures_operande2  (GL_SRC_ALPHA);
*/
 Mode_texture2(false);

 Mode_polygone();
}

//______________________________________________________________________________
alx_noeud_polygone_sdl_opengl::alx_noeud_polygone_sdl_opengl() : alx_noeud_scene()
                                                         , classe_seri_poly( this
                                                                           , &serialisation_type_poly
                                                                           , &nouv_serialisation_type_poly
                                                                           , &serialisation_differences_type_poly )
{init();}

//______________________________________________________________________________
alx_noeud_polygone_sdl_opengl::~alx_noeud_polygone_sdl_opengl() {
 gpc_free_polygon(&gpc_poly);
 gpc_free_polygon(&gpc_poly_tmp);
 gpc_free_polygon(&gpc_poly_add_sub);
}

//______________________________________________________________________________
void alx_noeud_polygone_sdl_opengl::Operation(gpc_polygon *p, gpc_op operation)
{gpc_free_polygon( poly_calcul );
 gpc_polygon_clip( operation
                 , poly_ok
                 , p
                 , poly_calcul);
 gpc_polygon *tmp = poly_calcul;
 poly_calcul = poly_ok;
 poly_ok     = tmp;
 maj();
}


//______________________________________________________________________________
void alx_noeud_polygone_sdl_opengl::Maj_poly(alx_noeud_polygone_sdl_opengl &p)
{Deserialiser_type( p.Serialiser_type() );
 Coords_textures_perimetre( p.Coords_textures_perimetre() );
 Coords_couleurs_perimetre( p.Coords_couleurs_perimetre() );
 Coords_couleurs          ( p.Coords_couleurs          () );
 Coords_textures          ( p.Coords_textures          () );
 Coords_textures2         ( p.Coords_textures2         () );
 Fct_interpollation       ( p.Fct_interpollation       () );
 Info_texture             ( p.info_texture                );
 Info_texture2            ( p.info_texture2               );
}

//______________________________________________________________________________
void alx_noeud_polygone_sdl_opengl::Operation(alx_noeud_polygone_sdl_opengl &p, const alx_noeud_scene *n_racine, gpc_op operation)
{a_ete_modifie = true;
 gpc_polygon gpc_tmp;
   gpc_tmp.num_contours = 0;
   gpc_tmp.hole         = NULL;
   gpc_tmp.contour      = NULL;
 gpc_vertex_list *VL;
 gpc_free_polygon( poly_calcul );

// Faire une copie de travail du polygone passé en paramètre.
 for(unsigned int i=0; i<(unsigned int)p.poly_ok->num_contours; i++)
  {gpc_add_contour(&gpc_tmp
                  ,&(p.poly_ok->contour[i])
                  , p.poly_ok->hole[i] );
  }
// Appliquer la transformation de p à tous les points de tmp !
// PUIS
// Appliquer la transformation inverse du polygone local à tous les points de tmp !
 if(n_racine == NULL)
  {for(unsigned int i=0; i<(unsigned int)gpc_tmp.num_contours; i++)
    {VL = &(gpc_tmp.contour[i]);
     p.Changer_coordonnees_inverse((double*)VL->vertex, VL->num_vertices);
     this->Changer_coordonnees    ((double*)VL->vertex, VL->num_vertices);}
  } else {// On a une racine commune, ilfaut donc :
          // Récupérer le chemin de cette racine à p
          alx_liste<const alx_noeud_scene*> L_rep_p;
            p.Chemin_ancetre(n_racine, L_rep_p);
          // Récupérer le chemin de cette racine à this
          alx_liste<const alx_noeud_scene*> L_rep_this;
            this->Chemin_ancetre(n_racine, L_rep_this);
          // Pour chaque contour du polygone
          for(unsigned int i=0; i<(unsigned int)gpc_tmp.num_contours; i++)
           {VL = &(gpc_tmp.contour[i]);
           // Modification en cascade des coordonnées de p en remontant jusqu'à la racine
            alx_element_liste<const alx_noeud_scene*> *it      = L_rep_p.Dernier()
                                                    , *it_prem = L_rep_p.Premier();
            for(; it!=it_prem; it=it->prcdt)
             {it->E()->Changer_coordonnees_inverse((double*)VL->vertex, VL->num_vertices);}
           // Modification en cascade des coordonnées de p en descendant de la racine à this
                                                       it     = L_rep_this.Premier(); it = it->svt;
            alx_element_liste<const alx_noeud_scene*> *it_fin = L_rep_this.Fin();
            for(; it!=it_fin; it=it->svt)
             {it->E()->Changer_coordonnees((double*)VL->vertex, VL->num_vertices);}
           }
         }
// Effectuer l'opération voulue sur le polygone local !
 gpc_polygon_clip( operation
                 , poly_ok
                 ,&gpc_tmp
                 , poly_calcul);
 gpc_polygon *tmp_flip = poly_calcul;
 poly_calcul = poly_ok;
 poly_ok     = tmp_flip;
 maj();
 gpc_free_polygon( &gpc_tmp );
   gpc_tmp.num_contours = 0;
   gpc_tmp.hole         = NULL;
   gpc_tmp.contour      = NULL;
}

//______________________________________________________________________________
const alx_point2D& alx_noeud_polygone_sdl_opengl::Pt(const unsigned int num_contour, const unsigned num_pt) const
{pt_tmp_consultation.maj( (double*)&(poly_ok->contour[num_contour].vertex[num_pt]) );
 return pt_tmp_consultation;}
const double* alx_noeud_polygone_sdl_opengl::Tab_pt(const unsigned int num_contour, const unsigned num_pt) const
{return (double*)&(poly_ok->contour[num_contour].vertex[num_pt]);}
const double* alx_noeud_polygone_sdl_opengl::Tab_pt(const unsigned int num_contour) const
{return (double*)poly_ok->contour[num_contour].vertex;}
const unsigned int alx_noeud_polygone_sdl_opengl::Nb_pt(const unsigned int num_contour) const
{return poly_ok->contour[num_contour].num_vertices;}
const unsigned int alx_noeud_polygone_sdl_opengl::Est_contour_plein(const unsigned int num_contour) const
{return 1-poly_ok->hole[num_contour];}
const unsigned int alx_noeud_polygone_sdl_opengl::Nb_contours() const
{return (unsigned int)(poly_ok->num_contours);}

//______________________________________________________________________________
void alx_noeud_polygone_sdl_opengl::Init_textures()
{if(info_texture) {glPushAttrib( GL_ENABLE_BIT | GL_TEXTURE_BIT | GL_COLOR_BUFFER_BIT );
                  glActiveTextureARB(GL_TEXTURE0_ARB);
                  glEnable(GL_TEXTURE_2D);
                  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
                  if (Combinaison_couleurs_rgb  ()) glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB  , Combinaison_couleurs_rgb  ());
                  if (Combinaison_couleurs_alpha()) glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, Combinaison_couleurs_alpha());
                  glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, Const_couleur());

                  if (Combinaison_couleurs_source_arg0()) glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, Combinaison_couleurs_source_arg0());
                  if (Combinaison_couleurs_operande0  ()) glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB_ARB, Combinaison_couleurs_operande0 ());
                  if (Combinaison_couleurs_source_arg1()) glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, Combinaison_couleurs_source_arg1());
                  if (Combinaison_couleurs_operande1  ()) glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB_ARB, Combinaison_couleurs_operande1 ());
                  if (Combinaison_couleurs_source_arg2()) glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_RGB_ARB, Combinaison_couleurs_source_arg2());
                  if (Combinaison_couleurs_operande2  ()) glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_RGB_ARB, Combinaison_couleurs_operande2());
                  if (Combinaison_couleurs_source_arg0_Alpha()) glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, Combinaison_couleurs_source_arg0_Alpha());
                  if (Combinaison_couleurs_operande0_Alpha  ()) glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA_ARB, Combinaison_couleurs_operande0_Alpha());
                  if (Combinaison_couleurs_source_arg1_Alpha()) glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA_ARB, Combinaison_couleurs_source_arg1_Alpha());
                  if (Combinaison_couleurs_operande1_Alpha  ()) glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA_ARB, Combinaison_couleurs_operande1_Alpha());
                  if (Combinaison_couleurs_source_arg2_Alpha()) glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_ALPHA_ARB, Combinaison_couleurs_source_arg2_Alpha());
                  if (Combinaison_couleurs_operande2_Alpha  ()) glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_ALPHA_ARB, Combinaison_couleurs_operande2_Alpha());

                  //DEBUG if(Ordre_application_couleurs_rgb() == 0) {glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE); glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PREVIOUS_ARB);
                  //DEBUG  } else {glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_TEXTURE); glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_PREVIOUS_ARB);}
                  //DEBUG if(Ordre_application_couleurs_alpha() == 0) {glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_TEXTURE); glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA_ARB, GL_PREVIOUS_ARB);
                  //DEBUG  } else {glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA_ARB, GL_TEXTURE); glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_PREVIOUS_ARB);}
                  glBindTexture(GL_TEXTURE_2D, info_texture->id_texture);
                  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                  if( Lisser() )
                   {glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);}
                   else
                    {glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);}
                  if(info_texture2)
                   {glActiveTextureARB(GL_TEXTURE1_ARB);
                    glEnable(GL_TEXTURE_2D);
                    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
                    if (Combinaison_textures_rgb  ()) glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB  , Combinaison_textures_rgb());
                    if (Combinaison_textures_alpha()) glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, Combinaison_textures_alpha());
                      if (Combinaison_textures_source_arg0()) glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB , Combinaison_textures_source_arg0());
                      if (Combinaison_textures_operande0  ()) glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB_ARB, Combinaison_textures_operande0  ());
                      if (Combinaison_textures_source_arg1()) glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB , Combinaison_textures_source_arg1());
                      if (Combinaison_textures_operande1  ()) glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB_ARB, Combinaison_textures_operande1  ());
                      if (Combinaison_textures_source_arg2()) glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_RGB_ARB , Combinaison_textures_source_arg2());
                      if (Combinaison_textures_operande2  ()) glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_RGB_ARB, Combinaison_textures_operande2  ());
                      if (Combinaison_textures_source_arg0_Alpha()) glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB , Combinaison_textures_source_arg0_Alpha());
                      if (Combinaison_textures_operande0_Alpha  ()) glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA_ARB, Combinaison_textures_operande0_Alpha  ());
                      if (Combinaison_textures_source_arg1_Alpha()) glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA_ARB , Combinaison_textures_source_arg1_Alpha());
                      if (Combinaison_textures_operande1_Alpha  ()) glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA_ARB, Combinaison_textures_operande1_Alpha  ());
                      if (Combinaison_textures_source_arg2_Alpha()) glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_ALPHA_ARB , Combinaison_textures_source_arg2_Alpha());
                      if (Combinaison_textures_operande2_Alpha  ()) glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_ALPHA_ARB, Combinaison_textures_operande2_Alpha  ());
                      glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, Const_couleur());
                    glTexEnvf(GL_TEXTURE_ENV, GL_ALPHA_SCALE  , Alpha_scale());
                    glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, RGB_scale()  );
                    //DEBUG if(Ordre_application_textures_rgb() == 0) {glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE); glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PREVIOUS_ARB);
                    //DEBUG  } else {glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_TEXTURE); glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_PREVIOUS_ARB);}
                    //DEBUG if(Ordre_application_textures_alpha() == 0) {glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_TEXTURE); glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA_ARB, GL_PREVIOUS_ARB);
                    //DEBUG  } else {glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA_ARB, GL_TEXTURE); glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_PREVIOUS_ARB);}
                    glBindTexture(GL_TEXTURE_2D, info_texture2->id_texture);
                   }
                 }
}

//______________________________________________________________________________
void alx_noeud_polygone_sdl_opengl::Stop_textures()
{if(info_texture) {glPopAttrib();
                  if(info_texture2)
                   {glDisable(GL_TEXTURE_2D);
                    glActiveTextureARB(GL_TEXTURE0_ARB);
                   }
                 }
}

//______________________________________________________________________________
void alx_noeud_polygone_sdl_opengl::Trianguler()
{if(get_Mode_line()) {return;}
 const int NBE = 64000;
 GLdouble T[ NBE ];

 a_ete_modifie = true;
 boite_noeud_deja_calculees = false;
   Calculer_boite_noeud(); // Pour avoir la voite englobante
// double tx = pt_max[0] - pt_min[0]
//      , ty = pt_max[1] - pt_min[1];
 //printf("Dimension du polygone (%f, %f)", tx, ty);

 int num_liste_courant;
 if(Mode_texture() && Mode_texture2())
  {if(num_liste_mode_texture2==-1) num_liste_mode_texture2 = glGenLists(1);
    else {glDeleteLists(num_liste_mode_texture2, 1);
          num_liste_mode_texture2 = glGenLists(1);}
   num_liste_courant = num_liste_mode_texture2;
  } else {if(num_liste==-1) num_liste = glGenLists(1);
             else {glDeleteLists(num_liste, 1);
                   num_liste = glGenLists(1);}
          num_liste_courant = num_liste;
         }

 gpc_vertex_list *VL = poly_ok->contour;

 GLUtesselator *tobj = (GLUtesselator*)NULL;
 tobj = gluNewTess();
   gluTessCallback(tobj, GLU_TESS_BEGIN  , (GLvoid (__stdcall*) ( ))&glBegin);
   gluTessCallback(tobj, GLU_TESS_END    , (GLvoid (__stdcall*) ( ))&glEnd);
   gluTessProperty(tobj, GLU_TESS_WINDING_RULE , GLU_TESS_WINDING_ODD); //GLU_TESS_WINDING_NONZERO);
   if(info_texture)
    {if( Coords_couleurs() && Fct_interpollation() )
       {if(info_texture2)
         {gluTessCallback(tobj, GLU_TESS_VERTEX , (GLvoid (__stdcall*) ( ))&vertexCouleurTexture2Callback);
          gluTessCallback(tobj, GLU_TESS_COMBINE, (GLvoid (__stdcall*) ( ))&combineCouleurTexture2Callback);
         } else {gluTessCallback(tobj, GLU_TESS_VERTEX , (GLvoid (__stdcall*) ( ))&vertexCouleurTextureCallback);
                 gluTessCallback(tobj, GLU_TESS_COMBINE, (GLvoid (__stdcall*) ( ))&combineCouleurTextureCallback);
                }
       } else {if(info_texture2)
                {gluTessCallback(tobj, GLU_TESS_VERTEX , (GLvoid (__stdcall*) ( ))&vertexTexture2Callback);
                 gluTessCallback(tobj, GLU_TESS_COMBINE, (GLvoid (__stdcall*) ( ))&combineTexture2Callback);
                } else {gluTessCallback(tobj, GLU_TESS_VERTEX , (GLvoid (__stdcall*) ( ))&vertexTextureCallback);
                        gluTessCallback(tobj, GLU_TESS_COMBINE, (GLvoid (__stdcall*) ( ))&combineTextureCallback);
                       }
              }
    } else {if( Coords_couleurs() && Fct_interpollation() )
             {gluTessCallback(tobj, GLU_TESS_VERTEX , (GLvoid (__stdcall*) ( ))&vertexCouleurCallback);
              gluTessCallback(tobj, GLU_TESS_COMBINE, (GLvoid (__stdcall*) ( ))&combineCouleurCallback);
             } else {gluTessCallback(tobj, GLU_TESS_VERTEX , (GLvoid (__stdcall*) ( ))&glVertex2dv);
                     gluTessCallback(tobj, GLU_TESS_COMBINE, (GLvoid (__stdcall*) ( ))&combineCallback);}
           }


//   gluTessProperty(tobj, GLU_TESS_BOUNDARY_ONLY, GL_TRUE);

 unsigned int pos = 0;
 glNewList(num_liste_courant,GL_COMPILE);
   //Init_textures();
   gluTessBeginPolygon(tobj, NULL);
   Fonction_interpolation_couleur_polygone f = Fct_interpollation();
   if(info_texture)
    {//printf("texture!, info_texture\n  taille(%i, %i)\n  taille_reelle(%i, %i)\n  texture_id(%i)\n", info_texture->tx, info_texture->ty, info_texture->taille_reelle[0], info_texture->taille_reelle[1], info_texture->id_texture);
      for(int i=0; i<poly_ok->num_contours; i++)
        {gluTessBeginContour(tobj);
         if(Coords_couleurs() && (f != NULL))
          {for(unsigned int j=0; j<(unsigned int)VL[i].num_vertices; j++)
            {T[pos]   = VL[i].vertex[j].x;
             T[pos+1] = VL[i].vertex[j].y;
             T[pos+2] = 0;
            // Calcul des coordonnées texture par rapport à pt_min et pt_max
             if(info_texture2)
              {interpollateur->CTP(&(T[pos+5]), &(T[pos+3]), &T[pos+9]);
               interpollateur->Interpoller(*this, Coords_couleurs(), Coords_textures(), Coords_textures2(), T[pos], T[pos+1]);
               gluTessVertex(tobj, &(T[pos]), &(T[pos]));
               pos += 11;
              } else {interpollateur->CTP(&(T[pos+5]), &(T[pos+3]), NULL);
                      interpollateur->Interpoller(*this, Coords_couleurs(), Coords_textures(), NULL, T[pos], T[pos+1]);
                      gluTessVertex(tobj, &(T[pos]), &(T[pos]));
                      pos += 9;
                     }


            }
          } else {for(unsigned int j=0; j<(unsigned int)VL[i].num_vertices; j++)
                   {T[pos]   = VL[i].vertex[j].x;
                    T[pos+1] = VL[i].vertex[j].y;
                    T[pos+2] = 0;
                   // Calcul des coordonnées texture par rapport à pt_min et pt_max
                    if(info_texture2)
                     {interpollateur->CTP((double*)NULL, &(T[pos+3]), &(T[pos+5]));
                      interpollateur->Interpoller(*this, (double*)NULL, Coords_textures(), Coords_textures2(), T[pos], T[pos+1]);
                      gluTessVertex(tobj, &(T[pos]), &(T[pos]));
                      pos += 7;
                     } else {interpollateur->CTP((double*)NULL, &(T[pos+3]), (double*)NULL);
                             interpollateur->Interpoller(*this, (double*)NULL, Coords_textures(), (double*)NULL, T[pos], T[pos+1]);
                             gluTessVertex(tobj, &(T[pos]), &(T[pos]));
                             pos += 5;
                            }
                   }
                 }
        }
       gluTessEndContour(tobj);
     }
     else {//printf("Pas de texture");
           if( Coords_couleurs()
             &&(f != NULL) )
           {for(int i=0; i<poly_ok->num_contours; i++)
                    {gluTessBeginContour(tobj);
                     for(unsigned int j=0; j<(unsigned int)VL[i].num_vertices; j++)
                      {T[pos]   = VL[i].vertex[j].x;
                       T[pos+1] = VL[i].vertex[j].y;
                       T[pos+2] = 0;
                       interpollateur->CTP(&(T[pos+3]), (double*)NULL, (double*)NULL);
                       interpollateur->Interpoller(*this, Coords_couleurs(), (double*)NULL, (double*)NULL, T[pos], T[pos+1]);
                       gluTessVertex(tobj, &(T[pos]), &(T[pos]));
                       pos += 7;
                      }
                     gluTessEndContour(tobj);
                    }
            } else {for(int i=0; i<poly_ok->num_contours; i++)
                     {gluTessBeginContour(tobj);
                      for(unsigned int j=0; j<(unsigned int)VL[i].num_vertices; j++)
                       {T[pos]   = VL[i].vertex[j].x;
                        T[pos+1] = VL[i].vertex[j].y;
                        T[pos+2] = 0;
                        //printf("gluTessVertex(%f, %f)\n", T[pos], T[pos+1]);
                        gluTessVertex(tobj, &(T[pos]), &(T[pos]));
                        pos += 3;}
                      gluTessEndContour(tobj);
                     }
                   }
          }
   gluTessEndPolygon(tobj);
 glEnd();
 //Stop_textures();
 /*if(info_texture) {glPopAttrib();
                   if(info_texture2)
                    {glDisable(GL_TEXTURE_2D);
                     glActiveTextureARB(GL_TEXTURE0_ARB);
                    }
                  }*/
 glEndList();
 gluDeleteTess(tobj);

 return;
 /*
 gpc_polygon_to_tristrip(&gpc_poly, &gpc_tri);
 gpc_vertex_list *VL;
 double *ptr;
 glNewList(num_liste,GL_COMPILE);
   for(unsigned int i=0; i<(unsigned int)gpc_tri.num_strips; i++)
   {VL = &(gpc_tri.strip[i]);
    ptr = (double*)(void*)VL->vertex;
    glBegin(GL_TRIANGLE_STRIP);
    for(unsigned int j=0; j<VL->num_vertices; j++)
      glVertex2dv( &(ptr[2*j]) );
    glEnd();
   }
 glEndList();    */
}

//______________________________________________________________________________
void alx_noeud_polygone_sdl_opengl::Contourner()
{double Tab_tex[16000], Tab_tex2[16000], Tab_col[8000];
 unsigned int pos_col, pos_tex, pos_tex2;

 if(num_liste_perimetre==-1) num_liste_perimetre = glGenLists(1);
    else {glDeleteLists(num_liste_perimetre, 1);
          num_liste_perimetre = glGenLists(1);}

 gpc_vertex_list *VL = poly_ok->contour;

// Création de la liste proprement dite à partir des infos
 glNewList(num_liste_perimetre, GL_COMPILE);
// if(Perimetre_texture())
//  {Init_textures();}
/*  &&info_texture) {glPushAttrib( GL_ENABLE_BIT );
                   glEnable(GL_TEXTURE_2D);
                   glBindTexture(GL_TEXTURE_2D, info_texture->id_texture);
                   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                   if( Lisser() )
                    {glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);}
                    else
                     {glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);}
                  }
*/
   glPushAttrib(GL_ENABLE_BIT);

   glEnableClientState( GL_VERTEX_ARRAY );
  // Texture 0
   glClientActiveTextureARB(GL_TEXTURE0_ARB);
     if(info_texture && Perimetre_texture())
      {glEnableClientState( GL_TEXTURE_COORD_ARRAY );
       glTexCoordPointer(2, GL_DOUBLE, 0, Tab_tex);
      }
     if(Coords_couleurs_perimetre())
      {glEnableClientState( GL_COLOR_ARRAY );
       glColorPointer(4, GL_DOUBLE, 0, Tab_col);
      }
  // Texture 1
   if(info_texture && info_texture2 && Perimetre_texture())
    {glClientActiveTextureARB(GL_TEXTURE1_ARB);
     glEnableClientState(GL_TEXTURE_COORD_ARRAY);
     glTexCoordPointer(2,GL_DOUBLE,0,Tab_tex2);
     glClientActiveTextureARB(GL_TEXTURE0_ARB); // Retour état initial
    }


   for(unsigned int i=0; i<(unsigned int)poly_ok->num_contours; i++)
    {if(i==0 && get_Mode_line()) {
       glVertexPointer (2, GL_DOUBLE, 0, ligne_originale );
      } else {glVertexPointer (2, GL_DOUBLE, 0, VL[i].vertex );}

     if(nb_elements_ordre_affichage < (unsigned int)VL[i].num_vertices)
      {// Redimensionnement du tableau d'indexage
       delete[] ordre_affichage_poly;
       nb_elements_ordre_affichage = Amax(2*nb_elements_ordre_affichage, (unsigned int)(2*VL[i].num_vertices));
       ordre_affichage_poly = new unsigned int [nb_elements_ordre_affichage];
       for(unsigned int j=0; j<nb_elements_ordre_affichage; j++)
        {ordre_affichage_poly[j] = j;}
      }
    // Texture? Couleurs?
     if( (Coords_couleurs_perimetre())
       ||(info_texture && Perimetre_texture()) )
      {pos_col = pos_tex = pos_tex2 = 0;
       if(info_texture2)
        {for(unsigned int j=0; j<(unsigned int)VL[i].num_vertices; j++)
          {interpollateur_perimetre->CTP(&(Tab_col[pos_col]), &(Tab_tex[pos_tex]), &(Tab_tex2[pos_tex]));
           interpollateur_perimetre->Interpoller(*this, Coords_couleurs_perimetre(), Coords_textures_perimetre(), Coords_textures_perimetre(), VL[i].vertex[j].x, VL[i].vertex[j].y);
           pos_col  += 4;
           pos_tex  += 2;
           pos_tex2 += 2;
          }
        } else {for(unsigned int j=0; j<(unsigned int)VL[i].num_vertices; j++)
                 {interpollateur_perimetre->CTP(&(Tab_col[pos_col]), &(Tab_tex[pos_tex]), (double*)NULL);
                  interpollateur_perimetre->Interpoller(*this, Coords_couleurs_perimetre(), Coords_textures_perimetre(), (double*)NULL, VL[i].vertex[j].x, VL[i].vertex[j].y);
                  pos_col += 4;
                  pos_tex += 2;
                 }
               }
      }

     if(i==0 && get_Mode_line()) {
       glDrawElements( Lignes_GL()
                     , taille_ligne_originale/2
                     , GL_UNSIGNED_INT
                     , ordre_affichage_poly);

      } else {glDrawElements( Lignes_GL()
                            , VL[i].num_vertices
                            , GL_UNSIGNED_INT
                            , ordre_affichage_poly);
             }
    }
   glPopAttrib();
   glDisableClientState( GL_VERTEX_ARRAY );
   glDisableClientState( GL_COLOR_ARRAY );
   glDisableClientState( GL_TEXTURE_COORD_ARRAY );
//   if(Perimetre_texture())
//    {Stop_textures();}
    //&&info_texture) {glPopAttrib();}
 glEndList();
}

//______________________________________________________________________________
void alx_noeud_polygone_sdl_opengl::Lignes_GL(const unsigned int mode_ligne) {lignes_GL = mode_ligne;}
const unsigned int alx_noeud_polygone_sdl_opengl::Lignes_GL()                {return lignes_GL;}

//______________________________________________________________________________
//void alx_noeud_polygone_sdl_opengl::Combinaison_textures(const GLenum m) {combinaison_textures = m; Trianguler();}
//const GLenum alx_noeud_polygone_sdl_opengl::Combinaison_textures()       {return combinaison_textures;}

//______________________________________________________________________________
//void alx_noeud_polygone_sdl_opengl::Combinaison_couleurs(const GLenum m) {combinaison_couleurs = m; Trianguler();}
//const GLenum alx_noeud_polygone_sdl_opengl::Combinaison_couleurs()       {return combinaison_couleurs;}

//______________________________________________________________________________
void alx_noeud_polygone_sdl_opengl::Vider()
{gpc_free_polygon(poly_ok);
   poly_ok->num_contours = 0;
   poly_ok->contour = (gpc_vertex_list *)NULL;
   poly_ok->hole    = (int *)NULL;
 gpc_free_polygon(poly_calcul);
   poly_calcul->num_contours = 0;
   poly_calcul->contour = (gpc_vertex_list *)NULL;
   poly_calcul->hole    = (int *)NULL;
 maj();
}
//______________________________________________________________________________
void alx_noeud_polygone_sdl_opengl::Retirer_contour(const unsigned int nb_points, double *Tspt)
{gpc_vertex_list VL;
 VL.num_vertices = nb_points;
 VL.vertex = (gpc_vertex*)(void*)Tspt;
 gpc_add_contour(&gpc_poly_add_sub, &VL, 0);
 VL.vertex = NULL;
 Difference(&gpc_poly_add_sub);
 gpc_free_polygon(&gpc_poly_add_sub);
 maj();}

//______________________________________________________________________________
void alx_noeud_polygone_sdl_opengl::Ajouter_contour(const unsigned int nb_points, double *Tspt)
{if(get_Mode_line())
  {if(taille_relle_ligne_originale < 2*nb_points)
    {if(ligne_originale) {delete[] ligne_originale;}
     taille_relle_ligne_originale = taille_ligne_originale = 2*nb_points;
     ligne_originale        = new double [taille_ligne_originale];
    } else {taille_ligne_originale = 2*nb_points;}
   memcpy(ligne_originale, Tspt, taille_ligne_originale*sizeof(double));
  }

 gpc_vertex_list VL;
   VL.num_vertices = nb_points;
   VL.vertex = (gpc_vertex*)(void*)Tspt;
 gpc_poly_add_sub.num_contours = 0;
   gpc_poly_add_sub.hole         = NULL;
   gpc_poly_add_sub.contour      = NULL;
 gpc_add_contour(&gpc_poly_add_sub, &VL, 0);
 VL.vertex       = NULL;
 VL.num_vertices = 0;
 Union(&gpc_poly_add_sub);
 gpc_free_polygon(&gpc_poly_add_sub);
 maj();
}

//______________________________________________________________________________
void alx_noeud_polygone_sdl_opengl::Mode_texture2(const bool b) {
  Marqueur_attributs_poly(POLY_mode_texture2(), b);
  A_changer(true);
  if(b) {
    INFOS_TEXTURE *info_tmp = info_texture;

    Contenu_dans_fils_ssi_contenu_dans_noeud(true);
    // DEBUG PreRendre();
    info_texture = Info_texture_scene();

    if( pre_rendu_en_cours )
      {maj_tess();
      } else { Pre_rendu_exeptionnel( true );
             }
    info_texture = info_tmp;

   }
}
// <DEBUG>
//   Se passer absolument du pré rendu dans Mode_texture 2 !
//   Faire en sorte de la remplacer par un marqueur/flag adéquat afin que
//   le prérendu et la tessalation ne soit effectué sue lors du parcours des prérendu classique !
// </DEBUG>
//______________________________________________________________________________
void alx_noeud_polygone_sdl_opengl::PreRendre()
{if(pre_rendu_en_cours) return;
 pre_rendu_en_cours = true;
 INFOS_TEXTURE *info_tmp = info_texture2;
 bool changer = A_changer()
    , b = Mode_texture2()
    , p = Perimetre_a_afficher()
    , n = Boite_noeud_pour_texture();
 if(b)
  {Boite_noeud_pour_texture(true);
   Perimetre_a_afficher(false);
   Mode_texture2( false );
   info_texture2 = NULL;
  }

   A_changer( changer );
   alx_noeud_scene::PreRendre();

   //changer = A_changer();
 if(b)
  {info_texture2 = info_tmp;
   Mode_texture2( true );
   Perimetre_a_afficher(p);
   Boite_noeud_pour_texture(n);
   A_changer( changer );
  }
 pre_rendu_en_cours = false;
}
//______________________________________________________________________________
// Conserver le résultats de la triangulation avec les coordonnées de la texture alx_noeud_scene::Info_texture_scene()
// afin de ne pas avoir à refaire la triangluation à chaque changement des fils (puisque la taille de la texture restera la même)
// Ne retrianguler que lorsque la tailel de la texture change...ou bien avoir des textures rectangulaires...?
//______________________________________________________________________________
void alx_noeud_polygone_sdl_opengl::Dessin_texture_noeud()
{if(Mode_texture2())
  {INFOS_TEXTURE *SVG_info_texture = info_texture;
   int SVG_ordre_application_couleurs_rgb   = ordre_application_couleurs_rgb
     , SVG_ordre_application_couleurs_alpha = ordre_application_couleurs_alpha;
   GLenum SVG_combinaison_couleurs_alpha = combinaison_couleurs_alpha
        , SVG_combinaison_couleurs_rgb   = combinaison_couleurs_rgb;
   combinaison_couleurs_rgb         = GL_REPLACE;
   combinaison_couleurs_alpha       = GL_REPLACE;
   ordre_application_couleurs_alpha = 0;
   ordre_application_couleurs_rgb   = 0;
   info_texture                     = alx_noeud_scene::Info_texture_scene();
     Afficher_polygone();
     Afficher_perimetre();
   combinaison_couleurs_rgb         = SVG_combinaison_couleurs_rgb;
   combinaison_couleurs_alpha       = SVG_combinaison_couleurs_alpha;
   ordre_application_couleurs_alpha = SVG_ordre_application_couleurs_alpha;
   ordre_application_couleurs_rgb   = SVG_ordre_application_couleurs_rgb;
   info_texture                     = SVG_info_texture;
  } else {alx_noeud_scene::Dessin_texture_noeud();
         }
}
//______________________________________________________________________________
void alx_noeud_polygone_sdl_opengl::Dessin_noeud()
{Afficher_polygone();
 Afficher_perimetre();
}
//______________________________________________________________________________
/*void alx_noeud_polygone_sdl_opengl::Afficher()
{if( !LR_Avant_affichage.Vide() ) {LR_Avant_affichage.Rappeler( this );}
 glPushMatrix();
   Changement_repere( *this );
   if(Noeud_puis_fils())
    {Afficher_polygone();
     Afficher_perimetre();
     Afficher_fils(0);}
    else
     {Afficher_fils(0);
      Afficher_polygone();
      Afficher_perimetre();}
 glPopMatrix();
 if( !LR_Apres_affichage.Vide() ) {LR_Apres_affichage.Rappeler( this );}
}
*/
//______________________________________________________________________________
void alx_noeud_polygone_sdl_opengl::Mode_ligne()
{Lignes_GL( GL_line_loop() );
 Polygone_a_afficher( false );
 set_Mode_line( true );
 maj();
}

//______________________________________________________________________________
void alx_noeud_polygone_sdl_opengl::Mode_polygone()
{Lignes_GL( GL_line_loop() );
 Polygone_a_afficher( true );
 set_Mode_line( false );
 maj();
}

//______________________________________________________________________________
void alx_noeud_polygone_sdl_opengl::Afficher_perimetre()
{if(!Perimetre_a_afficher()) return;
 glPushAttrib( GL_ENABLE_BIT );
 if( Lisser() ) {glEnable(GL_BLEND);
                 glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
                }
  else glDisable(GL_BLEND);
 glPushAttrib(GL_COLOR_BUFFER_BIT);
   glColor4dv( Couleur_perimetre() );
   if(Perimetre_texture()) Init_textures();
     glCallList(num_liste_perimetre);
   if(Perimetre_texture()) Stop_textures();
 glPopAttrib();
 glPopAttrib();
 return;
}
//______________________________________________________________________________
void alx_noeud_polygone_sdl_opengl::Afficher_polygone()
{if(!Polygone_a_afficher()) return;
 glPushAttrib( GL_ENABLE_BIT );
 if( Translucidite()
   &&( (Couleur(3) < (double)1)
     ||(Coords_couleurs() && (Fct_interpollation() != NULL))
     ||Info_texture() ) ) {glEnable(GL_BLEND);
                           glEnable(GL_ALPHA_TEST);
                           glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
                          }
  else {glDisable(GL_BLEND);
        glDisable(GL_ALPHA_TEST);
       }
 glPushAttrib(GL_COLOR_BUFFER_BIT);
 glColor4dv( Couleur() );
 Init_textures();
   if( Mode_texture() && Mode_texture2() )
    {glCallList(num_liste_mode_texture2);
    } else {
            glCallList(num_liste);}
   Stop_textures();
 glPopAttrib();
 glPopAttrib();
 return;
}

//______________________________________________________________________________
void alx_noeud_polygone_sdl_opengl::Calculer_boite_noeud()
{// Mise à jour des coordonnées de la boîte englobante dans les repères des pères.
 if(!boite_noeud_deja_calculees)
  {gpc_vertex_list *VL;
    bool attribue = false;
    for(unsigned int i=0; i<(unsigned int)poly_ok->num_contours; i++)
     {if(poly_ok->hole[i]) continue;
      VL = &(poly_ok->contour[i]);
      if(!attribue) {pt_min[0] = pt_max[0] = VL->vertex[0].x;
                     pt_min[1] = pt_max[1] = VL->vertex[0].y;
                     attribue = true;}
      for(unsigned int j=0; j<(unsigned int)VL->num_vertices; j++)
       {if(VL->vertex[j].x < pt_min[0]) {pt_min[0] = VL->vertex[j].x;}
         else if(VL->vertex[j].x > pt_max[0]) {pt_max[0] = VL->vertex[j].x;}
        if(VL->vertex[j].y < pt_min[1]) {pt_min[1] = VL->vertex[j].y;}
         else if(VL->vertex[j].y > pt_max[1]) {pt_max[1] = VL->vertex[j].y;}
       }
     }
   boite_noeud_deja_calculees = true;
   boite_noeud.maj(pt_min[0], pt_min[1], pt_max[0], pt_max[1]);
  }

 //printf("Boite noeud (%f, %f, %f, %f)\n", boite_noeud.X1(), boite_noeud.Y1(), boite_noeud.X2(), boite_noeud.Y2() );
 boite_noeud_et_fils.maj( boite_noeud );
 if (!Mode_texture2()) {boite_noeud_et_fils.Inclure_boite( boite_fils );}
 boite_noeud_et_fils_glob.maj(boite_noeud_et_fils);
 boite_noeud_et_fils_glob.Prendre_en_compte( *this );
 //printf("boite_noeud_et_fils_glob (%f, %f, %f, %f)\n", boite_noeud_et_fils_glob.X1(), boite_noeud_et_fils_glob.Y1(), boite_noeud_et_fils_glob.X2(), boite_noeud_et_fils_glob.Y2() );
}

//______________________________________________________________________________
// Calcul s'il y a intersection entre le segment V1-V2 et la demi droite horizontal d'origine pt
const unsigned int alx_noeud_polygone_sdl_opengl::Intersection(const alx_point2D &pt, const gpc_vertex_list *VL, const unsigned int pos2, unsigned int &pos)
{gpc_vertex *V = VL->vertex;
 unsigned int i, j;

 if(V[pos2].y == pt.y) {pos++; return 0;}

 if(V[pos].x < V[pos2].x) {i = pos;
                           j = pos2;} else {i = pos2;
                                            j = pos;}
 if (V[j].x < pt.x) {pos++; return 0;}
// Gestion du cas où on passe par un sommet
 if(V[pos].y == pt.y)
  {if(V[pos].x < pt.x) {pos++; return 0;}
   if(pos == (unsigned int)(VL->num_vertices - 1))
    {// Calcul du coté d'arrivé
       int p=pos2;
       while(V[p].y == pt.y) p = (p-1)%VL->num_vertices;
       if(V[p].y < pt.y) cote_arrive = -1; else cote_arrive = 1;
     // Calcul du coté de départ
       p = 0;
       while(V[p].y == pt.y) p = (p+1)%VL->num_vertices;
       if( ((cote_arrive==-1)&&(V[p].y < pt.y)) //Crête
         ||((cote_arrive== 1)&&(V[p].y > pt.y)) //Abîme
         ) return 0; else return 1;
    }
   unsigned int p, pos_original = pos;
   if(true/*cote_arrive == 0*/) {p=pos2;
                         while(V[p].y == pt.y) p = (p-1)%VL->num_vertices;
                         if(V[p].y < pt.y) cote_arrive = -1;
                          else cote_arrive = 1;
                        }
   pos++;
   while( (pos < (unsigned int)(VL->num_vertices))
        &&(V[pos].y == pt.y) ) pos++;
   if(pos == (unsigned int)(VL->num_vertices))
    {int p = 0;
     while(V[p].y == pt.y) p++;
     if( ((cote_arrive==-1)&&(V[p].y < pt.y)) //Crête
       ||((cote_arrive== 1)&&(V[p].y > pt.y)) //Abîme
       ) {return 0;}
    }

   if( ((V[pos].y < pt.y)&&(cote_arrive==-1))
     ||((V[pos].y > pt.y)&&(cote_arrive== 1)) )
                                                return 0;
   return 1;
  } else {cote_arrive = 0;}

 if(V[i].x < pt.x)
  {if(V[j].x < pt.x) {
                      return 0;}
   if( ((V[i].y < pt.y)&&(V[j].y < pt.y))
     ||((V[i].y > pt.y)&&(V[j].y > pt.y)) ) {
                                              return 0;}
   double x_min // abscisse minimum pour qu'il y ait intersection
        , dy1 = V[i].y - pt.y
        , dy2 = pt.y - V[j].y
        , dx1 = pt.x - V[i].x;
   if(dy1 == 0) x_min = pt.x;
     else x_min = pt.x + dx1*dy2/dy1;
   if(x_min <= V[j].x) {
                        return 1;}
  }
  else {if ( ((V[i].y <= pt.y)&&(V[j].y >= pt.y))
           ||((V[i].y >= pt.y)&&(V[j].y <= pt.y)) ) {
                                                     return 1;}
       }

 return 0;
}

//______________________________________________________________________________
info_du_contenant* alx_noeud_polygone_sdl_opengl::Noeud_contient(const alx_point2D &pt, int action)
{info_du_contenant *rep;
// alx_model_simulation_physique1 **mp_rep;

 // Vérifier que le point est bien dans le rectangle de l'image
 if( (pt.x < pt_min[0])
   ||(pt.y < pt_min[1])
   ||(pt.x > pt_max[0])
   ||(pt.y > pt_max[1]) ) {return (info_du_contenant*)NULL;}

// Test d'appartenance du point au polygone par la méthode de la demi-droite de la mort
// Vérifier qu'on coupe bien les l'ensemble des contours un nombre pair de fois

 gpc_vertex_list *VL;
 unsigned int nb_intersections = 0
            , dernier;
 double tmp_x, tmp_y;
 for(unsigned int i=0; i<(unsigned int)poly_ok->num_contours; i++)
  {VL = &(poly_ok->contour[i]);
   dernier = VL->num_vertices - 1;
   unsigned int j = 0;
   cote_arrive = 0; //DEBUG
   while(j < dernier)
    {if(VL->vertex[j].x < pt.x)
      { // Cas où le point est à gauche
      // Placer une sentinelle en X à la fin
       tmp_x = VL->vertex[dernier].x;
       VL->vertex[dernier].x = pt.x + 1;
       while(VL->vertex[j].x < pt.x) j++;
       VL->vertex[dernier].x = tmp_x;
       if( (j == dernier)
         &&(VL->vertex[dernier].x < pt.x) ) break;
       if(j != 0) j--;
      }
    // On s'est ramené au cas où le point courant est à droite ou à la verticale
    // Placer une sentinelle en Y à la fin
     if(VL->vertex[j].y < pt.y)
      {tmp_y = VL->vertex[dernier].y;
       VL->vertex[dernier].y = pt.y + 1;
       while(VL->vertex[j].y < pt.y) j++;
       VL->vertex[dernier].y = tmp_y;
       if( (j == dernier)
         &&(VL->vertex[j].y < pt.y) ) break;
      }
      else {tmp_y = VL->vertex[dernier].y;
            VL->vertex[dernier].y = pt.y - 1;
            while(VL->vertex[j].y > pt.y) j++;
            VL->vertex[dernier].y = tmp_y;
            if( (j == dernier)
              &&(VL->vertex[j].y > pt.y) ) break;
           }
     // Deux points sont susceptibles de définir un segment coupant la demi-droite...
     nb_intersections += Intersection(pt, VL, j-1, j);
     // XXX Vérifier qu'on n'est pas sur le point j, auquel cas il faut incrémenter j pour ne pas retomber dessus au prochain segment
    }
   // Ne pas oublier de considérer le premier et le dernier point comme segment!
   j = 0;
   nb_intersections += Intersection(pt, VL, dernier, j);
  }


 // Prendre en compte l'ordre d'affichage du noeud par rapport aux fils
 if((nb_intersections&1) == 0)
  {return (info_du_contenant*)NULL;}
 rep = new info_du_contenant();
 // Si c'est bon, renvoyer la réponse.
 rep->mp    = &(MP());
 rep->noeud = (alx_noeud_scene*)this;
 rep->Point_au_contact(pt);

 return rep;
}


//___________________________________________________________________________________________________________________________________
//___________________________________________________________________________________________________________________________________
//___________________________________________________________________________________________________________________________________
void alx_noeud_polygone_sdl_opengl::Deseri_info_texture_1(const alx_chaine_char &t)
{cc_Texture_1_desiree = t;
 L_rap_maj_Info_texture_1.Rappeler( this );
}

//___________________________________________________________________________________________________________________________________
const bool alx_noeud_polygone_sdl_opengl::Seri_diff_info_texture_1_a_ete_change(const alx_chaine_char **ptr_cc) const
{bool rep = texture_1_modif;
 texture_1_modif = false;
 if(rep) {
   if(Info_texture()) {*ptr_cc = Info_texture()->Ptr_Nom_const();} else {*ptr_cc = NULL;}
  }
 return rep;
}

//___________________________________________________________________________________________________________________________________
void alx_noeud_polygone_sdl_opengl::Deseri_info_texture_2(const alx_chaine_char &t)
{cc_Texture_2_desiree = t;
 L_rap_maj_Info_texture_2.Rappeler( this );
}

//___________________________________________________________________________________________________________________________________
const bool alx_noeud_polygone_sdl_opengl::Seri_diff_info_texture_2_a_ete_change(const alx_chaine_char **ptr_cc) const
{bool rep = texture_2_modif;
 texture_2_modif = false;
 if(rep) {
   if(Info_texture2()) {*ptr_cc = Info_texture2()->Ptr_Nom_const();} else {*ptr_cc = NULL;}
  }
 return rep;
}

//___________________________________________________________________________________________________________________________________
//___________________________________________________________________________________________________________________________________
//___________________________________________________________________________________________________________________________________
const alx_chaine_char& alx_noeud_polygone_sdl_opengl::Serialiser_difference_type(const alx_chaine_char &diff_contenu) const
{if(serialisation_type_poly.Taille()==0)
  {Serialiser_type();
   serialisation_differences_type_poly = serialisation_type_poly;
   return serialisation_differences_type_poly;}

 serialisation_differences_type_poly.Maj("",0,0);
 nouv_serialisation_type_poly.Maj("",0,0);
   unsigned int pos = Nom_classe().Taille()+2;
   pos = serialisation_type_poly.Position_du_prochain(';', pos); pos++; //On passe nom=...;
   seri_diff_type_poly.Maj("",0,0);
   classe_seri_poly.chaine_cour_ecriture_diff = &seri_diff_type_poly;
   classe_seri_poly.Serialiser_difference_type(pos);
   if( (classe_seri_poly.chaine_cour_ecriture_diff->Taille() > 0)
     ||(diff_contenu.Taille() > 0) )
    {serialisation_differences_type_poly.Maj("<",0,1);
     serialisation_differences_type_poly += Nom_classe();
     serialisation_differences_type_poly.Ajouter(" nom=",0,5);
     serialisation_differences_type_poly += Nom();
     serialisation_differences_type_poly.Ajouter(";",0,1);
     serialisation_differences_type_poly += seri_diff_type_poly;
     serialisation_differences_type_poly.Ajouter(">",0,1);
     serialisation_differences_type_poly += diff_contenu;
     serialisation_differences_type_poly.Ajouter("</",0,2);
     serialisation_differences_type_poly += Nom_classe();
     serialisation_differences_type_poly.Ajouter(">",0,1);}
 serialisation_type_poly.Maj("<",0,1);
 serialisation_type_poly += Nom_classe();
 serialisation_type_poly.Ajouter(" nom=",0,5);
 serialisation_type_poly += Nom();
 serialisation_type_poly.Ajouter(";",0,1);
 serialisation_type_poly += nouv_serialisation_type_poly;
 serialisation_type_poly.Ajouter(">",0,1);
 serialisation_type_poly += alx_noeud_scene::Serialisation_type();
 serialisation_type_poly.Ajouter("</",0,2);
 serialisation_type_poly += Nom_classe();
 serialisation_type_poly.Ajouter(">",0,1);

 return serialisation_differences_type_poly;
}
//___________________________________________________________________________________________________________________________________
const alx_chaine_char& alx_noeud_polygone_sdl_opengl::Serialiser_difference_contenu() const
{return alx_noeud_scene::Serialiser_difference_type();}
//___________________________________________________________________________________________________________________________________
// La sérialisation __________________________________________________________
const alx_chaine_char& alx_noeud_polygone_sdl_opengl::Serialiser_type(const alx_chaine_char &contenu) const
{alx_chaine_char cc_seri_gpc;
 serialisation_type_poly  = "<"+Nom_classe()+" nom="+Nom()+";";
    /*serialisation_type_poly += "0,4="; serialisation_type_poly += Translucidite(); serialisation_type_poly.Ajouter(";",0,1);
    serialisation_type_poly += "0,5="; serialisation_type_poly += Translucidite_perimetre(); serialisation_type_poly.Ajouter(";",0,1);
    serialisation_type_poly += "0,6="; serialisation_type_poly += Lisser()     ; serialisation_type_poly.Ajouter(";",0,1);
    serialisation_type_poly += "0,7="; serialisation_type_poly += Polygone_a_afficher(); serialisation_type_poly.Ajouter(";",0,1);
    serialisation_type_poly += "0,8="; serialisation_type_poly += Perimetre_a_afficher(); serialisation_type_poly.Ajouter(";",0,1);*/
    //___________________________________________________________________________________________________________________________________
    serialisation_type_poly += "4,6=";
      if( Info_texture() ) {serialisation_type_poly += Info_texture()->Nom().Taille();
                            serialisation_type_poly.Ajouter("|",0,1);
                            serialisation_type_poly += Info_texture()->Nom();
                            serialisation_type_poly.Ajouter(";",0,1);} else {serialisation_type_poly += "0|;";}
    serialisation_type_poly += "4,7=";
      if( Info_texture2() ) {serialisation_type_poly += Info_texture2()->Nom().Taille();
                             serialisation_type_poly.Ajouter("|",0,1);
                             serialisation_type_poly += Info_texture2()->Nom();
                             serialisation_type_poly.Ajouter(";",0,1);} else {serialisation_type_poly += "0|;";}


    serialisation_type_poly += "6,8="; serialisation_type_poly += Marqueur_attributs_poly(); serialisation_type_poly.Ajouter(";",0,1);
    const double *tab = Couleur();
      serialisation_type_poly += "3,9=";
      serialisation_type_poly += tab[0]; serialisation_type_poly.Ajouter("_",0,1);
      serialisation_type_poly += tab[1]; serialisation_type_poly.Ajouter("_",0,1);
      serialisation_type_poly += tab[2]; serialisation_type_poly.Ajouter("_",0,1);
      serialisation_type_poly += tab[3];
      serialisation_type_poly.Ajouter(";",0,1);
    //
    tab = Couleur_perimetre();
      serialisation_type_poly += "3,10=";
      serialisation_type_poly += tab[0]; serialisation_type_poly.Ajouter("_",0,1);
      serialisation_type_poly += tab[1]; serialisation_type_poly.Ajouter("_",0,1);
      serialisation_type_poly += tab[2]; serialisation_type_poly.Ajouter("_",0,1);
      serialisation_type_poly += tab[3];
      serialisation_type_poly.Ajouter(";",0,1);

    serialisation_type_poly += "7,11=";
      cc_seri_gpc  = poly_ok->num_contours;
        cc_seri_gpc.Ajouter("_",0,1);
      for(unsigned int i=0; i<(unsigned int)(poly_ok->num_contours); i++)
       {gpc_vertex_list *VL = &(poly_ok->contour[i]);
        cc_seri_gpc += poly_ok->hole[i];
        cc_seri_gpc.Ajouter("_",0,1);
        cc_seri_gpc += VL->num_vertices;
        cc_seri_gpc.Ajouter("_",0,1);
        for(unsigned int j=0; j<(unsigned int)(VL->num_vertices); j++)
         {cc_seri_gpc += VL->vertex[j].x;
          cc_seri_gpc.Ajouter("'",0,1);
          cc_seri_gpc += VL->vertex[j].y;
          cc_seri_gpc.Ajouter("_",0,1);}
        cc_seri_gpc.Ajouter("|",0,1);
       }
      serialisation_type_poly += cc_seri_gpc.Taille(); serialisation_type_poly.Ajouter("_",0,1); serialisation_type_poly += cc_seri_gpc;
    serialisation_type_poly.Ajouter(";",0,1);

    tab = Const_couleur_d();
      serialisation_type_poly += "3,12=";
      serialisation_type_poly += tab[0]; serialisation_type_poly.Ajouter("_",0,1);
      serialisation_type_poly += tab[1]; serialisation_type_poly.Ajouter("_",0,1);
      serialisation_type_poly += tab[2]; serialisation_type_poly.Ajouter("_",0,1);
      serialisation_type_poly += tab[3];
      serialisation_type_poly.Ajouter(";",0,1);

    serialisation_type_poly += "1,13="; serialisation_type_poly += Alpha_scale                           (); serialisation_type_poly.Ajouter(";",0,1);
    serialisation_type_poly += "1,14="; serialisation_type_poly += RGB_scale                             (); serialisation_type_poly.Ajouter(";",0,1);
    serialisation_type_poly += "1,15="; serialisation_type_poly += Constante_combinaison_couleurs        (); serialisation_type_poly.Ajouter(";",0,1);
    serialisation_type_poly += "1,16="; serialisation_type_poly += Constante_combinaison_textures        (); serialisation_type_poly.Ajouter(";",0,1);

    serialisation_type_poly += "6,17="; serialisation_type_poly += Combinaison_couleurs_source_arg2      (); serialisation_type_poly.Ajouter(";",0,1);
    serialisation_type_poly += "6,18="; serialisation_type_poly += Combinaison_couleurs_operande2        (); serialisation_type_poly.Ajouter(";",0,1);
    serialisation_type_poly += "6,19="; serialisation_type_poly += Combinaison_textures_source_arg2      (); serialisation_type_poly.Ajouter(";",0,1);
    serialisation_type_poly += "6,20="; serialisation_type_poly += Combinaison_textures_operande2        (); serialisation_type_poly.Ajouter(";",0,1);
    serialisation_type_poly += "6,21="; serialisation_type_poly += Ordre_application_textures_rgb        (); serialisation_type_poly.Ajouter(";",0,1);
    serialisation_type_poly += "6,22="; serialisation_type_poly += Ordre_application_textures_alpha      (); serialisation_type_poly.Ajouter(";",0,1);
    serialisation_type_poly += "6,23="; serialisation_type_poly += Combinaison_textures_rgb              (); serialisation_type_poly.Ajouter(";",0,1);
    serialisation_type_poly += "6,24="; serialisation_type_poly += Combinaison_textures_alpha            (); serialisation_type_poly.Ajouter(";",0,1);
    serialisation_type_poly += "6,25="; serialisation_type_poly += Ordre_application_couleurs_rgb        (); serialisation_type_poly.Ajouter(";",0,1);
    serialisation_type_poly += "6,26="; serialisation_type_poly += Ordre_application_couleurs_alpha      (); serialisation_type_poly.Ajouter(";",0,1);
    serialisation_type_poly += "6,27="; serialisation_type_poly += Combinaison_couleurs_rgb              (); serialisation_type_poly.Ajouter(";",0,1);
    serialisation_type_poly += "6,28="; serialisation_type_poly += Combinaison_couleurs_alpha            (); serialisation_type_poly.Ajouter(";",0,1);

    serialisation_type_poly += "6,29="; serialisation_type_poly += Combinaison_couleurs_source_arg0      (); serialisation_type_poly.Ajouter(";",0,1);
    serialisation_type_poly += "6,30="; serialisation_type_poly += Combinaison_couleurs_operande0        (); serialisation_type_poly.Ajouter(";",0,1);
    serialisation_type_poly += "6,31="; serialisation_type_poly += Combinaison_couleurs_source_arg1      (); serialisation_type_poly.Ajouter(";",0,1);
    serialisation_type_poly += "6,32="; serialisation_type_poly += Combinaison_couleurs_operande1        (); serialisation_type_poly.Ajouter(";",0,1);
    serialisation_type_poly += "6,33="; serialisation_type_poly += Combinaison_couleurs_source_arg2      (); serialisation_type_poly.Ajouter(";",0,1);
    serialisation_type_poly += "6,34="; serialisation_type_poly += Combinaison_couleurs_operande2        (); serialisation_type_poly.Ajouter(";",0,1);
    serialisation_type_poly += "6,35="; serialisation_type_poly += Combinaison_couleurs_source_arg0_Alpha(); serialisation_type_poly.Ajouter(";",0,1);
    serialisation_type_poly += "6,36="; serialisation_type_poly += Combinaison_couleurs_operande0_Alpha  (); serialisation_type_poly.Ajouter(";",0,1);
    serialisation_type_poly += "6,37="; serialisation_type_poly += Combinaison_couleurs_source_arg1_Alpha(); serialisation_type_poly.Ajouter(";",0,1);
    serialisation_type_poly += "6,38="; serialisation_type_poly += Combinaison_couleurs_operande1_Alpha  (); serialisation_type_poly.Ajouter(";",0,1);
    serialisation_type_poly += "6,39="; serialisation_type_poly += Combinaison_textures_source_arg0      (); serialisation_type_poly.Ajouter(";",0,1);
    serialisation_type_poly += "6,40="; serialisation_type_poly += Combinaison_textures_operande0        (); serialisation_type_poly.Ajouter(";",0,1);
    serialisation_type_poly += "6,41="; serialisation_type_poly += Combinaison_textures_source_arg1      (); serialisation_type_poly.Ajouter(";",0,1);
    serialisation_type_poly += "6,42="; serialisation_type_poly += Combinaison_textures_operande1        (); serialisation_type_poly.Ajouter(";",0,1);
    serialisation_type_poly += "6,43="; serialisation_type_poly += Combinaison_textures_source_arg2      (); serialisation_type_poly.Ajouter(";",0,1);
    serialisation_type_poly += "6,44="; serialisation_type_poly += Combinaison_textures_operande2        (); serialisation_type_poly.Ajouter(";",0,1);
    serialisation_type_poly += "6,45="; serialisation_type_poly += Combinaison_textures_source_arg0_Alpha(); serialisation_type_poly.Ajouter(";",0,1);
    serialisation_type_poly += "6,46="; serialisation_type_poly += Combinaison_textures_operande0_Alpha  (); serialisation_type_poly.Ajouter(";",0,1);
    serialisation_type_poly += "6,47="; serialisation_type_poly += Combinaison_textures_source_arg1_Alpha(); serialisation_type_poly.Ajouter(";",0,1);
    serialisation_type_poly += "6,48="; serialisation_type_poly += Combinaison_textures_operande1_Alpha  (); serialisation_type_poly.Ajouter(";",0,1);

 serialisation_type_poly += ">"+contenu+"</"+Nom_classe()+">";
 return serialisation_type_poly;
}
//___________________________________________________________________________________________________________________________________
const alx_chaine_char& alx_noeud_polygone_sdl_opengl::Serialiser_contenu() const
{return alx_noeud_scene::Serialiser_type();}
//___________________________________________________________________________________________________________________________________
// La désérialisation ________________________________________________________
void alx_noeud_polygone_sdl_opengl::Deserialiser_type(const alx_chaine_char &txt, unsigned int &pos)
{//unsigned int num_f, num_a;
 const char *texte = txt.Texte();

 classe_seri_poly.chaine_cour = &txt;
 if(texte[pos] == '<') pos+= Nom_classe().Taille()+2; // on passe "<polygon "
 pos = txt.Position_du_prochain(';', pos); pos++;   //On passe nom=...;

 classe_seri_poly.Deserialiser_type(txt, pos);
 pos++; // On passe le >
 if( (texte[pos]   == '<')
   &&(texte[pos+1] != '/') )
  Deserialiser(txt, pos);

 pos += Nom_classe().Taille()+3; // On passe la balise de fin "</polygon>"
 if ( classe_seri_poly.Recharger_polygone() )
   Maj_poly_gpc( classe_seri_poly.get_poly_tmp() );
}
//___________________________________________________________________________________________________________________________________
void alx_noeud_polygone_sdl_opengl::Deserialiser(const alx_chaine_char &txt, unsigned int &pos)
{alx_noeud_scene::Deserialiser_type(txt, pos);}

//___________________________________________________________________________________________________________________________________
Fonction_interpolation_couleur_polygone alx_noeud_polygone_sdl_opengl::Fct_interpollation() const
{return interpollateur->Fct();}

//___________________________________________________________________________________________________________________________________
void alx_noeud_polygone_sdl_opengl::Fct_interpollation(Fonction_interpolation_couleur_polygone f)
{interpollateur->Fct(f);}

//___________________________________________________________________________________________________________________________________
Fonction_interpolation_couleur_polygone alx_noeud_polygone_sdl_opengl::Fct_interpollation_perimetre() const
{return interpollateur_perimetre->Fct();}

//___________________________________________________________________________________________________________________________________
void alx_noeud_polygone_sdl_opengl::Fct_interpollation_perimetre(Fonction_interpolation_couleur_polygone f)
{interpollateur_perimetre->Fct(f);}

//___________________________________________________________________________________________________________________________________
//___________________________________________________________________________________________________________________________________
//___________________________________________________________________________________________________________________________________
// Col et Tex contiennent les coordonnées couleurs et texture de (x, y).
// Ces coordonnées seront calculées par rapport à la boite englobante du noeud.
void Fonction_interpolation_poly_boite_englobante( const alx_noeud_polygone_sdl_opengl &poly
                                                 , const double x, const double y
                                                 , const double *Tab_col, const double *Tab_tex, const double *Tab_tex2
                                                 , double *Col, double *Tex, double *Tex2
                                                 , void *params)
{const boite_englobante &boite = poly.Boite_noeud();
 const double rx = (x-boite.X1()) / boite.Tx()
            , ry = (y-boite.Y1()) / boite.Ty() ;
// Couleurs
 if(Tab_col)
  {for(unsigned int i=0; i<4; i++) // Pour chaque canal
    {Col[i]  = (1-ry)*(rx*Tab_col[4+i] + (1-rx)*Tab_col[i]   )
             +    ry *(rx*Tab_col[8+i] + (1-rx)*Tab_col[12+i]);
    }
  }
// Texture 1
 if(poly.Info_texture() && Tex)
  {if(Tab_tex)
     {for(unsigned int i=0; i<2; i++) // Pour chaque canal
       {Tex[i]  = (1-ry)*(rx*Tab_tex[2+i] + (1-rx)*Tab_tex[i]   )
                +     ry*(rx*Tab_tex[4+i] + (1-rx)*Tab_tex[6+i]);
       }
     } else {const INFOS_TEXTURE *info_texture = poly.Info_texture();
             double tx = (double)info_texture->Taille_reelle_x()/(double)info_texture->Tx()
                  , ty = (double)info_texture->Taille_reelle_y()/(double)info_texture->Ty()
                  , Ox = (double)info_texture->Start_x()        /(double)info_texture->Tx()
                  , Oy = (double)info_texture->Start_y()        /(double)info_texture->Ty();
             Tex[0] = Ox + tx * rx;
             Tex[1] = Oy + ty * ry;
            }
  }
// Texture 2
 if(poly.Info_texture2() && Tex2)
  {if(Tab_tex2) {
     for(unsigned int i=0; i<2; i++) // Pour chaque canal
      {Tex2[i]  = (1-ry)*(rx*Tab_tex2[2+i] + (1-rx)*Tab_tex2[i]   )
                +     ry*(rx*Tab_tex2[4+i] + (1-rx)*Tab_tex2[6+i]);
      }
    } else {const INFOS_TEXTURE *info_texture2 = poly.Info_texture2();
            double tx2 = (double)info_texture2->Taille_reelle_x()/(double)info_texture2->Tx()
                       , ty2 = (double)info_texture2->Taille_reelle_y()/(double)info_texture2->Ty()
                       , Ox2 = (double)info_texture2->Start_x()        /(double)info_texture2->Tx()
                       , Oy2 = (double)info_texture2->Start_y()        /(double)info_texture2->Ty();
            Tex2[0] = Ox2 + tx2 * rx;
            Tex2[1] = Oy2 + ty2 * ry;
           }
  }
}

//___________________________________________________________________________________________________________________________________
void Interpollateur_couleur_polygone::init()
{Col = (double*)NULL;
 Tex = (double*)NULL;
 fct = NULL;
}
//___________________________________________________________________________________________________________________________________
void Interpollateur_couleur_polygone::Interpoller( const alx_noeud_polygone_sdl_opengl &poly
                                                 , const double *Tab_col, const double *Tab_tex, const double *Tab_tex2)
{if(!L_rappel_avant_interpollation.Vide())
   L_rappel_avant_interpollation.Rappeler( this );
 Fct()(poly, X(), Y(), Tab_col, Tab_tex, Tab_tex2, Couleur(), Texture(), Texture2(), Params());
}

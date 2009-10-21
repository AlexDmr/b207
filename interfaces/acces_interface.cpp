#include "acces_interface.h"

alx_interface_sdl_opengl_1 *interface_courante = (alx_interface_sdl_opengl_1*)NULL;
alx_interface_sdl_opengl_1* Interface_mere()       {return interface_courante;}
void Interface_mere(alx_interface_sdl_opengl_1 *i) {interface_courante = i;}



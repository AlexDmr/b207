#ifndef __ALX_TEXTURE_RENDU_H__
#define __ALX_TEXTURE_RENDU_H__

#include <windows.h>
#include <extgl.h>

class alx_texture_rendu
{
 private:
   static const int MAX_ATTRIBS  = 256;
   static const int MAX_PFORMATS = 256;

   HPBUFFERARB  hpbuffer;      // Handle to a pbuffer.
   HDC          hdc;           // Handle to a device context.
   HGLRC        hglrc;         // Handle to a GL rendering context.
   int          width;         // Width of the pbuffer
   int          height;        // Height of the pbuffer

 public :
   alx_texture_rendu()
    {
     width = height = -1;
     hpbuffer = NULL;
    }
    
   ~alx_texture_rendu()
    {

    }

   bool Initialiser(int x, int y);
   void Devenir_cible__du_rendu_opengl();

   inline void Hpbuffer(HPBUFFERARB h) {hpbuffer = h;}
/*   inline void Hauteur (int        ht) {height = ht;}
   inline void Largeur (int         l) {width = l;}*/
   inline void Hglrc   (HGLRC       h) {hglrc = h;}
   inline void Hdc     (HDC         h) {hdc = h;}
   inline void Dimension(int h, int l) {height = h; width = l;}

   inline HPBUFFERARB Hpbuffer() {return hpbuffer;}
   inline int         Hauteur () {return height;}
   inline int         Largeur () {return width;}
   inline HGLRC       Hglrc   () {return hglrc;}
   inline HDC         Hdc     () {return hdc;}
};

#endif
 
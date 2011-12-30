#ifndef __TEXTE_H__
#define __TEXTE_H__

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>

//#include <gl.h>

class gl_fonte
{
 private:
   UINT id_fonte;
   HFONT hOldFont;
   int nb_char;
   double profondeur_fonte
        , hauteur;
   HDC hdc;
   GLYPHMETRICSFLOAT *g_GlyphInfo;

   UINT CreerFonteOpenGL(LPCWSTR NomFonte, float profondeur);
   double Calculer_hauteur();
   int mode; //0:Pas d'annalyse des %, 1:Annalyse des %
/*
HWND  g_hWnd;											// This is the handle for the window
RECT  g_rRect;											// This holds the window dimensions
HDC   g_hDC;											// General HDC - (handle to device context)
HGLRC g_hRC;											// General OpenGL_DC - Our Rendering Context for OpenGL
HINSTANCE g_hInstance;									// This holds the global hInstance for UnregisterClass() in DeInit()
*/
 public:
  // Les constructeurs.
   gl_fonte(char *nom_fonte, float p);
   ~gl_fonte();

  // les méthodes.
   double ecrire(int &NbCarEcrits, const float taille_max, char *strString, ...);
   double longueur_effective(int &NbCarEcrits, const float taille_max, char *strText);
   unsigned int Position_lettre(float dec, unsigned char *strText);
   const double Largeur_char(const char c) const {return g_GlyphInfo[c].gmfCellIncX;}

   inline const UINT  ID_FONTE()   const {return id_fonte;}
   inline const double PROFONDEUR() const {return profondeur_fonte;}
   inline const double HAUTEUR()    const {return hauteur;}
   inline void  Mode(const int m)  {mode = m;}
   inline const int   Mode() const       {return mode;}
};

#endif


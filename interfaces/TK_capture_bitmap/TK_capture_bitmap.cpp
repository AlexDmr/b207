#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <tcl.h>
#include <tk.h>
#include <tkPlatDecls.h>

//implib "C:/TCL/lib/tcl84.lib"
//#import <tk84.lib>

//_______________________________________________________________________________
// CORE
//_______________________________________________________________________________
char TK_capture_bitmap_TCL_answer[4096];
char *Buffer_for_HBITMAP_pixels = NULL;
unsigned int taille_Buffer_for_HBITMAP_pixels = 0;

void* get_Buffer_for_HBITMAP_pixels() {return Buffer_for_HBITMAP_pixels;}

#include "tk_capture_bitmap_SWIG_wrap.cxx"
//_______________________________________________________________________________
char* get_Buffer_for_HBITMAP_pixels(const unsigned int t)
{if(t > taille_Buffer_for_HBITMAP_pixels) {
   if(Buffer_for_HBITMAP_pixels) {delete[] Buffer_for_HBITMAP_pixels;}
   taille_Buffer_for_HBITMAP_pixels = t;
   Buffer_for_HBITMAP_pixels = new char[t];
  }
 return Buffer_for_HBITMAP_pixels;
}

//_______________________________________________________________________________
void Tcl_FreeRes(char *r) {if(r) delete[] r;}

//_______________________________________________________________________________
int Get_bitmap_from_TK_win( ClientData clientData
                          , Tcl_Interp *interp
                          , int argc
                          , const char *argv[] )
{
// Un seul argument, la fenêtre TK
 if(argc != 2) {Tcl_AddErrorInfo(interp, "Usage : Get_bitmap_from_TK_win TK_window"); return TCL_ERROR;}
 const char *fen = argv[1];
 Tk_Window const tkmainwin = Tk_MainWindow(interp);
   if (tkmainwin==NULL) {Tcl_AddErrorInfo(interp, "Impossible to find the main window for TK..."); return TCL_ERROR;}
 Tk_Window const tkwin = Tk_NameToWindow(interp,fen,tkmainwin);
   if (tkwin==NULL) {Tcl_AddErrorInfo(interp, "Impossible to find a TK window of this name..."); return TCL_ERROR;}
 HWND hWnd = Tk_GetHWND(Tk_WindowId(tkwin));
   if(hWnd==NULL) {Tcl_AddErrorInfo(interp, "The HWND for this TK window is NULL..."); return TCL_ERROR;}

// On a la fenêtre système, on aura le bitmap...
 HDC hDC    = GetWindowDC(hWnd);
 HDC hMemDC = CreateCompatibleDC(hDC);
 RECT r;
   GetWindowRect(hWnd,&r);
 SIZE size;
   size.cx = r.right-r.left;
   size.cy = r.bottom-r.top;
 HBITMAP hBitmap = CreateCompatibleBitmap(hDC, size.cx, size.cy);
 sprintf(TK_capture_bitmap_TCL_answer, "ERROR");
 if (hBitmap)
    {   HBITMAP hOld = (HBITMAP) SelectObject(hMemDC, hBitmap);
		BitBlt(hMemDC, 0, 0, size.cx, size.cy, hDC, 0, 0, SRCCOPY);
        SelectObject(hMemDC, hOld);
        DeleteDC(hMemDC);
		BITMAP bmp;
          GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bmp);
        unsigned int taille_pixel = bmp.bmBitsPixel / 8;
        unsigned int taille = taille_pixel * bmp.bmHeight * bmp.bmWidth;
		char *ptr     = get_Buffer_for_HBITMAP_pixels(taille);

		ReleaseDC(NULL, hDC);
		int nb_copied = GetBitmapBits (hBitmap, taille, ptr);
		sprintf(TK_capture_bitmap_TCL_answer, "%d %d %d (nb_copied = %d)", bmp.bmWidth, bmp.bmHeight, taille_pixel, nb_copied);

        DeleteObject(hBitmap);
    }



 Tcl_SetResult(interp, TK_capture_bitmap_TCL_answer, TCL_STATIC);
 return TCL_OK;
}

//_______________________________________________________________________________
// INIT
//_______________________________________________________________________________
int init(Tcl_Interp *interp)
{if (interp == 0) return TCL_ERROR;

 Tcl_PkgProvide(interp, "TkCaptureBitmap", "8.4.14.0.2");
 Tcl_CreateCommand(interp, "Get_bitmap_from_TK_win", Get_bitmap_from_TK_win, NULL, NULL);
 SWIG_init(interp);
 return TCL_OK;
}

int Tk_capture_bitmap_SafeInit(Tcl_Interp *interp) {
  return init(interp);
}

int Tk_capture_bitmap_Init(Tcl_Interp *interp) {
  return init(interp);
}

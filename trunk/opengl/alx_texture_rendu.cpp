#include "alx_texture_rendu.h"

bool alx_texture_rendu::Initialiser(int x, int y)
    {
     Dimension(x, y);
     HDC hdc = wglGetCurrentDC();
     HGLRC hglrc = wglGetCurrentContext();
     
     int     format;
     int     pformat[MAX_PFORMATS];
     unsigned int nformats;    int     iattributes[2*MAX_ATTRIBS];
     float   fattributes[2*MAX_ATTRIBS];
     int     nfattribs = 0;
     int     niattribs = 0;

     // Attribute arrays must be "0" terminated - for simplicity, first
     // just zero-out the array entire, then fill from left to right.
     memset(iattributes,0,sizeof(int)*2*MAX_ATTRIBS);
     memset(fattributes,0,sizeof(float)*2*MAX_ATTRIBS);
     // Since we are trying to create a pbuffer, the pixel format we
     // request (and subsequently use) must be "p-buffer capable".
     iattributes[niattribs  ] = WGL_DRAW_TO_PBUFFER_ARB;
     iattributes[++niattribs] = GL_TRUE;
     // we are asking for a pbuffer that is meant to be bound
     // as an RGBA texture - therefore we need a color plane
     iattributes[++niattribs] = WGL_BIND_TO_TEXTURE_RGBA_ARB;
     iattributes[++niattribs] = GL_TRUE;

     if ( !wglChoosePixelFormatARB( hdc, iattributes, fattributes, MAX_PFORMATS, pformat, &nformats ) )
      {
       //cerr << "pbuffer creation error:  wglChoosePixelFormatARB() failed.\n";
       exit( -1 );
      }
     //wglGetLastError();
     if ( nformats <= 0 )
      {
       //cerr << "pbuffer creation error:  Couldn't find a suitable pixel format.\n";
       exit( -1 );
      }
     format = pformat[0];

     // Set up the pbuffer attributes
     memset(iattributes,0,sizeof(int)*2*MAX_ATTRIBS);
     niattribs = 0;
     // the render texture format is RGBA
     iattributes[niattribs] = WGL_TEXTURE_FORMAT_ARB;
     iattributes[++niattribs] = WGL_TEXTURE_RGBA_ARB;
     // the render texture target is GL_TEXTURE_2D
     iattributes[++niattribs] = WGL_TEXTURE_TARGET_ARB;
     iattributes[++niattribs] = WGL_TEXTURE_2D_ARB;
     // ask to allocate room for the mipmaps
     //iattributes[++niattribs] = WGL_MIPMAP_TEXTURE_ARB;
     //iattributes[++niattribs] = TRUE;
     // ask to allocate the largest pbuffer it can, if it is
     // unable to allocate for the width and height
     iattributes[++niattribs] = WGL_PBUFFER_LARGEST_ARB;
     iattributes[++niattribs] = FALSE;
     // Create the p-buffer.
     this->hpbuffer = wglCreatePbufferARB( hdc, format, width, height, iattributes );
     if ( this->hpbuffer == 0)
      {
       //cerr << "pbuffer creation error:  wglCreatePbufferARB() failed\n";
       //wglGetLastError();
       exit( -1 );
      }
     //wglGetLastError();

     // Get the device context.
     this->hdc = wglGetPbufferDCARB( this->hpbuffer );
     if ( this->hdc == 0)
      {
       //cerr << "pbuffer creation error:  wglGetPbufferDCARB() failed\n";
       //wglGetLastError();
       exit( -1 );
      }
     //wglGetLastError();

     // Create a gl context for the p-buffer.
     this->hglrc = wglCreateContext( this->hdc );
     if ( this->hglrc == 0)
      {
        //cerr << "pbuffer creation error:  wglCreateContext() failed\n";
        //wglGetLastError();
       exit( -1 );
      }
     //wglGetLastError();

     // Determine the actual width and height we were able to create.
     wglQueryPbufferARB( this->hpbuffer, WGL_PBUFFER_WIDTH_ARB, &(this->width) );
     wglQueryPbufferARB( this->hpbuffer, WGL_PBUFFER_HEIGHT_ARB, &(this->height) );

 return true;     
}

void alx_texture_rendu::Devenir_cible__du_rendu_opengl()
{
 wglMakeCurrent( this->hdc
               , this->hglrc );
}

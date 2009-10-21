#include "texte.h"
#include "..\physique\math_alex.cpp"
#include <stdio.h>
#include <stdlib.h>
#include <extgl.h>
#include <glu.h>										// Header File For The GLu32 Library

///////////////////////////////// CREATE OPENGL FONT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function creates a windows font like "Arial" and returns a display list ID
/////
///////////////////////////////// CREATE OPENGL FONT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

UINT gl_fonte::CreerFonteOpenGL(LPSTR NomFonte, float profondeur)
{
 HFONT	hFont;

	// Here we generate the lists for each character we want to use.
	// This function then returns the base pointer, which will be 1 because
	// we haven't created any other lists.  If we generated another list after
	// this, the base pointer would be at 257 since the last one used was 256 (which is MAX_CHARS)
 id_fonte = glGenLists(nb_char);

	// Now we actually need to create the font.  We use a windows function called:
	// CreateFont() that returns a handle to a font (HFONT).  Our CreateOpenGLFont()
	// function allows us to pass in a name and height.  For simplistic reasons, I left
	// other options out, but feel free to add them to your function (like bold, italic, width..)
	// In the last tutorial we passed in a height, now we pass in a extrude value.  This
	// value basically makes the thickness in 3D.  The height parameter in CreateFont() doesn't
	// matter now, so it doesn't really matter what value we put in, it is just ignored by wglUseFontOutlines().

	hFont = CreateFont(	0,								// This HEIGHT parameter is ignored by wglUseFontOutlines(). Use glScalef()
						0,								// The WIDTH (If we leave this zero it will pick the best width depending on the height)
						0,								// The angle of escapement
						0,								// The angle of orientation
						FW_NORMAL,						// The font's weight (We want it bold)
						FALSE,							// Italic - We don't want italic
						FALSE,							// Underline - We don't want it underlined
						FALSE,							// Strikeout - We don't want it strikethrough
						ANSI_CHARSET,					// This is the type of character set
			/*OUT_RASTER_PRECIS	*/	OUT_TT_PRECIS,					// The Output Precision
						CLIP_DEFAULT_PRECIS ,			// The Clipping Precision
						ANTIALIASED_QUALITY,			// The quality of the font - We want anitaliased fonts
						FF_DONTCARE|DEFAULT_PITCH,		// The family and pitch of the font.  We don't care.
						NomFonte);				    	// The font name (Like "Arial", "Courier", etc...)

	// Now that we have created a new font, we need to select that font into our global HDC.
	// We store the old font so we can select it back in when we are done to avoid memory leaks.
	hOldFont = (HFONT)SelectObject(hdc, hFont);

	// This is the function that creates all the magic.  This will actually output the
	// polygons into our display lists, which creates the outlined characters.
	// But first, we need to give it our desired information.
	// One new thing from the last tutorial is GLYPH's.  A glyph is a 3D character.
	// The GLYPHMETRICSFLOAT structure used holds information about that 3D character,
	// such as it's width and height in 3D units, as well as orientation within it's bounding box.
	// We can use this information if we want to find out how long our sentences are in actual
	// unit lengths, not pixel length.

       // TEST
       //CA foire, pas de rotation possible : wglUseFontBitmaps(hdc, 0, nb_char, id_fonte);
       // On revient à la 3D :
	wglUseFontOutlines(	hdc,							// This is the global HDC with the desired font selected.
						0,								// This is the starting ASCII value.
						nb_char - 1,					// This is the ending ASCII value to use (255 is the last).
						id_fonte,						// This is the base pointer of our display list we wish to use.
						0,								// This is the deviation from a true outline (floating point)
						profondeur,							// This is the extrude value, or in other words, the thickness in Z.
						WGL_FONT_POLYGONS,				// We specify here that we want it rendered in polygons rather than lines
						g_GlyphInfo);					// The address to the buffer that will hold the 3D font info for each character.
 hauteur = Calculer_hauteur();
 return 0;
}

float gl_fonte::Calculer_hauteur()
{
 float rep = 0;

 for(int i=0;i<nb_char;i++)
  {
   rep = Amax(rep, g_GlyphInfo[i].gmfBlackBoxY);
  }

 return 1.2*rep;
}

unsigned int gl_fonte::Position_lettre(float dec, unsigned char *strText)
{
 float dec_accu = 0.0000f;
 unsigned int rep=0;
 unsigned char c = strText[rep];

 while( (c) // != 0
      &&(dec_accu < dec) )
  {
   dec_accu += g_GlyphInfo[c].gmfCellIncX;
   c = strText[++rep];
  }

 return Amax(0, (int)rep-1);
}

float gl_fonte::longueur_effective(int &NbCarEcrits, const float taille_max, unsigned char *strText)
{
 // Below we find out the total length of the characters in 3D units, then center them.
 float unitLength=0.0000f;
 int taille = (int)strlen(strText)
   , p;

 NbCarEcrits = 0;
 for (int i=0; i < taille; i++)		// Go through all of the characters
  {
  // Here we go through each character and add up it's length.
  // It doesn't matter where the text if facing in 3D, it uses a 2D system
  // for the width and height of the characters.  So X is the width, Y is the height.
   p = strText[i];
   if(unitLength + g_GlyphInfo[p].gmfCellIncX <= taille_max)
    {
     unitLength += g_GlyphInfo[p].gmfCellIncX;
     NbCarEcrits++;
     //if(strText[i]NbMaxCarSurUneLigne++
    }
    else break;
  }

 return unitLength;
}

float gl_fonte::ecrire(int &NbCarEcrits, const float taille_max, unsigned char *strString, ...)
{
	// First we need to check if there was even a string given
	if (strString == NULL)								// Check if a string was given
		return 0.0;											// Don't render anything then


	unsigned char  *strText;							// This will hold our text to display
        unsigned char   tab_texte[65536];
	va_list		argumentPtr;							// This will hold the pointer to the argument list
	float		unitLength;						// This will store the length of the 3D Font in unit length


        if(mode == 1)
         {
          strText = tab_texte;
   	  // First we need to parse the string for arguments given
	  // To do this we pass in a va_list variable that is a pointer to the list of arguments.
	  // Then we pass in the string that holds all of those arguments.
	  va_start(argumentPtr, strString);					// Parse the arguments out of the string

	  // Then we use a special version of sprintf() that takes a pointer to the argument list.
	  // This then does the normal sprintf() functionality.
	  vsprintf(strText, strString, argumentPtr);			// Now add the arguments into the full string

	  va_end(argumentPtr);								// This resets and frees the pointer to the argument list.
         }
         else strText = strString;

	 // If you have never used a va_list, listen up.  Remember printf()?
	 // or sprintf()?  Well, you can add unlimited arguments into the text like:
	 // printf("My name is %s and I am %d years old!", strName, age);
	 // Well, that is what va_list's do.



	// Here we translate the text centered around the XYZ according to it's width.
	// Since we have the length, we can just divide it by 2, then subtract that from the X.
	// This will then center the text at that position, in the way of width.
	// Can you see how to center the height too if you wanted? (hint: .gmfCellIncY)
	//glTranslatef(0.0f - (unitLength / 2), 0.0f, 0.0f);

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *
        unitLength = longueur_effective(NbCarEcrits, taille_max, strText);

	// Now, before we set the list base, we need to save off the current one.
	glPushAttrib(GL_LIST_BIT);							// This saves the list base information

	// Then we want to set the list base to the font's list base, which should be 1 in our case.
	// That way when we call our display list it will start from the font's lists'.
	glListBase(id_fonte + 1);							// This sets the lists base

	// Now comes the actually rendering.  We pass in the length of the string,
	// then the data types (which are characters so its a UINT), then the actually char array.
	// This will then take the ASCII value of each character and associate it with a bitmap.
	glCallLists(NbCarEcrits/*strlen(strText)*/, GL_UNSIGNED_BYTE, strText);

	glPopAttrib();										// Return the display list back to it's previous state

        return unitLength;
    //glTranslatef(0.0f - (unitLength / 2), 0.0f, 0.0f);

/*


	char		strText[256];							// This will hold our text to display
	va_list		argumentPtr;							// This will hold the pointer to the argument list

	// If you have never used a va_list, listen up.  Remember printf()?
	// or sprintf()?  Well, you can add unlimited arguments into the text like:
	// printf("My name is %s and I am %d years old!", strName, age);
	// Well, that is what va_list's do.

	// First we need to check if there was even a string given
	if (strString == NULL)								// Check if a string was given
		return;											// Don't render anything then

	// First we need to parse the string for arguments given
	// To do this we pass in a va_list variable that is a pointer to the list of arguments.
	// Then we pass in the string that holds all of those arguments.
	va_start(argumentPtr, strString);					// Parse the arguments out of the string

	// Then we use a special version of sprintf() that takes a pointer to the argument list.
	// This then does the normal sprintf() functionality.
	vsprintf(strText, strString, argumentPtr);			// Now add the arguments into the full string

	va_end(argumentPtr);								// This resets and frees the pointer to the argument list.




	va_end(argumentPtr);								// This resets and frees the pointer to the argument list.

	// Below we find out the total length of the characters in 3D units, then center them.

    float unitLength;
	for (int i=0; i < (int)strlen(strText); i++)		// Go through all of the characters
	{
		// Here we go through each character and add up it's length.
		// It doesn't matter where the text if facing in 3D, it uses a 2D system
		// for the width and height of the characters.  So X is the width, Y is the height.
		unitLength += g_GlyphInfo[strText[i]].gmfCellIncX;
	}

	// Here we translate the text centered around the XYZ according to it's width.
	// Since we have the length, we can just divide it by 2, then subtract that from the X.
	// This will then center the text at that position, in the way of width.
	// Can you see how to center the height too if you wanted? (hint: .gmfCellIncY)
	glTranslatef(0.0f - (unitLength / 2), 0.0f, 0.0f);

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


	// Now, before we set the list base, we need to save off the current one.
	glPushAttrib(GL_LIST_BIT);							// This saves the list base information

	// Then we want to set the list base to the font's list base, which should be 1 in our case.
	// That way when we call our display list it will start from the font's lists'.
	glListBase(g_FontListID);							// This sets the lists base

	// Now comes the actually rendering.  We pass in the length of the string,
	// then the data types (which are characters so its a UINT), then the actually char array.
	// This will then take the ASCII value of each character and associate it with a bitmap.
	glCallLists(strlen(strText), GL_UNSIGNED_BYTE, strText);

	glPopAttrib();										// Return the display list back to it's previous state









/*
	// Before we draw the text, we need to position it with our own function.
	PositionText(x, y, ecran_x, ecran_y, rotation);									// Call our own function to position the text on screen

	// Now, before we set the list base, we need to save off the current one.
	glPushAttrib(GL_LIST_BIT);							// This saves the list base information

	// Then we want to set the list base to the font's list base, which should be 1 in our case.
	// That way when we call our display list it will start from the font's lists'.
	glListBase(id_fonte+1);							// This sets the lists base

	// Now comes the actually rendering.  We pass in the length of the string,
	// then the data types (which are characters so its a UINT), then the actually char array.
	// This will then take the ASCII value of each character and associate it with a bitmap.
	glCallLists(strlen(strText), GL_UNSIGNED_BYTE, strText);

	glPopAttrib();										// Return the display list back to it's previous state
*/
}


///////////////////////////////// DESTROY FONT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function frees our display lists
/////
///////////////////////////////// DESTROY FONT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

gl_fonte::~gl_fonte()
{
 glDeleteLists(id_fonte, nb_char);				// Free the display list
 SelectObject(hdc, hOldFont);					// Select the old font back in so we don't have memory leaks
}


gl_fonte::gl_fonte(char *nom_fonte, float p)
{
 hdc = wglGetCurrentDC();
 profondeur_fonte = p;
 nb_char = 256;
 mode = 1; // Annalyse des %
 g_GlyphInfo = new GLYPHMETRICSFLOAT[nb_char];

 id_fonte = CreerFonteOpenGL(nom_fonte, p);
}

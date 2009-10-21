
#include <windows.h>
#pragma hdrstop
#include <condefs.h>


//---------------------------------------------------------------------------
/*
//USEUNIT("src\testalpha.c");
USELIB("C:\SDL et OpenGL\glut-3.7\glut32.lib");
USELIB("C:\SDL et OpenGL\OGLSDK\bclib\glu.lib");
USELIB("src\sdl.lib");
USELIB("C:\SDL et OpenGL\OGLSDK\bclib\opengl.lib");
//USELIB("C:\SDL et OpenGL\SDL_NET\SDL_net-1.2.4\lib\bc_SDL_net.lib");
*/
USELIB("src\sdl.lib");

USELIB("C:\SDL et OpenGL\OGLSDK\bclib\glu.lib");
USELIB("C:\SDL et OpenGL\OGLSDK\bclib\opengl.lib");
USELIB("C:\SDL et OpenGL\glut-3.7\glut32.lib");

//USELIB("C:\SDL et OpenGL\SDL_NET\SDL_net-1.2.4\lib\bc_SDL_net.lib");
//SDL.lib SDLmain.lib glu.lib opengl.lib

USEUNIT("opengl\texte.cpp");
USEUNIT("opengl\config_opengl.cpp");
USEUNIT("opengl\principal.cpp");
USEUNIT("physique\alx_cercle2D.cpp");
USEUNIT("physique\alx_choc2D.cpp");
USEUNIT("physique\alx_classe_base_model.cpp");
USEUNIT("physique\alx_deplacement2D.cpp");
USEUNIT("physique\alx_droite2D.cpp");
USEUNIT("physique\alx_entite_bloc.cpp");
USEUNIT("physique\alx_entite_palet.cpp");
USEUNIT("physique\alx_model_simulation_bidon1.cpp");
USEUNIT("physique\alx_model_simulation_physique1.cpp");
USEUNIT("physique\alx_point2D.cpp");
USEUNIT("physique\alx_primitive2D.cpp");
USEUNIT("physique\alx_segment2D.cpp");
USEUNIT("physique\alx_simulateur_boite_rectangulaire.cpp");
USEUNIT("physique\alx_simulateur_choc_2D.cpp");
USEUNIT("physique\alx_simulateur_global_jeu_palet.cpp");
USEUNIT("physique\alx_simulateur_mouvement_newton.cpp");
USEUNIT("physique\alx_simulateur_physique_champs_uniforme.cpp");
USEUNIT("physique\alx_simulateur_physique_initialisation.cpp");
USEUNIT("physique\alx_simulateur_ressort_2D.cpp");
USEUNIT("physique\alx_vecteur2D.cpp");
USEUNIT("physique\math_alex.cpp");
USEUNIT("interfaces\classiques\alx_fiche.cpp");
USEUNIT("interfaces\classiques\alx_fiche_opengl.cpp");
USEUNIT("interfaces\alx_simulateur_pointeurs.cpp");
USEUNIT("physique\alx_polygone2D.cpp");
USEUNIT("interfaces\alx_evennement_pointeur.cpp");
USEUNIT("interfaces\alx_simulateur_pointeurs_souris.cpp");
USEUNIT("physique\alx_simulateur_points_contact.cpp");
USEUNIT("interfaces\alx_liant_ptr_contact.cpp");
USEUNIT("utilitaires\alx_arbre.cpp");
USEUNIT("interfaces\alx_simulateur_couche.cpp");
/**/
USEUNIT("interfaces\sdl_opengl\alx_interface_sdl_opengl_1.cpp");
USEUNIT("interfaces\alx_noeud_scene.cpp");
USEUNIT("interfaces\sdl_opengl\alx_noeud_fiche_sdl_opengl.cpp");

//---------------------------------------------------------------------------
extern "C"{

#include <stdio.h>
#include <string.h>
#include <ctype.h>

//#include <windows.h>
#include <malloc.h>		/* For _alloca() */

/* Include the SDL main definition header */
#include "SDL.h"
#include "SDL_main.h"
#include <gl.h>
#include <glu.h>

#ifdef main
#undef main
#endif

}

#include "principal.h"

/* The standard output files */
#define STDOUT_FILE	"stdout.txt"
#define STDERR_FILE	"stderr.txt"


// extern "C" SDL_RegisterApp(char *, unsigned int, void *);


/* Parse a command line buffer into arguments */
static int ParseCommandLine(char *cmdline, char **argv)
{
	char *bufp;
	int argc;

	argc = 0;
	for ( bufp = cmdline; *bufp; ) {
		/* Skip leading whitespace */
		while ( isspace(*bufp) ) {
			++bufp;
		}
		/* Skip over argument */
		if ( *bufp == '"' ) {
			++bufp;
			if ( *bufp ) {
				if ( argv ) {
					argv[argc] = bufp;
				}
				++argc;
			}
			/* Skip over word */
			while ( *bufp && (*bufp != '"') ) {
				++bufp;
			}
		} else {
			if ( *bufp ) {
				if ( argv ) {
					argv[argc] = bufp;
				}
				++argc;
			}
			/* Skip over word */
			while ( *bufp && ! isspace(*bufp) ) {
				++bufp;
			}
		}
		if ( *bufp ) {
			if ( argv ) {
				*bufp = '\0';
			}
			++bufp;
		}
	}
	if ( argv ) {
		argv[argc] = NULL;
	}
	return(argc);
}

/* Show an error message */
static void ShowError(const char *title, const char *message)
{
/* If USE_MESSAGEBOX is defined, you need to link with user32.lib */
#ifdef USE_MESSAGEBOX
	MessageBox(NULL, message, title, MB_ICONEXCLAMATION|MB_OK);
#else
	fprintf(stderr, "%s: %s\n", title, message);
#endif
}

/* Pop up an out of memory message, returns to Windows */
static BOOL OutOfMemory(void)
{
	ShowError("Fatal Error", "Out of memory - aborting");
	return FALSE;
}

#ifdef _MSC_VER /* The VC++ compiler needs main defined */
#define console_main main
#endif

/* This is where execution begins [console apps] */
int console_main(int argc, char *argv[])
{
	int n;
	char *bufp, *appname;

	/* Get the class name from argv[0] */
	appname = argv[0];
	if ( (bufp=strrchr(argv[0], '\\')) != NULL ) {
		appname = bufp+1;
	} else
	if ( (bufp=strrchr(argv[0], '/')) != NULL ) {
		appname = bufp+1;
	}

	if ( (bufp=strrchr(appname, '.')) == NULL )
		n = strlen(appname);
	else
		n = (bufp-appname);

	bufp = (char *)alloca(n+1);
	if ( bufp == NULL ) {
		return OutOfMemory();
	}
	strncpy(bufp, appname, n);
	bufp[n] = '\0';
	appname = bufp;

	/* Load SDL dynamic link library */
	if ( SDL_Init(0) < 0 ) {
		ShowError("WinMain() error", SDL_GetError());
		return(FALSE);
	}
	atexit(SDL_Quit);

	/* Create and register our class, then run main code */
	if ( SDL_RegisterApp( appname, CS_BYTEALIGNCLIENT
			    , GetModuleHandle(NULL)) < 0 ) {
		ShowError("WinMain() error", SDL_GetError());
		exit(1);
	}
	//SDL_main(argc, argv);

        principal prog;
        prog.demarrer();

	/* Exit cleanly, calling atexit() functions */
	exit(0);
        return 0;
}

/* This is where execution begins [windowed apps] */
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR szCmdLine, int sw)
{
	HINSTANCE handle;
	FILE *newfp;
	char **argv;
	int argc;
	char *cmdline;
	char *bufp;

	/* Start up DDHELP.EXE before opening any files, so DDHELP doesn't
	   keep them open.  This is a hack.. hopefully it will be fixed 
	   someday.  DDHELP.EXE starts up the first time DDRAW.DLL is loaded.
	 */
	handle = LoadLibrary("DDRAW.DLL");
	if ( handle != NULL ) {
		FreeLibrary(handle);
	}

	/* Redirect standard input and standard output */
	newfp = freopen(STDOUT_FILE, "w", stdout);
	if ( newfp == NULL ) {	/* This happens on NT */
#if !defined(stdout)
		stdout = fopen(STDOUT_FILE, "w");
#else
		*stdout = *fopen(STDOUT_FILE, "w");
#endif
	}
	newfp = freopen(STDERR_FILE, "w", stderr);
	if ( newfp == NULL ) {	/* This happens on NT */
#if !defined(stdout)
		stderr = fopen(STDERR_FILE, "w");
#else
		*stderr = *fopen(STDERR_FILE, "w");
#endif
	}
	setvbuf(stdout, NULL, _IOLBF, BUFSIZ);	/* Line buffered */
	setbuf(stderr, NULL);			/* No buffering */

	/* Grab the command line (use alloca() on Windows) */
	bufp = GetCommandLine();
	cmdline = (char *)alloca(strlen(bufp)+1);
	if ( cmdline == NULL ) {
		return OutOfMemory();
	}
	strcpy(cmdline, bufp);

	/* Parse it into argv and argc */
	argc = ParseCommandLine(cmdline, NULL);
	argv = (char **)alloca((argc+1)*(sizeof *argv));
	if ( argv == NULL ) {
		return OutOfMemory();
	}
	ParseCommandLine(cmdline, argv);

	/* Run the main program (after a little SDL initialization) */
	return (console_main(argc, argv));
}


/*
#pragma argsused
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
		return 0;
}
*/

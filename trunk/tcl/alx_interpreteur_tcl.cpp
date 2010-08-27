#include "alx_interpreteur_tcl.h"
#include "..\physique\alx_methode_rappel.h"

int Tcl_AppInit(Tcl_Interp *interp)
{
    if (Tcl_Init(interp) == TCL_ERROR) {
	return TCL_ERROR;
    }

    /*
     * Install a signal handler to the win32 console tclsh is running in.
     */
    //SetConsoleCtrlHandler(sigHandler, TRUE);
    //exitToken = Tcl_AsyncCreate(asyncExit, NULL);

#ifdef TCL_TEST
    if (Tcltest_Init(interp) == TCL_ERROR) {
	return TCL_ERROR;
    }
    Tcl_StaticPackage(interp, "Tcltest", Tcltest_Init,
            (Tcl_PackageInitProc *) NULL);
    if (TclObjTest_Init(interp) == TCL_ERROR) {
	return TCL_ERROR;
    }
#ifdef TCL_THREADS
    if (TclThread_Init(interp) == TCL_ERROR) {
	return TCL_ERROR;
    }
#endif
    if (Procbodytest_Init(interp) == TCL_ERROR) {
	return TCL_ERROR;
    }
    Tcl_StaticPackage(interp, "procbodytest", Procbodytest_Init,
            Procbodytest_SafeInit);
#endif /* TCL_TEST */

#if defined(STATIC_BUILD) && defined(TCL_USE_STATIC_PACKAGES)
    {
	extern Tcl_PackageInitProc Registry_Init;
	extern Tcl_PackageInitProc Dde_Init;

	if (Registry_Init(interp) == TCL_ERROR) {
	    return TCL_ERROR;
	}
	Tcl_StaticPackage(interp, "registry", Registry_Init, NULL);

	if (Dde_Init(interp) == TCL_ERROR) {
	    return TCL_ERROR;
	}
	Tcl_StaticPackage(interp, "dde", Dde_Init, NULL);
   }
#endif

    /*
     * Call the init procedures for included packages.  Each call should
     * look like this:
     *
     * if (Mod_Init(interp) == TCL_ERROR) {
     *     return TCL_ERROR;
     * }
     *
     * where "Mod" is the name of the module.
     */

    /*
     * Call Tcl_CreateCommand for application-specific commands, if
     * they weren't already created by the init procedures called above.
     */

    /*
     * Specify a user-specific startup file to invoke if the application
     * is run interactively.  Typically the startup file is "~/.apprc"
     * where "app" is the name of the application.  If this line is deleted
     * then no user-specific startup file will be run under any conditions.
     */

    Tcl_SetVar(interp, "tcl_rcFileName", "~/tclshrc.tcl", TCL_GLOBAL_ONLY);
    return TCL_OK;
}


resultat_interpreteur_tcl alx_interpreteur_tcl::Evaluer(const alx_chaine_char &commande)

{return Evaluer(commande.Texte());}


resultat_interpreteur_tcl alx_interpreteur_tcl::Evaluer(const char *commande)
{//static char c_commande[65536];

 //strcpy(c_commande, commande);

// dernier_resultat.reponse_int   = Tcl_EvalEx(interpreteur, c_commande, -1, TCL_EVAL_GLOBAL);
 dernier_resultat.reponse_int   = Tcl_EvalEx(interpreteur, commande, -1, TCL_EVAL_GLOBAL);
 dernier_resultat.reponse_texte = Tcl_GetStringResult(interpreteur);
 return dernier_resultat;
}

alx_interpreteur_tcl::alx_interpreteur_tcl(const char *nom_exec) : dernier_resultat("", 0)
{Tcl_FindExecutable ( nom_exec );
 interpreteur = Tcl_CreateInterp();

 Tcl_Init(interpreteur);
   Evaluer("package require Tk");
 //Tk_Init(interpreteur);
 texte_edition = (alx_chaine_char*)NULL;
}


//______________________________________________________________________________
void Appel_TCL::init()
{m_rappel = FONCTION_DE_RAPPEL( Appel_TCL
                              ,&Appel_TCL::F_rappel );
 interp = (alx_interpreteur_tcl*)NULL;
 tcl_obj = Tcl_NewObj();
 Tcl_IncrRefCount(tcl_obj);
}

//______________________________________________________________________________
void Appel_TCL::F_rappel(void *param)
{if(interp)
  {Param(param);
   //interp->Evaluer( Texte() );
   Tcl_EvalObjEx(interp->Interpreteur_tcl(), tcl_obj, 0);
  }
}

//______________________________________________________________________________
Appel_TCL::Appel_TCL(alx_interpreteur_tcl *i)
{init();
 interp = i;
}

//______________________________________________________________________________
Appel_TCL::Appel_TCL(alx_interpreteur_tcl *i, const char *txt)
{init();
 Texte(txt);
 interp = i;
}

//______________________________________________________________________________
void Appel_TCL::Texte(const char *t)
{texte = t;
 if (Tcl_IsShared(tcl_obj)) {
   printf("Tcl_IsShared(tcl_obj), can not set texte to \"%s\"", t);
  } else {Tcl_SetStringObj(tcl_obj, texte.Texte(), texte.Taille());}
}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
int Convert_void_to_binary(alx_interpreteur_tcl *alx_interp, void *ptr_void, int length, char *var_name)
{
  Tcl_Obj *obj_array = Tcl_NewByteArrayObj((unsigned char *)ptr_void, length);
  Tcl_SetVar2Ex(alx_interp->Interpreteur_tcl(), var_name, NULL, obj_array, TCL_NAMESPACE_ONLY);

  return length;
}



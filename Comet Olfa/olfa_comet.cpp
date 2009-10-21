#include "olfa_comet.h"
#include <stdio.h>
#include <stdlib.h>

//*****************************************************************************|
//______________________________________________________________________________
//________________ Les nouvelles commandes de l'interpréteur TCL________________
//______________________________________________________________________________
/*static cogitant::Environment *env_courant = (cogitant::Environment*)NULL;
static cogitant::ResultOpeProjection proj;

int Env_courant(ClientData clientdata, Tcl_Interp *interp, int argc, char *argv[])
{
 if( (argc==2)&&(!strcmp(argv[1], "obtenir")) )
  {char rep[16];
   Tcl_SetResult(interp, itoa((int)(void*)(env_courant), rep, 10), TCL_VOLATILE);
   return TCL_OK;}

 if( (argc==3)&&(strcmp(argv[1], "par_fichier")==0) )
  {
   try
    {cogitant::Environment env;
     env.readSupport(argv[2]);
     env_courant = &env;
     char rep[16];
     Tcl_SetResult(interp, itoa((int)(void*)env_courant, rep, 10), TCL_VOLATILE);
     return TCL_OK;}
    catch(cogitant::Exception& e)
     {std::string erreur = "L'affectation par fichier de l'environnement des graphes conceptuels pose problème :\n";
      erreur+= e.toString();
      char c_erreur[1024];
      strcpy(c_erreur, erreur.Texte());
      Tcl_SetResult(interp, c_erreur, TCL_VOLATILE);
      return TCL_ERROR;}
  }
 if( (argc==3)&&(strcmp(argv[1], "par_pointeur")==0) )
  {char rep[16];
   int adresse = atoi(argv[2]);
   env_courant = (cogitant::Environment*)(void*)(adresse);
   Tcl_SetResult(interp, itoa((int)(void*)env_courant, rep, 10), TCL_VOLATILE);
   return TCL_OK;
  }

 std::string erreur = "Env_courant renvoi l'adresse de l'environnement courant.\nEnv_courant se manipule comme suit :\n";
 erreur+= "     Env_courant par_fichier  {nom du fichier}\n";
 erreur+= "     Env_courant par_pointeur {entier représentant l'adresse de l'environnement}\n";
 char c_erreur[1024];
 strcpy(c_erreur, erreur.Texte());
 Tcl_SetResult(interp, c_erreur, TCL_VOLATILE);
 return TCL_ERROR;
}

int cogitantGC(ClientData clientdata, Tcl_Interp *interp, int argc, char *argv[])
{
 if( (env_courant)&&(argc >= 2) )
  {
   if( !strcmp(argv[1], "creer") && (argc==4) )
    {
     if( !strcmp(argv[2], "par_fichier") )
      {
       try
        {FILE *f = fopen(argv[3], "r");
         if(f==NULL)
          {
           char rep[] = "Impossible d'ouvrir ce fichier";
           Tcl_SetResult(interp, rep, TCL_VOLATILE);
           return TCL_ERROR;
          }
         fflush(f);
         fclose(f);
         cogitant::iSet nGC = env_courant->readGraphs(argv[3]);
         char reponse[16];
         int adresse = (int)(void*)(nGC);
         Tcl_SetResult(interp, itoa(adresse, reponse, 10), TCL_VOLATILE);
         return TCL_OK;}
        catch(cogitant::Exception& e)
         {std::string erreur = "Le chargement du graphe conceptuel par fichier pose problème :\n";
          erreur+= e.toString();
          char c_erreur[1024];
          strcpy(c_erreur, erreur.Texte());
          Tcl_SetResult(interp, c_erreur, TCL_VOLATILE);
          return TCL_ERROR;}
      }
    }
   else if( !strcmp(argv[1], "detruire") && (argc==2) )
         {
          //cogitant::iSet GC = (cogitant::iSet)(void*)(atoi(argv[1]));
          //delete GC;
          Tcl_SetResult(interp, "Pas encore implémantée", TCL_VOLATILE);
          return TCL_ERROR;
         }
  }

 std::string rep;
   rep = "cogitantGC renvoi un entier sous forme de chaine.\nL'appel à cogitantGC se fait comme suit : \n";
   rep+= "        cogitantGC creer par_fichier {un nom de fichier contenant un graphe conceptuel}\n";
   rep+= "        cogitantGC creer par_pointeur {une adresse mémoire (sous forme d'entier) contenant le graphe conceptuel}\n";
   rep+= "        cogitantGC detruire {une adresse mémoire (sous forme d'entier) contenant le graphe conceptuel}\n";
   char c_rep[1024];
   strcpy(c_rep, rep.Texte());
   Tcl_SetResult(interp, c_rep, TCL_VOLATILE);
   return TCL_ERROR;
}

int GC_proj_GC(ClientData clientdata, Tcl_Interp *interp, int argc, char *argv[])
{
 if(argc == 3)
  {
   int adresse1 = atoi(argv[1])
     , adresse2 = atoi(argv[2]);

   char rep[2]; rep[1] = 0;
   if( (adresse1 < 0)
     ||(adresse2 < 0) ) // Par convention ce sont alors des graphes vides, aucune projection n'est possible, on renvoi donc 0
    {rep[0] = '0';
     Tcl_SetResult(interp, rep, TCL_VOLATILE);
     return TCL_OK;}

   cogitant::iSet GC1 = (cogitant::iSet)(void*)(adresse1)
                , GC2 = (cogitant::iSet)(void*)(adresse2);

  // Si les graphes ne sont pas vides, on tente la projection
   env_courant->projections( GC1
                           , GC2
                           , proj);
// on vérifie qu'il y a au moins une projection.
   if( proj.size() != 0 )
     rep[0] = '1';
    else rep[0] = '0';
   Tcl_SetResult(interp, rep, TCL_VOLATILE);
   return TCL_OK;
  }

 std::string rep;
   rep = "GC_proj_GC renvoi un booléen sous forme de chaine.\nL'appel à GC_proj_GC se fait comme suit : \n";
   rep+= "        GC_proj_GC {@GC1} {@GC2}\n";
   char c_rep[1024];
   strcpy(c_rep, rep.Texte());
   Tcl_SetResult(interp, c_rep, TCL_VOLATILE);
   return TCL_ERROR;
}
*/
//******************************************************************************
//______________________________________________________________________________
//_________________________ Les comets proprements dites _______________________
//______________________________________________________________________________
unsigned int olfa_comet::nb_comets = 1;

//______________________________________________________________________________
//_______________________________ Les constructeurs ____________________________
//______________________________________________________________________________
olfa_comet::olfa_comet( const char *nom
                      //, cogitant::Environment *e
                      , alx_interpreteur_tcl  *interp) : evoluer(true)
{alx_chaine_char s_commande;

 this->nom_usage = nom;
 this->nom_usage+= "_";
 this->nom_usage+= itoa(nb_comets, numero, 10);
 classe_comet = "c_";
 classe_comet+= itoa(nb_comets, numero, 10);
 //Environnement(e);
 Interpreteur(interp);
 nb_comets++;

// Faire hériter la nouvelle classe de la classe de base des comets:
 s_commande  = "inherit ";
 s_commande += classe_comet;
 s_commande += " c_0";
 Interpreteur()->Evaluer( s_commande.Texte() );
}

//______________________________________________________________________________
void olfa_comet::Enregistrer_dans_TCL()
{alx_chaine_char s_commande;
 s_commande  = Classe_comet(); s_commande += " "; s_commande += Nom_usage();
 Interpreteur()->Evaluer( s_commande.Texte() ); }

//______________________________________________________________________________
void olfa_comet::Commande(const char *c)
{alx_chaine_char s_commande;
 s_commande  = Nom_usage();
 s_commande += " ";
 s_commande += c;
 Interpreteur()->Evaluer( s_commande.Texte() );}

//*****************************************************************************|
//______________________________________________________________________________
//_________________________________ Les méthodes________________________________
//______________________________________________________________________________

//______________________________________________________________________________
//_______________________ Les méthodes pour les graphes ________________________
//______________________________________________________________________________
/*bool olfa_comet::Charger_graphe_description_semantique(const char *nom_fichier)
{
 if(!env)
  {
   erreur = "L'affectation du graphe conceptuel de description sémantique par fichier pose problème :\n";
   erreur+= "Aucun environnement n'a été spécifié";
   return false;
  }
 try
  {
   description_semantique = env->readGraphs(nom_fichier);
   graphe_charge = true;
   return true;
  }
  catch(cogitant::Exception& e)
   {
    erreur = "L'affectation du graphe conceptuel de description sémantique par fichier pose problème :\n";
    erreur+= e.toString();
    return false;
   }
}

//______________________________________________________________________________
bool olfa_comet::Charger_graphe_description_semantique(const cogitant::iSet &g)
{
 try
  {
   description_semantique = g;
   graphe_charge = true;
   return true;
  }
  catch(cogitant::Exception& e)
   {
    erreur = "L'affectation du graphe conceptuel de description sémantique par copie pose problème :\n";
    erreur+= e.toString();
    return false;
   }
}

bool olfa_comet::Decharger_graphe_description()
{
 if(!env)
  {
   erreur = "L'affectation du graphe conceptuel de description sémantique par fichier pose problème :\n";
   erreur+= "Aucun environnement n'a été spécifié";
   return false;
  }
 if(graphe_charge)
  {env->deleteGraph( description_semantique );
   graphe_charge = false;
   return true;}
 return false;
}     */

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________

Environnement_comet_olfa::Environnement_comet_olfa(const char *nom_exec) : alx_interpreteur_tcl(nom_exec)
{
 Evaluer("source [file join . gml_Object.tcl]");
 /*Tcl_CreateCommand( interpreteur
                  , "cogitantGC"
                  , cogitantGC
                  , NULL
                  , (Tcl_CmdDeleteProc *)NULL );
 Tcl_CreateCommand( interpreteur
                  , "Env_courant"
                  , Env_courant
                  , NULL
                  , (Tcl_CmdDeleteProc *)NULL );
 Tcl_CreateCommand( interpreteur
                  , "GC_proj_GC"
                  , GC_proj_GC
                  , NULL
                  , (Tcl_CmdDeleteProc *)NULL );  */
 Evaluer("source [file join . essai.tcl]" );
}




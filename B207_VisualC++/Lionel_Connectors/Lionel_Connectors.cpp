// Lionel_Connectors.cpp : définit les fonctions d'initialisation pour la DLL.
//

#include "stdafx.h"
#include "Lionel_Connectors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO : si cette DLL est liée dynamiquement aux DLL MFC,
//		toute fonction exportée de cette DLL qui appelle
//		MFC doit avoir la macro AFX_MANAGE_STATE ajoutée au
//		tout début de la fonction.
//
//		Par exemple :
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// corps de fonction normal ici
//		}
//
//		Il est très important que cette macro se trouve dans chaque
//		fonction, avant tout appel à MFC. Cela signifie qu'elle
//		doit être la première instruction dans la 
//		fonction, avant toute déclaration de variable objet
//		dans la mesure où leurs constructeurs peuvent générer des appels à la DLL
//		MFC.
//
//		Consultez les notes techniques MFC 33 et 58 pour plus de
//		détails.
//


// CLionel_ConnectorsApp

BEGIN_MESSAGE_MAP(CLionel_ConnectorsApp, CWinApp)
END_MESSAGE_MAP()


// construction CLionel_ConnectorsApp

CLionel_ConnectorsApp::CLionel_ConnectorsApp()
{
	// TODO : ajoutez ici du code de construction,
	// Placez toutes les initialisations significatives dans InitInstance
}


// Seul et unique objet CLionel_ConnectorsApp

CLionel_ConnectorsApp theApp;


// initialisation de CLionel_ConnectorsApp

BOOL CLionel_ConnectorsApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

// Lionel_Connectors.h : fichier d'en-tête principal pour la DLL Lionel_Connectors
//

#pragma once

#ifndef __AFXWIN_H__
	#error "incluez 'stdafx.h' avant d'inclure ce fichier pour PCH"
#endif

#include "resource.h"		// symboles principaux


// CLionel_ConnectorsApp
// Consultez Lionel_Connectors.cpp pour l'implémentation de cette classe
//

class CLionel_ConnectorsApp : public CWinApp
{
public:
	CLionel_ConnectorsApp();

// Substitutions
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

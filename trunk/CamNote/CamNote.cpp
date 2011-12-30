#include "CamNote.h"
#include "..\interfaces\interface.h"
#include "..\interfaces\fontes.h"
#include "..\interfaces\choses_communes.h"
#include "..\physique\math_alex.cpp"

#include "..\Comet Olfa\Creation_objet_dans_tcl.h"
#include <route66.h>
/*
CamNote *camnote_courant;

void Lancer_serveur(Uint16 port_CamNote)
{
	IPaddress ip,*remoteip;
	TCPsocket server,client;
	char message[65536];
	int len;
	Uint32 ipaddr;
	Uint16 port = port_CamNote;

	// Resolve the argument into an IPaddress type
	if(SDLNet_ResolveHost(&ip,NULL,port_CamNote)==-1)
	 {
	  printf("SDLNet_ResolveHost: %s\n",SDLNet_GetError());
	  exit(3);
	 }

	// open the server socket
	server=SDLNet_TCP_Open(&ip);
	if(!server)
	 {
	  printf("SDLNet_TCP_Open: %s\n",SDLNet_GetError());
	  exit(4);
	 }

        bool connection_etablit = false;
	while(1)
	{
         if(connection_etablit)
          {
	   // read the buffer from client
	   len=SDLNet_TCP_Recv(client,message,65536);
	   //SDLNet_TCP_Close(client);
	   if(!len)
	    {
	     printf("SDLNet_TCP_Recv: %s\n",SDLNet_GetError());
	     continue;
	    }

	    // print out the message
	    printf("Received: %.*s\n",len,message);

	    switch(message[0])
	     {
              case '0':
                camnote_courant->mise_a_jour_a_faire = true;
                message[4] = 0;
                //camnote_courant->Fixer_numero_transparent( atoi( &(message[2]) ) );
              break;
              case '1':
               switch(message[2])
                {
                 case '0':
                   //camnote_courant->Activer_pixels_miroirs(message[4] == '0');
                 break;
                 case '1':
                   //camnote_courant->Fixer_intensiter_pixels_miroirs( atoi( &(message[4]) ) );
                 break;
                }
              break;
              case '2':
                //camnote_courant->Texte_note( &(message[2]) );
              break;
      	     }
           continue;
          }

		// try to accept a connection
		client=SDLNet_TCP_Accept(server);
		if(!client)
		 { // no connection accepted
			//printf("SDLNet_TCP_Accept: %s\n",SDLNet_GetError());
		  SDL_Delay(100); //sleep 1/10th of a second
		  continue;
		 }

		// get the clients IP and port number
		remoteip=SDLNet_TCP_GetPeerAddress(client);
		if(!remoteip)
		 {
		  printf("SDLNet_TCP_GetPeerAddress: %s\n",SDLNet_GetError());
		  continue;
		 }

		// print out the clients IP and port number
                connection_etablit = true;
		ipaddr=SDL_SwapBE32(remoteip->host);
		printf("Accepted a connection from %d.%d.%d.%d port %hu\n",
			ipaddr>>24,
			(ipaddr>>16)&0xff,
			(ipaddr>>8)&0xff,
			ipaddr&0xff,
			remoteip->port);
	}
}


int thread_func(void *camnote)
{Lancer_serveur(9999);
 return(0);}
*/
//______________________________________________________________________________
//_______________________________CamNote proprement dit_________________________
//______________________________________________________________________________
void CamNote::Reception_message_telec(unsigned int argc, char **argv)
{// On regarde quel est le message :
 if(argc != 2)
   return;
 info_transparents = true;

 nouveau_num_transparent[1] = Num_transparent();

 if( !strcmp("PREV", argv[0]) )
  {nouveau_num_transparent[1] = Amax((unsigned int)1, nouveau_num_transparent[1]-1);}
 if( !strcmp("NEXT", argv[0]) )
  {nouveau_num_transparent[1]++;}
 if( !strcmp("GOTO", argv[0]) )
  {nouveau_num_transparent[1] = Amax(0, atoi(argv[1]));}
}

//______________________________________________________________________________
void CamNote::Reception_message_telec_pix_mirroirs(unsigned int argc, char **argv)
{if(argc != 1)
   return;
 info_pix_mirroirs = true;
 if( !strcmp("ENABLE", argv[0]) )
  {nouvelle_activation_pix_mirroirs = true;}
 else if( !strcmp("DISABLE", argv[0]) )
  {nouvelle_activation_pix_mirroirs = false;}
 else
  {nouvelle_translucidite = (float)(atoi(argv[0]))/(float)(100);}
}

//______________________________________________________________________________
void CamNote::Demande_reemission(unsigned int argc, char **argv)
{alx_chaine_char s_message("<slideremote current_slide=\"");
 s_message += nouveau_num_transparent[1];
 s_message.Ajouter("\"/>",0,3);
 Emission_66( s_message.Texte() );
}

//______________________________________________________________________________
void CamNote::Quitter()
{alx_chaine_char m("<stop speaker=\"");
 m += telecommande->Nom_de_transparent();
 m.Ajouter("\"/>",0,3);
 Emission_66( m.Texte() );
}

void CamNote::Demarrer()
{alx_chaine_char m;
 m = "<start speaker=\"" + telecommande->Nom_de_transparent() + "\" subject=\"Cameleon meeting\"/>";
 Emission_66(m.Texte());
}

//______________________________________________________________________________
CamNote::CamNote( alx_interface_sdl_opengl_1 *interface_m, alx_noeud_visualisation_camera *cam
                /*, cogitant::Environment *e*/, alx_interpreteur_tcl  *interp ) : deformation_telec_translucidite(NULL)
                                                                            , deformation_telec_pos_et_dim(NULL)
                                                                            , deformation_telec(NULL)
                                                                            , route66(interface_m->Route_66())
                                                                            , olfa_comet( "CometCamNote"
                                                                                        //, e
                                                                                        , interp)
{visu_cam = cam;
// Route 66_____________________________________________________________________
 info_pix_mirroirs = info_transparents = false;

// Interfaçage avec TCL
 Enregistrer_CamNote((olfa_comet*)this, this);

 Prendre_en_compte_repere(true);

// camnote_courant = this;
 mise_a_jour_a_faire = true;
 temps_transition = 2000;
 Telec_invisible = false;

 interface_mere = interface_m;
 //env_GC     = e;
 interp_tcl = interp;

 interface_m->A_scene->Ajouter_fils( (alx_noeud_scene*)this );
 this->Position_dans_liste_des_fils_changeable(true);

 Nom_IU("CamNote");
 visualisateur = new alx_visu_image_camnote_cpp_bigre( "Visu_diapo"
                                                     //, e
                                                     , interp
                                                     , interface_m
                                                     , (alx_noeud_scene*)this
                                                     , visu_cam->Ptr_Info_texture()
                                                     , visu_cam->Nb_octets_par_pixel()
                                                     , visu_cam->Ordonnancement_couleurs() );
 visualisateur->Position_dans_liste_des_fils_changeable(false);
 visualisateur->Nom_IU("Visualisateur\nCamNote");
 telecommande  = new alx_telec_camnote_cpp_bigre( "Telec_diapo"
                                                //, e
                                                , interp
                                                , interface_m
                                                , (alx_noeud_scene*)this
                                                , visu_cam->Ptr_Info_texture()
                                                , visu_cam->Nb_octets_par_pixel()
                                                , visu_cam->Ordonnancement_couleurs() );
 telecommande->Nom_IU("Telecommande\nCamNote");
 if(route66)
  {route66->bindMessageCallback("^<slideremote nav=\"(.*)\" slide_nb=\"([0-9]*)\"/>"              , new Route66MessageCallbackOf<CamNote>(this, &CamNote::Reception_message_telec));
   route66->bindMessageCallback("^<slideremote> VIDEO TRANSPARENCY ([0-9]*)</slideremote>"        , new Route66MessageCallbackOf<CamNote>(this, &CamNote::Reception_message_telec_pix_mirroirs));
   route66->bindMessageCallback("^<slideremote> VIDEO TRANSPARENCY (ENABLE|DISABLE)</slideremote>", new Route66MessageCallbackOf<CamNote>(this, &CamNote::Reception_message_telec_pix_mirroirs));
   route66->bindMessageCallback("^<get_current_slide_nb/>"                                        , new Route66MessageCallbackOf<CamNote>(this, &CamNote::Demande_reemission));
   Demarrer();
  }

// Enregistrement des varibles mises à disposition du moteur d'évolution
//#     CamNote   : Le système interactif CamNote
//#     Cam_visu  : Le visualisateur de diapo
//#     Cam_tele  : La télécommande
//#     classeCam : La classe de CamNote (vis à vis de gmlObject) qui hérite de c_0
 Enregistrer_dans_TCL();
 alx_chaine_char commande;
 commande  = "set CometCamNote "; commande += this->Nom_usage(); Interpreteur()->Evaluer( commande );
 commande  = "set classe "; commande += this->Classe_comet(); Interpreteur()->Evaluer( commande );
 commande  = "set Cam_visu "; commande += visualisateur->Nom_usage(); Interpreteur()->Evaluer( commande );
 commande  = "set Cam_tele "; commande += telecommande->Nom_usage();  Interpreteur()->Evaluer( commande );
 Interpreteur()->Evaluer("source [file join . CamNote_evolution.tcl]" );
 Interpreteur()->Evaluer("source [file join . init_CamNote_evolution.tcl]" );
 Nom_presentation( telecommande->Nom_de_transparent() );

// Création des liens de bases
 telecommande->abonner_a_activation_pix_mirroir( FONCTION_DE_RAPPEL( CamNote
                                                                   ,&CamNote::Rationnaliser_activation_pix_mirroir) );
 telecommande->abonner_a_intensite_pix_mirroir( FONCTION_DE_RAPPEL( CamNote
                                                                  ,&CamNote::Rationnaliser_intensite_pix_mirroir) );
 telecommande->abonner_a_nom_transparent( FONCTION_DE_RAPPEL( CamNote
                                                            ,&CamNote::Rationnaliser_nom_racine_trans) );
 telecommande->abonner_a_numero( FONCTION_DE_RAPPEL( CamNote
                                                   ,&CamNote::Rationnaliser_num_trans) );



// Création du visualisateur et de la télécommande. De base ce sont les léments _bigre
/*// Le serveur :
 thread_serveur = SDL_CreateThread(thread_func, this);
 if ( thread_serveur == NULL )
  {
   fprintf(stderr, "impossible de créer thread_serveur: %s\n", SDL_GetError());
   return;
  }*/

}

CamNote::~CamNote()
{}

void CamNote::Rationnaliser_bouton_retour(void *param)
{if( deformation_telec || !Telec_invisible ) return;
 Telec_invisible = false;

 Ajouter_fils_au_debut( (alx_noeud_scene*)telecommande );
 alx_noeud_fenetre_sdl_opengl *fen_tele = telecommande->Noeud_fenetre_sdl_opengl()
                            , *fen_visu = visualisateur->Noeud_fenetre_sdl_opengl();
// Création de la déformation
 deformation_telec = new deformation_dynamique_etirement_fenetre( temps_transition
                                                                , fen_tele
                                                                , (double)0
                                                                , &repere_svg_telec
                                                                , Translucidite_telec
                                                                , 0, 0);

// Quelques petites adaptations
 fen_tele->Origine( fen_visu->Origine() );
 fen_tele->Rotation( fen_visu->Rotation() );
 fen_tele->Etirement( fen_visu->Longueur_corp() / fen_tele->Longueur_corp()
                    , fen_visu->Hauteur_corp() / fen_tele->Hauteur_corp() );

// Démarrage:
 deformation_telec->Demarrer();
}

void CamNote::Rationnaliser_bouton_transfo(void *param)
{if( deformation_telec || Telec_invisible ) return;
 Telec_invisible = true;
// Sauvegarde du repère de la télécommande :
 repere_svg_telec.maj( *(alx_repere2D*)(alx_noeud_fenetre_sdl_opengl*)(telecommande->Noeud_fenetre_sdl_opengl()) );

// Calcul de la translucidite
 if( telecommande->Noeud_fenetre_sdl_opengl()->Corp_translucide() )
   Translucidite_telec = telecommande->Noeud_fenetre_sdl_opengl()->Translucidite_corp();
  else Translucidite_telec = 1;

// Création de la déformation
 deformation_telec = new deformation_dynamique_etirement_fenetre( temps_transition
                                                                , telecommande->Noeud_fenetre_sdl_opengl()
                                                                , Translucidite_telec
                                                                , visualisateur->Noeud_fenetre_sdl_opengl()
                                                                , (double)0 );
// Démarrage:
 deformation_telec->Demarrer();
}

void CamNote::PreRendre()
{visu_cam->PreRendre();
 if(info_transparents)
  {telecommande->Numero_transparent( nouveau_num_transparent[1] );
   Rationnaliser_num_trans( nouveau_num_transparent ); }
 if(info_pix_mirroirs)
 {if(nouvelle_translucidite != visualisateur->Intensite_pixels_miroirs())
    {telecommande->Intensite_pixels_miroirs( nouvelle_translucidite );
     telecommande->Mettre_a_jour();
     Rationnaliser_intensite_pix_mirroir(&nouvelle_translucidite); }
 if(nouvelle_activation_pix_mirroirs != visualisateur->Activation_pix_mirroir())
    {telecommande->Activation_pix_mirroir( nouvelle_activation_pix_mirroirs );
     Rationnaliser_activation_pix_mirroir( &nouvelle_activation_pix_mirroirs ); }
  }

 info_pix_mirroirs = info_transparents = false;
 alx_noeud_scene::PreRendre();
}

void CamNote::Emission_66(const char *m)
{if(route66)
  route66->sendMessage(m);}

void CamNote::Afficher()
{if( Evoluer() && Interpreteur() )
  {Proposer_evolutions   ();
   resultat_interpreteur_tcl res = Interpreteur()->Dernier_resultat();
   Analyser_propositions ();
   res = Interpreteur()->Dernier_resultat();
   Effectuer_propositions();
   res = Interpreteur()->Dernier_resultat();
    }
 alx_noeud_scene::Afficher();
}

//______________________________________________________________________________
void CamNote::rationnaliser(int num)
{
// (this->*(T_rationnalisation_global[num]))(T_modeles_bases[num]->infos_comp);
}


//______________________________________________________________________________
/*void CamNote::Rationnaliser_pixels_mirroirs(const int *infos)
{
 alx_evennement_dispositif_saisie *evt = &( interface_mere->Sim_dispo_saisi()->L_evt.front() );
 if(evt->Type_evt() != alx_dispositif_saisie_appuie )
   return;
 char s[2];
 s[0] = evt->Info_comp().caractere_saisie;
 s[1] = 0;
 if( (s[0] == ' ')
   ||(evt->Info_comp().caractere_saisie == SDLK_RIGHT) )
  {
   int infos2[10];
   infos2[0] = 0;
   infos2[1] = 2;
   Rationnaliser_Bouton_svt(infos2);
  }
 if( (evt->Info_comp().caractere_saisie == SDLK_LEFT) )
  {
   int infos2[10];
   infos2[0] = 0;
   infos2[1] = 2;
   Rationnaliser_Bouton_prcdt(infos2);
  }

 if(s[0]=='p' || s[0]=='P')
  {
   if(barriere_pour_pixmiroirs.Bloquer())
     p_translucidite.couleur[3] = 1-intensite_pixels_miroirs;
    else
      p_translucidite.couleur[3] = 1;

   noeud_translucidite.Param1(&p_translucidite);
   barriere_pour_pixmiroirs.Bloquer(!barriere_pour_pixmiroirs.Bloquer());
  }
}*/


void CamNote::Rationnaliser_nom_racine_trans(void *param)
{Nom_presentation( *((alx_chaine_char*)param) );
 visualisateur->Nom_racine_diapo( *((alx_chaine_char*)param) );}

void CamNote::Rationnaliser_intensite_pix_mirroir(void *param)
{nouvelle_translucidite = *((float*)param);
 visualisateur->Intensite_pixels_miroirs( nouvelle_translucidite );
 }

void CamNote::Rationnaliser_num_trans(void *param)
{nouveau_num_transparent[1] = ((unsigned int*)param)[1];
 Num_transparent( nouveau_num_transparent[1] );
 Nom_presentation( telecommande->Nom_de_transparent() );
 if( !strcmp(Nom_presentation().Texte(), "") ) return;
 alx_chaine_char s_message("<slideremote current_slide=\"");
 char c_tmp[256];
 s_message += nouveau_num_transparent[1];
 s_message.Ajouter("\"/>",0,3);
 strcpy_s(c_tmp, s_message.Texte());
 Emission_66(c_tmp);
 //static char c_tmp[16];
 alx_chaine_char s_tmp( Nom_presentation() );
 int num = Num_transparent(), div = 100;
 char c[2]; c[1]=0;
 for(int i=0;i<3;i++)
  {c[0] = '0'+num/div;
   num %= div; div /= 10;
   s_tmp += c;}
 s_tmp += ".bmp";
 visualisateur->Charger_image( s_tmp );
}

void CamNote::Rationnaliser_activation_pix_mirroir(void *param)
{nouvelle_activation_pix_mirroirs = *((bool*)param);
 visualisateur->Activation_pix_mirroir( nouvelle_activation_pix_mirroirs );}


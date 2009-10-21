#include "Server_TCP_TCL.h"
#include <time.h>

#include "../interfaces/sdl_opengl/alx_interface_sdl_opengl_1.h"
#include "../physique/math_alex.cpp"
#include "../interfaces/sdl_opengl/deformations_dynamiques.h"
#include "../interfaces/sdl_opengl/alx_noeud_image_sdl_opengl.h"
#include "../interfaces/sdl_opengl/alx_noeud_polygone_sdl_opengl.h"

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
Threaded_TCP_client::Threaded_TCP_client(alx_interface_sdl_opengl_1 *mere, const int handle, const Liste_de_rappel &L) : SlimThread(true), finished(false)
{setHandle(handle);
 L_rappel = L;
 Interface_mere = mere;
}

//______________________________________________________________________________
void Threaded_TCP_client::run(void)
{L_rappel.Rappeler( this );
 L_rappel.Vider();
 closeSocket();
 finished = true;
}

//______________________________________________________________________________
void Threaded_TCP_client::Update_L_rappel(const Liste_de_rappel &L)
{L_rappel = L;
}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
SlimTcpConnectionHandler_TCL::SlimTcpConnectionHandler_TCL(alx_interface_sdl_opengl_1 *mere)
{Interface_mere = mere;
}

//______________________________________________________________________________
void SlimTcpConnectionHandler_TCL::onConnection (int handle)
{
   Threaded_TCP_client *new_thread_TCP = new Threaded_TCP_client(Interface_mere, handle, L_rappel_nouvelle_connection);
   L_Threaded_TCP_client.Ajouter_a_la_fin( new_thread_TCP );
   Vider_Threaded_TCP_client_termines();

 new_thread_TCP->resume();
}

//______________________________________________________________________________
void SlimTcpConnectionHandler_TCL::Vider_Threaded_TCP_client_termines()
{alx_element_liste <Threaded_TCP_client*> *it     = L_Threaded_TCP_client.Premier()
                                        , *it_fin = L_Threaded_TCP_client.Fin()
                                        , *it_svt;
 Threaded_TCP_client *TC;
 while(it!=it_fin)
  {it_svt = it->svt;
   TC = it->E();
   if( TC->Is_finised() ) {
     L_Threaded_TCP_client.Retirer( it );
     delete TC;
     TC = NULL;
    }
   it = it_svt;
  }
}

/*
//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
Serveur_TCP_TCL::Serveur_TCP_TCL(unsigned int port) : SlimThread(true)
{connection_handler_TCL = new SlimTcpConnectionHandler_TCL();
 Serveur_TCP            = new SlimTcpServerSocket(port);
   Serveur_TCP->setConnectionHandler( connection_handler_TCL );
 this->port             = port;
 init();
}

//______________________________________________________________________________
Serveur_TCP_TCL::~Serveur_TCP_TCL()
{}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
void Serveur_TCP_TCL::init()
{Serveur_TCP->setConnectionHandler( connection_handler_TCL );
 resume();
}

//______________________________________________________________________________
void Serveur_TCP_TCL::run(void)
{Serveur_TCP->start();
}
*/
//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________

//______________________________________________________________________________
Image_net_loader::Image_net_loader( const char *ip
                                  , const unsigned int port
                                  , alx_interface_sdl_opengl_1 *mere
                                  , const char *nom_img
                                  , alx_noeud_image_sdl_opengl *img_locale) : SlimThread(true), finished(false), thread_termine(false), sock_TCP()
{this->cc_nom_img     = nom_img;
   this->cc_msg_to_send = "get texture of "; this->cc_msg_to_send += nom_img; this->cc_msg_to_send += ";";
 this->cc_ip       = ip;
 this->TCP_port    = port;
 this->img_locale  = img_locale;
 this->Interface_mere_of_image      = mere;
 this->delay_wait_for_retry_connection = 50;
 //this->data_buffer = (char *)NULL;
 this->chargement_texture_synchrone = new deformation_dynamique(0);
 //sock_TCP = new SlimTcpClientSocket();
 resume();
}

//______________________________________________________________________________
Image_net_loader::~Image_net_loader()
{
}

//______________________________________________________________________________
void Image_net_loader::Load_texture(void *p)
{info_for_loading_image *info_loading = (info_for_loading_image *)p;
 //printf("  Load texture\n");
 if(info_loading->tempon)
  {this->img_locale->maj_raw( info_loading->lg
                            , info_loading->ht
                            , info_loading->order
                            , info_loading->size_pixel
                            , info_loading->tempon );
   delete[] info_loading->tempon;
   info_loading->tempon = (char *)NULL;
  }
 delete info_loading; info_loading = NULL;
 L_rap_fin.Rappeler( this->img_locale );
 thread_termine = true;
}

//______________________________________________________________________________
void Image_net_loader::run(void)
{
 //printf("%s\n", this->cc_msg_to_send.Texte());
 int erreur = getConnectionTo(cc_ip.Texte(), TCP_port);
 if (erreur == SLIMSOCKET_NOERROR)
  {sendData_fromChar(this->cc_msg_to_send.Texte(), this->cc_msg_to_send.Taille());
   const unsigned int taille_txt_buffer = 10;
   int recut = 0, nb, nb_failure = 0;
   char txt_buffer[taille_txt_buffer + 1];
   while(recut < taille_txt_buffer)
    {nb = waitingForData_fromChar(txt_buffer, taille_txt_buffer, recut);
     if(nb != SLIMSOCKET_RECEPTION_FAILED)
      {recut += nb; nb_failure = 0;} else {nb_failure++;
                                           if(nb_failure > 10) break;
                                           Sleep(delay_wait_for_retry_connection);
                                          }
    }
   cc_tmp = txt_buffer;
   unsigned int pos = 0;
   const unsigned int taille_data_buffer = cc_tmp.Lire_entier(pos), taille_reelle_image_locale = img_locale->Lg() * img_locale->Ht() * img_locale->Nb_octets_par_pixel();
   if(nb_failure == 0 && taille_data_buffer >=  taille_reelle_image_locale)
    {char *data_buffer = new char[taille_data_buffer + 1];
     recut = 0; nb_failure = 0;
     while(recut < (int)taille_data_buffer)
      {nb = waitingForData_fromVoid(data_buffer, taille_data_buffer, recut, false);
       if(nb != SLIMSOCKET_RECEPTION_FAILED)
         {recut += nb; nb_failure = 0;} else {nb_failure++;
                                              if(nb_failure > 10) break;
                                              Sleep(delay_wait_for_retry_connection);
                                             }
      }
     //unsigned int taille_raw = Amin(recut, (int)(img_locale->Lg()) * (int)(img_locale->Ht()) * Amax(4, (int)img_locale->Nb_octets_par_pixel()));
     Interface_mere_of_image->Demande_acces();
       alx_methode_rappel_de<Image_net_loader> *rappel_chargement_texture = new alx_methode_rappel_de<Image_net_loader>( this, &Image_net_loader::Load_texture );
         info_for_loading_image *info_for_loader = new info_for_loading_image;
           info_for_loader->lg         = img_locale->Lg();
           info_for_loader->ht         = img_locale->Ht();
           info_for_loader->order      = img_locale->Ordonnancement_couleurs();
           info_for_loader->size_pixel = img_locale->Nb_octets_par_pixel();
           info_for_loader->tempon     = data_buffer;
         rappel_chargement_texture->Params( info_for_loader );
       chargement_texture_synchrone->abonner_a_rappel_fin( rappel_chargement_texture );
       chargement_texture_synchrone->Demarrer();
       Interface_mere_of_image->Ajouter_deformation_dynamique( chargement_texture_synchrone );
       //printf("  Loading process added\n");
     Interface_mere_of_image->Liberer_acces();
    } else {
            L_rap_fin.Rappeler( this->img_locale );
            thread_termine = true;
            printf("  ___!!!___ Unable to load image %s, error taille_data_buffer (%d) < X*Y*B (%d)\n", cc_nom_img.Texte(), taille_data_buffer, taille_reelle_image_locale);
           }
   //sock_TCP.closeSocket();
   } else {
           L_rap_fin.Rappeler( this->img_locale );
           thread_termine = true;
           printf("  ___!!!___ Unable to load image %s, error %d\n", cc_nom_img.Texte(), erreur);
          }
 sock_TCP.closeSocket();
 finished = true;
}

//______________________________________________________________________________
int Image_net_loader::getConnectionTo(const char *ip, int port) {return sock_TCP.getConnectionTo(ip, port);}

//______________________________________________________________________________
int Image_net_loader::sendData_fromVoid      (const void *tempon, int nbByteToSend)           {return sock_TCP.sendData(static_cast<const char*>(tempon), nbByteToSend);}
//______________________________________________________________________________
int Image_net_loader::sendData_fromChar      (const char *tempon, int nbByteToSend)           {return sock_TCP.sendData(tempon, nbByteToSend);}
//______________________________________________________________________________
int Image_net_loader::waitingForData_fromVoid(void *tempon, const unsigned int taille_buffer, const unsigned int pos, const bool add_zero) {return sock_TCP.waitingForData(static_cast<char*>(tempon), taille_buffer, pos, add_zero);}
//______________________________________________________________________________
int Image_net_loader::waitingForData_fromChar(char *tempon, const unsigned int taille_buffer, const unsigned int pos)                      {return sock_TCP.waitingForData(                   tempon , taille_buffer, pos, true );}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
#include "../Route 66/route66/includes/route66.h"
//______________________________________________________________________________

Texture_Server_TCP::Texture_Server_TCP(const unsigned int port, alx_interface_sdl_opengl_1 *mere) : SlimThread(true)
{connection_handler_TCL  = new SlimTcpConnectionHandler_TCL(mere);
 Serveur_TCP             = new SlimTcpServerSocket(port);
 this->port              = port;
 mutex_acces_tables      = new Mutex();
 Interface_mere_of_image = mere;
   Interface_mere_of_image->abonner_a_creation_noeud_replique( FONCTION_DE_RAPPEL(Texture_Server_TCP, &Texture_Server_TCP::Ajouter_reference_noeud) );
   Interface_mere_of_image->Route_66()->bindMessageCallback( "^Texture_Server_TCP from (.*) is at TCP port (.*)$"
                                               , new Route66MessageCallbackOf<Texture_Server_TCP>(this, &Texture_Server_TCP::Save_info_Texture_Server_TCP) );
   alx_chaine_char cc_tmp = "^Get Texture_Server_TCP from ";
     cc_tmp += Interface_mere_of_image->IP();
     cc_tmp += "$";
   Interface_mere_of_image->Route_66()->bindMessageCallback( cc_tmp.Texte()
                                               , new Route66MessageCallbackOf<Texture_Server_TCP>(this, &Texture_Server_TCP::Send_info_Texture_Server_TCP) );
   Interface_mere_of_image->Route_66()->bindMessageCallback( "^Get Texture_Server_TCP from EVERYWHERE$"
                                               , new Route66MessageCallbackOf<Texture_Server_TCP>(this, &Texture_Server_TCP::Send_info_Texture_Server_TCP) );

 Serveur_TCP->setConnectionHandler( connection_handler_TCL );
   connection_handler_TCL->abonner_a_nouvelle_connection( FONCTION_DE_RAPPEL(Texture_Server_TCP, &Texture_Server_TCP::Interp_msg) );
 init();
}

//______________________________________________________________________________
Texture_Server_TCP::~Texture_Server_TCP()
{delete Serveur_TCP;            Serveur_TCP            = NULL;
 delete connection_handler_TCL; connection_handler_TCL = NULL;
}

//______________________________________________________________________________
void Texture_Server_TCP::init()
{delay_wait_for_retry_connection = 50;
 resume();
}

//______________________________________________________________________________
void Texture_Server_TCP::run(void)
{Interface_mere_of_image->Demande_acces();
   Interface_mere_of_image->Route_66()->sendMessage( "Get Texture_Server_TCP from EVERYWHERE" );
 Interface_mere_of_image->Liberer_acces();
 Serveur_TCP->start();
}

//______________________________________________________________________________
const int Texture_Server_TCP::Get_port(const alx_chaine_char &nom_noeud) const
{unsigned int pos = nom_noeud.Position_du_prochain('_', 2); pos++; // Numéro du noeud suivit de '_'
 alx_chaine_char cc_tmp_IP;
 cc_tmp_IP.Maj(nom_noeud.Texte(), pos, nom_noeud.Taille()-pos);
 if( Table_ports.find( cc_tmp_IP ) == Table_ports.end() ) {
   return 0;}
 return Table_ports[ cc_tmp_IP ];
}

//______________________________________________________________________________
void Texture_Server_TCP::Get_IP(const alx_chaine_char &nom_noeud, alx_chaine_char &IP) const
{unsigned int pos_deb = nom_noeud.Position_du_prochain('_', 2      ); pos_deb++; // Numéro du noeud suivit de '_'
 unsigned int pos_fin = nom_noeud.Position_du_prochain('_', pos_deb); // IP
 IP.Maj(nom_noeud.Texte(), pos_deb, pos_fin-pos_deb);
}

//______________________________________________________________________________
void Texture_Server_TCP::Rappel_Get_datas_for(void *p)
{Get_datas_for( (alx_noeud_scene *)p );
}

//______________________________________________________________________________
const bool Texture_Server_TCP::Retirer_transfert( Envoie_Object *t)
{return L_Envoie_Objects.Retirer( t );
}

//______________________________________________________________________________
void Texture_Server_TCP::Rappel_end_texture_loading(void *p)
{alx_noeud_image_sdl_opengl *img_locale = (alx_noeud_image_sdl_opengl*) p;
 Interface_mere_of_image->Demande_acces();
 mutex_acces_tables->lock();
   Envoie_Object *transfert = get_Transfert_en_cours( img_locale );
   if( transfert && transfert->A_renvoyer() )
   {alx_chaine_char img_originale_IP; Get_IP(img_locale->Nom(), img_originale_IP);
    const int img_originale_PORT = Get_port( img_locale->Nom() );
    transfert->A_renvoyer(false);
    Image_net_loader *inl = new Image_net_loader( img_originale_IP.Texte()
                                                             , img_originale_PORT
                                                             , Interface_mere_of_image
                                                             , img_locale->Nom().Texte()
                                                             , img_locale);
    inl->set_delay_wait_for_retry_connection( this->get_delay_wait_for_retry_connection() );
    inl->abonner_a_fin( FONCTION_DE_RAPPEL(Texture_Server_TCP, &Texture_Server_TCP::Rappel_end_texture_loading) );
    L_Image_net_loader.Ajouter_a_la_fin( inl );
    } else {
            if(transfert)
             {Retirer_transfert( transfert );
              delete transfert;
             }
           }
// Rappel de maj des polygones dépendants
 alx_liste<alx_noeud_polygone_sdl_opengl*> *L_polys = Table_dependances_img_polys[img_locale];
 alx_element_liste<alx_noeud_polygone_sdl_opengl*> *it     = L_polys->Premier()
                                                 , *it_fin = L_polys->Fin();
 for(; it!=it_fin; it=it->svt)
  {Maj_texture_polygone( it->E() );
  }

 mutex_acces_tables->unlock();
 Interface_mere_of_image->Liberer_acces();
}

//______________________________________________________________________________
Envoie_Object* Texture_Server_TCP::get_Transfert_en_cours(alx_noeud_scene *noeud, const bool creer_si_non_existant)
{Envoie_Object *rep = NULL;
 alx_element_liste<Envoie_Object*> *it     = L_Envoie_Objects.Premier()
                                 , *it_fin = L_Envoie_Objects.Fin();
 for(; it!=it_fin; it=it->svt)
  {if( it->E()->Noeud() == noeud ) {rep = it->E(); break;}
  }

 if (rep == NULL && creer_si_non_existant)
  {rep = new Envoie_Object(noeud);
   L_Envoie_Objects.Ajouter_a_la_fin( rep );
  }

 return rep;
}

//______________________________________________________________________________
void Texture_Server_TCP::Get_datas_for(alx_noeud_scene *noeud)
{mutex_acces_tables->lock();
// Check if not still a transfert in court
 Envoie_Object *transfert = get_Transfert_en_cours(noeud, true);
 if( transfert->En_cours_envoie() )
  {transfert->A_renvoyer(true);
  } else {// If not then trigger the transfert
          if( noeud->Nom_classe() == "alx_noeud_image_sdl_opengl" )
           {// Vider les loader qui sont terminés
            Image_net_loader *inl;
            alx_element_liste<Image_net_loader*> *it     = L_Image_net_loader.Premier()
                                               , *it_fin = L_Image_net_loader.Fin()
                                               , *it_svt;
            while( it != it_fin )
             {it_svt = it->svt;
              inl = it->E();
              if( inl->Is_finised() ) {L_Image_net_loader.Retirer( it );
                                       delete inl;
                                       inl = NULL;}
              it = it_svt;
             }

           // Lancer un nouveau pour charger la texture associées à l'image
            alx_noeud_image_sdl_opengl *img_locale = (alx_noeud_image_sdl_opengl *) noeud;
            const int img_originale_PORT = Get_port( img_locale->Nom() );

            if(img_originale_PORT != 0)
             {alx_chaine_char img_originale_IP;
              Get_IP(img_locale->Nom(), img_originale_IP);
              transfert->En_cours_envoie(true);
              Image_net_loader *inl = new Image_net_loader( img_originale_IP.Texte()
                                                                       , img_originale_PORT
                                                                       , Interface_mere_of_image
                                                                       , img_locale->Nom().Texte()
                                                                       , img_locale);
              inl->set_delay_wait_for_retry_connection( this->get_delay_wait_for_retry_connection() );
              inl->abonner_a_fin( FONCTION_DE_RAPPEL(Texture_Server_TCP, &Texture_Server_TCP::Rappel_end_texture_loading) );
              L_Image_net_loader.Ajouter_a_la_fin( inl );
             } else {printf("Port 0 pour l'image distante \"%s\"\n", noeud->Nom().Texte());}
           } else {if( noeud->Nom_classe() == "polygon" )
                    {if( !Maj_texture_polygone( (alx_noeud_polygone_sdl_opengl*)noeud ))
                     {L_maj_dependences.Ajouter_a_la_fin( noeud );}
                    }
                  }
         }
 if( !transfert->En_cours_envoie() )
  {Retirer_transfert(transfert); delete transfert;
  }
 mutex_acces_tables->unlock();
}

//______________________________________________________________________________
void Texture_Server_TCP::Rap_Maj_texture_polygone(void *p)
{Maj_texture_polygone( (alx_noeud_polygone_sdl_opengl*)p );
}

//______________________________________________________________________________N
const bool Texture_Server_TCP::Maj_texture_polygone(alx_noeud_polygone_sdl_opengl *poly)
{// Rechercher les textures dépendantes
 bool rep = false;
 INFOS_TEXTURE *info;
 alx_noeud_image_sdl_opengl *img_originale, *new_image;

 Interface_mere_of_image->Demande_acces();
 //mutex_acces_tables->lock(); // DEBUG ?

 if( poly->Info_texture() ) {Trouver_Infos_texture_correspondant_a(poly->Info_texture()->Nom(), &info, &img_originale);} else {img_originale = NULL;}
 Trouver_Infos_texture_correspondant_a(poly->Texture_1_desiree(), &info, &new_image);
   if(img_originale != new_image) {poly->Info_texture ( info );
                                   rep = true;}
   if(img_originale && img_originale != new_image && Table_dependances_img_polys.find(img_originale) != Table_dependances_img_polys.end() ) {Table_dependances_img_polys[img_originale]->Retirer(poly);}
   if(new_image     && img_originale != new_image && Table_dependances_img_polys.find(new_image)     != Table_dependances_img_polys.end() ) {Table_dependances_img_polys[new_image    ]->Ajouter_a_la_fin(poly);}
 if( poly->Info_texture2() ) {Trouver_Infos_texture_correspondant_a(poly->Info_texture2()->Nom(), &info, &img_originale);} else {img_originale = NULL;}
 Trouver_Infos_texture_correspondant_a(poly->Texture_2_desiree(), &info, &new_image);
   if(img_originale != new_image) {poly->Info_texture ( info );
                                   rep = true;}
   if(img_originale && img_originale != new_image && Table_dependances_img_polys.find(img_originale) != Table_dependances_img_polys.end()) {Table_dependances_img_polys[img_originale]->Retirer(poly);}
   if(new_image     && img_originale != new_image && Table_dependances_img_polys.find(new_image)     != Table_dependances_img_polys.end()) {Table_dependances_img_polys[new_image    ]->Ajouter_a_la_fin(poly);}

 if(rep)
  {poly->maj();}

 //mutex_acces_tables->unlock(); // DEBUG ?
 Interface_mere_of_image->Liberer_acces();
 return rep;
}

//______________________________________________________________________________
alx_noeud_scene* Texture_Server_TCP::Noeud_repliqer_nomme( const alx_chaine_char &cc_nom )
{if( Table_replicated_nodes.find( cc_nom ) == Table_replicated_nodes.end() ) {
   return NULL;
  } else {return Table_replicated_nodes[cc_nom];}
}

//______________________________________________________________________________
const bool Texture_Server_TCP::Trouver_Infos_texture_correspondant_a( const alx_chaine_char &cc
                                                                    , INFOS_TEXTURE **ptr_info
                                                                    , alx_noeud_image_sdl_opengl **ptr_img )
{// Le nom cc indique la provenance de la texture, il peut être de plusieurs formes:
 //   Info_texture_scene_de_XXX
 //   Image_de_XXX
 //   ''
 bool rep = false;
 *ptr_img = NULL;
 if( cc.Taille() == 0)
  {rep = true;
   *ptr_info = NULL;
  } else {if( cc.Egal_a("Info_texture_scene_de_", 0, 21) )
           {alx_chaine_char cc_nom_noeud;
              cc_nom_noeud.Maj(cc.Texte(), 22, cc.Taille()-22);
            alx_noeud_scene *noeud = Noeud_repliqer_nomme(cc_nom_noeud);
            if(noeud)
             {rep = true;
              *ptr_info = noeud->Info_texture_scene();
             }
           } else {if( cc.Egal_a("Image_de_", 0, 8) )
                    {alx_chaine_char cc_nom_noeud;
                       cc_nom_noeud.Maj(cc.Texte(), 9, cc.Taille()-9);
                     alx_noeud_image_sdl_opengl *img = (alx_noeud_image_sdl_opengl*)Noeud_repliqer_nomme(cc_nom_noeud);
                     *ptr_img = img;
                     if(img)
                      {rep = true;
                       *ptr_info = img->Ptr_Info_texture();
                      }
                    }
                  }
         }
 return rep;
}

//______________________________________________________________________________
void Texture_Server_TCP::Ajouter_reference_noeud(void *p)
{alx_noeud_scene *noeud = (alx_noeud_scene *)p;
 mutex_acces_tables->lock();
   Table_replicated_nodes[noeud->Nom()] = noeud;
 mutex_acces_tables->unlock();
   // Lancer un thread pour charger les données associées
   Get_datas_for( noeud );
   if( noeud->Nom_classe() == "alx_noeud_image_sdl_opengl" )
    {alx_noeud_image_sdl_opengl *img_locale = (alx_noeud_image_sdl_opengl *) noeud;
     img_locale->abonner_a_maj_Info_texture( FONCTION_DE_RAPPEL(Texture_Server_TCP, &Texture_Server_TCP::Rappel_Get_datas_for) );
     mutex_acces_tables->lock();
       Table_dependances_img_polys[img_locale] = new alx_liste<alx_noeud_polygone_sdl_opengl *>;
     mutex_acces_tables->unlock();
    } else {if( noeud->Nom_classe() == "polygon" )
             {alx_noeud_polygone_sdl_opengl *poly = (alx_noeud_polygone_sdl_opengl*)noeud;
              poly->abonner_a_maj_Info_texture_1( FONCTION_DE_RAPPEL(Texture_Server_TCP, &Texture_Server_TCP::Rap_Maj_texture_polygone) );
              poly->abonner_a_maj_Info_texture_2( FONCTION_DE_RAPPEL(Texture_Server_TCP, &Texture_Server_TCP::Rap_Maj_texture_polygone) );
             }
           }
 // Vérifier si ce noeud n'est pas référencer par un autre
 alx_element_liste<alx_noeud_scene*> *it     = L_maj_dependences.Premier()
                                   , *it_fin = L_maj_dependences.Fin()
                                   , *it_svt;
 while(it!=it_fin)
  {noeud = it->E();
   it_svt = it->svt;
   if( noeud->Nom_classe() == "polygon" )
    {mutex_acces_tables->lock();
      if(Maj_texture_polygone( (alx_noeud_polygone_sdl_opengl*)noeud ))
       {L_maj_dependences.Retirer( noeud );}
     mutex_acces_tables->unlock();
    }
   it = it_svt;
  }
}

//______________________________________________________________________________
void Texture_Server_TCP::Save_info_Texture_Server_TCP(unsigned int argc, char **argv)
{if(argc < 2) return;
 mutex_acces_tables->lock();
 alx_chaine_char cc_tmp_IP   = argv[0]
               , cc_tmp_PORT = argv[1];
 unsigned int pos = 0;
   Table_ports[ cc_tmp_IP ] = cc_tmp_PORT.Lire_entier( pos );
 mutex_acces_tables->unlock();
}

//______________________________________________________________________________
void Texture_Server_TCP::Send_info_Texture_Server_TCP(unsigned int argc, char **argv)
{Interface_mere_of_image->Demande_acces();
   alx_chaine_char cc_tmp = "Texture_Server_TCP from ";
     cc_tmp += Interface_mere_of_image->IP();
     cc_tmp += " is at TCP port ";
     cc_tmp += port;
   Interface_mere_of_image->Route_66()->sendMessage( cc_tmp.Texte() );
 Interface_mere_of_image->Liberer_acces();
}

//________________________________buf_to_send______________________________________________
// Can be called from multiple threads...
void Texture_Server_TCP::Interp_msg(void *p)
{Threaded_TCP_client *Client_TCP = (Threaded_TCP_client *)p;
 const unsigned int taille_msg = 1024;
 char msg[taille_msg]; memset(msg, 0, taille_msg);
 int nb, recut = 0, nb_failure = 0;
 while( msg[ Amax(0, recut-1) ] != ';')
  {nb = Client_TCP->waitingForData_fromVoid(msg, taille_msg, recut, true);
   if( nb == -1)
    {if(++nb_failure > 10) break;
     Sleep(delay_wait_for_retry_connection);
    } else {nb_failure = 0; recut += nb;}
  }
 alx_chaine_char cc_msg = msg;
 if( nb_failure == 0 && cc_msg.Egal_a("get texture of ", 0, 14) ) {
   alx_chaine_char cc_nom_noeud;
   cc_nom_noeud.Maj(cc_msg.Texte(), 15, cc_msg.Taille()-16);
  Interface_mere_of_image->Demande_acces();
  mutex_acces_tables->lock();
   if( Table_local_nodes.find( cc_nom_noeud ) == Table_local_nodes.end() )
    {// Try to find it in the B207 scene graph !
     alx_noeud_scene *noeud_demande = Interface_mere_of_image->Trouver_noeud_nomme( cc_nom_noeud );
       if(noeud_demande)
        {Table_local_nodes[ cc_nom_noeud ] = noeud_demande;
        } else {Client_TCP->sendData_fromChar("0           ", 10);
                printf("Impossible de trouver le noeud demander (%s)\n", cc_nom_noeud.Texte());
                mutex_acces_tables->unlock();
                Interface_mere_of_image->Liberer_acces();
                return;}
    }
   alx_noeud_image_sdl_opengl *img = (alx_noeud_image_sdl_opengl *)Table_local_nodes[ cc_nom_noeud ];
  mutex_acces_tables->unlock();
  Interface_mere_of_image->Liberer_acces();
  // Send image datas
   alx_chaine_char cc_msg_to_send;
   img->Lock_mutex_tempon();
     unsigned int taille_buf_to_send = img->Lg() * img->Ht() * img->Nb_octets_par_pixel();
     char *buf_to_send = new char[taille_buf_to_send];
     memcpy(buf_to_send, img->Tempon_const(), taille_buf_to_send);
   img->UnLock_mutex_tempon();
     cc_msg_to_send  = img->Taille_Tempon();
     cc_msg_to_send.Ajouter("          ", 0, 10-cc_msg_to_send.Taille() );
     Client_TCP->sendData_fromChar( cc_msg_to_send.Texte(), 10                   );
     //printf("Send %d bytes for image %s ...", img->Taille(), img->Nom() );
     Client_TCP->sendData_fromVoid(buf_to_send, taille_buf_to_send);
     delete[] buf_to_send;
     //printf("OK!\n");
  } else {printf("Unknown message : \"%s\", %d failure(s)\n", msg, nb_failure);}
}

#ifndef __Server_TCP_TCL_H__
#define __Server_TCP_TCL_H__

#include "../Route 66/slimlib/slimcore/includes/slimsocket.h"
#include "../Route 66/slimlib/slimcore/includes/slimthread.h"
#include "../physique/alx_methode_rappel.h"

class alx_interface_sdl_opengl_1;

class GogoTestThreadAllocator : public SlimThread
{public :
   GogoTestThreadAllocator() : SlimThread(false) {}
   virtual void run(void) {
     char *ptr;
     while(true)
      {ptr = new char[1024];
       ptr[0] = 0; ptr[32] = 4;
       delete[] ptr;
      }
    }
};

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
class Threaded_TCP_client : public SlimThread, SlimTcpClientSocket
{private:
   Liste_de_rappel L_rappel;
   bool finished;
   alx_interface_sdl_opengl_1 *Interface_mere;

 public :
   Threaded_TCP_client(alx_interface_sdl_opengl_1 *mere, const int handle, const Liste_de_rappel &L);

   void Update_L_rappel(const Liste_de_rappel &);
   virtual void run(void);
   const bool Is_finised() const {return finished;}
   virtual void resume() {SlimThread::resume();}

   int getConnectionTo(const char *ip, int port) {return SlimTcpClientSocket::getConnectionTo(ip, port);}

   int sendData_fromVoid      (const void *tempon, int nbByteToSend)           {return SlimTcpClientSocket::sendData(static_cast<const char*>(tempon), nbByteToSend);}
   int sendData_fromChar      (const char *tempon, int nbByteToSend)           {return SlimTcpClientSocket::sendData(tempon, nbByteToSend);}
   int waitingForData_fromVoid(void *tempon, const unsigned int taille_buffer, const unsigned int pos, const bool add_zero) {return SlimTcpClientSocket::waitingForData(static_cast<char*>(tempon), taille_buffer, pos, add_zero);}
   int waitingForData_fromChar(char *tempon, const unsigned int taille_buffer, const unsigned int pos)                      {return SlimTcpClientSocket::waitingForData(                   tempon , taille_buffer, pos, true );}
};
inline Threaded_TCP_client* Void_vers_Threaded_TCP_client (void *p) {return (Threaded_TCP_client*)p;}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
class SlimTcpConnectionHandler_TCL : public SlimTcpConnectionHandler
{private:
   alx_interface_sdl_opengl_1 *Interface_mere;
   Liste_de_rappel     L_rappel_nouvelle_connection;
   alx_liste<Threaded_TCP_client*> L_Threaded_TCP_client;

   void Vider_Threaded_TCP_client_termines();

 public :
   SlimTcpConnectionHandler_TCL(alx_interface_sdl_opengl_1 *mere);

   void abonner_a_nouvelle_connection    (alx_methode_rappel *m) {L_rappel_nouvelle_connection.Ajouter_methode_a_la_fin(m);}
   void desabonner_de_nouvelle_connection(alx_methode_rappel *m) {L_rappel_nouvelle_connection.Retirer_methode(m);}

   virtual void onConnection (int handle);

   int sendData      (const char *tempon, int nbByteToSend)           {return socket.sendData(tempon, nbByteToSend);}
   int waitingForData(char *tempon, const unsigned int taille_buffer) {return socket.waitingForData(tempon, taille_buffer);}

   void               closeSocket      ()                          {SlimTcpConnectionHandler::closeSocket();}
};
inline SlimTcpConnectionHandler_TCL* Void_vers_SlimTcpConnectionHandler_TCL (void *p) {return (SlimTcpConnectionHandler_TCL*)p;}

inline char* Void_vers_string(void *p) {return (char*)p;}

/*
//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
class Serveur_TCP_TCL : public SlimThread
{private:
   unsigned int        port;
   SlimTcpServerSocket *Serveur_TCP;
     SlimTcpConnectionHandler_TCL *connection_handler_TCL;

   void init();
   virtual void run(void);

 public :
   Serveur_TCP_TCL(unsigned int port);
  ~Serveur_TCP_TCL();

   void abonner_a_nouvelle_connection    (alx_methode_rappel *m) {connection_handler_TCL->abonner_a_nouvelle_connection    (m);}
   void desabonner_de_nouvelle_connection(alx_methode_rappel *m) {connection_handler_TCL->desabonner_de_nouvelle_connection(m);}

   const unsigned int Port  ()                               const {return port;}
   SlimTcpConnectionHandler_TCL* get_SlimTcpConnectionHandler_TCL() {return connection_handler_TCL;}
};
*/
//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
//#include "../Route 66/slimlib/slimcore/includes/slimthread.h"
//#include "../Route 66/slimlib/slimcore/includes/slimsocket.h"
#include "alx_chaine_char.h"

class alx_noeud_scene;
class alx_noeud_image_sdl_opengl;
class alx_noeud_polygone_sdl_opengl;
class INFOS_TEXTURE;
class deformation_dynamique;
class Image_net_loader : public SlimThread
{private:
   class info_for_loading_image
    {public:
       unsigned int lg, ht, order, size_pixel;
       char *tempon;
    };
   SlimTcpClientSocket sock_TCP;
   bool finished, thread_termine;
   alx_interface_sdl_opengl_1 *Interface_mere_of_image;
   alx_noeud_image_sdl_opengl *img_locale;
   alx_chaine_char cc_nom_img, cc_ip, cc_msg_to_send, cc_tmp;
   unsigned int TCP_port, delay_wait_for_retry_connection;
   deformation_dynamique *chargement_texture_synchrone;
   Liste_de_rappel L_rap_fin;
   //char *data_buffer;

   void Load_texture(void *p);
   void run(void);

 public :
   Image_net_loader(const char *ip, const unsigned int port, alx_interface_sdl_opengl_1 *mere, const char *nom_img, alx_noeud_image_sdl_opengl *img_locale);
  ~Image_net_loader();

   const bool Is_finised() const {return finished&&thread_termine;}
   virtual void resume() {SlimThread::resume();}

   int getConnectionTo(const char *ip, int port);
   void abonner_a_fin(alx_methode_rappel *m) {L_rap_fin.Ajouter_methode_a_la_fin(m);}

   int sendData_fromVoid      (const void *tempon, int nbByteToSend);
   int sendData_fromChar      (const char *tempon, int nbByteToSend);
   int waitingForData_fromVoid(void *tempon, const unsigned int taille_buffer, const unsigned int pos, const bool add_zero);
   int waitingForData_fromChar(char *tempon, const unsigned int taille_buffer, const unsigned int pos);

   const unsigned int get_delay_wait_for_retry_connection() const {return delay_wait_for_retry_connection;}
   void set_delay_wait_for_retry_connection(const unsigned int v) {delay_wait_for_retry_connection = v;}
};

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
#include <hash_map>

class alx_noeud_scene;

//______________________________________________________________________________
class Envoie_Object {
  private:
    alx_noeud_scene *obj;
    bool en_cours_envoie, a_renvoyer;
  public :
    inline const bool En_cours_envoie() const {return en_cours_envoie;}
    inline void En_cours_envoie(const bool b) {en_cours_envoie = b;}
    inline const bool A_renvoyer() const {return a_renvoyer;}
    inline void A_renvoyer(const bool b) {a_renvoyer = b;}
    inline const alx_noeud_scene* Noeud() const {return obj;}
    Envoie_Object(alx_noeud_scene *o) {obj = o; en_cours_envoie = false; a_renvoyer = false;}
 };

//______________________________________________________________________________
struct Hash_config_for_alx_chaine_char
{	enum {bucket_size = 4,
		  min_buckets = 8};
	
	size_t operator()(const alx_chaine_char &key) const {
		return (size_t)key.Texte();// .operator();
		}
    bool operator()(const alx_chaine_char & x, const alx_chaine_char & y) const {
        return x > y;
    }
};

//______________________________________________________________________________
class Texture_Server_TCP : public SlimThread
{private:
   alx_liste<Envoie_Object *> L_Envoie_Objects;
     Envoie_Object* get_Transfert_en_cours(alx_noeud_scene *noeud, const bool creer_si_non_existant = false);
     const bool Retirer_transfert( Envoie_Object *t);
     void Rappel_end_texture_loading(void *p);
   unsigned int        port, delay_wait_for_retry_connection;
   alx_interface_sdl_opengl_1 *Interface_mere_of_image;
   mutable stdext::hash_map<alx_chaine_char, alx_noeud_scene*, Hash_config_for_alx_chaine_char > Table_replicated_nodes, Table_local_nodes;
   mutable stdext::hash_map<alx_noeud_image_sdl_opengl*, alx_liste<alx_noeud_polygone_sdl_opengl*> *> Table_dependances_img_polys;
   mutable stdext::hash_map<alx_chaine_char, int, Hash_config_for_alx_chaine_char >              Table_ports;
   //stdext::hash_map<int, int> Bobo;
   SlimTcpServerSocket *Serveur_TCP;
     SlimTcpConnectionHandler_TCL *connection_handler_TCL;
   Mutex *mutex_acces_tables;
   alx_liste<Image_net_loader *> L_Image_net_loader;

   alx_liste<alx_noeud_scene*> L_maj_dependences;
   void Rap_Maj_texture_polygone(void *p);
   const bool Maj_texture_polygone(alx_noeud_polygone_sdl_opengl *poly, bool force_update = false);
   const bool Trouver_Infos_texture_correspondant_a(const alx_chaine_char &cc, INFOS_TEXTURE **ptr_info, alx_noeud_image_sdl_opengl **ptr_img );
   alx_noeud_scene* Noeud_repliqer_nomme( const alx_chaine_char &cc_nom );

   void init();
   virtual void run(void);
   void Interp_msg(void *p);
   void Ajouter_reference_noeud(void *p);
   void Save_info_Texture_Server_TCP(unsigned int argc, char **argv);
   void Send_info_Texture_Server_TCP(unsigned int argc, char **argv);
   void Get_datas_for(alx_noeud_scene *noeud);
   void Rappel_Get_datas_for(void *p);

   const unsigned int get_delay_wait_for_retry_connection() const {return delay_wait_for_retry_connection;}
   void set_delay_wait_for_retry_connection(const unsigned int v) {delay_wait_for_retry_connection = v;}

   const int Get_port(const alx_chaine_char &nom_noeud) const;
   void Get_IP(const alx_chaine_char &nom_noeud, alx_chaine_char &IP) const;

 public :
   Texture_Server_TCP(const unsigned int port, alx_interface_sdl_opengl_1 *mere);
  ~Texture_Server_TCP();
};

#endif

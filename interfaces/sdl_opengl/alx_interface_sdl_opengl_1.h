#ifndef __ALX_INTERFACE_SDL_OPENGL_1_H__
#define __ALX_INTERFACE_SDL_OPENGL_1_H__

#include "..\..\physique\alx_geometrie2D.h"

#include "..\alx_evennement_pointeur.h"
#include "..\alx_liant_ptr_contact.h"
#include "..\alx_simulateur_couche.h"
#include "..\alx_simulateur_pointeurs.h"
#include "..\alx_simulateur_pointeurs_souris.h"
#include "..\alx_simulateur_pointeurs_joystick.h"
#include "..\alx_simulateur_pointeurs_TCL.h"
#include "..\alx_lieur_ptr_liant.h"

#include "affichage_pointeurs.h"

#include "..\alx_simulateur_clavier.h"

//#include "..\classiques\alx_fiche.h"
//#include "..\classiques\alx_fiche_opengl.h"

#include "alx_prerendeur.h"
#include "alx_noeud_transformations_sdl_opengl.h"

#include "..\..\Comet Olfa\olfa_comet.h"

#include "C:\FMOD\api\inc\fmod.h"


//______________________________________________________________________________

class Route66;
class Mutex;
class alx_horloge_vectorielle;
class alx_groupe_bigre;
class deformation_dynamique;
class alx_film;
class Image_net_loader;

class message_a_traiter
{public:
   alx_chaine_char cc_ip, cc_racine, cc_maj, cc_h, cc_ptr;
   unsigned int num_g;
   message_a_traiter( const unsigned int g
                    , const alx_chaine_char &ip
                    , const alx_chaine_char &h
                    , const alx_chaine_char &racine
                    , const alx_chaine_char &maj
                    , const alx_chaine_char &ptr) {num_g     = g;
                                                   cc_ip     = ip;
                                                   cc_h      = h;
                                                   cc_racine = racine;
                                                   cc_maj    = maj;
                                                   cc_ptr    = ptr;}
   message_a_traiter( const unsigned int g
                    , const char *ip
                    , const char *h
                    , const char *racine
                    , const char *maj
                    , const char *ptr) {num_g     = g;
                                        cc_ip     = ip;
                                        cc_h      = h;
                                        cc_racine = racine;
                                        cc_maj    = maj;
                                        cc_ptr    = ptr;}
};


const unsigned int FSOUND_Mono    () {return FSOUND_MONO;}
const unsigned int FSOUND_Stereo  () {return FSOUND_STEREO;}
const unsigned int FSOUND_16b     () {return FSOUND_16BITS;}
const unsigned int FSOUND_8b      () {return FSOUND_8BITS;}
const unsigned int FSOUND_unsigned() {return FSOUND_UNSIGNED;}
const unsigned int FSOUND_signed  () {return FSOUND_SIGNED;}

class Info_for_sound_CB;
class B207_info_sound_TCL {
  private:
    Info_for_sound_CB    *ifscb;
    //alx_interpreteur_tcl *interp_TCL;
    alx_methode_rappel   *rappel;
    void *buff;
    int len;

  public :
    B207_info_sound_TCL( Info_for_sound_CB *info
                       /*, alx_interpreteur_tcl *interp*/
                       , alx_methode_rappel *rap) {ifscb = info; /*interp_TCL = interp;*/ rappel = rap;}
    Info_for_sound_CB   * get_Info_for_sound_CB() {return ifscb;}
    /*alx_interpreteur_tcl* get_Interp_TCL       () {return interp_TCL;}*/
    alx_methode_rappel* get_Rappel()                        {return rappel;}
    void                set_Rappel(alx_methode_rappel *rap) {rappel = rap;}
    void*               get_buffer()                        {return buff;}
    void                set_buffer(void *b)                 {buff = b;}
    const int           get_buff_len()                      {return len;}
    void                set_buff_len(const int l)           {len = l;}
    void                set_buf_infos(void *b, const int l) {buff = b; len = l;}

    const int Copy_to_buffer(const int dec_buf, void *buf_src, const int deb_buf_src, const int size_buf_src) {
      int nb_to_copy = Amin(size_buf_src, len-dec_buf);
      memcpy((char*)buff+dec_buf, (char*)(buf_src) + deb_buf_src, nb_to_copy);
      return nb_to_copy;
     }
};
Info_for_sound_CB* Void_vers_B207_info_sound_TCL(void *ptr) {return (Info_for_sound_CB*)ptr;}

//signed char F_CALLBACKAPI B207_FMOD_Stream_Info_audio(FSOUND_STREAM *stream, void *buff, int len, void *userdata);
FSOUND_STREAMCALLBACK Get_B207_FMOD_Stream_Info_audio();

//______________________________________________________________________________
class R66_B_Rappel_TCL_binding
{private:
   alx_methode_rappel *B_rap;
   Route66 *R66;
   unsigned int cp_argc;
   char **      cp_argv;

 public :
   R66_B_Rappel_TCL_binding(Route66 *r66, const char *regexp, alx_methode_rappel *m);
   void CallBack(unsigned int argc, char **argv);

   unsigned int Nb_args() const {return cp_argc;}
   const char * Arg_num(const unsigned int n) const {return cp_argv[n];}
};
R66_B_Rappel_TCL_binding* Void_vers_R66_B_Rappel_TCL_binding(void *p) {return (R66_B_Rappel_TCL_binding*)p;}

//______________________________________________________________________________
class alx_interface_sdl_opengl_1 : public olfa_comet
{private:
   unsigned int nb_milliseconds_ellapsed;
  // Liste de rappel pour ajout de simulateurs
   Liste_de_rappel L_rap_simulateur_sup, L_rap_post_simulation
                 , L_rap_creation_noeud_replique;

  // Horloge logique vectorielle
   alx_horloge_vectorielle *horloge_tmp;

  // Nécessaire au cause du multi-thread, OpenGL ne s'exécute bien que dans un thread, il faut donc passer les infos d'un thread à l'autre.
   alx_evennement_pointeur evt_ptr_tmp;
   info_comp_evennement    info_comp_tmp;
   unsigned int nb_info_ptr;
   unsigned int der_chgmnt_L_fils;
   alx_liste<alx_repere2D*> L_repere, L_repere_groupe;
   alx_point2D pt_tmp_ptr_groupe;
   alx_liste<message_a_traiter*> L_messages_a_traiter, L_messages_a_traiter_temp;
  // Optimisation
   alx_vecteur2D A_v_tmp;
   alx_chaine_char message_emis, entete_message, A_cc_tmp
                 , cc_maj_tmp, cc_h_tmp, cc_ip_tmp, cc_racine_tmp
                 , cc_nom_tmp, cc_type_tmp // Pour la mise à jour des interfcaes distantes.
                 , cc_nom_ptr_tmp;
   unsigned int taille_entete;

  // Sérialisation
   alx_groupe_bigre *Tgb; unsigned int nb_gb;
   alx_chaine_char serialisation_type     , serialisation_contenu
                 , serialisation_type_diff, serialisation_contenu_diff;
  // Pour le cas du pré-rendu avec la sérialisation...on veut ne pas oublier
  // qu'on a voulu pré-rendre un noeud, même quand on reçoit un message, disant
  // qu'on ne veut plus le pre-rendre, AVANT de l'avoir pré-rendu (teu diou!)
   P_alx_noeud_scene Tab_noeud_a_prerendre[1024]; unsigned int nb_noeud_a_prerendre;

  // Reste
   Route66 *route66;
   Mutex *mutex_R66, *mutex_deformations/*, *M_EmissionReception*/;
   alx_chaine_char cc_IP;

   alx_liste<deformation_dynamique*> L_deformations_dynamiques; // Gestion d'une liste de déformations dynamiques

   int ecran_x, ecran_y;
   double angle_camera;

   unsigned int groupe_en_cours;
   int pointeur_x, pointeur_y;
   int id_souris, id_joysticks, nb_joysticks;
   SDL_Joystick **sdl_joy;

   alx_point2D coin_bg;
   alx_vecteur2D vect_h, vect_v;
   alx_vecteur2D trans;
   double rotation;

   alx_simulateur_clavier           *sim_clavier;
   alx_simulateur_dispositif_saisie *sim_dispo_saisis;

   affichage_pointeurs               *sim_aff_pointeurs;
   alx_simulateur_pointeurs_souris   *sim_ptr_souris;
   alx_simulateur_pointeurs_joystick *sim_joystick;
   alx_simulateur_pointeurs          *sim_pointeurs;
   alx_liant_ptr_contact             *liant_ptr_contact;
   alx_simulateur_couche             *sim_couche;

   alx_prerendeur                   *prerendeur;

   void Ajouter_membre(unsigned int argc, char **argv);
   void Retirer_membre(unsigned int argc, char **argv);
   void Lister_groupes(unsigned int argc, char **argv);
   //void Info_groupes(unsigned int argc, char **argv);
   void Maj_horloge(unsigned int argc, char **argv);
   void Maj_membre(unsigned int argc, char **argv);

  // Analyse et mise à jour du groupe à partir d'un message
   alx_noeud_scene* Creer_noeud( const alx_chaine_char &T
                               , unsigned int pos
                               , const alx_chaine_char &nom
                               , const alx_chaine_char &cc_maj
                               , unsigned int &pos_finale );
   void Analyser_maj( const unsigned int g
                    , const alx_chaine_char &maj
                    , const alx_chaine_char &h
                    , const alx_chaine_char &racine
                    , const alx_chaine_char &ip
                    , const alx_chaine_char &cc_ptr );
   void Etablir_liens_entre_noeuds(alx_groupe_bigre &gb, const alx_liste<alx_chaine_char> &L_noms);
  // Gestion de la cohérnce entre threads
   int compteur;
   bool ne_pas_emettre_changement, emission_changement_en_cours
      , ne_pas_traiter_messages  , traitement_messages_en_cours;
   void Emission_changement_en_cours(const bool b) {emission_changement_en_cours = b;}
   const bool Emission_changement_en_cours() const {return emission_changement_en_cours;}
   void Ne_pas_emettre_changement(const bool b) {ne_pas_emettre_changement = b;}
   const bool Ne_pas_emettre_changement() const {return ne_pas_emettre_changement;}
   void Ne_pas_traiter_messages(const bool b) {ne_pas_traiter_messages = b;}
   const bool Ne_pas_traiter_messages() const {return ne_pas_traiter_messages;}
   void Traitement_messages_en_cours(const bool b) {traitement_messages_en_cours = b;}
   const bool Traitement_messages_en_cours() const {return traitement_messages_en_cours;}

   class zic {public:
                FSOUND_STREAM *strm;
                int canal;

                zic() {}
                zic(FSOUND_STREAM *s, int c) {strm = s; canal = c;}

                const zic &operator =(const zic &z) {strm = z.strm; canal = z.canal; return *this;}
             };
   alx_liste<zic> L_musique_a_demarrer;

 public :
   alx_lieur_ptr_liant             *lieur_ptr_liant;
   alx_simulateur_points_contact   *sim_contact;
   alx_simulateur_points_contact*   Sim_contact() {return sim_contact;}
  // les constituants publiques:
   alx_noeud_scene    *A_scene;
   alx_noeud_scene&    Arbre_scene() const {return *A_scene;}
   alx_noeud_scene*    Noeud_scene() const {return A_scene;}

  // Gestion des accès
   void Demande_acces();
   void Liberer_acces();

  // Les constructeurs
   alx_interface_sdl_opengl_1( int ex, int ey, double angle, Route66 *r
                             //, cogitant::Environment *e
                             , alx_interpreteur_tcl  *interp );
   ~alx_interface_sdl_opengl_1();

  // Accès aux noeuds répliqués sur les groupes (distribution par ~UDP~ de l'IHM)
   alx_noeud_scene* Adresse_noeud   (const unsigned int groupe, const char *nom);
   alx_noeud_scene* Adresse_noeud_CC(const unsigned int groupe, const alx_chaine_char &nom);

  // Les simulateurs supplémentaires peuvent être branchés par là:
   alx_simulateur_pointeurs_souris* Sim_ptr_souris() {return sim_ptr_souris;}
   inline void abonner_a_fin_simulation    (alx_methode_rappel *m) {L_rap_post_simulation.Ajouter_methode_a_la_fin(m);}
   inline void desabonner_de_fin_simulation(alx_methode_rappel *m) {L_rap_post_simulation.Retirer_methode(m);}
   inline void abonner_a_simulateur    (alx_methode_rappel *m) {L_rap_simulateur_sup.Ajouter_methode_a_la_fin(m);}
   inline void desabonner_de_simulateur(alx_methode_rappel *m) {L_rap_simulateur_sup.Retirer_methode(m);}
   int* Ecran_X()             {return &ecran_x;}
   int* Ecran_Y()             {return &ecran_y;}
   alx_point2D* Coin_BG()     {return &coin_bg;}
   alx_vecteur2D* Vecteur_X() {return &vect_h;}
   alx_vecteur2D *Vecteur_Y() {return &vect_v;}

  // L'interpréteur TCL associé
   inline alx_interpreteur_tcl* Interp_TCL() {return olfa_comet::Interpreteur();}
  // La gestion des groupes :
   void Creer_groupe(const unsigned int nb, const char *nom, alx_noeud_scene *racine, const bool emettre_pointeurs, const bool visualiser_pointeurs);
   void Se_retirer_de(const unsigned int g);
   void Se_rajouter_a(const unsigned int g, alx_noeud_scene *racine, const bool emettre_pointeurs, const bool visualiser_pointeurs);
   void Lister_groupes();
   void Info_groupes(const unsigned int g);
   void Infos_noeuds_groupe(const unsigned int g);
   void Emettre_horloge(const unsigned int g, const alx_chaine_char &);
   void Emettre_interface(const alx_chaine_char &);
   void Emettre_changements(const bool b);
   const char* IP() {return cc_IP.Texte();}

  // Abonnements relatifs à la réplication des noeuds du groupe
   inline void abonner_a_creation_noeud_replique    (alx_methode_rappel *m) {L_rap_creation_noeud_replique.Ajouter_methode_a_la_fin(m);}
   inline void desabonner_de_creation_noeud_replique(alx_methode_rappel *m) {L_rap_creation_noeud_replique.Retirer_methode(m);}

  // Gestion de la souris,
   void Afficher_souris();
   void Cacher_souris();

  // Gestion des animations (AVI etc...)
   alx_film* Ouvrir_film(const char *nom, INFOS_TEXTURE *infos_texture);
   alx_film* Ouvrir_film(const char *nom);
   INFOS_TEXTURE* Texture_film(alx_film *f);
   void Jouer_film(alx_film *f);
   void Finir_film(alx_film *f);

  // La gestion du son
   void Init_film();
   void Init_son( const unsigned int hz = 44100
                , const unsigned int nb_voices = 32)        {FSOUND_Init(hz, nb_voices, FSOUND_INIT_USEDEFAULTMIDISYNTH);}
   FSOUND_STREAM* Nouveau_flux(FSOUND_STREAMCALLBACK callback, int canal
                                    , int lenbytes
                                    , unsigned int mode
                                    , int samplerate
                                    , void *userdata );

   const bool Fermer_flux(FSOUND_STREAM *flux);

   FSOUND_STREAM* Jouer_musique ( const char *nom
                                , const unsigned int canal) {FSOUND_STREAM *musique = FSOUND_Stream_Open( nom
                                                                                                        , FSOUND_NONBLOCKING, 0, 0);
                                                             L_musique_a_demarrer.Ajouter_a_la_fin( zic(musique, canal) );
                                                             return musique;
                                                            }
   void Demarrer_musique_en_attente()                       {alx_element_liste<zic> *it     = L_musique_a_demarrer.Premier()
                                                                                  , *it_fin = L_musique_a_demarrer.Fin()
                                                                                  , *it_svt;
                                                             while(it != it_fin)
                                                              {it_svt = it->svt;
                                                               if( FSOUND_Stream_GetOpenState(it->E().strm) == 0 )
                                                                {FSOUND_Stream_Play(it->E().canal, it->E().strm);
                                                                 L_musique_a_demarrer.Retirer( it );
                                                                }
                                                               it = it_svt;
                                                              }
                                                            }
   const int Volume_musique(const int canal) const          {return FSOUND_GetVolume(canal);}
   void Volume_musique(const int canal, const int vol)      {FSOUND_SetVolume(canal, vol);}
   void Finir_musique(FSOUND_STREAM *strm)                  {FSOUND_Stream_Close(strm);}
   void Fin_son()                                           {FSOUND_Close();}


  // Les méthodes
   void Translation(const alx_vecteur2D &t);
   void Rotation(const double r);

  // Méthodes liées aux listes de simulations supplémentaires
   inline void Ajouter_deformation_dynamique(deformation_dynamique *d) {L_deformations_dynamiques.Ajouter_a_la_fin(d);}
   inline const bool Retirer_deformation_dynamique(deformation_dynamique *d) {return L_deformations_dynamiques.Retirer(d);}

  // Dire si un point est contenu dans l'interface
   info_du_contenant* Contient(alx_point2D &pt, int action);

  // La sérialisation :
   const alx_chaine_char& Serialiser_type(alx_noeud_scene *racine, unsigned int marqueur, const alx_liste<alx_repere2D*> *L_rep);
   const alx_chaine_char& Serialiser_type(alx_noeud_scene *racine, const alx_chaine_char &contenu, const alx_liste<alx_repere2D*> *L_rep);
   const alx_chaine_char& Serialiser_contenu(alx_noeud_scene *racine, unsigned int marqueur);
  // La sérialisation des différences survenues :
   const alx_chaine_char& Serialiser_type_diff(alx_noeud_scene *racine, unsigned int marqueur, const alx_liste<alx_repere2D*> *L_rep);
   const alx_chaine_char& Serialiser_type_diff(alx_noeud_scene *racine, const alx_chaine_char &contenu, const alx_liste<alx_repere2D*> *L_rep);
   const alx_chaine_char& Serialiser_contenu_diff(alx_noeud_scene *racine, unsigned int marqueur);
  // La desserialisation :
   const alx_noeud_scene* Deserialiser(const alx_chaine_char &texte);

  // Les méthodes de la boucle
   void Simuler();
   void Afficher();
   void Pre_afficher();

  // Les accès:
   inline const unsigned int ms() const {return nb_milliseconds_ellapsed;}
   inline int Largeur() {return ecran_x;}
   inline int Hauteur() {return ecran_y;}

   inline alx_prerendeur*                   Prerendeur()          {return prerendeur;}
   inline alx_simulateur_pointeurs*         Sim_pointeur()        {return sim_pointeurs;}
   inline alx_simulateur_couche*            Sim_couche()          {return sim_couche;}
   inline alx_simulateur_points_contact*    Sim_points_contacts() {return sim_contact;}
   inline alx_simulateur_dispositif_saisie* Sim_dispo_saisi()     {return sim_dispo_saisis;}

   inline Route66* Route_66()       {return route66;}

   alx_noeud_scene * Trouver_noeud_nomme(const alx_chaine_char &cc_nom_demande, alx_noeud_scene *racine ) const;
   alx_noeud_scene * Trouver_noeud_nomme(const alx_chaine_char &cc_nom_demande ) const;
};

extern "C" {
  //#define __BC_COMPILER_PIPO_PROTECTION__   1
  //#include "c:\These\Hasselt\Mutable\Eunomia\FFMpegInit.h"
}

/*int Send_data_on_chan( ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);

class Transfert_donnees_TCP
{private:
   alx_interface_sdl_opengl_1 *i_mere;
   void init();

 public :
   Transfert_donnees_TCP( alx_interface_sdl_opengl_1 *interface );
   void Repondre_a_connection (void *params);
};
*/
#endif

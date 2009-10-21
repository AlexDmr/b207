
#include "alx_interface_sdl_opengl_1.h"
//#include <gl.h>
#include "../../opengl/config_opengl.h"
#include "../../utilitaires/alx_arbre.cpp"
#include "..\fontes.h"
#include "..\choses_communes.h"
#include "..\..\physique\constantes.h"
#include "..\..\Comet Olfa\Creation_objet_dans_tcl.h"
#include "..\..\Route 66\route66\includes\route66.h"
#include "..\..\horloge\alx_horloge_vectorielle.h"
#include "..\alx_groupe_bigre.h"

#include "alx_noeud_fenetre_sdl_opengl.h"
#include "alx_noeud_fiche_sdl_opengl.h"
#include "alx_noeud_fiche_ovale_sdl_opengl.h"
#include "alx_noeud_zone_texte_sdl_opengl.h"
#include "alx_noeud_image_sdl_opengl.h"
#include "deformations_dynamiques.h"
#include "alx_noeud_polygone_sdl_opengl.h"

#include "../alx_noeud_pointeur.h"

#include <windows.h>
//#include <atlbase.h>
#include <vfw.h>

UINT texture_flash;
INFOS_TEXTURE info_texture_flash;
char *buffer_flash;

double g_ecran_y;
extern alx_prerendeur *prerendeur_defaut;

#include <Infos_audio.h>
signed char F_CALLBACKAPI B207_FMOD_Stream_Info_audio(FSOUND_STREAM *stream, void *buff, int len, void *userdata)
{B207_info_sound_TCL *Info_sound_TCL = (B207_info_sound_TCL*)userdata;
 Info_for_sound_CB    *info_for_sound_CB = Info_sound_TCL->get_Info_for_sound_CB();
 //Mutex *mutex = Info_for_sound_CB_Get_mutex(info_for_sound_CB);
 //mutex->lock();
   //alx_interpreteur_tcl *interp_TCL        = Info_sound_TCL->get_Interp_TCL();
   alx_methode_rappel   *rappel            = Info_sound_TCL->get_Rappel();
   Info_sound_TCL->set_buf_infos(buff, len);
   rappel->Rappel( Info_sound_TCL );
 //mutex->unlock();
 return TRUE;
}
/*
signed char F_CALLBACKAPI B207_FMOD_Stream_Info_audio(FSOUND_STREAM *stream, void *buff, int len, void *userdata)
{Info_for_sound_CB *info_for_sound_CB = (Info_for_sound_CB*)userdata;
 Mutex                        *mutex       = Info_for_sound_CB_Get_mutex(info_for_sound_CB);
 mutex->lock(); printf("B207_FMOD_Stream_Info_audio->Lock(%i pkt)---", Info_for_sound_CB_Nb_pkt(info_for_sound_CB));
 unsigned int total_size = Info_for_sound_CB_Size_buffers(info_for_sound_CB);
 int remain = len, nb_to_copy, index_info_wav;
 Info_buffer_audio infos_wav;

 // is there enough informations?
 if(total_size < len) {printf("---B207_FMOD_Stream_Info_audio->UnLock\n"); mutex->unlock(); return TRUE;}
 // Copy informations to the buffer
 total_size = 0;
 while(total_size < len)
  {index_info_wav = Info_for_sound_CB_Get(info_for_sound_CB, &infos_wav);
   if(index_info_wav == -1) {break;}
   nb_to_copy = Amin((int)infos_wav.size, remain);
   memcpy((char*)buff+total_size, (char*)(infos_wav.buf) + infos_wav.deb, nb_to_copy);
   total_size += nb_to_copy;
   remain     -= nb_to_copy;
   // If there all information is used
   if(nb_to_copy == (int)(infos_wav.size))
    {printf("R");
     Info_for_sound_CB_Release(info_for_sound_CB);
     printf("*%i", nb_to_copy);
    } else {// There is still some information to keep
            Info_for_sound_CB_Maj_size( info_for_sound_CB
                                      , index_info_wav
                                      , infos_wav.deb  + nb_to_copy
                                      , infos_wav.size - nb_to_copy );
            printf("|%i", nb_to_copy);
            break;
           }
  }
 printf("---B207_FMOD_Stream_Info_audio->UnLock\n"); mutex->unlock();
 return TRUE;
}
*/

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
R66_B_Rappel_TCL_binding::R66_B_Rappel_TCL_binding(Route66 *r66, const char *regexp, alx_methode_rappel *m)
{R66 = r66;
 B_rap = m;
 R66->bindMessageCallback( regexp, new Route66MessageCallbackOf<R66_B_Rappel_TCL_binding>(this, &R66_B_Rappel_TCL_binding::CallBack) );
}

//______________________________________________________________________________
void R66_B_Rappel_TCL_binding::CallBack(unsigned int argc, char **argv)
{cp_argc = argc;
 cp_argv = argv;
 B_rap->Rappel( this );
}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
FSOUND_STREAMCALLBACK Get_B207_FMOD_Stream_Info_audio() {return &B207_FMOD_Stream_Info_audio;}

//______________________________________________________________________________
//___________________________________ Les films ________________________________
//______________________________________________________________________________
void alx_interface_sdl_opengl_1::Init_film() {AVIFileInit();}

class alx_film {
 private:
   PAVIFILE    avi;
   AVIFILEINFO avi_info;
   PAVISTREAM  pStream;
   PGETFRAME   pFrame;

   int iNumFrames, iFirstFrame, num;

   const int Ouvrir(const char *n) {
     int res = 1; //AVIFileOpen(&avi, n, OF_READ, NULL);
     num = 0;
     if (res!=AVIERR_OK)
      {//an error occures
       if (avi!=NULL)
         AVIFileRelease(avi);
       return res;
      }
     AVIFileInfo(avi, &avi_info, sizeof(AVIFILEINFO));
     res = AVIFileGetStream(avi, &pStream, streamtypeVIDEO /*video stream*/, 0 /*first stream*/);
     if (res!=AVIERR_OK)
      {//an error occures
       if (pStream!=NULL)
         AVIStreamRelease(pStream);
       return res;
      }
     iFirstFrame = AVIStreamStart (pStream);
     iNumFrames  = AVIStreamLength(pStream);
     pFrame      = AVIStreamGetFrameOpen(pStream, NULL);

     return res;
    }
   int is_ok;

 public:
   INFOS_TEXTURE *texture;

   alx_film(const char *n) {
     is_ok = Ouvrir(n);
    }
   alx_film(const char *n, INFOS_TEXTURE *t) {
     is_ok = Ouvrir(n);
     texture = t;
    }
   ~alx_film() {Stop();}

   const char* Lire_image() {
     return (const char*)AVIStreamGetFrame(pFrame, num++);
    }

   void Stop() {
     if (pFrame)
       AVIStreamGetFrameClose(pFrame);
     if (pStream)
       AVIStreamRelease(pStream);
    }
  // Infos
   const unsigned int Ht() const {return avi_info.dwHeight;}
   const unsigned int Lg() const {return avi_info.dwWidth ;}
   const int Is_ok() const {return is_ok;}
   INFOS_TEXTURE* Texture() {return texture;}
};

INFOS_TEXTURE* alx_interface_sdl_opengl_1::Texture_film(alx_film *f) {return f->Texture();}

alx_film* alx_interface_sdl_opengl_1::Ouvrir_film(const char *nom, INFOS_TEXTURE *infos_texture)
{return new alx_film(nom, infos_texture);
}

alx_film* alx_interface_sdl_opengl_1::Ouvrir_film(const char *nom)
{INFOS_TEXTURE *texture = new INFOS_TEXTURE();
 alx_film *res = new alx_film(nom);
 int id_texture;
 if(!res->Is_ok())
   return NULL;

 CreerTexture(&id_texture, res->Lg(), res->Ht(), 4, GL_bvr(), texture);
 res->texture = texture;

 return res;
}

void alx_interface_sdl_opengl_1::Jouer_film(alx_film *f)
{const char *frame = f->Lire_image();
 glPushAttrib(GL_ENABLE_BIT);
   glBindTexture(GL_TEXTURE_2D, f->texture->Id_texture() );
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //GL_LINEAR);
   glCopyTexSubImage2D( GL_TEXTURE_2D, 0
                      , 0, 0 // Pas de décalage de la destination par rapport au coin bas gauche de la texture
                      , 0, 0 // On copie à partir du coin bas gauche du tempon image
                      , f->Lg(), f->Ht() );
 glPopAttrib();
}

void alx_interface_sdl_opengl_1::Finir_film(alx_film *f)
{delete f;
}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
void alx_interface_sdl_opengl_1::Afficher_souris() {
  SDL_ShowCursor(SDL_ENABLE);
  SDL_WM_GrabInput(SDL_GRAB_OFF);
 }
void alx_interface_sdl_opengl_1::Cacher_souris() {
  SDL_ShowCursor(SDL_DISABLE);
  SDL_WM_GrabInput(SDL_GRAB_ON);
 }

//______________________________________________________________________________
//_______________ La fonction de controle d'accès entre les threads_____________
//______________________________________________________________________________
void alx_interface_sdl_opengl_1::Demande_acces()
{// On se base sur le compteur
 /*compteur++;
 while(compteur > 1)
  {compteur--;
   Sleep(1);mutex_R66
   compteur++;}*/
 //printf("Demande d'accès; ");
 mutex_R66->lock();
 //printf("Accès obtenu\n");
}
void alx_interface_sdl_opengl_1::Liberer_acces() {//printf("Demande de libération; ");
                                                  mutex_R66->unlock();
                                                  //printf("Accès libéré\n");
                                                 }
//______________________________________________________________________________
//___________________________ Les messages Route66 _____________________________
//______________________________________________________________________________
void alx_interface_sdl_opengl_1::Ajouter_membre(unsigned int argc, char **argv)
{Demande_acces();
 printf("Ajouter membre\n");
 // Un autre ordinateur demande à faire parti d'un groupe, voir si on appartient à ce groupe
 //, si oui va falloir penser à emettre une description des noeuds locaux.
 unsigned int g, pos_g=0;
 sscanf(argv[1], "%i", &g); // le numéro de groupe

 // Fait on parti du même groupe?
 while( (pos_g < nb_gb)
      &&(Tgb[pos_g].Numero() != g) ) pos_g++;
 if(pos_g == nb_gb) {Liberer_acces(); return;} // On ne fait pas parti du même groupe.

 // On fait partit du même groupe, je l'ajoute à ma connaissance du groupe
 A_cc_tmp = argv[0];
 if( (A_cc_tmp == cc_IP) ) {Liberer_acces(); return;} // rien à faire, on ne fait que s'écouter parler...
 Tgb[pos_g].horloge.Inserer(0, A_cc_tmp);

 // Je prévoi de ré-émettre l'horloge, puis l'état de l'interface
 Tgb[pos_g].Emettre_IP_horloge( true );
 Tgb[pos_g].Tout_reemettre( true );
 Liberer_acces();
}

//______________________________________________________________________________
void alx_interface_sdl_opengl_1::Retirer_membre(unsigned int argc, char **argv)
{// Vérifier qu'on appartient au même groupe et modifier l'horloge en conséquence
 // Initiliser un marqueur à vrai pour émettre la nouvelle horloge avant toute émission
 // des changements survenues dans l'interface.
 Demande_acces();
 //printf("Retirer membre\n");
 unsigned int g, pos_g=0;
 sscanf(argv[1], "%i", &g); // le numéro de groupe

 // Fait on parti du même groupe?
 while( (pos_g < nb_gb)
      &&(Tgb[pos_g].Numero() != g) ) pos_g++;
 if(pos_g == nb_gb) {Liberer_acces(); return;} // On ne fait pas parti du même groupe.

 // On fait partit du même groupe, je le retire de ma connaissance du groupe
 A_cc_tmp = argv[0];
 Tgb[pos_g].horloge.Supprimer(A_cc_tmp);

 // Je prévoi de ré-émettre l'horloge.
 Tgb[pos_g].Emettre_IP_horloge( true );
 Liberer_acces();
}

//______________________________________________________________________________
// "^<interface SIZE=(.*) IP=(.*) Racine=(.*) G=([0-9]*) P=(.); H=([0-9|;|(|)]*)>(.*)$"
void alx_interface_sdl_opengl_1::Maj_membre(unsigned int argc, char **argv)
{// On reçoit une mise à jour de certains noeuds.
 // On parcours les noeuds décrits, si le noeud est inconnu dans la table de correspondance ET
 // s'il ne s'agit pas de la racine, on en crée une réplique que l'on enregistre dans la table.
 // 0) Si c'est un de nos messages, on ne le traite pas.
 //printf("Demande de MAJ membre\n");
// printf("MAJ membre\n");
 Demande_acces();
 if(argc < 3)
  {printf("PB!!!");}
 if(cc_IP == argv[1])
  {Liberer_acces();
   //printf("  Talking to ourself");
   return;}
 // 1) Vérifier qu'on appartient au même groupe.
 //printf("MAJ MEMBRE :\n 0) %s\n 1) %s\n 2) %s\n", argv[0], argv[1], argv[2]);
 unsigned int pos_g = 0
            , g, size_msg;
 sscanf(argv[3], "%i", &g);
 sscanf(argv[0], "%i", &size_msg);
 while( (pos_g < nb_gb)
      &&(Tgb[pos_g].Numero() != g) ) pos_g++;
 if( pos_g == nb_gb )
  {//printf("FIN DE MAJ MEMBRE\n");
   Liberer_acces();
   return; // On n'appartient pas au même groupe !
  } else {int taille_reelle = strlen(argv[6]);
          if(size_msg != taille_reelle)
           {printf("Message corrompue! taille obtenue %d, taille demandée %d\n", taille_reelle, size_msg);
            Liberer_acces();
            return;
           }
         }

 // 2) Enregistrer l'horloge et l'IP de l'émetteur, ainsi que la racine.
 /*cc_h_tmp      = argv[3];
 cc_ip_tmp     = argv[0];
 cc_racine_tmp = argv[1];
 cc_maj_tmp    = argv[4];*/
 // 3) Invalider les évennements qui sont postérieurs à ce message (du plus récent au plus ancien)
 // XXX A FAIRE XXX
 // 4) Traduire les évennement invalidés en tenant compte de celui qu'on insère (du plus au plus)
 // XXX A FAIRE XXX
 // 5) Faire la mise à jour prescrite dans le message
   // Créer les noeuds s'ils ne le sont pas déja, mettre à jour sinon.
   //printf("  TRAITEMENT MAJ MEMBRE\n");
   //DEBUG_2008 Ne_pas_traiter_messages( true );
   //DEBUG_2008 if( Traitement_messages_en_cours() )
    {//DEBUG_2008 printf("    TRAITEMENT MESSAGE EN COURS\n");
    //DEBUG_2008  L_messages_a_traiter_temp.Ajouter_a_la_fin( new message_a_traiter(pos_g, argv[0], argv[4], argv[1], argv[5], argv[3]) );}
     //DEBUG_2008 else
     //DEBUG_2008  {//printf("    PAS DE TRAITEMENT MESSAGE EN COURS\n");
       L_messages_a_traiter.Ajouter_a_la_fin(L_messages_a_traiter_temp);
       L_messages_a_traiter_temp.Vider();
       L_messages_a_traiter.Ajouter_a_la_fin( new message_a_traiter(pos_g, argv[1], argv[5], argv[2], argv[6], argv[4]) );}
   //DEBUG_2008  Ne_pas_traiter_messages(false);

 //printf("FIN DE MAJ MEMBRE\n");

   //Analyser_maj(pos_g, cc_maj_tmp, cc_h_tmp, cc_racine_tmp, cc_ip_tmp);
 // 6) Ré-effectuer les évennements qui ont étés invalidés
 // XXX A FAIRE XXX
 Liberer_acces();
}

//______________________________________________________________________________
alx_noeud_scene* alx_interface_sdl_opengl_1::Creer_noeud( const alx_chaine_char &T
                                                        , unsigned int pos
                                                        , const alx_chaine_char &nom
                                                        , const alx_chaine_char &cc_maj
                                                        , unsigned int &pos_finale)
{// Isoler le type du noeuds
 unsigned int pos_deb = ++pos;
 alx_noeud_scene *rep = (alx_noeud_scene*)NULL;
 pos = T.Position_du_prochain(' ', pos);
 cc_type_tmp.Maj(T.Texte(), pos_deb, pos-pos_deb); // Le type du noeud.

if(cc_type_tmp == "alx_noeud_scene")
 {rep = new alx_noeud_scene();
  rep->Deserialiser_type(cc_maj, pos_finale);
    rep->Est_une_replique(true);
    rep->Nom( nom );
  return rep;}
 if(cc_type_tmp == "polygon")
  {rep = new alx_noeud_polygone_sdl_opengl();
   rep->Deserialiser_type(cc_maj, pos_finale);
     rep->Est_une_replique(true);
     rep->Nom( nom );
   return rep;}
 if(cc_type_tmp == "fiche_r")
  {alx_noeud_fiche_sdl_opengl *f;
   f = new alx_noeud_fiche_sdl_opengl("",10,10,this);
   f->Deserialiser_type(cc_maj, pos_finale);
     f->Est_une_replique(true);
     f->Nom( nom );
     f->Titre( f->Titre() );
     f->Pre_rendu_exeptionnel(true);
   return (alx_noeud_scene*)f;}
 if(cc_type_tmp == "alx_noeud_fenetre_sdl_opengl")
  {alx_noeud_fiche_ovale_sdl_opengl *f;
   f = new alx_noeud_fiche_ovale_sdl_opengl("",10,10,this);
   f->Deserialiser_type(cc_maj, pos_finale);
     f->Est_une_replique(true);
     f->Nom( nom );
     f->Titre( f->Titre() );
     f->Origine( f->Origine() );
     f->Pre_rendu_exeptionnel(true);
   return (alx_noeud_scene*)f;}
 if(cc_type_tmp == "zone_txt_bigre")
  {alx_noeud_zone_texte_sdl_opengl *zt = new alx_noeud_zone_texte_sdl_opengl( 10, 10
                                                                            , 20
                                                                            , fonte_Arial()
                                                                            , Sim_dispo_saisi() );
   zt->Deserialiser_type(cc_maj, pos_finale);
     zt->Est_une_replique(true);
     zt->Nom( nom );
   return (alx_noeud_scene*)zt;}
 if(cc_type_tmp == "alx_noeud_image_sdl_opengl")
  {alx_noeud_image_sdl_opengl *img = new alx_noeud_image_sdl_opengl();
   img->Deserialiser_type(cc_maj, pos_finale);
   img->alx_image_opengl::maj( img->Lg(), img->Ht()
                             , img->Ordonnancement_couleurs()
                             , img->Nb_octets_par_pixel()
                             , NULL);
   img->Est_une_replique(true);
   img->Nom( nom );
   return (alx_noeud_scene*)img;}

 printf("TYPE DE NOEUD INCONNU : %s", cc_type_tmp.Texte());
 return rep;
}

//______________________________________________________________________________
void alx_interface_sdl_opengl_1::Etablir_liens_entre_noeuds(alx_groupe_bigre &gb, const alx_liste<alx_chaine_char> &L_noms)
{alx_noeud_scene *noeud, *n_fils;
 alx_element_liste<alx_chaine_char> *it     = L_noms.Premier()
                                  , *it_fin = L_noms.Fin()
                                  , *it_n, *it_n_fin;
 for(; it!=it_fin; it=it->svt)
  {noeud = gb.Adresse_noeud( it->E() );
   if(noeud == NULL) continue;
   noeud->Vider_fils();
   it_n     = noeud->L_fils_seri.Premier();
   it_n_fin = noeud->L_fils_seri.Fin();
   for(;it_n!=it_n_fin; it_n=it_n->svt)
    {n_fils = gb.Adresse_noeud( it_n->E() );
     if(n_fils == NULL) continue;
     noeud->Ajouter_fils( n_fils );}
  }
}

//_____________________________________________________________________________
void alx_interface_sdl_opengl_1::Analyser_maj( const unsigned int g
                                             , const alx_chaine_char &cc_maj
                                             , const alx_chaine_char &cc_h
                                             , const alx_chaine_char &cc_racine
                                             , const alx_chaine_char &cc_ip_dist
                                             , const alx_chaine_char &cc_ptr)
{//printf("ANALYSE MAJ de %s avec H=%s\n", cc_ip_dist.Texte(), cc_h.Texte());
 unsigned int pos = 0, pos_deb, pos_deb_interacteur;
 alx_noeud_scene *noeud, *racine_locale, *racine_distante;
 const char *texte = cc_maj.Texte();
 alx_liste<alx_chaine_char> L_noeuds; // La liste des noeuds qui sont à modifier, utiles pour établir ou modifier ensuite les liens effectifs

// Mise à jour de la connaissance horloge
 unsigned int pos_deserialisation_horloge = 0;

 horloge_tmp->Deserialiser(cc_h, pos_deserialisation_horloge);
 Tgb[g].Horloge().Prendre_en_compte( *horloge_tmp );

// Parcours de la sérialisation
 while( (texte[pos]   == '<')
      &&(texte[pos+1] != '/') )
  {// On a une balise indiquant un interacteur, il faut déja trouver le nom
   pos_deb_interacteur = pos;
   pos_deb = cc_maj.Position_du_prochain('=', pos); pos_deb++;
   pos = cc_maj.Position_du_prochain(';', pos_deb);
   cc_nom_tmp.Maj(texte, pos_deb, pos-pos_deb);
   if(noeud = Tgb[g].Adresse_noeud(cc_nom_tmp)) // On compare le résultat de l'affectation à 0.
    {// On a touvé le noeud, on le met à jour
     pos = pos_deb_interacteur;
     noeud->Deserialiser_type(cc_maj, pos);
    }
    else
     {// On le l'a pas trouvé, on va le créer !
      pos = pos_deb_interacteur;
      noeud = Creer_noeud(cc_maj, pos, cc_nom_tmp, cc_maj, pos); // et faire la désérialisation et les ajustements
      if(noeud)
       {Tgb[g].Enregistrer_noeud( noeud );
        L_rap_creation_noeud_replique.Rappeler( noeud );
       } else {break;}
     }
  // Gestion des noeuds à problème pour le pré-rendu
   if( !noeud->Ne_pas_pre_rendre() )
    {// Déja présent?
     unsigned int i=0;
     while( (i<nb_noeud_a_prerendre)
          &&(Tab_noeud_a_prerendre[i] != noeud) ) i++;
     if(i == nb_noeud_a_prerendre) // Pas encore présent
      {Tab_noeud_a_prerendre[nb_noeud_a_prerendre] = noeud;
       nb_noeud_a_prerendre++;}
    }
   noeud->A_changer(true);
   L_noeuds.Ajouter_au_debut(cc_nom_tmp);
  }
// On établit les liens entre les noeuds!!!!
 //printf("  Lier noeuds\n");
 Etablir_liens_entre_noeuds(Tgb[g], L_noeuds);

// Gestion des pointeurs répliqués
 // On parcours les pointeurs sérialiser, pour chacun, on le cré en locale s'il n'éxiste pas
 // On crée ensuite un ou deux alx_evennement_pointeur, suivant que le pointeur à seulement bougé ou bien à aussi été cliqué.
 double ptr_x, ptr_y;
 int    appuie, id;
 unsigned int pos_txt = 0, dep = 1; // On passe la (
 const char *txt_ptr = cc_ptr.Texte();
 alx_pointeur *ptr;
 // cc_nom_ptr_tmp
  // Calcul e la liste de repère menant de la racine du groupe à celle de la scène
   //L_repere_groupe.Vider();
   //Tgb[g].Racine()->Donner_liste_repere(L_repere_groupe, Noeud_scene());
 //printf("  Gérer pointeurs\n");
 if(cc_ptr.Taille() != 0)
 {//printf("  cc_ptr: %s", cc_ptr.Texte());
  pos_txt++; // (
  while(txt_ptr[pos_txt] != ')')
  {dep = pos_txt;
   // DEBUG OLD pos_txt = cc_ptr.Position_du_prochain('_', pos_txt); pos_txt++; // le _
   // DEBUG OLD cc_nom_ptr_tmp.Maj(txt_ptr, dep, pos_txt-dep);
   id = cc_ptr.Lire_entier(pos_txt); pos_txt++; // le _
   cc_nom_ptr_tmp  = id;
   cc_nom_ptr_tmp.Ajouter("_",0,1);
   cc_nom_ptr_tmp += Tgb[g].Numero();//g;
   cc_nom_ptr_tmp.Ajouter("_",0,1);
   cc_nom_ptr_tmp += cc_ip_dist;
   ptr_x  = cc_ptr.Lire_double(pos_txt); pos_txt++; // le _
   ptr_y  = cc_ptr.Lire_double(pos_txt); pos_txt++; // le _
   appuie = cc_ptr.Lire_entier(pos_txt); pos_txt++; // la ,

    // Changement de repère...  INUTILE!!!!
    pt_tmp_ptr_groupe.maj(ptr_x, ptr_y);
    //Repere_transformation_inverse(pt_tmp_ptr_groupe, L_repere_groupe);
    ptr_x = pt_tmp_ptr_groupe.x;
    ptr_y = pt_tmp_ptr_groupe.y;
   if( (ptr = sim_pointeurs->Pointeur(cc_nom_ptr_tmp)) == NULL ) // recherche du pointeur
    {// On n'a pas trouvé le pointeur, il faut le créer
     id = sim_pointeurs->Ajouter_pointeur(cc_nom_ptr_tmp.Texte(), Tgb[g].Racine());
     ptr = sim_pointeurs->Pointeur(id);
       ptr->Est_une_replique( true );
       ptr->Nom(cc_nom_ptr_tmp);
       ptr->pt.maj(ptr_x, ptr_y);
        ptr->appuie = 0;
     if(Tgb[g].Visualiser_pointeurs())
       ptr->Noeud_representation( new alx_noeud_pointeur(ptr) );
      else ptr->Noeud_representation( new alx_noeud_scene() );
     Tgb[g].Enregistrer_noeud_pointeur( ptr->Noeud_representation() );
     ptr->Noeud_representation()->Nom_IU("Replicated pointer");
     ptr->Noeud_representation()->Est_une_replique(true);
     // On lui associe une apparence:
     //sim_aff_pointeurs->Associer_fonction_affichage_a_pointeur(Pointeur_classique, NULL, id);
     }
    // Il faut maintenant créer l'évennement :
     if( (ptr_x != ptr->pt.x)
       ||(ptr_y != ptr->pt.y) )
      {info_comp_tmp.coord.x = ptr_x;
       info_comp_tmp.coord.y = ptr_y;
       info_comp_tmp.dep.x = ptr_x - ptr->pt.x;
       info_comp_tmp.dep.y = ptr_y - ptr->pt.y;
       ptr->pt.maj(ptr_x, ptr_y);
       evt_ptr_tmp.maj( ptr->Id()
                      , alx_pointeur_mouvement
                      , info_comp_tmp
                      , Tgb[g].Racine() );
       evt_ptr_tmp.ptr = ptr;
       sim_pointeurs->Simuler(evt_ptr_tmp);
       sim_contact->simuler();
       ptr->Rappel_changement();
       //sim_couche->Zone_pointee(Tgb[g].Racine(), &evt_ptr_tmp);
      }
     if(appuie != ptr->appuie)
      {alx_pointeur_type_evennement t_evt;    // Son type.
       if(appuie < ptr->appuie) // On vient de relacher
         t_evt = alx_pointeur_relache;
        else t_evt = alx_pointeur_enfonce;
       ptr->appuie = appuie;
       info_comp_tmp.coord.x = ptr_x;
       info_comp_tmp.coord.y = ptr_y;
       evt_ptr_tmp.maj( ptr->Id()
                      , t_evt
                      , info_comp_tmp
                      , Tgb[g].Racine() );
       evt_ptr_tmp.ptr = ptr;
       sim_pointeurs->Simuler(evt_ptr_tmp);
       sim_contact->simuler();
       ptr->Rappel_changement();
       //sim_couche->Zone_pointee(Tgb[g].Racine(), &evt_ptr_tmp);
       if(appuie == -1)
        {sim_pointeurs->Retirer_pointeur( id );
         sim_pointeurs->Simuler();
        }
      }
  }
 }

// Avant de faire le calcul de ce qu'il y a à changer, on ôte les noeuds des pointeurs distants du groupe
 const alx_liste<alx_noeud_scene *> &L_ptr_gp = Tgb[g].Liste_des_pointeurs_distants();
 alx_noeud_scene *noeud_ptr_gp
               , *noeud_racine_gp = Tgb[g].Racine();
 alx_element_liste< alx_noeud_scene* > *it = L_ptr_gp.Init_parcours();
 while(L_ptr_gp.Parcours_continue(it))
  {noeud_ptr_gp = L_ptr_gp.Courant_dans_parcours(it);
     noeud_racine_gp->Retirer_fils(noeud_ptr_gp);
   it = L_ptr_gp.Suivant_dans_parcours( it );
  }
 L_ptr_gp.Terminer_parcours( it );

 //printf("  Fin interprétation message\n  Insertion des noeuds\n");
// Ok maintenant il faut combiner la racine locale avec la racine distante...c'est là que les choses se compliquent
 racine_locale   = Tgb[g].Racine();
 racine_distante = Tgb[g].Adresse_noeud(cc_racine);  // On va la laisser pendouiller dans le vent...
 if( (racine_locale   == NULL)
   ||(racine_distante == NULL)) return;

 if(!racine_distante->Fils_a_subi_deseri_diff())
   return; // On n'a pas de changement dans l'ordre des fils à faire en local car aucun n'est survenu à distance.

 // Le traitement est différent suivant l'ordre des IPs, priorité à la plus grande IP.
 bool appliquer_changement_L_fils = false;
 if(horloge_tmp->Horloge( Tgb[g].Horloge().Proprietaire() ) < der_chgmnt_L_fils)
  {printf("Outdated message Proprio(%i) : %i < %i\n", Tgb[g].Horloge().Proprietaire()
                                                     , horloge_tmp->Horloge( Tgb[g].Horloge().Proprietaire() )
                                                     , der_chgmnt_L_fils );
   if(cc_IP < cc_ip_dist)
    {// On laisse tomber, un message plus récent provenant d'ici le mettra à jour.
    } else {// Appliquer le changement mais attention à bien rajouter les fils orphelins !!!
            appliquer_changement_L_fils = true;
           }
  } else {appliquer_changement_L_fils = true;
         }

 if(appliquer_changement_L_fils)
  {/*printf("Changements à faire dans la liste des fils de la racine locale:\n");
     printf("En local: ("); racine_locale->Init_parcours_fils();
                            while(!racine_locale->Est_parcours_fils_fini())
                             {printf("%s; ", racine_locale->Courant_dans_parcours_fils()->Nom());
                              racine_locale->Suivant_dans_parcours_fils();}
                            printf(")\n");
     printf("En dist : ("); racine_distante->Init_parcours_fils();
                            while(!racine_distante->Est_parcours_fils_fini())
                             {printf("%s; ", racine_distante->Courant_dans_parcours_fils()->Nom());
                              racine_distante->Suivant_dans_parcours_fils();}
                            printf(")\n");*/

   alx_liste<alx_arbre<alx_noeud_scene*>*> L_fils_tmp( racine_locale->L_fils );
   racine_locale->L_fils = racine_distante->L_fils;
   alx_noeud_scene *noeud_fils_loc_tmp;
   alx_element_liste<alx_arbre<alx_noeud_scene*>*> *it_l_tmp     = L_fils_tmp.Premier()
                                                 , *it_l_fin_tmp = L_fils_tmp.Fin();
   for(; it_l_tmp!=it_l_fin_tmp; it_l_tmp=it_l_tmp->svt)
    {noeud_fils_loc_tmp = it_l_tmp->E()->e;
     if(!racine_distante->L_fils.Contient( noeud_fils_loc_tmp )) {
       if(cc_IP < cc_ip_dist)
         racine_locale->L_fils.Ajouter_au_debut( noeud_fils_loc_tmp );
        else  racine_locale->L_fils.Ajouter_a_la_fin( noeud_fils_loc_tmp );
      }
    }
  }
/* if(cc_IP < cc_ip_dist)
  {// Interclassement des fils. Ordre total arbitraire basé sur ordre des fils pour classement alphabétique des IP
   // Double parcours des listes.
   it_l=racine_locale->L_fils.Premier();
   noeud_fils_loc = it_l->E()->e;
   for(; it_l != it_l_fin; it_l = it_l->svt)
    {// tant que le noeud distant est une réplique et qu'il est différent du noeud courant local, on l'insère à la liste locale au début
     noeud_fils_dist = it_d->E()->e;
     while( (it_d != it_d_fin)
          &&(!noeud_fils_dist->Est_une_replique()) ) {it_d = it_d->svt;
                                                      noeud_fils_dist = it_d->E()->e;}
     if(it_d == it_d_fin) break;
     if(noeud_fils_dist == noeud_fils_loc) {it_d = it_d->svt;}
      else {racine_locale->L_fils.Ajouter_avant(noeud_fils_dist, it_l);
           // Enlever une éventuelle présence de noeud_fils_dist dans la suite de la liste
            it_l_tmp = it_l->svt;
            while(it_l_tmp != it_l_fin)
             {it_l_svt_tmp = it_l_tmp->svt;
                noeud_fils_loc_tmp = it_l_tmp->E()->e;
                if(noeud_fils_loc_tmp == noeud_fils_dist) {racine_locale->L_fils.Retirer(it_l_tmp);}
              it_l_tmp = it_l_svt_tmp;
             }
           }
    }
  } else {for(;it_d!=it_d_fin;it_d=it_d->svt)
           {it_l = racine_locale->L_fils.Premier();
            while( (it_l != it_l_fin)
                 &&(it_l->E()->e != it_d->E()->e) ) it_l = it_l->svt;
            if(it_l != it_l_fin) // On l'a trouvé, on le sort
             {racine_locale->L_fils.Retirer( it_l );}
           }
          // A la fin on ajoute les fils distants
          racine_locale->L_fils.Ajouter_au_debut( racine_distante->L_fils );
         }
*/
 //printf("  Fin insertion\nFIN MAJ\n");
}

//______________________________________________________________________________
void alx_interface_sdl_opengl_1::Lister_groupes(unsigned int argc, char **argv)
{// Ré-emettre tous les groupes connus:
 Demande_acces();
 //printf("Lister groupe\n");
 message_emis = entete_message;
 message_emis.Ajouter("INFO GROUPES G=",0,15);
 unsigned int t_dep = message_emis.Taille();
 for(unsigned int i=0; i<nb_gb; i++)
  {message_emis.Fixer_taille(t_dep);
   message_emis += Tgb[i].Numero();
   route66->sendMessage( message_emis.Texte() );}
 Liberer_acces();
}

//______________________________________________________________________________
void alx_interface_sdl_opengl_1::Maj_horloge(unsigned int argc, char **argv) // ^BIGre IP=(.*); INFO GROUPE ([0-9]*) HORLOGE_IP=(.*)$
{Demande_acces();
 //printf("MAJ horloge\n");
 unsigned int g, pos_g=0;
 sscanf(argv[1], "%i", &g); // le numéro de groupe

 // Fait on parti du même groupe?
 while( (pos_g < nb_gb)
      &&(Tgb[pos_g].Numero() != g) ) pos_g++;
 if(pos_g == nb_gb) {Liberer_acces(); return;} // On ne fait pas parti du même groupe.

 // On fait partit du même groupe, je l'ajoute à ma connaissance du groupe
 A_cc_tmp = argv[0];
 if( Tgb[pos_g].horloge.Connaissance(A_cc_tmp, argv[2]) ) // On a du rajouter cet inconnu, il faut ré-emettre notre structure d'horloge!
   Emettre_horloge(Tgb[pos_g].Numero(), Tgb[pos_g].horloge.Serialiser_IPs().Texte() );
 Liberer_acces();
}

//______________________________________________________________________________
//____________________________Actions sur Route 66______________________________
//______________________________________________________________________________
void alx_interface_sdl_opengl_1::Emettre_horloge(const unsigned int g, const alx_chaine_char &cc_h)
{//BIGre IP=(.*); INFO GROUPE ([0-9]*) HORLOGE_IP=(.*)
 message_emis = entete_message;
 message_emis.Ajouter("INFO GROUPE ",0,12);
 message_emis += g;
 message_emis.Ajouter(" HORLOGE_IP=",0,12);
 message_emis += cc_h;
 route66->sendMessage( message_emis.Texte() );
}

//______________________________________________________________________________
void alx_interface_sdl_opengl_1::Se_retirer_de(const unsigned int g)
{message_emis = entete_message;
 message_emis.Ajouter("QUITTER GROUPE ", 0, 15);
 message_emis += g;
 route66->sendMessage( message_emis.Texte() );
// On efface notre connaissance du groupe.
 unsigned int pos = 0;
 while( (pos < nb_gb)
      &&(Tgb[pos].Numero() != g) ) pos++;
 if(Tgb[pos].Numero() == g) {return;} //Pas trouvé
 alx_groupe_bigre *nTgb = new alx_groupe_bigre[nb_gb-1];
 for(unsigned int i=0; i<pos; i++)
   nTgb[i] = Tgb[i];
 for(unsigned int i=pos+1; i<nb_gb; i++)
   nTgb[i-1] = Tgb[i];

// Mise à jour des attributs
 if(Tgb)
   delete[] Tgb;
 Tgb = nTgb;
 nb_gb--;
}
//______________________________________________________________________________
void alx_interface_sdl_opengl_1::Se_rajouter_a(const unsigned int g, alx_noeud_scene *racine, const bool emettre_pointeurs = true, const bool visualiser_pointeurs = true)
{
  //printf("Se rajouter à %d\n", g);
  message_emis = entete_message;
  message_emis.Ajouter("JOINDRE GROUPE ", 0, 15);
  message_emis += g;
  route66->sendMessage( message_emis.Texte() );
  Creer_groupe(g, "JOINT", racine, emettre_pointeurs, visualiser_pointeurs);
  //printf("FIN de se rajouter à %d\n", g);
}
//______________________________________________________________________________
void alx_interface_sdl_opengl_1::Lister_groupes()
{message_emis = entete_message;
 message_emis.Ajouter("LISTER GROUPES", 0, 14);
 route66->sendMessage( message_emis.Texte() ); }

//______________________________________________________________________________
void alx_interface_sdl_opengl_1::Info_groupes(const unsigned int g)
{}
//______________________________________________________________________________
void alx_interface_sdl_opengl_1::Infos_noeuds_groupe(const unsigned int g)
{}

//______________________________________________________________________________
//____________________________ Création de groupe ______________________________
//______________________________________________________________________________
void alx_interface_sdl_opengl_1::Creer_groupe(const unsigned int nb, const char *nom, alx_noeud_scene *racine, const bool emettre_pointeurs = true, const bool visualiser_pointeurs = true)
{//printf("Créer groupe\n");
 alx_groupe_bigre *nTgb = new alx_groupe_bigre[nb_gb+1];
 for(unsigned int i=0; i< nb_gb; i++)
   nTgb[i] = Tgb[i];
 nTgb[nb_gb].Nom(nom);
 nTgb[nb_gb].Numero(nb);
 nTgb[nb_gb].Racine(racine);
 nTgb[nb_gb].horloge.IP(0, cc_IP);
 nTgb[nb_gb].horloge.Proprietaire(0);
 nTgb[nb_gb].Groupe_coherent(false);
 nTgb[nb_gb].Emettre_pointeurs(emettre_pointeurs);
 nTgb[nb_gb].Visualiser_pointeurs(visualiser_pointeurs);

 //printf("Mise à jour des attributs\n");

 Ne_pas_emettre_changement(true);
 while( Emission_changement_en_cours() );
   if(Tgb)
     delete[] Tgb;
   Tgb = nTgb;
   nb_gb++;

  //printf("Emission du sous graphe\n");
   //unsigned int m = 0;
   L_repere.Vider();
   racine->Donner_liste_repere(L_repere, (alx_noeud_scene*)(A_scene));
   alx_liste<alx_repere2D*> *L_rep = (alx_liste<alx_repere2D*>*)NULL;
   if( !L_repere.Vide() )
     L_rep = &L_repere;

   //printf("  Sérialisation de la racine\n");
   const alx_chaine_char &cc = Serialiser_type(racine, 0, L_rep);
   //printf("  Emission de la racine\n");
   route66->sendMessage( cc.Texte() );
   //printf("  Emission de la racine effectuée\n");
   nTgb[nb_gb-1].Groupe_coherent(true);
 Ne_pas_emettre_changement(false);
 //printf("FIN de créer groupe\n");
}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
alx_interface_sdl_opengl_1::alx_interface_sdl_opengl_1( int ex, int ey, double angle
                                                      , Route66 *r
                                                      //, cogitant::Environment *e
                                                      , alx_interpreteur_tcl  *interp ) : route66(r)
                                                                                        , olfa_comet( "CometInterface_mere"
                                                                                                    //, e
                                                                                                    , interp)
{//FSOUND_Init(44100, 32, FSOUND_INIT_USEDEFAULTMIDISYNTH);
// Initialisation de l'environnement :
 ecran_x = ex;
 ecran_y = ey;
 g_ecran_y = ey;
 angle_camera = angle;

 A_scene = new alx_noeud_scene(false);
 prerendeur = new alx_prerendeur(A_scene, ex, ey, angle_camera);
 prerendeur_defaut = prerendeur;

// Initialisation des groupes auxquels appartient cette interface :
 nb_noeud_a_prerendre = 0;
 nb_gb = 0;
 compteur = 0;
 Tgb = (alx_groupe_bigre*)NULL;
 cc_IP = IP_locale();
 /*cc_IP = slimGetLocalIp();
   cc_IP += ".";
   cc_IP += (unsigned int)(this);*/
 entete_message.Maj("BIGre IP=",0,9);
 entete_message += cc_IP;
 entete_message.Ajouter("; ",0,2);
 taille_entete = entete_message.Taille();
 Ne_pas_emettre_changement(false);
 Emission_changement_en_cours(false);
 Ne_pas_traiter_messages(false);
 Traitement_messages_en_cours(false);

// Dernier changement survenu dans l'ordre des fils de la racine du groupe.
 der_chgmnt_L_fils = 0;
 horloge_tmp = new alx_horloge_vectorielle();

// Les mutex
 mutex_R66           = new Mutex();
 mutex_deformations  = new Mutex();

// M_EmissionReception = new Mutex();

// La gestion des autres ordinateurs :
 route66->bindMessageCallback( "^BIGre IP=(.*); INFO GROUPE ([0-9]*) HORLOGE_IP=(.*)$"
                      , new Route66MessageCallbackOf<alx_interface_sdl_opengl_1>(this, &alx_interface_sdl_opengl_1::Maj_horloge) );
 route66->bindMessageCallback( "^BIGre IP=(.*); LISTER GROUPES$"
                      , new Route66MessageCallbackOf<alx_interface_sdl_opengl_1>(this, &alx_interface_sdl_opengl_1::Lister_groupes) );
 route66->bindMessageCallback( "^BIGre IP=(.*); JOINDRE GROUPE ([0-9]*)$"
                      , new Route66MessageCallbackOf<alx_interface_sdl_opengl_1>(this, &alx_interface_sdl_opengl_1::Ajouter_membre) );
 route66->bindMessageCallback( "^BIGre IP=(.*); QUITTER GROUPE ([0-9]*)$"
                      , new Route66MessageCallbackOf<alx_interface_sdl_opengl_1>(this, &alx_interface_sdl_opengl_1::Retirer_membre) );

 route66->bindMessageCallback( "^<interface SIZE=([0-9]*) IP=(.*) Racine=(.*) G=([0-9]*) P=(.*); H=([0-9|;|(|)]*)>(.*)$"
                      , new Route66MessageCallbackOf<alx_interface_sdl_opengl_1>(this, &alx_interface_sdl_opengl_1::Maj_membre) );

// L'observateur de plate-forme
// ptfo = new PlatformObserver(r);

// On enregistre l'interface mère
 Enregistrer_interface((olfa_comet*)this, this);
 alx_chaine_char commande;
 commande  = "set i_mere "; commande += this->Nom_usage(); Interpreteur()->Evaluer( commande );


// Initialisation des choses communes et des états puits:
 Initialisation_etats_puits();
 Initialiser_matos();
 Initialiser_boutons();

 Enregistrer_image_opengl(Interpreteur(), Image_bouton_dec() , "imgl_bt_dec");
 Enregistrer_image_opengl(Interpreteur(), Image_bouton_inc() , "imgl_bt_inc");
 Enregistrer_image_opengl(Interpreteur(), Image_bouton_rect(), "imgl_bt_rec");



 coin_bg.maj(0.0, 0.0);
 vect_h.maj((double)ecran_x, 0.0);
 vect_v.maj(0.0, (double)ecran_y);
 trans.maj(0.0, 0.0);
 rotation = 0.0;

// Les dispositifs de saisie
 alx_pointeur *ptr;

 sim_dispo_saisis  = new alx_simulateur_dispositif_saisie();
 sim_clavier       = new alx_simulateur_clavier();
 sim_clavier->Ajouter_rapporteur(sim_dispo_saisis);

// La souris
 sim_pointeurs  = new alx_simulateur_pointeurs();
 sim_ptr_souris = new alx_simulateur_pointeurs_souris( &ecran_x, &ecran_y
                                                     , &coin_bg
                                                     , &vect_h, &vect_v);
 id_souris = sim_ptr_souris->Ajouter_rapporteur(sim_pointeurs, Noeud_scene());
 sim_ptr_souris->Noeud_pere( Noeud_scene() );
 ptr = sim_pointeurs->Pointeur(id_souris);
 ptr->Noeud_representation( new alx_noeud_pointeur(ptr) );
 ptr->Noeud_representation()->Nom_IU("Mouse\npointer");

// Souris multiples...
/* sim_ptr_USB_raw = new alx_simulateur_pointeur_souris_USB_raw( &ecran_x, &ecran_y
                                                             , &coin_bg
                                                             , &vect_h, &vect_v);
 int id_multimouse = sim_ptr_USB_raw->Ajouter_rapporteur(sim_pointeurs, Noeud_scene());
*/
// Les joysticks
 //SDL_JoystickEventState(SDL_ENABLE);
 nb_joysticks = SDL_NumJoysticks();
  sdl_joy = new SDL_Joystick*[nb_joysticks];
   for(int i=0;i<nb_joysticks;i++)
     sdl_joy[i] = SDL_JoystickOpen(i);

   sim_joystick = new alx_simulateur_pointeurs_joystick( &ecran_x, &ecran_y
                                                       , &coin_bg
                                                       , &vect_h, &vect_v
                                                       , nb_joysticks);
 if(nb_joysticks)
  {sim_joystick->Noeud_pere( Noeud_scene() );
   id_joysticks = sim_joystick->Ajouter_rapporteur(sim_pointeurs, Noeud_scene());
   ptr = sim_pointeurs->Pointeur(id_joysticks);
   ptr->Noeud_representation( new alx_noeud_pointeur(ptr) );
   for(int i=0;i<nb_joysticks-1;i++)
    {sim_pointeurs->Ajouter_pointeur("", Noeud_scene());
     ptr = sim_pointeurs->Pointeur(id_joysticks);
     ptr->Noeud_representation( new alx_noeud_pointeur(ptr) );}
  }

// On s'occupe de l'affichage des pointeurs:
 sim_aff_pointeurs = new affichage_pointeurs( sim_pointeurs );
 //sim_aff_pointeurs->Associer_fonction_affichage_a_pointeur(Pointeur_classique, NULL, id_souris);
 //for(int i=0;i<nb_joysticks;i++)
 //  sim_aff_pointeurs->Associer_fonction_affichage_a_pointeur(Pointeur_classique, NULL, id_joysticks+i);

// Les simulateurs de contact
 sim_contact = new alx_simulateur_points_contact();
 sim_couche  = new alx_simulateur_couche(sim_pointeurs);
 liant_ptr_contact = new alx_liant_ptr_contact(sim_pointeurs, sim_contact);
 lieur_ptr_liant   = new alx_lieur_ptr_liant(sim_pointeurs);
}

//______________________________________________________________________________
alx_interface_sdl_opengl_1::~alx_interface_sdl_opengl_1()
{delete horloge_tmp;
 }

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
alx_noeud_scene* alx_interface_sdl_opengl_1::Adresse_noeud   (const unsigned int groupe, const char *nom)            {
 for(unsigned int i=0; i<nb_gb; i++)
  {if(Tgb[i].Numero() == groupe) {return Tgb[i].Adresse_noeud(nom);}
  }
 return (alx_noeud_scene*)NULL;
}
//______________________________________________________________________________
alx_noeud_scene* alx_interface_sdl_opengl_1::Adresse_noeud_CC(const unsigned int groupe, const alx_chaine_char &nom) {
  for(unsigned int i=0; i<nb_gb; i++)
   {if(Tgb[i].Numero() == groupe) {return Tgb[i].Adresse_noeud(nom);}
   }
  return (alx_noeud_scene*)NULL;
}

//______________________________________________________________________________
//_____________________________ La sérialisation _______________________________
//______________________________________________________________________________
class Parcours_serialisation
{public:
   alx_chaine_char *texte;
   unsigned int marqueur;

   Parcours_serialisation(alx_chaine_char *cc, unsigned int m)
    {texte    = cc;
     marqueur = m;}
};

//______________________________________________________________________________
bool Mise_a_pas_encore_serialise(alx_noeud_scene **e, void *info_comp)
{unsigned int m = *((unsigned int*)info_comp);
 if( !(*e)->Deja_serialise()
   &&((*e)->Marqueur_difference() == m) ) return false; // On est déja passé par ici
 (*e)->Deja_serialise( false );
 //(*e)->A_changer(false);
 (*e)->Marqueur_difference(m);
 return true;}

//______________________________________________________________________________
//alx_chaine_char cc_tmp;
bool Tout_serialiser(alx_noeud_scene **e, void *info_comp)
{Parcours_serialisation *ps = (Parcours_serialisation*)info_comp;
 alx_chaine_char *txt = ps->texte;
 alx_noeud_scene *noeud = *e;

 if( noeud->Deja_serialise()
   &&noeud->Marqueur_difference() == ps->marqueur) return false; // On est déja passé par ici
 noeud->Deja_serialise( true );
 noeud->Marqueur_difference( ps->marqueur );
 if( !noeud->Est_une_replique() )
  {const alx_chaine_char &cc_tmp = noeud->Serialiser_contenu();
   *txt  += noeud->Serialiser_type( cc_tmp );}

 return true;
}

//______________________________________________________________________________
bool Tout_serialiser_diff(alx_noeud_scene **e, void *info_comp)
{Parcours_serialisation *ps = (Parcours_serialisation*)info_comp;
 if( (*e)->Deja_serialise()
   &&(*e)->Marqueur_difference() == ps->marqueur) return false; // On est déja passé par ici

 alx_noeud_scene *n = *e;
 n->Deja_serialise( true );

 alx_chaine_char *txt = ps->texte;
// tmp   = (*e)->Serialiser_contenu_diff();
 if( !n->Est_une_replique() )
  {if(ps->marqueur > n->Marqueur_difference() ) // On a déja calculé la différence
     *txt += n->Serialisation_differences_type();
    else *txt += n->Serialiser_difference_type();
  }
 n->Marqueur_difference( ps->marqueur ); // Pour ne pas se marcher sur les pieds
 return true;
}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
const alx_chaine_char& alx_interface_sdl_opengl_1::Serialiser_type_diff( alx_noeud_scene *racine
                                                                       , unsigned int marqueur
                                                                       , const alx_liste<alx_repere2D*> *L_rep)
{return Serialiser_type_diff(racine, Serialiser_contenu_diff(racine, marqueur), L_rep );}
//______________________________________________________________________________
const alx_chaine_char& alx_interface_sdl_opengl_1::Serialiser_type_diff( alx_noeud_scene *racine
                                                                       , const alx_chaine_char &contenu
                                                                       , const alx_liste<alx_repere2D*> *L_rep )
{if( (contenu.Taille() == 0)
   &&(nb_info_ptr == 0) )
  {serialisation_type_diff.Maj("",0,0);}
  else {unsigned der_chgmnt = Tgb[groupe_en_cours].horloge.Incrementer();
        if( racine->Fils_a_subi_seri_diff() ) der_chgmnt_L_fils = der_chgmnt;
        serialisation_type_diff.Maj("<interface SIZE=",0,16);
        serialisation_type_diff += contenu.Taille()+12;
        serialisation_type_diff.Ajouter(" IP=", 0, 4); serialisation_type_diff += IP();
        serialisation_type_diff.Ajouter(" Racine=",0,8);
        serialisation_type_diff += racine->Nom();

        serialisation_type_diff.Ajouter(" G=",0,3);
        serialisation_type_diff += Tgb[groupe_en_cours].Numero();
        serialisation_type_diff.Ajouter(" P=",0,3);
        if(Tgb[groupe_en_cours].Emettre_pointeurs())
          serialisation_type_diff += sim_pointeurs->Serialiser_diff( L_rep );
        serialisation_type_diff.Ajouter(";",0,1);
        serialisation_type_diff.Ajouter(" H=",0,3);
        serialisation_type_diff += Tgb[groupe_en_cours].horloge.Serialiser();

        serialisation_type_diff.Ajouter(">",0,1);
        serialisation_type_diff += contenu;
        serialisation_type_diff.Ajouter("</interface>",0,12);}
 return serialisation_type_diff;}
//______________________________________________________________________________
const alx_chaine_char& alx_interface_sdl_opengl_1::Serialiser_contenu_diff( alx_noeud_scene *racine
                                                                          , unsigned int marqueur)
{unsigned int m = -1;
 racine->Parcourir_et_traiter(Mise_a_pas_encore_serialise, &m);
 serialisation_contenu_diff.Maj("",0,0);
   Parcours_serialisation ps(&serialisation_contenu_diff, marqueur);
   racine->Parcourir_et_traiter(Tout_serialiser_diff, &ps);
// serialisation_contenu+= ")";
 return serialisation_contenu_diff;}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
const alx_chaine_char& alx_interface_sdl_opengl_1::Serialiser_type( alx_noeud_scene *racine
                                                                  , unsigned int marqueur
                                                                  , const alx_liste<alx_repere2D*> *L_rep)
{return Serialiser_type(racine, Serialiser_contenu(racine, marqueur), L_rep );}

//______________________________________________________________________________
const alx_chaine_char& alx_interface_sdl_opengl_1::Serialiser_type( alx_noeud_scene *racine
                                                                  , const alx_chaine_char &contenu
                                                                  , const alx_liste<alx_repere2D*> *L_rep )
{serialisation_type.Maj("<interface SIZE=",0,16);
 serialisation_type += contenu.Taille() + 12;
 serialisation_type.Ajouter(" IP=", 0, 4); serialisation_type += IP();
 serialisation_type.Ajouter(" Racine=",0,8);
 serialisation_type += racine->Nom();

 serialisation_type.Ajouter(" G=",0,3);
 serialisation_type += Tgb[groupe_en_cours].Numero();
 serialisation_type.Ajouter(" P=",0,3);
 if(Tgb[groupe_en_cours].Emettre_pointeurs())
   serialisation_type += sim_pointeurs->Serialiser( L_rep );
 serialisation_type.Ajouter(";",0,1);
 serialisation_type.Ajouter(" H=",0,3);
 serialisation_type += Tgb[groupe_en_cours].horloge.Serialiser();

 serialisation_type.Ajouter(">",0,1);
 serialisation_type += contenu;
 serialisation_type.Ajouter("</interface>",0,12);
 return serialisation_type;}

//______________________________________________________________________________
const alx_chaine_char& alx_interface_sdl_opengl_1::Serialiser_contenu(alx_noeud_scene *racine, unsigned int marqueur)
{unsigned int m = -1;
 racine->Parcourir_et_traiter(Mise_a_pas_encore_serialise, &m);
 serialisation_contenu.Maj("",0,0);
   Parcours_serialisation ps(&serialisation_contenu, marqueur);
   racine->Parcourir_et_traiter(Tout_serialiser, &ps);
// serialisation_contenu+= ")";
 return serialisation_contenu;}

//______________________________________________________________________________
const alx_noeud_scene* alx_interface_sdl_opengl_1::Deserialiser(const alx_chaine_char &texte)
{return NULL;}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
void alx_interface_sdl_opengl_1::Emettre_interface(const alx_chaine_char &m)
{route66->sendMessage( m.Texte() );
}

//______________________________________________________________________________
// Une petite structure bien utile
void alx_interface_sdl_opengl_1::Emettre_changements(const bool b)
{const alx_noeud_scene *noeud;
 // Traitement des messages en attentes:
 //DEBUG_2008 Traitement_messages_en_cours(true);
 //DEBUG_2008 if( !Ne_pas_traiter_messages() )
  {//printf("COUCOU\n");
   //if(!L_messages_a_traiter_temp.Vide())
   //  printf("    Emettre_changements: L_messages_a_traiter_temp n'est pas vide\n");
   //printf("LE RETOUR DE COUCOU\n");
   L_messages_a_traiter.Ajouter_a_la_fin( L_messages_a_traiter_temp );
   L_messages_a_traiter_temp.Vider();
   //Traitement_messages_en_cours(true);
   alx_element_liste<message_a_traiter*> *it;
   //DEBUG_2008 if( !Ne_pas_traiter_messages() )
   while( !L_messages_a_traiter.Vide() )
    {//DEBUG_2008 if( Ne_pas_traiter_messages() )
     //DEBUG_2008   break;
     it = L_messages_a_traiter.Premier();
     message_a_traiter *m_a_t = it->E();
     // Traiter
     Analyser_maj(m_a_t->num_g, m_a_t->cc_maj, m_a_t->cc_h, m_a_t->cc_racine, m_a_t->cc_ip, m_a_t->cc_ptr);
     // Supprimer le message
     delete m_a_t;
     // Supprimer l'élement dans la liste
     L_messages_a_traiter.Retirer_premier();
    }
  }
 //DEBUG_2008 Traitement_messages_en_cours(false);

 if( Ne_pas_emettre_changement() )
  {printf("Ne_pas_emettre_changement() !\n");
   return;}
 //DEBUG_2008 Emission_changement_en_cours(true);
 // Emissions des messages de changements après calcul:
 const alx_chaine_char *message;
 for(groupe_en_cours=0; groupe_en_cours<nb_gb; groupe_en_cours++)
  {if( !Tgb[groupe_en_cours].Groupe_coherent() )
    {printf("Groupe %i incohérent\n", groupe_en_cours);
     continue; // On n'émet pas les changements des groupes incohérents, c'est notemment le cas lors de leurs création.
    }
   if( Tgb[groupe_en_cours].Emettre_IP_horloge() )                                              // Ré-émettre l'horloge?
     Emettre_horloge(Tgb[groupe_en_cours].Numero(), Tgb[groupe_en_cours].horloge.Serialiser_IPs().Texte() );

   L_repere.Vider();
     Tgb[groupe_en_cours].Racine()->Donner_liste_repere(L_repere, (alx_noeud_scene*)(A_scene));
   alx_liste<alx_repere2D*> *L_rep = (alx_liste<alx_repere2D*>*)NULL;
   if( !L_repere.Vide() )
     L_rep = &L_repere;

  // Avant de faire le calcul de ce qu'il y a à ré-émettre on prend soin d'ôter les pointeurs répliqués
   const alx_liste<alx_noeud_scene *> &L_ptr_gp = Tgb[groupe_en_cours].Liste_des_pointeurs_distants();
   alx_noeud_scene *noeud_ptr_gp
                 , *noeud_racine_gp = Tgb[groupe_en_cours].Racine();
   alx_element_liste<alx_noeud_scene*> *it = L_ptr_gp.Init_parcours();
   while(L_ptr_gp.Parcours_continue(it))
    {noeud_ptr_gp = L_ptr_gp.Courant_dans_parcours(it);
       noeud_racine_gp->Retirer_fils(noeud_ptr_gp);
     it = L_ptr_gp.Suivant_dans_parcours( it );
    }
   L_ptr_gp.Terminer_parcours( it );

  // Calcul de la description de l'interface à ré-émettre
   if( Tgb[groupe_en_cours].Tout_reemettre() )                                                  // Ré-émettre toute l'interface?
     message = &( Serialiser_type(Tgb[groupe_en_cours].Racine(), groupe_en_cours, L_rep) );                // Oui!
    else message = &( Serialiser_type_diff(Tgb[groupe_en_cours].Racine(), groupe_en_cours, L_rep) );       // Non! Simplement les différences survenues
   // Emission de l'interface s'il y a lieu de le faire.
   if( b && message->Taille() )
     Emettre_interface( message->Texte() ); //serialisation_type_diff.Texte() );
//    else {printf(".");}
   Tgb[groupe_en_cours].Emettre_IP_horloge( false );
   Tgb[groupe_en_cours].Tout_reemettre    ( false );
  // Enregistrement des noeuds fils de la racine locale s'il y a lieu
  // cela dans le but de les reconnaitre quand d'autres ordinateur y font référence par leurs noms.
   const alx_noeud_scene *racine_groupe = Tgb[groupe_en_cours].Racine()
                       , *fils_racine_groupe;
    alx_element_liste<alx_arbre<alx_noeud_scene*>*> *it_fils = racine_groupe->Init_parcours_fils();
   while ( !racine_groupe->Est_parcours_fils_fini(it_fils) ) {
     fils_racine_groupe = racine_groupe->Courant_dans_parcours_fils( it_fils );
     if( Tgb[groupe_en_cours].Adresse_noeud( fils_racine_groupe->Nom() ) == (alx_noeud_scene*)NULL ) {
       Tgb[groupe_en_cours].Enregistrer_noeud(fils_racine_groupe);
       L_rap_creation_noeud_replique.Rappeler( (void*)fils_racine_groupe );
      }
     it_fils = racine_groupe->Suivant_dans_parcours_fils( it_fils );
    }
   racine_groupe->Terminer_parcours_fils( it_fils );
  }
 //DEBUG_2008 Emission_changement_en_cours(false);
}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
bool Mise_a_pas_encore_pre_rendu(alx_noeud_scene **e, void *info_comp)
{unsigned int m = *((unsigned int *)info_comp);
 if( m == (*e)->Marqueur_de_parcours() ) return false; // On est déja passé par ici
 (*e)->Nb_pre_rendu( 0 );
 (*e)->Nb_affichage( 0 );
 (*e)->Marqueur_de_parcours(m);
 if( (*e)->Est_entite() ) {
    (*e)->Prendre_en_compte_repere(false);
   }
 (*e)->A_changer(false);

 return true;}

//______________________________________________________________________________
void alx_interface_sdl_opengl_1::Pre_afficher()
{//A_scene->e->Vider_contexte_global();
 //A_scene->e->Charger_contexte_local();

// Mise_a_pas_encore_pre_rendu(&n, &marqueur);
 A_scene->Avorter( false );
 A_scene->Limite ( NULL  );
 A_scene->pere   = NULL;
 A_scene->Etirement_courant(1,1);

// Gestion des noeuds suspect de ne pas être pré-rendu correctement, ce sont des noeuds issus de la désérialisation.
 for(unsigned int i=0; i<nb_noeud_a_prerendre; i++)
  {if(Tab_noeud_a_prerendre[i]->Ne_pas_pre_rendre())
    {// On en tiens un qui n'aurait pas été pré-rendu si on ne l'avait pas chopé!!!
     Tab_noeud_a_prerendre[i]->Ne_pas_pre_rendre(false);
     Tab_noeud_a_prerendre[i]->PreRendre();
     Tab_noeud_a_prerendre[i]->Ne_pas_pre_rendre(true);
    }
  }
 nb_noeud_a_prerendre = 0;
// Gestion des pointeurs dans le graphe de scène.
 alx_element_liste<alx_pointeur> *it     = sim_pointeurs->L_ptr().Premier()
                               , *it_fin = sim_pointeurs->L_ptr().Fin();
 alx_noeud_scene *pere_ptr, *preso_ptr;
 for(;it!=it_fin;it=it->svt)
  {pere_ptr  = it->E().Noeud_pere();
   preso_ptr = it->E().Noeud_representation();
   if(pere_ptr && preso_ptr)
    {pere_ptr->Retirer_fils         ( preso_ptr );
     pere_ptr->Ajouter_fils_au_debut( preso_ptr );}
  }
// Gestion des noeuds communs
 prerendeur->simuler();
}

//______________________________________________________________________________
void alx_interface_sdl_opengl_1::Afficher()
{// On vide le contexte :
 Tempon(0);
 //A_scene->e->Vider_contexte_global();
 //A_scene->e->Charger_contexte_local();
 //int pointeur_x, pointeur_y;
 unsigned int marqueur = A_scene->GET_a_new_marqueur();
 A_scene->Parcourir_et_traiter(Prise_en_compte_des_reperes_ou_pas, &marqueur);

 camera_pour_ecran(coin_bg, vect_h, vect_v.Norme(), angle_camera, (double)ecran_y/(double)ecran_x);
   A_scene->Avorter(false);
   A_scene->Limite( NULL );
   A_scene->pere = NULL;
   A_scene->e->Etirement_courant(1,1);
   alx_noeud_scene::Noeud_courant(A_scene->e);
   A_scene->e->Afficher();

   //SDL_GetMouseState(&pointeur_x, &pointeur_y);
  // Parcour des pointeurs pour les mettre à jour par rapport
  // A_v_tmp.maj(ecran_x, 0);
  // camera_pour_ecran( Pt_nul(), A_v_tmp/*alx_vecteur2D(ecran_x, 0)*/, ecran_y
  //                  , angle_camera, (double)ecran_y/(double)ecran_x);
  //   sim_aff_pointeurs->Afficher();
  // glPopMatrix();
 glPopMatrix(); // Pour le camera_pour_ecran.

 int nbcecrit;
}

//______________________________________________________________________________
info_du_contenant* alx_interface_sdl_opengl_1::Contient(alx_point2D &pt, int action)
{A_scene->Avorter(false);
 return A_scene->Contient(pt, action);
}

//______________________________________________________________________________
void alx_interface_sdl_opengl_1::Simuler()
{SDL_Event  event;
 //bool truc_fait = false;

 // Marquer les noeuds comme pas encore pré-rendus et inchangés
  unsigned int marqueur = A_scene->GET_a_new_marqueur();
  A_scene->Parcourir_et_traiter(Mise_a_pas_encore_pre_rendu, &marqueur);

// Musique en attente d'être prètes à jouer:
 Demarrer_musique_en_attente();

// Gestion des évennement TCL et TK avant le reste
 Tcl_Time t;
 t.sec  = 0;
 t.usec = 1;
 //Tcl_WaitForEvent(&t);
 unsigned int i=0;
 //printf("SIM");
 while( Tcl_DoOneEvent(TCL_ALL_EVENTS|TCL_DONT_WAIT) /*&& (i<20)*/ ) i++;
 //printf(" %i", i);
//XXX  i++;

// Voyons les simulateurs importés
 L_rap_simulateur_sup.Rappeler();

// Gestion des évennements SDL
 nb_info_ptr = 0;
// SDL_EventState(SDL_ACTIVEEVENT  , SDL_IGNORE);
// SDL_EventState(SDL_APPMOUSEFOCUS, SDL_IGNORE);
// SDL_EventState(SDL_APPINPUTFOCUS, SDL_IGNORE);
// SDL_EventState(SDL_APPACTIVE    , SDL_IGNORE);

 SDL_PumpEvents();
 int nb_SDL_event = 0;
 while( SDL_PollEvent( &event ) )
  {//truc_fait = true;
   nb_SDL_event++;
   A_scene->Avorter(false);
  // On distribue l'évennement
   sim_ptr_souris->event = event;
   sim_joystick->event   = event;
   sim_clavier->event    = event;
  // On simule la réaction pour les pointeurs
   sim_ptr_souris->simuler();
   sim_joystick->simuler();
   //nb_info_ptr += sim_pointeurs->Simuler();
  // On simule la réaction pour les dispositifs d'entrés
   sim_clavier->simuler();
   //sim_dispo_saisis->simuler();
  }
 //printf(" %i...",nb_SDL_event);
// C'est partit pour la simuation des gestionnaire de simulateur!!!
 nb_info_ptr += sim_pointeurs->Simuler();
 sim_dispo_saisis->simuler();
 sim_contact->simuler();

// Voyons les simulateurs importés
 L_rap_post_simulation.Rappeler();

 // On dispose d'une liste de modèles de bases qui peuvent être simulés
 mutex_deformations->lock();
 deformation_dynamique *deformation;
 alx_element_liste<deformation_dynamique*> *it     = L_deformations_dynamiques.Premier()
                                         , *it_fin = L_deformations_dynamiques.Fin()
                                         , *it_tmp;
 const unsigned int ti = nb_milliseconds_ellapsed = (unsigned int)SDL_GetTicks();

 double td;
 while(it!=it_fin)
  {it_tmp = it->svt;
   deformation = it->E();
   if(deformation->Simuler_amorce(ti))
    {td = deformation->Position_temporelle_relative(ti);
     if(td>=1) {deformation->Simuler( 1 );
                L_deformations_dynamiques.Retirer(it);
                delete deformation;}
      else {deformation->Simuler( td );}
    }
   it = it_tmp;
  }
 mutex_deformations->unlock();
}


//______________________________________________________________________________
/******************** modifications de l'interface ****************************/
void alx_interface_sdl_opengl_1::Translation(const alx_vecteur2D &t)
{A_scene->Translation(t);}

void alx_interface_sdl_opengl_1::Rotation(const double r)
{A_scene->Rotation(r);}

//______________________________________________________________________________
alx_noeud_scene * alx_interface_sdl_opengl_1::Trouver_noeud_nomme(const alx_chaine_char &cc_nom_demande ) const
{return Trouver_noeud_nomme(cc_nom_demande, Noeud_scene());
}

//______________________________________________________________________________
alx_noeud_scene * alx_interface_sdl_opengl_1::Trouver_noeud_nomme(const alx_chaine_char &cc_nom_demande, alx_noeud_scene *racine ) const
{if(racine->Nom() == cc_nom_demande) {return racine;}
 if(racine->Val_MetaData("Trouver_noeud_nomme")) {return NULL;}

 racine->Ajouter_MetaData_T("Trouver_noeud_nomme", "1");
   alx_noeud_scene *noeud_courant, *rep = NULL;
   alx_element_liste<alx_arbre<alx_noeud_scene*>*> *it     = racine->L_fils.Premier()
                                                 , *it_fin = racine->L_fils.Fin();
   for(; it!=it_fin; it=it->svt)
    {noeud_courant = (it->E())->e;                     // on définit le noeud fils
     rep = Trouver_noeud_nomme(cc_nom_demande, noeud_courant);
     if(rep) {break;}
    }
 racine->Retirer_MetaData_T("Trouver_noeud_nomme");
 return rep;
}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
//__________________________________ Musique !!! _______________________________
//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
const bool alx_interface_sdl_opengl_1::Fermer_flux(FSOUND_STREAM *flux)
{return FSOUND_Stream_Close(flux);
}

//______________________________________________________________________________
FSOUND_STREAM* alx_interface_sdl_opengl_1::Nouveau_flux( FSOUND_STREAMCALLBACK callback, int canal
                    , int lenbytes
                    , unsigned int mode
                    , int samplerate
                    , void *userdata )
{
 FSOUND_STREAM *audio_strm = FSOUND_Stream_Create( callback
                     , lenbytes, FSOUND_STREAMABLE | FSOUND_NONBLOCKING | mode
                     , samplerate
                     , userdata );
 L_musique_a_demarrer.Ajouter_a_la_fin( zic(audio_strm, canal) );
 return audio_strm;
}


/*
int Send_data_on_chan( ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
}

void Transfert_donnees_TCP::init()
{}

//______________________________________________________________________________
Transfert_donnees_TCP::Transfert_donnees_TCP(alx_interface_sdl_opengl_1 *interface)
{}

Transfert_donnees_TCP::Transfert_donnees_TCP(alx_interface_sdl_opengl_1 *interface)
{i_mere = interface;
 init();
}
//______________________________________________________________________________
void Repondre_a_connection (void *params)
{// Convertion du paramètre en SlimTcpConnectionHandler_TCL
 SlimTcpConnectionHandler_TCL *connecteur = (SlimTcpConnectionHandler_TCL*)params;

 // Allouer un buffer pour la réception des données
 char *buffer = connecteur->Allouer_un_Buffer(1024);

 // Réceptionner les données
 int taille_donnees = connecteur->waitingForData(buffer);

 // Anaylser les données
 // XXX TODO

 // Emettre une réponse

 // Libérer le buffer
 connecteur->Liberer_buffer(buffer);
}
*/


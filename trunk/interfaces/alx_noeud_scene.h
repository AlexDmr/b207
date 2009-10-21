#ifndef __ALX_NOEUD_SCENE_H__
#define __ALX_NOEUD_SCENE_H__

//#define __ALX_DEBUG__

class alx_model_simulation_physique1;
class alx_prerendeur;

#include "../physique/alx_repere2D.h"
#include "../utilitaires/alx_arbre.cpp"
#include "../utilitaires/alx_chaine_char.h"
#include "../physique/alx_model_simulation_physique1.h"
#include "../physique/math_alex.cpp"
#include "../opengl/config_opengl.h"
#include "../utilitaires/MetaData.h"

typedef void* P_void;

// Définition des actions liés aux contacts possibles avec les pointeurs
const int ALX_NOEUD_OPERATION_SI_CONTIENT_ALORS_RIEN         = 1;     inline const int Op_Contient_alors_rien()         {return ALX_NOEUD_OPERATION_SI_CONTIENT_ALORS_RIEN;}
const int ALX_NOEUD_OPERATION_SI_CONTIENT_ALORS_PREMIER_PLAN = 2;     inline const int Op_Contient_alors_premier_plan() {return ALX_NOEUD_OPERATION_SI_CONTIENT_ALORS_PREMIER_PLAN;}
const int ALX_NOEUD_OPERATION_SI_CONTIENT_ALORS_ACCUMULER    = 65536; inline const int Op_Contient_alors_accumuler()    {return ALX_NOEUD_OPERATION_SI_CONTIENT_ALORS_ACCUMULER;}


// Définition des masques bianires d'accès aux attributs binaires du noeud :
const unsigned int M_ne_pas_pre_rendre                       = 1;
//const unsigned int M_deja_prerendu                         = 2;
const unsigned int M_position_dans_liste_des_fils_changeable = 4;
const unsigned int M_ne_pas_pre_rendre_fils                  = 8;
const unsigned int M_ne_pas_afficher_fils                    = 16;
const unsigned int M_afficher                                = 32;
const unsigned int M_gerer_contacts                          = 64;
const unsigned int M_toujours_au_premier_plan                = 128;
const unsigned int M_compter_affichage                       = 256;
const unsigned int M_compter_pre_rendu                       = 512;
const unsigned int M_appartenance_deja_testee                = 1024;
//const unsigned int M_rationnalisation_en_cours             = 2048;
const unsigned int M_afficher_boite_noeud                    = 4096;
const unsigned int M_position_des_fils_changeable            = 8192;
const unsigned int M_verouiller                              = 16384;
const unsigned int M_modif_action_transmise                  = 32768;
const unsigned int M_empiler_contenant                               = 0x00010000;
const unsigned int M_forcer_a_ne_plus_empiler_contenant              = 0x00020000;
const unsigned int M_afficher_boites_fils                            = 0x00040000;
const unsigned int M_contenu_dans_fils_ssi_contenu_dans_noeud        = 0x00080000;
const unsigned int M_contenu_dans_fils_ssi_ne_contenu_dans_pas_noeud = 0x00100000;
const unsigned int M_mode_texture                                    = 0x00200000;
const unsigned int M_mode_texture_fils                               = 0x00400000;
const unsigned int M_texture_translucide                             = 0x00800000;
const unsigned int M_lisser_texture                                  = 0x01000000;
const unsigned int M_Boite_noeud_pour_texture                        = 0x02000000;
const unsigned int M_tout                                            = 0xFFFFFFFF;

class alx_prerendeur;
class alx_noeud_scene;
class alx_evennement_pointeur;
class Liste_de_rappel;
class alx_pointeur;

class info_du_contenant
{public:
   alx_model_simulation_physique1 *mp;
   alx_noeud_scene *noeud;
   alx_liste<Liste_de_rappel*> *L_rappel;
   alx_evennement_pointeur *evt_ptr;
   alx_liste<alx_repere2D*> L_reperes;
   alx_point2D pt_au_contact; bool point_au_contact_definit, empiler_contenant;
   alx_liste<info_du_contenant*> L_reponses_cumulees;

   info_du_contenant() {L_rappel=(alx_liste<Liste_de_rappel*>*)NULL;
                        noeud = NULL;
                        mp = NULL;
                        evt_ptr = NULL;
                        point_au_contact_definit = false;
                        empiler_contenant = false;}
   info_du_contenant(alx_model_simulation_physique1 *m, alx_noeud_scene *n, alx_evennement_pointeur *evt)
    {mp = m; noeud = n;
     L_rappel=(alx_liste<Liste_de_rappel*>*)NULL;
     evt_ptr = evt;
     point_au_contact_definit = false;
     empiler_contenant = false;}
  ~info_du_contenant()
   {mp    = (alx_model_simulation_physique1*)NULL;
    noeud = (alx_noeud_scene*)NULL;
    evt_ptr = (alx_evennement_pointeur*)NULL;
    if(L_rappel)
     {L_rappel->Vider();
      delete L_rappel;
     }
    L_rappel=(alx_liste<Liste_de_rappel*>*)NULL;
    alx_element_liste<info_du_contenant*> *it     = L_reponses_cumulees.Premier()
                                        , *it_fin = L_reponses_cumulees.Fin();
    for(;it!=it_fin; it=it->svt)
     {delete it->E();}
   }

  // Les méthodes
   inline const bool Empiler_contenant() const {return empiler_contenant;}
   inline void Empiler_contenant(const bool b) {empiler_contenant = b;}
   void                           LL_RAPPEL(alx_liste<Liste_de_rappel*> *L) {L_rappel = L;}
   alx_liste<Liste_de_rappel*>*   LL_RAPPEL()                               {return L_rappel;}
   alx_liste<info_du_contenant*>& L_Reponses_cumulees()                     {return L_reponses_cumulees;}
   alx_evennement_pointeur*       EVT()                                     {return evt_ptr;}
   void                           EVT(alx_evennement_pointeur *e)           {evt_ptr = e;}

   const alx_pointeur*            Ptr() const;

   alx_model_simulation_physique1* MP()        {return mp;}
   alx_noeud_scene*                NOEUD()     {return noeud;}
   alx_liste<alx_repere2D*>&       L_REPERES() {return L_reperes;}

   void Point_au_contact(const alx_point2D &pt)          {pt_au_contact.maj(pt);   point_au_contact_definit=true;}
   void Point_au_contact(const double x, const double y) {pt_au_contact.maj(x, y); point_au_contact_definit=true;}
   alx_point2D& Point_au_contact() {return pt_au_contact;}
   const double X_au_contact() const {return pt_au_contact.X();}
   const double Y_au_contact() const {return pt_au_contact.Y();}
};

info_du_contenant* Void_vers_info(void *p) {return (info_du_contenant*)p;}

void IP_locale(const alx_chaine_char &a);
void IP_locale(const char *a);
const alx_chaine_char& IP_locale();
const unsigned int TAILLE_CONTEXTE = 1024;


class boite_englobante : public Couleur_RVBA<float>
{private:
   alx_point2D p1, p2, p_tmp_1, p_tmp_2;
   mutable alx_point2D pt_milieu;
   bool   a_change, est_vide, afficher;
   void init_var() {a_change = true;
                    est_vide = true;
                    afficher = false;
                    maj_couleur(1, 1, 0, 1);}

 public :
   boite_englobante() {init_var();}
   boite_englobante( const double x1, const double y1
                   , const double x2, const double y2) {init_var();
                                                        maj(x1,y1,x2,y2);}
   inline void maj( const boite_englobante &b) {Est_vide( b.Est_vide() );
                                                maj( b.X1(), b.Y1()
                                                   , b.X2(), b.Y2() );}
   inline void maj( const double x1, const double y1
                  , const double x2, const double y2) {p1.maj(x1, y1);
                                                       p2.maj(x2, y2);
                                                       A_change(true);}
  // Accès aux ancêtres
   inline Couleur_RVBA<float>& P_Couleur() {return (Couleur_RVBA<float>&)*this;}

  // Pour afficher...et au passage débugger
   const bool Est_a_afficher() const {return afficher;}
   void Est_a_afficher(const bool b) {afficher = b;}
   void Afficher() const;

  // Les fonctions d'accès aux constituants
   inline const double X1() const {return this->p1.X();}
     inline void X1(const double v) {this->p1.X(v);}
   inline const double Y1() const {return this->p1.Y();}
     inline void Y1(const double v) {this->p1.Y(v);}
   inline const double X2() const {return this->p2.X();}
     inline void X2(const double v) {this->p2.X(v);}
   inline const double Y2() const {return this->p2.Y();}
     inline void Y2(const double v) {this->p2.Y(v);}
   inline const bool A_change() const {return this->a_change;}
     inline void A_change(const bool v) {this->a_change = v;}
   inline const bool Est_vide() const {return this->est_vide;}
     inline void Est_vide(const bool v) {this->est_vide = v;}
   inline const double Tx() const {return absd(X1()-X2());}
   inline const double Ty() const {return absd(Y1()-Y2());}
   inline const double BG_X() const {return Amin(X1(), X2());}
   inline const double BG_Y() const {return Amin(Y1(), Y2());}
   inline const double HD_X() const {return Amax(X1(), X2());}
   inline const double HD_Y() const {return Amax(Y1(), Y2());}
   inline const double Cx()   const {return (X1() + X2())/2.0;}
   inline const double Cy()   const {return (Y1() + Y2())/2.0;}

   inline const alx_point2D& Pt_milieu() const {pt_milieu.maj( (X1()+X2())/2
                                                             , (Y1()+Y2())/2);
                                                return pt_milieu;}
  // Les fonctions de gestions
   inline void Calculer_boite( const alx_point2D &pt1, const alx_point2D &pt2
                             , const alx_point2D &pt3, const alx_point2D &pt4) {double x1, x2, y1, y2;
                                                                                x1 = Amin( Amin(pt1.X(), pt2.X())
                                                                                         , Amin(pt3.X(), pt4.X()) );
                                                                                x2 = Amax( Amax(pt1.X(), pt2.X())
                                                                                         , Amax(pt3.X(), pt4.X()) );
                                                                                y1 = Amin( Amin(pt1.Y(), pt2.Y())
                                                                                         , Amin(pt3.Y(), pt4.Y()) );
                                                                                y2 = Amax( Amax(pt1.Y(), pt2.Y())
                                                                                         , Amax(pt3.Y(), pt4.Y()) );
                                                                                X1(x1); Y1(y1);
                                                                                X2(x2); Y2(y2);}

   inline void Inclure_boite(const boite_englobante &b) {if(b.Est_vide()) return;
                                                         if(Est_vide()) {maj(b);}
                                                          else {X1(Amin(X1(), b.X1()));
                                                                Y1(Amin(Y1(), b.Y1()));
                                                                X2(Amax(X2(), b.X2()));
                                                                Y2(Amax(Y2(), b.Y2()));}
                                                         A_change(true);
                                                         Est_vide(false);}
   inline void Prendre_en_compte(const alx_repere2D &r) {if(Est_vide()) return;
                                                         p_tmp_1.maj(p1.X(), p2.Y());
                                                         p_tmp_2.maj(p2.X(), p1.Y());
                                                         r.Changer_coordonnees_inverse(p1);
                                                         r.Changer_coordonnees_inverse(p2);
                                                         r.Changer_coordonnees_inverse(p_tmp_1);
                                                         r.Changer_coordonnees_inverse(p_tmp_2);
                                                         Calculer_boite(p1, p2, p_tmp_1, p_tmp_2);
                                                         A_change(true);}
  // Fonctions d'appartenance
   inline const bool Contient(const alx_point2D &pt) const {return ( (pt.X() >= X1())&&(pt.X()<=X2())
                                                                   &&(pt.Y() >= Y1())&&(pt.Y()<=Y2()) );}
};

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
class alx_noeud_scene;

inline const unsigned int Rappel_Seulement_si_noeud_contient() {return 4194304;}
inline const unsigned int Rappel_Seulement_si_fils_contient () {return 8388608;}
inline const unsigned int Pas_rappel_si_noeud_contient() {return 16777216;}
inline const unsigned int Pas_rappel_si_fils_contient () {return 33554432;}

class info_si_noeud_contient
{public:
   alx_noeud_scene         *noeud_contenant;
   info_du_contenant       *info;
   alx_liste<alx_repere2D*> L_reperes;

   alx_noeud_scene*           Noeud() {return noeud_contenant;}
   info_du_contenant*         Info()  {return info;}
   alx_liste<alx_repere2D*>&  L_rep() {return L_reperes;}
};

info_si_noeud_contient* Void_vers_info_si_noeud_contient(void *p) {return (info_si_noeud_contient*)p;}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
class alx_noeud_zone_rendu_sdl_opengl;
class alx_noeud_scene : public alx_arbre<alx_noeud_scene*>, public alx_repere2D
{private:
   info_si_noeud_contient i_si_contient;
   void init_ptr();
   alx_point2D pt2, pt_env, pt_pour_contient;
   static alx_chaine_char ip_locale;
   alx_chaine_char nom_classe;
   static unsigned int nb_noeud;
   unsigned int nb;
   alx_chaine_char nom;
   int action_transmise;

   unsigned int marqueur_de_parcours;
   unsigned int M_attrib;

   bool deja_serialise, avorter, rationnalisation_en_cours, deja_prerendu
      , est_une_replique, pre_rendu_exeptionnel
      , noeud_touchable, noeud_de_coupe, noeud_puis_fils
      , maj_boites_recursif;
   mutable bool   fils_a_subi_seri_diff, fils_a_subi_deseri_diff;
   mutable bool   changement_dans_liste_fils;
   mutable double COPIE_nb_pix_par_unite;

   bool pipoter_affichage, heritage_de_pipoter_affichage;
   const bool Pipoter_affichage();

   unsigned int marqueur_difference;

   alx_prerendeur  *prerendeur;
   alx_noeud_scene *limite, *limite_svg;
   mutable alx_chaine_char serialisation_contenu, serialisation_type, serialisation_Lfils
                         , serialisation_differences_type, serialisation_differences_contenu
                         , serialisation_fils
                         , nouv_contenu, nouv_type, cc_tmp, nom_f_tmp
                         , nom_iu;
   static alx_chaine_char cc_marqueur_parcours_val;

   static unsigned int marqueur_pour_parcours_recursif;

 protected:
// Les changements
   mutable bool parcours_changement, forcer_pre_rendu_noeud;
   const bool Changement();

// Gestion de la texture représantante du noeud
   alx_noeud_zone_rendu_sdl_opengl *zdr;
   int id_texture;
   bool mode_texture, maj_texture, translucide;
   inline void Maj_texture(const bool b) {maj_texture = b;}
   const bool Maj_texture();
   double couleur_masque_texture[4];

   alx_repere2D repere_tmp;
   const boite_englobante& get_boite_pour_texture();

// Autres...
   static alx_noeud_scene *noeud_courant;
   alx_noeud_scene *noeud_pour_seri_fils;
   unsigned int nb_ptr_present_ds_noeud, nb_ptr_present_ds_noeud_prcdt
              , nb_ptr_present_ds_fils , nb_ptr_present_ds_fils_prcdt;
   bool replication_juste_effectuee;
  // Pour la sérialisation/désérialisation rapide
   void Deserialiser_bool  (unsigned int & pos, void *res);
   void Deserialiser_int   (unsigned int & pos, void *res);
   void Deserialiser_double(unsigned int & pos, void *res);
   void Deserialiser_repere(unsigned int & pos, void *res);
   void Deserialiser_chaine(unsigned int & pos, void *res);
   void Deserialiser_Lfils (unsigned int & pos, void *res);
   P_void a_ptr[50];
   typedef void (alx_noeud_scene::*f_deserialisation_scene)(unsigned int &pos, void *res);
   f_deserialisation_scene f_ptr[6]; // pointeurs sur les fonctions de désérialisation
   const alx_chaine_char *chaine_cour;

 // Pour les parcours de graphe
   unsigned int nb_max_pre_rendu, nb_pre_rendu
              , nb_max_affichage, nb_affichage
              , limite_nb_recursions;
   const int Chercher_dans_contexte( const alx_chaine_char &contexte, const unsigned int nb_elements
                                   , const unsigned int *positions, const unsigned int *tailles
                                   , const alx_chaine_char &var) const;
   MetaData M_contexte_local, M_meta_data;
   mutable alx_chaine_char cc_var_tmp;
/*   alx_chaine_char contexte_global
                 , contexte_local
                 , meta_data;
   mutable alx_chaine_char cc_var_tmp, cc_val_tmp, cc_var_for_val, cc_var_for_maj_var, cc_var_for_val_meta_data;
   unsigned int positions_locales[TAILLE_CONTEXTE]
              , positions_globales[TAILLE_CONTEXTE]
              , positions_meta_data[TAILLE_CONTEXTE]
              , tailles_locales[TAILLE_CONTEXTE]
              , tailles_globales[TAILLE_CONTEXTE]
              , tailles_meta_data[TAILLE_CONTEXTE]
              , nb_elements_locaux
              , nb_elements_globaux
              , nb_elements_meta_data;
 */
   bool tenir_compte_des_boites_des_fils;
   boite_englobante boite_noeud               // Le rectangle anglobant du noeud seulement dans les coordonnées locales.
                  , boite_fils                // Le rectangle anglobant des fils seulement dans les coordonnées locales.
                  , boite_noeud_et_fils       // Le rectangle anglobant du sous graphe ayant ce noeud pour racine, en coordonnées locales.
                  , boite_noeud_et_fils_glob; // Le rectangle anglobant du sous graphe ayant ce noeud pour racine, en coordonnées globales.

   alx_model_simulation_physique1  mp, *ref_mp;

   Liste_de_rappel L_rappel_pointeur_detecte             // On enregistre tout ceux qui veulent être informé d'une activité d'un pointeur sur le noeud
                 , L_rappel_effective_pointeur_detecte;  // On placera ici les rappels qui correspondent effectivement à une activité donnée.
                                                        // C'est cette liste qui sera communiqué à "l'info_si_noeud_contient".
   alx_liste<unsigned int>  L_etat_pointeur;                      // On enregistre pour quels évennements les rappels doivent avoir lieu

   Liste_de_rappel LR_Avant_pre_rendu_fils   , LR_Apres_pre_rendu_fils
                 , LR_Avant_affichage_fils   , LR_Apres_affichage_fils
                 , LR_Avant_contenu_dans_fils, LR_Apres_contenu_dans_fils
                 , LR_Avant_affichage        , LR_Apres_affichage
                 , LR_Avant_pre_rendu        , LR_Apres_pre_rendu
                 ;
   info_du_contenant  *contenant_courant;
   alx_methode_rappel *rap_pre_traitement_contient, *rap_post_traitement_contient;
   bool                    pre_traitement_contient;

   void Vider_Listes();

   alx_element_liste<alx_methode_rappel*> *mr_cour, *mr_fin;
   alx_element_liste<unsigned int>        *ep_cour, *ep_fin;
   bool noeud_a_changer, serialisation_a_changer;

 public :
   mutable alx_liste<alx_chaine_char> L_fils_seri;
   int val_stencil
     , operation_si_appuie
     , operation_si_relache
     , operation_si_passage;

//_____________________________________________________________________________
   inline alx_model_simulation_physique1& Intrinseque_MP() {return mp;}
   inline alx_model_simulation_physique1& MP()             {return *ref_mp;}
   inline void MP(alx_model_simulation_physique1 *m)       {ref_mp = m;}

//_____________________________________________________________________________
   alx_noeud_scene(const bool est_texture_optimisable = true);
   ~alx_noeud_scene() {Vider_peres();
                       Vider_fils();
                       Vider_Listes();
                      }
   virtual const char* Real_class_cmd() {return "Void_vers_noeud";}

//_____________________________________________________________________________
   const unsigned int GET_a_new_marqueur() {return ++marqueur_pour_parcours_recursif;}

//_____________________________________________________________________________
   inline const bool Pipoter_affichage_noeud() const {return pipoter_affichage;}
   inline void Pipoter_affichage_noeud(const bool b) {pipoter_affichage = b;}

//_____________________________________________________________________________
   void maj_noeud (const alx_noeud_scene &);
   void maj_repere(const alx_repere2D &);
//_____________________________________________________________________________
   inline const alx_point2D& Pt_pour_contient() const {return pt_pour_contient;}

//_____________________________________________________________________________
   info_du_contenant* Creer_contenant();
   inline info_du_contenant* Contenant_courant() const {return contenant_courant;}
   inline void Contenant_courant(info_du_contenant *i) {contenant_courant = i;}

   inline alx_methode_rappel* Rap_post_traitement_contient() const {return rap_post_traitement_contient;}
   inline void Rap_post_traitement_contient(alx_methode_rappel *r) {rap_post_traitement_contient = r;}

   inline alx_methode_rappel* Rap_pre_traitement_contient() const {return rap_pre_traitement_contient;}
   inline void Rap_pre_traitement_contient(alx_methode_rappel *r) {rap_pre_traitement_contient = r;}

   inline const bool Pre_traitement_contient() const {return pre_traitement_contient;}
   inline void Pre_traitement_contient(const bool b) {pre_traitement_contient = b;}

//_____________________________________________________________________________
   static alx_noeud_scene* Noeud_courant()                   {return noeud_courant;}
   static alx_noeud_scene* Noeud_courant(alx_noeud_scene *n) {noeud_courant = n;; return n;}

   INFOS_TEXTURE* Info_texture_scene() const;
//_____________________________________________________________________________
//   inline const char* T_Contexte_global() const {return contexte_global.Texte();}
//   inline const alx_chaine_char& CC_Contexte_local()  const {return contexte_local;}
//   inline const alx_chaine_char& CC_Contexte_global() const {return contexte_global;}
//   inline const alx_chaine_char& CC_MetaData() const {return meta_data;}
//   inline virtual void Vider_contexte_global() {nb_elements_globaux=0; contexte_global.Maj("",0,0); positions_globales[0]=0;}
//   inline virtual bool Evaluer_dans_contexte(const char *var) {cc_var_tmp = var; return Evaluer_dans_contexte(cc_var_tmp);}
//   const char* CC_Tab_Val(const alx_chaine_char &var)                                const {return Tab_Val(var.Texte());}
//   const char* Tab_Val(const char *var)                                              const {}
   inline const char* T_MetaData()                                                   const {return M_meta_data.get_texte();}
   inline virtual void Vider_MetaData()                                                    {M_meta_data.Vider();}
   inline virtual void Ajouter_MetaData_T(const char *var, const char *val)                {M_meta_data.Ajouter(var, val);}
   inline void Ajouter_MetaData_CC(const alx_chaine_char &var, const alx_chaine_char &val) {M_meta_data.Ajouter(var.Texte(), val.Texte());}
   inline virtual const bool Retirer_MetaData_T(const char *var)                           {return M_meta_data.Retirer(var);}
   inline const bool Retirer_MetaData_CC(const alx_chaine_char &var)                       {return M_meta_data.Retirer(var);}
   const char* CC_Val_MetaData(const alx_chaine_char &var)                           const {return M_meta_data.Val_CC(var);}
   const char* Val_MetaData(const char *var)                                         const {return M_meta_data.Val(var);}

   inline const char* T_Contexte_local()                                             const {return M_contexte_local.get_texte();}
   inline virtual void Vider_contexte_local()                                              {M_contexte_local.Vider();}
   inline virtual void Maj_var(const char *var, const char *val)                           {M_contexte_local.Ajouter(var, val);}
   inline virtual void Ajouter_au_contexte( const char *var, const char *val
                                          , bool verifier_unicite = true)                  {M_contexte_local.Ajouter(var, val);}
   virtual const bool Retirer_du_contexte(const char *var)                                 {return M_contexte_local.Retirer(var);}
   const char* CC_Val(const alx_chaine_char &var)                                    const;
   const char* Val(const char *var)                                                  const;
   virtual bool Evaluer_dans_contexte(const alx_chaine_char &var)                          {return M_contexte_local.Val_CC(var);}

//_____________________________________________________________________________
   inline virtual alx_liste<alx_arbre<alx_noeud_scene*>*>& Liste_fils()  {return L_fils;}
   inline virtual alx_liste<alx_arbre<alx_noeud_scene*>*>& Liste_peres() {return L_peres;}

   inline       alx_noeud_scene* Pere      ()       {if(pere) {return pere->E();} else {return (alx_noeud_scene*)NULL;}}
   inline const alx_noeud_scene* Pere_const() const {if(pere) {return pere->E();} else {return (alx_noeud_scene*)NULL;}}

//   virtual void Vider_peres() {alx_arbre<alx_noeud_scene*>::Vider_peres();}
   inline virtual alx_noeud_scene* Fils_numero (const unsigned int num) const                {return (alx_noeud_scene*)(alx_arbre<alx_noeud_scene*>::Fils_numero(num));}
   virtual void Vider_peres();
   inline virtual void Vider_fils()                                                          {alx_arbre<alx_noeud_scene*>::Vider_fils();}
   inline virtual bool A_pour_fils(alx_noeud_scene *e)                                       {return alx_arbre<alx_noeud_scene*>::A_pour_fils(e);}
   inline virtual void Ajouter_fils_au_debut(alx_noeud_scene *e)                             {A_changer(true); alx_arbre<alx_noeud_scene*>::Ajouter_fils_au_debut(e);}
   inline virtual void Ajouter_fils_a_la_fin(alx_noeud_scene *e)                             {A_changer(true); alx_arbre<alx_noeud_scene*>::Ajouter_fils_a_la_fin(e);}
   inline virtual void Ajouter_fils_replique(alx_noeud_scene *e)                             {A_changer(true); alx_arbre<alx_noeud_scene*>::Ajouter_fils_replique(e);}
   inline virtual void Ajouter_fils(alx_noeud_scene *e)                                      {A_changer(true); alx_arbre<alx_noeud_scene*>::Ajouter_fils(e);}
   inline virtual void Ajouter_fils_a_pos(alx_noeud_scene *e, const unsigned int pos)        {A_changer(true); alx_arbre<alx_noeud_scene*>::Ajouter_fils_a_pos(e, pos);}
   inline virtual void Ajouter_fils_it( alx_noeud_scene *e
                                      , alx_element_liste<alx_arbre<alx_noeud_scene*>*> &it) {A_changer(true); alx_arbre<alx_noeud_scene*>::Ajouter_fils(e, it);}
   inline virtual void Retirer_fils(alx_noeud_scene *e)                                      {A_changer(true); alx_arbre<alx_noeud_scene*>::Retirer_fils(e);}
   inline void Retirer_fils_it(alx_element_liste<alx_arbre<alx_noeud_scene*>*> &it)          {A_changer(true); alx_arbre<alx_noeud_scene*>::Retirer_fils(it);}
   const unsigned int Chemin_ancetre(const alx_noeud_scene *racine, alx_liste<const alx_noeud_scene*> &L_rep);

   virtual bool A_pour_L_fils(const alx_liste<alx_arbre<alx_noeud_scene*>*> &L)          ;
   virtual void Ajouter_L_fils_au_debut(const alx_liste<alx_arbre<alx_noeud_scene*>*> &L);
   virtual void Ajouter_L_fils_a_la_fin(const alx_liste<alx_arbre<alx_noeud_scene*>*> &L);
   virtual void Ajouter_L_fils_replique(const alx_liste<alx_arbre<alx_noeud_scene*>*> &L);
   virtual void Ajouter_L_fils(const alx_liste<alx_arbre<alx_noeud_scene*>*> &L)         ;
   virtual void Ajouter_fils_avant(alx_noeud_scene *noeud, alx_noeud_scene *noeud_fils);
   virtual void Ajouter_L_fils( const alx_liste<alx_arbre<alx_noeud_scene*>*> &L
                            , alx_element_liste<alx_arbre<alx_noeud_scene*>*> &it)     ;
   virtual void Retirer_L_fils(const alx_liste<alx_arbre<alx_noeud_scene*>*> &L)         ;

  // Parcours des fils avec des méthodes type machine à état
  // Pour un parcours plus facile...
   inline alx_element_liste<alx_arbre<alx_noeud_scene*>*>* Init_parcours_peres()                    const {return L_peres.Init_parcours();}
   inline const bool Est_parcours_peres_fini( alx_element_liste<alx_arbre<alx_noeud_scene*>*> *it ) const {return L_peres.Est_parcours_fini(it);}
   inline      alx_noeud_scene* Courant_dans_parcours_peres( alx_element_liste<alx_arbre<alx_noeud_scene*>*> *it )  const {return L_peres.Courant_dans_parcours(it)->e;}
   inline alx_element_liste<alx_arbre<alx_noeud_scene*>*>* Suivant_dans_parcours_peres( alx_element_liste<alx_arbre<alx_noeud_scene*>*> *it ) const {return L_peres.Suivant_dans_parcours(it);}
   inline void Terminer_parcours_peres( alx_element_liste<alx_arbre<alx_noeud_scene*>*>  *it )                      const {L_peres.Terminer_parcours(it);}

   inline const unsigned int Nb_fils () const {return L_fils.Taille();}
   inline const unsigned int Nb_Peres() const {return L_peres.Taille();}

   inline alx_element_liste<alx_arbre<alx_noeud_scene*>*>* Init_parcours_fils()                           const {return L_fils.Init_parcours();}
   inline const bool Est_parcours_fils_fini( alx_element_liste<alx_arbre<alx_noeud_scene*>*> *it )                 const {return L_fils.Est_parcours_fini(it);}
   inline const alx_noeud_scene* Courant_dans_parcours_fils( alx_element_liste<alx_arbre<alx_noeud_scene*>*> *it ) const {return L_fils.Courant_dans_parcours(it)->e;}
   inline alx_element_liste<alx_arbre<alx_noeud_scene*>*>* Suivant_dans_parcours_fils( alx_element_liste<alx_arbre<alx_noeud_scene*>*> *it ) const {return L_fils.Suivant_dans_parcours(it);}
   inline void Terminer_parcours_fils( alx_element_liste<alx_arbre<alx_noeud_scene*>*>  *it )                       const {L_fils.Terminer_parcours(it);}

   inline alx_element_liste<alx_arbre<alx_noeud_scene*>*>* Init_parcours_inverse_fils()                           const {return L_fils.Init_parcours_inverse_fils();}
   inline const bool Est_parcours_inverse_fils_fini( alx_element_liste<alx_arbre<alx_noeud_scene*>*> *it )                 const {return L_fils.Est_parcours_inverse_fils_fini(it);}
   inline const alx_noeud_scene* Courant_dans_parcours_inverse_fils( alx_element_liste<alx_arbre<alx_noeud_scene*>*> *it ) const {return L_fils.Courant_dans_parcours_inverse_fils(it)->e;}
   inline alx_element_liste<alx_arbre<alx_noeud_scene*>*>* Suivant_dans_parcours_inverse_fils( alx_element_liste<alx_arbre<alx_noeud_scene*>*> *it ) const {return L_fils.Suivant_dans_parcours_inverse_fils(it);}
   inline void Terminer_parcours_inverse_fils( alx_element_liste<alx_arbre<alx_noeud_scene*>*>  *it )                       const {L_fils.Terminer_parcours_inverse(it);}

//@}

  // On peut aussi vouloir l'utiliser comme une pauvre texture...
   inline const bool Forcer_pre_rendu_noeud() const {return forcer_pre_rendu_noeud;}
   inline void Forcer_pre_rendu_noeud(const bool b) const {forcer_pre_rendu_noeud = b;}
   inline const alx_noeud_zone_rendu_sdl_opengl& ZDR() const {return *zdr;}
   //const INFOS_TEXTURE& Infos_texture(const unsigned int i = 0) const;
   inline void Mode_texture(const bool b)        {Marqueur_attributs(M_mode_texture, b); Maj_texture(b);}
   inline const bool Mode_texture() const        {return M_mode_texture&Marqueur_attributs();}
   inline void Mode_texture_fils(const bool b)   {Marqueur_attributs(M_mode_texture_fils, b); Maj_texture(true);}
   inline const bool Mode_texture_fils() const   {return M_mode_texture_fils&Marqueur_attributs();}
   inline void Texture_translucide(const bool b) {Marqueur_attributs(M_texture_translucide, b);}
   inline const bool Texture_translucide() const {return M_texture_translucide&Marqueur_attributs();}
   inline void Lisser_texture(const bool b)      {Marqueur_attributs(M_lisser_texture, b); A_changer(true);}
   inline const bool Lisser_texture() const      {return M_lisser_texture&Marqueur_attributs();}
   inline const bool Boite_noeud_pour_texture() const {return M_Boite_noeud_pour_texture&Marqueur_attributs();}
   inline void Boite_noeud_pour_texture(const bool b) {Marqueur_attributs(M_Boite_noeud_pour_texture, b);}

   void Nb_pixels_par_unite(const double n);
   const double Nb_pixels_par_unite() const;
   const double* Couleur_masque_texture () const {return couleur_masque_texture;}
   void Couleur_masque_texture(double r, double v, double b, double a) {couleur_masque_texture[0] = r; couleur_masque_texture[1] = v; couleur_masque_texture[2] = b; couleur_masque_texture[3] = a;}
   void Couleur_masque_texture(double *c) {memcpy(couleur_masque_texture, c, 4*sizeof(double));}
   const double Composante_couleur_masque_texture(const unsigned int i) const {return couleur_masque_texture[i];}
   void Composante_couleur_masque_texture(const unsigned int i, const double v) {couleur_masque_texture[i] = v;}
   const double  Composante_couleur_fond_texture(const unsigned int i);
   void          Composante_couleur_fond_texture(const unsigned int i, const double v);
   void          Couleur_fond_texture           (const double r, const double v, const double b, const double a);
   void          Couleur_fond_texture(const double *c);
   const double* Couleur_fond_texture() const;
   double*       P_couleur_fond_texture(const unsigned int i);

  //_____________________________________________________________________________
   inline virtual const bool A_changer() const {return alx_repere2D::A_changer();}
   inline virtual void A_changer(const bool b) {//noeud_a_changer = b;
                                                //Forcer_pre_rendu_noeud(true);
                                                alx_repere2D::A_changer(b);
                                                /*if(b)
                                                 {
                                                  alx_noeud_scene *p;
                                                  Init_parcours_peres();
                                                  while(!Est_parcours_peres_fini())
                                                   {p = Courant_dans_parcours_peres();
                                                    //if(!p->A_changer()) {p->A_changer(true);}
                                                    Suivant_dans_parcours_peres();
                                                   }
                                                 }*/
                                               }

   inline const bool Est_verouille() const {return M_verouiller&Marqueur_attributs();}
   inline void Verouiller  () {while(Est_verouille()); Marqueur_attributs(M_verouiller, true);}
   inline void Deverouiller() {Marqueur_attributs(M_verouiller, false);}

  // La gestion des listes de rappels
   inline Liste_de_rappel& get_L_rappel_effective_pointeur_detecte() {return L_rappel_effective_pointeur_detecte;}
   inline void abonner_a_detection_pointeur    ( alx_methode_rappel *m
                                               , const unsigned int etats_ptr=2) {L_rappel_pointeur_detecte.Ajouter_methode_a_la_fin(m);
                                                                                  L_etat_pointeur.Ajouter_a_la_fin(etats_ptr);}
   inline bool desabonner_de_detection_pointeur(alx_methode_rappel *m) {const int index = L_rappel_pointeur_detecte.Index(m);
                                                                        if(index >= 0)
                                                                         {L_rappel_pointeur_detecte.Retirer_methode(m);
                                                                          L_etat_pointeur.Retirer_index(index);
                                                                          return true;}
                                                                        return false;
                                                                       }
  // Gestion des listes de rappels aux entrées et sortit des fonctions de parcours
   inline Liste_de_rappel& LR_Av_aff()       {return LR_Avant_affichage;}
   inline Liste_de_rappel& LR_Ap_aff()       {return LR_Apres_affichage;}
   inline Liste_de_rappel& LR_Av_PR_fils()   {return LR_Avant_pre_rendu_fils;}
   inline Liste_de_rappel& LR_Ap_PR_fils()   {return LR_Apres_pre_rendu_fils;}
   inline Liste_de_rappel& LR_Av_aff_fils()  {return LR_Avant_affichage_fils;}
   inline Liste_de_rappel& LR_Ap_aff_fils()  {return LR_Apres_affichage_fils;}
   inline Liste_de_rappel& LR_Av_cont_fils() {return LR_Avant_contenu_dans_fils;}
   inline Liste_de_rappel& LR_Ap_cont_fils() {return LR_Apres_contenu_dans_fils;}
   inline Liste_de_rappel& LR_Av_pre_rendu() {return LR_Avant_pre_rendu;}
   inline Liste_de_rappel& LR_Ap_pre_rendu() {return LR_Apres_pre_rendu;}

   inline void abonner_a_LR_parcours ( Liste_de_rappel &LR
                                     , alx_methode_rappel *m) {LR.Ajouter_methode_a_la_fin(m);}
   inline void abonner_a_LR_parcours_deb ( Liste_de_rappel &LR
                                         , alx_methode_rappel *m) {LR.Ajouter_methode_au_debut(m);}
   inline void abonner_a_LR_parcours_fin ( Liste_de_rappel &LR
                                         , alx_methode_rappel *m) {LR.Ajouter_methode_a_la_fin(m);}

   inline void desabonner_de_LR_parcours ( Liste_de_rappel &LR
                                         , alx_methode_rappel *m) {LR.Retirer_methode(m);}


/** @name Gestion des boîtes englobantes.
 */
   const boite_englobante& Boite_noeud()              const {return boite_noeud;}
   const boite_englobante& Boite_fils()               const {return boite_fils;}
   const boite_englobante& Boite_noeud_et_fils()      const {return boite_noeud_et_fils;}
   const boite_englobante& Boite_noeud_et_fils_glob() const {return boite_noeud_et_fils_glob;}
   void Maj_boites_recursif();

   inline virtual void Afficher_boites(const bool b) {Afficher_boite_noeud(b); Afficher_boites_fils(b);}
   inline virtual void Afficher_boite_noeud(const bool b) {Marqueur_attributs(M_afficher_boite_noeud, b);}
   inline virtual const bool Afficher_boite_noeud() const {return M_afficher_boite_noeud&Marqueur_attributs();}
   inline virtual void Afficher_boites_fils(const bool b) {Marqueur_attributs(M_afficher_boites_fils, b);}
   inline virtual const bool Afficher_boites_fils() const {return M_afficher_boites_fils&Marqueur_attributs();}
   inline virtual void Calculer_boites() {alx_noeud_scene::Calculer_boite_fils(); alx_noeud_scene::Calculer_boite_noeud();}
   virtual void Calculer_boite_noeud();
   virtual void Calculer_boite_fils();

   const double Taille_x() const {return boite_noeud.Tx();}
   const double Taille_y() const {return boite_noeud.Ty();}

  // Contrôle d'accès multi-thread:
   void Ajouter_appels(info_du_contenant *rep, alx_noeud_scene *noeud_contenant, Liste_de_rappel &L_rappel_effective);
   void Ajouter_appels(info_du_contenant *rep, const unsigned int type, Liste_de_rappel &L_rappel);

   inline void Nom_classe(const alx_chaine_char &n) {nom_classe = n;}
   inline void Nom_classe(const char *n)            {nom_classe = n;}
   inline const alx_chaine_char& Nom_classe() const {return nom_classe;}

   void Suffixer_nom(const char *t);
   inline const alx_chaine_char& Nom() const {return nom;}
   inline void Nom(const alx_chaine_char &n) {nom = n;}

   inline virtual void Nom_IU(const alx_chaine_char &cc)   {nom_iu = cc;}
   inline virtual void Nom_IU(const char *n)               {nom_iu = n;}
   inline virtual const char* Nom_IU()               const {return nom_iu.Texte();}
   inline virtual const alx_chaine_char& Nom_IU_cc() const {return nom_iu;}

   inline void Limite_nb_recursions(const unsigned int n) {limite_nb_recursions = n;}
   inline const unsigned int Limite_nb_recursions() const {return limite_nb_recursions;}

   inline void Marqueur_de_parcours(const unsigned int n) {marqueur_de_parcours = n;}
   inline const unsigned int Marqueur_de_parcours() const {return marqueur_de_parcours;}

   inline void Marqueur_attributs(const unsigned int m, bool b)
    {if(b) M_attrib |= m;
      else M_attrib &= ~m;}
   inline void Marqueur_attributs(const unsigned int m) {M_attrib = m;}
   inline const unsigned int Marqueur_attributs() const {return M_attrib;}

   inline void Pre_rendu_exeptionnel(const bool b) {pre_rendu_exeptionnel = b;}
   inline const bool Pre_rendu_exeptionnel() const {return pre_rendu_exeptionnel;}

   inline void Changement_dans_liste_fils(const bool b) {changement_dans_liste_fils = b;}
   inline const bool Changement_dans_liste_fils() const {return changement_dans_liste_fils;}

   inline void Est_une_replique(const bool b) {est_une_replique = b;}
   inline const bool Est_une_replique() const {return est_une_replique;}

   inline void Deja_serialise(const bool b) {deja_serialise = b;}
   inline const bool Deja_serialise() const {return deja_serialise;}
   inline void Marqueur_difference(const unsigned int m) {marqueur_difference = m;}
   inline const unsigned int Marqueur_difference() const {return marqueur_difference;}

   inline void Avorter(const bool b) {avorter = b;}
   inline const bool Avorter() const {return avorter;}

   inline void Limite(alx_noeud_scene *n) {limite = n;}
   inline alx_noeud_scene* Limite() const {return limite;}
   inline void Limite_svg(alx_noeud_scene *n) {limite_svg = n;}
   inline alx_noeud_scene* Limite_svg() const {return limite_svg;}

   inline void Rationnalisation_en_cours(const bool b) {rationnalisation_en_cours = b;}
   inline const bool Rationnalisation_en_cours() const {return rationnalisation_en_cours;}

   inline void Forcer_a_ne_plus_empiler_contenant(const bool b) {Marqueur_attributs(M_forcer_a_ne_plus_empiler_contenant, b);}
   inline const bool Forcer_a_ne_plus_empiler_contenant() const {return M_forcer_a_ne_plus_empiler_contenant&Marqueur_attributs();}

   inline void Empiler_contenant(const bool b) {Marqueur_attributs(M_empiler_contenant, b);}
   inline const bool Empiler_contenant() const {return M_empiler_contenant&Marqueur_attributs();}

   inline void Toujours_au_premier_plan(const bool b) {Marqueur_attributs(M_toujours_au_premier_plan, b);}
   inline const bool Toujours_au_premier_plan() const {return M_toujours_au_premier_plan&Marqueur_attributs();}

   inline void Noeud_touchable(const bool b) {noeud_touchable = b;}
   inline const bool Noeud_touchable() const {return noeud_touchable;}

   inline void Noeud_de_coupe(const bool b) {noeud_de_coupe = b;}
   inline const bool Noeud_de_coupe() const {return noeud_de_coupe;}

   inline void Noeud_puis_fils(const bool b) {noeud_puis_fils = b;}
   inline const bool Noeud_puis_fils() const {return noeud_puis_fils;}

   virtual void Afficher_fils(int add_val_stencil);
   virtual void Afficher();
   virtual void Transfo_repere();
   virtual void Depiler_transfo_repere();
   virtual void Dessin_noeud();
   virtual void Dessin_texture_noeud();
   virtual void Dessin();

   void Prerendeur(alx_prerendeur *p)                                {prerendeur = p;}
   alx_prerendeur* Prerendeur()                                      {return prerendeur;}

  // Sérialisation pour le transfert
   virtual const alx_chaine_char& Serialiser_difference_contenu() const;
   inline virtual const alx_chaine_char& Serialiser_difference_type() const {return alx_noeud_scene::Serialiser_difference_type( alx_noeud_scene::Serialiser_difference_contenu() );}
   virtual const alx_chaine_char& Serialiser_difference_type(const alx_chaine_char &diff_contenu) const;
   virtual const alx_chaine_char& Serialiser_difference_fils() const;
   inline virtual const alx_chaine_char& Serialiser_type() const {return alx_noeud_scene::Serialiser_type( alx_noeud_scene::Serialiser_contenu() );}
   virtual const alx_chaine_char& Serialiser_type(const alx_chaine_char &contenu) const;
   virtual const alx_chaine_char& Serialiser_contenu() const;

   inline virtual const alx_chaine_char& Serialisation_differences_type()    const {return serialisation_differences_type;}
   inline virtual const alx_chaine_char& Serialisation_differences_contenu() const {return serialisation_differences_contenu;}

   virtual const alx_chaine_char& Serialisation_contenu() const {return serialisation_contenu;}
   virtual const alx_chaine_char& Serialisation_type   () const {return serialisation_type;}
   virtual const alx_chaine_char& Serialiser_fils      () const;
   inline virtual void Deserialiser(const alx_chaine_char &txt) {unsigned int pos=0; alx_noeud_scene::Deserialiser(txt, pos);}
   virtual void Deserialiser(const alx_chaine_char &txt, unsigned int &pos);
   inline virtual void Deserialiser_type(const alx_chaine_char &txt) {unsigned int pos=0; this->Deserialiser_type(txt, pos);}
   virtual void Deserialiser_type(const alx_chaine_char &txt, unsigned int &pos);
   const bool Fils_a_subi_seri_diff()   const {return fils_a_subi_seri_diff;}
   const bool Fils_a_subi_deseri_diff() const {return fils_a_subi_deseri_diff;}

  // Faire un rendu destiné à être copier dans une texture:
   virtual void PreRendre_fils();
   virtual void PreRendre();

  // Renvoi un pointeur sur le modèle physique qui contient la primitive, NULL si aucun.
   inline const bool Est_modifier_action_type() const {return M_modif_action_transmise&Marqueur_attributs();}
   inline const int  Modification_action_type() const {return action_transmise;}
   inline void Modifier_action_click(const bool m, const int action=0) {Marqueur_attributs(M_modif_action_transmise, m);
                                                                        action_transmise      = action;}
   virtual info_du_contenant* Contenu_dans_fils(alx_point2D &pt, int action);
   virtual info_du_contenant* Contient(const alx_point2D &pt, int action);
   virtual info_du_contenant* Resultat_contient(info_du_contenant *rep);
   virtual info_du_contenant* Noeud_contient(const alx_point2D &pt, int action);
   inline void Contenu_dans_fils_ssi_contenu_dans_noeud(const bool b)        {Marqueur_attributs(M_contenu_dans_fils_ssi_contenu_dans_noeud, b);}
   inline const bool Contenu_dans_fils_ssi_contenu_dans_noeud()        const {return M_contenu_dans_fils_ssi_contenu_dans_noeud&Marqueur_attributs();}
   inline void Contenu_dans_fils_ssi_pas_contenu_dans_noeud(const bool b) {Marqueur_attributs(M_contenu_dans_fils_ssi_ne_contenu_dans_pas_noeud, b);}
   inline const bool Contenu_dans_fils_ssi_pas_contenu_dans_noeud() const {return M_contenu_dans_fils_ssi_ne_contenu_dans_pas_noeud&Marqueur_attributs();}

  // Intercalage et compagnie
   void Intercaler_noeud(alx_noeud_scene *noeud);
   void Intercaler_noeud_entre(alx_noeud_scene *noeud_pere, alx_noeud_scene *noeud_fils);
   void Retirer_et_relier();

  // Retrouver la suite des repères qui ont menées jusqu'à ce noeud.
   void Transformer_point_depuis_racine(alx_point2D &pt);
   void Transformer_point_depuis_racine(alx_point2D &pt, alx_noeud_scene *racine);
   void Transformer_point_depuis_racine_sauf_noeud_courant(alx_point2D &pt);
   void Transformer_point_depuis_racine_sauf_noeud_courant(alx_point2D &pt, alx_noeud_scene *racine);
   void Donner_liste_repere(alx_liste<alx_repere2D*> &L_repere);
   void Donner_liste_repere(alx_liste<alx_repere2D*> &L_repere, alx_noeud_scene *racine);
   void Donner_liste_repere_moins_courant(alx_liste<alx_repere2D*> &L_repere);
   void Maj_etirement_absolu();
  //__________________________________________________________________
   inline void Ne_pas_pre_rendre(const bool b) {Marqueur_attributs(M_ne_pas_pre_rendre, b);}
   inline const bool Ne_pas_pre_rendre() const {return M_ne_pas_pre_rendre&Marqueur_attributs();}

   inline void Position_des_fils_changeable(const bool b) {Marqueur_attributs(M_position_des_fils_changeable, b);}
   inline const bool Position_des_fils_changeable() const {return M_position_des_fils_changeable&Marqueur_attributs();}
   inline void Position_dans_liste_des_fils_changeable(const bool b) {Marqueur_attributs(M_position_dans_liste_des_fils_changeable, b);}
   inline const bool Position_dans_liste_des_fils_changeable() const {return M_position_dans_liste_des_fils_changeable&Marqueur_attributs();}

   inline void Appartenance_deja_testee(const bool b) {Marqueur_attributs(M_appartenance_deja_testee, b);}
   inline const bool Appartenance_deja_testee() const {return M_appartenance_deja_testee&Marqueur_attributs();}

   inline void Ne_pas_pre_rendre_fils(const bool b) {Marqueur_attributs(M_ne_pas_pre_rendre_fils, b);}
   inline const bool Ne_pas_pre_rendre_fils() const {return M_ne_pas_pre_rendre_fils&Marqueur_attributs();}
   inline void Ne_pas_afficher_fils(const bool b)   {Marqueur_attributs(M_ne_pas_afficher_fils, b);}
   inline const bool Ne_pas_afficher_fils() const   {return M_ne_pas_afficher_fils&Marqueur_attributs();}

   inline void Compter_affichage(const bool b) {Marqueur_attributs(M_compter_affichage, b);}
   inline const bool Compter_affichage() const {return M_compter_affichage&Marqueur_attributs();}
   inline void Compter_pre_rendu(const bool b) {Marqueur_attributs(M_compter_pre_rendu, b);}
   inline const bool Compter_pre_rendu() const {return M_compter_pre_rendu&Marqueur_attributs();}

   inline void Nb_max_pre_rendu(const unsigned int n) {nb_max_pre_rendu = n;}
   inline const unsigned int Nb_max_pre_rendu() const {return nb_max_pre_rendu;}
   inline void Nb_pre_rendu(const unsigned int n) {nb_pre_rendu = n;}
   inline const unsigned int Nb_pre_rendu() const {return nb_pre_rendu;}
   inline void Nb_max_affichage(const unsigned int n) {nb_max_affichage = n;}
   inline const unsigned int Nb_max_affichage() const {return nb_max_affichage;}
   inline void Nb_affichage(const unsigned int n) {nb_affichage = n;}
   inline const unsigned int Nb_affichage() const {return nb_affichage;}

   inline void Afficher_noeud(const bool b)       {if( Afficher_noeud() != b)
                                                    {Marqueur_attributs(M_afficher, b);
                                                     A_changer(true);
                                                    }
                                                  }
   inline const bool Afficher_noeud() const       {return M_afficher&Marqueur_attributs();}
   inline void Gerer_contacts(const bool b) {Marqueur_attributs(M_gerer_contacts, b);}
   inline const bool Gerer_contacts() const {return M_gerer_contacts&Marqueur_attributs();}

   inline void Apparaitre(const bool b)
    {Afficher_noeud(b);
     Gerer_contacts(b);}

  // Surcharge de la mise à jour de l'étirement courant par prise en compte de l'étirement du père
   virtual void maj_etirement_courant() const;
  // Les surcharges d'opérateurs unaires.
   alx_noeud_scene &operator+=(const alx_liste<alx_arbre<alx_noeud_scene*>*> &);  // Surcharge du  +=
   alx_noeud_scene &operator+=(alx_noeud_scene *);  // Surcharge du  +=
};

typedef alx_noeud_scene* P_alx_noeud_scene;

alx_noeud_scene* Repere_vers_noeud(alx_repere2D *r) {return (alx_noeud_scene*)r;}
alx_noeud_scene* Void_vers_noeud  (void *p)         {return (alx_noeud_scene*)p;}

alx_liste<alx_noeud_scene*>* L_repere_vers_L_noeud_scene(const alx_liste<alx_repere2D*> &L_reperes)
{alx_liste<alx_noeud_scene*> *L_noeuds = new alx_liste<alx_noeud_scene*>();
 alx_element_liste<alx_repere2D*> *it     = L_reperes.Premier()
                                , *it_fin = L_reperes.Fin();
 for(; it!=it_fin; it=it->svt)
  {L_noeuds->Ajouter_a_la_fin( (alx_noeud_scene*)it->E() );
  }
 return L_noeuds;
}

void Reconstruire_rappels(info_du_contenant *rep, const alx_liste<alx_repere2D*> &L_rep);

void Initialisation_etats_puits();
alx_model_simulation_physique1* Mp_puit();
alx_noeud_scene* Noeud_puit();
info_du_contenant* Rep_puit();

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
void Ajouter_noeud_en_debut_de_liste_rep( alx_liste<alx_repere2D*> &L_rep
                                        , alx_noeud_scene *noeud );

//______________________________________________________________________________
void Ajouter_noeud_en_fin_de_liste_rep( alx_liste<alx_repere2D*> &L_rep
                                        , alx_noeud_scene *noeud );

//______________________________________________________________________________
void Lister_partie_index( const alx_liste<alx_repere2D*>         &L_rep
                        , const unsigned int prem
                        , const unsigned int der
                        , alx_liste<alx_repere2D*> &L_res);

//______________________________________________________________________________
void Lister_partie_noeud( const alx_liste<alx_repere2D*>         &L_rep
                        , alx_noeud_scene* premier
                        , alx_noeud_scene* dernier
                        , alx_liste<alx_repere2D*> &L_res);

bool Prise_en_compte_des_reperes_ou_pas(alx_noeud_scene **e, void *info_comp);

// Gestion du stencil

// Gestion textures
#include "sdl_opengl/alx_noeud_zone_rendu_sdl_opengl.h"


#endif

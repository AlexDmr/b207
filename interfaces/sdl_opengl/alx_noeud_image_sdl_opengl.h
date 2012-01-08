#ifndef __ALX_NOEUD_IMAGE_SDL_OPENGL_H__
#define __ALX_NOEUD_IMAGE_SDL_OPENGL_H__

#include "..\alx_noeud_scene.h"
#include "..\..\opengl\alx_image_opengl.h"

#include "../fonctions_serialisations.cpp"

inline void* AlphaNum_vers_Void(const char *ad) {return (void*)strtol(ad, (char **)NULL, 16);}

class alx_noeud_image_sdl_opengl : public alx_noeud_scene, public alx_image_opengl
{private:
   Liste_de_rappel L_rap_maj_Info_texture, L_rap_fin_maj_data_texture;
   mutable alx_chaine_char cc_tmp;
   CFSD<alx_noeud_image_sdl_opengl> classe_serialisation;
   void init();
   void init_ptr();
   mutable alx_chaine_char serialisation_type_img   , serialisation_differences_type_img
                         , serialisation_contenu_img, serialisation_differences_contenu_img
                         , nouv_serialisation_type_img , seri_diff_type_img;

 protected:
   alx_point2D A_pt_tmp;

 public :
   alx_noeud_image_sdl_opengl(const bool mipmap = false);
   alx_noeud_image_sdl_opengl(const char *n, const bool mipmap = false);
   alx_noeud_image_sdl_opengl(const alx_image_opengl &img);
   //alx_noeud_image_sdl_opengl(alx_image_opengl *image_gl);
   alx_noeud_image_sdl_opengl(const INFOS_TEXTURE &info_text, const double taille_x, const double taille_y);
   ~alx_noeud_image_sdl_opengl();

   inline const alx_chaine_char& Nom() const {return alx_noeud_scene::Nom();}
   inline void Nom(const alx_chaine_char &n) {alx_noeud_scene::Nom(n);}
   inline const int Nb_octets_par_pixels_texture() const {return alx_image_opengl::Nb_octets_par_pixels_texture();}
   inline void Nb_octets_par_pixels_texture(const int v) {alx_image_opengl::Nb_octets_par_pixels_texture(v);}
   inline const int Ordre_couleur_texture()        const {return alx_image_opengl::Ordre_couleur_texture();}
   inline void Ordre_couleur_texture       (const int v) {alx_image_opengl::Ordre_couleur_texture(v);}

   
   virtual const char* Real_class_cmd() {return "Void_vers_image";}
   inline void maj(const int tx, const int ty, const int ordre_couleur, const int nb_octet_par_pix, const char *buffer) {alx_image_opengl::maj(tx, ty, ordre_couleur, nb_octet_par_pix, buffer); L_rap_fin_maj_data_texture.Rappeler(this);}
   inline void maj_raw(const int tx, const int ty, const int ordre_couleur, const int nb_octet_par_pix, void *ad)       {maj(tx, ty, ordre_couleur, nb_octet_par_pix, (char*)ad)               ; L_rap_fin_maj_data_texture.Rappeler(this);}
   inline const bool Threaded_maj_raw_with_transfo( const int tx, const int ty
                                   , const int ordre_couleur_src, const int nb_octet_par_pix_src
                                   , const int ordre_couleur_tgt, const int nb_octet_par_pix_tgt, void *ad)          {return Threaded_maj(tx, ty, ordre_couleur_src, nb_octet_par_pix_src, ordre_couleur_tgt, nb_octet_par_pix_tgt, (char*)ad); L_rap_fin_maj_data_texture.Rappeler(this);}

   inline void maj_raw_with_transfo( const int tx, const int ty
                                   , const int ordre_couleur_src, const int nb_octet_par_pix_src
                                   , const int ordre_couleur_tgt, const int nb_octet_par_pix_tgt, void *ad)          {maj_transfo(tx, ty, ordre_couleur_src, nb_octet_par_pix_src, ordre_couleur_tgt, nb_octet_par_pix_tgt, (char*)ad); L_rap_fin_maj_data_texture.Rappeler(this);}
   inline void maj(const char *n)                                                                                       {alx_image_opengl::maj(n);                                             ; L_rap_fin_maj_data_texture.Rappeler(this);}
   inline void maj(const INFOS_TEXTURE &info)                                                                           {alx_image_opengl::maj(info);                                          ; L_rap_fin_maj_data_texture.Rappeler(this);}
   inline void maj(const alx_image_opengl &img)                                                                         {alx_image_opengl::maj(img);                                           ; L_rap_fin_maj_data_texture.Rappeler(this);}
   inline void maj(const alx_image_32 *img_32)                                                                          {alx_image_opengl::maj(img_32);                                        ; L_rap_fin_maj_data_texture.Rappeler(this);}

   inline void Copier(const alx_noeud_image_sdl_opengl &i) {alx_image_opengl::maj(i);}

   inline const bool Sauver_dans_fichier(const char *nf)                                                                {return alx_image_opengl::Sauver_dans_fichier(nf);}
  //____________________________________________________________________________
   inline virtual void Calculer_boites() {alx_noeud_scene::Calculer_boite_fils(); alx_noeud_image_sdl_opengl::Calculer_boite_noeud();}
   virtual void Calculer_boite_noeud();

  //____________________________________________________________________________
   inline void     abonner_a_fin_maj_data_texture(alx_methode_rappel *m) {L_rap_fin_maj_data_texture.Ajouter_methode_a_la_fin(m);}
   inline void desabonner_de_fin_maj_data_texture(alx_methode_rappel *m) {L_rap_fin_maj_data_texture.Retirer_methode         (m);}

  //____________________________________________________________________________
   inline void     abonner_a_maj_Info_texture(alx_methode_rappel *m) {L_rap_maj_Info_texture.Ajouter_methode_a_la_fin(m);}
   inline void desabonner_de_maj_Info_texture(alx_methode_rappel *m) {L_rap_maj_Info_texture.Retirer_methode         (m);}

  //____________________________________________________________________________
   void   Lock_mutex_tempon() {alx_image_32::Lock_mutex_tempon  ();}
   void UnLock_mutex_tempon() {alx_image_32::UnLock_mutex_tempon();}

  //____________________________________________________________________________
   inline const bool Inverser_x() const {return alx_image_opengl::Inverser_x();}
   inline const bool Inverser_y() const {return alx_image_opengl::Inverser_y();}
   inline void Inverser_x(bool e) {alx_image_opengl::Inverser_x(e);}
   inline void Inverser_y(bool e) {alx_image_opengl::Inverser_y(e);}

  //____________________________________________________________________________
   virtual void PreRendre();
   void Dessin_noeud();
   virtual info_du_contenant* Noeud_contient(const alx_point2D &pt, int action); // Renvoi un pointeur sur le modèle physique qui contient la primitive, NULL si aucun.

   inline alx_image_opengl& Img() {return *((alx_image_opengl*)this);}
   inline INFOS_TEXTURE* Ptr_Info_texture() {return alx_image_opengl::Ptr_Info_texture();}
   inline INFOS_TEXTURE& Info_texture() {return alx_image_opengl::Info_texture();}
   inline const INFOS_TEXTURE& Info_texture_const() const {return alx_image_opengl::Info_texture_const();}
   inline const void* Tempon_const() const {return TEMPON_const();}
   inline void* Tempon() {return TEMPON();}
   inline const unsigned int Taille_Tempon() {return alx_image_opengl::Taille_Tempon();}
   inline void Nb_octets_par_pixel(const int n)     {alx_image_opengl::Nb_octets_par_pixel( (unsigned int)n);}
   inline const int Nb_octets_par_pixel()     const {return (int)alx_image_opengl::Nb_octets_par_pixel();}
   inline const int Ordonnancement_couleurs() const {return alx_image_opengl::Ordonnancement_couleurs();}
   inline void Ordonnancement_couleurs(const int o) {alx_image_opengl::Ordonnancement_couleurs(o);}
   inline const double Lg() const {return alx_image_opengl::L();}
   inline const double Ht() const {return alx_image_opengl::H();}
   inline void Lg(const double v) {alx_image_opengl::L((int)v);}
   inline void Ht(const double v) {alx_image_opengl::H((int)v);}

 // Les surcharges pour la sérialisation
  // La différence _____________________________________________________________
  inline virtual const alx_chaine_char& Serialiser_difference_type() const {return Serialiser_difference_type( Serialiser_difference_contenu() );}
  virtual const alx_chaine_char& Serialiser_difference_type(const alx_chaine_char &diff_contenu) const;
  virtual const alx_chaine_char& Serialiser_difference_contenu() const;
  inline virtual const alx_chaine_char& Serialisation_differences_type()    const {return serialisation_differences_type_img;}
  inline virtual const alx_chaine_char& Serialisation_differences_contenu() const {return serialisation_differences_contenu_img;}
  // La sérialisation __________________________________________________________
  virtual const alx_chaine_char& Serialiser_type(const alx_chaine_char &contenu) const;
  virtual const alx_chaine_char& Serialiser_contenu() const;
  virtual const alx_chaine_char& Serialisation_contenu() const {return serialisation_contenu_img;}
  inline virtual const alx_chaine_char& Serialiser_type() const {return Serialiser_type( Serialiser_contenu() );}
  virtual const alx_chaine_char& Serialisation_type   () const {return serialisation_type_img;}
  // La désérialisation ________________________________________________________
  inline virtual void Deserialiser_type(const alx_chaine_char &txt) {unsigned int pos=0; Deserialiser_type(txt, pos);}
  virtual void Deserialiser_type(const alx_chaine_char &txt, unsigned int &pos);
  inline virtual void Deserialiser(const alx_chaine_char &txt) {unsigned int pos=0; Deserialiser(txt, pos);}
  virtual void Deserialiser(const alx_chaine_char &txt, unsigned int &pos);

  void alx_noeud_image_sdl_opengl::Deseri_info_texture(const alx_chaine_char &t);
  const bool alx_noeud_image_sdl_opengl::Seri_diff_info_texture_a_ete_change(const alx_chaine_char **) const;
};

typedef alx_noeud_image_sdl_opengl* P_alx_noeud_image_sdl_opengl;
inline alx_noeud_image_sdl_opengl* Void_vers_image(void *p) {return (alx_noeud_image_sdl_opengl*)p;}

#endif

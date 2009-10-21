#ifndef __ALX_VISU_IMAGE_CAMNOTE_CPP_H__*
#define __ALX_VISU_IMAGE_CAMNOTE_CPP_H__*

// Evennements :
// Changement d'état d'activation des pixels mirroirs
//   P : Un ptr sur booléen représentant le nouvel état
// Changement de valeur de l'intensité
//   P : Un pointeur vers un flotant
//______________________________________________________________________________
// Rationnalisation :
// 1 : Changement des pixels mirroirs :
//   0 : Changement d'état d'activation
//     X : Un ptr sur booléen, indicant l'état
//   1 : Changement de valeur de l'intensité
//     X : un pointeur sur un flotant ontenant la nouvelle valeur

class alx_visu_image_camnote_cpp : public alx_visu_image_cpp
{
 private:
   Liste_de_rappel L_rappel_activation_pix_mirroir
                 , L_rappel_intensite_pix_mirroir;
   bool  activation_pix_mirroirs;
   float intensite_pixels_miroirs;

   inline void Changement_activation_pix_mirroirs()
    {L_rappel_activation_pix_mirroir.Rappeler(&activer_pix_mirroir);
     alx_classe_base_model::nb_infos_comp = 3;
     alx_classe_base_model::infos_comp[0] = 1;
     alx_classe_base_model::infos_comp[1] = 0;
     alx_classe_base_model::infos_comp[2] = (int)(void*)(&activer_pix_mirroir);
     alx_classe_base_model::Oyez_oyez();}
   inline void Changement_intensite_pix_mirroirs()
    {L_rappel_intensite_pix_mirroir.Rappeler(&intensite_pixels_miroirs);
     alx_classe_base_model::nb_infos_comp = 3;
     alx_classe_base_model::infos_comp[0] = 1;
     alx_classe_base_model::infos_comp[1] = 1;
     alx_classe_base_model::infos_comp[2] = (int)(void*)(&intensite_pixels_miroirs);
     alx_classe_base_model::Oyez_oyez();}
   
 public:
   alx_visu_image_camnote_cpp() : alx_visu_image_cpp() {}
   alx_visu_image_cpp( char *nom
                     , Environment *e
                     , alx_interpreteur_tcl  *interp ) : alx_visu_image_cpp(nom, e, interp) {}

   inline void abonner_a_activation_pix_mirroir(alx_methode_rappel *m) {L_rappel_activation_pix_mirroir.Ajouter_methode_a_la_fin(m);}
   inline void Activation_pix_mirroir(const bool b) {activer_pix_mirroir = b; Changement_activation_pix_mirroirs();}
   inline const bool Activation_pix_mirroir() const {return activer_pix_mirroir;}

   inline void abonner_a_intensite_pix_mirroir(alx_methode_rappel *m) {L_rappel_intensite_pix_mirroir.Ajouter_methode_a_la_fin(m);}
   inline void Intensite_pixels_miroirs(const float i)
    {intensite_pixels_miroirs = Amax( (float)0
                                , Amin((float)0, i) );
     Changement_intensite_pix_mirroirs();
    }
   inline const float Intensite_pixels_miroirs() {return intensite_pixels_miroirs;}
};

#endif
 
#include "alx_evennement_dispositif_saisie.h"

alx_evennement_dispositif_saisie::alx_evennement_dispositif_saisie( alx_evennement_dispositif_saisie *evt )
{maj(*evt);}

alx_evennement_dispositif_saisie::alx_evennement_dispositif_saisie( int id
                                                                  , alx_dispositif_saisie_type_evennement  t_evt
                                                                  , info_comp_evennement_dispositif_saisie comp)
{identifiant = id;
 type_evt    = t_evt;
 info_comp   = comp;}

 
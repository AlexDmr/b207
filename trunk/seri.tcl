source cool.tcl
set f $fen_editeur_tcl

# 1) Position 0
set txt [$f Serialiser_contenu]
set txt_type_tmp [$f Serialiser_type $txt]
set txt_type [$chaine_char $txt_type_tmp]
# 2) Position 1
$f Longueur_corp 640
$f Hauteur_corp  480
$f Mettre_a_jour
$f Origine 500 200
$f Rotation 0.5
$f Titre {Interpréteur TCL}
$f Nb_pixels_par_unite 0.5
$f Triangle_control 1
$f Couleur_fond 0 0 1 0.6
# On enregistre les différence entre la position 0 et la position 1
set t_diff_tmp [eval $f Serialiser_difference_type [$f Serialiser_difference_contenu]]
set t_diff [$chaine_char $t_diff_tmp]
# Position 2
$f Longueur_corp 500
$f Hauteur_corp  500
$f Mettre_a_jour
$f Origine 0 0
$f Rotation 0
$f Titre {Coucou ça change!}
$f Nb_pixels_par_unite 1
$f Triangle_control 0
$f Couleur_fond 0 0 0 0.6
return "A taper :\n\$f Deserialiser_type \$t_diff\n\$f Deserialiser_type \$txt_type";



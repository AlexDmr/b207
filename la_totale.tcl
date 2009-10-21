source groupe.tcl
source camnote_bigre.tcl
source loupe_ovale.tcl
source test_image.tcl

$noeud_partage Ajouter_fils $CamNote

proc etire_fond {} {}
$img_fond Origine 0 0
$img_fond Etirement 1.28 1.28

set titre {Interpréteur TCL de }
set titre $titre[$n_i_mere IP]
$fen_editeur_tcl Titre {Interpréteur TCL de}
$ftcn Couleur_fond 0 0 0 1

return {Taper "Positionner_partage x y {r 0} {ex 1} {ey 1}" pour positionner la caméra dans l'espace logique.}



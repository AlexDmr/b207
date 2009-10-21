source commandes_bigre.tcl

set ecran_x [$n_i_mere Largeur]
set ecran_y [$n_i_mere Hauteur]
set noeud_fen [$noeud_bigre]
set img_fond [$image_bigre "Desert sous la lune.jpg"]

$Noeud_scene Position_des_fils_changeable 0
$Noeud_scene Intercaler_noeud $noeud_fen
$Noeud_scene Ajouter_fils $img_fond
if {[expr [$n_i_mere Hauteur]>[$img_fond Ht]]} {
  $img_fond Etirement [expr [$n_i_mere Largeur]/[$img_fond Lg]] [expr [$n_i_mere Hauteur]/[$img_fond Ht]]}
$img_fond Lisser 1

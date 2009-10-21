source groupe.tcl
source loupe_ovale.tcl
set f_img [$fiche_o_bigre Image 512 384 $n_i_mere]
set fr_img [$fiche_bigre Image2 512 384 $n_i_mere]
set image [$image_bigre wow.bmp]
$f_img Ajouter_fils $image
$f_img Rendre_fond 0
$noeud_fen Ajouter_fils $fr_img
$fr_img Ajouter_fils $image
$fr_img Rendre_fond 0
$noeud_fen Ajouter_fils $f_img
$fr_img Ajouter_fils_au_debut $fzo
$Noeud_scene Retirer_fils $fzo
$fzo Retirer_fils $Noeud_scene
$fzo Ajouter_fils [$fr_img Noeud_repere_fils]
return {OK!}

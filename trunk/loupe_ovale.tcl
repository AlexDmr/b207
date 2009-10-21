set fzo [B_fiche_oz {Fiche de zoom ovale} 256 256 $n_i_mere]
$Noeud_scene Ajouter_fils_au_debut $fzo
$fzo Ajouter_fils $Noeud_scene
$fzo Origine [expr [N_i_mere Largeur]/2.0] [expr [N_i_mere Hauteur]/2.0]


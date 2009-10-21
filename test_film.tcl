if {[info exists noeud_partage]} {} else {
 source groupe.tcl
}

set nom_film "Metal_Dust_Trailer_small.mpeg"
set film [N_i_mere Ouvrir_film $nom_film]
set texture [N_i_mere Texture_film $film]

set poly_rect [B_polygone]
  $poly_rect Ajouter_contour [ProcRect 0 0 640 480]
  $poly_rect Info_texture    $texture

$noeud_partage Ajouter_fils_au_debut $poly_rect

proc Lire_film {f} {
 N_i_mere Jouer_film $f
 after 50 "Lire_film $f"
}

return "Lire_film $film"


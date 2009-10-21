set PI 3.14159265
set nb_elements 12

set tab_ovale_out [ProcOvale   0 0 100 100 128]
set tab_ovale_in  [ProcOvale   0 0 80  80  128]
set tab_ligne [B_TabDouble 6]
 $tab_ligne Ajouter 0   0  0
 $tab_ligne Ajouter 2 110 -3
 $tab_ligne Ajouter 4 110  3

# Les couleur pour les changement de saison
#   h : hivers...
set col_h "0    0    0.8  1"
set col_p "0.4  1    0.2  1"
set col_e "1    1    0    1"
set col_a "0.93 0.8  0.28 1"
set tab_col_hp [$TabDouble 16]
  eval "$tab_col_hp Ajouter 0  $col_p"
  eval "$tab_col_hp Ajouter 4  $col_p"
  eval "$tab_col_hp Ajouter 8  $col_h"
  eval "$tab_col_hp Ajouter 12 $col_h"
set tab_col_pe [$TabDouble 16]
  eval "$tab_col_pe Ajouter 0  $col_e"
  eval "$tab_col_pe Ajouter 4  $col_e"
  eval "$tab_col_pe Ajouter 8  $col_p"
  eval "$tab_col_pe Ajouter 12 $col_p"
set tab_col_ea [$TabDouble 16]
  eval "$tab_col_ea Ajouter 0  $col_e"
  eval "$tab_col_ea Ajouter 4  $col_e"
  eval "$tab_col_ea Ajouter 8  $col_a"
  eval "$tab_col_ea Ajouter 12 $col_a"
set tab_col_ah [$TabDouble 16]
  eval "$tab_col_ah Ajouter 0  $col_a"
  eval "$tab_col_ah Ajouter 4  $col_a"
  eval "$tab_col_ah Ajouter 8  $col_h"
  eval "$tab_col_ah Ajouter 12 $col_h"

#________________________________________________________
set poly_centre [B_polygone]
  $poly_centre Ajouter_contour $tab_ovale_in

#________________________________________________________
set poly_cal [B_polygone]
  $poly_cal Ajouter_contour $tab_ovale_out
  $poly_cal Retirer_contour $tab_ovale_in

#________________________________________________________
set poly_ligne [B_polygone]
  $poly_ligne Ajouter_contour $tab_ligne

#________________________________________________________
puts "Generation de l'étoile"
set poly_etoile [B_polygone]
  for {set i 0} {$i < $nb_elements} {incr i} {
   $poly_ligne  Rotation [expr $i*2*$PI/$nb_elements]
   $poly_etoile Union    $poly_ligne
  }

#________________________________________________________
set noeud_cal [B_noeud]
$noeud_fen Ajouter_fils_au_debut $noeud_cal
  $noeud_cal Ajouter_fils_au_debut $poly_centre
  $noeud_cal Ajouter_fils_au_debut $poly_etoile

#________________________________________________________
#_______________ Operations d'assemblages _______________
#________________________________________________________
puts "Operations d'assemblages"
set poly_etoile_tmp [B_polygone]
  $poly_etoile_tmp Union $poly_etoile
$poly_etoile Intersection $poly_cal
$poly_cal    Difference   $poly_etoile_tmp
# Découpage du calendrier en 4
set poly_decoupe [B_polygone]
  set L_moins [list 0 0 105 0 105 105 0 105]
  set L_plus  [list -110 -100 110 -100 110 110 -110 110]
  $poly_decoupe Ajouter_contour [ProcTabDouble $L_plus ]
  $poly_decoupe Retirer_contour [ProcTabDouble $L_moins]

set poly_cal_hp [B_polygone]
  $poly_cal_hp Union $poly_cal
  $poly_decoupe Rotation 0
  $poly_cal_hp Difference $poly_decoupe
set poly_cal_pe [B_polygone]
  $poly_cal_pe Union $poly_cal
  $poly_decoupe Rotation [expr -$PI/2]
  $poly_cal_pe Difference $poly_decoupe
set poly_cal_ea [B_polygone]
  $poly_cal_ea Union $poly_cal
  $poly_decoupe Rotation $PI
  $poly_cal_ea Difference $poly_decoupe
set poly_cal_ah [B_polygone]
  $poly_cal_ah Union $poly_cal
  $poly_decoupe Rotation [expr $PI/2]
  $poly_cal_ah Difference $poly_decoupe

$noeud_cal Ajouter_fils_au_debut $poly_cal_hp
$noeud_cal Ajouter_fils_au_debut $poly_cal_pe
$noeud_cal Ajouter_fils_au_debut $poly_cal_ea
$noeud_cal Ajouter_fils_au_debut $poly_cal_ah
#________________________________________________________
#______________ Operations de colorations _______________
#________________________________________________________
puts "Operations de colorations"
$poly_cal_hp Coords_couleurs [$tab_col_hp Tab]; $poly_cal_hp maj
$poly_cal_pe Coords_couleurs [$tab_col_pe Tab]; $poly_cal_pe maj
$poly_cal_ea Coords_couleurs [$tab_col_ea Tab]; $poly_cal_ea maj
$poly_cal_ah Coords_couleurs [$tab_col_ah Tab]; $poly_cal_ah maj

$poly_etoile Couleur 0.4 0.8 0.6 1
$poly_centre Couleur 0 0 1 1

#________________________________________________________
#_______________ Ajout des manipulations ________________
#________________________________________________________
B_contact contact_C_year "$noeud_cal 0" -pt_trans 0 0 -pt_rot 0 0 \
                                        -add "$poly_centre 1"     \
                                        -add "$poly_etoile 2"     \
                                        -add "$poly_cal_hp 3"     \
                                        -add "$poly_cal_pe 3"     \
                                        -add "$poly_cal_ea 3"     \
                                        -add "$poly_cal_ah 3"


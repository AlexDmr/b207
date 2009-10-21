set PI 3.14159265
set nb_elements 12

set tab_ovale_out [ProcOvale   0 0 100 100 128]
set tab_ovale_in  [ProcOvale   0 0 80  80  128]
set tab_ligne [B_TabDouble 8]
 $tab_ligne Ajouter 0   0 -5
 $tab_ligne Ajouter 2 100 -5
 $tab_ligne Ajouter 4 100  5
 $tab_ligne Ajouter 6   0  5

set tab_col [$TabDouble 16]
  $tab_col Ajouter 0  1 1 0   1
  $tab_col Ajouter 4  0 0 0.8 1
  $tab_col Ajouter 8  0 0 0.8 1
  $tab_col Ajouter 12 1 1 0   1
  
#________________________________________________________
set poly_centre [B_polygone]
  $poly_centre Ajouter_contour [$tab_ovale_in  Tab]

#________________________________________________________
set poly_cal [B_polygone]
  $poly_cal Ajouter_contour [$tab_ovale_out Tab]
  $poly_cal Retirer_contour [$tab_ovale_in  Tab]

#________________________________________________________
set poly_ligne [B_polygone]
  $poly_ligne Ajouter_contour [$tab_ligne Tab]

#________________________________________________________
set poly_etoile [B_polygone]
  for {set i 0} {$i < $nb_elements} {incr i} {
   $poly_ligne  Rotation [expr 2*$PI/$nb_elements]
   $poly_etoile Union    $poly_ligne
  }

#________________________________________________________
$noeud_fen Ajouter_fils_au_debut $poly_cal
  $poly_cal Ajouter_fils_au_debut $poly_etoile
  $poly_cal Ajouter_fils_au_debut $poly_centre

#________________________________________________________
#_______________ Operations d'assemblages _______________
#________________________________________________________
$poly_etoile Intersection $poly_cal    $poly_cal
$poly_etoile Difference   $poly_centre $poly_cal

$poly_cal    Difference   $poly_centre $poly_cal
$poly_cal    Difference   $poly_etoile $poly_cal

#________________________________________________________
#______________ Operations de colorations _______________
#________________________________________________________
$poly_cal Coords_couleurs [$tab_col Tab]
$poly_cal maj

$poly_etoile Couleur 0.6 0.8 0.4 1

$poly_centre Couleur 0 0 1 1

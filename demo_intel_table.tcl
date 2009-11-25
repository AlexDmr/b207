source test_menu_main.tcl
set zone_dessin [A_drawer get_canvas]; $zone_dessin Vider_peres
set server      [new_Texture_Server_TCP 8000 [N_i_mere]]

#___________________________________________________________________________________________________________________________________________
proc Recalibrating_laptop_position {poly_laptop noeud_pivot} {
  if {[catch {$noeud_pivot maj [expr -[$poly_laptop Px]] [expr -[$poly_laptop Py]] \
                   [expr -[$poly_laptop R]] \
                   [expr 1.0/[$poly_laptop Ex]] [expr 1.0/[$poly_laptop Ey]]} err]} {puts "Error : $err"}
}


#___________________________________________________________________________________________________________________________________________
proc Resize_laptop {poly tx ty} {
  set ex [expr $tx / 368.0]
  set ey [expr $ty / 231.0]
  $poly Vider
  $poly Ajouter_contour [ProcRect [expr -12*$ex] [expr -26*$ey] [expr 380*$ex] [expr (278-26)*$ey]]
  $poly Retirer_contour [ProcRect 0 0 [expr 368*$ex] [expr (278-26-21)*$ey]]
}

#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
$noeud_partage Position_des_fils_changeable 0

set poly_laptop [B_polygone]
  Resize_laptop $poly_laptop 1920 1150
  $poly_laptop Etirement 0.2 0.2
  $noeud_partage Ajouter_fils $poly_laptop
  B_Raise $poly_laptop
  B_contact ctc_laptop "$poly_laptop 11"
  
  set noeud_pivot [B_noeud]
    $noeud_pivot Ordre_rendu 1
    $poly_laptop Ajouter_fils $noeud_pivot
    $noeud_pivot Ajouter_fils $zone_dessin
    $poly_laptop Noeud_puis_fils 0
  
  set rappel_laptop_moved [B_rappel [Interp_TCL] "Recalibrating_laptop_position $poly_laptop $noeud_pivot"]
  ctc_laptop abonner $rappel_laptop_moved
  Recalibrating_laptop_position $poly_laptop $noeud_pivot
  
N_i_mere Se_rajouter_a 1 $poly_laptop 0 0

$fen_editeur_tcl Vider_peres
B_Raise $n_root

$poly_pour_main Etirement 1 1
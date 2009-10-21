if {[info exists noeud_partage]} {} else {source groupe.tcl}

set f_constr [B_fiche "Assemblage" 512 384 $n_i_mere]
  $noeud_partage Ajouter_fils $f_constr

set noeud_appli      [B_noeud]
  $noeud_appli Position_des_fils_changeable 0
  $f_constr Ajouter_fils $noeud_appli

set noeud_piece [B_noeud]
  $noeud_appli Ajouter_fils_au_debut $noeud_piece
set noeud_assemblage [B_noeud]
  $noeud_appli Ajouter_fils_au_debut $noeud_assemblage

set detour_zone  [ProcTabDouble {0 0 512 0 512 150   0 150}]
set detour_oval  [ProcOvale 256 150 257 32 64]
set poly_zone_assemblage [B_polygone]
  $poly_zone_assemblage Ajouter_contour $detour_zone
  $poly_zone_assemblage Retirer_contour $detour_oval
  $noeud_appli Ajouter_fils $poly_zone_assemblage

set detour_piece [ProcTabDouble {0 0 100 0 150 50  100 100 0 100}]
set poly_piece  [B_polygone]
  $poly_piece Ajouter_contour $detour_piece
  $noeud_piece Ajouter_fils $poly_piece

# Manage the piece when it is clicked
set n_prev $noeud_piece
set nb_n 1
set rappel_piece_origine [B_rappel $interp_tcl "set n \[B_noeud\]; \
                                                $noeud_assemblage Ajouter_fils_au_debut \$n; \
                                                \$n Ajouter_fils_au_debut $poly_piece; \
                                                B_contact cont_\$nb_n \"\$n 0\" -add \"$poly_piece 1\" -pt_trans 50 50 -pt_rot 50 50 \n
                                                incr nb_n; \
                                               "]
$noeud_piece abonner_a_detection_pointeur [$rappel_piece_origine Rappel] 1



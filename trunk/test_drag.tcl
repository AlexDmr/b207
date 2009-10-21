if {[info exists noeud_partage]} {} else {source groupe.tcl}
proc etire_fond args {}
N_i_mere Afficher_souris

set poly_drag [B_polygone]
  $noeud_partage Ajouter_fils_au_debut $poly_drag
  $poly_drag Ajouter_contour [ProcOvale 0 0 100 200 60]
  Drag_nodes $poly_drag 1 "puts START" "puts END"

  B_contact ctc_poly_drag "$poly_drag 1"
  
set poly_drop [B_polygone]
  $noeud_partage Ajouter_fils $poly_drop
  $poly_drop Ajouter_contour [ProcOvale 0 0 400 400 60]
  B_configure $poly_drop -Origine 400 400 -Couleur 0 1 1 1
  Drop_zones alex [list [list $poly_drop "expr 1" Afficher_ecran "puts ENTER" "puts EXIT"]]

set poly_drop_in [B_polygone]
  $poly_drop Ajouter_fils $poly_drop_in
  $poly_drop_in Ajouter_contour [ProcOvale 0 0 100 100 60]
  B_configure $poly_drop_in -Couleur 1 0 0 1

  
proc Afficher_ecran {n_zone infos} {
 puts "Afficher_ecran\n  n_zone : $n_zone\n  infos : $infos\n    ptr : [$infos Ptr]\n    NOEUD : [$infos NOEUD]"
}

proc Add_element_to_zone {n_zone infos} {
 set ptr [$infos Ptr]
 puts "Add_element_to_zone\n  n_zone : $n_zone\n  infos : $infos\n    ptr : [$infos Ptr]\n    NOEUD : [$infos NOEUD]"
 set x [$infos X_au_contact]; set y [$infos Y_au_contact]
 
 set n_dragged [$ptr Val_MetaData Dragging]
 
 $n_zone Ajouter_fils_au_debut $n_dragged
   $n_dragged Origine $x $y
}

#_______________________________________________________________________________________________________________________________________
#_______________________________________________________________________________________________________________________________________
#_______________________________________________________________________________________________________________________________________
proc Add_CometRepresentation_to_node {n_zone infos} {
 set t0 [clock clicks -milliseconds]
 set ptr [$infos Ptr]
# puts "Add_element_to_zone\n  n_zone : $n_zone\n  infos : $infos\n    ptr : [$infos Ptr]\n    NOEUD : [$infos NOEUD]"
 set x [$infos X_au_contact]; set y [$infos Y_au_contact]
 
 set n_dragged [$ptr Val_MetaData Dragging]
   set PM [$n_dragged Val_MetaData CometPM]
   set representation_PM [CSS++ $PM "#$PM <--< >>!Comet_RepresentationComet/>> Comet_RepresentationComet"]
   set PM_drop_zone [$n_zone Val_MetaData CometPM]
 puts "Dragged node:\n  - $n_dragged\n  - PM : $PM\n  - classes : [gmlObject info classes $PM]\n  - MetaData : [$n_dragged T_MetaData]\n  - representation_PM : $representation_PM\n  - PM_drop_zone : $PM_drop_zone"
 if {[string equal $representation_PM ""]} {return}
 set t1 [clock clicks -milliseconds]
 set generated_COMET [$representation_PM Instanciate_a_complete_COMET]
 set t2 [clock clicks -milliseconds]
# Plug the generated COMET under the COMET represented by the zone
 puts "[$PM_drop_zone get_LC] Add_daughters_R $generated_COMET"
 
 [$PM_drop_zone get_LC] Add_daughters_R $generated_COMET
 set t3 [clock clicks -milliseconds]
 set PM_dragged [CSS++ $PM_drop_zone "#$PM_drop_zone $generated_COMET"]
 puts "Position of the B207 PM ($PM_dragged) corresponding dragged node [$representation_PM get_RepresentedComet]"
 set prim [$PM_dragged get_prim_handle] 
 if {[string equal $prim {}]} {return}
   set n_placement [[$ptr Noeud_representation] Val_MetaData Noeud_placement_drag]
   $prim Calculer_boites
   set box [$prim Boite_noeud_et_fils]
   $prim Origine [expr $x-[$box Cx]] [expr $y-[$box Cy]]
   $PM_dragged Raise
 set t4 [clock clicks -milliseconds]
 for {set i 1} {$i<=4} {incr i} {
   puts "Phase $i : [expr \$t$i - \$t[expr $i-1]]"
  }
}

#_______________________________________________________________________________________________________________________________________
proc Push_PM_Instance {infos} {
 set ptr [$infos Ptr]
 puts "Push_PM_Instance $infos $ptr"
 set n_dragged [$ptr Val_MetaData Dragging]
 if {[string equal $n_dragged {}]} {return}
 set PM [$n_dragged Val_MetaData CometPM]
   if {[string equal $PM {}]} {return}
 set PM_dragged [CSS++ $PM "#$PM <--< >>!Comet_RepresentationComet/>> Comet_RepresentationComet"]
   if {[string equal $PM_dragged {}]} {return}
 set L_prev_rep_dragged [$ptr Val_MetaData L_prev_rep_dragged]
   lappend L_prev_rep_dragged $n_dragged
   $ptr Add_MetaData L_prev_rep_dragged $L_prev_rep_dragged
 set GDD_node_of_represented_dragged [$PM_dragged get_RepresentedComet]
   dsl_q QUERY "?n : $GDD_node_of_represented_dragged : NODE()<-REL(type~=GDD_inheritance && type!=GDD_restriction)*<REL(type~=GDD_implementation)<-\$n(ptf == Ptf_BIGre)"
   set GDD_node_implem [lindex [lindex [dsl_q get_Result] 0] 1]
   if {[string equal $GDD_node_implem {}]} {puts "No B207 node for $GDD_node_of_represented_dragged"; return}
   set class_of_represented_dragged [lindex [$GDD_node_implem get_L_factories] 0]
   if {[string equal $class_of_represented_dragged {}]} {puts "No factories for $GDD_node_implem"; return}
 set new_PM_instance "${ptr}_instance_of_$class_of_represented_dragged"
 if {![gmlObject info exists object $new_PM_instance]} {
   $class_of_represented_dragged $new_PM_instance "$class_of_represented_dragged" "Generated for ptr ($ptr)"
  }
 set n_placement [[$ptr Noeud_representation] Val_MetaData Noeud_placement_drag]
   $n_placement Vider_fils
   set prim [$new_PM_instance get_prim_handle]
   $n_placement Ajouter_fils $prim
   $prim Calculer_boites
   set box [$prim Boite_noeud_et_fils]
   $prim Origine [expr -[$n_placement Px]-[$box Cx]] [expr -[$n_placement Py]-[$box Cy]]
}

#_______________________________________________________________________________________________________________________________________
proc Pop_PM_Instance {infos} {
 set ptr [$infos Ptr]
 puts "Pop_PM_Instance $infos $ptr"
 set n_dragged [$ptr Val_MetaData Dragging]
 if {[string equal $n_dragged {}]} {return}
 set L_prev_rep_dragged [$ptr Val_MetaData L_prev_rep_dragged]
   set top [lindex $L_prev_rep_dragged end]
   set L_prev_rep_dragged [lrange $L_prev_rep_dragged 0 end-1]
   $ptr Add_MetaData L_prev_rep_dragged $L_prev_rep_dragged
 set n_placement [[$ptr Noeud_representation] Val_MetaData Noeud_placement_drag]
   $n_placement Vider_fils
   $n_placement Ajouter_fils $top
}




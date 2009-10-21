if {[info exists noeud_fen]} {} else {
 source groupe.tcl
}

set zone_dessin [B_polygone]
  $noeud_fen Ajouter_fils $zone_dessin
  $zone_dessin Ajouter_contour [ProcRect 0 0 320 240]
  $zone_dessin Couleur 0.45 1 0.3 1

set rap_zone [B_rappel $interp_tcl]
set cmd "FSM_Start $rap_zone $zone_dessin"

$rap_zone Texte $cmd

$zone_dessin abonner_a_detection_pointeur [$rap_zone Rappel] 67

#B_contact ctc_dessin "$zone_dessin 3" -pt_trans 160 120 -pt_rot 160 120

#______________________________________________________________________________________________________________
#______________________________________________________________________________________________________________
#______________________________________________________________________________________________________________
proc FSM_Start {rap z} {
 puts "FSM_Start $rap"
 set inf [$rap Param]
 set inf [Void_vers_info $inf]
 set evt [$inf EVT]
 set pt  [$inf Point_au_contact]

 switch [$evt Type_evt] {
   1  {set p [B_polygone]; $z Ajouter_fils $p; $rap Texte "FSM_New $rap $z [$inf Ptr] $p \{\{[$pt X] [$pt Y]\}\} 0"}
   2  {}
   64 {}
  }
}

#______________________________________________________________________________________________________________
proc FSM_New {rap z ptr poly L_pt quittable} {
 puts "FSM_New $rap $ptr $poly \{$L_pt\}"
 set p   [$rap Param]
 set inf [Void_vers_info $p]
 set evt [$inf EVT]
 set pt  [$inf Point_au_contact]

 if {[string equal $ptr [$inf Ptr]]} {} else {return}

 switch [$evt Type_evt] {
   1  {puts 1
       if {$quittable} {$rap Texte "FSM_Start $rap $z"}
      }
   2  {puts 2
       lappend L_pt "[$pt X] [$pt Y]"; $rap Texte "FSM_New $rap $z $ptr $poly \{$L_pt\} 1"}
   64 {puts 3
       set L_pt [lreplace $L_pt end end "[$pt X] [$pt Y]"]; $rap Texte "FSM_New $rap $z $ptr $poly \{$L_pt\} 0"}
  }
 $poly Vider
 set L [list]
 foreach p $L_pt {
   lappend L [lindex $p 0]
   lappend L [lindex $p 1]
  }
 $poly Ajouter_contour [ProcTabDouble $L]
}

#______________________________________________________________________________________________________________


set poly_constr [B_polygone]
  $poly_constr Ajouter_contour [ProcTabDouble {0 0 400 0 400 400 0 400}]
  $poly_constr Couleur 0 0.2 1 1
$noeud_partage Ajouter_fils $poly_constr

set automat_constr [B_HFSM -add_states_loop {TCHI APPUI RELACHE}]

set rappel_constr [$Rappel_TCL $interp_tcl "puts coucou"]
  $poly_constr abonner_a_detection_pointeur [$rappel_constr Rappel] 3

#________________________________________________________________________________
#________________________________________________________________________________
#________________________________________________________________________________
set HFSM_id 0
proc B_HFSM args {
 global HFSM_id
 set name "HFSM_$HFSM_id"
 return [eval HFSM $name $args]
}

method HFSM constructor args {
 set this(L_currents_state) [list]

#-------------------------
 set L_cmd [split $args -]
 puts "$objName :"
 foreach cmd $L_cmd {
   if {[string equal $cmd {}]} {continue}
   puts "   cmd : $cmd"
   eval "$objName $cmd"
  }
}

#________________________________________________________________________________
method HFSM add_states_loop {L_states_cond} {
 foreach s $L_states {
   this add_state $s
  }
 set pos    1
 set taille [llength $L_states]
 foreach sc $L_states {
   set s [lindex $sc 0]
   set c [lindex $sc 1]
   this add_transitions [list "$s [lindex $L_states $pos] $c"]
   set pos [expr ($pos+1)%$taille]
  }
 set sc [lindex $L_states 0]
 this add_currents [lindex $sc 0]
}

#________________________________________________________________________________
method HFSM add_transitions {L_trans} {
 set pos
 foreach t $L_trans {
   set dep  [lindex $t 0]
   set arr  [lindex $t 1]
   set cond [lindex $t 2]
   if {[info exists this(state,$state_dep)]} {} else {continue}
     set s $this(state,$state_dep)
       set transitions [lindex $s 0]
       set nested_FSM  [lindex $s 1]
       lappend transitions [list $arr $cond]
     set this(state,$state_dep) [list $transitions $nested_FSM]
  }
}

#________________________________________________________________________________
method HFSM add_state {state_name {transitions {}}} {
 if {[info exists this(state,$state_name)]} {return 0}
 set this(state,$state_name) [list $transition {}]
 return $this(state,$state_name)
}

#________________________________________________________________________________
method HFSM add_currents {state_name} {
 if {[info exists this(state,$state_name)]} {} else {
   this add_state $state_name {}
  }
 lappend this(L_currents_state) $state_name
}



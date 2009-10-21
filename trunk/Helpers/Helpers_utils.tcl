package require tdom

proc Load_helper_in {helper_file B207_node} {
 set f [open $helper_file]
   dom parse [read $f] doc
   $doc documentElement root
   set L_rep [$root asList]
   $B207_node Ajouter_MetaData_T help $L_rep
 close $f
}

proc Get_scenario {helper B207_node scenario_id} {
 set L_scenar [lindex [$B207_node Val_MetaData help] 2]
 foreach s $L_scenar {
   set go 0
   foreach {att val} [lindex $s 1] {
     if {[string equal $att id] && [string equal $val $scenario_id]} {set go 1; break}
    }
   if {$go} {
     set obj $B207_node
	 set cmd [subst [lindex $s 2]]
	 return [lindex $cmd 0]
    }
  }
}


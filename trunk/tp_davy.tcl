source test_poly.tcl

#___________________________________________________________________________________________________________________________________________
proc Generate_accessors {class_name L_vars} {
 foreach v $L_vars {
   set    cmd "method $class_name get_$v \{ \} \{"
   append cmd {return $this(} $v ")\}"
     eval $cmd
   set    cmd "method $class_name set_$v \{v\} \{"
   append cmd {set this(} $v ") \$v\}"
     eval $cmd
  }
}

#___________________________________________________________________________________________________________________________________________
set poly_workspace [B_polygone]
  $noeud_partage Ajouter_fils $poly_workspace
  B_configure $poly_workspace -Ajouter_contour [ProcRect 0 0 1000 1000] -Couleur 0.2 0.2 0.2 0.2

#___________________________________________________________________________________________________________________________________________
method Automate_tabbing constructor {workspace} {
 set this(rap_START)   [B_rappel [Interp_TCL] "$objName Open_tab_region"]
 set this(rap_STOP)    [B_rappel [Interp_TCL] "$objName Close_tab_region"]
 set this(rap_TAB)     [B_rappel [Interp_TCL] "$objName Detect_tabs"]

 set this(workspace)    $workspace
   $workspace abonner_a_detection_pointeur [$this(rap_START) Rappel] [expr [ALX_pointeur_enfonce]+[Rappel_Seulement_si_noeud_contient]]
   $workspace abonner_a_detection_pointeur [$this(rap_STOP)  Rappel] [expr [ALX_pointeur_relache]+[Rappel_Seulement_si_noeud_contient]]
 
 set this(tab_region) [B_polygone]
   B_configure $this(tab_region) -Ajouter_contour [ProcRect 0 0 100 100] -Couleur 1 0 0.5 1
   $this(tab_region) abonner_a_detection_pointeur [$this(rap_TAB) Rappel] [ALX_pointeur_enfonce]
  
 set this(current_polygon) ""
 set this(current_pointer) ""
 set this(cmd_to_eval)     ""
 set this(counter)         0
 set this(delay_to_press)  250
 set this(position_in_hierarchy) ""
}

#___________________________________________________________________________________________________________________________________________
method Automate_tabbing dispose {} {
 this inherited
}

#___________________________________________________________________________________________________________________________________________
Generate_accessors Automate_tabbing [list cmd_to_eval workspace tab_region rap_START rap_STOP rap_TAB counter delay_to_press]

#___________________________________________________________________________________________________________________________________________
method Automate_tabbing get_the_right_cmd_list {} {
 set cmd $this(cmd_to_eval)
 foreach e $this(position_in_hierarchy) {
   set cmd [lindex $cmd [lindex $e 0]]
  }
 return $cmd
}

#___________________________________________________________________________________________________________________________________________
method Automate_tabbing Open_tab_region {} {
 puts {Open tab region}

 if {![$this(workspace) A_pour_fils $this(tab_region)]} {
   $this(workspace) Ajouter_fils_au_debut $this(tab_region)
  } else {puts "$this(workspace) still has for daughter $this(tab_region)"}
 set infos [this get_info_from_rap $this(rap_START)]
   set ptr [$infos Ptr]
   set x [$infos X_au_contact]
   set y [$infos Y_au_contact]

 $this(tab_region) Origine $x $y
}

#___________________________________________________________________________________________________________________________________________
method Automate_tabbing Close_tab_region {} {
 puts "Close tab region :\n "
 #$this(tab_region) Vider_peres
}

#___________________________________________________________________________________________________________________________________________
method Automate_tabbing Detect_tabs {} {
 #puts {Start press}
 set this(counter) 1
 $this(rap_TAB) Texte "$objName Add_press"
 after $this(delay_to_press) "$objName Is_this_the_end $this(counter)"
}

#___________________________________________________________________________________________________________________________________________
method Automate_tabbing Add_press {} {
 #puts "Add press"
 incr this(counter)
 after $this(delay_to_press) "$objName Is_this_the_end $this(counter)"
}

#___________________________________________________________________________________________________________________________________________
# set back the original command list
method Automate_tabbing Restart {} {
 set e [lindex $this(position_in_hierarchy) 0]
 if {[string equal $e ""]} {
   #puts "No stack for the command list..."; 
   return
  }
 set this(cmd_to_eval) [lindex $e 1]
 set this(position_in_hierarchy) ""
 #puts "Set back the original command list:\n$this(cmd_to_eval)"
}

#___________________________________________________________________________________________________________________________________________
method Automate_tabbing Is_this_the_end {nb} {
 if {[string equal $nb $this(counter)]} {
   # It is the end
   $this(rap_TAB) Texte "$objName Detect_tabs"
   #puts "Eval cmd $this(counter) !!!"
   incr this(counter) -1
   set cmd [lindex $this(cmd_to_eval) $this(counter)]
   if {[string equal $cmd ""]} {
     this Restart
	 return
	}
   if {[llength $cmd] == 1} {
     #puts "[lindex $cmd 0]"; 
	 eval [lindex $cmd 0];
	 this Restart
	 return
	}
   puts "Several options:"; set i 0
   foreach c $cmd {puts " -$i : [lindex $cmd $i]"; incr i}
  
  # Recurse with the new commands list  
   lappend this(position_in_hierarchy) [list $this(counter) $this(cmd_to_eval)]
   set this(cmd_to_eval) [this get_the_right_cmd_list]
   #puts "There is a sub menu with following list of commands:\n$this(cmd_to_eval)"
  }
}

#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
method Automate_tabbing get_info_from_rap {rap} {
 set p [$rap Param]
 return [Void_vers_info $p]
}


#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
Automate_tabbing automate_tab $poly_workspace

automate_tab set_cmd_to_eval [list [list "puts coucou"] \
                                   [list {{puts {Edit 1}}} {{puts {Edit 2}}}] \
							 ]

#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
#_______________ ANNEXE __________________
proc Debug_with_trace {c} {
 set L_txt [split [gmlObject info interface $c] "\n"]
   set L {}
   foreach txt $L_txt {
     if {[regexp "^method $c (\[_a-zA-Z0-9\]*) .*$" $txt reco m]} {lappend L $m}
	}
 foreach m $L {
   puts $m
   set body [gmlObject info body $c $m]
   set cmd "method $c $m {"
     # Puts aruments
	 set L_args [gmlObject info args $c $m]
	 append cmd $L_args
	 set L_args_call {}
	   foreach a $L_args {lappend L_args_call "\$[lindex $a 0] "}
	 append cmd "} {\n puts \"\$objName $m $L_args_call\"\n$body}"
   eval $cmd
  }
}

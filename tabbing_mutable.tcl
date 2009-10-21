#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
method Automate_Mutable_tabbing get_info_from_rap {rap} {
 set p [$rap Param]
 return [Void_vers_info $p]
}

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
#___________________________________________________________________________________________________________________________________________
method Menu_item constructor {title cmd {L_subitem {}}} {
 set this(title)     $title
 set this(cmd)       $cmd
 set this(L_subitem) $L_subitem
}

#___________________________________________________________________________________________________________________________________________
Generate_accessors Menu_item [list title cmd L_subitem]

#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
method Automate_Mutable_tabbing constructor {workspace cmd_to_eval {use_fixed_tab_region FALSE}} {
 set this(workspace)    $workspace
 set this(tab_region)   $workspace
 
 if {[string equal $use_fixed_tab_region "FALSE"]} {
   set this(rap_START)   [B_rappel [Interp_TCL] "$objName Open_tab_region"]
    $workspace abonner_a_detection_pointeur [$this(rap_START) Rappel] [expr [ALX_pointeur_enfonce]+[Rappel_Seulement_si_noeud_contient]]
   set this(rap_STOP)    [B_rappel [Interp_TCL] "$objName Close_tab_region"]
    $workspace abonner_a_detection_pointeur [$this(rap_STOP)  Rappel] [expr [ALX_pointeur_relache]+[Rappel_Seulement_si_noeud_contient]]

   set this(tab_region) [B_polygone]
    B_configure $this(tab_region) -Ajouter_contour [ProcRect 0 0 100 100] -Couleur 1 0 0.5 1
  }
  
 set this(rap_TAB_press)     [B_rappel [Interp_TCL] "$objName Detect_tabs"]
  $this(tab_region) abonner_a_detection_pointeur [$this(rap_TAB_press) Rappel] [ALX_pointeur_enfonce]
 set this(rap_TAB_release)   [B_rappel [Interp_TCL] "$objName Add_release"]
  $this(tab_region) abonner_a_detection_pointeur [$this(rap_TAB_release) Rappel] [ALX_pointeur_relache]

 set this(counter)         0
 set this(index)           0
 set this(delay_to_press)  60

 set this(unique_id)       0
 
 set this(cmd_to_eval)           $cmd_to_eval
 set this(position_in_hierarchy) ""
}

#___________________________________________________________________________________________________________________________________________
method Automate_Mutable_tabbing dispose {} {
 this inherited
}

#___________________________________________________________________________________________________________________________________________
Generate_accessors Automate_Mutable_tabbing [list index rap_START rap_STOP rap_TAB_release rap_TAB_press workspace tab_region counter delay_to_press cmd_to_eval position_in_hierarchy]

#___________________________________________________________________________________________________________________________________________
method Automate_Mutable_tabbing Open_tab_region {} {
 puts "Open tab region"

 if {![$this(workspace) A_pour_fils $this(tab_region)]} {
   $this(workspace) Ajouter_fils_au_debut $this(tab_region)
  } else {puts "$this(workspace) still has child $this(tab_region)"}
 
 set infos [this get_info_from_rap $this(rap_START)]
   set ptr [$infos Ptr]
   set x [$infos X_au_contact]
   set y [$infos Y_au_contact]

 $this(tab_region) Origine $x $y
}

#___________________________________________________________________________________________________________________________________________
method Automate_Mutable_tabbing Close_tab_region {} {
 puts "Close tab region"

 $this(tab_region) Vider_peres
}

#___________________________________________________________________________________________________________________________________________
method Automate_Mutable_tabbing get_a_unique_id {} {
 incr this(unique_id)
 return $this(unique_id)
}

#___________________________________________________________________________________________________________________________________________
method Automate_Mutable_tabbing Detect_tabs {} {
 #puts "Start\n  - counter : $this(counter)\n  -index : $this(index)"
 
 set this(counter) 1; set this(index) 1
 $this(rap_TAB_press) Texte "$objName Add_press"
 after $this(delay_to_press) "$objName Is_this_the_end [this get_a_unique_id]"
}

#___________________________________________________________________________________________________________________________________________
method Automate_Mutable_tabbing Add_release {} {
# Add CB in constructor or somewhere...
 incr this(counter) -1
 set this(counter) [expr $this(counter)>=0?$this(counter):0]
 after $this(delay_to_press) "$objName Is_this_the_end [this get_a_unique_id]"
}

#___________________________________________________________________________________________________________________________________________
method Automate_Mutable_tabbing Add_press {} {
 #puts "Add press"
 
 incr this(counter)
 set this(index) $this(counter)
 after $this(delay_to_press) "$objName Is_this_the_end [this get_a_unique_id]"
}

#___________________________________________________________________________________________________________________________________________
method Automate_Mutable_tabbing get_right_cmd_list {} {
 set cmd $this(cmd_to_eval)

 foreach e $this(position_in_hierarchy) {
   set menu_i [lindex $cmd [lindex $e 0]]
   set cmd [$menu_i get_L_subitem]
  }
 
 return $cmd
}

#___________________________________________________________________________________________________________________________________________
# set back original command list
method Automate_Mutable_tabbing Restart {} {
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
method Automate_Mutable_tabbing Say_where_we_are {nb txt_to_say} {
 if {[string equal $nb $this(unique_id)]} {
   puts "$objName Say_where_we_are $nb \"$txt_to_say\""
   Say $txt_to_say
  }
}

#___________________________________________________________________________________________________________________________________________
method Automate_Mutable_tabbing Is_this_the_end {nb} {
 if {[string equal $nb $this(unique_id)]} {
   # list index starts at 0
   puts "$objName Is_this_the_end $this(index)"
   incr this(index) -1
   set cmd [lindex $this(cmd_to_eval) $this(index)]
   if {[string equal $cmd ""]} {
     puts "CMD :\n$this(cmd_to_eval)"
	 Say "Too much pointers"; 
	 return
	}
   
   # Are there still some pointer on the zone ?
   if {$this(counter)} {
     set this(index) $this(counter)
	 set cmd [lindex $this(cmd_to_eval) [expr $this(index) -1]]
	 puts "  => Index updated to $this(index)"
	 after 200 "$objName Say_where_we_are $nb {[$cmd get_title]}"
	 return
    }
   
   $this(rap_TAB_press) Texte "$objName Detect_tabs"
   set $this(counter) 0
   
   if {[string equal $cmd ""]} {
     this Restart
	 return
	}
   if {[llength [$cmd get_L_subitem]] == 0} {
     eval [$cmd get_cmd]
	 puts "  EVAL of [$cmd get_cmd]"
	 Say [$cmd get_title]
	 this Restart
	 return
	}
   Say [$cmd get_title]
   puts "Select option:"; set i 1
   foreach c [$cmd get_L_subitem] {puts " -> $i : [$c get_title]"; incr i}
  
   # recurse with new commands list  
   lappend this(position_in_hierarchy) [list $this(index) $this(cmd_to_eval)]
   set this(cmd_to_eval) [this get_right_cmd_list]
   #puts "There is a submenu with following commands:\n$this(cmd_to_eval)"
  }
}


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

#Debug_with_trace Automate_Mutable_tabbing
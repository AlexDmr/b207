#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
method Automate_tabbing get_info_from_rap {rap} {
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
method Automate_tabbing constructor {workspace cmd_to_eval {use_fixed_tab_region FALSE}} {
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
  
 set this(rap_TAB)     [B_rappel [Interp_TCL] "$objName Detect_tabs"]
  $this(tab_region) abonner_a_detection_pointeur [$this(rap_TAB) Rappel] [ALX_pointeur_enfonce]
 
 set this(counter)         0
 set this(delay_to_press)  250
 
 set this(cmd_to_eval)           $cmd_to_eval
 set this(position_in_hierarchy) ""
 
 set this(text_menu) [B_texte 100 100 20 [fonte_Arial] [B_sim_sds]]
 B_configure $this(text_menu) -Editable 0 -Couleur_texte 1 1 1 0.5
 $this(tab_region) Ajouter_fils $this(text_menu)
 
 set this(text_feedback) [B_texte 100 100 20 [fonte_Arial] [B_sim_sds]]
 B_configure $this(text_feedback) -Editable 0 -Couleur_texte 1 0 0 0.8
 $this(tab_region) Ajouter_fils $this(text_feedback)
 
 set i 1; set this(text_top_menu) "|\n|";
 foreach c $this(cmd_to_eval) {
   set this(text_top_menu) [concat $this(text_top_menu) "$i : [$c get_title]\n|"]
   incr i
  }
 this Set_text_menu $this(text_top_menu)
}

#___________________________________________________________________________________________________________________________________________
method Automate_tabbing dispose {} {
 this inherited
}

#___________________________________________________________________________________________________________________________________________
Generate_accessors Automate_tabbing [list rap_START rap_STOP rap_TAB workspace tab_region counter delay_to_press cmd_to_eval position_in_hierarchy]

#___________________________________________________________________________________________________________________________________________
method Automate_tabbing Open_tab_region {} {
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
method Automate_tabbing Close_tab_region {} {
 puts "Close tab region"

 $this(tab_region) Vider_peres
}

#___________________________________________________________________________________________________________________________________________
method Automate_tabbing Detect_tabs {} {
 #puts "Start press"

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
method Automate_tabbing get_right_cmd_list {} {
 set cmd $this(cmd_to_eval)

 foreach e $this(position_in_hierarchy) {
   set menu_i [lindex $cmd [lindex $e 0]]
   set cmd [$menu_i get_L_subitem]
  }
 
 return $cmd
}

#___________________________________________________________________________________________________________________________________________
# set back original command list
method Automate_tabbing Restart {} {
 set e [lindex $this(position_in_hierarchy) 0]
 
 if {[string equal $e ""]} {
   #puts "No stack for the command list..."; 
   return
  }
 
 set this(cmd_to_eval) [lindex $e 1]
 set this(position_in_hierarchy) ""
 
 this Set_text_menu $this(text_top_menu)
 this Set_text_feedback ""
 #puts "Set back the original command list:\n$this(cmd_to_eval)"
}

#___________________________________________________________________________________________________________________________________________
method Automate_tabbing Is_this_the_end {nb} {
 if {[string equal $nb $this(counter)]} {
   #puts "Eval cmd $this(counter) !!!" 

   # list index starts at 0
   incr this(counter) -1; set prev_counter $this(counter)
   set cmd [lindex $this(cmd_to_eval) $this(counter)]
   
   $this(rap_TAB) Texte "$objName Detect_tabs"
   set $this(counter) 0
   
   if {[string equal $cmd ""]} {
     this Restart
	 return
   }
   if {[llength [$cmd get_L_subitem]] == 0} {
	 if {$prev_counter == 4} {
	   this Restart
	 } else {
	   this Set_text_feedback "... [$cmd get_title]"
	   eval [$cmd get_cmd]
	 }
	 return
	}
   
   # generate textual feedback  
   set i 1; set text "|\n|";
   foreach c [$cmd get_L_subitem] {
	 set text [concat $text "$i : [$c get_title]\n|"]
	 incr i
    }
	this Set_text_menu $text
	this Set_text_feedback "... [$cmd get_title]"
  
   # recurse with new commands list  
   lappend this(position_in_hierarchy) [list $prev_counter $this(cmd_to_eval)]
   set this(cmd_to_eval) [this get_right_cmd_list]
  }
}

#___________________________________________________________________________________________________________________________________________
method Automate_tabbing Update_text_position {} {
 $this(text_menu) Calculer_boite_noeud
 
 set node_box [$this(tab_region) Boite_noeud]
 set text_box [$this(text_menu) Boite_noeud]
 
 set node_height [$node_box Y2]
 set text_height [$text_box Y2]
 
 $this(text_menu) 		Origine 10 [expr $node_height - $text_height - 10]
 $this(text_feedback) 	Origine 10 [expr $node_height - $text_height - 50]
}

#___________________________________________________________________________________________________________________________________________
method Automate_tabbing Set_text_menu {menu_text} {
 $this(text_menu) Maj_texte $menu_text
 $this(text_menu) Optimiser_dimensions
 
 this Update_text_position
}

#___________________________________________________________________________________________________________________________________________
method Automate_tabbing Set_text_feedback {feedback_text} {
 $this(text_feedback) Maj_texte $feedback_text
 $this(text_feedback) Optimiser_dimensions
 
 this Update_text_position
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

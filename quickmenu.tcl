#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu get_info_from_rap {rap} {
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
#___________________________________________________________________________________________________________________________________________
method Menu_item constructor {title cmd {help ""} {L_subitem {}}} {
 set this(title)     $title
 set this(cmd)       $cmd
 set this(help)      $help
 set this(L_subitem) $L_subitem
 set this(init_cmd)  ""
 
 return $objName
}

#___________________________________________________________________________________________________________________________________________
Generate_accessors Menu_item [list title cmd help L_subitem init_cmd]

#___________________________________________________________________________________________________________________________________________
method Menu_item get_title {} {
 return [subst $this(title)]
}

#___________________________________________________________________________________________________________________________________________
method Menu_item Has_sub_menus    { } {return [expr [llength $this(L_subitem)] > 0]}

#___________________________________________________________________________________________________________________________________________
method Menu_item Get_sub_item_num {n} {return [lindex $this(L_subitem) $n]}

#___________________________________________________________________________________________________________________________________________
method Menu_item Nb_sub_item_num {} {return [llength $this(L_subitem)]}

#___________________________________________________________________________________________________________________________________________
method Menu_item Get_recursive_submenus {} {
 set rep $this(L_subitem)
 foreach sm $this(L_subitem) {set rep [concat $rep [$sm Get_recursive_submenus]]}
 return $rep
}

#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu constructor {workspace Menu_item} {
	set this(input_region)	$workspace
    set this(expert_mode_delay)     200
	set this(last_interaction)      0
	set this(mode_expert_activated) 0
	
    set this(menu_item) $Menu_item

	set this(n_root)       [B_noeud]
	  set this(rap_events) [B_rappel [Interp_TCL] "$objName set_last_interaction \[N_i_mere ms\]"]
	  $this(n_root) abonner_a_detection_pointeur [$this(rap_events) Rappel] [ALX_pointeur_ALL]
	set this(text_title)   [B_texte 100 100 20 [fonte_Arial] [B_sim_sds]]; this Set_text_title [$this(menu_item) get_title];
	set this(text_help)	   [B_texte 100 100 20 [fonte_Arial] [B_sim_sds]]; this Set_text_help  [$this(menu_item) get_help];
	set this(L_text_sub_items) {}
	
	B_configure $this(text_help)		-Editable 0 -Couleur_texte	1		1		1		0.5 -Noeud_touchable 0
	B_configure $this(text_title)		-Editable 0 -Couleur_texte	1		0		0		0.8 -Noeud_touchable 0
	
	B_configure $this(n_root) -Ajouter_fils $this(text_help) -Ajouter_fils $this(text_title)
	$this(input_region) Ajouter_fils $this(n_root)

	# Text pool
    set this(Pool_of_textes) ""
    
	# A variable to express if the automate is active (take events) or not
	set this(is_active) 1
	set this(last_x) 0
	set this(last_y) 0
	
 this Update
 return $objName
}

#___________________________________________________________________________________________________________________________________________
Generate_accessors Automate_quickmenu [list mode_expert_activated last_interaction expert_mode_delay is_active input_region menu_item n_root text_help text_title L_text_sub_items]

#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu Reload {} {}

#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu Start_mode_expert {} {
 #puts "Automate_quickmenu :: $objName Start_mode_expert"
 set this(mode_expert_activated) 1
 $this(n_root) Gerer_contacts 1; $this(n_root) Afficher_noeud 0
 after $this(expert_mode_delay) "$objName Do_switch_to_beginner_mode_or_not"
}

#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu Do_switch_to_beginner_mode_or_not {} {
 #puts "Automate_quickmenu :: $objName Do_switch_to_beginner_mode_or_not"
 set ms [N_i_mere ms]
 if {($this(last_interaction) + $this(expert_mode_delay)) < $ms} {
  # Switch to beginner mode
   this Stop_mode_expert
  } else {set dt [expr abs($this(expert_mode_delay) - ($ms - $this(last_interaction)))]
          set dt [expr $dt>20?$dt:20]
		  after $dt "$objName Do_switch_to_beginner_mode_or_not"
         }
}

#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu Stop_mode_expert {} {
 if {$this(mode_expert_activated)} {
   $this(n_root) Gerer_contacts 1; $this(n_root) Afficher_noeud 1
   set this(mode_expert_activated) 0
  } 
}
#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu dispose {} {
 Detruire $this(n_root)
 Detruire $this(text_help)
 Detruire $this(text_title)
 foreach e $this(L_text_sub_items) {Detruire $e}
 this inherited
}

#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu Back {} {}

#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu get_a_B_texte {txt} {
 if {[llength $this(Pool_of_textes)] > 0} {
   set B_txt [lindex $this(Pool_of_textes) 0]
   set this(Pool_of_textes) [lrange $this(Pool_of_textes) 1 end]
  } else {set B_txt [B_texte 100 100 20 [fonte_Arial] [B_sim_sds]]
         }
 
 B_configure $B_txt	-Editable 0 -Couleur_texte	1	1	1	1 -Noeud_touchable 0 -Afficher_fond 1 -Couleur_fond 0 0 0 1
 $this(n_root) Ajouter_fils_au_debut $B_txt
 $B_txt Maj_texte $txt
 $B_txt Optimiser_dimensions
 $B_txt Calculer_boites
 return $B_txt
}

#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu Release_B_texte {t} {
 lappend this(Pool_of_textes) $t
 $t Vider_peres
}

#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu UnDisplay {} {
 $this(n_root) Afficher_noeud 0
 $this(n_root) Gerer_contacts 0
 this Focus_on ""
}

#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu Display {x y} {
 $this(n_root) Afficher_noeud 1
 $this(n_root) Gerer_contacts 0
 $this(n_root) Origine $x $y
   set this(last_x) $x
   set this(last_y) $y

 this Update
}

#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu Update {} {
# Title 
 this Set_text_title  [$this(menu_item) get_title]
   set title_box [$this(text_title) Boite_noeud]
   $this(text_title) Origine 10 "-[$title_box Ty]"
 set delta -[$title_box Ty]

# Sub menus
 foreach e $this(L_text_sub_items) {
   set e [lindex $e 0]
   this Release_B_texte $e
  }
 set this(L_text_sub_items) {}
 set i 1
 foreach e [$this(menu_item) get_L_subitem] {
   set e [list [this get_a_B_texte "$i : [$e get_title]"] $e]
   lappend this(L_text_sub_items) $e
   set e [lindex $e 0];
   $e Calculer_boites; 
   set menu_box [$e Boite_noeud]
   set delta [expr $delta - [$menu_box Ty]]
   $e Origine 25 $delta
   incr i
  }

# Help 
 this Set_text_help  [$this(menu_item) get_help]
 set box [$this(text_help) Boite_noeud]
 set delta [expr $delta - [$box Ty]]
 $this(text_help) Origine 10 $delta
}

#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu Focus_on_index {i} {
 set b_item [lindex $this(L_text_sub_items) $i]
 this Focus_on [lindex $b_item 1]
}

#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu Focus_on {m} {
 foreach e $this(L_text_sub_items) {
   set Btxt [lindex $e 0]; $Btxt Afficher_boites 0
   set item [lindex $e 1]
   if {[string equal $item $m]} {$Btxt Afficher_boites 1}
  }
}

#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu Activate {} {
 this set_is_active 1
 this Display $this(last_x) $this(last_y)
}

#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu UnActivate {} {
 this set_is_active 0
 this UnDisplay
}

#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu New_start {args} {
 return [this get_is_active]
}

#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu Info_Goto_sub_menu_index {i} {}

#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu Go_to_submenu {menu_item} {
 eval "set Automate_menu $objName; [$menu_item get_cmd]"
 this Focus_on ""
}

#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu Activate_item {menu_item} {
 if {[catch {eval "set Automate_menu $objName; [$menu_item get_cmd]"} err]} {puts "ERROR in $objName, eval of menu item $menu_item:\n$err"}
 this Focus_on ""
}

#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu Set_text_title {txt} {
 $this(text_title) Maj_texte $txt
 $this(text_title) Optimiser_dimensions
 $this(text_title) Calculer_boites
}

#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu Set_text_help {txt} {
 $this(text_help) Maj_texte $txt
 $this(text_help) Optimiser_dimensions
 $this(text_help) Calculer_boites
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

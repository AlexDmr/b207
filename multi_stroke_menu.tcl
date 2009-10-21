source quickmenu.tcl
inherit Automate_multi_stroke_menu Automate_quickmenu

#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
method Automate_multi_stroke_menu constructor {workspace Menu_item} {
	this inherited $workspace $Menu_item

	set this(rap_PRESS)     [B_rappel [Interp_TCL]]
	set this(rap_RELEASE)	[B_rappel [Interp_TCL]]
	set this(rap_MOVE)		[B_rappel [Interp_TCL]]
	
	$this(rap_PRESS)	Texte "$objName Start $this(rap_PRESS)"
	$this(rap_RELEASE)	Texte "$objName Stop $this(rap_RELEASE)"
	
	$this(input_region) abonner_a_detection_pointeur [$this(rap_PRESS) Rappel] 		[ALX_pointeur_enfonce]
	$this(input_region) abonner_a_detection_pointeur [$this(rap_RELEASE) Rappel]	[ALX_pointeur_relache]
	$this(input_region) abonner_a_detection_pointeur [$this(rap_MOVE) Rappel]		[ALX_pointeur_mouvement]
	
	set this(original_menu_item) $Menu_item
	
	set bb [$workspace Boite_noeud]
	this Display [$bb Cx] [$bb Cy]
}

#___________________________________________________________________________________________________________________________________________
method Automate_multi_stroke_menu dispose {} {
	this inherited
}

#___________________________________________________________________________________________________________________________________________
Generate_accessors Automate_multi_stroke_menu [list rap_PRESS rap_RELEASE rap_MOVE]

#___________________________________________________________________________________________________________________________________________
method Automate_multi_stroke_menu Start {rap} {
	if {![this get_is_active]} {return}
	this New_start
	
	# get info on position
	set infos [this get_info_from_rap $rap]
	set ptr [$infos Ptr]
	set x [$infos X_au_contact]
	set y [$infos Y_au_contact]
	
	set this(starting_point,$ptr)	[list $x $y]
	
	set this(trail_dots,$ptr)	""
	this VisualizePoint $ptr $x $y
	
	$this(rap_MOVE)		Texte "$objName Visualize_stroke $this(rap_MOVE)"
}

#___________________________________________________________________________________________________________________________________________
method Automate_multi_stroke_menu Stop {rap} {
	#puts "stop"
	
	# get info
	set infos [this get_info_from_rap $rap]
	set ptr [$infos Ptr]
	set x [$infos X_au_contact]
	set y [$infos Y_au_contact]
	
	foreach e $this(trail_dots,$ptr) {
		$e Vider_peres
	}
	
	$this(rap_MOVE)		Texte ""
	
	this Detect_stroke $infos
}

#___________________________________________________________________________________________________________________________________________
method Automate_multi_stroke_menu Visualize_stroke {rap} {
	#puts "detect"
	
	set infos [this get_info_from_rap $rap]
	#this VisualizePoint [$infos Ptr] [$infos X_au_contact] [$infos Y_au_contact]
}


#___________________________________________________________________________________________________________________________________________
method Automate_multi_stroke_menu Detect_stroke {infos} {
	#puts "detect direction"
	
	set ptr [$infos Ptr]
	set x2 [$infos X_au_contact]
	set y2 [$infos Y_au_contact]
	
	# do we have a 'beginning' and an 'end'?
	if {[llength $this(starting_point,$ptr)] > 0} {
		set x1 [lindex $this(starting_point,$ptr) 0]
		set y1 [lindex $this(starting_point,$ptr) 1]
		
		# detect direction of the stroke and activate the corresponding menu item
		this Activate_menu_item [this Detect_stroke_direction $x1 $y1 $x2 $y2]
	}
}

#___________________________________________________________________________________________________________________________________________
method Automate_multi_stroke_menu Activate_menu_item {index} {
    if {![this get_is_active]} {return}
	set selected_menu [$this(menu_item) Get_sub_item_num $index]
	
	puts "menu: $selected_menu"

	if {[string equal $selected_menu ""]} {
		this Set_text_feedback "Invalid choice!"
		return
	}
	if {![$selected_menu Has_sub_menus]} {
		this Evaluate_command $selected_menu
		this Restart
	} else {
		this Go_to_submenu $selected_menu
	}
	#Say [$selected_menu get_title]
	
	if {$index == 4} {
		this Restart
	}
}

#___________________________________________________________________________________________________________________________________________
# 0: right; 1: up; 2: left; 3: down; 4: stationary;
method Automate_multi_stroke_menu Detect_stroke_direction {x1 y1 x2 y2} {
	#puts "detect direction"
	
	set x_dir [expr $x2 - $x1]
	set y_dir [expr $y2 - $y1]
	
	#puts "dir: $x_dir, $y_dir"
	
	set x_diff [expr abs($x_dir)]
	set y_diff [expr abs($y_dir)]
	
	#puts "diff: $x_diff, $y_diff"
	
	if {$x_diff > $y_diff} {
		if {$x_dir > 0} {
			puts "R"
			return 0
		} elseif {$x_dir < 0} {	
			puts "L"
			return 2
		}
	} elseif {$x_diff < $y_diff} {
		if {$y_dir > 0} {
			puts "U"
			return 1
		} elseif {$y_dir < 0} {
			puts "D"
			return 3
		}
	}
	
	puts "-"
	return 4;
}

method Automate_multi_stroke_menu pipo {} {
 set L {}
 foreach {ptr n} [array get this trail_dots,*] {
   lappend L $n
  }

 return $L
}

#___________________________________________________________________________________________________________________________________________
method Automate_multi_stroke_menu VisualizePoint {ptr x y {size 2}} {
 return
	set poly [B_polygone]
	$poly Ajouter_contour [ProcOvale $x $y $size $size 8]
	
	$this(input_region) Ajouter_fils_au_debut $poly
	if {[string equal $this(trail_dots,$ptr) ""]} {
		set this(trail_dots,$ptr) $poly
	} else {
		lappend this(trail_dots,$ptr) $poly
	}
	
	$poly Noeud_touchable 0
}

#___________________________________________________________________________________________________________________________________________
method Automate_multi_stroke_menu Restart {} {
 set this(menu_item) $this(original_menu_item)
 eval [$this(menu_item) get_cmd]
 this Update
}

#___________________________________________________________________________________________________________________________________________
method Automate_multi_stroke_menu Go_to_submenu {m} {
 this inherited $m
 if {[$m Has_sub_menus]} {
   set this(menu_item) $m
   this Update
  }
}

#___________________________________________________________________________________________________________________________________________
method Automate_multi_stroke_menu Evaluate_command {m} {
# execute current command
 this Activate_item $m

# Save title and help
 set old_title [$this(menu_item) get_title]
 set old_help  [$this(menu_item) get_help]
 
# Change them
 $this(menu_item) set_title  "... [$m get_title]"
 $this(menu_item) set_help   "[$m get_help]"

# Update presentation
 this Update

# set back old title and help values
 $this(menu_item) set_title  $old_title
 $this(menu_item) set_help   $old_help
}

#___________________________________________________________________________________________________________________________________________
method Automate_multi_stroke_menu Update {} {
# Title 
 this Set_text_title  [$this(menu_item) get_title]
   set title_box [$this(text_title) Boite_noeud]
   $this(text_title) Origine -[$title_box Cx] 0

# Sub menus
 foreach e $this(L_text_sub_items) {
   set e [lindex $e 0]
   this Release_B_texte $e
  }
 set this(L_text_sub_items) {}

 set n [llength [$this(menu_item) get_L_subitem]]
 incr n -1
 set a [expr 3.14159265 * 2 / $n]
 
  set i 0;
 foreach e [lrange [$this(menu_item) get_L_subitem] 0 end-1] {
   set e [list [this get_a_B_texte "[$e get_title]"] $e]
   lappend this(L_text_sub_items) $e
   set e [lindex $e 0];
   $e Calculer_boites; 
   set menu_box [$e Boite_noeud]

   set x [expr 130 * cos($i*$a)]
   set y [expr 100 * sin($i*$a)]
   
   $e Origine [expr $x - [$menu_box Cx]] [expr $y - [$menu_box Cy]]
   incr i
  }

# Help 
 this Set_text_help  [$this(menu_item) get_help]
 set box [$this(text_help) Boite_noeud]
 $this(text_help) Origine -[$box Cx] -[$box HD_Y]
}
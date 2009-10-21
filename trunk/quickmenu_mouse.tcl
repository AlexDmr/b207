source quickmenu.tcl
inherit Automate_quickmenu_mouse Automate_quickmenu

#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu_mouse constructor {workspace Menu_item} {
	this inherited $workspace $Menu_item
    
	set this(rap_TAB_press)     [B_rappel [Interp_TCL] "$objName Detect_tabs"]
	  
	  $this(input_region) abonner_a_detection_pointeur [$this(rap_TAB_press) Rappel] [ALX_pointeur_enfonce]
    
	set this(original_menu_item) $Menu_item
	set this(delay_to_press)  	250
	set this(delay_to_restart)	2000
	
	set this(counter) 0
	
	set bb [$workspace Boite_noeud]
	this Display [$bb BG_X] [$bb HD_Y]
}

#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu_mouse dispose {} {
	this inherited
}

#___________________________________________________________________________________________________________________________________________
Generate_accessors Automate_quickmenu_mouse [list rap_TAB_press delay_to_press]


#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu_mouse Detect_tabs {} {
	if {![this get_is_active]} {return}
	this New_start

	set this(counter) 1
	  this Focus_on_index 0
	$this(rap_TAB_press) Texte "$objName Add_press"
	after $this(delay_to_press) "$objName Is_this_the_end $this(counter)"
}

#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu_mouse Add_press {} {
	this Focus_on_index $this(counter)
	incr this(counter)
	after $this(delay_to_press) "$objName Is_this_the_end $this(counter)"
}

#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu_mouse Restart {} {
 set this(menu_item) $this(original_menu_item)
 eval [$this(menu_item) get_cmd]
 this Update
 this Focus_on ""
}

#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu_mouse Go_to_submenu {m} {
 this inherited $m
 if {[$m Has_sub_menus]} {
   set this(menu_item) $m
   this Update
  }
}

#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu_mouse Evaluate_command {m} {
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
method Automate_quickmenu_mouse Is_this_the_end {n} {
	if {[string equal $n $this(counter)]} {
		#puts "Eval cmd $this(counter) !!!" 

		# list index starts at 0
		incr this(counter) -1; set prev_counter $this(counter)
        set selected_menu [$this(menu_item) Get_sub_item_num $this(counter)] 
		$this(rap_TAB_press) Texte "$objName Detect_tabs"
		set this(counter) 0

		if {[string equal $selected_menu ""]} {
			this Set_text_help "Invalid choice!"
			return
		  }
		if {![$selected_menu Has_sub_menus]} {
			this Evaluate_command $selected_menu
			this Restart
		  } else {
			this Go_to_submenu $selected_menu
		}
		
		#this Say 0 [$cmd get_title]
		
		if {$prev_counter == 4} {
			this Restart
		}
	}
}

#Debug_with_trace Automate_quickmenu_mouse
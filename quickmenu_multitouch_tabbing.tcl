source quickmenu.tcl
inherit Automate_quickmenu_multitouch_tabbing Automate_quickmenu_mouse

#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu_multitouch_tabbing constructor {workspace Menu_item} {
	this inherited $workspace $Menu_item

	#set this(rap_TAB_press)     [B_rappel [Interp_TCL] "$objName Detect_tabs"]
	set this(rap_TAB_release)   [B_rappel [Interp_TCL] "$objName Add_release"]
	#$this(input_region) abonner_a_detection_pointeur	[$this(rap_TAB_press) Rappel]	[ALX_pointeur_enfonce]
	$this(input_region) abonner_a_detection_pointeur	[$this(rap_TAB_release) Rappel]	[ALX_pointeur_relache]

	set this(index)				0
	set this(unique_id)         0

	#set this(delay_to_press)	250
	set this(delay_to_say)		500
}

#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu_multitouch_tabbing dispose {} {
	this inherited
}

#___________________________________________________________________________________________________________________________________________
Generate_accessors Automate_quickmenu_multitouch_tabbing [list rap_TAB_press rap_TAB_release index delay_to_press delay_to_say delay_to_restart]

#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu_multitouch_tabbing Detect_tabs {} {
	if {![this get_is_active]} {return}
	this New_start

	#puts "Start\n  - counter : $this(counter)\n  -index : $this(index)"
	set this(counter)	1
	set this(index)	1
	$this(rap_TAB_press) Texte "$objName Add_press"
	after $this(delay_to_press) "$objName Is_this_the_end [this get_a_unique_id]"
}

#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu_multitouch_tabbing Add_release {} {
	#puts "Add release"
	incr this(counter) -1
	set this(counter) [expr $this(counter)>=0?$this(counter):0]
	after $this(delay_to_press) "$objName Is_this_the_end [this get_a_unique_id]"
}

#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu_multitouch_tabbing Add_press {} {
	#puts "Add press"
	incr this(counter)
	set this(index) $this(counter)
	after $this(delay_to_press) "$objName Is_this_the_end [this get_a_unique_id]"
}

#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu_multitouch_tabbing Is_this_the_end {id} {
	if {[string equal $id $this(unique_id)]} {
		#puts "$objName Is_this_the_end $this(index)"

		# still pointers on the zone?
		if {$this(counter)} {
			# set current menu item (list index starts at 0 => -1)
			set this(index) $this(counter)
			set selected_menu [$this(menu_item) Get_sub_item_num [expr $this(index) -1]] 
			
			#puts " => index updated to $this(index)"
			
			# feedback on current command
			if {![string equal $selected_menu ""]} {
				after $this(delay_to_say) "$objName Say_where_we_are $id {[$selected_menu get_title]}"
			} else {
				this Say_where_we_are $id "invalid command"; 
			}
 
			return
		}
  
		# reset the automate
		$this(rap_TAB_press) Texte "$objName Detect_tabs"
		set this(counter) 0
		
		# set current command (list index starts at 0 => -1)
		incr this(index) -1    
		set selected_menu [$this(menu_item) Get_sub_item_num $this(index)] 
  		
		# valid command?
		if {[string equal $selected_menu ""]} {
			this Say_where_we_are $id "invalid command"
			this Set_text_feedback "Invalid command!"
			return
		}
		
		# evaluate the command or go to the submenu
		if {![$selected_menu Has_sub_menus]} {
			this Evaluate_command $selected_menu
			this Restart
		} else {
			this Go_to_submenu $selected_menu
		}
		this Say_where_we_are $id [$selected_menu get_title]
		
		# restart if necessary
		if {$this(index) == 4} {
			# counter == 5 => restart by default
			this Restart
		}
	}
}

#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu_multitouch_tabbing Say_where_we_are {id txt_to_say} {
	if {[string equal $id $this(unique_id)]} {
		#puts "$objName Say_where_we_are $id \"$txt_to_say\""
		Say $txt_to_say
	}
}

#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu_multitouch_tabbing get_a_unique_id {} {
 incr this(unique_id)
 return $this(unique_id)
}

#Debug_with_trace Automate_quickmenu_multitouch_tabbing
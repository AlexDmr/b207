source quickmenu.tcl
inherit Automate_quickmenu_multitouch_tab_and_hold Automate_quickmenu_multitouch_tabbing

#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu_multitouch_tab_and_hold constructor {workspace Menu_item} {
	this inherited $workspace $Menu_item

	set this(delay_to_execute)		500
	set this(delay_to_press)         80
}

#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu_multitouch_tab_and_hold dispose {} {
	this inherited
}

#___________________________________________________________________________________________________________________________________________
Generate_accessors Automate_quickmenu_multitouch_tab_and_hold [list delay_to_execute]

#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu_multitouch_tab_and_hold Detect_tabs {} {
	if {![this get_is_active]} {return}
	this New_start

	#puts "Start\n  - counter : $this(counter)\n  -index : $this(index)"
	set this(counter)	1
	set this(index)		0
	$this(rap_TAB_press) Texte "$objName Add_press"
	after $this(delay_to_press) "$objName Is_this_the_end [this get_a_unique_id]"
}

#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu_multitouch_tab_and_hold Add_release {} {
	if {![this get_is_active]} {return}
	#puts "Add release"
	#puts "  - counter : $this(counter)\n  -index : $this(index)"
	incr this(counter) -1
	set this(counter) [expr $this(counter)>=0?$this(counter):0]
	after $this(delay_to_press) "$objName Is_this_the_end [this get_a_unique_id]"
}

#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu_multitouch_tab_and_hold Add_press {} {
	#puts "Add press"
	incr this(counter)
	  set this(index) $this(counter)
	  incr this(index) -1
	  this Focus_on_index $this(index)
	after $this(delay_to_press) "$objName Is_this_the_end [this get_a_unique_id]"
}

#___________________________________________________________________________________________________________________________________________
method Automate_quickmenu_multitouch_tab_and_hold Is_this_the_end {id} {
	if {[string equal $id $this(unique_id)]} {
		#puts "$objName Is_this_the_end $this(counter) , $this(index)"

		# no more pointers on the zone?
		if {$this(counter) == 0} {
			set selected_menu [$this(menu_item) Get_sub_item_num $this(index)] 
			
			# valid command?
			if {[string equal $selected_menu ""]} {
				#this Say_where_we_are $id "invalid command";
				puts "invalid command"
				return
			}
			
			# deactivate the mode or open submenu?
			if {![$selected_menu Has_sub_menus]} {
				# reset the automate
				$this(rap_TAB_press) Texte "$objName Detect_tabs"
				set this(counter) 0
				  this Evaluate_command $selected_menu
				this Restart
				return
			} else {
				# release & submenu => open it
				this Go_to_submenu $selected_menu
				return
			}
		}

		# set current command (list index starts at 0 => -1)
		set this(index) $this(counter)
		  incr this(index) -1
		  this Focus_on_index $this(index)
		set selected_menu [$this(menu_item) Get_sub_item_num $this(index)] 
		
		#puts " => Index updated to $this(index)"
  
		# valid command?
		if {[string equal $selected_menu ""]} {
			this Say_where_we_are $id "invalid command";
			return
		}
		
		#after $this(delay_to_say) "$objName Say_where_we_are $id {[$selected_menu get_title]}"
		
		# evaluate the command
		if {![$selected_menu Has_sub_menus]} {
			this Evaluate_command $selected_menu
			this Focus_on_index $this(index)
		}
 } else {this Focus_on_index $this(index)}
}

#Debug_with_trace Automate_quickmenu_multitouch_tab_and_hold
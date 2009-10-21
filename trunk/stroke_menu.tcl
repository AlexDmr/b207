source quickmenu.tcl
inherit Automate_stroke_menu Automate_quickmenu

#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
method Automate_stroke_menu constructor {workspace Menu_item {is_a_sub_menu 0}} {

	 set this(L_colors) [list "1 0 0 1" \
                      "0 1 0 1" \
					  "0 0 1 1" \
					  "1 1 0 1" \
					  "1 0 1 1" \
					  "0 1 1 1" \
					  "1 0.5 0.15 1" \
					  "1 1 1 1" \
				]
	set this(L_poly_slices) ""
	  set this(poly_rond_bkgnd) [B_polygone]; 
	    $this(poly_rond_bkgnd) Ajouter_contour [ProcOvale 0 0 200 200 60]
		set this(size_hole) 30 
		$this(poly_rond_bkgnd) Retirer_contour [ProcOvale 0 0 $this(size_hole) $this(size_hole) 60]
	  #$this(poly_rond_bkgnd) Gerer_contacts 0
	  set this(actual_number_of_slices) 0
	  for {set i 0} {$i < 20} {incr i} {set p [B_polygone]
	                                    #$p Gerer_contacts 0; $p Noeud_touchable 0
										lappend this(L_poly_slices) $p
									   }
	this inherited $workspace $Menu_item
    
	set this(actual_Display_Menu_item) ""
	set this(Display_Menu_parent)      ""
	
if {!$is_a_sub_menu} {
	set this(rap_PRESS)     [B_rappel [Interp_TCL]]
	set this(rap_RELEASE)	[B_rappel [Interp_TCL]]
	set this(rap_MOVE)		[B_rappel [Interp_TCL]]
	
	$this(rap_PRESS)	Texte "if {\[catch \"$objName Start $this(rap_PRESS)\" err\]} {puts \"ERROR in $objName Start\n\$err\"}"
	#$this(rap_RELEASE)	Texte "if {\[catch \"$objName Stop $this(rap_RELEASE)\" err\]} {puts \"ERROR in $objName Stop\n\$err\"}"
	
	$this(input_region) abonner_a_detection_pointeur [$this(rap_PRESS) Rappel] 		[expr [ALX_pointeur_enfonce]+[ALX_pointeur_apparition]]
	#$this(input_region) abonner_a_detection_pointeur [$this(rap_RELEASE) Rappel]	[ALX_pointeur_relache]
	#$this(input_region) abonner_a_detection_pointeur [$this(rap_MOVE) Rappel]		[ALX_pointeur_mouvement]
	
	set this(threshold_movement)	5
	set this(threshold_distance)	20000
	
	set this(filter)            [list -1.5 -0.7 -0.5 0 0.5 0.7 1.5]
	set this(llength_filter)    [llength $this(filter)] 
	set this(filter_threshold)  0
	  for {set i 0} {$i < ($this(llength_filter) / 2)} {incr i} {
	    set this(filter_threshold) [expr $this(filter_threshold) - [lindex $this(filter) $i]]
	   }
	  #DEBUG set this(filter_threshold) [expr $this(filter_threshold) * 1.25]
	  set this(filter_threshold) 1
	  
	set this(L_pt)              ""
	set this(current_ptr)       ""
	set this(L_of_transfos)     [Liste_alx_repere2D]
	set this(pt_tmp)            [B_point] 
	set this(not_moving)        0
	
	set this(Pool_Automate_stroke_menu)   ""
	set this(L_used_Automate_stroke_menu) ""
	  set this(unique_id) 0
	
   # Get a pointer to the actual descendant menu which is in use
    set this(actual_Menu_item)         $Menu_item
	set this(actual_Display_Menu_item) $objName
	set this(starting_point)   "0 0"
	set this(minimal_radius)   20
	
   # Place the menu centered in the workspace by default
	set bb [$workspace Boite_noeud]
	this Display [$bb Cx] [$bb Cy]
   } 
}

#___________________________________________________________________________________________________________________________________________
method Automate_stroke_menu dispose {} {
	this inherited
}

#___________________________________________________________________________________________________________________________________________
Generate_accessors Automate_stroke_menu [list Display_Menu_parent actual_Display_Menu_item current_ptr L_colors filter_threshold rap_PRESS rap_MOVE filter_x filter_y was_moving_fast_in_x was_moving_fast_in_y]

#___________________________________________________________________________________________________________________________________________
method Automate_stroke_menu get_a_unique_id {} {
 incr this(unique_id)
 return $this(unique_id)
}

#___________________________________________________________________________________________________________________________________________
method Automate_stroke_menu Release_all_submenus {} {
 set L $this(L_used_Automate_stroke_menu)
 foreach m $L {this Release_a_stroke_menu $m}
}

#___________________________________________________________________________________________________________________________________________
method Automate_stroke_menu Release_a_stroke_menu {m} {
 lappend this(Pool_Automate_stroke_menu) $m
 set this(L_used_Automate_stroke_menu) [lremove $this(L_used_Automate_stroke_menu) $m]
 $m UnDisplay
}

#___________________________________________________________________________________________________________________________________________
method Automate_stroke_menu Get_a_stroke_menu {Menu_item x y} {
 #puts "$objName Get_a_stroke_menu $Menu_item $x $y"
 if {[llength $this(Pool_Automate_stroke_menu)]} {
   set rep [lindex $this(Pool_Automate_stroke_menu) 0]
   set this(Pool_Automate_stroke_menu) [lrange $this(Pool_Automate_stroke_menu) 1 end]
  } else {set rep "${objName}_sub_menu_[this get_a_unique_id]"
          Automate_stroke_menu $rep $this(input_region) $Menu_item 1
         }
 $rep Display $x $y
 $rep set_menu_item $Menu_item
 $rep Update
 lappend this(L_used_Automate_stroke_menu) $rep
 return $rep
}

#___________________________________________________________________________________________________________________________________________
method Automate_stroke_menu Start {rap} {
 #puts "$objName Start $rap"
	if {![this get_is_active]} {return}
	this New_start
	# get info on position
	set infos [this get_info_from_rap $rap]
	set ptr [$infos Ptr]
	set x [$infos X_au_contact]
	set y [$infos Y_au_contact]
	
	#this VisualizePoint $ptr $x $y 5
	
	
# ALEX DEBUG
 #puts "  this(current_ptr) : $this(current_ptr)"
 if {[string equal $this(current_ptr) ""]} {
   set this(L_pt) [list "$x $y"]
   set this(current_ptr) $ptr
  # Subscribe to the pointer...
   set rap $this(rap_RELEASE)
     $ptr abonner_a_changement [$rap Rappel]
     $rap Texte "if {\[catch \"$objName Stop_if_release $ptr\" err\]} {puts \"ERROR in $objName Stop_if_release $ptr\n\$err\"}"
   set rap $this(rap_MOVE)
     $this(input_region) abonner_a_LR_parcours [$this(input_region) LR_Av_aff] [$rap Rappel]
	 #$ptr abonner_a_changement [$rap Rappel]
	 $rap Texte "if {\[catch \"$objName Detect_stroke $ptr\" err\]} {puts \"ERROR in $objName Detect_stroke $ptr\n\$err\"}"
   $this(L_of_transfos) maj [$infos L_REPERES]
   set this(starting_point) "$x $y"
  }
}

#___________________________________________________________________________________________________________________________________________
method Automate_stroke_menu Stop_if_release {ptr} {
 #puts "$objName Stop_if_release $ptr"
 if {![$ptr Appuie]} {
  #puts "  ALL RELEASE !!!"
  # Activate a submenu ?
   $this(pt_tmp) maj [$ptr X] [$ptr Y]
	  Repere_transformation $this(pt_tmp) $this(L_of_transfos)
	  set x [$this(pt_tmp) X]
	  set y [$this(pt_tmp) Y]
   this Goto_submenu $ptr $x $y
   
  # Release all
   this Stop $ptr
  }
}

#___________________________________________________________________________________________________________________________________________
method Automate_stroke_menu Stop {ptr} {
 #puts "Stop"

# ALEX DEBUG
 if {[string equal $this(current_ptr) $ptr]} {
   set this(current_ptr) ""
   $this(input_region) desabonner_de_LR_parcours [$this(input_region) LR_Av_aff] [$this(rap_MOVE) Rappel] 
   #$ptr desabonner_de_changement [$this(rap_MOVE)    Rappel]
   $ptr desabonner_de_changement [$this(rap_RELEASE) Rappel]
     set this(actual_Menu_item)         $this(menu_item)
	 set this(actual_Display_Menu_item) $objName
   this Release_all_submenus
   set bb [$this(input_region) Boite_noeud]
   this Display [$bb Cx] [$bb Cy]
  }
}

#___________________________________________________________________________________________________________________________________________
method Automate_stroke_menu Detect_stroke {ptr} {
	#puts "Detect_stroke $ptr"
	
	# get info on position
	#set infos [this get_info_from_rap $rap]
	#set ptr [$infos Ptr]
	#set x [$infos X_au_contact]
	#set y [$infos Y_au_contact]
	$this(pt_tmp) maj [$ptr X] [$ptr Y]
	  Repere_transformation $this(pt_tmp) $this(L_of_transfos)
	  set x [$this(pt_tmp) X]
	  set y [$this(pt_tmp) Y]
	#puts "Detect_stroke $ptr : ([$ptr X] ; [$ptr Y]) => ($x ; $y)"

	set root [$this(actual_Display_Menu_item) get_n_root]
	    set dcx [expr $x - [$root Px]]
		set dcy [expr $y - [$root Py]]
		if {($dcx*$dcx + $dcy*$dcy) <= $this(size_hole)*$this(size_hole)} {$this(actual_Display_Menu_item) Lower_slices; return}
	
	
   #____________________
    set dx $dcx; set dy $dcy
	set dist [expr sqrt($dx*$dx + $dy*$dy)]
	if {$dy == 0} {
      if {$dx >= 0} {set angle 0} else {set angle 3.14159265}
     } else {set angle [expr acos($dx/$dist)]
             if {$dy < 0} {set angle [expr 2*3.14150265 - $angle]}
            } 
    set nb_slice_tmp [expr [$this(actual_Menu_item) Nb_sub_item_num]-1]
    if {$nb_slice_tmp < 1} {set nb_slice_tmp 1}
    set slice    [expr 2*3.14159265/$nb_slice_tmp]
    set num      [expr int( ($angle + $slice/2.0) /  $slice ) % $nb_slice_tmp]

	$this(actual_Display_Menu_item) Bigger_slice $num
   #____________________
   
	#puts coucou
    if {[string equal $this(current_ptr) $ptr]} {
	  lappend this(L_pt) [list $x $y]
	  set this(L_pt) [lrange $this(L_pt) end-[expr $this(llength_filter)+1] end]
	  if {[llength $this(L_pt)] >= ($this(llength_filter) + 2)} {
	   # Apply the filter
	    #puts "Apply the filter"
	   # Compute angles and apply filter
	    set filter_value_x 0; set filter_value_y 0
		set mvt_x          0; set mvt_y          0
	    for {set i 0} {$i < $this(llength_filter)} {incr i} {
		  set DX 0; set DY 0
		  set pt [lindex $this(L_pt) $i]
		  set OX [lindex $pt 0]; set OY [lindex $pt 1]
		  for {set j 1} {$j < 3} {incr j} {
			set pt [lindex $this(L_pt) [expr $i+$j]]
			set PX [lindex $pt 0]; set PY [lindex $pt 1]
			  set DX [expr $DX + $PX - $OX]
			  set DY [expr $DY + $PY - $OY]
		    set OX $PX; set OY $PY
		   }
		  set D_length [expr sqrt($DX*$DX + $DY*$DY)]
		  if {$D_length == 0} {continue}
		 # Apply the filter....last!
		  #puts "Apply the filter....last!"
		  set filter_value_x [expr $filter_value_x + ($DX/$D_length)*[lindex $this(filter) $i]]
		  set filter_value_y [expr $filter_value_y + ($DY/$D_length)*[lindex $this(filter) $i]]
		 # Updtae the global mouvment
		  set mvt_x [expr $mvt_x + $DX]; set mvt_y [expr $mvt_y + $DY]
		 }
	   # Are we away enough from the center?
	    set root [$this(actual_Display_Menu_item) get_n_root]
	    set dcx [expr [$root Px] - $x]
		set dcy [expr [$root Py] - $y]
		if {($dcx*$dcx + $dcy*$dcy) <= $this(size_hole)*$this(size_hole)} {
		  #puts "Too close to the center"
		 } else {set center_pt   [lindex $this(L_pt) [expr ($this(llength_filter)/2)+2]]
		         set center_pt_x [lindex $center_pt 0]
				 set center_pt_y [lindex $center_pt 1]
		         # Does the direction has changed ?
		         set filter_value [expr abs($filter_value_x) + abs($filter_value_y)]
		         if {$filter_value > $this(filter_threshold)} {
		           set this(L_pt) ""
		           this Goto_submenu $ptr $center_pt_x $center_pt_y
		          } else {
	                      # Are we stopped or almost?
	                      if {sqrt($mvt_x * $mvt_x  +  $mvt_y * $mvt_y) < 1.5*$this(llength_filter)} {
		                    if {!$this(not_moving)} {
		                      set this(not_moving) 1
			                  set this(L_pt) ""
			                  this Goto_submenu $ptr $center_pt_x $center_pt_y
		                     } else {set this(not_moving) 0}
						   }
		                 } 
				}
	   }
	 }
	
}

#___________________________________________________________________________________________________________________________________________
method Automate_stroke_menu Goto_submenu {ptr x y} {
 set dx [expr $x-[lindex $this(starting_point) 0]]; set dy [expr $y-[lindex $this(starting_point) 1]]
 set dist [expr sqrt($dx*$dx + $dy*$dy)]
 
# Given the angle between the starting point and <$x $y>, goto a submenu, provided that a minimum distance has been reached
 if {$dist <= $this(minimal_radius)} {$this(actual_Display_Menu_item) Lower_slices; return}
 if {$dy == 0} {
   if {$dx >= 0} {set angle 0} else {set angle 3.14159265}
  } else {set angle [expr acos($dx/$dist)]
          if {$dy < 0} {set angle [expr 2*3.14150265 - $angle]}
         }
 set nb_slice_tmp [expr [$this(actual_Menu_item) Nb_sub_item_num]-1]
   if {$nb_slice_tmp < 1} {set nb_slice_tmp 1}
 set slice    [expr 2*3.14159265/$nb_slice_tmp]
 set num      [expr int( ($angle + $slice/2.0) /  $slice ) % $nb_slice_tmp]
 
 set menu_candidate [$this(actual_Menu_item) Get_sub_item_num $num]
# Place the submenu or trigger the item
 if {[$menu_candidate Has_sub_menus]} {
   #puts "<Go_to_submenu>"
   this Info_Goto_sub_menu_index $num
   $this(actual_Display_Menu_item) UnDisplay
   set this(actual_Menu_item) $menu_candidate
   set this(starting_point) "$x $y"
   this Go_to_submenu $this(actual_Menu_item)
   set this(actual_Display_Menu_item) [this Get_a_stroke_menu $this(actual_Menu_item) $x $y]
     set color    [lindex $this(L_colors) [expr ($num-1)%$nb_slice_tmp]]; 
	 set L_colors ""; for {set i 0} {$i < 9} {incr i} {lappend L_colors $color}
     #XXX $this(actual_Display_Menu_item) set_L_colors $L_colors
	 $this(actual_Display_Menu_item) set_L_colors $this(L_colors)
	 $this(actual_Display_Menu_item) Update
	 $this(actual_Display_Menu_item) set_Display_Menu_parent $objName
	 if {[this get_mode_expert_activated]} {
	   $this(actual_Display_Menu_item) set_expert_mode_delay [this get_expert_mode_delay]
	   $this(actual_Display_Menu_item) Start_mode_expert
	  }
   #puts "</Go_to_submenu>"
  } else {
          if {[$ptr Appuie] == 0} {
            #puts "<Activate_item>"
            $this(actual_Display_Menu_item) UnDisplay
			set this(actual_Menu_item) $menu_candidate
			this Activate_item $this(actual_Menu_item)
            this Stop $this(current_ptr)
			this Stop_mode_expert
			#puts "</Activate_item>"
		   }
         }
}

#___________________________________________________________________________________________________________________________________________
method Automate_stroke_menu Lower_slices {} {
 foreach p $this(L_poly_slices) {$p Etirement 1 1}
}

#___________________________________________________________________________________________________________________________________________
method Automate_stroke_menu Bigger_slice {num} {
 this Lower_slices
 set nb [expr [[this get_menu_item] Nb_sub_item_num]-1]
 [lindex $this(L_poly_slices) [expr ($num-1)%$nb]] Etirement 1.1 1.1
}

#___________________________________________________________________________________________________________________________________________
method Automate_stroke_menu Do_switch_to_beginner_mode_or_not {} {
 #puts "Automate_stroke_menu :: $objName Do_switch_to_beginner_mode_or_not"
 if {$this(Display_Menu_parent) != ""} {
   if {[$this(Display_Menu_parent) get_actual_Display_Menu_item] == $this(Display_Menu_parent)} {return}
  }
  
 if {$this(actual_Display_Menu_item) != ""} {
   if {$this(actual_Display_Menu_item) != $objName} {return}
  }
 this inherited
}

#___________________________________________________________________________________________________________________________________________
method Automate_stroke_menu VisualizePoint {ptr x y {size 2}} {
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
method Automate_stroke_menu Reload {} {
 this inherited
 this Update
}

#___________________________________________________________________________________________________________________________________________
method Automate_stroke_menu Update {} {
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
 set nb_slices $n
 if {$n == 0} {set a 0} else {set a [expr 3.14159265 * 2 / $n]}
 
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

# Subdivide the background round
# Divide the polygone into slices, associate a color for each one
   set NB $nb_slices
   $this(poly_rond_bkgnd) Vider_peres
     if {$NB > 0} {
	   $this(n_root) Ajouter_fils $this(poly_rond_bkgnd)
	   $this(n_root) Position_des_fils_changeable 0
	   $this(poly_rond_bkgnd) Couleur 1 1 1 0.6
	   $this(poly_rond_bkgnd) Afficher_noeud 1
	  }
   foreach p $this(L_poly_slices) {$p Vider_peres}
   set PI 3.14159265; set RAYON 999999
   set L_colors $this(L_colors)
   if {$NB > 1} {
   $this(poly_rond_bkgnd) Couleur 0 0 0 0
   for {set i 0} {$i < $NB} {incr i} {
     set p [lindex $this(L_poly_slices) $i];
	 $p Etirement 1 1; $this(poly_rond_bkgnd) Ajouter_fils $p
	 if {$this(actual_number_of_slices) < $NB} {
	   set a0 [expr ($i+0.5)*2*$PI / $NB]
	   set a1 [expr ($i+1.5)*2*$PI / $NB]
	   set contour [ProcTabDouble [list 0 0 \
	                                    [expr $RAYON*cos($a0)] [expr $RAYON*sin($a0)] \
										[expr $RAYON*cos( ($a1+$a0)/2.0 )] [expr $RAYON*sin( ($a1+$a0)/2.0 )] \
										[expr $RAYON*cos($a1)] [expr $RAYON*sin($a1)] \
								  ]]
	   $p Ajouter_contour $contour; $p Origine 0 0; $p Noeud_touchable 0
	   Detruire $contour
	   $p Intersection $this(poly_rond_bkgnd) $this(poly_rond_bkgnd)
	  }
	 eval "$p Couleur [lindex $L_colors $i]"
    }
	 set this(actual_number_of_slices) $NB
	 this Set_text_help ""
	 this Set_text_title ""
    } else {
	        # Help 
            this Set_text_help  [$this(menu_item) get_help]
            set box [$this(text_help) Boite_noeud]
            $this(text_help) Origine -[$box Cx] -[$box HD_Y]
		   }
}
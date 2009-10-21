source stroke_menu.tcl

proc Bezier_of {L_points nb_discret} {
 set pos 0
 set nb_pt [expr [llength $L_points]/4]
 
 for {set i 0} {$i < ($nb_pt - 2)} {incr i} {
   set x1 [lindex $L_points $pos]; incr pos; set y1 [lindex $L_points $pos]; incr pos
   set x2 [lindex $L_points $pos]; incr pos; set y2 [lindex $L_points $pos]; incr pos
   set x3 [lindex $L_points $pos]; incr pos; set y3 [lindex $L_points $pos]; incr pos
   incr pos -2
# foreach {x1 y1 x2 y2 x3 y3} $L_points {}
   if {[string equal $x2 ""]} {set x2 $x1; set y2 $y1}
   if {[string equal $x3 ""]} {set x3 $x2; set y3 $y2}
   set nb_pt_to_interpolate [expr 1 + $nb_discret * (sqrt(($x2-$x3)*($x2-$x3) + ($y2-$y3)*($y2-$y3)) + sqrt(($x2-$x1)*($x2-$x1) + ($y2-$y1)*($y2-$y1)))]
   for {set j 0} {$j <= $nb_pt_to_interpolate} {incr j} {
     set t [expr double($j) / double($nb_pt_to_interpolate)]
	 set pt_x12 [expr $x1+$t*($x2-$x1)]; set pt_x23 [expr $x2+$t*($x3-$x2)]
	 set pt_y12 [expr $y1+$t*($y2-$y1)]; set pt_y23 [expr $y2+$t*($y3-$y2)]
	 lappend L_rep [expr $pt_x12 + $t*($pt_x23 - $pt_x12)] [expr $pt_y12 + $t*($pt_y23 - $pt_y12)]
    }
  }
  
 return $L_rep
}

#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
method B207_Helper constructor {{avatar Helpers/EDM_Avatar.png}} {

 set this(Pool_circle)    [list ]
 set this(contour_circle) [ProcOvale 0 0 11 11 60]
 set this(root)          [B_noeud]
 set this(root_for_hands) [B_noeud]  ; set this(MANIP_root_for_hands) [B_noeud]
 set this(poly_L_hand)   [B_polygone]; set this(MANIP_poly_L_hand) [B_polygone]
 set this(poly_R_hand)   [B_polygone]; set this(MANIP_poly_R_hand) [B_polygone]
 set this(pt_tmp)        [B_point]
 set this(avatar)        [B_image $avatar]; $this(avatar) Translucidite 1
 set this(rap_press_avatar) [B_rappel [Interp_TCL]]
   set this(help_mode) HELP; 
   $this(avatar) abonner_a_detection_pointeur [$this(rap_press_avatar) Rappel] [ALX_pointeur_enfonce]
 set this(poly_bkgrnd) [B_polygone]; $this(poly_bkgrnd) Ajouter_contour [ProcRect 0 0 [N_i_mere Largeur] [N_i_mere Hauteur]]
 set this(rap_press_bkgrnd) [B_rappel [Interp_TCL]]
   $this(poly_bkgrnd) abonner_a_detection_pointeur [$this(rap_press_bkgrnd) Rappel] [ALX_pointeur_enfonce]
   B_configure $this(MANIP_root_for_hands) Ajouter_fils $this(MANIP_poly_L_hand) \
                                          -Ajouter_fils $this(MANIP_poly_R_hand)
   B_configure $this(root_for_hands) Ajouter_fils $this(poly_L_hand) \
                                    -Ajouter_fils $this(poly_R_hand)
   B_configure $this(root) Ajouter_fils $this(root_for_hands) \
						  -Ajouter_fils $this(avatar) \
						  -Ajouter_fils $this(poly_bkgrnd) \
						  -Position_des_fils_changeable 0
						  
# Menu to choose help
 set this(root_menu_help) ${objName}_Item_root_of_menu_help
   Menu_item $this(root_menu_help) "Help" "Display contextual help" "Touch something on screen to display its contextual help" [list]
 set this(cancel_menu_help) ${objName}_Item_cancel_of_menu_help
   Menu_item $this(cancel_menu_help) "Cancel" "Cancel" "Cancel" [list]
 set this(help_menu) ${objName}_menu_help
   Automate_stroke_menu $this(help_menu) $this(poly_bkgrnd) $this(root_menu_help)
 set this(Pool_menu_item) ""
 set this(id_menu_item)   0

# Avoid to catch the pointers if triggered by itselve
 set this(rap_pre_traitement) [B_rappel [Interp_TCL]]
   $this(rap_pre_traitement) Texte "$objName Pre_traitement_on $this(MANIP_root_for_hands)"
   $this(MANIP_root_for_hands) Rap_pre_traitement_contient [$this(rap_pre_traitement) Rappel]
 set this(rap_post_traitement) [B_rappel [Interp_TCL]]
   $this(rap_post_traitement) Texte "$objName Post_traitement_on $this(MANIP_root_for_hands)"
   $this(MANIP_root_for_hands) Rap_post_traitement_contient [$this(rap_post_traitement) Rappel]

# Sim pointers
 set this(L_ptr_id)           [list ]
 set this(L_pressed_pointers) [list ]
 catch "set this(sim_ptr_TCL) [B_sim_ptr_TCL [N_i_mere Ecran_X] [N_i_mere Ecran_Y] [N_i_mere Coin_BG] [N_i_mere Vecteur_X] [N_i_mere Vecteur_Y]]"
 if {[catch "set this(id_ptr_TCL)  [$this(sim_ptr_TCL) Ajouter_rapporteur [B_sim_ptr] [N_i_mere Noeud_scene] 0]"]} {
   set this(B207_on) 0
  } else {$this(sim_ptr_TCL) Noeud_pere [N_i_mere Noeud_scene]
          set this(B207_on) 1
		  set this(L_MuTable_evt) {}
		  set this(L_MuTable_ids) {}
		  set this(rap_ptr_TCL) [B_rappel [Interp_TCL] "if {\[catch {$objName B207_reading} err\]} {puts \"ERROR B207 reading MuTable:\n\$err\"}"]
		  $this(sim_ptr_TCL) abonner_a_simulation [$this(rap_ptr_TCL) Rappel]
		  set this(B207_evt_ptr)   [new_alx_evennement_pointeur]
          set this(B207_info_comp) [new_info_comp_evennement]
		  set this(B207_pt_tmp_coord) [B_point]
		  set this(B207_pt_tmp_dep)   [B_point]
		  set this(L_ptr_preso_nodes) {}
		 }

# Standard initialisation
 this init

 this Hands_touchable 0
 this Press_on_Avatar
}

#___________________________________________________________________________________________________________________________________________
method B207_Helper init {} {
 $this(poly_R_hand) Vider
 #$this(poly_R_hand) Ajouter_contour [ProcTabDouble "0 0 -50 0 -100 50 -40 20 -20 120 -10 50 0 130 10 40 20 120 40 50 60 80 50 0"]
 set L_pt [list   0    0 -50 0 \
               -100   50 -90 60 \
                -50   35 -30 70 \
				-20  120 -12 50 \
				-10   50 -5 130 \
				  0  130  5 40 \
				 10   40 15 120 \
				 20  120 35 50 \
				 40   50 55 75 \
				 60   80 70 50 \
				 30  0 100 0 \
				 50 0 50 0]
 $this(poly_R_hand) Ajouter_contour [ProcTabDouble [Bezier_of $L_pt 0.1]]
   $this(poly_R_hand) Ex 1
 $this(poly_L_hand) Maj_poly $this(poly_R_hand); $this(poly_L_hand) Ex -1
 set this(pt_thumb)  "-100 50"
 set this(pt_index)  "-20 120"
 set this(pt_middle) "0 130"
 set this(pt_ring)   "20 120"
 set this(pt_little) "60 80"
 set this(pt_wrist)  "0 10 33 20"
  
 set this(Pool_CB)   ""
 set this(Pool_node) ""

# Manipulations poly
 $this(MANIP_poly_L_hand) Maj_poly $this(poly_L_hand)
  # Callback for positionning the original hand
   set rap [B_rappel [Interp_TCL] "$objName Adjust_hand_to_MANIP L"]
   set this(rap_MANIP_poly_L_hand) $rap
   $this(MANIP_poly_L_hand) abonner_a_LR_parcours [$this(MANIP_poly_L_hand) LR_Av_pre_rendu] [$rap Rappel]
  # Polygon for gragging the hand only 
   set poly_drag [B_polygone]; $poly_drag Ajouter_contour [ProcOvale 0 35 40 25 60]; $poly_drag Couleur 0.3 0.3 0.3 1
   $this(MANIP_poly_L_hand) Ajouter_fils_au_debut $poly_drag  ; 
   $poly_drag Intersection $this(MANIP_poly_L_hand) $this(MANIP_poly_L_hand)
  # Polygones for fingers
   foreach finger [list thumb index middle ring little wrist] {
     set x [lindex $this(pt_$finger) 0]; set y [lindex $this(pt_$finger) 1]
	 set dx [lindex $this(pt_$finger) 2]; if {$dx == ""} {set dx 13}
	 set dy [lindex $this(pt_$finger) 3]; if {$dy == ""} {set dy 13}
	 set pol_finger [B_polygone]; $pol_finger Origine $x $y
	 $pol_finger Ajouter_contour [ProcOvale 0 0 $dx $dy 60]; $pol_finger Couleur 0 1 0 0.66
	 $this(MANIP_poly_L_hand) Ajouter_fils_au_debut $pol_finger; 
	 set rap [B_rappel [Interp_TCL] "$objName Switch_PressRelease $pol_finger L $finger"];
	 $pol_finger abonner_a_detection_pointeur [$rap Rappel] [ALX_pointeur_enfonce]
	 $pol_finger Ajouter_MetaData_T finger    $finger
	 $pol_finger Ajouter_MetaData_T PRESS     0
	 $pol_finger Ajouter_MetaData_T rap_press $rap
    }
  # For the wrist (== pol_finger)
   $pol_finger Difference $poly_drag
   
  # Contacts manager  
   B_contact ctc_$this(MANIP_poly_L_hand) "$this(MANIP_poly_L_hand) 3" -add "$poly_drag 1"
   
 $this(MANIP_poly_R_hand) Maj_poly $this(poly_R_hand)
  # Callback for positionning the original hand  
   set rap [B_rappel [Interp_TCL] "$objName Adjust_hand_to_MANIP R"]
   set this(rap_MANIP_poly_R_hand) $rap
   $this(MANIP_poly_R_hand) abonner_a_LR_parcours [$this(MANIP_poly_R_hand) LR_Av_pre_rendu] [$rap Rappel]
  # Polygon for gragging the hand only 
   set poly_drag [B_polygone]; $poly_drag Ajouter_contour [ProcOvale 0 35 40 25 60]; $poly_drag Couleur 0.3 0.3 0.3 1
   $this(MANIP_poly_R_hand) Ajouter_fils_au_debut $poly_drag  ; 
   $poly_drag Intersection $this(MANIP_poly_R_hand) $this(MANIP_poly_R_hand)
  # Polygones for fingers
   foreach finger [list thumb index middle ring little wrist] {
     set x [lindex $this(pt_$finger) 0]; set y [lindex $this(pt_$finger) 1]
	 set dx [lindex $this(pt_$finger) 2]; if {$dx == ""} {set dx 13}
	 set dy [lindex $this(pt_$finger) 3]; if {$dy == ""} {set dy 13}
	 set pol_finger [B_polygone]; $pol_finger Origine $x $y
	 $pol_finger Ajouter_contour [ProcOvale 0 0 $dx $dy 60]; $pol_finger Couleur 0 1 0 0.66
	 $this(MANIP_poly_R_hand) Ajouter_fils_au_debut $pol_finger; 
	 set rap [B_rappel [Interp_TCL] "$objName Switch_PressRelease $pol_finger R $finger"];
	 $pol_finger abonner_a_detection_pointeur [$rap Rappel] [ALX_pointeur_enfonce]
	 $pol_finger Ajouter_MetaData_T finger    $finger
	 $pol_finger Ajouter_MetaData_T PRESS     0
	 $pol_finger Ajouter_MetaData_T rap_press $rap
    }
  # For the wrist (== pol_finger)
   $pol_finger Difference $poly_drag
   
  # Contacts manager  
   B_contact ctc_$this(MANIP_poly_R_hand) "$this(MANIP_poly_R_hand) 3" -add "$poly_drag 1"

# Generate pointers informations related to fingers
 foreach h [list L R] {
   foreach f [list thumb index middle ring little wrist] {
     set this(ptrID_${h}_hand_$f) ""
    }
  }
 
 set this(L_elements)  [list $this(poly_L_hand) $this(poly_R_hand)]
 
# Subscribe to avatar
 $this(rap_press_avatar) Texte "$objName Press_on_Avatar"
 $this(rap_press_bkgrnd) Texte "if {\[catch \"$objName Help_asked_by $this(rap_press_bkgrnd)\" err\]} {puts \$err}"
}

#___________________________________________________________________________________________________________________________________________
method B207_Helper Adjust_hand_to_MANIP {hand_LR} {
 set hand  $this(poly_${hand_LR}_hand)
 set manip $this(MANIP_poly_${hand_LR}_hand) 
 $hand Origine [$manip Origine]; $hand Rotation [$manip Rotation]
 set fingers ""
 foreach e $this(L_pressed_pointers) {
   if {[lindex $e 0] == $hand_LR} {lappend fingers [lindex $e 1]}
  }
 
 if {$fingers != ""} {this Place $hand_LR $fingers [$manip Px] [$manip Py]}
} 

#___________________________________________________________________________________________________________________________________________
method B207_Helper Switch_PressRelease {pol_finger hand_LR finger} {
 set PRESS [expr 1 - [$pol_finger Val_MetaData PRESS]]
 $pol_finger Ajouter_MetaData_T PRESS $PRESS
 $pol_finger Couleur $PRESS [expr 1-$PRESS] 0 0.66
 eval [$this(rap_MANIP_poly_${hand_LR}_hand) Texte]
 
 foreach e $this(L_elements) {
   $e Ajouter_MetaData_T B207_Helper_en_cours 0
   $e Ajouter_MetaData_T B207_Helper          ""
  }

 if {$PRESS} {this Press $hand_LR $finger} else {this Release $hand_LR $finger}
 eval "$objName Exec R index {$this(root_menu_help) set_L_subitem \"\"; $this(help_menu) Update}"
}

#___________________________________________________________________________________________________________________________________________
Generate_accessors B207_Helper [list MANIP_root_for_hands MANIP_poly_R_hand MANIP_poly_L_hand poly_L_hand poly_R_hand avatar help_mode poly_bkgrnd root root_for_hands]

#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
method B207_Helper get_a_menu_item {title cmd help L_subitems} {
 if {[string equal $this(Pool_menu_item) ""]} {
   set rep ${objName}_menu_item_$this(id_menu_item)
     Menu_item $rep $title $cmd $help $L_subitems
   incr this(id_menu_item)
  } else {set rep [lindex $this(Pool_menu_item) 0]
          set this(Pool_menu_item) [lrange $this(Pool_menu_item) 1 end]
		  $rep set_title     $title
		  $rep set_cmd       $cmd
		  $rep set_help      $help
		  $rep set_L_subitem $L_subitems
         }
 return $rep
}

#___________________________________________________________________________________________________________________________________________
method B207_Helper Release_menu_items {Lm} {
 foreach m $Lm {
   if {[string equal $m $this(cancel_menu_help)]} {continue}
   if {[lsearch $this(Pool_menu_item) $m] == -1} {
     lappend this(Pool_menu_item) $m
	 $m set_L_subitem ""
    }
  }
}

#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
method B207_Helper get_a_presentation_node {} {
 if {[string equal $this(Pool_node) ""]} {
   set rep [B_noeud]
  } else {set rep [lindex $this(Pool_node) 0]
          set this(Pool_node) [lrange $this(Pool_node) 1 end]
         }
 return $rep
}

#___________________________________________________________________________________________________________________________________________
method B207_Helper Release_presentation_node {n} {
 if {[lsearch $this(Pool_node) $n] != -1} {return}
 lappend this(Pool_node) $n
}

#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
method B207_Helper Plug_under {n} {
 $n Ajouter_fils_au_debut $this(root)
}

#___________________________________________________________________________________________________________________________________________
method B207_Helper get_a_CB {} {
 if {[string equal $this(Pool_CB) ""]} {
   set rep [B_rappel [Interp_TCL]]
  } else {set rep [lindex $this(Pool_CB) 0]
          set this(Pool_CB) [lrange $this(Pool_CB) 1 end]
         }
}

#___________________________________________________________________________________________________________________________________________
method B207_Helper Release_CB {cb} {
 lappend this(Pool_CB) $cb
}

#___________________________________________________________________________________________________________________________________________
method B207_Helper Hands_touchable {b} {
 $this(root_for_hands) Gerer_contacts $b
}

#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
method B207_Helper Press_on_Avatar {} {
 if {[string equal $this(help_mode) NO_HELP]} {
   set this(help_mode) HELP
   B_configure $this(poly_bkgrnd) -Couleur 0 0 0 0.6 \
                                  -Gerer_contacts 1  \
								  -Afficher_noeud 1
  } else {set this(help_mode) NO_HELP
          B_configure $this(poly_bkgrnd) -Couleur 0 0 0 0.6 \
                                         -Gerer_contacts 0  \
				    	                 -Afficher_noeud 0
         }
}

#___________________________________________________________________________________________________________________________________________
method B207_Helper get_val {name L} {
 foreach {att val} $L {
   if {[string equal $name $att]} {return $val}
  }
 return ""
}

#___________________________________________________________________________________________________________________________________________
method B207_Helper Play_scenario {n scenar_id} {
 set obj    $n
 set helper $objName
 this Press_on_Avatar
 eval [Get_scenario $objName $n $scenar_id]
}

#___________________________________________________________________________________________________________________________________________
method B207_Helper Help_asked_by {rap} {
 set infos [Void_vers_info [$rap Param]]
 set pt [$infos Point_au_contact]

# Free the current help menu
 this Release_menu_items [$this(root_menu_help) Get_recursive_submenus]

# Where did we pressed?
 set b [$this(root) Gerer_contacts]
   $this(root) Gerer_contacts 0
   set Noeud_scene [N_i_mere Noeud_scene]
   set new_infos [$Noeud_scene Contient $pt 0]
   if {![string equal $new_infos NULL]} {
     set noeud [Real_class [$new_infos NOEUD]]
	 set L_menus ""
	 foreach n [Invert_list [Ancetre_de_a $noeud $Noeud_scene]] {
	   set help [$n Val_MetaData help]
	   if {![string equal $help ""]} {
	     puts "Help on $n"
		# Build submenus corresponding to scenarios
		 set L_sm ""
		 foreach scenar [lindex $help 2] {
		   set title     [this get_val name [lindex $scenar 1]]
		   set scenar_id [this get_val id   [lindex $scenar 1]]
		   puts "  - $title"
		   lappend L_sm [this get_a_menu_item $title "puts {$title}; $objName Play_scenario $n $scenar_id" "" [list]]
		  } 
		 lappend L_menus [this get_a_menu_item [$n Nom_IU] "puts $n" "Help about B207 node $n" "$L_sm $this(cancel_menu_help)"]
		}
	  }
	# Build the corresponding menu
	 $this(root_menu_help) set_L_subitem "$L_menus $this(cancel_menu_help)"
	 $this(help_menu)      Display [$pt X] [$pt Y]
	 $this(help_menu)      Update
	# Delete the B207 contact information new_infos
	 Detruire $new_infos
    }
 $this(root) Gerer_contacts $b
}

#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
method B207_Helper Do args {
 foreach e $this(L_elements) {
   $e Ajouter_MetaData_T B207_Helper_en_cours 0
   $e Ajouter_MetaData_T B207_Helper          ""
  }
 set mx [expr [N_i_mere Largeur]/2]
 eval "$objName $args Move_hand L index 1000 [expr $mx - 100] 100 Move_hand R index 1000 [expr $mx + 100] 100 Exec R index {$this(root_menu_help) set_L_subitem \"\"; $this(help_menu) Update}"
}

#___________________________________________________________________________________________________________________________________________
method B207_Helper Place {hand fingers x y} {
 set poly_hand $this(poly_${hand}_hand)

# Place the polygone hand at the right position/place
 $poly_hand Origine $x $y
 
#XXX If finger == "" Look at every active finger, i.e. related to a pointer, and trigger a movment event
#XXX If finger == xx triger an event just for the pointer related to finger xx...
 if {[string equal $fingers ""]} {set fingers [list thumb index middle ring little]}
 
# Trigger eventually a movment event
 foreach f $fingers {
   if { ![string equal $this(ptrID_${hand}_hand_$f) ""]} {
    # Get the related pointer
     set id $this(ptrID_${hand}_hand_$f)
	 if {$id == ""} {continue}
	 set ptr [B_sim_ptr Pointeur_id $id]
    # Get previous position of the pointer
	 set ox [$ptr X]; set oy [$ptr Y]
	# Get current position related to the finger f
	   set f_pt $this(pt_$f)
	   $this(pt_tmp) maj [lindex $f_pt 0] [lindex $f_pt 1]
	   $poly_hand Changer_coordonnees_prim_inverse $this(pt_tmp)
	 set px [$this(pt_tmp) X]; set py [expr [N_i_mere Hauteur] - [$this(pt_tmp) Y]]
	# Compute delta
     set dx [expr $px - $ox]; set dy [expr $py - $oy]
	# Build and trigger movment event
     $this(sim_ptr_TCL) convertir_coordonnees $px $py $this(B207_pt_tmp_coord)
     $this(sim_ptr_TCL) convertir_coordonnees $dx $dy $this(B207_pt_tmp_dep)
     $this(B207_info_comp) Dep   $this(B207_pt_tmp_dep)
     $this(B207_info_comp) Coord $this(B207_pt_tmp_coord)
     $this(B207_evt_ptr) maj $id [ALX_pointeur_mouvement] $this(B207_info_comp) [$this(sim_ptr_TCL) Noeud_pere]
     B_sim_ptr Ajouter_evt $this(B207_evt_ptr)
    }
  }
}

#___________________________________________________________________________________________________________________________________________
method B207_Helper Move_hand {hand_LR finger t x y args} {
#puts "$objName Move_hand $hand_LR $finger $t $x $y"
 set hand   $this(poly_${hand_LR}_hand)
   set action "$hand $finger $t $x $y $args"
  # Still doing something ?
   if {[$hand Val_MetaData B207_Helper_en_cours]} {
     if {[string equal $args ""]} {return 0}
	 return [eval "$objName $args"]
	}
  # Already done ? 
   set val    [$hand Val_MetaData B207_Helper]
   if {[string first $val $action] != -1} {
     if {![string equal $args ""]} {
	   return [eval "$objName $args"]
	  } else {return 0}
	}
   $hand Ajouter_MetaData_T B207_Helper          $action
   $hand Ajouter_MetaData_T B207_Helper_en_cours 1
 set transfo [B_transfo $t]
 set rap     [this get_a_CB]
   set finger_pt $this(pt_${finger})
   set fx [lindex $finger_pt 0]; set fy [lindex $finger_pt 1]
   set ox [$hand Px]        ; set oy [$hand Py]
   $this(pt_tmp) maj $fx $fy
   $hand Changer_coordonnees_prim_inverse $this(pt_tmp)
   set tx [expr $x - [$this(pt_tmp) X] + $ox]   ; set ty [expr $y - [$this(pt_tmp) Y] + $oy]
   $rap Texte "set v \[$transfo V_courant\]; $objName Place $hand_LR {} \[expr \$v * $tx + (1-\$v) * $ox\] \[expr \$v * $ty + (1-\$v) * $oy\]"
   $transfo abonner_a_rappel_pendant [$rap Rappel]
   $transfo Demarrer
   N_i_mere Ajouter_deformation_dynamique $transfo

 set rap [this get_a_CB]
   $transfo abonner_a_rappel_fin [$rap Rappel]
 set cmd "$hand Ajouter_MetaData_T B207_Helper_en_cours 0; "
 if {![string equal $args ""]} {
   eval "$objName $args"
   append cmd "$objName $args"
  }
 $rap Texte $cmd
 
 return 1
}

#___________________________________________________________________________________________________________________________________________
method B207_Helper Exec {hand_LR finger cmd args} {
 set hand   $this(poly_${hand_LR}_hand)
   set action "$hand_LR $finger $args"
  # Still doing something ?
   if {[$hand Val_MetaData B207_Helper_en_cours]} {
     if {[string equal $args ""]} {return 0}
	 return [eval "$objName $args"]
	}
  # Already done ? 
   set val    [$hand Val_MetaData B207_Helper]
   if {[string first $val $action] != -1} {return 0}
   $hand Ajouter_MetaData_T B207_Helper          $action
   
 eval $cmd
 eval "$objName $args"
 return 1
}

#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
method B207_Helper Release_circle {c} {
 $c Vider_peres
 if {[lsearch $this(Pool_circle) $c] == -1} {lappend this(Pool_circle) $c}
}

#___________________________________________________________________________________________________________________________________________
method B207_Helper get_a_circle {} {
 if {![string equal $this(Pool_circle) ""]} {
   set rep [lindex $this(Pool_circle) 0]
   set this(Pool_circle) [lrange $this(Pool_circle) 1 end]
  } else {set rep [B_polygone]
         }
 $rep Vider
   $rep Couleur 1 0.3 0.3 0.72
   $rep Ajouter_contour $this(contour_circle)
 return $rep
}

#___________________________________________________________________________________________________________________________________________
# Ajouter un rond au niveau des doigts qui appuient...
#___________________________________________________________________________________________________________________________________________
method B207_Helper Press {hand_LR finger args} {
 puts "$objName Press $hand_LR $finger"
 set hand   $this(poly_${hand_LR}_hand)
   set action "$hand_LR $finger $args"
  # Still doing something ?
   if {[$hand Val_MetaData B207_Helper_en_cours]} {
     if {[string equal $args ""]} {puts "Still doing something"; return 0}
	 return [eval "$objName $args"]
	}
  # Already done ? 
   set val    [$hand Val_MetaData B207_Helper]
   if {[string first $val $action] != -1} {puts "Still done"; return 0}
   $hand Ajouter_MetaData_T B207_Helper          $action

 set id $this(ptrID_${hand_LR}_hand_$finger)
 if {[string equal $id ""]} {
   set id [B_sim_ptr Ajouter_pointeur "Helper $objName pointer" [N_i_mere Noeud_scene] 1]
   set this(ptrID_${hand_LR}_hand_$finger) $id
   lappend this(L_ptr_id) $id; puts "Added pointer $id for $finger"
   set ptr [B_sim_ptr Pointeur $id] 
     $ptr Add_MetaData W [lindex $this(pt_$finger) 2]
	 $ptr Add_MetaData H [lindex $this(pt_$finger) 3]
  }
 if {[lsearch $this(L_pressed_pointers) "$hand_LR $finger $id"] == -1} {lappend this(L_pressed_pointers) "$hand_LR $finger $id"}
 
 set hand $this(poly_${hand_LR}_hand)
 $this(B207_pt_tmp_coord) maj [lindex $this(pt_${finger}) 0] [lindex $this(pt_${finger}) 1]
   $hand Changer_coordonnees_prim_inverse $this(B207_pt_tmp_coord)
 this Place $hand_LR $finger [$hand Px] [$hand Py]
 $this(B207_info_comp)    Coord       $this(B207_pt_tmp_coord)
 $this(B207_info_comp)    Type_appuie 1
 $this(B207_evt_ptr) maj $id [ALX_pointeur_enfonce] $this(B207_info_comp) [$this(sim_ptr_TCL) Noeud_pere]
 B_sim_ptr Ajouter_evt $this(B207_evt_ptr)

# Visual Feedback !
   set poly_circle [this get_a_circle]
   $hand Ajouter_fils_au_debut $poly_circle
   $poly_circle Origine [lindex $this(pt_${finger}) 0] [lindex $this(pt_${finger}) 1]
   $hand Ajouter_MetaData_T "${finger}_poly_press" $poly_circle
 
 if {![string equal $args ""]} {eval "$objName $args"}
 puts "Done in [$this(B207_pt_tmp_coord) X] [$this(B207_pt_tmp_coord) Y]"
 return 1
}

#___________________________________________________________________________________________________________________________________________
method B207_Helper Release {hand_LR finger args} {
#puts "$objName Release $hand_LR $finger"
 set hand   $this(poly_${hand_LR}_hand)
   set action "$hand_LR $finger $args"
  # Still doing something ?
   if {[$hand Val_MetaData B207_Helper_en_cours]} {
     if {[string equal $args ""]} {return 0}
	 return [eval "$objName $args"]
	}
  # Already done ? 
   set val    [$hand Val_MetaData B207_Helper]
   if {[string first $val $action] != -1} {return 0}
   $hand Ajouter_MetaData_T B207_Helper          $action

 set id $this(ptrID_${hand_LR}_hand_$finger)
 if {[string equal $id ""]} {return 0}
 set this(L_pressed_pointers) [lremove $this(L_pressed_pointers) "$hand_LR $finger $id"]
 
 set ptr [B_sim_ptr Pointeur_id $id]
 $this(B207_pt_tmp_coord) maj [lindex $this(pt_${finger}) 0] [lindex $this(pt_${finger}) 1]
   set hand $this(poly_${hand_LR}_hand)
   $hand Changer_coordonnees_prim_inverse $this(B207_pt_tmp_coord)
 this Place $hand_LR $finger [$hand Px] [$hand Py]
 $this(B207_info_comp) Coord       $this(B207_pt_tmp_coord)
 $this(B207_info_comp) Type_appuie 1
 $this(B207_evt_ptr) maj $id [ALX_pointeur_relache] $this(B207_info_comp) [$this(sim_ptr_TCL) Noeud_pere]
 B_sim_ptr Ajouter_evt $this(B207_evt_ptr)
 
# Visual Feedback !
   set poly_circle [$hand Val_MetaData "${finger}_poly_press"]
   if {![string equal $poly_circle ""]} {
     this Release_circle $poly_circle
	 $hand Retirer_MetaData_T "${finger}_poly_press"
    }
 
 this Release_presentation_node [$ptr Noeud_representation]
 #DEBUG B_sim_ptr Retirer_pointeur $id 
 #DEBUG set this(ptrID_${hand_LR}_hand_$finger) ""
 
 if {![string equal $args ""]} {eval "$objName $args"}
 
 return 1
}

#helpr Do Move_hand R index 1000 700 400 Move_hand L index 2000 200 400 SYNCHRO Move_hand R index 1000 200 300 Move_hand L index 2000 700 300
#___________________________________________________________________________________________________________________________________________
method B207_Helper SYNCHRO args {
 foreach e $this(L_elements) {
   if {[$e Val_MetaData B207_Helper_en_cours]} {return 0}
  }
  
 if {![string equal $args ""]} {eval "$objName $args"}
 return 1
}


#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
method B207_Helper Pre_traitement_on {node} {
 set evt [B_sim_couche Evt_courant]
 set ptr [$evt Ptr]
 set id  [$ptr Id]
 
 set rep 1
 foreach i $this(L_ptr_id) {
   if {$i == $id} {set rep 0; break}
  }
 $node Pre_traitement_contient $rep
}

#___________________________________________________________________________________________________________________________________________
method B207_Helper Post_traitement_on {node} {
 if {![$node Pre_traitement_contient]} {$node Contenant_courant NULL}
}


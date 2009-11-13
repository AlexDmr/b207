source quickmenu.tcl
inherit Automate_Menu_Main Automate_quickmenu

#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
# Génération des items du menu en lien avec le logiciel de dessin
 proc Complete_with {L_name menu multiple} {
   upvar $L_name L
   set nb [expr ($multiple - [llength $L]%$multiple) % $multiple]
   for {set i 0} {$i < $nb} {incr i} {lappend L $menu}
  }

#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
method Automate_Menu_Main constructor {workspace Menu_item {side right}} {
 set this(starting_children_index) 0
 set this(nb_pol_for_item_display) 0
 set this(L_text_sub_items)        {}
 set this(pt_tmp)                  [B_point]
 set this(L_slices_thumb)          [list]
 set this(thumb_zone)              ""

 set this(Pool_L_repere)       [list]
 set this(L_repere_actives)    [list]
 set this(Pool_polygones)      [list]
 set this(L_polygones_actives) [list]
 set this(rap_press_menu) [B_rappel [Interp_TCL]]
   $this(rap_press_menu) Texte "$objName Press_zone $this(rap_press_menu)"
 this inherited $workspace $Menu_item

 set this(rap_TAB_press)  [B_rappel [Interp_TCL]]
   $this(rap_TAB_press) Texte "$objName Position_menu $this(rap_TAB_press)"
   $workspace abonner_a_detection_pointeur [$this(rap_TAB_press) Rappel] [expr [ALX_pointeur_enfonce]+[ALX_pointeur_apparition]]
  
 set this(rap_change_ptr) [B_rappel [Interp_TCL]]
   $this(rap_change_ptr) Texte "$objName Change_in_ptr $this(rap_change_ptr)"
   
 set this(original_menu_item) $Menu_item
 set this(L_menu_roots_and_starting_item_index) ""
 
 set this(L_colors) [list "1 0 0 1" \
                      "0 1 0 1" \
					  "0 0 1 1" \
					  "1 1 0 1" \
					  "1 0 1 1" \
					  "0 1 1 1" \
					  "1 0.5 0.15 1" \
					  "1 1 1 1" \
				]

}

#___________________________________________________________________________________________________________________________________________
Generate_accessors Automate_Menu_Main [list thumb_zone starting_children_index nb_pol_for_item_display L_colors]

#___________________________________________________________________________________________________________________________________________
method Automate_Menu_Main set_delta_for_label {num x y} {
 set n [this get_label $num] 
 $n Ajouter_MetaData_T delta_x $x
 $n Ajouter_MetaData_T delta_y $y
 this Update
}

#___________________________________________________________________________________________________________________________________________
method Automate_Menu_Main get_label {num} {
 set t_i_p [lindex $this(L_text_sub_items) $num]
     set b_txt [lindex $t_i_p 0]; 
	 set b_pol [lindex $t_i_p 1];
	 
 return $b_txt
}

#___________________________________________________________________________________________________________________________________________
method Automate_Menu_Main Display {x y} {
 this inherited $x $y
 $this(n_root) Gerer_contacts 1
}

#___________________________________________________________________________________________________________________________________________
method Automate_Menu_Main Position_menu {rap} {
 set infos [Void_vers_info [$rap Param]]
 set noeud [Real_class [$infos NOEUD]]
 set t_i_p [lindex $this(L_text_sub_items) 0]
   set b_pol [lindex $t_i_p 1]
   
 if {$noeud == $b_pol} {
   set ptr [$infos Ptr]
   if { ([$ptr Appuie] == 0) || [$ptr Val_MetaData ${objName}_cmd_to_exec] != "" || [$ptr Val_MetaData ${objName}_thumb] != ""} {return}
   $ptr Add_MetaData ${objName}_thumb $b_pol
   this Switch_L_items $b_pol $ptr
  } 
 return
 
 set infos [Void_vers_info [$rap Param]]
 set noeud [Real_class [$infos NOEUD]] 
 if {![string equal $noeud $this(input_region)]} {return}
 $this(n_root) Origine [$infos Point_au_contact]
}

#___________________________________________________________________________________________________________________________________________
method Automate_Menu_Main Change_in_ptr {rap} {
 #puts "Change_in_ptr"
 set         ptr [Void_vers_alx_pointeur [$rap Param]]
 set       noeud [$ptr Noeud_sous_pointeur]; 
 set       noeud [Real_class $noeud]
 set cmd_to_exec [$ptr Val_MetaData ${objName}_cmd_to_exec]
 set small_label      0
 set   big_label      0
 set finish_subscribe 0
 set trigger_item     0
 
# Is it the right pointer corresponding to the right node?
 if {$noeud == [$ptr Val_MetaData ${objName}_noeud]} {
   set big_label 1 
   # Is there a particular command to eval? (Used for thumb...)
   if {$cmd_to_exec != ""} {
    # If we release then count if it is like a double click, then come back...
     if {[$ptr Appuie] == 0} {
	   set info_clic [$noeud Val_MetaData ${objName}_info_clic]
	   if {[string equal $info_clic ""]} {puts "No info clic!"; return}
	   set nb [lindex $info_clic 1]
	   set ms [N_i_mere ms]
	   if {($ms - [lindex $info_clic 0]) < 300} {
	     if {$nb >= 1} {
		   this Back
		   $noeud Ajouter_MetaData_T ${objName}_info_clic [list $ms 0]
		  } else {$noeud Ajouter_MetaData_T ${objName}_info_clic [list $ms [expr $nb + 1]]
		          eval $cmd_to_exec
				 }
	    } else {$noeud Retirer_MetaData_T ${objName}_info_clic
		        eval $cmd_to_exec
			   }
	  } else {eval $cmd_to_exec}
    } 
   # Is it a release so we have to activate the menu item?
   if {[$ptr Appuie] == 0} {
     set small_label      1
	 set   big_label      0
	 set finish_subscribe 1
	 set trigger_item     1
    }
  } else {set small_label 1
          if {[$ptr Appuie] == 0} {set finish_subscribe 1} else {eval $cmd_to_exec}
         }

# Process
 if {$small_label}      {this Lower_label_of  [$ptr Val_MetaData ${objName}_noeud]
				        }
 if {$big_label}        {this Bigger_label_of [$ptr Val_MetaData ${objName}_noeud]
                        }
 if {$trigger_item}     {set item [$noeud Val_MetaData item];
                         if {$item != ""} {this Goto_item $item
	                                       this Anim_triggered_label_of $noeud 2 4
										   puts "  Click on item $item"
	                                      }
                        }
 if {$finish_subscribe} {$ptr Sub_MetaData ${objName}_noeud
                         $ptr Sub_MetaData ${objName}_cmd_to_exec
						 $ptr Sub_MetaData ${objName}_L_repere
						 $ptr Sub_MetaData ${objName}_thumb
	                     $ptr desabonner_de_changement [$this(rap_change_ptr) Rappel]
	                     set L_repere [$ptr Val_MetaData ${objName}_L_repere]
	                     if {![string equal $L_repere {}]} {this Release_L_repere $L_repere
	                                                        this Sub_MetaData     ${objName}_L_repere
										                   }
                        }
}

#___________________________________________________________________________________________________________________________________________
method Automate_Menu_Main Back {} {
 puts "$objName Back"
 if {[llength $this(L_menu_roots_and_starting_item_index)] > 0} {
   set m [lindex $this(L_menu_roots_and_starting_item_index) end]
     set this(menu_item)               [lindex $m 0]
	 set this(starting_children_index) [lindex $m 1]
   set this(L_menu_roots_and_starting_item_index) [lrange $this(L_menu_roots_and_starting_item_index) 0 end-1]
   this Load_polygone_from_str this(menu_descr_str)
   #this Update
  } 
}

#___________________________________________________________________________________________________________________________________________
method Automate_Menu_Main Reload {} {
 this Load_polygone_from_str this(menu_descr_str)
}

#___________________________________________________________________________________________________________________________________________
method Automate_Menu_Main Goto_item {item} {
 this Focus_on      $item
 this Activate_item $item
 if {[$item Has_sub_menus]} {
   lappend this(L_menu_roots_and_starting_item_index) [list $this(menu_item) $this(starting_children_index)]
     set this(menu_item)               $item
	 set this(starting_children_index) 0
   this Load_polygone_from_str this(menu_descr_str)
   #this Update
  }
 #this Set_text_title [$item get_title]
}

#___________________________________________________________________________________________________________________________________________
method Automate_Menu_Main Lower_label_of {noeud} {
 if {$noeud != ""} {
   set b_txt   [$noeud Val_MetaData Associated_label]
   $b_txt Composante_couleur_masque_texture 3 1; $b_txt Mode_texture 0
   set origine [$b_txt Val_MetaData Origine]
   eval "$b_txt Origine $origine"; $b_txt Etirement 1 1
  }
}

#___________________________________________________________________________________________________________________________________________
method Automate_Menu_Main Bigger_label_of {noeud {e 2}} {
 set lab [$noeud Val_MetaData Associated_label]
   B_Raise $lab; $lab Calculer_boites; set ex [$lab Ex]; set ey [$lab Ey]; $lab Etirement $e $e
   set boite_lab [$lab Boite_noeud]
   $lab Translation [expr ($ex-[$lab Ex])*[$boite_lab Cx]] [expr ($ey-[$lab Ey])*[$boite_lab Cy]]
}

#___________________________________________________________________________________________________________________________________________
method Automate_Menu_Main Boing_for {noeud transfo e1 e2} {
 set b_txt   [$noeud Val_MetaData Associated_label] 
 set t [$transfo T_courant]
   set v [expr exp($t)-1]
 this Lower_label_of $noeud
 $b_txt Mode_texture 1
 $b_txt Composante_couleur_masque_texture 3 [expr 1-$v]
 this Bigger_label_of $noeud [expr (1-$v)*$e1 + $v*$e2]
}

#___________________________________________________________________________________________________________________________________________
method Automate_Menu_Main Anim_triggered_label_of {noeud e1 e2} {
 set transfo [B_transfo 500]
 set rap_pdt [B_rappel [Interp_TCL]]
   $rap_pdt Texte "$objName Boing_for $noeud $transfo $e1 $e2"
   puts "Boing $noeud"
   $noeud Pre_rendu_exeptionnel 1
 set rap_fin [B_rappel [Interp_TCL]]
   $rap_fin Texte "$objName Lower_label_of $noeud"
 $transfo abonner_a_rappel_pendant [$rap_pdt Rappel]
 $transfo abonner_a_rappel_fin     [$rap_fin Rappel]
 $transfo Demarrer
 N_i_mere Ajouter_deformation_dynamique $transfo
}

#___________________________________________________________________________________________________________________________________________
method Automate_Menu_Main Press_zone {rap} {
 set infos [Void_vers_info [$rap Param]]
 set noeud [Real_class [$infos NOEUD]]
 set ptr   [$infos Ptr]

 if { ([$ptr Appuie] == 0) || [$ptr Val_MetaData ${objName}_noeud] != ""} {return}
 
 if {[$noeud Val_MetaData item] != ""} {$ptr Add_MetaData ${objName}_cmd_to_exec " "}
 
# Visual feedback on the associated label
 this Bigger_label_of $noeud

# Subscribe to the pointeur
 $noeud Ajouter_MetaData_T ptr $ptr
   $ptr abonner_a_changement [$this(rap_change_ptr) Rappel]
   $ptr Add_MetaData ${objName}_noeud    $noeud
   set L_repere [this get_a_L_repere]
     $L_repere maj [$infos L_REPERES]; Ajouter_noeud_en_fin_de_liste_rep $L_repere $noeud
	 $ptr Add_MetaData ${objName}_L_repere $L_repere
	 set info_clic [$noeud Val_MetaData ${objName}_info_clic]
	 if {[string equal $info_clic {}]} {
	   $noeud Ajouter_MetaData_T ${objName}_info_clic [list [N_i_mere ms] 0]
	  } else {if {([N_i_mere ms] - [lindex $info_clic 0]) > 300} {
	            $noeud Ajouter_MetaData_T ${objName}_info_clic [list [N_i_mere ms] 0]
			   }
	         }
#puts "$objName Press_zone $rap\n  - noeud : $noeud\n  - ptr : $ptr"
}

#___________________________________________________________________________________________________________________________________________
method Automate_Menu_Main Release_L_repere {L} {
 lappend this(Pool_L_repere)  $L
 set this(L_repere_actives) [lremove $this(L_repere_actives) $L]
}

#___________________________________________________________________________________________________________________________________________
method Automate_Menu_Main get_a_L_repere {} {
 if {[llength $this(Pool_L_repere)] > 0} {
   set rep [lindex $this(Pool_L_repere) 0]
   set this(Pool_L_repere) [lrange $this(Pool_L_repere) 1 end]
  } else {set rep [Liste_alx_repere2D]
         }
		 
 lappend this(L_repere_actives) $rep
 return $rep
}

#___________________________________________________________________________________________________________________________________________
method Automate_Menu_Main Release_polygone {p} {
 $p Vider_peres; $p Etirement 1 1; $p Origine 0 0; $p Rotation 0; $p Couleur 1 1 1 1; $p Vider_MetaData
 lappend this(Pool_polygones)  $p
 set this(L_polygones_actives) [lremove $this(L_polygones_actives) $p]
}

#___________________________________________________________________________________________________________________________________________
method Automate_Menu_Main get_a_polygone {} {
 if {[llength $this(Pool_polygones)] > 0} {
   set rep [lindex $this(Pool_polygones) 0]
   set this(Pool_polygones) [lrange $this(Pool_polygones) 1 end]
  } else {set rep [B_polygone]
		  $rep abonner_a_detection_pointeur [$this(rap_press_menu) Rappel] [expr [ALX_pointeur_enfonce]+[ALX_pointeur_apparition]]
         }
		 
 $rep Vider_MetaData
 lappend this(L_polygones_actives) $rep
 return $rep
}

#___________________________________________________________________________________________________________________________________________
method Automate_Menu_Main Switch_L_items {b_pol ptr} {
 #puts "$objName Switch_L_items $b_pol $ptr"
 $ptr Add_MetaData ${objName}_cmd_to_exec [$b_pol Val_MetaData ${objName}_cmd_to_exec]
 set boite_pol [$b_pol Boite_noeud]
 set L_reperes [$ptr Val_MetaData ${objName}_L_repere]

 if {[string equal $L_reperes ""]} {return}

 $this(pt_tmp) maj [$ptr X] [$ptr Y]
   Repere_transformation $this(pt_tmp) $L_reperes
   set x [$this(pt_tmp) X]
   set y [$this(pt_tmp) Y]

 set x [expr $x-[$boite_pol Cx]]; set y [expr $y-[$boite_pol Cy]]
# puts "  <$x ; $y>"
 set L_sub_items  [$this(menu_item) get_L_subitem]
 set nb_sub_items [llength $L_sub_items]
 
# Split of the space in $nb_sub_items / $this(nb_pol_for_item_display) parts...each associated to a subset of sub items
 set PI 3.14159265
 set d [expr sqrt($x*$x + $y*$y)]
 set x [expr $x / $d]
 set y [expr $y / $d]
 if {$x == 0} {
   if {$y > 0} {set a [expr $PI / 2.0]} else {set a [expr 3*$PI / 2.0]}
  } else {set a [expr acos($x)]
          if {$y < 0} {set a [expr 2*$PI - $a]}
         }
 set nb_parts [expr 1 + int( double($nb_sub_items) / ($this(nb_pol_for_item_display)+1) )]
 #puts "angle : $a ; nb_part : $nb_parts"
 if {$nb_parts == 1} {return}
 set n_index [expr $this(nb_pol_for_item_display) * (int (($a * $nb_parts) / (2*$PI)))]
 #puts "n_index : $n_index"
 if {$n_index != $this(starting_children_index)} {
   set this(starting_children_index) $n_index
   this Info_Goto_sub_menu_index [expr $n_index / $this(nb_pol_for_item_display)]
   this Update
  }
}

#___________________________________________________________________________________________________________________________________________
method Automate_Menu_Main Load_polygone {f_name} {
# Load new polygons from file $f_name
 set f [open $f_name]
   set str [read $f]
 close $f

 set this(menu_descr_str) "" 
 this Load_polygone_from_str str
 set this(menu_descr_str) $str
}

#___________________________________________________________________________________________________________________________________________
method Automate_Menu_Main Load_polygone_from_str {str_name} {
 upvar $str_name str
 
if {$str != $this(menu_descr_str)} {
# Release previously created polygons
 foreach e $this(L_text_sub_items) {
   set p [lindex $e 1]
     this Release_polygone $p
   set e [lindex $e 0]
     this Release_B_texte $e; $e Vider_MetaData
  }
 set this(L_text_sub_items) {}
 

 
# Create polygons and insert them in the scene graph
 set L_pol_txt [list]; set L_lab_txt [list]
 set L_lignes [split $str "\n"]
 set positionner_labels 0
 foreach ligne $L_lignes {
   if {!$positionner_labels} {
     if {[regexp {^_*$} $ligne]} {
	   set positionner_labels 1
	  } else {lappend L_pol_txt $ligne
	         }
    } else {lappend L_lab_txt $ligne
	       }
  }

 
 set cc [B_cc]
 set pos 0
 $this(n_root) Position_des_fils_changeable 0
 foreach txt $L_pol_txt {
   if {[string equal $txt ""]} {continue}
   $cc = $txt
   set b_txt [this get_a_B_texte ""]; $b_txt Noeud_touchable 0; $b_txt Ajouter_MetaData_T position [lindex $L_lab_txt $pos]
     $b_txt Ajouter_MetaData_T delta_x 0; $b_txt Ajouter_MetaData_T delta_y 0; 
   set b_pol [this get_a_polygone]  ; $b_pol Deserialiser_type $cc; $b_pol Couleur 1 1 1 1; $b_pol Ajouter_MetaData_T Associated_label $b_txt
   lappend this(L_text_sub_items) [list $b_txt $b_pol]
   $this(n_root) Ajouter_fils_au_debut $b_pol
   incr pos
  }
 Detruire $cc
 set this(nb_pol_for_item_display) [expr [llength $this(L_text_sub_items)] - 1]
}


  
# Special set up for the first polygon that correspond to the thumb
 set t_i_p [lindex $this(L_text_sub_items) 0]
   set b_pol [lindex $t_i_p 1]; $b_pol Couleur 0 1 0 1
   $b_pol Ajouter_MetaData_T ${objName}_cmd_to_exec "$objName Switch_L_items $b_pol \$ptr"
   $b_pol Calculer_boites; set boite [$b_pol Boite_noeud]
   set cx [$boite Cx]; set cy [$boite Cy]
   set this(thumb_zone) $b_pol
   
  # Divide the polygone into slices, associate a color for each one
   foreach p $this(L_slices_thumb) {this Release_polygone $p; $p Vider_peres; $p Noeud_touchable 1}
   set L_sub_items  [$this(menu_item) get_L_subitem]
   set nb_sub_items [llength $L_sub_items]
   set PI 3.14159265; set RAYON 999999
   set L_colors $this(L_colors)
   set this(L_slices_thumb) [list]
   set NB [expr ($nb_sub_items / $this(nb_pol_for_item_display))]
   for {set i 0} {$i < $NB} {incr i} {
     set p [this get_a_polygone]; lappend this(L_slices_thumb) $p
	 $p Vider; $b_pol Ajouter_fils $p
	   set a0 [expr $i    *2*$PI / $NB]
	   set a1 [expr ($i+1)*2*$PI / $NB]
	   set a2 [expr (2*$a0+$a1)/3.0]
	   set a3 [expr ($a2+$a1)/2.0]
	   set contour [ProcTabDouble [list 0 0 \
	                                    [expr $RAYON*cos($a0)] [expr $RAYON*sin($a0)] \
										[expr $RAYON*cos($a2)] [expr $RAYON*sin($a2)] \
										[expr $RAYON*cos($a3)] [expr $RAYON*sin($a3)] \
										[expr $RAYON*cos($a1)] [expr $RAYON*sin($a1)] \
								  ]]
	   $p Ajouter_contour $contour; $p Origine $cx $cy; $p Noeud_touchable 0
	   Detruire $contour
	   $p Intersection $b_pol $b_pol
	   set num_slice [expr ($i-1)%8]
	   puts "num_slice : $num_slice with i = $i"
	   eval "$p Couleur [lindex $L_colors $num_slice]"
    }
  
  # Save the list of slices
   $b_pol Ajouter_MetaData_T L_slices $this(L_slices_thumb)
   
  # Center + current slice  big border
   set p_center [this get_a_polygone]; $p_center Vider; $p_center Noeud_touchable 0; $b_pol Ajouter_fils_au_debut $p_center
     lappend this(L_slices_thumb) $p_center; $p_center Couleur 0 0 0 1; $p_center Origine $cx $cy
	 set center_ray_x [expr 0.15*[$boite Tx]]; set center_ray_y [expr 0.15*[$boite Ty]]
     set contour [ProcOvale 0 0 $center_ray_x $center_ray_y 60]; $p_center Ajouter_contour $contour; Detruire $contour
   set p_bord_selector [this get_a_polygone]
	 lappend this(L_slices_thumb) $p_bord_selector; $p_bord_selector Couleur 0 0 0 1
	 set width [expr 0.25*$center_ray_x]
	 #set contour [ProcTabDouble ]
	 

# Then go on
 this Update
}

#___________________________________________________________________________________________________________________________________________
method Automate_Menu_Main Update {} {
 if {[string equal $this(L_text_sub_items) ""]} {return}
 set PI 3.14159265
 $this(n_root) Etirement 0.72 0.72

# Zoom the currently selected slice
 set L_poly_slice [$this(thumb_zone) Val_MetaData L_slices]
  
 foreach p $L_poly_slice {$p Etirement 1 1}
 [lindex $L_poly_slice [expr int($this(starting_children_index)/$this(nb_pol_for_item_display))]] Etirement 1.3 1.3
 
# Draw a hand... the thumb is used to navigate into submenus, each other finger can access to two items
 set L_sub_items  [$this(menu_item) get_L_subitem]
 set i_tip 1; set i_item $this(starting_children_index)
 set couleur [lindex $this(L_colors) [expr (int($i_item / $this(nb_pol_for_item_display)) - 1) % $this(nb_pol_for_item_display)]]
 for {set i_tip 1} {$i_tip <= $this(nb_pol_for_item_display)} {incr i_tip} {
   set item  [lindex $L_sub_items $i_item]
   set t_i_p [lindex $this(L_text_sub_items) $i_tip]
     set b_txt [lindex $t_i_p 0]; 
	 set b_pol [lindex $t_i_p 1]; eval "$b_pol Couleur $couleur"
     if {[string equal $item ""]} {$b_txt Afficher_noeud 0
	                               $b_pol Retirer_MetaData_T item
	                               continue
	                              }
   
    #	Placement
     $b_txt Maj_texte          [$item get_title]      ; $b_txt Afficher_noeud 1; $b_txt Optimiser_dimensions; 
	   eval [$item get_init_cmd]
	   $b_txt Calculer_boites
	   set position_txt [$b_txt Val_MetaData position]
	   set boite_label [$b_txt Boite_noeud]
	   $b_txt Origine [lindex $position_txt 0] [lindex $position_txt 1]
	   $b_txt Translation [$b_txt Val_MetaData delta_x] [$b_txt Val_MetaData delta_y]
	   switch [lindex $position_txt 2] {
	     left   {$b_txt Translation_interne -[$boite_label Tx] 0}
		 right  {}
		 center {$b_txt Translation_interne [expr -[$boite_label Tx]/2] 0}
	    }
	 $b_pol Ajouter_MetaData_T item              $item
	 $b_txt Ajouter_MetaData_T Origine           "[$b_txt Px] [$b_txt Py]"
  # Next...
   incr i_item
  }

# Set up title
 set txt ""; set dec ""
 foreach r_i $this(L_menu_roots_and_starting_item_index) {
   append txt "$dec[[lindex $r_i 0] get_title]\n"
   append dec "  "
  }
 append txt "$dec[$this(menu_item) get_title]\n"
 this Set_text_title $txt
   set title_box [$this(text_title) Boite_noeud]
#   $this(text_title) Origine -[$title_box Tx] [$boite_pol Cy]

 return 
}

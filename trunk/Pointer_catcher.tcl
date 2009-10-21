method Pointer_catcher constructor {node poly_root_menu menu} {
 set this(menu)           $menu
 set this(node)           $node
 set this(poly_root_menu) $poly_root_menu
 set this(area_min)       2.5
 set this(cx_paume)       120; set this(cy_paume)       90
 set this(contour)        [ProcOvale $this(cx_paume) $this(cy_paume) 100 75 60]
 set this(time_to_disappear) 1000
 set this(last_apparition)   0
 $this(poly_root_menu) Ajouter_contour $this(contour)
 
 set this(dist_min_for_orientation) 42
 set this(mode_orientation_fixe)    0

 set this(rap_pre_traitement) [B_rappel [Interp_TCL]]
   $this(rap_pre_traitement) Texte "$objName Pre_traitement_on $node"
   $node Rap_pre_traitement_contient [$this(rap_pre_traitement) Rappel]
 set this(rap_post_traitement) [B_rappel [Interp_TCL]]
   $this(rap_post_traitement) Texte "$objName Post_traitement_on $node"
   $node Rap_post_traitement_contient [$this(rap_post_traitement) Rappel]
 set this(evt_on_node) [B_rappel [Interp_TCL]]
   $node abonner_a_detection_pointeur [$this(evt_on_node) Rappel] [expr [Rappel_Seulement_si_noeud_contient]+[ALX_pointeur_mouvement]+[ALX_pointeur_relache]+[ALX_pointeur_enfonce]]
   $this(evt_on_node) Texte "$objName Contact_rap $this(evt_on_node)"
# Visible only if one pointer on it at least   
 set this(rap_count_ptr) [B_rappel [Interp_TCL]]
   $this(rap_count_ptr) Texte "if {\[catch {$objName Count_ptr $this(rap_count_ptr)} err\]} {puts \"ERROR in Count_ptr:\n\$err\"}"
   $this(poly_root_menu) abonner_a_detection_pointeur [$this(rap_count_ptr) Rappel] [expr [ALX_pointeur_relache]+[ALX_pointeur_enfonce]+[ALX_pointeur_disparition]+[ALX_pointeur_apparition]]
   set this(L_ptr_on_it) ""
 set this(rap_undisplay_menu) [B_rappel [Interp_TCL]]; 
   $this(rap_undisplay_menu) Texte "$objName Do_UnDisplay_or_not"
   $this(poly_root_menu) abonner_a_LR_parcours [$this(poly_root_menu) LR_Av_aff] [$this(rap_undisplay_menu) Rappel] 
}

#___________________________________________________________________________________________________________________________________________
method Pointer_catcher Do_UnDisplay_or_not {} {
 if {$this(L_ptr_on_it) == ""} {
   if {([N_i_mere ms] - $this(last_apparition)) > $this(time_to_disappear)} {
     $this(poly_root_menu) Afficher_noeud 0
	 $this(poly_root_menu) Gerer_contacts 0
     puts "Close menu, delay [expr [N_i_mere ms] - $this(last_apparition)]"
    }
  } else {set this(last_apparition) [N_i_mere ms]
         }
}

#___________________________________________________________________________________________________________________________________________
method Pointer_catcher Count_ptr {rap} {
 set infos [Void_vers_info [$rap Param]]
 set ptr   [$infos Ptr]
 set evt   [$infos EVT]
 
 this Count_ptr_from_evt $evt $ptr
}

#___________________________________________________________________________________________________________________________________________
method Pointer_catcher Count_ptr_from_evt {evt ptr} {
 set type [$evt Type_evt]
 if {($type == [ALX_pointeur_apparition]) || ($type == [ALX_pointeur_enfonce])} {
   set this(last_apparition) [N_i_mere ms]
   if {[lsearch $this(L_ptr_on_it) $ptr] == -1} {lappend this(L_ptr_on_it) $ptr; 
												}
  }
 if {($type == [ALX_pointeur_relache]) || ($type == [ALX_pointeur_disparition])} {
   set this(L_ptr_on_it) [lremove $this(L_ptr_on_it) $ptr]
   set this(last_apparition) [N_i_mere ms]
  }
}

#___________________________________________________________________________________________________________________________________________
method Pointer_catcher Pre_traitement_on {node} {
 set evt [B_sim_couche Evt_courant]
 set ptr [$evt Ptr]
 
 if {[info exists this(mtd,$ptr)]} {
   $node Pre_traitement_contient 0
   return
  }
 set H [$ptr Val_MetaData H]
 set W [$ptr Val_MetaData W]
 #puts "$H * $W = [expr $H*$W]  /à area_min = $this(area_min)"
 if {$H != "" && $W != ""} {
   if {($H * $W) >= $this(area_min)} {
	 $node Pre_traitement_contient 0
    } else {$node Pre_traitement_contient 1
	        
		   }
  } else {$node Pre_traitement_contient 1}
}

#___________________________________________________________________________________________________________________________________________
method Pointer_catcher Post_traitement_on {node} {
 #puts "$objName Post_traitement_on $node"
# Identify the event and the related pointer
 set evt [B_sim_couche Evt_courant]
 set ptr [$evt Ptr]

 set H [$ptr Val_MetaData H]
 set W [$ptr Val_MetaData W]
 #if {$H == "" || $W == ""}        {return}
 #if {($H * $W) < $this(area_min)} {
 #                                 # return
 #                                 }
 this Count_ptr_from_evt $evt $ptr
# Check if the contact node is not the menu polygon
 set infos [$node Contenant_courant]
 if {$infos != "NULL"} {set contact_node [Real_class [$infos NOEUD]]} else {set contact_node NULL}
 if {$contact_node == $this(poly_root_menu)} {
   set this(last_apparition) [N_i_mere ms]
   $node Contenant_courant [Rep_puit]
  } else {
         # Is it a previously known pointer?
          if {[$node Pre_traitement_contient] == 0} {
		    if {[info exists this(mtd,$ptr)]} {
              $node Contenant_courant [$node Creer_contenant]
	         } else {if {[$node Pre_traitement_contient] == 0} {
		               set this(mtd,$ptr) ""
			           $node Contenant_courant [$node Creer_contenant]
		              }
                    }
           } 
		 }
}

#___________________________________________________________________________________________________________________________________________
method Pointer_catcher Contact_rap {rap} {
 set this(last_apparition) [N_i_mere ms]
 
 set infos [Void_vers_info [$rap Param]]
 set evt   [$infos EVT]
 set ptr   [$infos Ptr]
 set pt    [$infos Point_au_contact]

 if {[lsearch $this(L_ptr_on_it) $ptr] == -1 && [$evt Type_evt] != [ALX_pointeur_relache]}   {lappend this(L_ptr_on_it) $ptr; puts "L_pointeurs : {$this(L_ptr_on_it)}"}
 if {[$evt Type_evt] == [ALX_pointeur_relache]} {puts RELACHE; set     this(L_ptr_on_it) [lremove $this(L_ptr_on_it) $ptr]}
 
 $this(poly_root_menu) Afficher_noeud 1; $this(poly_root_menu) Gerer_contacts 1
 if {[$this(poly_root_menu) Val_MetaData Linked_to_ptr] == ""} {
   $this(menu) Start_mode_expert
   $this(poly_root_menu) Ajouter_MetaData_T Linked_to_ptr $ptr
  }
 
 lappend this(mtd,$ptr) [$pt X] [$pt Y]
 if {[llength $this(mtd,$ptr)] >= 4} {
   set x1 [lindex $this(mtd,$ptr) 0]    ; set y1 [lindex $this(mtd,$ptr) 1]
   set x2 [lindex $this(mtd,$ptr) end-1]; set y2 [lindex $this(mtd,$ptr) end]
   #set dx [expr $x2 - $x1]; set dy [expr $y2 - $y1]
   set dx [expr $x1 - $x2]; set dy [expr $y1 - $y2]
   
   set r [expr sqrt($dx*$dx + $dy*$dy)]
   if {$r > $this(dist_min_for_orientation)} {
     if {$dx != 0} {set a [expr acos($dx/$r)]} else {set a 1.57079632}
     if {$dy <  0} {set a [expr 6.28318530 - $a - 1.57079632]} else {set a [expr $a - 1.57079632]}
	 $this(poly_root_menu) Origine [expr $x2 - [$this(poly_root_menu) Ex]*$this(cx_paume)] \
                                   [expr $y2 - [$this(poly_root_menu) Ey]*$this(cy_paume)]
     $this(poly_root_menu) Rotation 0; 
     $this(poly_root_menu) Pivoter_interne $a [expr [$this(poly_root_menu) Ex]*$this(cx_paume)] \
                                              [expr [$this(poly_root_menu) Ey]*$this(cy_paume)]
	} else {set a [$this(poly_root_menu) Rotation]}
   
  } else {set a [$this(poly_root_menu) Rotation]
          set x2 [lindex $this(mtd,$ptr) end-1]; set y2 [lindex $this(mtd,$ptr) end]
		  $this(poly_root_menu) Origine [expr $x2 - [$this(poly_root_menu) Ex]*$this(cx_paume)] \
                                        [expr $y2 - [$this(poly_root_menu) Ey]*$this(cy_paume)]
          $this(poly_root_menu) Rotation 0;
          $this(poly_root_menu) Pivoter_interne $a [expr [$this(poly_root_menu) Ex]*$this(cx_paume)] \
                                                   [expr [$this(poly_root_menu) Ey]*$this(cy_paume)]
         }
 
 
 $this(poly_root_menu) Vider
 
 set H [$ptr Val_MetaData H]
 set W [$ptr Val_MetaData W]
 if {$this(mode_orientation_fixe) || [string equal [$evt Type_evt] [ALX_pointeur_relache]] || ($H * $W) < $this(area_min)} {
   unset this(mtd,$ptr)
  # Round polygon
   $this(poly_root_menu) Ajouter_contour $this(contour)
   $this(poly_root_menu) Retirer_MetaData_T Linked_to_ptr
   #set this(L_ptr_on_it) [lremove $this(L_ptr_on_it) $ptr]
  }
}

#___________________________________________________________________________________________________________________________________________
Generate_accessors Pointer_catcher [list mode_orientation_fixe dist_min_for_orientation L_ptr_on_it time_to_disappear area_min poly_root_menu]

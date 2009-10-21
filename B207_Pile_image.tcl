#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
method Pile_image constructor args {
 set this(root_node)       [B_noeud]
   $this(root_node) Position_des_fils_changeable 0
   set this(rap_Reposition_pile) [B_rappel [Interp_TCL]]
   $this(root_node) abonner_a_LR_parcours [$this(root_node) LR_Av_aff_fils] [$this(rap_Reposition_pile) Rappel]
 set this(Pool_image)      ""
 set this(L_images)        [B_image]
 set this(L_ptr)           ""
 set this(starting_point)  "0 0"
 set this(ptr_browse)      ""
 set this(ptr_browse_path) ""
 set this(L_reperes)       [Liste_alx_repere2D]
 set this(pt_tmp)          [B_point]
 
 set this(rap_mvt) [B_rappel [Interp_TCL]]; $this(rap_mvt) Texte "$objName Ptr_Move $this(rap_mvt)"
 set this(rap_fin) [B_rappel [Interp_TCL]]; $this(rap_fin) Texte "$objName Ptr_Fin  $this(rap_fin)"
 set this(rap_press) [B_rappel [Interp_TCL]]
 
 this init
 this Resize 1
 eval "$objName configure $args"
}

#___________________________________________________________________________________________________________________________________________
method Pile_image get_root       {} {return $this(root_node) }
#___________________________________________________________________________________________________________________________________________
method Pile_image get_L_images   {} {return $this(L_images)  }
#___________________________________________________________________________________________________________________________________________
method Pile_image get_L_ptr      {} {return $this(L_ptr)     }
#___________________________________________________________________________________________________________________________________________
method Pile_image get_ptr_browse {} {return $this(ptr_browse)}

#___________________________________________________________________________________________________________________________________________
method Pile_image init {} {
 $this(root_node) abonner_a_detection_pointeur [$this(rap_press) Rappel] [ALX_pointeur_enfonce]
   $this(rap_press) Texte "$objName Evt_ptr $this(rap_press)"
}

#___________________________________________________________________________________________________________________________________________
method Pile_image Ptr_Fin {rap} {
# puts "$objName Ptr_Fin $rap, param : [$rap Param]"
 set ptr [Void_vers_alx_pointeur [$rap Param]]
 if {[$ptr Appuie] != 0} {this Ptr_Move $rap; return}
# puts "  GO"
 set this(L_ptr) [lremove $this(L_ptr) $ptr]
 $ptr desabonner_de_changement [$this(rap_fin) Rappel]
 if {[string equal $ptr $this(ptr_browse)]} {
   this Stop_mode_accordeon $ptr
  }
}

#___________________________________________________________________________________________________________________________________________
method Pile_image Reposition_pile {ptr} {
 if {![string equal $ptr $this(ptr_browse)]} {
   if {![string equal "" $this(ptr_browse)]} {set ptr $this(ptr_browse)} else {return}
  }

 lappend this(ptr_browse_path) [$ptr X] [$ptr Y]
# Place the images along the path....
 set i 1; set nb [expr [llength $this(L_images)] - 1]
 set dep_x [lindex $this(starting_point) 0]; set dep_y [lindex $this(starting_point) 1]
 $this(pt_tmp) maj [$ptr X] [$ptr Y]
   Repere_transformation $this(pt_tmp) $this(L_reperes)
   set x [$this(pt_tmp) X]
   set y [$this(pt_tmp) Y]
 foreach img [lrange $this(L_images) 1 end] {
   set dt [expr double($i) / double($nb)]
   $img Origine [expr $dt*($x - $dep_x)] \
                [expr $dt*($y - $dep_y)]
   incr i
  }
}

#___________________________________________________________________________________________________________________________________________
method Pile_image Ptr_Move {rap} {
# puts "$objName Ptr_Move $rap"
 set ptr [Void_vers_alx_pointeur [$rap Param]]
# puts "  ptr : $ptr"
}

#___________________________________________________________________________________________________________________________________________
method Pile_image Start_mode_accordeon {infos} {
 puts "$objName Start_mode_accordeon $infos"
 set ptr [$infos Ptr]
 set this(ptr_browse) $ptr
 set this(ptr_browse_path) ""
 foreach img [this get_L_images] {$img Afficher_noeud 1; $img Gerer_contacts 1}
 
 $this(rap_Reposition_pile) Texte "$objName Reposition_pile $ptr"
 $ptr abonner_a_changement [$this(rap_mvt) Rappel]
   set this(starting_point) "[$infos X_au_contact] [$infos Y_au_contact]"
   $this(L_reperes) maj [$infos L_REPERES]
 puts "  END Start_mode_accordeon"
}

#___________________________________________________________________________________________________________________________________________
method Pile_image Stop_mode_accordeon {ptr} {
 puts "$objName Stop_mode_accordeon $ptr" 
 set this(ptr_browse) ""
 $this(rap_Reposition_pile) Texte ""
 foreach img [this get_L_images] {$img Afficher_noeud 0; $img Gerer_contacts 0}
 B_configure [lindex [this get_L_images] 0] -Afficher_noeud 1 -Gerer_contacts 1
 $ptr desabonner_de_changement [$this(rap_mvt) Rappel]
 puts "  FIN STOP"
}

#___________________________________________________________________________________________________________________________________________
method Pile_image Evt_ptr {rap} {
 puts "$objName Evt_ptr $rap"
 set infos [Void_vers_info [$rap Param]]
 set ptr   [$infos Ptr]
 set evt   [$infos EVT]
 set node  [$infos NOEUD]
 
 if {[$evt Type_evt] == [ALX_pointeur_enfonce]} {
   if {[lsearch $this(L_ptr) $ptr] == -1} {
     lappend this(L_ptr) $ptr; $ptr abonner_a_changement [$this(rap_fin) Rappel]
	 puts "Start ?"
	 if {([llength $this(L_ptr)] == 3)  &&  [string equal $this(ptr_browse) ""]} {
	   puts "  YES"
	   this Start_mode_accordeon $infos
      }
	}
  }
}

#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
method Pile_image configure args {
 foreach {m a} $args {
   regexp {^-(.*)$} $m reco m
   eval "$objName $m $a"
  }
}

#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
method Pile_image Release_img {img} {
 if {[lsearch $this(Pool_image) $img] == -1} {
   lappend this(Pool_image) $img
   $img Vider_peres
   set this(L_images) [lremove $this(L_images) $img]
  }
}

#___________________________________________________________________________________________________________________________________________
method Pile_image get_an_image {} {
 if {[llength $this(Pool_image)]} {
   set rep [lindex $this(Pool_image) 0]
   set this(Pool_image) [lrange $this(Pool_image) 1 end]
  } else {set rep [B_image]
         }

 $this(root_node) Ajouter_fils $rep
 lappend this(L_images) $rep
 $rep Afficher_noeud 0; $rep Gerer_contacts 0
 return $rep
}

#___________________________________________________________________________________________________________________________________________
method Pile_image Resize {nb} {
 foreach img $this(L_images) {this Release_img $img}
 for {set i 0} {$i < $nb} {incr i} {this get_an_image}
 if {$nb} {
   [lindex $this(L_images) 0] Afficher_noeud 1
   [lindex $this(L_images) 0] Gerer_contacts 1
  }
}

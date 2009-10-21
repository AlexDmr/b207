#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
method B207_graph_plot constructor {} {
 set this(Pool_poly_points) {}
   set this(L_active_poly_points) {}
   set this(L_coord_form_poly_point) {-10 -10 10 -10 10 10 -10 10}
   set this(couleur_poly_point)      {0 1 0 1}
 set this(root)     [B_noeud]
 set this(title)    [B_texte 100 100 20 [fonte_Arial] [B_sim_sds]]
   $this(title) TEXTE "GRAPH"; $this(title) Optimiser_dimensions; $this(title) Origine 0 -20
 set this(n_rep)           [B_noeud]
   $this(root) Ajouter_fils $this(title)
   $this(root) Ajouter_fils $this(n_rep)
   set this(is_setting_current_point) 0
   # Axis representation
     set this(x_axis) [B_polygone]; 
	   B_configure $this(x_axis) -Ajouter_contour [ProcRect 0 0 1 1] -Etirement 2000 1 -Couleur 0 0 1 1 -Couleur_perimetre 0 1 1 1
	 set this(y_axis) [B_polygone]
	   B_configure $this(y_axis) -Ajouter_contour [ProcRect 0 0 1 1] -Etirement 1 2000 -Couleur 0 0 1 1 -Couleur_perimetre 0 1 1 1
	 B_configure $this(n_rep) -Ajouter_fils $this(x_axis) -Ajouter_fils $this(y_axis)
   # Stuff for current point
     set this(pt_x_axis) [B_polygone]; $this(pt_x_axis) Ajouter_contour [ProcRect 0 0 1 1]
	 set this(pt_y_axis) [B_polygone]; $this(pt_y_axis) Ajouter_contour [ProcRect 0 0 1 1]
	 set this(ptr)       [B_polygone]; $this(ptr) Ajouter_contour [ProcRect -15 -15 15 15]; $this(ptr) Couleur 1 1 1 0.5
	 B_configure $this(n_rep) -Ajouter_fils $this(pt_x_axis) -Ajouter_fils $this(pt_y_axis) -Ajouter_fils $this(ptr) 
	 set this(ctc_ptr) ${objName}_ctc_ptr
	   B_contact $this(ctc_ptr) "$this(ptr) 1"
	 set this(rap_ptr_mvt) [B_rappel [Interp_TCL]]
	   $this(rap_ptr_mvt) Texte "if {\[$this(ptr) A_changer\]} {$objName set_current_point \[$this(ptr) Px\] \[$this(ptr) Py\]}"
	   $this(ptr) abonner_a_LR_parcours [$this(ptr) LR_Av_PR_fils] [$this(rap_ptr_mvt) Rappel]
   # Stuff for managing poly zones
   # Button for creating a new polygone   
     set this(bt_new_polygon)   [B_bouton_texte "New polygon" 30 $this(ptr) 1]  ;
	   $this(bt_new_polygon) Calculer_boites; set bt_box [$this(bt_new_polygon) Boite_noeud_et_fils_glob]
	   $this(bt_new_polygon) Origine [expr -10 -[$bt_box Tx]] 20
	   set this(rap_bt_new_polygon) [B_rappel [Interp_TCL] "$objName Start_a_new_zone polygon"]
	   $this(bt_new_polygon) abonner_a_activation [$this(rap_bt_new_polygon) Rappel]
   # Button for creating a new rectangle
	 set this(bt_new_rectangle) [B_bouton_texte "New rectangle" 30 $this(ptr) 1];
	   $this(bt_new_rectangle) Calculer_boites; set bt_box [$this(bt_new_rectangle) Boite_noeud_et_fils_glob]
	   $this(bt_new_rectangle) Origine 10 20
	   set this(rap_bt_new_rectangle) [B_rappel [Interp_TCL] "$objName Start_a_new_zone rectangle"]
	   $this(bt_new_rectangle) abonner_a_activation [$this(rap_bt_new_rectangle) Rappel]
   # Button for canceling
	 set this(bt_cancel)        [B_bouton_texte "CANCEL" 30 $this(ptr) 1]       ;
	   $this(bt_cancel) Calculer_boites; set bt_box [$this(bt_cancel) Boite_noeud_et_fils_glob]
	   $this(bt_cancel) Origine [expr -10 -[$bt_box Tx]] -20
	   set this(rap_bt_cancel) [B_rappel [Interp_TCL] "$objName Cancel_poly_zone"]
	   $this(bt_cancel) abonner_a_activation [$this(rap_bt_cancel) Rappel]   
   # Button for adding a point	 
	 set this(bt_add_point)     [B_bouton_texte "Add point" 30 $this(ptr) 1]    ;
	   $this(bt_add_point) Calculer_boites; set bt_box [$this(bt_add_point) Boite_noeud_et_fils_glob]
	   $this(bt_add_point) Origine [expr 30] [expr -[$bt_box Cy]]
	   set this(rap_bt_add_point) [B_rappel [Interp_TCL] "$objName Add_current_point_to_current_poly_zone"]
	   $this(bt_add_point) abonner_a_activation [$this(rap_bt_add_point) Rappel]   
   # Button for adding the shape
	 set this(bt_OK_add_poly)   [B_bouton_texte "OK" 30 $this(ptr) 1]           ;
	   $this(bt_OK_add_poly) Calculer_boites; set bt_box [$this(bt_OK_add_poly) Boite_noeud_et_fils_glob]
	   $this(bt_OK_add_poly) Origine [expr 10] [expr -20-[$bt_box Ty]]
	   set this(rap_bt_OK_add_poly) [B_rappel [Interp_TCL] "$objName Add_current_poly_zone"]
	   $this(bt_OK_add_poly) abonner_a_activation [$this(rap_bt_OK_add_poly) Rappel]
   # Buttons display management
     set this(rap_bt_display_on)  [B_rappel [Interp_TCL] "$objName Display_buttons"]
	 set this(rap_bt_display_off) [B_rappel [Interp_TCL] "$objName Hide_buttons"]
	 $this(ptr) abonner_a_detection_pointeur [$this(rap_bt_display_on)  Rappel] [ALX_pointeur_apparition]
	 $this(ptr) abonner_a_detection_pointeur [$this(rap_bt_display_off) Rappel] [expr [Pas_rappel_si_fils_contient]+[ALX_pointeur_disparition]]
   # Save buttons list
     set this(L_buttons) [list $this(bt_OK_add_poly) $this(bt_add_point) $this(bt_cancel) $this(bt_new_rectangle) $this(bt_new_polygon)]
# The node under which every zones are plugged...
 set this(n_rep_poly_zone) [B_noeud]
   $this(root) Ajouter_fils $this(n_rep_poly_zone)
   $this(n_rep_poly_zone) Position_dans_liste_des_fils_changeable 0
# Data structures to deal with poly zones
 set this(zone_type)   {}  
 set this(L_points)    {}
 set this(L_poly_zone) {}
 set this(current_point) "0 0"
 set this(current_poly_zone_L_pt) {}
 set this(current_poly_zone) {}
 set this(L_B207_nodes) [list $this(root) $this(title) $this(n_rep) $this(ptr) $this(x_axis) $this(y_axis) $this(pt_x_axis) $this(pt_y_axis)]
 
 this Hide_buttons
}

#___________________________________________________________________________________________________________________________________________
method B207_graph_plot dispose {} {
 foreach n $this(L_B207_nodes) {
   Detruire $n
  }
 this inherited
}

#___________________________________________________________________________________________________________________________________________
method B207_graph_plot Free_all_poly_points {} {
 set L $this(L_active_poly_points)
 foreach p $L {this Release_poly_point $p}
}

#___________________________________________________________________________________________________________________________________________
method B207_graph_plot get_a_poly_point {x y} {
 if {[llength $this(Pool_poly_points)]} {
   set rep [lindex $this(Pool_poly_points) 0]
   set this(Pool_poly_points) [lreplace $this(Pool_poly_points) 0 0]
  } else {set rep [B_polygone]
         }
 $rep Ajouter_contour [ProcTabDouble $this(L_coord_form_poly_point)]
 $rep Origine $x $y
 $rep Ajouter_MetaData_T Is_an_example 1
 $this(n_rep) Ajouter_fils_au_debut $rep
 eval "$rep Couleur $this(couleur_poly_point)"
 
 lappend this(L_active_poly_points) $rep
 return $rep
}

#___________________________________________________________________________________________________________________________________________
method B207_graph_plot Release_poly_point {p} {
 Add_element this(Pool_poly_points)     $p
 Sub_element this(L_active_poly_points) $p
 $p Vider_peres
}

#___________________________________________________________________________________________________________________________________________
method B207_graph_plot Display_buttons {} {
 puts "$objName Display_buttons"
 foreach bt $this(L_buttons) {
   $bt Afficher_noeud 1
   $bt Gerer_contacts 1
  }
  
 $this(bt_new_polygon) Calculer_boites; set bt_box [$this(bt_new_polygon) Boite_noeud_et_fils]
 $this(bt_new_polygon) Origine [expr -15-[$bt_box Tx]-[$bt_box BG_X]] [expr 15-[$bt_box BG_X]]

 $this(bt_new_rectangle) Calculer_boites; set bt_box [$this(bt_new_rectangle) Boite_noeud_et_fils]
 $this(bt_new_rectangle) Origine 10 [expr 15-[$bt_box BG_X]]

 $this(bt_cancel) Calculer_boites; set bt_box [$this(bt_cancel) Boite_noeud_et_fils]
 $this(bt_cancel) Origine [expr -15 -[$bt_box Tx]-[$bt_box BG_X]] [expr -[$bt_box Cy]]

 $this(bt_add_point) Calculer_boites; set bt_box [$this(bt_add_point) Boite_noeud_et_fils]
 $this(bt_add_point) Origine [expr 15-[$bt_box BG_X]] [expr -[$bt_box Cy]]

 $this(bt_OK_add_poly) Calculer_boites; set bt_box [$this(bt_OK_add_poly) Boite_noeud_et_fils]
 $this(bt_OK_add_poly) Origine [expr -[$bt_box Cx]] [expr -15-[$bt_box Ty]-[$bt_box BG_Y]]
}

#___________________________________________________________________________________________________________________________________________
method B207_graph_plot Hide_buttons {} {
 puts "$objName Hide_buttons"
 foreach bt $this(L_buttons) {
   $bt Afficher_noeud 0
   $bt Gerer_contacts 0
  } 
}

#___________________________________________________________________________________________________________________________________________
method B207_graph_plot set_dims {px py} {
 $this(x_axis) Etirement $px 1
 $this(y_axis) Etirement 1 $py
}

#___________________________________________________________________________________________________________________________________________
method B207_graph_plot get_rap_bt_display_on  {} {return $this(rap_bt_display_on) }
method B207_graph_plot get_rap_bt_display_off {} {return $this(rap_bt_display_off)}

#___________________________________________________________________________________________________________________________________________
method B207_graph_plot get_n_rep {} {return $this(n_rep)}

#___________________________________________________________________________________________________________________________________________
method B207_graph_plot get_n_ptr {} {return $this(ptr)}

#___________________________________________________________________________________________________________________________________________
method B207_graph_plot get_n_title {} {return $this(title)}

#___________________________________________________________________________________________________________________________________________
method B207_graph_plot get_rap_ptr {} {return $this(rap_ptr_mvt)}

#___________________________________________________________________________________________________________________________________________
method B207_graph_plot get_root {} {return $this(root)}

#___________________________________________________________________________________________________________________________________________
method B207_graph_plot set_title {t} {$this(title) TEXTE $t}

#___________________________________________________________________________________________________________________________________________
method B207_graph_plot Start_a_new_zone {{type rectangle}} {
 if {![string equal $this(current_poly_zone) ""]} {
   $this(current_poly_zone) Vider
  } else {set this(current_poly_zone) [B_polygone]}
 set this(current_poly_zone_L_pt) [list [$this(ptr) Px] [$this(ptr) Py]]
 $this(n_rep_poly_zone) Ajouter_fils_au_debut $this(current_poly_zone)
#this Add_current_point_to_current_poly_zone
 set this(zone_type) $type
}

#___________________________________________________________________________________________________________________________________________
method B207_graph_plot Cancel_poly_zone {} {
 if {![string equal $this(current_poly_zone) ""]} {
   Detruire $this(current_poly_zone)
  }
 set this(current_poly_zone) ""
}

#___________________________________________________________________________________________________________________________________________
method B207_graph_plot Add_current_poly_zone {} {
 lappend this(L_poly_zone) $this(current_poly_zone)
 set this(current_poly_zone) {}
 B_Raise_L $this(L_B207_nodes)
}

#___________________________________________________________________________________________________________________________________________
method B207_graph_plot set_L_points {L} {

}

#___________________________________________________________________________________________________________________________________________
method B207_graph_plot get_current_point {} {return $this(current_point)}

#___________________________________________________________________________________________________________________________________________
method B207_graph_plot pipo {} {
 return $this(bt_new_polygon)
}

#___________________________________________________________________________________________________________________________________________
method B207_graph_plot set_current_point {x y} {
#puts "$objName set_current_point $x $y"
 $this(ptr) Origine $x $y; $this(ptr) A_changer 0
 if {$this(is_setting_current_point)} {return}
 set this(is_setting_current_point) 1
   set this(current_point) "$x $y"
  #puts "$this(pt_y_axis) $x $y"
   $this(pt_x_axis) Origine $x 0; $this(pt_x_axis) Etirement 1 10000
   $this(pt_y_axis) Origine 0 $y; $this(pt_y_axis) Etirement 10000 1
   if {![$this(ptr) A_changer]} {$this(ptr) Origine $x $y}
   this Draw_current_poly_zone_end_to $x $y
   $this(title) TEXTE "$x \; $y"; $this(title) Optimiser_dimensions
 set this(is_setting_current_point) 0
}

#___________________________________________________________________________________________________________________________________________
method B207_graph_plot Add_current_point_to_current_poly_zone {} {
 if {![string equal $this(zone_type) polygon]} {return}
 lappend this(current_poly_zone_L_pt) [$this(ptr) Px] [$this(ptr) Py]
 this Draw_current_poly_zone_end_to [$this(ptr) Px] [$this(ptr) Py]
}

#___________________________________________________________________________________________________________________________________________
method B207_graph_plot Draw_current_poly_zone_end_to {x y} {
 if {[string equal $this(current_poly_zone) ""]} {return}
 $this(current_poly_zone) Vider
 set L $this(current_poly_zone_L_pt)
   lappend L $x $y
 switch $this(zone_type) {
   rectangle {$this(current_poly_zone) Ajouter_contour [eval ProcRect $L]
              $this(current_poly_zone) Mode_polygone
             }
   polygon   {if {[llength $L]==4} {$this(current_poly_zone) Mode_ligne} else {$this(current_poly_zone) Mode_polygone}
              $this(current_poly_zone) Ajouter_contour [ProcTabDouble $L]
             }
  }
}

#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
method B207_graph_plot get_current_zone {} {
 return [this get_poly_zone_under [$this(ptr) Px] [$this(ptr) Py]]
}

#___________________________________________________________________________________________________________________________________________
method B207_graph_plot get_poly_zone_under {x y} {
 set pt [B_point $x $y]
 set info_rep [$this(n_rep_poly_zone) Contient $pt [Op_Contient_alors_rien]]
 if {![string equal $info_rep NULL]} {
  #puts "$objName get_poly_zone_under $x $y => $info_rep"
   set noeud [$info_rep NOEUD]
  #puts "  noeud : $noeud"
   Detruire $info_rep
   return $noeud
  }
 return NULL 
}

#___________________________________________________________________________________________________________________________________________
method B207_graph_plot set_ref_vars_to_poly_zone_under_current_point {acces_name L_ref_vars}  {
 set poly_zone [this get_poly_zone_under [$this(ptr) Px] [$this(ptr) Py]]
 if {![string equal $poly_zone NULL]} {
   $poly_zone Ajouter_MetaData_T $acces_name $L_ref_vars
  }
}

#___________________________________________________________________________________________________________________________________________
method B207_graph_plot get_ref_vars_to_poly_zone_under_current_point {acces_name}  {
 set poly_zone [this get_poly_zone_under [$this(ptr Px] [$this(ptr) Py]]
 if {![string equal $poly_zone NULL]} {
   return [$poly_zone Val_MetaData $acces_name
  }
}

# B207_graph_plot gp; $noeud_partage Ajouter_fils [set root [gp get_root]]
# $root Origine 30 30

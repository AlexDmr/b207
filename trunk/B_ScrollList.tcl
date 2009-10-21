#______________________________________________________________________________________________________
#______________________________________________________________________________________________________
#______________________________________________________________________________________________________
method B_ScrollList constructor {} {
 set this(root) [B_noeud]
   set this(panel)        [B_polygone]
     set this(inner_root) [B_noeud]
	 B_configure $this(inner_root) -Position_des_fils_changeable 0
	 B_configure $this(panel) -Ajouter_fils $this(inner_root) \
	                          -Couleur 0.3 0.3 0.3 1 \
							  -Mode_texture 1 \
							  -Mode_texture_fils 1 \
							  -Mode_texture2 1
   set this(scrollbar) ${objName}_scollbar
     B_ScrollBar $this(scrollbar)
   B_configure $this(root) -Ajouter_fils $this(panel) \
                           -Ajouter_fils [$this(scrollbar) get_root]
   
   [$this(scrollbar) get_bt_down] Subscribe_to_Press   $objName "$objName Start_scroll bt_down -1"
   [$this(scrollbar) get_bt_down] Subscribe_to_UnPress $objName "$objName Stop_scroll  bt_down"
   [$this(scrollbar) get_bt_up  ] Subscribe_to_Press   $objName "$objName Start_scroll bt_up    1"
   [$this(scrollbar) get_bt_up  ] Subscribe_to_UnPress $objName "$objName Stop_scroll  bt_up"
   
   this Resize 300 500
   B_configure $this(scrollbar) -set_min 0 -set_max 0
   $this(scrollbar) Subscribe_to_set_value $objName "$objName recalibrate"
   
 return $objName
}

#______________________________________________________________________________________________________
Generate_accessors B_ScrollList [list root scrollbar panel inner_root]

#______________________________________________________________________________________________________
method B_ScrollList Scroll {mark i} {
 #puts "$objName Scroll $mark"
 if {[info exists this(press,$mark)]} {
   $this(scrollbar) set_value [expr [$this(scrollbar) get_value] + $i*[$this(scrollbar) get_step]]
   after 10 "$objName Scroll $mark $i"
  }
}

#______________________________________________________________________________________________________
method B_ScrollList Start_scroll {mark i} {
# puts "$objName Start_scroll $mark"
 set this(press,$mark) [N_i_mere ms]
 after 100 "$objName Scroll $mark $i"
}

#______________________________________________________________________________________________________
method B_ScrollList Stop_scroll {mark} {
# puts "$objName Stop_scroll $mark"
 if {[info exists this(press,$mark)]} {unset this(press,$mark)}
}

#______________________________________________________________________________________________________
method B_ScrollList Optimal_Resize {{sb_X 32}} {
 set max_X 0
 set cumul_Y 0
 foreach e [this get_elements] {
   $e Calculer_boites
   set box [$e Boite_noeud_et_fils_glob]
     set X [$box Tx]; set Y [$box Ty]
   if {$X > $max_X} {set max_X $X}
   set cumul_Y [expr $cumul_Y + $Y]
  }
 
 $this(scrollbar) Resize $max_X $cumul_Y
}

#______________________________________________________________________________________________________
method B_ScrollList Resize {X Y {sb_X 32}} {
 $this(scrollbar) Resize $sb_X $Y
 [$this(scrollbar) get_root] Origine [expr $X - $sb_X] 0
 
 set contour [ProcRect 0 0 [expr $X - $sb_X] $Y]
 B_configure $this(panel) -Vider \
                          -Ajouter_contour $contour
 Detruire $contour
 
 this Layout_elements
}

#______________________________________________________________________________________________________
method B_ScrollList set_elements {L} {
 this Sub_elements [this get_elements]
 this Add_elements $L
}

#______________________________________________________________________________________________________
method B_ScrollList get_elements {} {
 return [Lister_fils_de $this(inner_root)]
}

#______________________________________________________________________________________________________
method B_ScrollList Sub_elements {L} {
 foreach e $L {
   $this(inner_root) Retirer_fils $e
  }
 this Layout_elements
}

#______________________________________________________________________________________________________
method B_ScrollList Add_elements {L} {
 foreach e $L {
   if {![$this(inner_root) A_pour_fils $e]} {
     $this(inner_root) Ajouter_fils_au_debut $e
    }
  }
 this Layout_elements
}

#______________________________________________________________________________________________________
method B_ScrollList Layout_elements {} {
 #puts "$objName Layout_elements"
 set dec 0
 foreach e [this get_elements] {
   $e Origine 0 0
   $e PreRendre
   Update_boites_rec $e
   set box [$e Boite_noeud_et_fils_glob]
   $e Origine [expr -[$box BG_X]] [expr $dec - [$box BG_Y]]
   set dec [expr $dec + [$box Ty]]
   $e Calculer_boites
  }
  
 $this(inner_root) Calculer_boites
 set inner_box  [$this(inner_root) Boite_noeud_et_fils_glob]
 set outter_box [$this(panel)      Boite_noeud]

 set tx [expr [$inner_box Ty] - [$outter_box Ty]]
 
 set sb_scroll  [$this(scrollbar) get_scrollbar]
   set box_scroll [$sb_scroll Boite_noeud]
 set sb_current [$this(scrollbar) get_current] 
   set box_list [$this(panel)      Boite_noeud]
   set box_fils [$this(inner_root) Boite_noeud_et_fils]
   if {[$box_fils Ty] == 0} {
     set v 1
	} else {set v [expr [$box_list Ty] / [$box_fils Ty]]}
   set relative_size [expr $v>1?1:$v]
   $sb_current Ey [expr $relative_size * [$box_scroll Ty]]
 
 $this(scrollbar) set_max [expr $tx>0?$tx:0]
 $this(scrollbar) set_value [$this(scrollbar) get_value]
}

#______________________________________________________________________________________________________
method B_ScrollList recalibrate {} {
 $this(inner_root) Py -[$this(scrollbar) get_value]
}

#______________________________________________________________________________________________________
#______________________________________________________________________________________________________
#______________________________________________________________________________________________________
method B_ScrollBar constructor {} {
 set this(min)   0
 set this(max)   100
 set this(step)  1
 set this(value) 0
 
 set contour       [ProcRect 0 0 1 1]
 set contour_arrow [ProcTabDouble "0.1 0.1 0.9 0.1 0.5 0.9"]
 set this(root) [B_noeud]
   set this(bt_up)     [B_polygone]; set this(arrow_up)  [B_polygone]; 
									   B_configure $this(arrow_up) -Ajouter_contour $contour_arrow \
									                               -Couleur 0.3 0.3 0.3 1 \
																   -Noeud_touchable 0
									 B_configure $this(bt_up)   -Ajouter_contour $contour \
									                            -Ajouter_fils $this(arrow_up)
                                     B_bouton_perso $this(bt_up)_B_perso $this(bt_up) \
                              									      "$this(bt_up) Couleur_perimetre 0 1 0 1; $this(arrow_up) Couleur 0.3 1   0.3 1" \
																	  "$this(bt_up) Couleur_perimetre 0 0 0 1; $this(arrow_up) Couleur 0.3 0.3 0.3 1" \
																	  "$this(bt_up) Couleur 0 1 0 1; $this(arrow_up) Couleur 0.5 1 0.5 1" \
																	  "$this(bt_up) Couleur 1 1 1 1; $this(arrow_up) Couleur 0.3 1 0.3 1" \
																	  "$objName set_value \[expr \[$objName get_value\] + \[$objName get_step\]\]"
   set this(bt_down)   [B_polygone]; set this(arrow_down)  [B_polygone]
                                       B_configure $this(arrow_down) -Ajouter_contour $contour_arrow \
									                                 -Pivoter_interne 3.14159265 0.5 0.5 \
																	 -Couleur 0.3 0.3 0.3 1 \
																	 -Noeud_touchable 0
                                     B_configure $this(bt_down) -Ajouter_contour $contour \
									                            -Ajouter_fils $this(arrow_down)
                                     B_bouton_perso $this(bt_down)_B_perso $this(bt_down) \
                              									      "$this(bt_down) Couleur_perimetre 0 1 0 1; $this(arrow_down) Couleur 0.3 1   0.3 1" \
																	  "$this(bt_down) Couleur_perimetre 0 0 0 1; $this(arrow_down) Couleur 0.3 0.3 0.3 1" \
																	  "$this(bt_down) Couleur 0 1 0 1; $this(arrow_down) Couleur 0.5 1 0.5 1" \
																	  "$this(bt_down) Couleur 1 1 1 1; $this(arrow_down) Couleur 0.3 1 0.3 1" \
																	  "$objName set_value \[expr \[$objName get_value\] - \[$objName get_step\]\]"
   set this(scrollbar) [B_polygone]
     set this(rap_press_scrollbar) [B_rappel [Interp_TCL]]; $this(rap_press_scrollbar) Texte "$objName Scroll_page $this(rap_press_scrollbar)"
     $this(scrollbar) abonner_a_detection_pointeur [$this(rap_press_scrollbar) Rappel] [expr [ALX_pointeur_enfonce] + [Rappel_Seulement_si_noeud_contient]]

	 
   set this(current)   [B_polygone]; B_configure $this(current) -Ajouter_contour $contour \
                                                                -Couleur 0.5 0.5 0.5 1
     $this(current) Ajouter_MetaData_T ${objName}_is_setting_value 0
	 B_contact ctc_${objName}_current "$this(current) 1"
	 set this(rap_current) [B_rappel [Interp_TCL] "$objName Reposition_current"]
	 N_i_mere abonner_a_fin_simulation [$this(rap_current) Rappel]
   B_configure $this(scrollbar) -Ajouter_fils $this(current)
   B_configure $this(root) -Ajouter_fils $this(bt_up) \
                           -Ajouter_fils $this(bt_down) \
						   -Ajouter_fils $this(scrollbar)
   
 this Resize 16 128
 Detruire $contour
 
 return $objName
}

#______________________________________________________________________________________________________
Generate_accessors B_ScrollBar [list root scrollbar current min max step value]

#______________________________________________________________________________________________________
method B_ScrollBar get_bt_down {} {return $this(bt_down)_B_perso}
method B_ScrollBar get_bt_up   {} {return $this(bt_up)_B_perso}

#______________________________________________________________________________________________________
method B_ScrollBar set_min {v} {
 set this(min) $v
 if {$this(value) < $v} {this set_value $this(min)}
}

#______________________________________________________________________________________________________
method B_ScrollBar set_max {v} {
 set this(max) $v
 if {$this(value) > $v} {this set_value $this(max)}
}

#______________________________________________________________________________________________________
method B_ScrollBar Reposition_current {} {
 if {[$this(current) A_changer] && ![$this(current) Val_MetaData ${objName}_is_setting_value]} {
   set box_scrollB [$this(scrollbar) Boite_noeud]
   set box_current [$this(current)   Boite_noeud_et_fils_glob]
   
   if {[$box_scrollB Ty] != [$box_current Ty]} {
     set v [expr [$this(current) Py] / ([$box_scrollB Ty]-[$box_current Ty])]
     set v [expr $v>1?1:($v<0?0:$v)]
     set v [expr $this(min) + ($this(max)-$this(min))*$v]
    } else {set v $this(min)}
	
   this set_value $v
  }
 $this(current) Ajouter_MetaData_T ${objName}_is_setting_value 0
}

#______________________________________________________________________________________________________
method B_ScrollBar Scroll_page {rap} {
 set infos [Void_vers_info [$rap Param]]
 set y     [$infos Y_au_contact]
 
 set box_scrollbar [$this(scrollbar) Boite_noeud]
 set box_current   [$this(current)   Boite_noeud_et_fils_glob]
 set step [expr ($this(max) - $this(min)) * ([$box_current Ty] / ([$box_scrollbar Ty] - [$box_current Ty]))]
 if {$y < [$this(current) Py]} {
   this set_value [expr [this get_value] - $step]
  } else {this set_value [expr [this get_value] + $step]
         }
}

#______________________________________________________________________________________________________
method B_ScrollBar set_value {v} {
 $this(current) Ajouter_MetaData_T ${objName}_is_setting_value 1
 
 if {$v > $this(max)} {set v $this(max)} else {if {$v < $this(min)} {set v $this(min)}}
 set v [expr $this(min) + int(($v - $this(min)) / $this(step)) * $this(step)]
 
 if {$v == $this(value)} {set do_CB 0} else {set do_CB 1}
 set this(value) $v
 
 set box_scrollB [$this(scrollbar) Boite_noeud]
 set box_current [$this(current)   Boite_noeud_et_fils_glob]
 if {$this(max) != $this(min)} {
   $this(current) Origine 0 [expr ($this(value) - $this(min)) * ([$box_scrollB Ty] - [$box_current Ty]) / ($this(max)-$this(min))]
  } else {$this(current) Origine 0 0}
 
 if {$do_CB} {
   # INSERT CALLBACKS HERE
  }
 
 set this(is_setting_value) 0
 return $this(value)
}

#______________________________________________________________________________________________________
method B_ScrollBar Resize {X Y {bt_Y ""}} {
 if {$bt_Y == ""} {set bt_Y $X}
 
 set contour [ProcRect 0 0 $X [expr $Y - 2*$bt_Y]]
   B_configure $this(scrollbar) -Vider \
                                -Ajouter_contour $contour \
								-Origine 0 $bt_Y
 Detruire $contour
 
 B_configure $this(bt_up)   -Etirement $bt_Y $bt_Y -Origine 0 [expr $Y-$bt_Y]
 B_configure $this(bt_down) -Etirement $bt_Y $bt_Y -Origine 0 0
 
 $this(current) Etirement $X $X
}

#______________________________________________________________________________________________________
Manage_CallbackList B_ScrollBar set_value ""


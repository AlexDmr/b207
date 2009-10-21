source B_2D_plotter.tcl

#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
method B_2D_multi_plotter_2_ref constructor {} {
 set this(Pool_text) {}
 set this(id_plotter) 0
 set this(Pool_windows) {}
 set this(L_multi_plotter)    {}
 set this(Pool_multi_plotter) {}
 set this(toolglass_root) [B_noeud]
   set this(toolglass_bord)   [B_polygone]; $this(toolglass_root) Ajouter_fils $this(toolglass_bord)
   set this(toolglass_centre) [B_polygone]; $this(toolglass_root) Ajouter_fils $this(toolglass_centre)
   $this(toolglass_bord)   Ajouter_contour [ProcRect -50 -50 50 50]
   $this(toolglass_centre) Ajouter_contour [ProcRect -30 -30 30 30]; $this(toolglass_centre) Couleur 0 1 1 0.5
   $this(toolglass_bord) Difference $this(toolglass_centre)
   B_contact   ctc_$this(toolglass_bord) "$this(toolglass_root) 0" -add "$this(toolglass_bord) 1"
   C_B_Toolglass $this(toolglass_centre) "if {\[catch {$objName Clic_on_zone \[\$infos NOEUD\]} err]} {puts \"$objName : ERROR in $this(toolglass_bord)\n\$err\"}"
}

#___________________________________________________________________________________________________________________________________________
method B_2D_multi_plotter_2_ref dispose {} {
 foreach e $this(L_multi_plotter)    {$e dispose}
 foreach e $this(Pool_multi_plotter) {$e dispose}
 this inherited
}

#___________________________________________________________________________________________________________________________________________
method B_2D_multi_plotter_2_ref get_root {} {return $this(toolglass_root)}

#___________________________________________________________________________________________________________________________________________
method B_2D_multi_plotter_2_ref get_a_window {title tx ty} {
 if {[llength $this(Pool_windows)]} {
   set rep [lindex $this(Pool_windows) 0]
   set this(Pool_windows) [lreplace $this(Pool_windows) 0 0]
   $rep Titre $title
   $rep Longueur_corp $tx
   $rep Hauteur_corp  $ty
   $rep Mettre_a_jour
  } else {set rep [B_fiche $title $tx $ty [N_i_mere]]
         }
 $rep Couleur_fond 0 0 1 1
 return $rep
}

#___________________________________________________________________________________________________________________________________________
method B_2D_multi_plotter_2_ref Release_window {w} {
 Add_element this(Pool_windows) $w
 $w Vider_peres
}

#___________________________________________________________________________________________________________________________________________
method B_2D_multi_plotter_2_ref get_a_unique_id {} {
 incr this(id_plotter)
 return $this(id_plotter)
}

#___________________________________________________________________________________________________________________________________________
method B_2D_multi_plotter_2_ref Release_multi_plotter {p} {
 Add_element this(Pool_multi_plotter) $p
 set f [[$p get_root] Val_MetaData ${objName}_win]
 [$p get_root] Vider_peres
 this Release_window $f
 $p set_ref_var ""
}

#___________________________________________________________________________________________________________________________________________
method B_2D_multi_plotter_2_ref get_a_multi_plotter {} {
 if {[llength $this(Pool_multi_plotter)]} {
   set rep [lindex $this(Pool_multi_plotter) 0]
   set this(Pool_multi_plotter) [lreplace $this(Pool_multi_plotter) 0 0]
  } else {set rep ${objName}_plotter_[this get_a_unique_id]
          B_2D_multi_plotter $rep
         }
 Add_element this(L_multi_plotter) $rep
 return $rep
}

#___________________________________________________________________________________________________________________________________________
method B_2D_multi_plotter_2_ref Clic_on_zone {noeud} {
 puts "$objName Clic_on_zone $noeud" 
 set L_vars [$noeud Val_MetaData Meta_IHM_PM_P_B207_toolglass_art_resizing_L_var_obj]
 foreach v $this(L_multi_plotter) {this Release_multi_plotter $v}
 set this(L_multi_plotter) {}
 set pos 0
 foreach v $L_vars {
   puts "  - $v"
   set p [this get_a_multi_plotter]
   set f [this get_a_window $v 768 384]; $f Origine_du_contenu -256 0; [$p get_root] Ajouter_MetaData_T ${objName}_win $f
   [$this(toolglass_root) Pere] Ajouter_fils_au_debut $f
   $f Ajouter_fils [$p get_root]
   $p set_ref_var $v
   incr pos
  }
}

#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
method B_2D_multi_plotter constructor {} {
 set this(Pool_text)        {}
   set this(txt_root)       [B_noeud]
 set this(L_var_plotter)    {}
 set this(Pool_of_plotters) {}
 set this(ref_var)          {}
 set this(id_plotter)       0
 set this(root)            [B_noeud]
   $this(root) Ajouter_fils $this(txt_root)
}

#___________________________________________________________________________________________________________________________________________
method B_2D_multi_plotter dispose {} {
 foreach e $this(Pool_of_plotters) {$e dispose}
 foreach e $this(L_var_plotter)    {[lindex $e 1] dispose}
 Detruire $this(root)
 this inherited
}

#___________________________________________________________________________________________________________________________________________
method B_2D_multi_plotter get_a_unique_id_plotter {} {
 incr this(id_plotter)
 return $this(id_plotter)
}

#___________________________________________________________________________________________________________________________________________
method B_2D_multi_plotter get_Pool_text {} {return $this(Pool_text)}

#___________________________________________________________________________________________________________________________________________
method B_2D_multi_plotter get_a_text {plot txt} {
# puts "$objName get_a_text $plot $txt"
 if {[llength $this(Pool_text)]} {
   set rep [lindex $this(Pool_text) 0]
   set this(Pool_text) [lreplace $this(Pool_text) 0 0]
   set rappel [$rep Val_MetaData B_2D_multi_plotter_2_ref_plot]
  } else {set rep    [B_texte 20 100 30 [fonte_Arial] [B_sim_sds]]
          set rappel [B_rappel [Interp_TCL]]
		    $rep abonner_a_detection_pointeur [$rappel Rappel] [ALX_pointeur_relache]
            $rep Ajouter_MetaData_T B_2D_multi_plotter_2_ref_plot $plot
         }
 $rep TEXTE $txt; $rep Optimiser_dimensions; $rep Editable 0
 $rep Calculer_boites
 set plot_root [$plot get_root]
   B_configure $rep -Couleur_fond 0 [$rep Afficher_noeud] 0 1 -Afficher_fond 1 -Afficher_boites 1
 $rappel Texte "set ${objName}_plot_root \[$plot get_root\]; B_configure \$${objName}_plot_root -Afficher_noeud \[expr 1-\[\$${objName}_plot_root Afficher_noeud\]\]; B_configure $rep -Afficher_boites 1 -Afficher_noeud 1 -Couleur_fond 0 \[\$${objName}_plot_root Afficher_noeud\] 0 1"

 return $rep
}

#___________________________________________________________________________________________________________________________________________
method B_2D_multi_plotter Release_text {t} {
 Add_element this(Pool_text) $t
 $t Vider_peres
}

#___________________________________________________________________________________________________________________________________________
method B_2D_multi_plotter get_ref_var { } {return $this(ref_var)}

#___________________________________________________________________________________________________________________________________________
method B_2D_multi_plotter get_root { } {return $this(root)}

#___________________________________________________________________________________________________________________________________________
method B_2D_multi_plotter set_ref_var {v} {
#puts "$objName set_ref_var $v"
 set L $this(L_var_plotter)
 foreach v_p $L {
   set r [this Sub_var [lindex $v_p 0]]
   #puts "$objName Sub_var [lindex $v_p 0] => $r"
  }
 set this(L_var_plotter) {}
 set this(ref_var) $v
 if {[string equal $v {}]} {return}
 foreach v_rel [$v get_L_related_vars] {
   set plot [this Add_var $v_rel]
   $this(root) Ajouter_fils_au_debut [$plot get_root]
  }
}

#___________________________________________________________________________________________________________________________________________
method B_2D_multi_plotter Add_var {v} {
#puts "$objName Add_var $v"
 foreach v_p $this(L_var_plotter) {
   if {[string equal $v [lindex $v_p 0]]} {return {}}
  }
 set plot [this get_a_plotter]; $plot Represent_var $v 512 384
 lappend this(L_var_plotter) [list $v $plot]
   # Add the corresponding name
   [$plot get_root] Afficher_noeud 1
   set txt [this get_a_text $plot [$v get_cmd_get]]; set box [$txt Boite_noeud]
   $this(txt_root) Ajouter_fils $txt
   B207_Line_v $this(txt_root) right
 return $plot
}

#___________________________________________________________________________________________________________________________________________
method B_2D_multi_plotter Sub_var {v} {
#puts "$objName Sub_var $v"
 set pos 0
 foreach v_p $this(L_var_plotter) {
   if {[string equal $v [lindex $v_p 0]]} {
     set this(L_var_plotter) [lreplace $this(L_var_plotter) $pos $pos]
	 this Release_a_plotter [lindex $v_p 1]
	 return 1
	}
   incr pos
  }
 return 0
}

#___________________________________________________________________________________________________________________________________________
method B_2D_multi_plotter get_plotter_of_var {v} {
 foreach v_p $this(L_var_plotter) {
   if {[string equal $v [lindex $v_p 0]]} {return [lindex $v_p 1]}
  }
 return {}
}

#___________________________________________________________________________________________________________________________________________
method B_2D_multi_plotter get_a_plotter {} {
 if {[llength $this(Pool_of_plotters)]} {
   set rep [lindex $this(Pool_of_plotters) 0]
   set this(Pool_of_plotters) [lreplace $this(Pool_of_plotters) 0 0]
  } else {set rep ${objName}_plotter_[this get_a_unique_id_plotter]
          B_2D_plotter $rep
         }
 return $rep
}
#___________________________________________________________________________________________________________________________________________
method B_2D_multi_plotter get_Pool_of_plotters {} {return $this(Pool_of_plotters)}
method B_2D_multi_plotter get_L_var_plotter    {} {return $this(L_var_plotter)}

#___________________________________________________________________________________________________________________________________________
method B_2D_multi_plotter Release_a_plotter {p} {
 Add_element this(Pool_of_plotters) $p
 [$p get_root] Vider_peres
}

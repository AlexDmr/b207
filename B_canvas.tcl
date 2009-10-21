#_________________________________________________________________________________________________________
#_________________________________________________________________________________________________________
#_________________________________________________________________________________________________________
method BIGre_canvas constructor {{racine ""}} {
 set this(root) [B_noeud]
   set n_rep_fils [B_noeud]; $this(root) Ajouter_fils $n_rep_fils; set this(n_rep_fils) $n_rep_fils
   $n_rep_fils Position_des_fils_changeable 0
   set this(contact_fond) ${objName}_contact_fond

   set this(fond) ""
 
 if {$racine != ""} {$racine Ajouter_fils_au_debut $this(root)}
 set this(L_nodes) {}
 
 set this(rap_clic_canvas) [B_rappel [Interp_TCL]]
   $this(rap_clic_canvas) Texte "$objName Clic_on $this(rap_clic_canvas)"
 $n_rep_fils abonner_a_detection_pointeur [$this(rap_clic_canvas) Rappel] [ALX_pointeur_enfonce]
 
# Highlighters
 set this(L_pool_highlighters)    [list]
 set this(L_actives_highlighters) [list]
 
 set this(contour_highlighter) [ProcRect 0 0 1 1]
 set this(color_highlighter)   "1 1 0 0.5"
 
 return $objName
}

#_________________________________________________________________________________________________________
method BIGre_canvas dispose {} {
 this Vider
 
 this inherited
}

#_________________________________________________________________________________________________________
#_________________________________________________________________________________________________________
#_________________________________________________________________________________________________________
method BIGre_canvas get_highlighters {} {return $this(L_actives_highlighters)}

#_________________________________________________________________________________________________________
method BIGre_canvas get_a_highlighter {} {
 if {[llength $this(L_pool_highlighters)] > 0} {
   set rep [lindex $this(L_pool_highlighters) 0]
   set this(L_pool_highlighters) [lrange $this(L_pool_highlighters) 1 end]
  } else {set rep [B_polygone]
          B_configure $rep -Ajouter_contour $this(contour_highlighter) \
		                   -Gerer_contacts 0
         }

 $this(n_rep_fils) Ajouter_fils_au_debut $rep
 eval "$rep Couleur $this(color_highlighter)"
 lappend this(L_actives_highlighters) $rep
 return $rep
}

#_________________________________________________________________________________________________________
method BIGre_canvas release_a_highlighter {e} {
 set pere [$e Pere]; if {$pere != ""} {$pere A_changer 1}
 $e Vider_peres
 lappend this(L_pool_highlighters) $e
 set this(L_actives_highlighters) [lremove $this(L_actives_highlighters) $e]
}

#_________________________________________________________________________________________________________
method BIGre_canvas set_color_highlighters {r v b a} {
 foreach n $this(L_actives_highlighters) {
   $n Couleur $r $v $b $a
  }
}

#_________________________________________________________________________________________________________
method BIGre_canvas Stop_Enlight {} {
 set L $this(L_actives_highlighters)
 foreach n $L {this release_a_highlighter $n}
}

#_________________________________________________________________________________________________________
method BIGre_canvas Enlight {L_L_nodes} {
 foreach L_nodes $L_L_nodes {
   #puts "L_nodes : $L_nodes"
   if {[llength $L_nodes]==0} {continue}
   set n [lindex $L_nodes 0]; set L_nodes [lrange $L_nodes 1 end]
   #$n Calculer_boites
   set box  [$n Boite_noeud_et_fils_glob]
   #puts "box = $box"
   set x1 [$box BG_X]; set y1 [$box BG_Y]; set x2 [$box HD_X]; set y2 [$box HD_Y]
   foreach n $L_nodes {
     set box  [$n Boite_noeud_et_fils_glob]
	 #puts "box = $box"
	 set BG_X [$box BG_X]; set BG_Y [$box BG_Y]; set HD_X [$box HD_X]; set HD_Y [$box HD_Y]
	 if {$BG_X < $x1} {set x1 $BG_X}
	 if {$BG_Y < $y1} {set y1 $BG_Y}
	 if {$HD_X > $x2} {set x2 $HD_X}
	 if {$HD_Y > $y2} {set y2 $HD_Y}
    }
   set highlighter [this get_a_highlighter]
   
   B_configure $highlighter -Etirement [expr 1.2*($x2 - $x1)]       [expr 1.2*($y2 - $y1)] \
                            -Origine   [expr $x1 - 0.1*($x2 - $x1)] [expr $y1 - 0.1*($y2 - $y1)]
  }
}

#_________________________________________________________________________________________________________
#_________________________________________________________________________________________________________
#_________________________________________________________________________________________________________
method BIGre_canvas Clic_on {rap} {
 set infos [Void_vers_info [$rap Param]]
 set noeud [Real_class [$infos NOEUD]]
 
 this Select $noeud
}

#_________________________________________________________________________________________________________
method BIGre_canvas Select {node} {
}

#_________________________________________________________________________________________________________
Manage_CallbackList BIGre_canvas [list Select] end

#_________________________________________________________________________________________________________
method BIGre_canvas Has_node {n} {
 if {[lsearch $this(L_nodes) $n] == -1} {return 0} else {return 1}
}

#_________________________________________________________________________________________________________
method BIGre_canvas Link_contacts_and_co {} {
 set this(noeud_rep_fils) $this(n_rep_fils)
 set this(rap_fen_graph) [$this(root) Val_MetaData rap_fen_graph]
 if {[string equal $this(rap_fen_graph) {}]} {
   set this(rap_fen_graph) [B_rappel [Interp_TCL]]
   $this(root) Ajouter_MetaData_T rap_fen_graph     $this(rap_fen_graph) 
  }
  $this(rap_fen_graph) Texte "$this(root) Ne_pas_pre_rendre \[expr 1-\[$this(noeud_rep_fils) A_changer\]\]"
  $this(root) desabonner_de_LR_parcours [$this(root) LR_Av_aff] [$this(rap_fen_graph) Rappel]
  $this(root) abonner_a_LR_parcours     [$this(root) LR_Av_aff] [$this(rap_fen_graph) Rappel]
 set this(rap_fen_graph_dim) [$this(root) Val_MetaData rap_fen_graph_dim]
 if {[string equal $this(rap_fen_graph_dim) {}]} {
   set this(rap_fen_graph_dim) [B_rappel [Interp_TCL]]
   $this(root) Ajouter_MetaData_T rap_fen_graph_dim $this(rap_fen_graph_dim)
  }
  $this(rap_fen_graph_dim) Texte "$this(noeud_rep_fils) A_changer 1"
#  $this(root) desabonner_de_dimension [$this(rap_fen_graph_dim) Rappel]
#  $this(root) abonner_a_dimension     [$this(rap_fen_graph_dim) Rappel]
} 

#_________________________________________________________________________________________________________
method BIGre_canvas get_fen  {} {return $this(root)}
method BIGre_canvas get_root {} {return $this(root)}
method BIGre_canvas get_repere_fils {} {return $this(n_rep_fils)}
method BIGre_canvas get_fond {} {return $this(fond)}

#_________________________________________________________________________________________________________
method BIGre_canvas get_L_nodes {} {return $this(L_nodes)}

#_________________________________________________________________________________________________________
method BIGre_canvas Gestion_ctc {} {
 set n $this(n_rep_fils)
 set it [$n Init_parcours_inverse_fils]
 if {[$n Est_parcours_inverse_fils_fini $it]} {$n Terminer_parcours_inverse_fils $it; return}
 set fond [$n Courant_dans_parcours_inverse_fils $it]
 if {[gmlObject info exists object $this(contact_fond)]} {puts "$this(contact_fond) dispose : [$this(contact_fond) dispose]"
                                                         } else {puts "$this(contact_fond) n'existe pas"}
 B_contact $this(contact_fond) "$n 1" -add "$fond 1" -pt_trans 50 50 -pt_rot 50 50
 $this(contact_fond) pt_rot_actif 0
 
 $fond Gerer_contacts 1
 $this(root) Ajouter_MetaData_T contact_fond $this(contact_fond)
 this set_contact_to 11
}

#_________________________________________________________________________________________________________
method BIGre_canvas get_contact_fond {} {return $this(contact_fond)}

#_________________________________________________________________________________________________________
method BIGre_canvas set_contact_to {property} {
 set n $this(n_rep_fils)
 set it [$n Init_parcours_inverse_fils]
 if {[$n Est_parcours_inverse_fils_fini $it]} {$n Terminer_parcours_inverse_fils $it; return}
 set fond [$n Courant_dans_parcours_inverse_fils $it]
   set boite [$fond Boite_noeud]
 $this(contact_fond) add "$fond $property"
 $this(contact_fond) pt_trans [$boite Cx] [$boite Cy]
 $this(contact_fond) pt_rot   [$boite Cx] [$boite Cy]
}

#_________________________________________________________________________________________________________
method BIGre_canvas Convert_col {col} {
 if {$col == "red"  } {return "1 0 0 1"}
 if {$col == "blue" } {return "0 0 1 1"}
 if {$col == "green"} {return "0 1 0 1"}
 if {$col == "black"} {return "0 0 0 1"}
 if {$col == "white"} {return "1 1 1 1"}
 if {$col == "grey" } {return "0.5 0.5 0.5 1"}
 if {$col == "grey60" } {return "0.6 0.6 0.6 1"}
 if {$col == "green4"} {return "0 [expr 0x8b/255.0] 0 1"}

 if {[regexp "^#(.)(.)(.)$" $col reco r v b]} {return "[expr Ox$r/15.0] [expr 0x$v/15.0] [expr 0x$b/15.0]"}
 return "0 0 0 1"
}

#_________________________________________________________________________________________________________
method BIGre_canvas Invert_y_for_coords {str_name} {
 if {$this(fond) == ""} {return}
 
 upvar $str_name str
 set new_str ""
 
 set box_fond [$this(fond) Boite_noeud]; set Y [$box_fond Ty]
 foreach {x y} $str {
   lappend new_str $x [expr $Y - $y]
  }
  
 set str $new_str
}

#_________________________________________________________________________________________________________
method BIGre_canvas create args {
   # XXX DEBUG /XXX
     #return; 
   # XXX	 
#puts $args
 set cmd {}
 regsub {\-smooth true} $args {} gogo
 if {[regexp "^polygon (.*) \\-fill (.*) \\-outline (.*) \\-tags (.*)\$" $gogo reco coords col_fond col_bord tag]} {
   this Invert_y_for_coords coords
   set cmd "set n [B_polygone]; \$n Ajouter_contour [ProcTabDouble $coords]; \$n Couleur [this Convert_col $col_fond]; \$n Couleur_perimetre [this Convert_col $col_bord]; \$n Ajouter_MetaData_T tags $tag"
 } else {
 if {[regexp "^text (.*) (.*) \\-text (.*) \\-fill (.*) \\-font (.*) \\-state (.*) \\-tags (.*)\$" $gogo reco x y txt col fonte state tag]} {
   if {[regexp { ([0-9]*) } $fonte reco taille]} {} else {set taille 18}
   set taille 12
   set rect_fond [lindex $this(L_nodes) end]; $rect_fond Calculer_boites; set b_rect_fond [$rect_fond Boite_noeud_et_fils_glob]; 
   if {![regexp {^.*graph.*$} [$rect_fond Val_MetaData tags]]} {
     set cmd "set n \[B_texte 1000 1000 [expr [$b_rect_fond Ty]-2] [fonte_Arial] [B_sim_sds]\]; \$n Maj_texte $txt; \$n Editable 0; \$n Optimiser_dimensions; set n_box \[\$n Boite_noeud\]; \$n Mode_texture 1; \$n Etirement \[expr 0.8*[$b_rect_fond Tx]/\[\$n Largeur\]\] \[expr 0.5*[$b_rect_fond Ty]/\[\$n Hauteur\]\]; \$n Calculer_boites; \$n Origine \[expr [$b_rect_fond Cx] - \[\$n_box Cx\]*\[\$n Ex\]\] \[expr 2 + [$b_rect_fond Cy] - \[\$n_box Cy\]* \[\$n Ey\]\]; \$n Couleur_texte [this Convert_col $col]; \$n Ajouter_MetaData_T tags $tag;"
	}
 } else {
 if {[regexp "^oval (.*) (.*) (.*) (.*) \\-fill (.*) \\-outline (.*) \\-tags (.*)\$" $gogo reco X1 Y1 X2 Y2 col_fond col_bord tag]} {
   set MX [expr ($X2+$X1)/2.0]
   set MY [expr ($Y2+$Y1)/2.0]
   set DX [expr ($X2-$X1)/2.0]
   set DY [expr ($Y2-$Y1)/2.0]
       set box_fond [$this(fond) Boite_noeud]; set Y [$box_fond Ty]
	   set MY [expr $Y - $MY]
   set cmd "set n [B_polygone]; \$n Ajouter_contour [ProcOvale $MX $MY $DX $DY 60]; \$n Couleur [this Convert_col $col_fond]; \$n Couleur_perimetre [this Convert_col $col_bord]; \$n Ajouter_MetaData_T tags $tag"
 } else {
 if {[regexp "^line (.*) \\-fill (.*) \\-tags (.*)\$" $gogo reco coords col tag]} {
   this Invert_y_for_coords coords
   set coords [get_Spline $coords 16]
   set cmd "set n [B_polygone]; \$n Ajouter_contour \[ProcTabDouble \{[Go_and_return $coords]\}\]; \$n Couleur [this Convert_col $col]; \$n Couleur_perimetre [this Convert_col $col]; \$n Ajouter_MetaData_T tags $tag"
 }}}}

# Evaluate the command
 #puts "  => Eval de \"$cmd\""
 if {[string equal $cmd {}]} {} else {
   eval $cmd
   eval "$this(n_rep_fils) Ajouter_fils_au_debut \$n"
   if {$this(L_nodes) == ""} {set this(fond) $n}
   if {[regexp {^.*graph.*$} [$n Val_MetaData tags]]} {
     $n Gerer_contacts 0
	}
   lappend this(L_nodes) $n
  }
#puts "___ END of $objName create" 
}

#_________________________________________________________________________________________________________
method BIGre_canvas Vider {} {
 this Stop_Enlight
 foreach n $this(L_nodes) {
   Detruire $n
  #puts "Detruire $n"
  }
 set this(L_nodes) ""
 set this(fond)    ""
}

#_________________________________________________________________________________________________________
method BIGre_canvas get_fond {} {return $this(fond)}

#_________________________________________________________________________________________________________
method BIGre_canvas get_nodes_representing {e} {
 set L_rep [list]
 foreach node [this get_L_nodes] {
   if {[$node Val_MetaData CometPM] == $e} {lappend L_rep $node}
  }
 return $L_rep
}

#_________________________________________________________________________________________________________
method BIGre_canvas Maj_MetaData_Markers {mark} {
 set L [this get_L_nodes]
 foreach n [lrange $L 1 end] {
   set c [$n Val_MetaData tags]
   if {[llength $c] == 1} {
     $n Ajouter_MetaData_T  $mark $c
	 $n Ajouter_au_contexte $mark $c
    }
  }
}

#_________________________________________________________________________________________________________
# GraphServer contient une adresse et un port
method BIGre_canvas Maj_with {GraphServer txt_name} {
 this Vider
 upvar $txt_name txt
 set ip   [lindex $GraphServer 0]
 set port [lindex $GraphServer 1]
 set s [socket -async $ip $port]
   #puts "WRITE"
   puts $s $txt
   flush $s
   set rep {}
   #set marker_fin "\n \n\n" 
   #puts "READ"
   set rep [read $s]
 close $s  
 #puts "GENERATE POLYGONES"
 set c $objName
   set __tkgen_smooth_type ""
   set rep [string range $rep [string first {$c} $rep] end]
 eval $rep
 
 this Gestion_ctc
 this Link_contacts_and_co
 this Maj_MetaData_Markers CometPM
}

#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
method FishEye_on_Images constructor {L_images} {
 set this(root) [B_polygone]; 
   set c [ProcRect 0 0 400 300]
   B_configure $this(root) -Ajouter_contour $c 
   set this(rap_aff_root) [B_rappel [Interp_TCL]]
   $this(root) abonner_a_LR_parcours [$this(root) LR_Ap_PR_fils] [$this(rap_aff_root) Rappel]
 set this(L_pool_img) {}
 set this(L_images) ""
 this set_L_images $L_images
}

#___________________________________________________________________________________________________________________________________________
Generate_accessors FishEye_on_Images [list root rap_aff_root L_images]

#___________________________________________________________________________________________________________________________________________
method FishEye_on_Images set_color_bg {r g b a} {
 $this(root) Couleur $r $g $b $a
}

#___________________________________________________________________________________________________________________________________________
method FishEye_on_Images set_dims {tx ty} {
 set c [ProcRect 0 0 $tx $ty]
   $this(root) Vider
   $this(root) Ajouter_contour $c
 Detruire $c
}

#___________________________________________________________________________________________________________________________________________
method FishEye_on_Images get_rap_aff_roo_txt {} {
 return [$this(rap_aff_root) Texte]
}

#___________________________________________________________________________________________________________________________________________
method FishEye_on_Images set_rap_aff_roo_txt {txt} {
 $this(rap_aff_root) Texte $txt
}

#___________________________________________________________________________________________________________________________________________
method FishEye_on_Images Release_img {img} {
 lappend this(L_pool_img) $img
 $img Vider_peres
}

#___________________________________________________________________________________________________________________________________________
method FishEye_on_Images get_a_img {{src {}}} {
 if {[llength $this(L_pool_img)]} {
   set rep [lindex $this(L_pool_img) 0]
   foreach img $this(L_pool_img) {
     if {[string equal [$img Val_MetaData src] $src]} {set rep $img; break;}
	}
   set this(L_pool_img) [lremove $this(L_pool_img) $rep]
  } else {set rep [B_image]
          if {![string equal $src ""]} {$rep maj $src}
         }
 $rep Ajouter_MetaData_T src $src
 return $rep
}

#___________________________________________________________________________________________________________________________________________
method FishEye_on_Images set_L_images {L} {
 foreach img $this(L_images) {this Release_img $img}
 set this(L_images) ""
 foreach e $L {
   set img [this get_a_img $e]
   #$img maj $e
   #$img Ajouter_MetaData_T src $e
   $this(root) Ajouter_fils_au_debut $img
   lappend this(L_images) $img
  }
}

#___________________________________________________________________________________________________________________________________________
method FishEye_on_Images set_E_for_daughters {e} {
 foreach img $this(L_images) {$img Etirement $e $e}
}

#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
proc B207_flow {root} {
 set surrounding_box [$root Boite_noeud]
 set it [$root Init_parcours_inverse_fils]
 $root Position_des_fils_changeable 0
 set y 0; set num_l 0; 
 while {![$root Est_parcours_inverse_fils_fini $it]} {
  # Compute line
   set acc_x 0; set max_y 0; set L_n {}; 
   while {![$root Est_parcours_inverse_fils_fini $it]} {
     set nf [$root Courant_dans_parcours_inverse_fils $it]
	   $nf Calculer_boites
	   set box [$nf Boite_noeud_et_fils_glob]
	   set acc_x [expr $acc_x + [$box Tx]]
	   if {$acc_x > [$surrounding_box Tx] && [llength $L_n]} {break;}
	 lappend L_n $nf
	 set box_ty [$box Ty]
	 set max_y [expr $box_ty>$max_y?$box_ty:$max_y]
	 set it [$root Suivant_dans_parcours_inverse_fils $it]
    }
  # Placement of the line
   set x 0; set num_c 0
   foreach n $L_n {
     set box [$n Boite_noeud_et_fils]
	   $n Origine [expr $x] $y
	   $n Ajouter_MetaData_T FishEye_pos "$num_l $num_c"
	   incr num_c
	 set x [expr $x + [$box Tx]*[$n Ex]]
    }
   set y [expr $max_y + $y]
   incr num_l
  }
  
 $root Terminer_parcours_inverse_fils $it
 
 # Compute resulting box  
 $root Calculer_boites
}


#___________________________________________________________________________________________________________________________________________
proc Compute_Fish_Eye {x X T d} {
# Calcul de rep_x
 set rep_x $x
 if {$x < $X} {
   set e [expr ($d+1)/($d + $X/($X-$x))]
   set rep_x [expr $X*(1-$e)] 
  }
 if {$x > $X} {
   set e [expr ($d+1)/($d + ($T-$X)/($x-$X))]
   set rep_x [expr $X+($T-$X)*$e] 
  }
  
# return the result point
 return $rep_x
}

#___________________________________________________________________________________________________________________________________________
proc B207_position_fisheye {root d} { 
 set L_ptr [$root Val_MetaData L_ptr]
 if {[llength $L_ptr] == 0} {return}
 set pt [lindex $L_ptr 0]
   set pt_x [lindex $pt 1]
   set pt_y [lindex $pt 2]
   set ptr [B_sim_ptr Pointeur [lindex $pt 3]]
   #puts $ptr
   if {![string equal $ptr NULL]} {
     #puts coucou
	 #puts [$ptr Val_MetaData I]
	 #set d [expr [$ptr Val_MetaData I]/5500.0]
     #puts $d
	 #.l configure -text $d 
	}
 $root Calculer_boites
 set surrounding_box [$root Boite_noeud]; 
   set TX [$surrounding_box Tx]
   set TY [$surrounding_box Ty]
   set T_ref [expr sqrt($TX*$TX + $TY*$TY)]
 set it [$root Init_parcours_inverse_fils]
 while {![$root Est_parcours_inverse_fils_fini $it]} {
   set nf [$root Courant_dans_parcours_inverse_fils $it]
     $nf Calculer_boites
	 set box [$nf Boite_noeud_et_fils_glob]
	   set Cx [$box Cx]; set Cy [$box Cy]
	   set BG_X [$box BG_X]; set BG_Y [$box BG_Y]
	   set HD_X [$box HD_X]; set HD_Y [$box HD_Y]
	   set x1 [Compute_Fish_Eye $BG_X $pt_x $TX $d]
	   set x2 [Compute_Fish_Eye $HD_X $pt_x $TX $d] 
	   set y1 [Compute_Fish_Eye $BG_Y $pt_y $TY $d]
	   set y2 [Compute_Fish_Eye $HD_Y $pt_y $TY $d] 
       $nf Origine [expr ($x1+$x2)/2.0]	[expr ($y1+$y2)/2.0]
	     set box_nf [$nf Boite_noeud_et_fils]
		   set ex [expr ($x2 - $x1)/[$box_nf Tx]]
		   set ey [expr ($y2 - $y1)/[$box_nf Ty]]
		   set e  [expr $ex<$ey?$ex:$ey]
		   $nf Etirement $e $e
	     $nf Calculer_boites
		   set box [$nf Boite_noeud_et_fils_glob]
		   $nf Translation [expr -[$box Cx]+[$box BG_X]] [expr -[$box Cy]+[$box BG_Y]]
   set it [$root Suivant_dans_parcours_inverse_fils $it]
  }
 $root Terminer_parcours_inverse_fils $it
}

if {![gmlObject info exists object fish] && 0} {
 set L_img [glob "./cesame/*.jpg"]
 FishEye_on_Images fish [lrange $L_img 0 8]
 fish set_E_for_daughters 0.3
 fish set_dims 1000 600
 set root [fish get_root]
   fish set_rap_aff_roo_txt "fish set_E_for_daughters 0.3; B207_flow $root; B207_position_fisheye $root 2"
 $noeud_partage Vider_fils
 $noeud_partage Ajouter_fils $root
 B_contact ctc_root "$root 1"
   $root Couleur 0 0 0 0.7
 set pt_tmp [B_point]
 #B_configure $root -Contenu_dans_fils_ssi_contenu_dans_noeud 1 -Contenu_dans_fils_ssi_pas_contenu_dans_noeud 1
 set rap_root_mvt [B_rappel [Interp_TCL]]
   $rap_root_mvt Texte "if {\[catch {Change_fish $rap_root_mvt $root $pt_tmp} err\]} {puts \"Change_fish ERROR\\n\$err\"}"
   $root abonner_a_detection_pointeur [$rap_root_mvt Rappel] [ALX_pointeur_mouvement]
}

proc Change_fish {rap root pt_tmp} {
 set infos [$rap Param]
 set infos [Void_vers_info $infos]
 set ptr [$infos Ptr]
 set n [Real_class [$infos NOEUD]]
 set L_rep [Ancetre_de_a $n $root]
     $pt_tmp maj [$infos Point_au_contact]
	 B207_transformation_inverse $pt_tmp [lrange $L_rep 1 end]
   $root Ajouter_MetaData_T L_ptr [list "souris [$pt_tmp X] [$pt_tmp Y] [$ptr Id]"]
   #puts "ptr : $ptr\n  - Is : [$ptr Id]"
}


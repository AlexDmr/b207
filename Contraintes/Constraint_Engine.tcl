#______________________________________________________________________________
#______________________________________________________________________________
#______________________________________________________________________________
proc load_constraints_from_in {file_name} {
 set f [open $file_name]
   set rep [read $f]
 close $f
 return $rep
}

#______________________________________________________________________________
proc interpolate_min {v L} {
 set pos 0; set lenght [llength $L]
 while {$pos < $lenght} {
   set cpl [lindex $L $pos]
   set v_pos [lindex $cpl 0]
   if {$v_pos > $v} {break}
   incr pos
  }
 set pos [expr $pos>0?$pos-1:0]
 set cpl [lindex $L $pos]
 return [lindex $cpl 1]
}

#______________________________________________________________________________
proc inter_extra_polate {v L} {
 set taille_L [llength $L]
# Cas pas de valeurs fournies
 if {$taille_L == 0} {return 0}
 set cpl0 [lindex $L 0]
 set v0   [lindex $cpl0 0]
 set r0   [lindex $cpl0 1]
# Cas une seule valeur fournie 
 if {$taille_L == 1} {return $r0}
 set pos 0
 while {$v > $v0 && $pos < $taille_L} {
   set cpl0 [lindex $L $pos]
   set v0   [lindex $cpl0 0]
   incr pos
  }
# Cas extrapolation supérieur
 if {$pos != 0} {
   set cpl1 $cpl0
   incr pos -2
   set cpl0 [lindex $L $pos]
   set v0   [lindex $cpl0 0]
   set r0   [lindex $cpl0 1]
  } 
# set r0 [lindex $cpl0 1]
 
# Interpolation
 set cpl1 [lindex $L [expr $pos + 1]]
 set v1   [lindex $cpl1 0]
 set r1   [lindex $cpl1 1]
 if {$v0 == $v1} {return $r0}
 set alpha [expr double($v1-$v)/double($v1-$v0)]
 return [expr $r0*$alpha + $r1*(1-$alpha)]
}

#______________________________________________________________________________
#______________________________________________________________________________
#______________________________________________________________________________
method B207_Transfo_Structure constructor {} {
  set this(noeud_redim) [B_noeud]
  $this(noeud_redim) Etirement 400 400
  set this(pol_dim_rect) [B_polygone]
    $this(pol_dim_rect) Couleur 1 1 1 0.5
    $this(noeud_redim) Ajouter_fils $this(pol_dim_rect)
    $this(pol_dim_rect) Ajouter_contour [ProcRect 0 0 1 1]
    set this(pol_dim_oval) [B_polygone]
      $this(pol_dim_rect) Ajouter_fils $this(pol_dim_oval)
	  B_configure $this(pol_dim_oval) -Ajouter_contour [ProcOvale 0.5 0.5 0.4 0.4 60] -Couleur 0 0 1 0.5
  set this(noeud_redim_boxes) [B_noeud]
    $this(noeud_redim) Ajouter_fils $this(noeud_redim_boxes)
    set this(pol_dim_left)   [B_polygone]; $this(pol_dim_left)   Ajouter_contour [ProcRect -15 -15 15 15]; $this(noeud_redim_boxes) Ajouter_fils $this(pol_dim_left)  ; 
    set this(pol_dim_right)  [B_polygone]; $this(pol_dim_right)  Union $this(pol_dim_left)               ; $this(noeud_redim_boxes) Ajouter_fils $this(pol_dim_right) ; 
    set this(pol_dim_top)    [B_polygone]; $this(pol_dim_top)    Union $this(pol_dim_left)               ; $this(noeud_redim_boxes) Ajouter_fils $this(pol_dim_top)   ; 
    set this(pol_dim_bottom) [B_polygone]; $this(pol_dim_bottom) Union $this(pol_dim_left)               ; $this(noeud_redim_boxes) Ajouter_fils $this(pol_dim_bottom); 
 	
 set this(ctc_pol_dim)    ctc_pol_dim_$objName   ; B_contact $this(ctc_pol_dim) "$this(noeud_redim) 0" -add "$this(pol_dim_rect) 3" -add "$this(pol_dim_oval) 9"
 set this(ctc_pol_left)   ctc_pol_left_$objName  ; B_contact $this(ctc_pol_left)   "$this(pol_dim_left)   1" 
 set this(ctc_pol_right)  ctc_pol_right_$objName ; B_contact $this(ctc_pol_right)  "$this(pol_dim_right)  1" 
 set this(ctc_pol_top)    ctc_pol_top_$objName   ; B_contact $this(ctc_pol_top)    "$this(pol_dim_top)    1" 
 set this(ctc_pol_bottom) ctc_pol_bottom_$objName; B_contact $this(ctc_pol_bottom) "$this(pol_dim_bottom) 1" 

 set this(rap_cstr) [B_rappel [Interp_TCL] "$objName Redim"]
 set this(rap_redim_box_mvt) [B_rappel [Interp_TCL]]
   $this(rap_redim_box_mvt) Texte "if {\[$this(pol_dim_left) A_changer\] || \[$this(pol_dim_right) A_changer\] || \[$this(pol_dim_top) A_changer\] || \[$this(pol_dim_bottom) A_changer\]} {eval \[$this(rap_cstr) Texte\]}"
   $this(noeud_redim) abonner_a_LR_parcours [$this(noeud_redim) LR_Av_PR_fils] [$this(rap_redim_box_mvt) Rappel]
 set this(rap_redim_zone_mvt) [B_rappel [Interp_TCL] "if {\[$this(noeud_redim) A_changer\]} {$objName Movment}"]
   $this(pol_dim_rect) abonner_a_LR_parcours [$this(pol_dim_rect) LR_Av_PR_fils] [$this(rap_redim_zone_mvt) Rappel]
 set this(node_original) [B_noeud]
}

#______________________________________________________________________________
method B207_Transfo_Structure get_root_node {} {
 return $this(noeud_redim)
}

#______________________________________________________________________________
method B207_Transfo_Structure Translucidity {v} {
 B_configure $this(pol_dim_rect) -Translucidite 1 -Couleur 1 1 1 $v
 B_configure $this(pol_dim_oval) -Translucidite 1 -Couleur 0 0 1 $v
}

#______________________________________________________________________________
method B207_Transfo_Structure dispose {} {
#foreach n $this(L_nodes) {$n -delete}
 this inherited
}

#______________________________________________________________________________
method B207_Transfo_Structure Redim {} {
 set px1 [$this(pol_dim_left)   Px]
 set px2 [$this(pol_dim_right)  Px]
 set py1 [$this(pol_dim_top)    Py]
 set py2 [$this(pol_dim_bottom) Py]
   $this(noeud_redim) Etirement [expr $px2-$px1] [expr $py1-$py2]

 set noeud_redim_ex [$this(noeud_redim) Ex]
 set noeud_redim_ey [$this(noeud_redim) Ey]
   $this(noeud_redim_boxes) Etirement [expr 1.0/$noeud_redim_ex ] [expr 1.0/$noeud_redim_ey ]
   $this(pol_dim_rect)      Origine   [expr $px1/$noeud_redim_ex] [expr $py2/$noeud_redim_ey]
   set box [$this(pol_dim_rect) Boite_noeud_et_fils]
   set px [expr $px1+$noeud_redim_ex*[$box Cx]]; set py [expr $py2+$noeud_redim_ey*[$box Cy]]
   $this(pol_dim_left)  Py $py; $this(pol_dim_bottom) Px $px
   $this(pol_dim_right) Py $py; $this(pol_dim_top)    Px $px
   set px [expr [$this(pol_dim_rect) Px]+[$box Cx]]; set py [expr [$this(pol_dim_rect) Py]+[$box Cy]]
   $this(ctc_pol_dim) pt_rot $px $py; $this(ctc_pol_dim) pt_trans $px $py
 $this(managed_node) Etirement [expr $this(node_ref_original_ex)*$noeud_redim_ex/$this(noeud_redim_original_ex)] [expr $this(node_ref_original_ey)*$noeud_redim_ey/$this(noeud_redim_original_ey)]
 this Movment
}

#______________________________________________________________________________
method B207_Transfo_Structure Movment {} {
# Apply transformation to managed node
 $this(managed_node) Origine  [$this(noeud_redim) Origine]
   $this(managed_node) Rotation [$this(noeud_redim) Rotation]
   $this(managed_node) Calculer_boites
   set box [$this(managed_node) Boite_noeud_et_fils]
   $this(managed_node) Translation_interne [expr [$this(pol_dim_rect) Px] * [$this(noeud_redim) Ex] - [$box BG_X]*[$this(managed_node) Ex]] [expr [$this(pol_dim_rect) Py]*[$this(noeud_redim) Ey]  - [$box BG_Y]*[$this(managed_node) Ey]]
}

#______________________________________________________________________________
method B207_Transfo_Structure Manage_node {node} {
 set this(managed_node) $node
 set this(noeud_redim_original_ex) [$this(noeud_redim) Ex]
 set this(noeud_redim_original_ey) [$this(noeud_redim) Ey]
 set this(node_ref_original_ex)    [$node Ex]
 set this(node_ref_original_ey)    [$node Ey]

 [$node Pere] Ajouter_fils_au_debut $this(noeud_redim)
# Copy the CometPM information
 $this(noeud_redim) Ajouter_au_contexte CometPM [$node Val CometPM]
# Node bounding box
$node Calculer_boites
 set box [$node Boite_noeud_et_fils]
# Orientation, shift
 $this(noeud_redim) Rotation   [$node Rotation]
 $this(noeud_redim) Glissement [$node Glissement]
# Placement of redim boxes
 #set px [$node Px]; set py [$node Py]
 #set px [$this(noeud_redim) Ex]; set py [$this(noeud_redim) Ey]
 set pere [$node Pere]
   set e_courant [$pere Etirement_courant]
   set inv_e_pere_x [expr 1.0/[$e_courant Dx]]
   set inv_e_pere_y [expr 1.0/[$e_courant Dy]]
 set px 0; set py 0; set ex [$node Ex]; set ey [$node Ey]
   $this(noeud_redim) Origine [$node Origine]
 $this(pol_dim_left)   Origine [expr $ex*[$box BG_X] + $px] [expr $ey*[$box Cy]   + $py]; $this(pol_dim_left)   Etirement $inv_e_pere_x $inv_e_pere_y
 $this(pol_dim_right)  Origine [expr $ex*[$box HD_X] + $px] [expr $ey*[$box Cy]   + $py]; $this(pol_dim_right)  Etirement $inv_e_pere_x $inv_e_pere_y
 $this(pol_dim_bottom) Origine [expr $ex*[$box Cx]   + $px] [expr $ey*[$box BG_Y] + $py]; $this(pol_dim_bottom) Etirement $inv_e_pere_x $inv_e_pere_y
 $this(pol_dim_top)    Origine [expr $ex*[$box Cx]   + $px] [expr $ey*[$box HD_Y] + $py]; $this(pol_dim_top)    Etirement $inv_e_pere_x $inv_e_pere_y
# Redemension
 this Redim
 $this(node_original) maj $node
 set this(noeud_redim_original_ex) [$this(noeud_redim) Ex]
 set this(noeud_redim_original_ey) [$this(noeud_redim) Ey]

# Save managed node in context of the managing polygons
 [this get_root_node] Ajouter_au_contexte B207_Transfo_Structure_Managed_node $node
 [this get_root_node] Ajouter_au_contexte B207_Transfo_Structure              $objName
}

#______________________________________________________________________________
method B207_Transfo_Structure UnManage_node {} {
 $this(noeud_redim) Vider_peres
 set this(managed_node) ""
}


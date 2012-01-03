method Panel constructor {panel_root panel L_img} {
 # La boite englobante de panel nous aidera à placer le bouton
 $panel Calculer_boites
 set panel_box [$panel Boite_noeud_et_fils]

 set this(side) right
 
 # Nous définissons un polygone texturé
 set this(bt_img)  [B_polygone]
  B_configure $this(bt_img) -Couleur 1 1 1 1 \
					        -Ajouter_contour [ProcOvale 0 0 16 16 60] \
					        -Info_texture [[lindex $L_img 1] Info_texture]

 # La fonction B_bouton_from construit un bouton à partir d'un noeud quelconque (argument 1)
 # Elle branche ce bouton sous un noeud pere (argument 2)
 # L'argument 3 définit s'il s'agit d'un bouton rectangulaire (valeur 1) ou oval (valeur 2)
 set this(bouton) [B_bouton_from $this(bt_img) $panel_root 2]
   set this(rap_click) [B_rappel [Interp_TCL] "$objName toggle_extension"]
   B_configure $this(bouton) -Origine [$panel_box Tx] [expr [$panel_box Ty]/2] \
                             -abonner_a_activation [$this(rap_click) Rappel]

 # Nous définissons ensuite des attributs utiles pour gérer l'animation
 set this(anim_time)  330
 set this(extended)   1
 set this(panel_root) $panel_root
 set this(panel)      $panel
 set this(L_images)   $L_img
 set this(L_cmd)      [list "$objName Stretch_to 0" "$objName Stretch_to 1"]
 
 #puts "$objName : this(bouton) = $this(bouton)"
 
 this set_side right
 
 return $objName
}

#______________________________________________________________________________________________________
method Panel get_button {} {return $this(bouton)}

#______________________________________________________________________________________________________
Generate_accessors Panel [list side]

#______________________________________________________________________________________________________
method Panel set_side {v} {
 set this(side) $v
 
 set PI 3.14156592
 set panel_box [$this(panel) Boite_noeud_et_fils]
 switch $v {
   left   {$this(bouton) Origine [$panel_box BG_X] [expr [$panel_box BG_Y] + [$panel_box Ty]/2]; $this(bouton) Rotation $PI}
   right  {$this(bouton) Origine [$panel_box HD_X] [expr [$panel_box BG_Y] + [$panel_box Ty]/2]; $this(bouton) Rotation 0}
   top    {$this(bouton) Origine [expr [$panel_box BG_X] + [$panel_box Tx]/2] [$panel_box HD_Y]; $this(bouton) Rotation [expr  $PI / 2.0]}
   bottom {$this(bouton) Origine [expr [$panel_box BG_X] + [$panel_box Tx]/2] [$panel_box BG_Y]; $this(bouton) Rotation [expr -$PI / 2.0]}
  }
}

#______________________________________________________________________________________________________
method Panel toggle_extension {} {
 #puts "$objName Panel::toggle_extension ($this(extended))"
 set this(extended) [expr 1 - $this(extended)]
 eval [lindex $this(L_cmd) $this(extended)]
 #puts "$objName Panel::toggle_extension END"
}

#______________________________________________________________________________________________________
method Panel Stretch_to {e} {
 set ex_prcdt [$this(panel) Ex]; set ey_prcdt [$this(panel) Ey]
 set panel_box [$this(panel) Boite_noeud_et_fils_glob]
 puts "set panel_box \[$this(panel) Boite_noeud_et_fils_glob\]\n\t$panel_box"
 if {$this(side) == "left" || $this(side) == "right"} {
   set    cmd "$this(panel) Etirement \[expr \$v*$e + (1-\$v)*$ex_prcdt\] 1\n"
   append cmd "$this(panel) Calculer_boites\n"
   if {[this get_side] == "right"} {
     append cmd "$this(bouton) Origine \[$panel_box HD_X\] \[expr \[$panel_box BG_Y\] + \[$panel_box Ty\]/2.0\]\n"
    } else {append cmd "$this(bouton)     Origine \[$panel_box BG_X\] \[expr \[$panel_box BG_Y\] + \[$panel_box Ty\]/2.0\]\n"
	        append cmd "$this(panel_root) Px \[expr [$this(panel_root) Px] + ([$panel_box Tx] - \[$panel_box Tx\]) \]\n"
		   }
  }
 if {$this(side) == "top" || $this(side) == "bottom"} {
   set    cmd "$this(panel) Etirement 1 \[expr \$v*$e + (1-\$v)*$ey_prcdt\]\n"
   append cmd "$this(panel) Calculer_boites\n"
   if {[this get_side] == "top"} {
     append cmd "$this(bouton) Origine \[expr \[$panel_box BG_X\] + \[$panel_box Tx\]/2.0\] \[$panel_box HD_Y\] \n"
    } else {append cmd "$this(bouton) Origine \[expr \[$panel_box BG_X\] + \[$panel_box Tx\]/2.0\] \[$panel_box BG_Y\] \n"
	        append cmd "$this(panel_root) Py \[expr [$this(panel_root) Py] + ([$panel_box Ty] - \[$panel_box Ty\]) \]\n"
		   }
  }
 B_transfo_rap $this(anim_time) $cmd "$this(panel) Noeud_touchable 0" "$this(panel) Noeud_touchable 1; $this(bt_img) Info_texture [[lindex $this(L_images) $this(extended)] Info_texture]"
}

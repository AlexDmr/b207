source Object_gfx.tcl
source Vaisseau.tcl

#_________________________________________________________________________________________________________
#_________________________________________________________________________________________________________
#_________________________________________________________________________________________________________
inherit Joueur Object_gfx

#_________________________________________________________________________________________________________
method Joueur constructor {nom couleur} {
 this inherited
 
# Partie fonctionnelle
 set this(nom)         $nom
 set this(L_vaisseaux) [list]

# Partie graphique
 set this(root) [B_polygone]
   set this(poly_color)    [B_polygone]
     set this(rap_color) [B_rappel [Interp_TCL] "after 10 \"$objName set_color \[tk_chooseColor\]\""]
     B_configure $this(poly_color) -Ajouter_contour [ProcRect 0 0 30 30] \
	                               -Origine 0 10 \
								   -abonner_a_detection_pointeur [$this(rap_color) Rappel] [ALX_pointeur_enfonce]
   set this(txt_nom)       [B_texte 30 $nom]
     B_configure $this(txt_nom) -Editable 1 \
	                            -Origine 37 10 \
								-Largeur 120
   set this(poly_vaisseau) [B_polygone]
     B_configure $this(poly_vaisseau) -Ajouter_contour [ProcOvale 0 0 10 10 60] \
	                                  -Origine 185 25 \
									  -Ajouter_MetaData_T CLASSE Vaisseau \
									  -Ajouter_MetaData_T Joueur $objName \
									  -Ajouter_MetaData_T Rayon  10
   B_configure $this(root) -Ajouter_fils $this(poly_color) \
                           -Ajouter_fils $this(txt_nom) \
						   -Ajouter_fils $this(poly_vaisseau) \
						   -Ajouter_contour [ProcRect 0 0 200 50] \
						   -Couleur 0.2 0.2 0.2 1
 set this(root_for_vaisseaux) ""
 
# Configuration
 this set_color [lindex $couleur 0] [lindex $couleur 1] [lindex $couleur 2] [lindex $couleur 3]

# Fin
 return $objName
}

#_________________________________________________________________________________________________________
method Joueur dispose {} {
 foreach v $this(L_vaisseaux) {$v dispose}
 this inherited
}

#_________________________________________________________________________________________________________
Generate_accessors     Joueur [list nom color root_for_vaisseaux txt_nom poly_color poly_vaisseau]
Generate_List_accessor Joueur L_vaisseaux L_vaisseaux

#_________________________________________________________________________________________________________
method Joueur set_color {args} {
 if {[llength $args] == 4} {
   set r [lindex $args 0]
   set v [lindex $args 1]
   set b [lindex $args 2]
   set a [lindex $args 3]
  } else {set r [expr 0x[string range $args 1 2] / 255.0]
          set v [expr 0x[string range $args 3 4] / 255.0]
		  set b [expr 0x[string range $args 5 6] / 255.0]
		  set a 1
         }
 
 set this(color) "$r $v $b $a"
 $this(poly_color)    Couleur $r $v $b $a
 $this(poly_vaisseau) Couleur $r $v $b $a
 foreach V $this(L_vaisseaux) {
   $V set_color $r $v $b $a
  }
}

#_________________________________________________________________________________________________________
#_________________________________________________________________________________________________________
#_________________________________________________________________________________________________________
method Joueur Add_L_vaisseaux {LV} {
 Add_list this(L_vaisseaux) $LV
 foreach V $LV {
   $this(root_for_vaisseaux) Ajouter_fils_au_debut [$V get_root]
   [$V get_poly] Ajouter_MetaData_T Joueur $objName
  }
}

#_________________________________________________________________________________________________________
method Joueur Sub_L_vaisseaux {LV} {
 Sub_list this(L_vaisseaux) $LV
 foreach V $LV {
   $this(root_for_vaisseaux) Retirer_fils [$V get_root]
   $V dispose
  }
}

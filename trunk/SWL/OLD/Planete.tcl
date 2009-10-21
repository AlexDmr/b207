source Object_gfx.tcl

#_________________________________________________________________________________________________________
inherit Planete Object_gfx

#_________________________________________________________________________________________________________
#_________________________________________________________________________________________________________
#_________________________________________________________________________________________________________
method Planete constructor {x y r d} {
 this inherited
 
# Partie fonctionnelle
 set this(x)       $x
 set this(y)       $y
 set this(rayon)   $r
 set this(densite) $d
 
 this update_masse

# Partie graphique
 set this(root) [B_noeud]
 set this(poly) [B_polygone]
   $this(root) Ajouter_fils $this(poly)
   set contour [ProcOvale 0 0 1 1 60]
     B_configure $this(poly) -Ajouter_contour $contour
	 B_configure $this(root) -Origine $x $y \
	                         -Etirement $r $r \
							 -Position_des_fils_changeable 0
     # Partie redimensionnement
	 set this(redim) [B_polygone]
	   $this(root) Ajouter_fils_au_debut $this(redim)
	   B_configure $this(redim) -Ajouter_contour $contour \
	                            -Origine $r 0
     B_contact ctc_$this(root) "$this(root) 0" -add "$this(poly) 1"
   Detruire $contour

# Fin
 return $objName
}

#_________________________________________________________________________________________________________
method Planete dispose {} {
 ctc_$this(root) dispose
 Detruire $this(root)
 Detruire $this(poly)
 
 this inherited
}

#_________________________________________________________________________________________________________
Generate_accessors Planete [list rayon masse densite]

#_________________________________________________________________________________________________________
#_________________________________________________________________________________________________________
#_________________________________________________________________________________________________________
method Planete update_masse {} {
 set this(masse) [expr $this(rayon)*$this(rayon)*$this(densite)]
 return $this(masse)
}

#_________________________________________________________________________________________________________
method Vaisseau set_mode {m} {
 if {$mode == "edition"} {
   ctc_$this(root) add "$this(poly) 1"
   B_configure $this(redim) -Afficher_noeud 1 \
                            -Gerer_contacts 1
  } else {ctc_$this(root) add "$this(poly) 0"
          B_configure $this(redim) -Afficher_noeud 0 \
                                   -Gerer_contacts 0
         }
}

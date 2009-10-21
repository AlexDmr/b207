source Object_gfx.tcl

#_________________________________________________________________________________________________________
#_________________________________________________________________________________________________________
#_________________________________________________________________________________________________________
inherit Vaisseau Object_gfx

#_________________________________________________________________________________________________________
method Vaisseau constructor {x y r couleur} {
 this inherited
 
# Partie fonctionnelle
 set this(x)         $x
 set this(y)         $y
 set this(rayon)     $r
 
 set this(angle)     0
 set this(puissance) 0

# Partie graphique
 set this(root) [B_noeud]
     set this(poly) [B_polygone]
     $this(root) Ajouter_fils $this(poly)
	 B_contact ctc_$this(root) "$this(root) 0" -add "$this(poly) 1"
   set contour [ProcOvale 0 0 1 1 60]
     B_configure $this(poly) -Ajouter_contour $contour
	 B_configure $this(root) -Origine $x $y \
							 -Etirement $r $r
   Detruire $contour
   eval "$this(poly) Couleur $couleur"

# Fin
 return $objName
}

#_________________________________________________________________________________________________________
method Vaisseau dispose {} {
 ctc_$this(root) dispose
 puts "Vaisseau $objName : Detruire $this(root)"
 Detruire $this(root)
 puts "Vaisseau $objName : Detruire $this(poly)"
 Detruire $this(poly)
 
 this inherited
}

#_________________________________________________________________________________________________________
Generate_accessors  Vaisseau [list poly rayon angle puissance]

#_________________________________________________________________________________________________________
method Vaisseau set_color {r v b a} {
 $this(poly) Couleur $r $v $b $a
}

#_________________________________________________________________________________________________________
method Vaisseau set_rayon {r} {
 $this(poly) Etirement $r $r
}

#_________________________________________________________________________________________________________
method Vaisseau set_mode {m} {
 if {$mode == "edition"} {
   ctc_$this(root) add "$this(poly) 1"
  } else {ctc_$this(root) add "$this(poly) 0"
         }
}

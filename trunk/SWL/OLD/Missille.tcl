source Object_gfx.tcl

#_________________________________________________________________________________________________________
#_________________________________________________________________________________________________________
#_________________________________________________________________________________________________________
inherit Missille Object_gfx

#_________________________________________________________________________________________________________
method Missille constructor {x y vx vy} {
 this inherited 
 
# Partie fonctionnelle
 set this(x)  $x
 set this(y)  $y
 set this(vx) $vx
 set this(vy) $vy

# Partie graphique
 set this(root) [B_polygone]
 set contour [ProcOvale 0 0 7 7 60]
   B_configure $this(root) -Ajouter_contour $contour \
                           -Couleur 1 0 0 1 \
						   -Origine $x $y
 Detruire $contour

# Fin
 return $objName
}

#_________________________________________________________________________________________________________
method Missille dispose {} {
 Detruire $this(root)

 this inherited
}

#_________________________________________________________________________________________________________
Generate_accessors Missille [list vx vy]

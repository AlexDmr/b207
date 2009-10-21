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
 set this(poly_edition) [B_polygone]
   set this(poly_jeu) [B_polygone] 
   B_configure $this(root) -Ajouter_fils $this(poly_edition) \
                           -Etirement $r $r \
						   -Origine $x $y \
                           -Ajouter_fils_au_debut $this(poly_jeu) \
                           -Position_des_fils_changeable 0
   
   set contour [ProcOvale 0 0 1 1 60]
     B_configure $this(poly_edition) -Ajouter_contour $contour \
	                                 -Origine 0 0
     B_configure $this(poly_jeu)     -Ajouter_contour $contour \
	                                 -Origine 0 0
   
   Detruire $contour
   eval "$this(poly_edition) Couleur $couleur"
   eval "$this(poly_jeu)      Couleur $couleur"

   B_contact ${objName}_ctc "$this(root) 0" -add "$this(poly_edition) 1"
   
   set this(rap_poly_jeu) [B_rappel [Interp_TCL] "puts {COUCOU dans le vaisseau $objName}"]
   
   $this(poly_jeu) abonner_a_detection_pointeur [$this(rap_poly_jeu) Rappel] [ALX_pointeur_apparition]
   
# Partie graphique backup
# set this(root) [B_noeud]
# set this(poly) [B_polygone]
#   $this(root) Ajouter_fils $this(poly)
#   set contour [ProcOvale 0 0 $r $r 60]
#     B_configure $this(poly) -Ajouter_contour $contour \
	                         -Origine $x $y
#   Detruire $contour
#   eval "$this(poly) Couleur $couleur"

# Fin
 return $objName
}

#_________________________________________________________________________________________________________
method Vaisseau dispose {} {
 puts "Vaisseau $objName : Detruire $this(root)"
 Detruire $this(root)
 puts "Vaisseau $objName : Detruire $this(poly)"
 Detruire $this(poly)
 
 this inherited
}

#_________________________________________________________________________________________________________
Generate_accessors  Vaisseau [list poly_jeu poly_edition rayon angle puissance]

#_________________________________________________________________________________________________________
method Vaisseau set_mode {m} {
 if {$m == "jeu"} {set v 1} else {set v 0}
 
 B_configure $this(poly_jeu)     -Afficher_noeud $v          -Gerer_contacts $v
 B_configure $this(poly_edition) -Afficher_noeud [expr 1-$v] -Gerer_contacts [expr 1-$v]
}

#_________________________________________________________________________________________________________
method Vaisseau set_color {r v b a} {
 $this(poly) Couleur $r $v $b $a
}
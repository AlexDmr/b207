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
   
   set contour [ProcOvale 0 0 1 1 60]
     B_configure $this(poly) -Ajouter_contour $contour \
	                         -Origine 0 0
     B_configure $this(root) -Origine $x $y \
                             -Ajouter_fils $this(poly) \
                             -Etirement $r $r
				
 set this(n_edition) [B_noeud]
 $this(root) Position_des_fils_changeable 0
 $this(root) Ajouter_fils_au_debut $this(n_edition)

 set this(poly_translation) [B_polygone]
 set this(poly_etirement) [B_polygone]
 $this(poly_etirement) Couleur 0 1 0 1
 $this(poly_translation) Couleur 1 0 0 1
 $this(n_edition) Ajouter_fils $this(poly_translation)
 $this(n_edition) Ajouter_fils $this(poly_etirement)
 #$this(n_edition) Nom_IU
 #$this(n_edition) Nom_IU EDITION
 $this(poly_etirement)   Ajouter_contour $contour
 Detruire $contour; set contour [ProcOvale 0 0 0.8 0.8 60]
 $this(poly_translation) Ajouter_contour $contour
 Detruire $contour

 $this(poly_etirement) Difference $this(poly_translation)
 B_contact ${objName}_ctc "$this(root) 0" -add "$this(poly_translation) 1" -add "$this(poly_etirement) 10"

 #B_configure $n_edition -Afficher_noeud 0 -Gerer_contacts 0
 
 
#$noeud_partage Ajouter_fils $root
#$root Origine 200 200
#$root Ajouter_fils $planete
#$root Etirement 30 30

#$root Position_des_fils_changeable 0

#Afficher_rec $root
#B_Raise $n_edition
#$poly_translation Ajouter_contour [ProcOvale 0 0 0.8 0.8 60]
#$poly_etirement Ajouter_contour [ProcOvale 0 0 1 1 60]
#$poly_etirement Difference $poly_translation

#set mode 1
#B_configure $n_edition -Afficher_noeud $mode -Gerer_contacts mode

#set mode 0
#B_configure $n_edition -Afficher_noeud $mode -Gerer_contacts mode

#set mode 1
#B_configure $n_edition -Afficher_noeud $mode -Gerer_contacts $mode
#B_configure $n_edition -Afficher_noeud 0 -Gerer_contacts 1


# Fin
 return $objName
}

#_________________________________________________________________________________________________________
method Planete dispose {} {
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
method Planete get_masse {} {
 set this(masse) [expr $this(rayon)*$this(rayon)*$this(densite)]
 return $this(masse)
}

#_________________________________________________________________________________________________________
method Planete get_rayon { } {return [$this(root) Ex]}
method Planete set_rayon {r} {$this(root) Etirement $r $r
                              this update_masse
							 }

method Planete set_mode {m} {
 if {$m == "jeu"} {
   set v 0
  } else {set v 1}
  
 B_configure $this(n_edition) -Afficher_noeud $v -Gerer_contacts $v
}
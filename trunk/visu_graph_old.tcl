source gml_Object.tcl

#___________________________________________________________________________________________
#___________________________________________________________________________________________
#___________________________________________________________________________________________
method visu_graph constructor {racine} {
 global noeud_bigre
 global poly2D
 global ztxt_bigre
 global n_i_mere
 set sds     [$n_i_mere Sim_dispo_saisi]
 set f_Arial [fonte_Arial]

# Marqueur de parcours
 set this(marqueur)       -1
# Longueur du cercle
 set this(L)              100
# Hauteur  du cercle
 set this(H)              60
 set this(nb_sommets)     60
# Le noeud qui englobe
 set this(tab_noeud_poly) [ProcOvale 0 0 $this(L) $this(H) 60]
 set this(noeud_poly)     [$poly2D]
 $this(noeud_poly) Ajouter_contour $this(nb_sommets) [$this(tab_noeud_poly) Tab]
# La flèche
 set l_e                       [expr $this(H)/4.0]
 set l_i                       [expr $this(H)/8.0]
 set this(l_i)                 $l_i
 set L                         [list       0  $this(H)               \
                                     "-$l_e"  [expr $this(H)+3*$l_i] \
                                     "-$l_i"  [expr $this(H)+3*$l_i] \
                                     "-$l_i"  [expr $this(H)+4*$l_i] \
                                      "$l_i"  [expr $this(H)+4*$l_i] \
                                      "$l_i"  [expr $this(H)+3*$l_i] \
                                      "$l_e"  [expr $this(H)+3*$l_i]   ]
 set this(tab_noeud_poly_fleche) [ProcTabDouble $L]
 $this(noeud_poly) Ajouter_contour [expr "int([llength $L]/2)"] [$this(tab_noeud_poly_fleche) Tab]

# Le noeud qui est représenté
 set this(racine) $racine
# La liste des fils
 set this(L_fils) [list]
# Le repère des fils
 set this(n_fils)              [$noeud_bigre]
 set this(noeud_poly_lien)     [$poly2D]
 $this(noeud_poly) Ajouter_fils $this(noeud_poly_lien)
# Longueur de la représentation des fils
 set this(L_f)    0
# Hauteur  de la représentation des fils
 set this(H_f)    0
 $this(noeud_poly) Ajouter_fils $this(n_fils)
 $this(n_fils) Origine 0 [expr -3*$this(H)]
 
# Zone de texte contenant le nom du noeud
 set l [expr 1.6*$this(L)]
 set h [expr 1.4*$this(H)]
 set t [expr 0.6*$this(H)]
 set this(z_txt_nom) [$ztxt_bigre $l $h $t $f_Arial $sds]
 $this(noeud_poly) Ajouter_fils_au_debut $this(z_txt_nom)
 $this(z_txt_nom) Origine [expr -$l/2.0] [expr -$h/2.0]
 $this(z_txt_nom) Editable 0
 $this(z_txt_nom) Couleur_texte 0 0 0 1
 $this(z_txt_nom) Maj_texte [$racine Nom_IU]
}

#___________________________________________________________________________________________
method visu_graph Ajouter_fils {vgf} {
 lappend this(L_fils) $vgf
 $this(n_fils) Ajouter_fils [$vgf Noeud_poly]
}

#___________________________________________________________________________________________
method visu_graph Noeud_poly {} {return $this(noeud_poly)}

#___________________________________________________________________________________________
method visu_graph Positionner {pos_X} {$this(noeud_poly) Px $pos_X}

#___________________________________________________________________________________________
method visu_graph Calculer_boites {} {$this(noeud_poly) Calculer_boites}

#___________________________________________________________________________________________
method visu_graph Boite_englobante {} {return [$this(noeud_poly) Boite_noeud_et_fils_glob]}

#___________________________________________________________________________________________
method visu_graph Longueur_fils {}    {set b [$this(n_fils) Boite_noeud_et_fils_glob]
                                       return [$b Tx]}

#___________________________________________________________________________________________
method visu_graph Relier {marque} {
 $this(racine) Marqueur_de_parcours $marque

 $this(racine) Init_parcours_inverse_fils
 set i 0
 while {[expr ![$this(racine) Est_parcours_inverse_fils_fini]]} {
   set n [$this(racine) Courant_dans_parcours_inverse_fils]
   if {[expr [$n Marqueur_de_parcours] == $marque]} {set nom "vgf_${objName}_$i"
                                                     visu_graph $nom $n
                                                     this Ajouter_fils $nom} else {set nom "vgf_${objName}_$i"
                                                                                   visu_graph $nom $n
                                                                                   this Ajouter_fils $nom
                                                                                   $nom Relier $marque }
   incr i
   $this(racine) Suivant_dans_parcours_inverse_fils
  }
}

#___________________________________________________________________________________________
method visu_graph Recalculer {marque} {
 if {[string equal $marque $this(marqueur)]} {return NULL}
 set $this(marqueur) $marque
 
 set prcdt_X 0
 foreach vgf $this(L_fils) {
  # Récusivité
   set rep_vgf [$vgf Recalculer $marque]
   if {[string equal $rep_vgf NULL]} {
     # Il faudra traiter le cas où le noeud est déja placé
     continue}
  # Placement du sous graphe en fonction de ses dimensions
   set nb   [$vgf Noeud_poly]
   set bnf  [$nb Boite_noeud_et_fils]
   set bnfg [$nb Boite_noeud_et_fils_glob]
     $nb Calculer_boites
     if {[expr $prcdt_X != 0]} {$nb Px [expr $prcdt_X-[$bnf BG_X]]
                                $nb Calculer_boites }
   set prcdt_X [expr [$bnfg HD_X]+$this(L)/5.0]

 }

# Si pas de fils on se barre
 set nb_fils [llength $this(L_fils)]
 if {[expr $nb_fils == 0]} {return OK}

 if {[expr $nb_fils > 1]} {
   $this(n_fils) Calculer_boites
   set bnf_n_fils [$this(n_fils) Boite_noeud_et_fils]
   $this(n_fils) Px [expr -([$bnf_n_fils BG_X]+[$bnf_n_fils HD_X])/2.0]
   $this(n_fils) Calculer_boites
  }

# Positionnement du repère des fils compte tenu des fils
 set tx 0

# Tracer les liens
 set prems [lindex $this(L_fils) 0]  ; set prems [$prems Noeud_poly]
 set derns [lindex $this(L_fils) end]; set derns [$derns Noeud_poly]
 $this(noeud_poly_lien) Vider
 set v $this(l_i)
 set g [expr [$this(n_fils) Px]+[$prems Px]]
 set d [expr [$this(n_fils) Px]+[$derns Px]]
 set L [list "-$v"        0                     \
             "-$v"        [expr -$this(H)-2*$v] \
             [expr $g-$v] [expr -$this(H)-2*$v] \
             [expr $g-$v] [expr -$this(H)-4*$v] \
             [expr $d+$v] [expr -$this(H)-4*$v] \
             [expr $d+$v] [expr -$this(H)-2*$v] \
             "$v"         [expr -$this(H)-2*$v] \
             "$v"         0]
 set td [ProcTabDouble $L]
 $this(noeud_poly_lien) Ajouter_contour [expr int([$td Taille]/2)] [$td Tab]
 
 return OK
}

return "v Recalculer 5\n\$noeud_partage Ajouter_fils \[v Noeud_poly\]\nv Relier 10\nvisu_graph v \$Noeud_scene"

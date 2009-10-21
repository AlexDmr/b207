source gml_Object.tcl

#___________________________________________________________________________________________
#___________________________________________________________________________________________
#___________________________________________________________________________________________
method visu_graph constructor {racine {L 50} {H 30}} {
 global noeud_bigre
 global poly2D
 global ztxt_bigre
 global n_i_mere
 set sds     [$n_i_mere Sim_dispo_saisi]
 set f_Arial [fonte_Arial]

# Marqueur de parcours
 set this(marqueur)       -1
# Longueur du cercle
 set this(L)              $L
# Hauteur  du cercle
 set this(H)              $H
 set this(nb_sommets)     60
# Le noeud qui englobe
 set this(tab_noeud_poly) [ProcOvale 0 0 $this(L) $this(H) $this(nb_sommets)]
 set this(noeud_poly)     [$poly2D]
 $this(noeud_poly) Ajouter_contour $this(nb_sommets) [$this(tab_noeud_poly) Tab]
# La flèche
 set l_e                       [expr $this(H)/3.0]
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

 $this(noeud_poly) Position_des_fils_changeable 0

# Le noeud qui est représenté
 set this(racine) $racine
# Les listes des pères et des fils
 set this(L_peres) [list]
 set this(L_fils)  [list]
# Le repère des fils
 set this(n_fils)              [$noeud_bigre]
 set this(noeud_poly_lien_v)   [$poly2D]
   $this(noeud_poly_lien_v) Perimetre_a_afficher 0
 set this(noeud_poly_lien_h)   [$poly2D]
   $this(noeud_poly_lien_h) Perimetre_a_afficher 0
 $this(noeud_poly) Ajouter_fils $this(noeud_poly_lien_v); $this(noeud_poly_lien_v) Afficher_noeud 0
 $this(noeud_poly) Ajouter_fils $this(noeud_poly_lien_h); $this(noeud_poly_lien_h) Afficher_noeud 0
 set v $this(l_i)
 set L [list "-$v"        0                     \
             "-$v"        [expr -$this(H)-2*$v] \
               $v         [expr -$this(H)-2*$v] \
               $v         0]
 set td [ProcTabDouble $L]
 $this(noeud_poly_lien_v) Ajouter_contour [expr int([$td Taille]/2)] [$td Tab]
 set L [list 0 "-$v" \
             1 "-$v" \
             1   $v  \
             0   $v]
 set td [ProcTabDouble $L]
 $this(noeud_poly_lien_h) Ajouter_contour [expr int([$td Taille]/2)] [$td Tab]


# Longueur de la représentation des fils
 set this(L_f)    0
# Hauteur  de la représentation des fils
 set this(H_f)    0
 $this(noeud_poly) Ajouter_fils $this(n_fils)
 $this(n_fils) Origine 0 [expr -3*$this(H)]
 
# Zone de texte contenant le nom du noeud
 set l [expr 1.7*$this(L)]
 set h [expr 1.5*$this(H)]
 set t [expr 0.6*$this(H)]
 set this(z_txt_nom) [$ztxt_bigre $l $h $t $f_Arial $sds]
 $this(noeud_poly) Ajouter_fils_au_debut $this(z_txt_nom)
 $this(z_txt_nom) Origine [expr -$l/2.0] [expr -$h/2.0]
 $this(z_txt_nom) Editable 0
 $this(z_txt_nom) Couleur_texte 0 0 0 1
 $this(z_txt_nom) Maj_texte [$racine Nom_IU]
 $this(z_txt_nom) Noeud_touchable 0
}

#___________________________________________________________________________________________
method visu_graph get_marqueur {}  {return $this(marqueur)}
method visu_graph set_marqueur {m} {set this(marqueur) $m}

#___________________________________________________________________________________________
method visu_graph get_nom {}  {return [$this(z_txt_nom) TEXTE]}
method visu_graph set_nom {n} {$this(z_txt_nom) Maj_texte $n}

#___________________________________________________________________________________________
method visu_graph Couleur {r v b a} {
 $this(noeud_poly)        Couleur $r $v $b $a
 $this(noeud_poly_lien_v) Couleur $r $v $b $a
 $this(noeud_poly_lien_h) Couleur $r $v $b $a
}

#___________________________________________________________________________________________
method visu_graph Ajouter_fils {vgf} {
 lappend this(L_fils) $vgf
 $this(n_fils) Ajouter_fils [$vgf Noeud_poly]
 $vgf Ajouter_pere $objName
}

#___________________________________________________________________________________________
method visu_graph Ajouter_pere {vg} {lappend this(L_peres) $vg}

#___________________________________________________________________________________________
method visu_graph Retirer_pere {vgp} {
 set pos [lsearch -exact $this(L_peres) $vgp]
 if {[expr $pos == -1]} {return 0}
 set this(L_peres) [lreplace $this(L_peres) $pos $pos]
 $vgp Retirer_fils $objName
 return 1
}

#___________________________________________________________________________________________
method visu_graph Retirer_fils {vgf} {
 set pos [lsearch -exact $this(L_fils) $vgf]
 if {[expr $pos == -1]} {return 0}
 set this(L_fils) [lreplace $this(L_fils) $pos $pos]
 $this(n_fils) Retirer_fils [$vgf Noeud_poly]
 $vgf Retirer_pere $objName
 return 1
}

#___________________________________________________________________________________________
method visu_graph Vider_fils_vg {} {
 set this(L_fils) [list]
 $this(noeud_poly) Retirer_fils $this(noeud_poly_lien_h)
 $this(noeud_poly) Retirer_fils $this(noeud_poly_lien_v)
 $this(noeud_poly) Retirer_fils $this(n_fils)
 $this(racine) Vider_fils
 return 1
}

#___________________________________________________________________________________________
method visu_graph Vider_peres_vg {vg_pere} {
 $vg_pere Retirer_fils $objName
 set racine_p [$vg_pere Noeud_racine]
 $racine_p Retirer_fils $this(racine)
# $this(noeud_poly) Vider_peres
# $this(racine)     Vider_peres
 return 1
}

#___________________________________________________________________________________________
method visu_graph Qui_est_pere_de {vgf marque} {
 if {[string equal $marque $this(marqueur)]} {return ""}
 set $this(marqueur) $marque
 
 set pos [lsearch $this(L_fils) $vgf]
 if {[expr $pos != -1]} {return $objName}
 foreach f $this(L_fils) {
   set rep [$f Qui_est_pere_de $vgf $marque]
   if {[string equal $rep {}]} {} else {return $rep}
  }

 return ""
}

#___________________________________________________________________________________________
method visu_graph Qui_represente {n marque} {
 if {[string equal $marque $this(marqueur)]} {return ""}
 set $this(marqueur) $marque
 
 set noeud [Void_vers_noeud $this(noeud_poly)]
 if {[string equal $n $noeud]} {return $objName}
 foreach vgf $this(L_fils) {
   set rep [$vgf Qui_represente $n $marque]
   if {[string equal $rep {}]} {} else {return $rep}
  }

 return ""
}

#___________________________________________________________________________________________
method visu_graph Noeud_racine {} {return $this(racine)}

#___________________________________________________________________________________________
method visu_graph get_L_fils {}  {return $this(L_fils)}
method visu_graph set_L_fils {L} {set this(L_fils) $L}

#___________________________________________________________________________________________
method visu_graph get_L_peres {}  {return $this(L_peres)}
method visu_graph set_L_peres {L} {set this(L_peres) $L}

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

 set v   [expr $this(L)]
 set rep [list [list "-$v" $v]]
 foreach vgf $this(L_fils) {
  # Récusivité
   set rep_vgf [$vgf Recalculer $marque]
   if {[string equal $rep_vgf NULL]} {
     # Il faudra traiter le cas où le noeud est déja placé
     continue}
  # Placement du sous graphe en fonction de ses dimensions
   set dec 0
   set n 1
   foreach element_f $rep_vgf {
     set gauche_f [lindex $element_f 0]
     set element  [lindex $rep $n]
     if {[string equal $element {}]} {
       set v 0
       break
      } else {set droite   [lindex $element   1]
              set v        [expr $droite-$gauche_f]
             }
     set dec [expr ($dec<$v)?$v:$dec]
     incr n
    }
   if {[expr $dec>0]} {set dec [expr $dec+$this(L)/5.0]}
  # Maj de la position du fils
   $vgf Positionner $dec
  # Maj de la liste des longueurs en fonction des niveaux
   set new_rep [list [lindex $rep 0]]
   set i 1
   foreach e $rep_vgf {set e_rep [lindex $rep $i]
                       if {[string equal $e_rep {}]} {
                         set g [expr $dec+[lindex $e 0]]
                        } else {set g [lindex $e_rep 0]}
                       set d [expr $dec+[lindex $e 1]]
                       lappend new_rep [list $g $d]
                       incr i}
   set n [llength $new_rep]
   set t [llength $rep]
   for {set i $n} {$i < $t} {incr i} {
     lappend new_rep [lindex $rep $i]
    }
   set rep $new_rep
 }

# Si pas de fils on se barre
 if {[expr [llength $this(L_fils)]==0]} {$this(noeud_poly_lien_v) Afficher_noeud 0
                                         $this(noeud_poly_lien_h) Afficher_noeud 0
                                         return $rep}

# Positionnement du repère des fils compte tenu des fils
 set tx 0
 if {[expr [llength $this(L_fils)]>1]} {
   set element [lindex $rep 1]
   if {[string equal $element {}]} {return $rep}
   set e [lindex $rep 1]
   set tx [expr "([lindex $e 1]+[lindex $e 0])/2.0"]
   $this(n_fils) Px [expr -$tx]
  } else {$this(n_fils) Px 0}

# Mise à jour de la liste des bornes gauches/droites
 set n_rep [list [lindex $rep 0]]
 set t [llength $rep]
 for {set i 1} {$i < $t} {incr i} {
   set e [lindex $rep $i]
   set g [lindex $e 0]
   set d [lindex $e 1]
   lappend n_rep [list [expr $g-$tx] [expr $d-$tx]]
  }
 set rep $n_rep

# Calcul du placement des liens
 this Lier_aux_fils

# Renvoyer la liste des cotés droits en fonction de la profondeur
 return $rep
}

method visu_graph Lier_aux_fils {} {
 $this(noeud_poly_lien_v) Afficher_noeud 1
 $this(noeud_poly_lien_h) Afficher_noeud 1

 set prems [lindex $this(L_fils) 0]  ; set prems [$prems Noeud_poly]
 set derns [lindex $this(L_fils) end]; set derns [$derns Noeud_poly]

 set v $this(l_i)
 set g [expr [$this(n_fils) Px]+[$prems Px]]
 set d [expr [$this(n_fils) Px]+[$derns Px]]

 $this(noeud_poly_lien_h) Origine   [expr $g-$v]           [expr -$this(H)-3*$v]
 $this(noeud_poly_lien_h) Etirement [expr 2*$v+($d-$g)]    1
}

return "v Recalculer 5\n\$noeud_partage Ajouter_fils \[v Noeud_poly\]\nv Relier 10\nvisu_graph v \$Noeud_scene"

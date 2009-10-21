source visu_graph.tcl

#________________________________________________________________________________________________________
#________________________________________________________________________________________________________
#________________________________________________________________________________________________________
method model_IU constructor {racine_bigre} {
 set this(marque)                  100
 set this(racine_bigre)            $racine_bigre
 set this(L_vg)                    50
 set this(H_vg)                    25
 set this(racine_visu)             [visu_graph "${objName}_vg" $racine_bigre $this(L_vg) $this(H_vg)]
 this Enregistrer_vg_bigre $this(racine_visu) $this(racine_bigre)
 set this(compteur_vg) 0
}

#________________________________________________________________________________________________________
method model_IU Existe {vg} {
 return [info exists this(tab_crspd_vg_bigre,$vg)]
}

#________________________________________________________________________________________________________
method model_IU Enregistrer_vg_bigre {vg n} {
 if {[info exists this(tab_crspd_bigre_vg,$n)]} {set L $this(tab_crspd_bigre_vg,$n)} else {set L {}}
   lappend L $vg
   set this(tab_crspd_bigre_vg,$n) $L
 set this(tab_crspd_vg_bigre,$vg) $n
 set nom_poly [$vg Noeud_poly]
 set nom_poly [Void_vers_noeud $nom_poly]
 set this(tab_crspd_poly_vg,$nom_poly) $vg
}

#________________________________________________________________________________________________________
method model_IU Nouveau_marqueur  {} {incr this(marque)
                                      return $this(marque)}

#________________________________________________________________________________________________________
method model_IU Noeud_vg_representant {n_b} {if {[info exists this(tab_crspd_bigre_vg,$n_b)]} {return $this(tab_crspd_bigre_vg,$n_b)}
                                             return {}}

#________________________________________________________________________________________________________
method model_IU Noeud_vg_represente_par {n_b} {if {[info exists this(tab_crspd_poly_vg,$n_b)]} {return $this(tab_crspd_poly_vg,$n_b)}
                                               return {}}

#________________________________________________________________________________________________________
method model_IU Racine_vg    {} {return $this(racine_visu)}

#________________________________________________________________________________________________________
method model_IU Racine_bigre {} {return $this(racine_bigre)}

#________________________________________________________________________________________________________
method model_IU Maj {vg} {
 set n [$vg Noeud_racine]
 if {[expr [$n Marqueur_de_parcours]==$this(marque)]} {return 0}
 $n Marqueur_de_parcours $this(marque)
 $vg set_marqueur $this(marque)

# Pour chaque fils, vérifier qu'il correspond au noeud bigre, le créer sinon. Dans tous les cas établir ou invalider les liens.
# On commence par casser les liens après les avoir enregistrés
 set L_fils   [$vg get_L_fils];
 set n_L_fils [list]
 foreach vgf $L_fils {$vg Retirer_fils $vgf}

# Reparcourir les liens bigre pas à pas en les comparant aux liens du modèle pour mettre ce dernier à jour.
# On renvoi 1 s'il y a eut changement, 0 sinon.
 set rep 0
 set pos 0
 $n Init_parcours_inverse_fils
 while {[expr ![$n Est_parcours_inverse_fils_fini]]} {
   set nf_b  [$n Courant_dans_parcours_inverse_fils]
   set vgf   [lindex $L_fils $pos]
   set a_creer 0
   set est_une_replique 0
   set vgf_non_dispo 0
   if {[string equal $vgf {}]} {set vgf_non_dispo 1} else {if {[expr [$vgf get_marqueur] == $this(marque)]} {set vgf_non_dispo 1}}
   if {[string equal $vgf_non_dispo 1]} {
                                if {[info exists this(tab_crspd_bigre_vg,$nf_b)]} {
                                 # Rechercher si un des éléments de la liste correspond à vgf
                                  set trouve 0; set pos_vgf 0
                                  foreach vgf $this(tab_crspd_bigre_vg,$nf_b) {
                                    if {[expr [$vgf get_marqueur] != $this(marque)]} {set trouve 1;
                                                                                      break;}
                                    incr pos_vgf
                                   }
                                  if {[string equal $trouve 1]} {if {[expr $pos_vgf > 0]} {set est_une_replique 1}
                                                                 } else {set a_creer 1;
                                                                         set est_une_replique 1
                                                                         set vgf {} }
                                 } else {set a_creer 1}
                                set rep 1} else {set vgf_b [$vgf Noeud_racine]
                                                 if {[string equal $nf_b $vgf_b]} {set pos_vgf [lsearch $this(tab_crspd_bigre_vg,$nf_b) $vgf]
                                                                                   if {[expr $pos_vgf == -1]} {puts "GROS PROBLEME POUR $vgf ([$nf_b Nom_IU])"}
                                                                                   if {[expr $pos_vgf > 0]}   {puts "REPLIQUE DETECTEE POUR $vgf ([$nf_b Nom_IU])"
                                                                                                               set est_une_replique 1}
                                                                                   } else {if {[info exists this(tab_crspd_bigre_vg,$nf_b)]} {
                                                                                             # Recherche d'un élément non pris dans la liste de représentant de $nf_b
                                                                                              set trouve 0; set pos_vgf 0
                                                                                              foreach vgf $this(tab_crspd_bigre_vg,$nf_b) {
                                                                                                if {[expr [$vgf get_marqueur] != $this(marque)]} {set trouve 1;
                                                                                                                                                  break;}
                                                                                                incr pos_vgf
                                                                                               }
                                                                                              if {[string equal $trouve 1]} {if {[expr $pos_vgf > 0]} {set est_une_replique 1}
                                                                                                                             } else {set a_creer 1;
                                                                                                                                     set est_une_replique 1
                                                                                                                                     set vgf {} }
                                                                                             } else {set a_creer 1}
                                                                                           set rep 1}
                                                }
   if {[expr $a_creer == 1]} {puts "Création d'un vg pour [$nf_b Nom_IU]"
                              incr this(compteur_vg)
                              set nom "${objName}_vg_$this(compteur_vg)"
                              visu_graph $nom $nf_b $this(L_vg) $this(H_vg)
                              this Enregistrer_vg_bigre $nom $nf_b
                              set n_tmp [$nom Noeud_racine]
                              set vgf $nom} else {set n_tmp [$vgf Noeud_racine]}
   if {[expr $est_une_replique == 1]} {puts "$vgf ([$nf_b Nom_IU]) est une réplique"
                                       $vgf Couleur 0 1 1 1} else {$vgf Couleur 1 1 1 1}
   $vgf set_nom [$nf_b Nom_IU]

   $vg Ajouter_fils $vgf
  # On progresse dans la liste de fils du noeud n
   $n Suivant_dans_parcours_inverse_fils
   incr pos
  # Récursion
   if {[expr $est_une_replique == 1]} {} else {set rep [expr $rep + [this Maj $vgf]]}
  }

 return $rep
}

#________________________________________________________________________________________________________
method model_IU Mise_en_forme {vg} {
 incr this(marque)
 puts "$vg Recalculer $this(marque)"
 $vg Recalculer $this(marque)
 puts "FIN Mise en forme"
}
#________________________________________________________________________________________________________
#________________________________________________________________________________________________________
#________________________________________________________________________________________________________
  proc Maj_vg {pere_bigre} {
    set vg  [MIU Racine_vg]
    set n_b [$vg Noeud_poly]
    MIU Nouveau_marqueur
    $pere_bigre Retirer_fils $n_b
      MIU Maj           $vg
      MIU Mise_en_forme $vg
    $pere_bigre Ajouter_fils $n_b
   }


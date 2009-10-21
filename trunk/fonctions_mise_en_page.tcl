source gml_Object.tcl

proc Lister_fils_noeud {noeud} {
 set   rep [list]
 set it [$noeud Init_parcours_inverse_fils]
 while {[expr ![$noeud Est_parcours_inverse_fils_fini $it]]} {
   lappend rep [$noeud Courant_dans_parcours_inverse_fils $it]
   set it [$noeud Suivant_dans_parcours_inverse_fils $it]
  }
 $noeud Terminer_parcours_inverse_fils $it
 
 return $rep
}

proc Mettre_en_segment {L_fmes} {
 foreach f $L_fmes {$f Effectuer_mise_en_segment}
}
#________________________________________________________________________________________________________________________________________________
#________________________________________________________________________________________________________________________________________________
#________________________________________________________________________________________________________________________________________________
method Mise_en_segment constructor {racine type_orga} {
 set this(racine)    $racine
 set this(type_orga) $type_orga
    # type_orga = 0: organisation en ligne
    # type_orga = 1: organisation en colonne
 set this(L_fils)    [Lister_fils_noeud $racine]
# La taille des objects de la liste, si 0 alors c'est leurs taille d'origine,
# si différent de 0 alors on les étire pour qu'ils aient la bonne taille.
 set this(taille)    0
 set this(temps_insertion) 1000
 set this(temps_decalage)  1000
 set this(temps_retrait)   1000
}

#________________________________________________________________________________________________________________________________________________
method Mise_en_segment get_racine {}  {return $this(racine)}
method Mise_en_segment set_racine {n} {set this(racine) $n; this Maj_fils}
#________________________________________________________________________________________________________________________________________________
method Mise_en_segment get_organisation {}  {return $this(type_orga)}
method Mise_en_segment set_organisation {o} {set this(type_orga) $o}
#________________________________________________________________________________________________________________________________________________
method Mise_en_segment Maj_fils {} {set this(L_fils) [Lister_fils_noeud $this(racine)]}
#________________________________________________________________________________________________________________________________________________
method Mise_en_segment Ajouter_noeud {n dx dy} {
 global fiche_bigre
 global fiche_o_bigre
 global image_bigre
 global ztxt_bigre
 global bouton_bigre
 global Noeud_scene
 global n_i_mere
 global noeud_bigre
 global Rappel_TCL
 global transfo_dyn_rep
 global interp_tcl

 set b [$this(racine) Boite_fils]
 switch $this(type_orga) {
   0 {set px [$b X2]
      set py 0}
   1 {set px 0
      set py [$b Y2]}
  }
 set noeud_arr [$noeud_bigre]
 $noeud_arr Origine $px $py
 $n Origine [expr $px+$dx] [expr $py+$dy]
 set transfo [$transfo_dyn_rep $this(temps_insertion) $n $noeud_arr]
 $transfo Demarrer
 $n_i_mere Ajouter_deformation_dynamique $transfo
 lappend this(L_fils) $n
}
#________________________________________________________________________________________________________________________________________________
method Mise_en_segment Retirer_noeud {n dx dy} {

}
#________________________________________________________________________________________________________________________________________________
method Mise_en_segment Effectuer_mise_en_segment {{o ""}} {
 if {[string equal $o ""]} {} else {this set_organisation $o}

 set pos 0
 switch $this(type_orga) {
   0 {foreach n $this(L_fils) {
        set boite [$n Boite_noeud_et_fils_glob]
        set o [$n Origine]
        set ox [$o X]
        set oy [$o Y]
        set dx [expr [$boite X1]-$ox]
        set dy [expr [$boite Y1]-$oy]
        $n Origine [expr $pos-$dx] [expr -$dy]
        set pos [expr $pos+[$boite X2]-[$boite X1]]
        $n Calculer_boite_noeud
       }
     }
   1 {foreach n $this(L_fils) {
        set boite [$n Boite_noeud_et_fils_glob]
        set o [$n Origine]
        set ox [$o X]
        set oy [$o Y]
        set dx [expr [$boite X1]-$ox]
        set dy [expr [$boite Y1]-$oy]
        $n Origine [expr -$dx] [expr $pos-$dy]
        set pos [expr $pos+[$boite Y2]-[$boite Y1]]
        $n Calculer_boite_noeud
       }
     }
  }
# On met à jour les boîtes
 $this(racine) Calculer_boites
}


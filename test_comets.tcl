if {[info exists noeud_partage]} {} else {source groupe.tcl}
proc etire_fond args {}
N_i_mere Afficher_souris
cd ..
cd COMETS/devCOMETS
source tamodia_2006.tcl


return "source graphviz.tcl"

set pos_y 100
set L_nodes {}
for {set i 0} {$i<10} {incr i} {
 set cmd "set n \[B_texte 1000 1000 18 [fonte_Arial] [B_sim_sds]\]; \$n Mode_texture 1; \$n Maj_texte \{coucou $i\}; \$n Editable 0; \$n Optimiser_dimensions; \$n Couleur_fond 0 0 1 1; \$n Origine 0 $pos_y; \$n Couleur_texte 1 1 1 1; \$n Ajouter_au_contexte tags toto$i;"
 eval $cmd
 eval "$noeud_partage Ajouter_fils_au_debut \$n"
 set pos_y [expr $pos_y+[$n Hauteur]]
 lappend L_nodes $n
}

set pos_y 50
for {set i 0} {$i<10} {incr i} {
 set cmd "set n \[B_texte 1000 1000 18 [fonte_Arial] [B_sim_sds]\]; \$n Mode_texture 1; \$n Maj_texte \{gogodancer => $i\}; \$n Editable 0; \$n Optimiser_dimensions; \$n Couleur_fond 0 0 1 1; \$n Origine 0 $pos_y; \$n Couleur_texte 1 1 1 1; \$n Ajouter_au_contexte tags toto$i;"
 eval $cmd
 eval "$fen_editeur_tcl Ajouter_fils_au_debut \$n"
 set pos_y [expr $pos_y+[$n Hauteur]]
 lappend L_nodes $n
}

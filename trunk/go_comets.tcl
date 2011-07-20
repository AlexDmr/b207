if {[info exists noeud_partage]} {} else {source groupe.tcl}
proc etire_fond args {}
N_i_mere Afficher_souris
if {[info exists ::env(ROOT_COMETS)]} {cd $::env(ROOT_COMETS)} else { puts stderr "Impossible de trouver devComet, install and add env variable"}

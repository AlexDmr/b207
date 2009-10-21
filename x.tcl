source test_quickmenu.tcl
return

if {[info exists noeud_partage]} {} else {source groupe.tcl}
proc etire_fond args {}

source ALX_Drawer.tcl

N_i_mere Afficher_souris

ALX_Drawer drawer

$noeud_partage Ajouter_fils [drawer get_canvas]

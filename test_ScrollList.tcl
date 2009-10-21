if {[info exists noeud_partage]} {} else {
 source groupe.tcl
 proc etire_fond args {}
 N_i_mere Afficher_souris
}
source B_ScrollList.tcl

$noeud_editeur_tcl Vider_peres

B_ScrollList sl
$noeud_partage Ajouter_fils [sl get_root]

for {set i 0} {$i<20} {incr i} {
 set txt_$i [B_texte 40 "Item numero $i"]
 set name   txt_$i
 sl Add_elements [subst $$name]
}


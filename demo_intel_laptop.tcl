if {![info exists noeud_partage]} {source groupe.tcl}
set server      [new_Texture_Server_TCP 8010 [N_i_mere]]

N_i_mere Afficher_souris

$noeud_fen     Vider_fils
$noeud_partage Vider_fils
$f_obs         Vider_peres

N_i_mere Creer_groupe 1 "Demo Intel" $noeud_fen 1 1

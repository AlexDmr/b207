if {[info exists noeud_partage]} {} else {
 source groupe.tcl
 proc etire_fond args {}
 N_i_mere Afficher_souris
}


source SWL.tcl

SWL swl
  
  
  $noeud_partage Ajouter_fils_au_debut [swl get_root]
  swl Add_L_planetes [list [Planete P0 100 100 50 5] [Planete P1 400 250 80 1]]
  swl Add_L_joueurs  [list [Joueur J1 toto "0 1 0 1"] [Joueur J2 titi "0 0 1 1"]]
  J1 Add_L_vaisseaux [list [Vaisseau J1_V1 100 400 10 "0 1 0 1"]]
  J2 Add_L_vaisseaux [list [Vaisseau J2_V1 400 400 10 "0 0 1 1"]]
  
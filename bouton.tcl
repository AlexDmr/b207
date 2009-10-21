set b [$bouton_bigre 200 200 $n_i_mere $Noeud_scene 2]
$b Translation 100 100

set r [$Rappel_TCL $interp_tcl {$fen_editeur_tcl Translation_interne -5 0}]
$b abonner_a_activation [$r Rappel]
$noeud_fen Ajouter_fils $b


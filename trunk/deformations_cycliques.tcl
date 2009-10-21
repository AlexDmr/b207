#_________________________________________________________________________________________________
#_________________________________________________________________________________________________
#_________________________________________________________________________________________________
proc deformation_cyclique_1 {tps fen fct} {
 global transfo_dyn
 global Rappel_TCL
 global interp_tcl
 global n_i_mere

 set rappel_1_fin  [$Rappel_TCL $interp_tcl]
 set rappel_1_cour [$Rappel_TCL $interp_tcl]
 $rappel_1_fin Texte "deformation_cyclique_2 $tps $fen Glissement_X_fen"
 set transfo     [$transfo_dyn $tps]
    $rappel_1_cour Texte "set t \[$transfo V_courant\]; $fct $fen \$t"
 $transfo abonner_a_rappel_pendant [$rappel_1_cour Rappel]
 $transfo abonner_a_rappel_fin     [$rappel_1_fin  Rappel]
 $n_i_mere Ajouter_deformation_dynamique $transfo
 $transfo Demarrer
}

#_________________________________________________________________________________________________
proc deformation_cyclique_2 {tps fen fct} {
 global transfo_dyn
 global Rappel_TCL
 global interp_tcl
 global n_i_mere

 set rappel_2_fin  [$Rappel_TCL $interp_tcl]
 set rappel_2_cour [$Rappel_TCL $interp_tcl]
 $rappel_2_fin Texte "deformation_cyclique_1 $tps $fen Glissement_X_fen"
 set transfo     [$transfo_dyn $tps]
    $rappel_2_cour Texte "set t \[$transfo V_courant\]; $fct $fen \[expr 1-\$t]"
 $transfo abonner_a_rappel_pendant [$rappel_2_cour Rappel]
 $transfo abonner_a_rappel_fin     [$rappel_2_fin  Rappel]
 $n_i_mere Ajouter_deformation_dynamique $transfo
 $transfo Demarrer
}

#_________________________________________________________________________________________________
proc Glissement_X_fen     {fen t} {$fen Glissement $t 0}

return {deformation_cyclique_1 2000 $coco Glissement_X_fen}


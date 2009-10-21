set n $fen_editeur_tcl
set r [B_rappel $interp_tcl {set e [$n Etirement_courant]; $fen_editeur_tcl Titre "Etirement courant == <[$e Dx]; [$e Dy]>"}]

$n abonner_a_LR_parcours [$n LR_Av_aff_fils] [$r Rappel]
proc Visu_EC {node} {
 global n
 global r

 $n desabonner_de_LR_parcours [$n LR_Av_aff_fils] [$r Rappel]
 set n $node
 $n abonner_a_LR_parcours     [$n LR_Av_aff_fils] [$r Rappel]
}



if {[info exists poly]} {
 set cmd {}
 append cmd "set e \[\$poly Etirement_courant\];      \
             if \{\[expr \[\$e Dx\] > 2\]\} \{        \
               $poly Ne_pas_afficher_fils 0           \
             \} else \{$poly Ne_pas_afficher_fils 1\} \
            "

 set r_pol [B_rappel $interp_tcl $cmd]
 $poly abonner_a_LR_parcours [$poly LR_Av_aff_fils] [$r_pol Rappel]
}


set icl_tmp  [$info_courbe_lentille]
set icl_tmp2 [$info_courbe_lentille]
set icl  [$info_courbe_lentille]
set iclb [$info_courbe_lentille]
set icl_creux [$info_courbe_lentille]

Courbe_lentille_bombe $iclb
$icl       maj $iclb
$icl_tmp   maj $iclb
$icl_creux maj $iclb
set nb  [$icl Nb]
for {set i 0} {$i<$nb} {incr i} {

 $icl       Affecter $i [$iclb Consulter_x $i] [$iclb Consulter_x $i]
 $icl_creux Affecter $i [$iclb Consulter_y $i] [$iclb Consulter_x $i]
#$iclb_inv  Affecter $i [$iclb Consulter_y $i] [$iclb Consulter_x $i]
}

proc Morpher_lentille {fen len1 len2 t} {
 global icl_tmp
 set nb  [$len1 Nb]
 for {set i 0} {$i<$nb} {incr i} {
   set x1 [$len1 Consulter_x $i]; set y1 [$len1 Consulter_y $i]
   set x2 [$len2 Consulter_x $i]; set y2 [$len2 Consulter_y $i]
   $icl_tmp Affecter $i [expr $x1+$t*($x2-$x1)] [expr $y1+$t*($y2-$y1)]}

 set rep [$fen Rep_lentille]
 set len [$rep ICL]
 $len maj $icl_tmp
 $fen maj_lentille
}

proc Morpher_fzo {fen len2 tps} {
 global info_courbe_lentille
 global transfo_dyn
 global Rappel_TCL
 global icl_tmp2
 global n_i_mere
 global interp_tcl

 set rappel_lentille [$Rappel_TCL  $interp_tcl]

 set rep       [$fen Rep_lentille]
 $icl_tmp2 maj [$rep ICL]

 set transfo         [$transfo_dyn $tps]
 $rappel_lentille Param $transfo
 $rappel_lentille Texte "set t \[$transfo V_courant\]; Morpher_lentille $fen $icl_tmp2 $len2 \$t"

 $transfo abonner_a_rappel_pendant [$rappel_lentille Rappel]
 N_i_mere Ajouter_deformation_dynamique $transfo
 $transfo Demarrer
}

#_______________________
return {Morpher_fzo $fzo $icl_creux 2000}


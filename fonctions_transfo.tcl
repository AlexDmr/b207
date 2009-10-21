proc Transfo_rep {noeud noeud_dest tps} {
 set n_origine [$noeud Val origine]
 if {[string equal $n_origine {}]} {
   set n_origine [B_noeud]
   $noeud Ajouter_au_contexte origine $n_origine
  } else {if {[string equal $noeud_dest $n_origine]} {
            $noeud Ajouter_au_contexte dest_to_delete $noeud_dest
            set n_origine [B_noeud]; $noeud Ajouter_au_contexte origine $n_origine
           }
         }
 $n_origine maj $noeud
 set transfo [B_transfo_rep $tps $noeud $noeud_dest]
   set rap_fin [B_rappel [Interp_TCL] "set dest_to_delete \[$noeud Val dest_to_delete\]; if {\[string equal \$dest_to_delete {}\]} {} else {Detruire \$dest_to_delete; $noeud Retirer_du_contexte dest_to_delete}"]
   $transfo abonner_a_rappel_fin [$rap_fin Rappel]

 N_i_mere Ajouter_deformation_dynamique $transfo
 $transfo Demarrer
 return $transfo
}

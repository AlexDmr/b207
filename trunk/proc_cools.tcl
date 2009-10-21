proc Pivoter_fen {f t pas} {
 global $f;
 set vpas [expr $pas]
 if {$t == 1} {$f Pivoter_interne $pas [expr [$f Longueur_corp]/2 ] [expr [$f Hauteur_corp]/2 ];} else {$f Pivoter_interne $pas 0 0;}
 after idle Pivoter_fen $f $t $pas;
}


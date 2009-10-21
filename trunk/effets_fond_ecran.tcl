proc Etirer_fond {nb pas} {
 global n_i_mere;
 set l [$n_i_mere Largeur]
 set h [$n_i_mere Hauteur]
 etire_fond $nb $nb $pas $l $h 0;
 return "l:$l ; h:$h";
}

proc etire_fond {nb_dep nb pas l_deb h_deb coin} {
 global img_fond;
 global n_i_mere;

 set l [$n_i_mere Largeur]
 set h [$n_i_mere Hauteur]
 set vpasx [expr -$pas]
 set vpasy [expr $vpasx * $h / $l]
 if {$nb <= 0} {
  if {$vpasx < 0} {
   set taille_x [expr -$nb_dep*$vpasx + $l_deb]
   set taille_y [expr -$nb_dep*$vpasy + $h_deb]
   after idle etire_fond $nb_dep $nb_dep [expr -$pas] $taille_x $taille_y $coin;} else {after idle etire_fond $nb_dep $nb_dep [expr -$pas] $l $h [expr ($coin+1)%4];}
  return;
  }
 set tx 0
 set ty 0
 set nb_faits [expr $nb_dep - $nb]
 switch $coin {
   0 {}
   1 {set tx $vpasx;}
   2 {set tx $vpasx; set ty $vpasy;}
   3 {set ty $vpasy;}
  }
 $img_fond Translation $tx $ty
 set o [$img_fond Origine]
 set taille_x [expr -$nb_faits*$vpasx + $l_deb]
 set taille_y [expr -$nb_faits*$vpasy + $h_deb]
 set ex [expr $taille_x / [$img_fond Lg]]
 set ey [expr $taille_y / [$img_fond Ht]]
 $img_fond Etirement $ex $ey
 after 10 etire_fond $nb_dep [expr $nb-1] $pas $l_deb $h_deb $coin;
}

proc fond_normal {} {
 global img_fond;
 global n_i_mere;
 set ex [expr [$n_i_mere Largeur] / [$img_fond Lg]];
 set ey [expr [$n_i_mere Hauteur] / [$img_fond Ht]];
 $img_fond Origine 0 0
 $img_fond Etirement $ex $ey
}

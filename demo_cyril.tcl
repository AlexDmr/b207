set fr1 [$fiche_bigre {Fiche 1} 320 240 $n_i_mere]
set fr2 [$fiche_bigre {Fiche 2} 320 240 $n_i_mere]
set fr3 [$fiche_bigre {Fiche 3} 320 240 $n_i_mere]
set fr4 [$fiche_bigre {Fiche 4} 320 240 $n_i_mere]

$fr1 Hauteur_entete 0; $fr1 Epaisseur_bordure 0; $fr1 Mettre_a_jour; $fr1 Gerer_corps 0
$fr2 Hauteur_entete 0; $fr2 Epaisseur_bordure 0; $fr2 Mettre_a_jour; $fr2 Gerer_corps 0
$fr3 Hauteur_entete 0; $fr3 Epaisseur_bordure 0; $fr3 Mettre_a_jour; $fr3 Gerer_corps 0
$fr4 Hauteur_entete 0; $fr4 Epaisseur_bordure 0; $fr4 Mettre_a_jour; $fr4 Gerer_corps 0

$noeud_partage Ajouter_fils $fr1;
$noeud_partage Ajouter_fils $fr2; $fr2 Origine 321 0
$noeud_partage Ajouter_fils $fr3; $fr3 Origine 0 241
$noeud_partage Ajouter_fils $fr4; $fr4 Origine 321 241


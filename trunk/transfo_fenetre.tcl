set fiche_r [new_alx_noeud_fiche_sdl_opengl "Une fenêtre" 500 500 $n_i_mere]
$fiche_r Position_bg [$fiche_o Position_bg]

$fiche_r Rotation [$fiche_o Rotation]
$fiche_r Longueur_corp [$fiche_o Longueur_corp]
$fiche_r Hauteur_corp  [$fiche_o Hauteur_corp]
$fiche_r Mettre_a_jour

$fiche_o Intercaler_noeud $fiche_r

$Noeud_scene Retirer_fils $fiche_o
$Noeud_scene Ajouter_fils $fiche_r

$fiche_r Top_classos 1



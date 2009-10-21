# Création de 4 fenêtres rectangulaires.                                     _
set fr1 [$fiche_bigre {Ecran 1} 320 240 $n_i_mere]
set fr2 [$fiche_bigre {Ecran 2} 320 240 $n_i_mere]
set fr3 [$fiche_bigre {Ecran 3} 320 240 $n_i_mere]
set fr4 [$fiche_bigre {Ecran 4} 320 240 $n_i_mere]

# Création d'une fenêtre sans bordure ni entete et de couleur de fond rouge  _
set coco [$fiche_bigre {coco} 320 240 $n_i_mere]
$coco Hauteur_entete 0; $coco Epaisseur_bordure 0; $coco Couleur_fond 1 0 0 1;
$coco Mettre_a_jour;

# Ajout de coco dans chacune des 4 première fenêtre                          _
$fr1 Ajouter_fils $coco
$fr2 Ajouter_fils $coco
$fr3 Ajouter_fils $coco
$fr4 Ajouter_fils $coco

# Le repère interne des fenêtre est modifié de sorte à donner l'impression   _
# d'une continuité entre ces repère internes dans le repère de leurs mère.   _
$noeud_partage Ajouter_fils $fr1;                      ; $fr1 Origine_du_contenu 0     0
$noeud_partage Ajouter_fils $fr2; $fr2 Origine 350 0   ; $fr2 Origine_du_contenu 350   0
$noeud_partage Ajouter_fils $fr3; $fr3 Origine 0 300   ; $fr3 Origine_du_contenu 0   300
$noeud_partage Ajouter_fils $fr4; $fr4 Origine 350 300 ; $fr4 Origine_du_contenu 350 300

# Illustre comment réaliser une déformation cyclique                         _
source deformations_cycliques.tcl

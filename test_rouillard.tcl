set L_fen [list]

set tab1 [$TabDouble]
$tab1 Reserver_place 8
$tab1 Ajouter 0   0 0
$tab1 Ajouter 2 200 0
$tab1 Ajouter 4 200 150
$tab1 Ajouter 6   0 150

for {set i 0} {$i<390} {incr i} {
 set    name n_
 append name $i
 set n [$poly2D]
   lappend L_fen $n
 $n Ajouter_contour 4 [$tab1 Tab]  
# $n Afficher_entete  0; $n Gerer_entete  0; $n Hauteur_entete    0
# $n Afficher_bordure 0; $n Gerer_bordure 0; $n Epaisseur_bordure 0
 $n Couleur [expr rand()] [expr rand()] [expr rand()] [expr rand()]
# $n Mettre_a_jour
# $n Fenetre_translucide 1
#$n PreRendre
# $n Ne_pas_pre_rendre 1
 $n Origine
 $noeud_partage Ajouter_fils $n
 $n Origine [expr 600*rand()+200] [expr 368*rand()+200]
 $n Rotation [expr 4.141592*rand()]
#Pivoter_fen $n 0 0.00001
}

$Noeud_scene Retirer_fils $img_fond
$Noeud_scene Retirer_fils $f_obs

set transfo          [$transfo_dyn 100000]
set rappel_rouillard [$Rappel_TCL $interp_tcl Rotate_all]
$transfo abonner_a_rappel_pendant [$rappel_rouillard Rappel]

proc Rotate_all {} {
 global L_fen
 foreach n $L_fen {
   $n Pivoter_interne 0.01 100 75
  }
}

$n_i_mere Ajouter_deformation_dynamique $transfo
$transfo Demarrer

proc etire_fond {} {}

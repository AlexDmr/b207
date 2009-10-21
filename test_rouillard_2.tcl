set L_fen [list]

set tab1 [$TabDouble]
$tab1 Reserver_place 60
set PI 3.1415926535897932384626433832795
for {set i 0} {$i<15} {incr i} {
 set angle [expr 2*$PI*$i/15]
   $tab1 Ajouter [expr 4*$i]   [expr 100*cos($angle)] [expr 100*sin($angle)]
 set angle [expr (4*$PI*$i+1)/30]
   $tab1 Ajouter [expr 4*$i+2] [expr 30*cos($angle)]  [expr 30*sin($angle)]
}

for {set i 0} {$i<200} {incr i} {
 set    name n_
 append name $i
 set n [$poly2D]
   lappend L_fen $n
 $n Ajouter_contour 30 [$tab1 Tab]
# $n Afficher_entete  0; $n Gerer_entete  0; $n Hauteur_entete    0
# $n Afficher_bordure 0; $n Gerer_bordure 0; $n Epaisseur_bordure 0
 $n Couleur [expr rand()] [expr rand()] [expr rand()] [expr rand()]
# $n Mettre_a_jour
# $n Fenetre_translucide 1
#$n PreRendre
# $n Ne_pas_pre_rendre 1
 $n Origine
 $noeud_partage Ajouter_fils $n
 $n Origine [expr 824*rand()+100] [expr 568*rand()+100]
 $n Rotation [expr 4.141592*rand()]
#Pivoter_fen $n 0 0.00001
}

$Noeud_scene Retirer_fils $f_obs

set transfo          [$transfo_dyn 100000]
set rappel_rouillard [$Rappel_TCL $interp_tcl Rotate_all]
$transfo abonner_a_rappel_pendant [$rappel_rouillard Rappel]

proc Rotate_all {} {
 global L_fen
 foreach n $L_fen {
   $n Pivoter_interne 0.01 0 0
  }
}

$n_i_mere Ajouter_deformation_dynamique $transfo
$transfo Demarrer

proc etire_fond {} {}

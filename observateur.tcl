
set f_obs [$fiche_bigre {Overview} 300 200 $n_i_mere]

set f_obs_tx 8192.0
set f_obs_ty 1536.0
set    cmd {}
append cmd "set f_tx \[$f_obs Longueur_corp\];\n"
append cmd "set f_ty \[$f_obs Hauteur_corp\] ;\n"
append cmd "set f_ex \[expr 1024.0/\$f_obs_tx\]\n"
append cmd "set f_ey \[expr 192.0/\$f_obs_ty\] \n"
append cmd "$f_obs Etirement_du_contenu \$f_ex \$f_ey\n"
append cmd "$f_obs Origine_du_contenu \[expr -(\$f_tx-1024*\$f_ex)/2.0\] \[expr -(\$f_ty-768*\$f_ey)/2.0\];\n"
append cmd "puts coco"

global interp_tcl

set r_obs [B_rappel [Interp_TCL] $cmd]
set f1 [$f_obs Fenetre]
$f1 abonner_a_dimension [$r_obs Rappel]
$f_obs Longueur_corp 256
$f_obs Hauteur_corp 192
$f_obs Origine 768 0
eval $cmd 

$f_obs Ajouter_fils $noeud_fen
$Noeud_scene Ajouter_fils_au_debut $f_obs
set f_vue_normale [$fiche_o_bigre {ZOOM} 30 15 $n_i_mere]


$f_vue_normale Nb_pixels_par_unite 0.01
$f_vue_normale Fenetre_translucide 1
$f_vue_normale Translucidite_corp 0.55
$f_vue_normale Couleur_fond 0 0 0 1
$f_vue_normale Dimension_corp 1000 500
$f_vue_normale Etirement 0.75 0.75
$f_vue_normale Hauteur_entete 300
$f_vue_normale Nb_pixels_par_unite_entete 0.125
$f_vue_normale Epaisseur_bordure 100
$f_vue_normale Translucidite_bord 0.7
#$f_vue_normale Etirement_du_contenu 0.05 0.05
$f_obs Ajouter_fils_au_debut $f_vue_normale
$f_vue_normale Mettre_a_jour

$f_obs Fenetre_translucide 1
$f_obs Translucidite_bord 0.3
$f_obs Couleur_fond 0 0 0 0
$f_obs Fond_touchable 0

$f_obs Position_des_fils_changeable 0
$f_obs Mettre_a_jour

set rep_res [$repere2D]
set rep_tmp [$repere2D]
source fonctions_observateur.tcl





set r_contient [B_rappel [Interp_TCL]]
set    cmd {}
append cmd "set p \[$r_contient Param\];\n"
append cmd "set info \[Void_vers_info \$p\];\n"
append cmd "set noeud_tmp \[\$info NOEUD\];\n"
append cmd "if \{\[string equal \[$f_obs Noeud_scene\] \$noeud_tmp\]\} \{\} else \{faire_zoom r_contient\}\n"

$r_contient Texte $cmd
$f_obs abonner_a_detection_pointeur [$r_contient Rappel] 2

return {La fiche créer est $f_obs, la fonction de rappel r_obs.}


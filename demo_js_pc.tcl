source groupe.tcl
set sds     [$n_i_mere Sim_dispo_saisi]
set f_arial [fonte_Arial]

# Desciption de l'interface à migrer
set L_cont 234; set H_cont 256;
set L_menu 150; set H_menu 256;
set js_fo   [$fiche_bigre "Home heating control system" 384 256 $n_i_mere]
  $js_fo Nom_IU "HHCS\nwindow"
set js_root [$noeud_bigre]; $js_root Nom_IU "HHCS\nroot"
  set js_menu [$fiche_bigre "MENU"    $L_menu $H_menu $n_i_mere]; $js_menu Nom_IU "Menu"
    set js_ch01 [$ztxt_bigre 130 30 25 $f_arial $sds]; $js_ch01 Editable 0; $js_ch01 Maj_texte "Living room"; $js_ch01 Nom_IU "Living R.\nlink"
    set js_ch02 [$ztxt_bigre 130 30 25 $f_arial $sds]; $js_ch02 Editable 0; $js_ch02 Maj_texte "Kitchen";     $js_ch02 Nom_IU "Kitchen\nlink"
  set js_cont [$fiche_bigre "CONTENU" $L_cont $H_cont $n_i_mere]; $js_cont Nom_IU "Content"
    set js_ct01 [$noeud_bigre]; $js_ct01 Nom_IU "Kitchen\ncontent"
      set js_img_cellar [$image_bigre "Demo_JS/cave cellier  009.jpg" 1]; $js_img_cellar Nom_IU "Kitchen\nimage"
        $js_img_cellar Etirement [expr $L_cont/[$js_img_cellar Lg]] [expr $H_cont/[$js_img_cellar Ht]]
######set js_bt   [$bouton_bigre 50 30 $n_i_mere $js_fo 2]; $js_bt Nom_IU "Bouton de\ncoloriage"
      set pot_t_cellar [$potentiometre_bigre $n_i_mere 100 16 -1 1]; $pot_t_cellar Nom_IU "Kitchen\nTemperature"
    set js_ct02 [$noeud_bigre]; $js_ct02 Nom_IU "Living R.\ncontent"
      set js_img_living [$image_bigre "Demo_JS/Salle_a_manger.jpg" 1]   ; $js_img_living Nom_IU "Living R.\nimage"
        $js_img_living Etirement [expr $L_cont/[$js_img_living Lg]] [expr $H_cont/[$js_img_living Ht]]
      set pot_t_living [$potentiometre_bigre $n_i_mere 100 16 -1 1]; $pot_t_living Nom_IU "Living R.\nTemperature"

# Ajustements
$js_fo Rendu_ecran_direct 1; $js_fo Gerer_bordure 0
$js_cont Rendu_ecran_direct 1; $js_cont Hauteur_entete 0
$js_cont Fond_touchable 0; $js_cont Epaisseur_bordure 0; $js_cont Gerer_bordure 0; $js_cont Position_des_fils_du_contenu_changeable 0; $js_cont Mettre_a_jour
$js_menu Hauteur_entete 0
$js_menu Fond_touchable 0; $js_menu Epaisseur_bordure 0; $js_menu Gerer_bordure 0; $js_menu Position_des_fils_du_contenu_changeable 0; $js_menu Mettre_a_jour
$js_menu Couleur_fond 0.3 0.3 0.3 1
$js_img_living Inverser_y 1
$js_img_cellar Inverser_y 1
$js_ct01 Position_des_fils_changeable 0
$js_ct02 Position_des_fils_changeable 0
$js_root Position_des_fils_changeable 0
$pot_t_living Origine 10 10
$pot_t_cellar Origine 10 10

$pot_t_living Valeur 50
$pot_t_living Valeur_min 0
$pot_t_living Valeur_max 256
$pot_t_living Mettre_a_jour


# Relations
$noeud_partage Ajouter_fils $js_fo
$js_fo Ajouter_fils $js_root
$js_root Ajouter_fils $js_menu
  $js_menu Ajouter_fils $js_ch01; $js_ch01 Origine 14 [expr [$js_fo Hauteur_corp]-[$js_ch01 Hauteur]-10];
  $js_menu Ajouter_fils $js_ch02; $js_ch02 Origine 14 [expr [$js_ch01 Py]-[$js_ch02 Hauteur]];
$js_root Ajouter_fils_au_debut $js_cont; $js_cont Origine $L_menu 0
  $js_cont Ajouter_fils $js_ct01
####$js_ct01 Ajouter_fils $js_bt
######$js_bt   Origine 128 128
#    $js_ct01 Ajouter_fils $pot_t_cellar; $pot_t_cellar Mettre_a_jour
    $js_ct01 Ajouter_fils $js_img_cellar;
#  $js_cont Ajouter_fils $js_ct02
    $js_ct02 Ajouter_fils $pot_t_living; $pot_t_living Mettre_a_jour
    $js_ct02 Ajouter_fils $js_img_living;


# Définition des actions si clics
 proc js_coco {p f} {
   $p Vider_fils
   $p Ajouter_fils $f}
 set js_R1 [$Rappel_TCL $interp_tcl {$js_ch01 Afficher_boites 1; $js_ch02 Afficher_boites 0; js_coco $js_cont $js_ct02}]
 set js_R2 [$Rappel_TCL $interp_tcl {$js_ch02 Afficher_boites 1; $js_ch01 Afficher_boites 0; js_coco $js_cont $js_ct01}]
 $js_ch01 abonner_a_detection_pointeur [$js_R1 Rappel] 2
 $js_ch02 abonner_a_detection_pointeur [$js_R2 Rappel] 2


# Définition de la zone de partage (invisible pour le coup)
set noeud_PDA [$noeud_bigre]
$noeud_partage Ajouter_fils $noeud_PDA; $noeud_PDA Nom_IU "PDA\nnode"
$noeud_PDA Afficher_noeud 0
$noeud_PDA Gerer_contacts 0

set rap [$Rappel_TCL $interp_tcl {$js_cont Couleur_fond [expr rand()] [expr rand()] [expr rand()] [expr rand()]}]
#$js_bt abonner_a_activation [$rap Rappel]

# Définission de l'animation de migration

proc Migrer_PC_PDA {tps n f1 f2} {
 global transfo_dyn
 global n_i_mere
 global Rappel_TCL
 global interp_tcl
 global js_cont
 global js_root
 global js_fo

 set taille_f1_init [$f1 Longueur_corp]
 set t  [$transfo_dyn $tps]
 set t2 [$transfo_dyn $tps]
 set rap_pdt [$Rappel_TCL $interp_tcl {$f1 Longueur_corp $taille;\
                                         $f1 Mettre_a_jour;\
                                       $f2 Origine [$f1 Longueur_corp] 0;\
                                         set js_fo_tx   [$js_fo   Longueur_corp]
                                         set js_cont_tx [$js_cont Longueur_corp]
                                       $f2 Etirement [expr ($js_fo_tx-$taille)/$js_cont_tx] 1}]
 set rap_ajout_distant [$Rappel_TCL $interp_tcl "set taille \[expr \[$t2 V_courant\]*$taille_f1_init\]; \
                                                 $f1 Longueur_corp \$taille; \
                                                   $f1 Mettre_a_jour;"]
 set rap_fin [$Rappel_TCL $interp_tcl "$js_root Retirer_fils $f1; \
                                       $n Ajouter_fils $f1; \
                                       $t2 Demarrer; \
                                       $n_i_mere Ajouter_deformation_dynamique $t2; \
                                       $t2 abonner_a_rappel_pendant [$rap_ajout_distant Rappel]; \
                                       Maj_vg \$noeud_partage"]

 set    cmd "set f1 $f1; set f2 $f2; set taille \[expr (1-\[$t V_courant\])*$taille_f1_init\]; "
 append cmd [$rap_pdt Texte]

 $rap_pdt Texte $cmd
 $t abonner_a_rappel_pendant [$rap_pdt Rappel]
 $t abonner_a_rappel_fin     [$rap_fin Rappel]
 $t Demarrer
 $n_i_mere Ajouter_deformation_dynamique $t
}

$f_obs Couleur_fond 0 0 0 0.7

## Partie visualisation du graphe de scène
#source visu_graph.tcl
#  visu_graph v $Noeud_scene
#  v Relier 10
#  $noeud_partage Ajouter_fils [v Noeud_poly]
#  set rep [v Recalculer 5]
## Placement
#  set noeud_v [v Noeud_poly]
#  $noeud_v Origine 512 735

source graphe_meta_ui.tcl

proc etire_fond {a b c d e f} {}
#load tk84.dll
#cd ../COMETS
return "Maj_vg \$noeud_partage\nMigrer_PC_PDA 2000 \$noeud_PDA \$js_menu \$js_cont\n\$n_i_mere Creer_groupe 1 JS_groupe \$noeud_PDA 1 0\nMaj_vg \$noeud_partage\nsource set_of_comets.tcl"




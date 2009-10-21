source model_IU.tcl
  puts "Création de la structure"
  model_IU MIU $Noeud_scene
  MIU Nouveau_marqueur
  set vg            [MIU Racine_vg]
  puts "Maj"
  MIU Maj           $vg
  puts "Mise en forme"
  MIU Mise_en_forme $vg
  puts "Ajout au graph de bigre"
  set n_b           [$vg Noeud_poly]
  $noeud_partage Ajouter_fils $n_b
  $n_b Origine 512 743
 
# Pouvoir déplacer la racine
 set ec_vg [$sim_ctc Ajouter_ensemble_contact]
 set mp_vg [$n_b MP]
 set mc_vg [$sim_ctc Ajouter_modele $ec_vg $mp_vg $n_b 5]


# Interaction avec le graphe de scène
# Gestion d'une palette à la toolglasses
set f_tools_js [$fiche_bigre "ToolGlass" 120 240 $n_i_mere]
$noeud_partage Ajouter_fils $f_tools_js
$f_tools_js Couleur_fond 0 0 1 1
$f_tools_js Fenetre_translucide 1
$f_tools_js Empiler_contenant 1

set poly_vider [B_polygone]
$poly_vider Ajouter_contour [ProcOvale 0 0 60 60 60]
$poly_vider Origine 60 60

$poly_vider Translucidite 0
$poly_vider Couleur 1 0 0 0.5
$poly_vider Empiler_contenant 1
$poly_vider Nom_IU "Tool\nCUT"
$f_tools_js Ajouter_fils $poly_vider
$f_tools_js Nom_IU "Tool\nglass"

set poly_get_name [B_polygone]
  $poly_get_name Ajouter_contour [ProcRect 0 0 120 90]; $poly_get_name Origine 0 120
  $poly_get_name Translucidite 0; $poly_get_name Couleur 0 1 0 0.5
  $poly_get_name Empiler_contenant 1; $poly_get_name Nom_IU "Get\nnode name"
  $f_tools_js Ajouter_fils $poly_get_name
set rappel_get_name [B_rappel $interp_tcl]
  set cmd "set vgf \[Get_node_name $rappel_get_name $f_tools_js\]\n\
           set node \[eval Real_class \[\$vgf Noeud_racine\]\]\n\
           $editeur_tcl Ecrire \$node\n\
           $editeur_tcl Ne_pas_pre_rendre 0\n\
          "
  $rappel_get_name Texte $cmd
  $poly_get_name   abonner_a_detection_pointeur [$rappel_get_name Rappel] 2

proc Get_node_name {rap node_to_avoid} {
 set p          [$rap Param]
 set inf        [Void_vers_info $p]
 set L_empilees [$inf L_Reponses_cumulees]
 $L_empilees Init_parcours
 while {[$L_empilees Parcours_continue]} {
   set info  [$L_empilees Courant_dans_parcours]
   set noeud [$info NOEUD]
   if {[string equal $noeud $node_to_avoid]} {$L_empilees Suivant_dans_parcours; continue}
   set vgf_tmp [MIU Noeud_vg_representant $noeud]
   if {[string equal $vgf_tmp {}]} {
     set vgf_tmp [MIU Noeud_vg_represente_par $noeud]
     return $vgf_tmp
    } else {return $vgf_tmp}
   
   if {[string equal $vgf_tmp {}]} {} else {return [$vgf_tmp ]}

   $L_empilees Suivant_dans_parcours
  }

 return {}
}

set rappel_vider [$Rappel_TCL $interp_tcl]

set    cmd ""
append cmd "puts coucou\n"
append cmd "set p \[$rappel_vider Param\];\n"
append cmd "set info \[Void_vers_info \$p\];\n"
append cmd "set L_empilees \[\$info L_Reponses_cumulees\];\n"
append cmd "\$L_empilees Init_parcours;\n"
append cmd "while \{\[\$L_empilees Parcours_continue\]\} \{\n"
append cmd "  set info \[\$L_empilees Courant_dans_parcours\];\n"
append cmd "  set noeud \[\$info NOEUD\];\n"
append cmd "  if \{\[string equal \$noeud $f_tools_js\]\} \{\$L_empilees Suivant_dans_parcours; continue\}\n"
# On regarde si on a cliqué sur un des noeuds représenté
append cmd "  puts \"On essai sur \$noeud\"\n"
append cmd "  set vgf_tmp \[MIU Noeud_vg_representant \$noeud]\n"
# On regarde si on a cliqué que un représentant d'un noeud
append cmd "  if \{\[string equal \$vgf_tmp \{\}\]\} \{set vgf_tmp \[MIU Noeud_vg_represente_par \$noeud]\}\n"
append cmd "  puts \"  rep_bigre = \$vgf_tmp\"\n"
append cmd "  if \{\[string equal \$vgf_tmp \{\}\]\} \{\} else \{\n"
append cmd "    set L_vg_pere \[\$vgf_tmp get_L_peres\]\n"
append cmd "    foreach papa \$L_vg_pere \{puts \"  \$vgf_tmp Vider_peres_vg \$papa\"; \$vgf_tmp Vider_peres_vg \$papa\} \n"
append cmd "    Maj_vg $noeud_partage \n"
append cmd "   \}\n"

append cmd "  \$L_empilees Suivant_dans_parcours\}\n"
append cmd ""

set cmd "set vgf_tmp \[Get_node_name $rappel_vider $f_tools_js\]\n\
         if \{\[string equal \$vgf_tmp \{\}\]\} \{\} else \{\n\
           set L_vg_pere \[\$vgf_tmp get_L_peres\]\n\
           foreach papa \$L_vg_pere \{puts \"  \$vgf_tmp Vider_peres_vg \$papa\"; \$vgf_tmp Vider_peres_vg \$papa\}\n\
           Maj_vg $noeud_partage\n\
          \}\n\
        "
$rappel_vider Texte $cmd
$poly_vider abonner_a_detection_pointeur [$rappel_vider Rappel] 2


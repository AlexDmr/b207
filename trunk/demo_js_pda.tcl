source groupe.tcl

# Desciption de l'interface à migrer
set js_f [$fiche_bigre "A PDA" 384 512 $n_i_mere]
$noeud_partage Ajouter_fils $js_f

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

# Définition de la zone de partage (invisible pour le coup)
return {$n_i_mere Se_rajouter_a 1 [$js_f Noeud_repere_fils] 1 0}



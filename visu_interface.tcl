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
  $noeud_fen Ajouter_fils $n_b
  $n_b Origine 512 743

return {Maj_vg $noeud_fen}

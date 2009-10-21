inherit SWL_menu Object_gfx

method SWL_menu constructor {} {
 set tab1 [ProcRect 0 0 400 400]

 set poly [B_polygone] 
  set this(root) $poly
  $poly Ajouter_contour $tab1
  $poly Origine 300 150
  $poly Couleur 0.1 0.1 0.1 0.7

 set this(bt_entrainement) "${objName}_entrainement"
 B_bouton_he_shan $this(bt_entrainement) 30 Entrainement
  $this(bt_entrainement) Accrocher_a $poly
  $this(bt_entrainement) Position 200 250
  $this(bt_entrainement) Centrer

 set this(bt_dual) "${objName}_Dual"
 B_bouton_he_shan $this(bt_dual) 30 Dual
  $this(bt_dual) Accrocher_a $poly
  $this(bt_dual) Position 200 200
  $this(bt_dual) Centrer

 set this(bt_aide) "${objName}_Aide"
 B_bouton_he_shan $this(bt_aide) 30 Aider
  $this(bt_aide) Accrocher_a $poly
  $this(bt_aide) Position 200 150
  $this(bt_aide) Centrer

 set this(bt_quit) "${objName}_quit"
 B_bouton_he_shan $this(bt_quit) 30 Quitter "exit"
  $this(bt_quit) Accrocher_a $poly
  $this(bt_quit) Position 200 100
  $this(bt_quit) Centrer
 
 return $objName
}

Generate_accessors SWL_menu [list bt_entrainement bt_dual bt_aide bt_quit]


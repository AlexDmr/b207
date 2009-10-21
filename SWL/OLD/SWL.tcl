source Object_gfx.tcl
source Joueur.tcl
source Planete.tcl
source Missille.tcl
source B_bouton_he_shan.tcl
source SWL_menu.tcl
source ../B_Panel.tcl
source ../B_ScrollList.tcl

#_________________________________________________________________________________________________________
#_________________________________________________________________________________________________________
#_________________________________________________________________________________________________________
inherit SWL Object_gfx

#_________________________________________________________________________________________________________
method SWL constructor {{X 1024} {Y 768}} {
 this inherited
 
# Partie fonctionnelle
 set this(L_joueurs)   [list ]
 set this(L_planetes)  [list ]
 set this(L_missiles)  [list ]
 
 set this(G)           1
 set this(id)          0

# Gestion du menu
 set this(menu) [SWL_menu ${objName}_menu_general]
   [$this(menu) get_bt_entrainement] Subscribe "$objName set_mode terrain"
   [$this(menu) get_bt_dual]         Subscribe "$objName set_mode terrain"
   
# Partie graphique
 set this(root)  [B_noeud]
   $this(root) Ajouter_fils [$this(menu) get_root]
   set this(terrain_jeu) [B_noeud]
   B_configure $this(terrain_jeu) -Position_des_fils_changeable 0
   $this(root) Ajouter_fils $this(terrain_jeu)
   set this(panel) [B_polygone]
     $this(terrain_jeu) Ajouter_fils $this(panel)
	 set img_bt_left  [B_image ../BDec.bmp]
     set img_bt_right [B_image ../BInc.bmp]
	 set contour [ProcRect 0 0 250 $Y]; 
	   B_configure $this(panel) -Ajouter_contour $contour \
	                            -Origine 0 0 \
								-Couleur 0.3 0.3 0.3 1
	 Detruire $contour
	 set this(B_panel) [Panel ${objName}_panel $this(terrain_jeu) $this(panel) [list $img_bt_right $img_bt_left]]
	 set this(ScrollList_Joueurs) [B_ScrollList ${objName}_sl_jr]
	   $this(panel) Ajouter_fils [$this(ScrollList_Joueurs) get_root]
	   $this(ScrollList_Joueurs) Resize 250 [expr $Y / 2.0 - 20]
	   [$this(ScrollList_Joueurs) get_root] Origine 0 [expr $Y / 2.0 + 20]
	 
   set this(space) [B_polygone]
     $this(terrain_jeu) Ajouter_fils $this(space)
	 set contour [ProcRect 0 0 $X $Y]; 
	   B_configure $this(space) -Ajouter_contour $contour \
	                            -Origine 0 0 \
								-Couleur 0 0 0 1
       # Configuration of the drop zone
	   Drop_zones A [list [list $this(space) "$objName Is_a_dragged_class \$n" "$objName Add_new_instance" "$this(space) Couleur 0.2 0.4 0.2 1" "$this(space) Couleur 0 0 0 1"]
	                ]

	 Detruire $contour

# Partie liée à la simulation et l'affichage des trajectoires
 set this(simulation_dt) 1
 set this(rap_Calculer_deplacements) [B_rappel [Interp_TCL] "$objName Calculer_deplacements \[$objName get_simulation_dt\]"]	 
 
# Definition des modes
 this set_mode menu

# Fin
 return $objName
}

#_________________________________________________________________________________________________________
method SWL dispose {} {
 foreach j $this(L_joueurs)  {$j dispose}
 foreach p $this(L_planetes) {$p dispose}
 Detruire $this(root)
 Detruire $this(panel)
 Detruire $this(space)
 
 this inherited
}

Generate_accessors     SWL [list mode G simulation_dt panel space]

#_________________________________________________________________________________________________________
#____________________________________________ Drag&Drop management _______________________________________
#_________________________________________________________________________________________________________
method SWL Is_a_dragged_class {n} {
 puts "$objName Is_a_dragged_class $n"
 return [expr "\"[$n Val_MetaData CLASSE]\" != \"\""]
}

#_________________________________________________________________________________________________________
method SWL Add_new_instance {drop_zone infos} {
# On récupère le pointeur 
 set ptr   [$infos Ptr]
# On récupère le noeud de placement de l'élément en cours de drag
 set n_plc [$ptr Val_MetaData Noeud_placement_drag]

 puts "BRAVO!!! Drop $infos on $drop_zone"
 set x            [$infos X_au_contact]
 set y            [$infos Y_au_contact]
# On peut récupérer le noeud en cours de drag (pour obtenir des informations sur lui par exemple)
 set dragged_node [Get_node_dragged_in $infos]
 puts "dragged_node : $dragged_node"
 
# Ici, on se contente d'ajouter un polygone de forme identique sur la zone de drop, à l'endroit précis du drop.
 set new_elements [this get_a_name]
 set classe       [$dragged_node Val_MetaData CLASSE]
 switch $classe {
   Vaisseau {set joueur [$dragged_node Val_MetaData Joueur]
             Vaisseau $new_elements [expr $x + [$n_plc Px]] [expr $y + [$n_plc Py]] [$dragged_node Val_MetaData Rayon] [$joueur get_color]
             $joueur Add_L_vaisseaux $new_elements
            }
   Planete  {
            }
  }
}

#_________________________________________________________________________________________________________
#_________________________________________________________________________________________________________
#_________________________________________________________________________________________________________
method SWL set_mode {mode} {
 set this(mode) $mode
 switch $mode {
     menu  {B_configure [$this(menu) get_root] -Afficher_noeud 1 -Gerer_contacts 1
	        B_configure $this(terrain_jeu)     -Afficher_noeud 0 -Gerer_contacts 0
           }
   terrain {B_configure [$this(menu) get_root] -Afficher_noeud 0 -Gerer_contacts 0
	        B_configure $this(terrain_jeu)     -Afficher_noeud 1 -Gerer_contacts 1
           }
   edition {
           }
       jeu {
	       }
  }
}



#_________________________________________________________________________________________________________
#_________________________________________________________________________________________________________
#_________________________________________________________________________________________________________
method SWL get_a_name {{prefixe "object"}} {
 incr this(id)
 return ${objName}_${prefixe}_$this(id)
}

#_________________________________________________________________________________________________________
Generate_List_accessor SWL L_joueurs  L_joueurs
Generate_List_accessor SWL L_planetes L_planetes

#_________________________________________________________________________________________________________
method SWL Add_L_joueurs {LJ} {
 Add_list this(L_joueurs) $LJ
 foreach J $LJ {
   $J set_root_for_vaisseaux $this(space)
   $this(ScrollList_Joueurs) Add_elements [$J get_root]
   Drag_nodes [$J get_poly_vaisseau] 1 "$this(B_panel) toggle_extension" "$this(B_panel) toggle_extension"
  }
}

#_________________________________________________________________________________________________________
#_________________________________________________________________________________________________________
#_________________________________________________________________________________________________________
method SWL Add_L_planetes {LP} {
 Add_list this(L_planetes) $LP
 foreach P $LP {
   $this(space) Ajouter_fils_au_debut [$P get_root]
  }
}

#_________________________________________________________________________________________________________
method SWL Sub_L_planetes {LP} {
 Sub_list this(L_planetes) $LP
 foreach P $LP {
   $this(space) Retirer_fils [$P get_root]
  }
}

#_________________________________________________________________________________________________________
method SWL Add_L_missiles {LM} {
 foreach M $LM {
   if {[Add_list this(L_missiles) $M]} {
     $this(space) Ajouter_fils_au_debut [$M get_root]
	}
  }
}

#_________________________________________________________________________________________________________
method SWL Sub_L_missiles {LM} {
 foreach M $LM {
   Sub_element this(L_missiles) $M; $M dispose
  }
}

#_________________________________________________________________________________________________________
#_________________________________________________________________________________________________________
#_________________________________________________________________________________________________________
method SWL Tirer {} {
# On efface les missilles déja présents
 foreach M $this(L_missiles) {$M dispose}
 set this(L_missiles) [list ]

# On s'abonne à B207 pour effectuer les calculs
 N_i_mere abonner_a_fin_simulation [$this(rap_Calculer_deplacements) Rappel]
 
# Pour chaque vaisseau de chaque joueur, on initialise un missille
 foreach j $this(L_joueurs) {
   foreach vaisseau [$j get_L_vaisseaux] {
     set rayon      [$vaisseau get_rayon]
	 set angle      [$vaisseau get_angle]
	 set puissance  [$vaisseau get_puissance]
     set vaisseau_x [$vaisseau get_x]
	 set vaisseau_y [$vaisseau get_y]
	 
	 set x          [expr $vaisseau_x + cos($angle)*1.01*$rayon]
	 set y          [expr $vaisseau_y + sin($angle)*1.01*$rayon]
	 set vx         [expr cos($angle)*$puissance]
	 set vy         [expr sin($angle)*$puissance]
	 
	 this Add_L_missiles [Missille [this get_a_name Missille] $x $y $vx $vy]
    }
  }
}

#_________________________________________________________________________________________________________
method SWL Calculer_deplacements {dt} {
 #puts "$objName Calculer_deplacements $dt"
 set LM $this(L_missiles)
 if {$LM == ""} {
   puts "Plus de missilles!"
   N_i_mere desabonner_de_fin_simulation [$this(rap_Calculer_deplacements) Rappel]
   puts "  Fin de l'abonnement"
  }
  
 foreach M $LM {
   #puts "Missille $M\n  - P : <[$M get_x] ; [$M get_y]>\n  - V : <[$M get_vx] ; [$M get_vy]>"
   set ax 0
   set ay 0
   foreach P $this(L_planetes) {
     set dx [expr [$P get_x]-[$M get_x]]
	 set dy [expr [$P get_y]-[$M get_y]]
	 set d  [expr sqrt($dx*$dx + $dy*$dy)]
	 if {$d < [$P get_rayon]} {
	   this Sub_L_missiles $M
	   break
	  }
	 
	 set ux [expr $dx / $d]
	 set uy [expr $dy / $d]
	 
	 set ax [expr $ax + $ux * $this(G) * [$P get_masse] / ($d*$d)]
	 set ay [expr $ay + $uy * $this(G) * [$P get_masse] / ($d*$d)]
    }
   $M set_vx [expr [$M get_vx] + $dt*$ax]
   $M set_vy [expr [$M get_vy] + $dt*$ay]
   
   $M set_x  [expr [$M get_x] + [$M get_vx]*$dt]
   $M set_y  [expr [$M get_y] + [$M get_vy]*$dt]
   
  # Calcul des collisions avec les vaisseaux
   foreach J $this(L_joueurs) {
     set LV [$J get_L_vaisseaux]
     foreach V $LV {
	   set dx [expr [$M get_x] - [$V get_x]]
	   set dy [expr [$M get_y] - [$V get_y]]
	   set d  [expr sqrt($dx*$dx + $dy*$dy)]
	   if {$d < [$V get_rayon]} {
	     $J   Sub_L_vaisseaux $V
		 this Sub_L_missiles  $M
	    }
	  }
    }
  }
}


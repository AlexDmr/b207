load Speech_api.dll

#_____________________________________________________________________________________________________
#_____________________________________________________________________________________________________
#_____________________________________________________________________________________________________
method Expe_menu_main constructor {noeud_locker_menu menu LL_items} {
# Infos for the experiment
 set this(init_block)           0
 set this(num_block)            0
 set this(nb_blocks)            30
 set this(num_trial)            0
 set this(nb_trials_done)       -1
 set this(nb_trials_per_blocks) 8
 set this(current_target)       ""
 set this(noeud_locker_menu)    $noeud_locker_menu
 
 set this(ordre) "X 6 2 0 7 1 3 5 4"
 set this(size_latin_square) 8
 set this(latin_square) [list "1 2 8 3 7 4 6 5" \
                              "2 3 1 4 8 5 7 6" \
                              "3 4 2 5 1 6 8 7" \
                              "4 5 3 6 2 7 1 8" \
                              "5 6 4 7 3 8 2 1" \
                              "6 7 5 8 4 1 3 2" \
                              "7 8 6 1 5 2 4 3" \
                              "8 1 7 2 6 3 5 4" \
                        ]
 set this(L_colors) [list "1 0 0 1" \
                          "1 0 0 1" \
					      "1 0 0 1" \
					      "0 1 0 1" \
					      "0 1 0 1" \
					      "0 1 0 1" \
					      "0 0 1 1" \
					      "0 0 1 1" \
				    ]
 $menu set_L_colors $this(L_colors)
 $menu Reload
 
 set this(LL_items) $LL_items
 
# B207 nodes
 set size 100
 set this(n_root)        [B_noeud]
   $this(n_root) Afficher_noeud 0; $this(n_root) Gerer_contacts 0
   set this(rap_display_UI) [B_rappel [Interp_TCL] "$objName Display_or_not_UI"]
   $this(n_root) abonner_a_LR_parcours [$this(n_root) LR_Av_aff] [$this(rap_display_UI) Rappel]
 set this(n_target)      [B_noeud]
   set this(n_target_poly) [B_polygone]; 
     $this(n_target_poly) Ajouter_contour [ProcRect 0 0 $size $size]
	 $this(n_target)      Ajouter_fils    $this(n_target_poly)
   set this(n_target_name) [B_texte 100 100 $size [fonte_Arial] [B_sim_sds]]; 
     $this(n_target_name) TEXTE NOTHING 
     $this(n_target_name) Editable 0
     $this(n_target)      Ajouter_fils $this(n_target_name)
 set this(txt_feedback)  [B_texte 100 100 $size [fonte_Arial] [B_sim_sds]]
   $this(n_root) Ajouter_fils $this(txt_feedback)
   $this(txt_feedback) Editable 0
 set this(bt_next)       [B_bouton_texte "NEXT" $size $this(n_root)] 
   [$this(bt_next) Noeud_fen] Couleur_fond 0.15 0.25 0.4 1; $this(bt_next) Mettre_a_jour
   set this(rap_bt_next) [B_rappel [Interp_TCL] "if {\[catch {$objName Next} err\]} {puts \"ERROR for next command :\n\$err\"}"]
   $this(bt_next) abonner_a_activation [$this(rap_bt_next) Rappel]
   $this(bt_next) Calculer_boites; 
   set b [$this(bt_next) Boite_noeud]
   $this(bt_next) Origine [expr [N_i_mere Largeur] - 280] 0

 B_configure $this(n_root) -Ajouter_fils $this(n_target)
}

#_____________________________________________________________________________________________________
Generate_accessors Expe_menu_main [list txt_feedback nb_trials_done init_block current_target nb_trials num_trial nb_blocks num_block L_colors LL_items n_root n_target_name bt_next]

#_____________________________________________________________________________________________________
method Expe_menu_main Display_or_not_UI {} {
 set rep [expr ![$this(noeud_locker_menu) Gerer_contacts] && [this get_the_unique_trial_id] == $this(nb_trials_done)]
 $this(bt_next)  Afficher_noeud $rep        ; $this(bt_next) Gerer_contacts $rep
 set rep [expr [this get_the_unique_trial_id] > $this(nb_trials_done)]
 $this(n_target) Afficher_noeud $rep; $this(n_target) Gerer_contacts $rep
}

#_____________________________________________________________________________________________________
method Expe_menu_main get_the_unique_trial_id {} {return  [expr $this(num_block)*$this(nb_trials_per_blocks) + $this(num_trial)]}

#_____________________________________________________________________________________________________
method Expe_menu_main set_current_target {name num_block num_trial} {
 # NOTHING, just for tracer...
}

#_____________________________________________________________________________________________________
method Expe_menu_main get_latin_num_for {nb} {
 set line_for_slice [expr ($nb / $this(size_latin_square)) % $this(size_latin_square)]
 set  col_for_slice [expr $nb % $this(size_latin_square)]
 set  num_for_slice [lindex [lindex $this(latin_square) $line_for_slice] $col_for_slice]
 set  num_for_slice [lindex $this(ordre) $num_for_slice]
 
 return $num_for_slice
}

#_____________________________________________________________________________________________________
method Expe_menu_main Update_display {} {
 set nb [this get_the_unique_trial_id]
 set num_for_slice [this get_latin_num_for $nb]
 set num_for_item  [this get_latin_num_for [expr $nb+1]]
 
 set this(current_target) [lindex [lindex $this(LL_items) $num_for_slice] $num_for_item]

 $this(n_target_name) TEXTE $this(current_target); $this(n_target_name) Optimiser_dimensions; $this(n_target_name) Calculer_boites
 set boite [$this(n_target_name) Boite_noeud]
 $this(n_target) Py [expr [N_i_mere Hauteur] - [$boite Ty]]
 $this(n_target_poly) Px [expr [$boite Tx]+10]; 
 set num [expr $num_for_slice * 8 + $num_for_item]
 eval "$this(n_target_poly) Couleur [lindex $this(L_colors) [expr (($num / 8)-1)%8]]"
 
 $this(txt_feedback) Optimiser_dimensions; $this(txt_feedback) Calculer_boites; 
 set b [$this(txt_feedback) Boite_noeud]
 $this(txt_feedback) Origine [expr [N_i_mere Largeur] - [$b Tx]] [expr [N_i_mere Hauteur] - [$b Ty]]
}

#_____________________________________________________________________________________________________
method Expe_menu_main Feedback {txt} {
 Say $txt
 $this(txt_feedback) TEXTE $txt
 this Update_display
}

#_____________________________________________________________________________________________________
method Expe_menu_main Propose {name} {
 if {$name == [this get_current_target]} {this Feedback "Good!"} else {this Feedback "Wrong!"}
 if {$this(nb_trials_done) < [this get_the_unique_trial_id]} {
   incr this(nb_trials_done)
  }
}

#_____________________________________________________________________________________________________
method Expe_menu_main Start {} {
 set this(num_block)      0
 set this(num_trial)      0
 set this(nb_trials_done) -1
 
 $this(txt_feedback) TEXTE ""
 this Update_display
 $this(n_root) Afficher_noeud 1; $this(n_root) Gerer_contacts 1
 this set_current_target $this(current_target) $this(num_block) $this(num_trial)
}

#_____________________________________________________________________________________________________
method Expe_menu_main Next {} {
 incr this(num_trial)
 if {$this(num_trial) >= $this(nb_trials_per_blocks)} {
   set  this(num_trial) 0
   incr this(num_block) 
   if {$this(num_block) > $this(nb_blocks)} {this Finish}
  }
 $this(txt_feedback) TEXTE ""
 
 this Update_display
 this set_current_target $this(current_target) $this(num_block) $this(num_trial)
}

#_____________________________________________________________________________________________________
method Expe_menu_main Finish {} {
 $this(n_root) Afficher_noeud 0; $this(n_root) Gerer_contacts 0; 
}

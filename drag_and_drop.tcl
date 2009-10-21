#_______________________________________________________________________________
# Drag and Drop sous B207                                                      _
#  Principe :                                                                  _
#  Chaque noeud déplaçable est marqué (soit en MetaData, soit dans le contexte)_
#  Chaque noeud déplaçable s'abonne à la détection d'un appuie                 _
#  Une fois qu'on a appuyé, 2 solutions (à paramétrer)                         _
#    - on détache le noeud et on l'attache au bon endroit sous le pointeur     _
#    - on laisse le noeud et on l'attache aussi sous le pointeur               _
#  Dans tous les cas, placer dans les MetaData du pointeur le noeud déplacé    _
#                                 ____                                         _
#  Chaque noeud récepteur s'abonne à la détection d'un relachement de pointeur _
#  Il vérifie si le pointeur déplace un noeud (regarde dans MetaData)          _
#  et examine une condition sur ce noeud (ex : c'est un noeud image...)        _
#  Eventuellement, l'abonnement peut même se faire sur l'entré et la sortie    _
#    ex : pour permettre des animations montrant que l'objet peut être déposé  _
#  A charge à la zone d'accueil d'intégrer ensuite le noeud...                 _
#                                 ____                                         _
#  Bien prévoir le cas où on relache dans une zone qui n'accepte pas le noeud  _
#  Dans ce cas, mécanisme qui va "nettoyer" le pointeur                        _
#    - Enlever le noeud (éventuellement avec anim de retour                    _
#    - Mise à 0 des MetaData correspondantes                                   _
#  COMMENT?                                                                    _
#    - Utilisation d'un B_Fire_Forget_after_simulations                        _
#                                 ____                                         _
#  CE QUI SERAIT QUAND MÊME BIEN...                                            _
#  Pouvoir définir les noeuds déplaçables en intention (ex : les fils de...)   _
#    de sorte à ne pas trop de soucier des mises à jour (ajout de noeud)       _
#    => Utilisation du contexte généralisé? Mais comment?                      _
#    => Utilisation de plusieurs fonctions d'abonnement au déplacement         _
#         - Liste de noeuds à abonner                                          _
#         - Liste de pères dont une méthode de rappel se chargera des fils     _
#           Si détection pointeur et que c'est un fils alors...                _
#_______________________________________________________________________________
#_______________________________________________________________________________
#_______________________________________________________________________________
proc Drag_this_node {n infos {detach 0}} {
 set ptr [$infos Ptr]
   if {![string equal [$ptr Val_MetaData Dragging] ""]} {return}
 
 set n_ptr [$ptr Noeud_representation]
 set rap_desactivate_dep_Aff [$n_ptr Val_MetaData rap_desactivate_dep_Aff]; if {[string equal $rap_desactivate_dep_Aff {}]} {set rap_desactivate_dep_Aff [B_rappel [Interp_TCL]]; $n_ptr Ajouter_MetaData_T rap_desactivate_dep_Aff $rap_desactivate_dep_Aff}
 set rap_activate_dep_Aff    [$n_ptr Val_MetaData rap_activate_dep_Aff];    if {[string equal $rap_activate_dep_Aff    {}]} {set rap_activate_dep_Aff    [B_rappel [Interp_TCL]]; $n_ptr Ajouter_MetaData_T rap_activate_dep_Aff    $rap_activate_dep_Aff}
 set rap_desactivate_dep_Pre [$n_ptr Val_MetaData rap_desactivate_dep_Pre]; if {[string equal $rap_desactivate_dep_Pre {}]} {set rap_desactivate_dep_Pre [B_rappel [Interp_TCL]]; $n_ptr Ajouter_MetaData_T rap_desactivate_dep_Pre $rap_desactivate_dep_Pre}
 set rap_activate_dep_Pre    [$n_ptr Val_MetaData rap_activate_dep_Pre];    if {[string equal $rap_activate_dep_Pre    {}]} {set rap_activate_dep_Pre    [B_rappel [Interp_TCL]]; $n_ptr Ajouter_MetaData_T rap_activate_dep_Pre    $rap_activate_dep_Pre}
 set rap_fin_drag        [$n_ptr Val_MetaData rap_fin_drag];        if {[string equal $rap_fin_drag        {}]} {set rap_fin_drag        [B_rappel [Interp_TCL]]; $n_ptr Ajouter_MetaData_T rap_fin_drag        $rap_fin_drag}
# Détacher le noeud?
 if {$detach} {$n Vider_peres}
# Attacher (au bon endroit de préférence)
# Intercaller un noeud de placement qui va rendre inopérant le placement de $n
 set n_placement [$n_ptr Val_MetaData Noeud_placement_drag]
 if {[string equal $n_placement {}]} {
   set n_placement [B_noeud]
   $n_ptr Ajouter_fils $n_placement
   $n_ptr Ajouter_MetaData_T Noeud_placement_drag $n_placement
   $ptr   Add_MetaData       Noeud_placement_drag $n_placement
   $n_placement abonner_a_LR_parcours [$n_placement LR_Av_aff]       [$rap_desactivate_dep_Aff Rappel]
   $n_placement abonner_a_LR_parcours [$n_placement LR_Ap_aff]       [$rap_activate_dep_Aff    Rappel]
   $n_placement abonner_a_LR_parcours [$n_placement LR_Av_pre_rendu] [$rap_desactivate_dep_Pre Rappel]
   $n_placement abonner_a_LR_parcours [$n_placement LR_Ap_pre_rendu] [$rap_activate_dep_Pre    Rappel]
  }
   
 $ptr Add_MetaData DRAG_ENDED 0
 [$ptr Val_MetaData Noeud_placement_drag] Afficher_noeud 1
 set L_new_nodes [$ptr Val_MetaData L_new_nodes]
 foreach new_node $L_new_nodes {Detruire $new_node}
 
 $n_ptr Noeud_puis_fils 0
 $rap_desactivate_dep_Pre Texte "$n_placement Ajouter_MetaData_T drag_rep_actif_Pre \[$n Prendre_en_compte_repere\]; $n Prendre_en_compte_repere 0"
 $rap_activate_dep_Pre    Texte "$n Prendre_en_compte_repere \[$n_placement Val_MetaData drag_rep_actif_Pre\];"
 $rap_desactivate_dep_Aff Texte "$n_placement Ajouter_MetaData_T drag_rep_actif_Aff \[$n Prendre_en_compte_repere\]; $n Prendre_en_compte_repere 0"
 $rap_activate_dep_Aff    Texte "$n Prendre_en_compte_repere \[$n_placement Val_MetaData drag_rep_actif_Aff\];"
 # XXX DEBUG 2009 02 16 : $n_placement Ajouter_fils $n
# ANNULER PLACEMENT DE $N
 $n_placement Noeud_touchable 0
 $n_placement Origine [expr -[$ptr X]] [expr -[$ptr Y]]

 B_configure $n_placement -Mode_texture 1        \
                          -Mode_texture_fils 1   \
						  -Couleur_masque_texture 1 1 1 0.5 \
						  -Afficher_boites 0
 
 
 set L_reperes [$infos L_REPERES]
 set L_new_nodes [list]
 set it [$L_reperes Init_parcours]
 while {[$L_reperes Parcours_continue $it]} {
   set repere [$L_reperes Courant_dans_parcours $it]; set noeud_du_repere [Real_class [Repere_vers_noeud $repere]]
   if {$noeud_du_repere == $n} {break}
   set new_node [B_noeud]; $new_node maj_repere $repere; lappend L_new_nodes $new_node
   set it [$L_reperes Suivant_dans_parcours $it]
  }
 $L_reperes Terminer_parcours $it
 set new_node [B_noeud]; $new_node maj_noeud $n; lappend L_new_nodes $new_node; $n_placement Ajouter_MetaData_T Original_node_position $new_node
 $ptr Add_MetaData L_new_nodes $L_new_nodes
 
 set prev_node $n_placement
 #puts "#L_new_nodes = [llength $L_new_nodes]"
 foreach new_node $L_new_nodes {
   $prev_node Ajouter_fils $new_node
   if {[$prev_node Est_entite]} {
     $prev_node Prendre_en_compte_repere 0
    } else {$prev_node Prendre_en_compte_repere 1
	       }
   set prev_node $new_node
  }
 $prev_node Ajouter_fils $n
   if {[$prev_node Est_entite]} {
     $prev_node Prendre_en_compte_repere 0
    } else {$prev_node Prendre_en_compte_repere 1
	       }
 
 $n_placement Prendre_en_compte_repere 1; 
 $n_placement Ajouter_MetaData_T X_au_contact [$infos X_au_contact]
 $n_placement Ajouter_MetaData_T Y_au_contact [$infos Y_au_contact]
# Marquer le pointeur
 $n_ptr Ajouter_MetaData_T Dragging $n
 $ptr   Add_MetaData       Dragging $n

# S'abonner au relachement pour tout remettre en ordre
 $ptr abonner_a_changement [$rap_fin_drag Rappel]
 $rap_fin_drag Texte "if {\[catch \"Fin_Drag_for_ptr $ptr $n_ptr $n_placement $rap_fin_drag $rap_desactivate_dep_Pre $rap_activate_dep_Pre\" err\]} {puts \"ERROR in Fin_Drag_for_ptr:\n \$err\"}"
 
# Déclancher le rappel de début de drag
 if {[catch "eval \[$n Val_MetaData cmd_start\]" err]} {
   puts "ERROR in Drag_this_node command start:\n  - cmd_start : [$n Val_MetaData cmd_start]\n  - ERROR : $err"
  }
}

#_______________________________________________________________________________
proc Fin_Drag_for_ptr {ptr n_ptr n_placement rap_fin_drag rap_desactivate_dep rap_activate_dep}  {
 if {[$ptr Appuie] == 0 && [$ptr Val_MetaData Drop_done] == ""} {
   $ptr Add_MetaData Drop_done 1
   set n_plc [$ptr Val_MetaData Noeud_placement_drag]
   if {$n_plc != ""} {$n_plc Afficher_noeud 0}
   $rap_desactivate_dep Texte {}; 
   $rap_activate_dep    Texte {}; 
   set cmd "set ptr \[B_sim_ptr Pointeur [$ptr Id]\]; if {!\[string equal \$ptr NULL\]} {$ptr Sub_MetaData Drop_done ; $ptr desabonner_de_changement [$rap_fin_drag Rappel]; $n_placement Vider_fils; $n_ptr Ajouter_MetaData_T Dragging {}; $ptr Sub_MetaData Dragging}"
   B_Fire_Forget_after_simulations $cmd
   
   # Déclancher le rappel de fin de drag
   set n [$n_ptr Val_MetaData Dragging]
   if {[catch "eval \[$n Val_MetaData cmd_stop\]" err]} {
     puts "ERROR in Drag_this_node command stop:\n  - cmd_stop : [$n Val_MetaData cmd_stop]\n  - ERROR : $err"
    }

  }
}

#_______________________________________________________________________________
proc Drag_nodes {L_n {condition 1} {cmd_start ""} {cmd_stop ""}} {
 foreach n $L_n {
   set rap [$n Val_MetaData rap_drag_element];
   if {[string equal $rap {}]} {
     set rap [B_rappel [Interp_TCL]]
     B_configure $n -Ajouter_MetaData_T rap_drag_element $rap
     $n abonner_a_detection_pointeur [$rap Rappel] [ALX_pointeur_enfonce]
    }
  B_configure $n -Ajouter_MetaData_T cmd_start $cmd_start  \
	 			 -Ajouter_MetaData_T cmd_stop  $cmd_stop
   set    cmd "set p \[$rap Param\];\n"
   append cmd "set infos \[Void_vers_info \$p\]\n"
   append cmd "if {$condition} {Drag_this_node $n \$infos;}\n"
   $rap Texte $cmd
  }
}

#_______________________________________________________________________________
proc Enter_Leave_zone {n rap index} {
 set p     [$rap Param]
 set infos [Void_vers_info $p]
 set ptr   [$infos Ptr]
 set L_drop_CB [$n Val_MetaData drop_zone_L_drop_CB]
 foreach CB $L_drop_CB {
   set descr [lindex $CB 1]
   set cond  [lindex $descr 1]
   set n [Get_node_dragged_in $infos]
   if {[eval $cond] && ([$ptr Val_MetaData DRAG_ENDED] == "0")} {
	 eval [lindex $descr $index]
	}
  }
}

#_______________________________________________________________________________
proc Do_drop_on_zone {n rap} {
 set p [$rap Param]
 set infos [Void_vers_info $p]
 set L_drop_CB [$n Val_MetaData drop_zone_L_drop_CB]
 foreach CB $L_drop_CB {
   set descr [lindex $CB 1]
   Drop_info_on $infos $n [lindex $descr 1] "[lindex $descr 4]; [lindex $descr 2]" [lindex $descr 4]
  }
}

#_______________________________________________________________________________
# Liste de < noeud zone de drop
#          , condition à évaluer sur l'élément dropé identifié par $n
#          , action à déclancher qui sera succédé par $zone et $info
#          , onEnter rappel
#          , onLeave rappel
#          >
proc Drop_zones {mark L_descr} {
 foreach descr $L_descr {
   set n    [lindex $descr 0]
   set cond [lindex $descr 1]
   set rap [$n Val_MetaData rap_drop_zone];
   if {[string equal $rap {}]} {
    # Que faire si...On relache sur la zone, drop?
     set rap [B_rappel [Interp_TCL]]; 
     $n Ajouter_MetaData_T rap_drop_zone $rap
     $n abonner_a_detection_pointeur [$rap Rappel] [ALX_pointeur_relache]
	# Que faire si...On entre dans la zone
	 set rap_enter [B_rappel [Interp_TCL]]
	 $n Ajouter_MetaData_T rap_drop_enter_zone $rap_enter
	 $n abonner_a_detection_pointeur [$rap_enter Rappel] [expr [ALX_pointeur_apparition] + [Pas_rappel_si_fils_contient]] 
	# Que faire si...On sort de la zone
	 set rap_leave [B_rappel [Interp_TCL]]
	 $n Ajouter_MetaData_T rap_drop_leave_zone $rap_leave
	 $n abonner_a_detection_pointeur [$rap_leave Rappel] [expr [ALX_pointeur_disparition]]
	 
	 # new style
	 $n Ajouter_MetaData_T drop_zone_L_drop_CB ""
	 $rap_enter Texte "Enter_Leave_zone $n $rap_enter 3"
	 $rap_leave Texte "Enter_Leave_zone $n $rap_leave 4"
	 $rap       Texte "Do_drop_on_zone  $n $rap"
	 #puts "Adding drop information step 1 OK"
    }
   set     L_drop_CB [$n Val_MetaData drop_zone_L_drop_CB]
   set new_L_drop_CB [list ]
     foreach CB $L_drop_CB {
	   if {[lindex $CB 0] != $mark} {lappend new_L_drop_CB $CB}
	  }
     lappend new_L_drop_CB [list $mark $descr]
	 $n Ajouter_MetaData_T drop_zone_L_drop_CB $new_L_drop_CB

   #puts "  Adding drop information step 2 OK"
   continue
   
# OLD STYLE
   $n Ajouter_MetaData_T drop_zone_caracteristics [lrange $descr 1 end]
     set    cmd "set p \[$rap Param\];\n"
     append cmd "set infos \[Void_vers_info \$p\]\n"
     append cmd "Drop_info_on \$infos $n {[lindex $descr 1]} {[lindex $descr 4]; [lindex $descr 2]} {[lindex $descr 4]}\n"
   $rap Texte $cmd
   set rap_enter [$n Val_MetaData rap_drop_enter_zone]
     set cmd_enter "set p \[$rap_enter Param\];\nset infos \[Void_vers_info \$p\]; set ptr \[\$infos Ptr\]; set n \[Get_node_dragged_in \$infos\]\n if {\[eval $cond\] && (\[\$ptr Val_MetaData DRAG_ENDED\] == \"0\")} {puts ENTER; [lindex $descr 3]}"
	 $rap_enter Texte $cmd_enter
   set rap_leave [$n Val_MetaData rap_drop_leave_zone]
     set cmd_leave "set p \[$rap_leave Param\];\nset infos \[Void_vers_info \$p\]; set ptr \[\$infos Ptr\]; set n \[Get_node_dragged_in \$infos\]\n if {\[eval $cond\]} {puts EXIT; [lindex $descr 4]}"
	 $rap_leave Texte $cmd_leave
   #puts "$rap Texte {$cmd}"
  }
}

#_______________________________________________________________________________
proc Get_node_dragged_in {infos} {
 set ptr   [$infos Ptr]
 set n_ptr [$ptr   Noeud_representation]
 set n     [$n_ptr Val_MetaData Dragging]
 if {$n == ""} {set rep ""} else {set rep [Real_class $n]}
 return $rep
}

#_______________________________________________________________________________
proc Drop_info_on {infos n_zone cond action {action_si_descendants ""}} {
 #puts "Drop_info_on $infos $n_zone {$cond} {$action}\n  n_zone : $n_zone\n  NOEUD  : [$infos NOEUD]"
 set ptr   [$infos Ptr]
 $ptr Add_MetaData DRAG_ENDED 1
 set n_ptr [$ptr   Noeud_representation]
 set n     [$n_ptr Val_MetaData Dragging]
 set n_drop_on [Real_class [$infos NOEUD]]
 if {![string equal $n_zone $n_drop_on]} {
   if {[catch {eval "$action_si_descendants"} err]} {puts "ERROR in Drop_info_on :\n $action_si_descendants $n_zone $infos \n  => $err"}
   return
  }
 if {[string equal $n {}]} {return}
 
 if {[eval $cond]} {
   #puts "Drop_info_on::Drop de $n dans $n_zone !!!"
   if {[catch {eval "$action $n_zone $infos"} err]} {puts "ERROR : Drop_info_on $infos $n_zone $cond $action\n  => $err"}
  }
}

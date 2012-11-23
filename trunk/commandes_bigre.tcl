global i_mere; global n_i_mere; global tcl_version
set n_i_mere    [$i_mere get_Noeud_interface]
set Arbre_scene [$n_i_mere Arbre_scene]
set Noeud_scene [$n_i_mere Noeud_scene]
set sim_ctc     [$n_i_mere Sim_contact]
set sim_ptr     [$n_i_mere Sim_pointeur]
set sim_sds     [$n_i_mere Sim_dispo_saisi]
set sim_couche  [$n_i_mere Sim_couche]

proc N_i_mere   args {global n_i_mere; 
                      if {[string length $args]} {return [eval $n_i_mere $args]}
                      return $n_i_mere}
proc Interp_TCL args {global interp_tcl
                      if {[string length $args]} {return [eval $interp_tcl $args]}
                      return $interp_tcl}
set interp2 [interp create]
set cmd "proc Interp_TCL2 args {if {\$args == \"\"} {return $interp2} else {return \[$interp2 eval \$args\]}}"
eval $cmd

proc B_sim_ctc  args {global sim_ctc;
                      if {[string length $args]} {return [eval $sim_ctc $args]}
                      return $sim_ctc
                     }
proc B_sim_ptr  args {global sim_ptr;
                      if {[string length $args]} {return [eval $sim_ptr $args]}
                      return $sim_ptr
                     }
proc B_sim_sds  args {global sim_sds;
                      if {[string length $args]} {return [eval $sim_sds $args]}
                      return $sim_sds
                     }
proc B_sim_couche args {global sim_couche;
                        if {[string length $args]} {return [eval $sim_couche $args]}
                        return $sim_couche
                       }


#___________________________________________________________________________________________________________________________________________
proc Noeud_courant {} {return [alx_noeud_scene_Noeud_courant]}

#___________________________________________________________________________________________________________________________________________
proc Real_class {n} {
 if {$n == "NULL"} {return NULL}
 set p [$n Real_class_cmd]
 return [$p $n]
}

#___________________________________________________________________________________________________________________________________________
proc Type_objet_SWIG {obj} {
 if {[regexp {^_[0-9a-f]*_p_(.*)$} $obj reco class]} {return $class}
 return ""
}

#___________________________________________________________________________________________________________________________________________
proc Detruire {obj} {
 if {[regexp {^_[0-9a-f]*_p_(.*)$} $obj reco class]} {
   return [delete_$class $obj]
  } else {return "$obj is not recognize as a SWIG object"}
}

#___________________________________________________________________________________________________________________________________________
source utils_B207.tcl
source B_bouton_perso.tcl
source fonctions_clipping.tcl

#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
set point_bigre         new_alx_point2D                                  ; proc B_point              args {return [eval new_alx_point2D                           $args]}
set vecteur_bigre       new_alx_vecteur2D                                ; proc B_vecteur            args {return [eval new_alx_vecteur2D                         $args]}
set noeud_bigre         new_alx_noeud_scene                              ; proc B_noeud              args {return [eval new_alx_noeud_scene                       $args]}
set fiche_bigre         new_alx_noeud_fiche_sdl_opengl                   ; proc B_fiche              args {return [eval new_alx_noeud_fiche_sdl_opengl            $args]}
set fiche_z_bigre       new_alx_noeud_fiche_zoom_sdl_opengl              ; proc B_fiche_z            args {return [eval new_alx_noeud_fiche_zoom_sdl_opengl       $args]}
set fiche_o_bigre       new_alx_noeud_fiche_ovale_sdl_opengl             ; proc B_fiche_o            args {return [eval new_alx_noeud_fiche_ovale_sdl_opengl      $args]}
set fiche_oz_bigre      new_alx_noeud_fiche_ovale_zoom_sdl_opengl        ; proc B_fiche_oz           args {return [eval new_alx_noeud_fiche_ovale_zoom_sdl_opengl $args]}
set image_bigre         new_alx_noeud_image_sdl_opengl                   ; proc B_image              args {return [eval new_alx_noeud_image_sdl_opengl            $args]}
set CamNote_bigre       new_CamNote                                      ; proc B_CamNote            args {return [eval new_CamNote                               $args]}
set visu_cam_bigre      new_alx_noeud_visualisation_camera               ; proc B_camera             args {return [eval new_alx_noeud_visualisation_camera        $args]}
set chaine_char         new_alx_chaine_char                              ; proc B_cc                 args {return [eval new_alx_chaine_char                       $args]}
set Rappel_TCL          new_Appel_TCL                                    ; proc B_rappel             args {return [eval new_Appel_TCL                             $args]}
set bouton_bigre        new_alx_noeud_bouton_sdl_opengl                  ; proc B_bouton             args {return [eval new_alx_noeud_bouton_sdl_opengl           $args]}
set repere2D            new_alx_repere2D                                 ; proc B_repere2D           args {return [eval new_alx_repere2D                          $args]}
set ztxt_bigre          new_alx_noeud_zone_texte_sdl_opengl              ; proc B_texte              args {set cmd "";
                                                                                                           switch [llength $args] {
                                                                                                             0       {set args "100 30 25 [fonte_Arial] [B_sim_sds]"
																											          set txt ""
																													  set cmd {$rep Editable 0}
																													 }
                                                                                                             1       {eval "set txt $args"
																											          set args "100 30 25 [fonte_Arial] [B_sim_sds]"
																											          set cmd {$rep Optimiser_dimensions; $rep Editable 0}
																													 }
																											 2       {set txt [lindex $args 1]
																													  set cmd {$rep Optimiser_dimensions; $rep Editable 0}
																													  set args "100 30 [lindex $args 0] [fonte_Arial] [B_sim_sds]"
																											         }
																											 default {set args $args
																											          set txt ""
																													 }
																											}
																										   set rep [eval new_alx_noeud_zone_texte_sdl_opengl       $args]
																										   $rep TEXTE $txt
																										   eval $cmd
																										   $rep Calculer_boites
																										   return $rep
																										  }
set potentiometre_bigre new_alx_noeud_potentiometre_sdl_opengl           ; proc B_potentiometre      args {return [eval new_alx_noeud_potentiometre_sdl_opengl    $args]}

set transfo_dyn            new_deformation_dynamique                     ; proc B_transfo            args {return [eval new_deformation_dynamique                 $args]}
set transfo_dyn_rep        new_deformation_dynamique_repere              ; proc B_transfo_rep        args {return [eval new_deformation_dynamique_repere          $args]}
set transfo_dyn_rep_et_dim new_deformation_dynamique_repere_et_dim       ; proc B_transfo_rep_et_dim args {return [eval new_deformation_dynamique_repere_et_dim   $args]}
set transfo_dyn_etire_fen  new_deformation_dynamique_etirement_fenetre   ; proc B_transfo_etire_fen  args {return [eval new_deformation_dynamique_etirement_fenetre $args]}
set transfo_dyn_fen        new_deformation_dynamique_fenetre             ; proc B_transfo_fen        args {return [eval new_deformation_dynamique_fenetre         $args]}
set transfo_dyn_int        new_deformation_dynamique_int                 ; proc B_transfo_int        args {return [eval new_deformation_dynamique_int             $args]}
set transfo_dyn_uint       new_deformation_dynamique_uint                ; proc B_transfo_uint       args {return [eval new_deformation_dynamique_uint            $args]}
set transfo_dyn_double     new_deformation_dynamique_double              ; proc B_transfo_double     args {return [eval new_deformation_dynamique_double          $args]}
set info_courbe_lentille   new_info_courbe_lentille                      ; proc B_courbe_lentille    args {return [eval new_info_courbe_lentille                  $args]}

set TabDouble              new_Tab_double                                ; proc B_TabDouble          args {return [eval new_Tab_double $args]}
set poly2D                 new_alx_noeud_polygone_sdl_opengl             ; proc B_polygone           args {return [eval new_alx_noeud_polygone_sdl_opengl $args]}

proc B_sim_ptr_TCL       args {return [eval new_alx_simulateur_pointeurs_TCL $args]}
proc B_evt_ptr           args {return [eval new_alx_evennement_pointeur      $args]}
proc B_evt_ptr_info_comp args {return [eval new_info_comp_evennement         $args]}
proc B_MetaData 		 args {return [eval new_MetaData         			 $args]}

proc Visu_Cam {{num_cam 0}} {
 global visu_cam
   if {![info exists visu_cam]} {
     set pre_rendeur [N_i_mere Prerendeur]
     set visu_cam [B_camera $num_cam]
     $visu_cam Afficher_noeud 0
     $visu_cam Gerer_contacts 0
	 $visu_cam Vider_peres
     [N_i_mere Noeud_scene] Ajouter_fils $visu_cam
    }
 return $visu_cam
}

#______________________________________________________________________________
proc B_Raise_L {L} {
 foreach e $L {B_Raise $e}
}

#______________________________________________________________________________
proc B_Raise {n} {
 set L_p {}
 set it [$n Init_parcours_peres]
 while {![$n Est_parcours_peres_fini $it]} {
   lappend L_p [$n Courant_dans_parcours_peres $it] 
   set it [$n Suivant_dans_parcours_peres $it]
  }
 $n Terminer_parcours_peres $it
 
 foreach p $L_p {
   $p Retirer_fils $n
   $p Ajouter_fils_au_debut $n
  }
}

#______________________________________________________________________________
proc Add_arg {txt_name arg} {
 upvar $txt_name txt
 if {[llength $arg] > 1 || $arg == ""} {
   append txt " {" $arg "}"
  } else {append txt " $arg"}
}

#______________________________________________________________________________
proc B_configure {n args} {
 set L_cmd {}; set cmd ""
 foreach arg $args {
   if {[string equal -length 1 - $arg]} {
     if {[regexp {^.[0-9].*$} $arg]} {
	   Add_arg cmd $arg
	  } else {lappend L_cmd $cmd; set cmd [string range $arg 1 end]
	         }
	} else {Add_arg cmd $arg}
  } 
 lappend L_cmd $cmd
 
 foreach cmd $L_cmd {
   if {[string equal $cmd {}]} {continue}
   eval "$n $cmd"
  }
}

#______________________________________________________________________________
proc B_Aide_nom_list {v re} {
 set rep {}
 if {[catch "$v auzlidgfsd" res]} {
   if {[regexp {Invalid method. Must be one of: (.*)$} $res reco L]} {
     foreach e $L {
       if {[regexp $re $e]} {lappend rep $e}
      }
    }
  }
 return $rep
}

#______________________________________________________________________________
proc B_Aide_nom {v re} {
 set L [B_Aide_nom_list $v $re]
 foreach r $L {puts $r}
}

#______________________________________________________________________________
#______________________________________________________________________________
#______________________________________________________________________________
proc Redirect_key_events_from_to {prim dest_prim} {
 puts "Redirect_key_events_from_to $prim $dest_prim"
 set    cmd "set dest_prim $dest_prim\n"
 append cmd {set evt [B_sim_couche Evt_courant];} "\n"
 append cmd {set ptr [$evt Ptr]; set pt [$ptr P_Point];} "\n"
 append cmd {set res ""; puts "Redirect keys from->to pt = $pt dest_prim = $dest_prim"; if {[catch "B_sim_sds Prendre_evennements_lies_a $pt \[$dest_prim Liant\]" res]} {puts "ERROR in Redirect_key_events_from_to:\n$err"}; }

 set rap_redirect_key [$prim Val_MetaData rap_redirect_key]
 if {$rap_redirect_key == ""} {set rap_redirect_key [B_rappel [Interp_TCL]]}
 $rap_redirect_key Texte $cmd
 $prim desabonner_de_detection_pointeur [$rap_redirect_key Rappel]
 $prim abonner_a_detection_pointeur     [$rap_redirect_key Rappel]
}

#______________________________________________________________________________
#______________________________________________________________________________
#______________________________________________________________________________
proc B_transfo_rap {t txt_rap_pdt {txt_rap_before {}} {txt_rap_after {}}} {
 set transfo [B_transfo $t]
   set rap [B_rappel [Interp_TCL]]; $rap Texte "set t \[$transfo T_courant\]; set v \[$transfo V_courant\]; $txt_rap_pdt"; $transfo abonner_a_rappel_pendant [$rap Rappel]
   if {$txt_rap_before != ""} {set rap [B_rappel [Interp_TCL]]; $rap Texte $txt_rap_before; $transfo abonner_a_rappel_fin_amorce [$rap Rappel]}
   if {$txt_rap_after  != ""} {set rap [B_rappel [Interp_TCL]]; $rap Texte $txt_rap_after ; $transfo abonner_a_rappel_fin        [$rap Rappel]}
 $transfo Demarrer
 [N_i_mere] Ajouter_deformation_dynamique $transfo
}

#______________________________________________________________________________
proc B_rappel_Fire_Forget {n abo txt} {
 set r [B_rappel [Interp_TCL]]
 if {[regexp {^abonner_a_(.*)$} $abo reco d]} {
   if {[regexp {^(.*)_deb +(.*)$} $d reco d l] || [regexp {^(.*)_fin +(.*)$} $d reco d l]} {
     set desabo "desabonner_de_$d $l"
    } else {set desabo "desabonner_de_$d"}
   set txt    "$txt; $n $desabo [$r Rappel]"
  } else {$r Texte $txt
         }
 $r Texte $txt
 eval "$n $abo [$r Rappel]"
 return $r
}

#______________________________________________________________________________
proc B_Fire_Forget_after_simulations {txt} {
 set transfo [B_transfo 0]
   set rap [B_rappel [Interp_TCL] $txt]
   $transfo abonner_a_rappel_fin [$rap Rappel]
   $transfo Demarrer
 [N_i_mere] Ajouter_deformation_dynamique $transfo
}

#______________________________________________________________________________
proc Aide {{sujet "rien"}} {
 set rep "Procédures d'aide de la bibliothèque BIGre\n";
 set comp "      Taper \"Aide generale\" pour une présentation de l'aide.";
 switch $sujet {
  generale       {set comp "Commandes présentent : \n  generale\n  chaine_char\n  Interface_mere\n  CamNote\n  noeud_bigre\n  image_bigre\n  fiche_bigre, fiche_z, fiche_o, fiche_oz\n  noeud_scene\n  bouton_bigre\n  Rappel_TCL\n  transfo : les transformations dynamiques de repères, fenêtres etc...\n  Variables";}
  Interface_mere {set comp {Interface_mere represente un gestionnaire d'interface, $n_i_mere en est une};}
  noeud_bigre    {set comp "Définit un noeud. Utilisation : \"set n [$noeud_bigre]\".\n Un noeud hérite d'un repère 2D. Pour modifier ce repère, utilisez les méthodes suivantes:\n  Origine [(x y)|p]:pour consulter ou modifier l'origine du repère.\n  Translation [(x y)|v]: pour déplacer l'origine par de (x,y) ou du vecteur v, par rapport au repère père.\n  Translation_interne [(x y)|v]: idem mais par rapport au repère du noeud.\n  Rotation: fixe la rotation par rapport au père.\n  Pivoter a [(x y)|p]: Pivoter de a degrés par rapport au point (x,y) ou p, exprimé dans le repère père.\n  Pivoter_interne a [(x y)|p]: Pivoter de a degrés par rapport au point (x,y) ou p, exprimé dans le repère du noeud."}
  fiche_bigre    {set comp "Fiche rectangulaire.\nConstructeurs :\n  \"\$fiche_bigre Titre Longueur Hauteur Interface_mere\"\n avec :\n   Titre : le titre de la fiche\n   Longueur : la longueur de la fiche, en unités\n   Hauteur : la hauteur de la fiche, en unités\n   Interface_mere : l'interface d'initialisation de la fiche\nLa fonction renvoi un pointeur sur une fiche, pour que celle-ci soit présente dans l'interface, il faut la raccrocher à un autre noeud.\nExemple :\nset fiche \[fiche_bigre coucou 128 64 \$n_i_mere]\n\$noeud Ajouter_fils \$fiche";}
  fiche_z_bigre  {set comp "Fiche rectangulaire de zoom. Instantiation idem que pour fiche_bigre";}
  fiche_o_bigre  {set comp "Fiche ovale. Instantiation idem que pour fiche_bigre";}
  fiche_oz_bigre {set comp "Fiche ovale de zoom. Instantiation idem que pour fiche_bigre";}
  image_bigre    {set comp "Noeud image.\nConstructeurs :\n  \"\$image_bigre\"\n  \"\$image_bigre Nom_fichier_image\"\n  \"\$image_bigre Image_gl\"\n  \"\$image_bigre Info_texture Longueur Hauteur\"\n avec\n   Nom_fichier_image : le nom du fichier contenant l'image, éventuellement avec le chemin.\n   Image_gl : une image_gl déja chargée.\n   Info_texture : des informations sur une texture déja existante.\n   Longueur : la longueur de l'image pour l'affichage, en unités. Il ne s'agit pas de sa longueur en pixels.\n   Hauteur : la hauteur de l'image pour l'affichage, en unité. Il ne s'agit pas de sa hauteur en pixels.\n";}
  CamNote_bigre  {set comp "CamNote est un visualisateur de présentation.\nInitialisation :\n   CamNote_bigre interface_mere visu_cam environnement_tcl";}
  visu_cam_bigre {set comp "Gère une source vidéo, de type caméra.\nInitialisation :\n   visu_cam_bigre longueur hauteur pre_rendeur\n   le prérendeur peut s'obtenir par \[\$n_i_mere Prerendeur]";}
  chaine_char    {set comp "Une chaine de charactères."}
  Rappel_TCL     {set comp "Contient une méthode de rappel (dans F_rappel). On peut l'utiliser pour se brancher sur des évennements de la bibliothèque BIGre, une fois l'évennement reçut la fonction de rappel est appeller, elle consiste a évaluer les commandes TCL contenues dans Texte\nCréation :\n  \$Rappel_TCL alx_interpréteur_TCL\n  \$Rappel_TCL alx_interpréteur_TCL commandesTCL"}
  Variables      {set comp "Les variables déja présentent sont\n  interp_tcl : L'alx_interpréteur TCL\n  n_i_mere : L'interface mère\n  Noeud_scene : La racine de la scène"}
  bouton_bigre   {set comp "Déclare un bouton, les boutons sont de deux type, rectangulaires (1) ou ovaux (2). Constructeurs:\n    \$bouton_bigre alx_image_opengl alx_interface_sdl_opengl_1 alx_noeud_scene type\n    \$bouton_bigre Longueur Hauteur alx_interface_sdl_opengl_1 alx_noeud_scene type"}
  transfo        {set comp "Une transformation dynamique parmi :\n  \$transfo_dyn durée\n  \$transfo_dyn_rep durée rep_départ rep_arrivé\n  \$transfo_dyn_etire_fen durée rep_départ translucidité_départ rep_arrivé translucidité_arrivée\n  \$transfo_dyn_\[int|uint|double\] durée P_scalaire \[P_\]scalaire"}
  texte          {set comp "Des zones de texte peuvent être créées à l'aide de la commande:\n ztxt_bigre tx ty taille_lettres fonte sds\n    \$sim_sds pour avoir le simulateur de saisie\n    \[fonte_Arial\] pour avoir la fonte Arial"}
  }
 append rep $comp "\n"
 return $rep
}

#____________________________________________________________________________
proc B207_transformation {pt L_reperes} {
 foreach R $L_reperes {$R Changer_coordonnees $pt}
}

#____________________________________________________________________________
proc B207_transformation_inverse {pt L_reperes} {
 while {[llength $L_reperes]} {
   set R [lindex $L_reperes end]
   $R Changer_coordonnees_inverse $pt
   set L_reperes [lrange $L_reperes 0 end-1]
  }
 # if {[string equal $L_reperes {}]} {return}
 # set repere [lindex $L_reperes end]
   # $repere Changer_coordonnees_inverse $pt
 # return [B207_transformation_inverse $pt [lrange $L_reperes 0 end-1]]
}

#____________________________________________________________________________
proc Invert_list {L} {
 set rep {}
 foreach e $L {set rep [linsert $rep 0 $e]}
 return $rep
}

#____________________________________________________________________________
proc Convert_in_screen_space_from {B207_node x_name y_name} {
 upvar $x_name x
 upvar $y_name y
 
 set root [N_i_mere Noeud_scene]
 set pt [$root Val_MetaData Convert_in_screen_space_from_pt]
 if {[string equal $pt ""]} {
   set pt [B_point]
   $root Ajouter_MetaData_T Convert_in_screen_space_from_pt $pt
  }
 set L_n [Invert_list [Ancetre_de_a $B207_node $root]]
 $pt maj $x $y
 foreach e $L_n {
   $e Changer_coordonnees_prim_inverse $pt
  }
 set x [$pt X]; set y [$pt Y]
}

#____________________________________________________________________________
proc Ancetre_de_a {n racine} {
 if {![string equal $n $racine]} {
   set pere [Real_class [$n Pere]]
   if {![string equal $pere NULL]} {
     set L [Ancetre_de_a $pere $racine]
	 set rep [concat $L $n]
	} else {set rep $n}
  } else {set rep $n}
 return $rep
}

#____________________________________________________________________________
proc Lister_peres_de {n} {
 set L {}
 set it [$n Init_parcours_peres]
 while {![$n Est_parcours_peres_fini $it]} {
   lappend L [Real_class [$n Courant_dans_parcours_peres $it]]
   set it [$n Suivant_dans_parcours_peres $it]
  }
 $n Terminer_parcours_peres $it
 
 return $L
}

#____________________________________________________________________________
proc Lister_fils_de {n} {
 set L {}
 set it [$n Init_parcours_fils]
 while {![$n Est_parcours_fils_fini $it]} {
   lappend L [Real_class [$n Courant_dans_parcours_fils $it]]
   set it [$n Suivant_dans_parcours_fils $it]
  }
 $n Terminer_parcours_fils $it
 
 return $L
}

#____________________________________________________________________________
proc Trouver_noeud_nomme {nom L_name} {
 upvar $L_name L
 foreach n $L {set cc [$n Nom]; if {[string equal [$cc Texte] $nom]} {return [Real_class $n]}}
 return ""
}

#____________________________________________________________________________
proc Update_boites_rec {n} {
 if {[$n Val_MetaData Update_boites_rec] != ""} {return}
 $n Ajouter_MetaData_T Update_boites_rec ici

 set it [$n Init_parcours_fils]
 while {![$n Est_parcours_fils_fini $it]} {
   set next_n [$n Courant_dans_parcours_fils $it]
   Update_boites_rec $next_n
   set it [$n Suivant_dans_parcours_fils $it]
  }
 $n Terminer_parcours_fils $it 
 
 $n Retirer_MetaData_T Update_boites_rec
 $n Calculer_boites
}

#____________________________________________________________________________
proc Lister_noeuds_depuis {n} {
 if {[$n Val_MetaData Lister_noeuds_depuis] != ""} {return {}}
 $n Ajouter_MetaData_T Lister_noeuds_depuis ici
 set rep $n
 set it [$n Init_parcours_fils]
 while {![$n Est_parcours_fils_fini $it]} {
   set next_n [$n Courant_dans_parcours_fils $it]
   set rep [concat $rep [Lister_noeuds_depuis $next_n]]
   set it [$n Suivant_dans_parcours_fils $it]
  }
 $n Terminer_parcours_fils $it
 
 $n Retirer_MetaData_T Lister_noeuds_depuis
 return $rep
}
#____________________________________________________________________________
proc Afficher_rec {n {dec {}} {nb_repere_actifs 0}} {
 if {[$n Val_MetaData Afficher_rec] == "Afficher_rec"} {puts "_________________ !!!!!!! __________________"; return {}}
 $n Ajouter_MetaData_T Afficher_rec Afficher_rec
 if {[$n Prendre_en_compte_repere]} {incr nb_repere_actifs}
 set CometPM [$n Val CometPM]
 if {$CometPM != ""} {set CometPM [$CometPM get_LC]; set name [$CometPM get_name]} else {set name ""}
 puts "__ $nb_repere_actifs ___$dec [$n Nom_IU] : $name : [Real_class $n]"
 foreach fils [Lister_fils_de $n] {
   Afficher_rec $fils "$dec " $nb_repere_actifs
  }
 
 $n Retirer_MetaData_T Afficher_rec
}

#____________________________________________________________________________
#_________________________________ BOUTONS __________________________________
#____________________________________________________________________________
proc B_bouton_texte {txt size pere {bt_type 1}} {
 set n_txt [B_texte 100 100 $size [fonte_Arial] [B_sim_sds]];
   $n_txt TEXTE $txt
   $n_txt Editable 0
   $n_txt Optimiser_dimensions
 return [B_bouton_from $n_txt $pere $bt_type]
}

#____________________________________________________________________________
proc B_bouton_from {n pere {bt_type 1}} {
 $n Calculer_boites
 set n_box [$n Boite_noeud_et_fils_glob]; puts "n_box <[$n_box Tx]; [$n_box Ty]>"
 set bt [B_bouton [$n_box Tx] [$n_box Ty] [N_i_mere] $pere $bt_type]
   [$bt Noeud_fen] Ajouter_fils $n
   $pere Ajouter_fils_au_debut $bt
   set fen [$bt Noeud_fen]
   if {$bt_type == 1} {
     $fen Origine_du_contenu [$n_box BG_X] [$n_box BG_Y]
	} else {$fen Origine_du_contenu [expr [$n_box BG_X] + [$n_box Tx]] [expr [$n_box BG_Y] + [$n_box Ty]]
	        puts "$fen Origine_du_contenu [expr [$n_box BG_X] + [$n_box Tx]] [expr [$n_box BG_Y] + [$n_box Ty]]"
	       }
   $bt Mettre_a_jour
   $bt Calculer_boites
 return $bt
}

#____________________________________________________________________________
#__________________________ POLYGONES _______________________________________
#____________________________________________________________________________

#____________________________________________________________________________
proc Contact_model {n_ref capa args} {
 set ctc [$n_ref Val_MetaData B_contact]
 if {[string equal $ctc ""]} {
   set ctc ctc_$n_ref
   B_contact $ctc "$n_ref $capa"
  } else {$ctc add "$n_ref $capa"
         }
 eval "$ctc configure $args"
}

#____________________________________________________________________________
method B_contact constructor {ref args} {
#puts "args  : $args"
 set this(pt_trans)        [B_point]
 set this(pt_rot)          [B_point]
 set this(L_modeles)       [list]
 set this(ec)              [B_sim_ctc Ajouter_ensemble_contact]
 this ref $ref

 [lindex $ref 0] Ajouter_MetaData_T B_contact $objName
 eval "$objName configure $args"
 return $objName
}

#____________________________________________________________________________
method B_contact destructor {} {
 if {[string equal $this(ec) {}]} {} else {B_sim_ctc Retirer_ensemble_contact $this(ec)       
                                           Detruire $this(ec)
                                          }
}

#____________________________________________________________________________
method B_contact configure {args} {
#puts "args : $args"
 set L_cmd [split $args -]
 foreach cmd $L_cmd {
   if {[string equal $cmd {}]} {continue}
   puts "cmd : $cmd"
   eval "$objName $cmd"
  }
}
#____________________________________________________________________________
method B_contact ref {noeud_et_capa} {
 set this(noeud_reference) [lindex $noeud_et_capa 0]
 set this(mc_ref) [this add $noeud_et_capa]
 
 $this(noeud_reference) Calculer_boites
 set b [$this(noeud_reference) Boite_noeud_et_fils]
 this pt_trans [$b Cx] [$b Cy]
 this pt_rot   [$b Cx] [$b Cy]
}
#____________________________________________________________________________
method B_contact Lister_modeles {} {
 set rep {}
 foreach m $this(L_modeles) {
   append rep [$m Noeud] " " [$m MP] "\n"
  }
 return $rep
}
#____________________________________________________________________________
method B_contact add {noeud_et_capa} {
 set n_c $noeud_et_capa
   set noeud [lindex $n_c 0]
   set capa  [lindex $n_c 1]
#   Voyons si noeud pas déja présent...
   set est_deja_dans ""
   foreach m $this(L_modeles) {
     if {[string equal [$noeud MP] [$m MP]]} {
       set est_deja_dans $m
       break
      }
    }
   if {[string equal $est_deja_dans {}]} {
     set MP [$noeud MP]
     set m [B_sim_ctc Ajouter_modele $this(ec) $MP $this(noeud_reference) $capa]
     lappend this(L_modeles) $m
    } else {set m $est_deja_dans
            $m Capacite $capa}
 
 $noeud Ajouter_MetaData_T B_contact_and_capa "$objName $capa"
 
 return $m
}

#____________________________________________________________________________
method B_contact pt_trans {x y} {$this(pt_trans) maj $x $y
                                 B_sim_ctc Fixer_point_translation_defaut $this(ec) $this(mc_ref) $this(pt_trans)}
#____________________________________________________________________________
method B_contact pt_rot   {x y} {$this(pt_rot)   maj $x $y
                                 B_sim_ctc Fixer_point_rotation_defaut    $this(ec) $this(mc_ref) $this(pt_rot)  }
#____________________________________________________________________________
method B_contact pt_trans_actif {v} {
 B_sim_ctc Activer_point_translation_defaut $this(ec) $v
}

#____________________________________________________________________________
method B_contact pt_rot_actif   {v} {
 B_sim_ctc Activer_point_rotation_defaut    $this(ec) $v
}

#____________________________________________________________________________
method B_contact abonner {rap} {
 set R [$rap Rappel]
 $this(ec) desabonner_de_changement $R
 $this(ec) abonner_a_changement     $R
}

#____________________________________________________________________________
method B_contact desabonner {rap} {
 $this(ec) desabonner_de_changement [$rap Rappel] 
}


#____________________________________________________________________________
#____________________________________________________________________________
#____________________________________________________________________________
proc TabDouble2list {T} {
 set L ""
 for {set i 0} {$i < [$T Taille]} {incr i} {lappend L [$T Consulter $i]}
 
 return $L
}

#____________________________________________________________________________
proc ProcTabDouble {Lpoints {t ""}} {
 if {$t == ""} {set t [B_TabDouble]}
 $t Reserver_place [llength $Lpoints]
 set pos 0
 foreach v $Lpoints {$t Ajouter $pos $v; incr pos}
 return $t
}

#____________________________________________________________________________
proc ProcRect {X1 Y1 X2 Y2} {
 set pos 0
 set t [B_TabDouble]
 $t Reserver_place 8
   $t Ajouter 0 $X1 $Y1
   $t Ajouter 2 $X2 $Y1
   $t Ajouter 4 $X2 $Y2
   $t Ajouter 6 $X1 $Y2
 return $t
}

#____________________________________________________________________________
proc ProcOeuf {X Y TX TY1 TY2 NBPOINTS} {
 set pos 0
 set Deux_PI 6.283185307179586476925286766559
 set t [B_TabDouble]
 $t Reserver_place [expr 2*$NBPOINTS]
 for {set i 0} {$i<$NBPOINTS} {incr i} {
   set angle [expr $pos*$Deux_PI/$NBPOINTS]
   if {2*$i < $NBPOINTS} {set TY $TY1} else {set TY $TY2}
   $t Ajouter [expr 2*$pos] [expr $X+$TX*cos($angle)] [expr $Y+$TY*sin($angle)]
   incr pos
  }
 return $t
}

#____________________________________________________________________________
proc ProcOvale {X Y TX TY NBPOINTS} {
 set pos 0
 set Deux_PI 6.283185307179586476925286766559
 set t [B_TabDouble]
 $t Reserver_place [expr 2*$NBPOINTS]
 for {set i 0} {$i<$NBPOINTS} {incr i} {
   set angle [expr $pos*$Deux_PI/$NBPOINTS]
   $t Ajouter [expr 2*$pos] [expr $X+$TX*cos($angle)] [expr $Y+$TY*sin($angle)]
   incr pos
  }
 return $t
}

puts coucou
source tkcon.tcl
if {[catch "package present Speech_API" res]} {
  #if {[catch "load Speech_api.dll" res]} {
  #  puts "ERROR while loading Speech_api.dll:\n$res"
  # }
 }

#___________________________________________________________________________________________________________________________________________
proc C_B_Toolglass {n cmd_todo} {
 $n Empiler_contenant 1
 set rappel [$n Val_MetaData Toolglass_Rappel]
 if {[string equal $rappel {}]} {
   set rappel [B_rappel [Interp_TCL]]
   set    cmd ""
   append cmd "set p \[$rappel Param\];\n"
   append cmd "set infos_origine \[Void_vers_info \$p\];\n"
   append cmd "set L_empilees \[\$infos_origine L_Reponses_cumulees\];\n"
   append cmd "set it \[\$L_empilees Init_parcours\];\n"
   append cmd "while \{\[\$L_empilees Parcours_continue \$it\]\} \{\n"
   append cmd "  set infos \[\$L_empilees Courant_dans_parcours \$it\];\n"
   append cmd "  set noeud \[\$infos NOEUD\];\n"
   append cmd "#RAPPEL CMD\n"
   append cmd "#/RAPPEL CMD\n"
   append cmd "  set it \[\$L_empilees Suivant_dans_parcours \$it\]\}\n"
   append cmd "\$L_empilees Terminer_parcours \$it\n"
   $rappel Texte $cmd
   $n abonner_a_detection_pointeur [$rappel Rappel] 2
   $n Ajouter_MetaData_T Toolglass_Rappel $rappel
  }
# Insert L_cmd
 if {[regexp "^(.*)#RAPPEL CMD\n(.*)#/RAPPEL CMD\n(.*)\$" [$rappel Texte] reco avt pdt apr]} {
   $rappel Texte "${avt}#RAPPEL CMD\n${cmd_todo};\n#/RAPPEL CMD\n${apr}"
  }
 return $rappel
}
 

#____________________________________________________________________________

return "set r \[B_rappel_Fire_Forget \$Noeud_scene \{abonner_a_LR_parcours_deb [$Noeud_scene LR_Av_aff]\} toto\]"
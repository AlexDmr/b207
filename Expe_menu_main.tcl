if {![info exists noeud_partage]} {source groupe.tcl}
source menu_main.tcl
source stroke_menu.tcl

  N_i_mere Afficher_souris
  $img_fond Vider_peres
  $f_obs Vider_peres
  $noeud_editeur_tcl Vider_peres

#___________________________________________________________________________________________________________________________________________
# Creation du logiciel de dessin
 set n_root [B_noeud]
   $n_root Position_des_fils_changeable 0
   $noeud_partage Ajouter_fils $n_root

#___________________________________________________________________________________________________________________________________________
# On pose un polygone
 set poly_pour_main [B_polygone]
   $n_root Ajouter_fils_au_debut $poly_pour_main
   $poly_pour_main Etirement 1.25 1.25
   $poly_pour_main Gerer_contacts 0; $poly_pour_main Afficher_noeud 0
   #B_contact ctc_poly_pour_main "$poly_pour_main 1"
  
#___________________________________________________________________________________________________________________________________________
 Menu_item menu_item_Draw_Void  "" "" "" [list]
 set L_item_to_find [list [list Paris London Amsterdam New-York Moscow Roma Berlin Madrid] \
                          [list Italia France Spain Belgium Romania Russia Japan Mexico] \
						  [list Leopard Lion Tiger Dog Cat Chicken Horse Bird] \
						  [list Lenin Churchill Wilson Kenedy Marx Gagarin Armstrong Trotsky] \
						  [list Apple Peach Banana Strawberry Raisin Nut Ananas Melon] \
						  [list Jo Jan Geert Alex Petr Nasim Kris Davy] \
						  [list Head Foot Hand Finger Knee Nose Eye Leg] \
						  [list Table Chair Cup Fork Knife Plate Spoon Glass] \
                    ]
 set L_items ""; set num_ss_menu 0
 foreach L $L_item_to_find {
   # Generate sub menus
   set num_item 0
   foreach item $L {
     lappend L_items [Menu_item menu_item_ss_menu_${num_ss_menu}_item_$num_item  "$item" "expe Propose $item" "" [list]]
	 incr num_item
    }
   Complete_with L_items menu_item_Draw_Void 8
   # Add new menu
   #lappend L_items [Menu_item menu_item_ss_menu_$num_ss_menu  "Free Hand" "" "" $L_ss_menus]
   incr num_ss_menu
  }
 Complete_with L_items menu_item_Draw_Void 8

 Menu_item menu_item_root "MenuRoot" "" "Menu root" $L_items
 

# Génération du menu
Automate_Menu_Main   automate_main $poly_pour_main menu_item_root
  automate_main Load_polygone menus/menu_main_8.poly

  
#___________________________________________________________________________________________________________________________________________
#_____________________________________________________________ Stroke menu _________________________________________________________________
#___________________________________________________________________________________________________________________________________________
 set L_items ""; set num_ss_menu 0
 foreach L $L_item_to_find {
   # Generate sub menus
   set L_ss_items ""; set num_item 0
   foreach item $L {
     lappend L_ss_items [Menu_item menu_item_stroke_ss_menu_${num_ss_menu}_item_$num_item  "$item" "puts $item; expe Propose $item" "" [list]]
	 incr num_item
    }
   lappend L_ss_items menu_item_Draw_Void
   lappend L_items [Menu_item menu_item_ss_menu_${num_ss_menu}  "" "" "" $L_ss_items]
   incr num_ss_menu
  }
 lappend L_items menu_item_Draw_Void
 Menu_item stroke_menu_item_root "MenuRoot" "" "Menu root" $L_items
 set noeud_root_pour_stroke [B_polygone]; $noeud_partage Ajouter_fils $noeud_root_pour_stroke
 set noeud_position_pour_stroke [B_noeud]; 
   $noeud_root_pour_stroke Ajouter_fils $noeud_position_pour_stroke
   $noeud_position_pour_stroke Origine 120 90
 set poly_pour_stroke       [B_polygone]; $noeud_position_pour_stroke Ajouter_fils $poly_pour_stroke
   $poly_pour_stroke Ajouter_contour [ProcRect -50 -50 50 50]; $poly_pour_stroke Couleur 0.2 0.4 0.4 1
 
#___________________________________________________________________________________________________________________________________________
# A FAIRE : PLACER UN POLYGONE non AFFICHE EN FOND POUR LE CATCHER
#           AJOUTER UN MODE POUR LE CATCHER QUI SOIT UTILE POUR LE STROKE MENU (détection contact et relache)
 Automate_stroke_menu automate_stroke $poly_pour_stroke stroke_menu_item_root
 set poly_fond [B_polygone]; $poly_fond Ajouter_contour [ProcRect 0 0 2000 2000]; $poly_fond Afficher_noeud 0
   #XXX [N_i_mere Noeud_scene] Ajouter_fils $poly_fond
   set rap_end_ptr [B_rappel [Interp_TCL]]; $rap_end_ptr Texte "Undisplay_menu_if_release $rap_end_ptr $noeud_root_pour_stroke"
   set rap_poly_fond [B_rappel [Interp_TCL]]; $rap_poly_fond Texte "if {\[catch {Place_menu $noeud_root_pour_stroke automate_stroke $rap_poly_fond $rap_end_ptr} err\]} {puts \"Error in menu stroke placement:\n\$err\"}"
   $poly_fond abonner_a_detection_pointeur [$rap_poly_fond Rappel] [ALX_pointeur_enfonce]
   $noeud_root_pour_stroke Afficher_noeud 0; $noeud_root_pour_stroke Gerer_contacts 0
 proc Undisplay_menu_if_release {rap n} {
   set ptr [Void_vers_alx_pointeur [$rap Param]]
   if {[$ptr Appuie] == 0} {
     $n Afficher_noeud 0; $n Gerer_contacts 0
	 $ptr desabonner_de_changement [$rap Rappel]
	}
  }
  
 proc Place_menu {n automate rap rap_end} {
   if {[$automate get_current_ptr] == ""} {
     set root [$automate get_n_root]
     $root Origine 0 0
	 set infos [Void_vers_info [$rap Param]]
	 $n Afficher_noeud 1; $n Gerer_contacts 1
	 $n Origine [expr [$infos X_au_contact]-120] [expr [$infos Y_au_contact]-90]
	 $automate Start_mode_expert
	 [$infos Ptr] abonner_a_changement [$rap_end Rappel]
    } else {puts "$automate is still in use ?"}
  }
 
 
#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
source test_mutable.tcl
  Liant_Mutable_sim_ptr Liant_mutable; Liant_mutable startListening 8910
if {0} {
source sim_mutable.tcl
  Sim_MuTable sim_table localhost 8910
  sim_table Send_infos 20
}

#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
source Pointer_catcher.tcl

#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
Pointer_catcher pointeur_catcher $noeud_fen $poly_pour_main automate_main
#XXX Pointer_catcher pointeur_catcher $noeud_fen $noeud_root_pour_stroke automate_stroke

source Expe_menu_main_class.tcl
  Expe_menu_main expe $poly_pour_main automate_main $L_item_to_find
  #XXX Expe_menu_main expe $noeud_root_pour_stroke automate_stroke $L_item_to_find
  $n_root Ajouter_fils [expe get_n_root]
  expe Start
   
#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
source Tracer.tcl
  Tracer tracer
  tracer Insert_tracer_in_L Expe_menu_main     [list Start Next Finish set_current_target Propose]
  tracer Insert_tracer_in_L Automate_quickmenu [list Start_mode_expert Stop_mode_expert Info_Goto_sub_menu_index]
  puts "Enter a new user description trace\ntracer Add_trace_new_user name sex age experienced(0 or 1)"
  
  set top ""
  frame $top.f; pack $top.f -expand 1 -fill both
    frame $top.f.infos; pack $top.f.infos -expand 1 -fill both
	  frame $top.f.infos.name; pack $top.f.infos.name -expand 1 -fill x
	    label $top.f.infos.name.label_name -text "Name : "; pack $top.f.infos.name.label_name -side left -expand 0 -fill none
	    entry $top.f.infos.name.entry_name -textvariable current_user_name; pack $top.f.infos.name.entry_name -expand 1 -fill x
	  frame $top.f.infos.sex_age_exp; pack $top.f.infos.sex_age_exp -expand 1 -fill x
	    tk_optionMenu $top.f.infos.sex_age_exp.sex "current_user_sex" ARGHHH; pack $top.f.infos.sex_age_exp.sex -side left -expand 1 -fill x 
		  $top.f.infos.sex_age_exp.sex.menu delete 0 last
		  $top.f.infos.sex_age_exp.sex configure -textvariable var_current_user_sex
		  $top.f.infos.sex_age_exp.sex.menu add radiobutton -label Male   -command "set var_current_user_sex Male"
		  $top.f.infos.sex_age_exp.sex.menu add radiobutton -label Female -command "set var_current_user_sex Female"
		label $top.f.infos.sex_age_exp.label_age -text "Age : "; pack $top.f.infos.sex_age_exp.label_age -side left -expand 0 -fill none
		entry $top.f.infos.sex_age_exp.entry_age -textvariable current_user_age; pack $top.f.infos.sex_age_exp.entry_age -side left -expand 1 -fill x
		label $top.f.infos.sex_age_exp.label_exp -text "Experienced : "; pack $top.f.infos.sex_age_exp.label_exp -side left -expand 0 -fill none
		entry $top.f.infos.sex_age_exp.entry_exp -textvariable current_user_exp; pack $top.f.infos.sex_age_exp.entry_exp -side left -expand 1 -fill x
      frame $top.f.infos.rest; pack $top.f.infos.rest -expand 1 -fill both
		set L_q [list "Getting_Started" "Speed_of_use" "Precision" "Learnability" "Satisfaction" "Fatigue"]
		foreach q $L_q {
		  frame $top.f.infos.rest._$q; pack $top.f.infos.rest._$q -expand 1 -fill x
		  label $top.f.infos.rest._$q.label -text "$q : "; pack $top.f.infos.rest._$q.label -side left -expand 0 -fill none
		  entry $top.f.infos.rest._$q.entry -textvariable current_user_$q; pack $top.f.infos.rest._$q.entry -side left -expand 1 -fill x
		 }
    frame $top.f.cmd; pack $top.f.cmd -expand 1 -fill x
	  button $top.f.cmd.start -text "Start" -command "tracer Add_trace ______________________________; tracer Save_to_file ./Expe_menus_trc/TMP.trc; tracer set_L_traces {}; tracer Add_trace_new_user \$current_user_name \$var_current_user_sex \$current_user_age \$current_user_exp; expe Start"; pack $top.f.cmd.start -side left -expand 1 -fill x
	  button $top.f.cmd.save  -text "Save"  -command "foreach q {$L_q} {set v \"\\\$current_user_\"; append v \$q; tracer Add_trace \"\$q \[subst \$v\]\"}; tracer Save_to_file ./Expe_menus_trc/\${current_user_name}.trc"; pack $top.f.cmd.save -side left -expand 1 -fill x

puts "Loading COMPLETED"

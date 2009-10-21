if {[info exists noeud_partage]} {} else {source groupe.tcl}
proc etire_fond args {}

  $f_obs Vider_peres
  
source paint.tcl

puts "Sourcing automates"
source quickmenu_mouse.tcl
source quickmenu_multitouch_tabbing.tcl
#source quickmenu_multitouch_holding.tcl
source quickmenu_multitouch_tab_and_hold.tcl

source stroke_menu.tcl
source multi_stroke_menu.tcl

source menu_main.tcl

puts "END Sourcing automates"

source evaluation.tcl

#source test_usb_mouse.tcl
#N_i_mere Cacher_souris
N_i_mere Afficher_souris

$fen_editeur_tcl Vider_peres

#___________________________________________________________________________________________________________________________________________
# Create the menu
set root_drawing_all [B_noeud]
  $noeud_partage Ajouter_fils $root_drawing_all
  $root_drawing_all Position_des_fils_changeable 0
set root_input [B_noeud]
  $root_input Position_des_fils_changeable 0
  $root_drawing_all Ajouter_fils $root_input
  set input_region [B_polygone]
	$root_input Ajouter_fils $input_region
	B_configure $input_region -Ajouter_contour [ProcRect 0 0 300 300] -Couleur 0.5 0.5 0.5 0.5 -Origine 0 0
  set handle_input_region [B_polygone]
	$root_input Ajouter_fils $handle_input_region
	B_configure $handle_input_region -Ajouter_contour [ProcRect 0 0 300 50] -Couleur 0.2 0.4 1 1 -Origine 0 300
    set input_title_text [B_texte 400 50 30 [fonte_Arial] [B_sim_sds]]
	$handle_input_region Ajouter_fils $input_title_text
	B_configure $input_title_text -Editable 0 -Noeud_touchable 0
	
B_contact ctc_input "$root_input 0" -add "$handle_input_region 1"

# Create the canves
set canvas [B_polygone]
	$root_drawing_all Ajouter_fils $canvas
	B_configure $canvas -Ajouter_contour [ProcRect 0 0 1100 1024] -Couleur 0.2 0.2 0.2 0.2 -Origine 0 0


Automate_paint automate_drawer $canvas

proc Update_title {txt} {
 global input_title_text
 $input_title_text Maj_texte $txt
}

#___________________________________________________________________________________________________________________________________________

Menu_item cmd_reset		"Top Menu"	{$Automate_menu Back}
Menu_item cmd_empty		Invalid		{}
		
Menu_item menu_Edit "Edit" "Update_title Edit" "Touch object to apply \nedit command..." \
	[list	[Menu_item cmd_select		"Copy"				{Update_title Copy; automate_drawer Deselect_all; automate_drawer Activate_mode "Multiselect"}			"Touch objects you want to \ncopy to select them."] \
			[Menu_item cmd_paste		"Paste"				{Update_title Paste; automate_drawer Activate_mode "Copy"}												"Touch canvas to create \ncopy of selected objects."] \
			[Menu_item cmd_delete		"Delete object"		{Update_title "Delete object"; automate_drawer Deselect_all; automate_drawer Activate_mode "Delete"}				"Touch object to delete it."] \
			[Menu_item cmd_clear		"Clear canvas"		{Update_title "Clear canvas"; automate_drawer Activate_mode "Clear_canvas"}										"Touch canvas to clear it."] \
			 cmd_reset]

Menu_item menu_Shape "Shape" "Update_title Shape" "Touch canvas to \ndraw shape..." \
	[list	[Menu_item cmd_polygone		"Freehand drawing"	{Update_title "Freehand drawing"; automate_drawer Deselect_all; automate_drawer Activate_mode "Draw polygon"}		"Create a 'freehand' drawing."] \
			[Menu_item cmd_rectangle	"Rectangle"			{Update_title Rectangle; automate_drawer Deselect_all; automate_drawer Activate_mode "Draw rectangle"}		"Create a rectangle."] \
			[Menu_item cmd_oval			"Oval"				{Update_title Oval; automate_drawer Deselect_all; automate_drawer Activate_mode "Draw oval"}			"Create an oval."] \
			[Menu_item cmd_triangle		"Triangle"			{Update_title Triangle; automate_drawer Deselect_all; automate_drawer Activate_mode "Draw triangle"}		"Create a triangle."] \
			 cmd_reset]

Menu_item menu_Colour "Colour" "Update_title Colour" "Touch shape to change \nits colour..." \
	[list	[Menu_item cmd_red			"Brown"			{Update_title Brown; automate_drawer Deselect_all; automate_drawer Activate_mode "Change_colour 1 0.5 0"}		"Touch shape to change \nits colour."] \
			[Menu_item cmd_green		"Green"			{Update_title Green; automate_drawer Deselect_all; automate_drawer Activate_mode "Change_colour 0 0.5 0"}		"Touch shape to change \nits colour."] \
			[Menu_item cmd_blue			"Blue"			{Update_title Blue; automate_drawer Deselect_all; automate_drawer Activate_mode "Change_colour 0 0 0.8"}		"Touch shape to change \nits colour."] \
			[Menu_item cmd_yellow		"Yellow"		{Update_title Yellow; automate_drawer Deselect_all; automate_drawer Activate_mode "Change_colour 1 1 0"}			"Touch shape to change \nits colour."] \
			 cmd_reset]
			 
Menu_item menu_Difference "Difference" "Update_title Difference" "Draw shape on top of selected \nshape to create hole in it..." \
	[list	[Menu_item cmd_diff_polygone			"Freehand drawing"		{Update_title "Freehand drawing"; automate_drawer Activate_mode "Draw polygon Difference"}			"Create a 'freehand' hole in \nthe selected shape."] \
			[Menu_item cmd_diff_rectangle			"Rectangle"				{Update_title Rectangle; automate_drawer Activate_mode "Draw rectangle Difference"}			"Create a rectangular hole in \nthe selected shape."] \
			[Menu_item cmd_diff_oval				"Oval"					{Update_title Oval; automate_drawer Activate_mode "Draw oval Difference"}				"Create an oval hole in \nthe selected shape."] \
			[Menu_item cmd_diff_triangle			"Triangle"				{Update_title Triangle; automate_drawer Activate_mode "Draw triangle Difference"}			"Create a triangular hole in \nthe selected shape."] \
			 cmd_reset]
			 
Menu_item menu_Union "Union" "Update_title Union" "Draw shape to unite it with \nselected shape..." \
	[list	[Menu_item cmd_union_polygone			"Freehand drawing"		{Update_title "Freehand drawing"; automate_drawer Activate_mode "Draw polygon Union"}				"Unite a 'freehand' shape with \nthe selected shape."] \
			[Menu_item cmd_union_rectangle			"Rectangle"				{Update_title Rectangle; automate_drawer Activate_mode "Draw rectangle Union"}				"Unite a rectangular shape with \nthe selected shape."] \
			[Menu_item cmd_union_oval				"Oval"					{Update_title Oval; automate_drawer Activate_mode "Draw oval Union"}					"Unite an oval shape with \nthe selected shape."] \
			[Menu_item cmd_union_triangle			"Triangle"				{Update_title Triangle; automate_drawer Activate_mode "Draw triangle Union"}				"Unite a triangular shape with \nthe selected shape."] \
			 cmd_reset]
			 
Menu_item menu_Intersection "Intersection" "Update_title Intersection" "Draw shape on top of selected \nshape, the intersection is kept..." \
	[list	[Menu_item cmd_intersect_polygone		"Freehand drawing"		{Update_title "Freehand drawing"; automate_drawer Activate_mode "Draw polygon Intersection"}			"Intersect the selected shape \nand a 'freehand' shape."] \
			[Menu_item cmd_intersect_rectangle		"Rectangle"				{Update_title Rectangle; automate_drawer Activate_mode "Draw rectangle Intersection"}		"Intersect the selected shape \nand a rectangular shape."] \
			[Menu_item cmd_intersect_oval			"Oval"					{Update_title Oval; automate_drawer Activate_mode "Draw oval Intersection"}			"Intersect the selected shape \nand an oval shape."] \
			[Menu_item cmd_intersect_triangle		"Triangle"				{Update_title Triangle; automate_drawer Activate_mode "Draw triangle Intersection"}		"Intersect the selected shape \nand a triangular shape."] \
			 cmd_reset]
			 
Menu_item menu_XOR "XOR" "Update_title XOR" "Draw shape on top of selected \nshape, the exclusive OR is kept..." \
	[list	[Menu_item cmd_xor_polygone				"Freehand drawing"		{Update_title "Freehand drawing"; automate_drawer Activate_mode "Draw polygon XOR"}					"XOR a 'freehand' shape and \nthe selected shape."] \
			[Menu_item cmd_xor_rectangle			"Rectangle"				{Update_title Rectangle; automate_drawer Activate_mode "Draw rectangle XOR"}				"XOR a rectangular shape and \nthe selected shape."] \
			[Menu_item cmd_xor_oval					"Oval"					{Update_title Oval; automate_drawer Activate_mode "Draw oval XOR"}						"XOR an oval shape and \nthe selected shape."] \
			[Menu_item cmd_xor_triangle				"Triangle"				{Update_title Triangle; automate_drawer Activate_mode "Draw triangle XOR"}					"XOR a triangular shape and \nthe selected shape."] \
			 cmd_reset]
	
Menu_item menu_Operation "Operation" {Update_title Operation; automate_drawer Activate_mode "Multiselect"} "Touch shape you want \nto manipulate..." \
	[list	menu_Difference \
			menu_Union \
			menu_Intersection \
			menu_XOR \
			cmd_reset]
			
Menu_item menu_Root "Top menu" "" "Choose a submenu..." \
	[list	menu_Shape \
			menu_Colour \
			menu_Operation \
			menu_Edit \
			cmd_reset]

Menu_item menu_Root_Large "Top menu" "" "Choose a submenu..." \
                          [list cmd_polygone cmd_rectangle   cmd_oval          cmd_triangle     \
						        cmd_red      cmd_green       cmd_blue          cmd_yellow       \
								cmd_select   cmd_paste       cmd_delete        cmd_clear        \
								menu_Union   menu_Difference menu_Intersection menu_XOR         \
						  ]
  
# Automate_quickmenu_mouse / Automate_quickmenu_multitouch_tabbing / Automate_quickmenu_multitouch_holding / Automate_quickmenu_multitouch_tab_and_hold
# Automate_stroke_menu / Automate_multi_stroke_menu
# Automate_Menu_Main
puts "Creating automate"
  Automate_stroke_menu                       automate_input_CS $input_region menu_Root; automate_input_CS UnActivate
  Automate_multi_stroke_menu                 automate_input_MS $input_region menu_Root; automate_input_MS UnActivate
  Automate_quickmenu_mouse                   automate_input_MO $input_region menu_Root; automate_input_MO UnActivate
  Automate_quickmenu_multitouch_tab_and_hold automate_input_TP $input_region menu_Root; automate_input_TP UnActivate
  Automate_Menu_Main                         automate_input_MA $input_region menu_Root_Large; automate_input_MA UnActivate
  
puts "END Creating automate"

  
set output_window [B_fiche "Current task" 220 300 [N_i_mere]]
	$Noeud_scene Ajouter_fils_au_debut $output_window
set output_region [B_polygone]
	$output_window Ajouter_fils $output_region
	B_configure $output_region -Ajouter_contour [ProcRect 0 0 220 300] -Couleur 0.3 0.5 0.3 0.5

Experiment test $output_region "test_case_menus.xml"

proc Task1_menus {} {
 menu_Difference   set_L_subitem ""
 menu_Union        set_L_subitem ""
 menu_Intersection set_L_subitem ""
 menu_XOR          set_L_subitem ""
}

proc Task2_menus {} {
 menu_Difference   set_L_subitem [list cmd_diff_polygone cmd_diff_rectangle cmd_diff_oval cmd_diff_triangle cmd_reset]
 menu_Union        set_L_subitem [list cmd_union_polygone cmd_union_rectangle cmd_union_oval cmd_union_triangle cmd_reset]
 menu_Intersection set_L_subitem [list cmd_intersect_polygone cmd_intersect_rectangle cmd_intersect_oval cmd_intersect_triangle cmd_reset]
 menu_XOR          set_L_subitem [list cmd_xor_polygone cmd_xor_rectangle cmd_xor_oval cmd_xor_triangle cmd_reset]
}

#___________________________________________________________________________________________________________________________________________
method Experimentator constructor {} {
 set this(num_exp)  0
 set this(automate) ""
 set this(automate_id) 0
 set this(user_order) ""
 set this(user_id) 0
 set this(Latin_square) [list "1 2 3 4" "2 4 1 3" "3 1 4 2" "4 3 2 1"]
 set this(L_automates) [list automate_input_MA automate_input_CS automate_input_MS automate_input_MO automate_input_TP]
 
 this Next_user
}

Generate_accessors Experimentator [list num_exp user_id user_order automate_id automate]

method Experimentator Activate_automate {a} {
 set this(automate) $a
   if {[catch "$a Activate" err]} {puts "$objName Activate_automate $a ERROR\n$err"}
}
method Experimentator Next_automate {} {
 if {![string equal $this(automate) ""]} {$this(automate) UnActivate}
 set i [lindex $this(user_order) $this(automate_id)]
 this Activate_automate [lindex $this(L_automates) [expr $i-1]]
   
 set this(automate_id) [expr ($this(automate_id) + 1) % [llength $this(L_automates)]]
}

method Experimentator Next_user {} {
 set length_latin [llength $this(Latin_square)]
 set this(user_order) [lindex $this(Latin_square) [expr $this(user_id) % $length_latin]]
   
 incr this(user_id)
 set this(automate_id) 0
 this Next_automate
}

#___________________________________________________________________________________________________________________________________________
source Tracer.tcl
  Tracer tracer_automate
  tracer_automate Insert_tracer_in_L Automate_quickmenu [list New_start Go_to_submenu Activate_item]
  tracer_automate Insert_tracer_in_L Experiment         [list Start_task]
  tracer_automate Insert_tracer_in_L Experimentator     [list Next_user Activate_automate]
puts coucou
Experimentator exper
test Subscribe_Test_case_finished experimentator "exper Next_automate"
test Subscribe_User_finished      experimentator "exper Next_user; puts \"Enter a new user description trace\\n  tracer_automate Add_trace_new_user name sex age experienced(0 or 1)\""

puts "Enter a new user description trace\ntracer_automate Add_trace_new_user name sex age experienced(0 or 1)"

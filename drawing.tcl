#___________________________________________________________________________________________________________________________________________
#__________________ AUXILIARIES  __________________
#___________________________________________________________________________________________________________________________________________
method Automate_drawing get_unique_id {} {
	incr this(unique_id)
	return $this(unique_id)
}

#___________________________________________________________________________________________________________________________________________
method Automate_drawing get_info_from_rap {rap} {
	set p [$rap Param]
	return [Void_vers_info $p]
}

#___________________________________________________________________________________________________________________________________________
proc Generate_accessors {class_name L_vars} {
	foreach v $L_vars {
		set    	cmd "method $class_name get_$v \{ \} \{"
		append 	cmd {return $this(} $v ")\}"
		eval 	$cmd
		set    	cmd "method $class_name set_$v \{v\} \{"
		append 	cmd {set this(} $v ") \$v\}"
		eval 	$cmd
	}
}



#___________________________________________________________________________________________________________________________________________
#__________________ CONSTRUCTOR  __________________
#___________________________________________________________________________________________________________________________________________
method Automate_drawing constructor {canvas} {
	set this(rap_ENTER)		[B_rappel [Interp_TCL] "$objName Enter_canvas"]
	set this(rap_EXIT)		[B_rappel [Interp_TCL] "$objName Exit_canvas"]
	set this(rap_ACTION)	[B_rappel [Interp_TCL]]
		$this(rap_ACTION)	Texte "$objName Move_in_canvas $this(rap_ACTION)"
 
	set this(canvas)          $canvas
		$canvas abonner_a_detection_pointeur [$this(rap_ENTER)   Rappel] [ALX_pointeur_apparition]  
		$canvas abonner_a_detection_pointeur [$this(rap_EXIT)    Rappel] [expr [ALX_pointeur_disparition]+[Pas_rappel_si_fils_contient]]
		$canvas abonner_a_detection_pointeur [$this(rap_ACTION)  Rappel] [expr [ALX_pointeur_mouvement]+[ALX_pointeur_enfonce]+[ALX_pointeur_relache]]
 
	set this(single_selection)			TRUE
	set this(allow_deselect)			FALSE
	
	set this(last_selected_object)		""
	set this(selected_objects)			""
	set this(manipulate_buffer)			""
  
	set this(mode)						""
	set this(type_of_polygon_to_draw)	""
	set this(type_of_polygon_operation)	""
 
	set this(unique_id)	0
}

#___________________________________________________________________________________________________________________________________________
Generate_accessors Automate_drawing [list rap_ENTER rap_EXIT rap_ACTION canvas last_selected_object selected_objects mode]

#___________________________________________________________________________________________________________________________________________
method Automate_drawing dispose {} {
	this inherited
}



#___________________________________________________________________________________________________________________________________________
#__________________ INTERFACE  __________________
#___________________________________________________________________________________________________________________________________________
method Automate_drawing Activate_command {} {
}

#___________________________________________________________________________________________________________________________________________
method Automate_drawing Activate_mode {} {
}

#___________________________________________________________________________________________________________________________________________
method Automate_drawing Activate_command_and_mode {} {
}

#___________________________________________________________________________________________________________________________________________
method Automate_drawing Reset {} {
	# set default values
	set this(mode)						""
	set this(type_of_polygon_to_draw)	""
	set this(type_of_polygon_operation)	""
	
	set this(single_selection)			TRUE
	set this(allow_deselect)			FALSE
 
	# we can select and manipulate objects on the canvas
	$this(canvas) Contenu_dans_fils_ssi_contenu_dans_noeud 0
	$this(canvas) Contenu_dans_fils_ssi_pas_contenu_dans_noeud 0
}

#___________________________________________________________________________________________________________________________________________
#__________________ AUTOMATE  __________________
#___________________________________________________________________________________________________________________________________________
method Automate_drawing Enter_canvas {} {
	#puts "$objName Enter_canvas"
	set i [this get_info_from_rap $this(rap_ENTER)]
	set ptr [$i Ptr]
	if {![info exists this(state,$ptr)]} {
		set this(state,$ptr) Inside_canvas
	} else {
		this $this(state,$ptr) $i
	}
}

#___________________________________________________________________________________________________________________________________________
method Automate_drawing Exit_canvas {rap} {
    puts "$objName Exit_canvas $rap"
	set i [this get_info_from_rap $rap]
	set ptr [$i Ptr]
	if {[info exists this(state,$ptr)]} {
		unset this(state,$ptr)
		# clear up if exit while drawing
		if {[llength $this(L_pt,$ptr)] > 0} {
			$this(poly,$ptr) Noeud_touchable 1
			B_contact [this get_unique_id] "$this(poly,$ptr) 3"
			unset this(L_pt,$ptr)
		}
	} else {
		#puts "$objName Exit_canvas => Exit but never entered..."
	}
}

#___________________________________________________________________________________________________________________________________________
method Automate_drawing Move_in_canvas {rap} {
    puts "$objName Move_in_canvas $rap"
	set i [this get_info_from_rap $rap]
	set ptr [$i Ptr]
	if {[info exists this(state,$ptr)]} {
		this $this(state,$ptr) $i
	} else {
		puts "$objName Move_in_canvas => Move but never entered..."
	}
}

#___________________________________________________________________________________________________________________________________________
method Automate_drawing Inside_canvas {infos} {
	set evt [$infos EVT]
	set ptr [$infos Ptr]
	#puts "$objName Inside_canvas : EVT([$evt Type_evt])"
	switch [$evt Type_evt] {
	1 {
		#puts "  PRESS"
		switch $this(mode) {
		"Draw" {
			# start drawing	
			set this(state,$ptr) Move_while_press_in_canvas
			set this(L_pt,$ptr)  [list [$infos X_au_contact] [$infos Y_au_contact]]
			set this(poly,$ptr)  [B_polygone]
			$this(canvas) Ajouter_fils_au_debut $this(poly,$ptr)
			$this(poly,$ptr) Noeud_touchable 0
		}
		"Copy" {
			# prevent selecting and manipulating objects on the canvas
			$this(canvas) Contenu_dans_fils_ssi_contenu_dans_noeud 		1
			$this(canvas) Contenu_dans_fils_ssi_pas_contenu_dans_noeud 	1
			
			this Paste $infos
			
			# we can select and manipulate objects on the canvas again
			$this(canvas) Contenu_dans_fils_ssi_contenu_dans_noeud		0
			$this(canvas) Contenu_dans_fils_ssi_pas_contenu_dans_noeud	0
		}
		default {
			# select polygon
			set poly  [Real_class [$infos NOEUD]]
			this Select $poly

			# execute current mode (timer to provide some feedback)
			if {![string equal $this(mode) ""]} {
				after 300 "$objName $this(mode)"
			}
		}
		}
	}
	2 {
		#puts "  RELEASE"
		set this(state,$ptr) Inside_canvas
	}
	default {}
	}
}

#___________________________________________________________________________________________________________________________________________
method Automate_drawing Move_while_press_in_canvas {infos} {
    puts "$objName Move_while_press_in_canvas $infos"
	set evt [$infos EVT]
	set ptr [$infos Ptr]
	
	#puts "$objName Mov_while_press_in_canvas : EVT([$evt Type_evt])"
	
	switch [$evt Type_evt] {
	2 {
		#puts "  RELEASE"
		switch $this(mode) {
		"Draw" {
			if {[string equal $this(type_of_polygon_operation) ""]} {
				$this(poly,$ptr) Noeud_touchable 1
				B_contact [this get_unique_id] "$this(poly,$ptr) 3"
			} else {
				if {[string equal $this(manipulate_buffer) ""]} {
					puts "!!!\n The object you are manipulating no longer exists! \n!!!"		    
					$this(last_selected_object) Vider_peres
				} else {
					$this(manipulate_buffer) $this(type_of_polygon_operation) $this(last_selected_object)
					$this(last_selected_object) Vider_peres
					this Select $this(manipulate_buffer)
				}
			}
		}
		}
		unset this(L_pt,$ptr)
		set   this(state,$ptr) Inside_canvas
	}
	64 {
		#puts "  MVT"
		switch $this(mode) {
		"Draw" {
			this Create_polygon $this(type_of_polygon_to_draw) $infos
		}
		}
	}
	32 {
		#puts "  EXIT"
	}
	default {}
	}
}



#___________________________________________________________________________________________________________________________________________
#__________________ CREATING POLYGONS  __________________
#___________________________________________________________________________________________________________________________________________
method Automate_drawing Draw {type_of_polygon} {
	set this(mode)						"Draw"
	set this(type_of_polygon_to_draw)	$type_of_polygon

	# prevent selecting and manipulating objects on the canvas
	$this(canvas) Contenu_dans_fils_ssi_contenu_dans_noeud 1
	$this(canvas) Contenu_dans_fils_ssi_pas_contenu_dans_noeud 1
}

#___________________________________________________________________________________________________________________________________________
method Automate_drawing Create_polygon {type_of_polygon infos} {
	# get info on position
	set ptr [$infos Ptr]
	set x [$infos X_au_contact]
	set y [$infos Y_au_contact]

	# empty the previous polygon
	$this(poly,$ptr) Vider

	# create a new polygon of the right type
	switch $type_of_polygon {
	polygon {
		lappend this(L_pt,$ptr) $x $y
		set contour [ProcTabDouble $this(L_pt,$ptr)]
	}
	rectangle {
		set x_origine [lindex $this(L_pt,$ptr) 0]
		set y_origine [lindex $this(L_pt,$ptr) 1]
		set contour [ProcRect $x_origine $y_origine $x $y]
	}
	oval {
		set x_origine [lindex $this(L_pt,$ptr) 0]
		set y_origine [lindex $this(L_pt,$ptr) 1]
		set contour [ProcOvale $x_origine $y_origine [expr $x-$x_origine] [expr $y-$y_origine] 60]
	}
	triangle {
		set x_origine [lindex $this(L_pt,$ptr) 0]
		set y_origine [lindex $this(L_pt,$ptr) 1]
		 
		set contour [B_TabDouble]
		$contour Reserver_place 6
		$contour Ajouter 0  $x_origine  $y_origine
		$contour Ajouter 2  $x          $y_origine 
		$contour Ajouter 4  [expr $x_origine+($x-$x_origine)/2]   [expr $y_origine+$y-$y_origine]
	}
	default {}
	}

	# add the contour and free the memory
	$this(poly,$ptr) Ajouter_contour $contour
	Detruire $contour

	# select the new polygon
	this Select $this(poly,$ptr)
}



#___________________________________________________________________________________________________________________________________________
#__________________ EDITING POLYGONS  __________________
#___________________________________________________________________________________________________________________________________________
method Automate_drawing Operation {type_of_polygon_operation} {
	set this(mode) 							"Operation $type_of_polygon_operation"
	set this(type_of_polygon_operation)		$type_of_polygon_operation

	# keep a reference to the current selection
	set this(manipulate_buffer) $this(last_selected_object)

	# prevent selecting and manipulating objects on the canvas
	$this(canvas) Contenu_dans_fils_ssi_contenu_dans_noeud 1
	$this(canvas) Contenu_dans_fils_ssi_pas_contenu_dans_noeud 1
}

#___________________________________________________________________________________________________________________________________________
method Automate_drawing Change_colour {r g b} {
	set this(mode) "Change_colour $r $g $b"
	
	# change the colour of the current selection
	if {![string equal $this(selected_objects) ""]} {
		foreach e $this(selected_objects) {
			$e Couleur $r $g $b [$e Couleur 3]
		}
	}
}



#___________________________________________________________________________________________________________________________________________
#__________________ COPY/PASTING  __________________
#___________________________________________________________________________________________________________________________________________
method Automate_drawing Copy {} {
	set this(mode) "Copy"
}

#___________________________________________________________________________________________________________________________________________
# Correction :
#   Use BG_X and BG_Y to get the coordinates of the bottom (Bas)  left  (Gauche) corner of the bounding box
#   Use HD_X and HD_Y to get the coordinates of the top    (Haut) right (Droite) corner of the bounding box
# Retrieve the bounding box of the shapes expressed with respect to their parent 
#   => use Boite_noeud_et_fils_glob (get the bounding box of the node and its daughters "globally" / globallement == with respect to the parent node)
# Compute a displacement vector instead of centering every pasted element on the cliqued point
method Automate_drawing Paste {infos} {
	# get the position of the event on the canvas
	set x [$infos X_au_contact]
	set y [$infos Y_au_contact]
    puts "Paste:n  - this(selected_objects) : $this(selected_objects)\n  - point : $x $y"
	if {![string equal $this(selected_objects) ""]} {
		# search for bounding box of entire selection
		set bbox [$this(last_selected_object) Boite_noeud_et_fils_glob]
		set min_x [$bbox BG_X]; set min_y [$bbox BG_Y];
		set max_x [$bbox HD_X]; set max_y [$bbox HD_Y];
	
	   # Compute the bounding box of the list of selected elements	
		foreach e $this(selected_objects) {
			set bbox [$e Boite_noeud_et_fils_glob]
			set x1 [$bbox BG_X]; set y1 [$bbox BG_Y];
			set x2 [$bbox HD_X]; set y2 [$bbox HD_Y];
			set min_x [expr $x1<$min_x?$x1:$min_x]
			set min_y [expr $y1<$min_y?$y1:$min_y]
			set max_x [expr $x2>$max_x?$x2:$max_x]
			set max_y [expr $y2>$max_y?$y2:$max_y]
		}
	   # Compute the center of the bbox and the displacement vectorfrom this center to the cliqued point
	    set CX     [expr ($min_x + $max_x) / 2.0]; set CY     [expr ($min_y + $max_y) / 2.0]
		set vect_x [expr $x - $CX]               ; set vect_y [expr $y - $CY]
	   
	   # For each object of the selected list, make a copy and move the copy using 	the vector of displacement (vect_x, vect_y)
		foreach e $this(selected_objects) {
			# create a new copy
			set new_copy [B_polygone]
			$this(canvas) Ajouter_fils_au_debut $new_copy
			$new_copy Union $e
			B_contact [this get_unique_id] "$new_copy 3"
	        $new_copy Translation $vect_x $vect_y
			# set new position (use difference with minimum to keep relative positions)
			#$new_copy Origine [expr $x - ($max_x - $min_x)/2 + ($x1 - $min_x)] [expr $y - ($max_y - $min_y)/2 + ($y1 - $min_y)]
		}
	}
}



#___________________________________________________________________________________________________________________________________________
#__________________ DELETING POLYGONS  __________________
#___________________________________________________________________________________________________________________________________________
method Automate_drawing Delete {} {
	set this(mode) "Delete"

	# clear buffer if object is being deleted
	if {[string equal $this(last_selected_object) $this(manipulate_buffer)]} {
		set this(manipulate_buffer) ""
	}
 
	# delete the current selection
	if {![string equal $this(selected_objects) ""]} {
		foreach e $this(selected_objects) {
			$e Vider_peres
		}
	}
	set this(selected_objects)		""
	set this(last_selected_object)	""
}

#___________________________________________________________________________________________________________________________________________
method Automate_drawing Clear_canvas {} {
	set this(mode) Clear_canvas
	
	# reset selected objects
	set this(selected_objects)		""
	set this(last_selected_object)	""
	set this(manipulate_buffer)		""
 
	# clear the canvas
	$this(canvas) Vider_fils
}



#___________________________________________________________________________________________________________________________________________
#__________________ SELECTING POLYGONS  __________________
#___________________________________________________________________________________________________________________________________________
method Automate_drawing Multiselect {} {
	set this(mode) Multiselect
	
	# set parameters of the drawing module
	set this(single_selection)	FALSE
	set this(allow_deselect)	TRUE
	
	# we can select and manipulate objects on the canvas
	$this(canvas) Contenu_dans_fils_ssi_contenu_dans_noeud		0
	$this(canvas) Contenu_dans_fils_ssi_pas_contenu_dans_noeud	0
}

#___________________________________________________________________________________________________________________________________________
method Automate_drawing Select {object} { 
	# if canvas selected, deselect current selection
	if {[string equal $this(canvas) $object]} {
		this Deselect_all
		return
	}
	
	# is the object already selected?
	set index [lsearch $this(selected_objects) $object]
	if {$index != -1} {
		if {$this(allow_deselect)} {
			# deselect the selected object
			this Deselect $object
		} else {
			# update the last selected object
			set this(last_selected_object) $object
		}
		return
	}

	# select only one object => deselect others
	if {$this(single_selection)} {
		this Deselect_all
	}

	# add object to the list
	lappend this(selected_objects) $object
	
	# save the last selected object
	set this(last_selected_object) $object

	# feedback on selection
	$this(last_selected_object) Couleur 3 0.5
}

#___________________________________________________________________________________________________________________________________________
method Automate_drawing Deselect {object} { 
	# reset feedback
	$object Couleur 3 1

	# remove from list of selected objects
	set index [lsearch $this(selected_objects) $object]
	set this(selected_objects) [lreplace $this(selected_objects) $index $index]
	set this(last_selected_object) "" 
}

#___________________________________________________________________________________________________________________________________________
method Automate_drawing Deselect_all {} { 
	# call Deselect for each object
	if {![string equal $this(selected_objects) ""]} {
		foreach e $this(selected_objects) {
			$e Couleur 3 1
		}
	}
	
	set this(last_selected_object)	"" 
	set this(selected_objects)		""
}



#___________________________________________________________________________________________________________________________________________
#__________________ ANNEXE __________________
#___________________________________________________________________________________________________________________________________________
proc Debug_with_trace {c} {
 set L_txt [split [gmlObject info interface $c] "\n"]
   set L {}
   foreach txt $L_txt {
     if {[regexp "^method $c (\[_a-zA-Z0-9\]*) .*$" $txt reco m]} {lappend L $m}
	}
 foreach m $L {
   puts $m
   set body [gmlObject info body $c $m]
   set cmd "method $c $m {"
     # Puts aruments
	 set L_args [gmlObject info args $c $m]
	 append cmd $L_args
	 set L_args_call {}
	   foreach a $L_args {lappend L_args_call "\$[lindex $a 0] "}
	 append cmd "} {\n puts \"\$objName $m $L_args_call\"\n$body}"
   eval $cmd
  }
}

#___________________________________________________________________________________________________________________________________________
#Debug_with_trace Automate_drawing

#  set    cmd {set p [}
#  append cmd $rap_ENTER { Param]; set infos_origine [Void_vers_info $p]; }
#  append cmd {set node [$infos_origine NOEUD]; puts "contact in ([$infos_origine X_au_contact] [$infos_origine Y_au_contact])"}
#  $rap_ENTER Texte "if {\[catch {$cmd} err\]} {puts \"ERROR in $rap_ENTER:\n\$err\"}"

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
   append cmd "\$L_empilees Init_parcours;\n"
   append cmd "while \{\[\$L_empilees Parcours_continue\]\} \{\n"
   append cmd "  set infos \[\$L_empilees Courant_dans_parcours\];\n"
   append cmd "  set noeud \[\$infos NOEUD\];\n"
   append cmd "#RAPPEL CMD\n"
   append cmd "#/RAPPEL CMD\n"
   append cmd "  \$L_empilees Suivant_dans_parcours\}\n"
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

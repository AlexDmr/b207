if {[info exists noeud_partage]} {} else {source groupe.tcl}
proc etire_fond args {}

catch "package require xml"

#___________________________________________________________________________________________________________________________________________
proc Generate_accessors {class_name L_vars} {
 foreach v $L_vars {
   set    cmd "method $class_name get_$v \{ \} \{"
   append cmd {return $this(} $v ")\}"
     eval $cmd
   set    cmd "method $class_name set_$v \{v\} \{"
   append cmd {set this(} $v ") \$v\}"
     eval $cmd
  }
}

#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
method Test_case constructor {{title ""} {description ""} {L_tasks {}} {pick_random_tasks -1} {repeats 1}} {
	set this(title)				$title
	set this(description)		$description
	set this(L_tasks)			$L_tasks
	
	set this(pick_random_tasks)	$pick_random_tasks
	set this(repeats)			$repeats
}

#___________________________________________________________________________________________________________________________________________
Generate_accessors Test_case [list title description L_tasks pick_random_tasks repeats]

#___________________________________________________________________________________________________________________________________________
method Test_case Has_sub_tasks { } {return [expr [llength $this(L_tasks)] > 0]}

#___________________________________________________________________________________________________________________________________________
method Test_case Get_task_num {n} {return [lindex $this(L_tasks) $n]}


#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
method Task constructor {{title ""} {description ""} {image ""}} {
	set this(title)			$title
	set this(description)	$description
	set this(image)			$image
	set this(menu)          ""
}

#___________________________________________________________________________________________________________________________________________
Generate_accessors Task [list title description image menu]


#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
method Experiment constructor {output_window {L_files_with_test_cases {}} {L_test_cases {}}} {
	set this(output_window) $output_window
	
	set this(L_files_with_test_cases)		$L_files_with_test_cases
	set this(L_test_cases)					$L_test_cases
	set this(L_todo)						{}
	
	set this(unique_id)						0
	set this(number_of_users)				8
	set this(number_of_tests_completed) 	0
	set this(next_test_case) 				0
	set this(number_of_repeats_to_do) 		0
	
	set Latin_square [list "1 2 3 4" "2 4 1 3" "3 1 4 2" "4 3 2 1"]
	
	set this(list_of_tasks) 		{}
	set this(random_list_of_tasks)	{}
 
	set this(parser) [xml::parser]
	$this(parser) configure -elementstartcommand "$objName EStart" -elementendcommand "$objName EEnd" -characterdatacommand "$objName CData"
	
	foreach e $this(L_files_with_test_cases) {
		this Read_from_file $e
	}
	
	set this(rap_TAB_press)     [B_rappel [Interp_TCL] "$objName Go_to_next_test_case"]
	  $this(output_window) abonner_a_detection_pointeur [$this(rap_TAB_press) Rappel] [ALX_pointeur_enfonce]
	
	set this(n_root)			[B_noeud]
	set this(text_title)		[B_texte 100 100 18 [fonte_Arial] [B_sim_sds]]; this Set_text_title			"Click here to start...";
	set this(text_description)	[B_texte 100 100 18 [fonte_Arial] [B_sim_sds]]; this Set_text_description   "...the test.";
	set this(image) 			[B_image]
	
	B_configure $this(text_title)		-Editable 0 -Couleur_texte	0	1	1	1	-Noeud_touchable 0
	B_configure $this(text_description)	-Editable 0 -Couleur_texte	1	1	1	1	-Noeud_touchable 0
	
	B_configure $this(n_root) -Ajouter_fils $this(text_description) -Ajouter_fils $this(text_title) -Ajouter_fils $this(image)
	$this(output_window) Ajouter_fils $this(n_root)
	
	this Display 0 0
	
# CB
 set this(L_CB_Test_case_finished) ""
 set this(L_CB_User_finished)      ""
}

#___________________________________________________________________________________________________________________________________________
Generate_accessors Experiment [list L_files_with_test_cases L_test_cases next_test_case number_of_users number_of_tests_completed]

#___________________________________________________________________________________________________________________________________________
method Experiment dispose {} {
	this inherited
	$this(parser) free
}

#___________________________________________________________________________________________________________________________________________
method Experiment get_a_unique_id {} {
 incr this(unique_id)
 return $this(unique_id)
}

#___________________________________________________________________________________________________________________________________________
method Experiment Set_random_tasks {n} {
	set this(random_list_of_tasks) {}
	set temp_list [[lindex $this(L_test_cases) $this(next_test_case)] get_L_tasks]
	for {set i 0} {$i < $n} {incr i} {
		# select item randomly
		set r [expr int(rand()*[llength $temp_list])]
		lappend this(random_list_of_tasks) [lindex $temp_list $r]
		
		# remove so we cannot randomly select the same item twice
		set temp_list [lreplace $temp_list $r $r]
		if {[llength $temp_list] <= 0} {
			set temp_list [[lindex $this(L_test_cases) $this(next_test_case)] get_L_tasks]
		}
	}
}

#___________________________________________________________________________________________________________________________________________
method Experiment Set_list_to_latin_square {} {
	set L_todo {}
	for {set i 0} {$i < $this(number_of_users)} {incr i} {lappend L_todo [expr $i%4]}
	
	return L_todo
}

#___________________________________________________________________________________________________________________________________________
method Experiment Get_one_from_list {L_name} {
 upvar $L_name L
 set i [expr int(rand()*[llength $L])]
 set rep [lindex $L $i]
 set L [lreplace $L $i $i]
 return $rep
}

#___________________________________________________________________________________________________________________________________________
method Experiment Go_to_next_test_case {} {
	set this(list_of_tasks) {}
	
	if {$this(next_test_case) >= [llength $this(L_test_cases)]} {
		set this(next_test_case) 0
		incr this(number_of_tests_completed)
		
		#if {$this(number_of_tests_completed) >= this(number_of_users)} {}
		
		this Set_text_title			"Test completely finished...";
		this Set_text_description   "Click here to start \na new test.";
		
		$this(rap_TAB_press) Texte "puts \"Timeout... please try again in a moment...\""
		after 100 "$this(rap_TAB_press) Texte \"$objName Go_to_next_test_case\""
	} else {
		set this(number_of_repeats_to_do) 	[[lindex $this(L_test_cases) $this(next_test_case)] get_repeats]
		incr this(number_of_repeats_to_do) -1

		this Set_up_new_task_list
		
		this Set_text_title			"Test case [expr $this(next_test_case) + 1]. (id $this(number_of_tests_completed))";
		this Set_text_description   "Take a breath... \nTest the new menu...\nand prepare for a \nnew serie :)";
				
		$this(rap_TAB_press) Texte "$objName Go_to_next_task"
	}
	this Update
}

#___________________________________________________________________________________________________________________________________________
method Experiment Set_up_new_task_list {} {
	set nr_of_random_tasks				[[lindex $this(L_test_cases) $this(next_test_case)] get_pick_random_tasks]

	if {$nr_of_random_tasks > 0} {
		this Set_random_tasks $nr_of_random_tasks
		foreach t $this(random_list_of_tasks) {
			lappend this(list_of_tasks) $t
			#puts "Task: [$t get_title] -- [$t get_description]"
		}
	} else {
		foreach t [[lindex $this(L_test_cases) $this(next_test_case)] get_L_tasks] {
			lappend this(list_of_tasks) $t
			#puts "Task: [$t get_title] -- [$t get_description]"
		}
	}
}

#___________________________________________________________________________________________________________________________________________
method Experiment Start_task {task} {}

#___________________________________________________________________________________________________________________________________________
method Experiment Subscribe_Test_case_finished {id cmd} {
 foreach CB $this(L_CB_Test_case_finished) {
   set CB_id [lindex $CB 0]
   if {[string equal $id $CB_id]} {return}
  }
 lappend this(L_CB_Test_case_finished) [list $id $cmd]
}

#___________________________________________________________________________________________________________________________________________
method Experiment Test_case_finished {} {
 foreach CB $this(L_CB_Test_case_finished) {
   eval [lindex $CB 1]
  }
}

#___________________________________________________________________________________________________________________________________________
method Experiment Subscribe_User_finished {id cmd} {
 foreach CB $this(L_CB_User_finished) {
   set CB_id [lindex $CB 0]
   if {[string equal $id $CB_id]} {return}
  }
 lappend this(L_CB_User_finished) [list $id $cmd]
}

#___________________________________________________________________________________________________________________________________________
method Experiment User_finished {} {
 foreach CB $this(L_CB_User_finished) {
   eval [lindex $CB 1]
  }
}

#___________________________________________________________________________________________________________________________________________
method Experiment Go_to_next_task {} {
	if {[llength $this(list_of_tasks)] > 0} {
		#puts "Task: [[lindex $this(list_of_tasks) 0] get_title] -- [[lindex $this(list_of_tasks) 0] get_description]"
		this Set_text_title			"[[lindex $this(list_of_tasks) 0] get_title]";
		this Set_text_description   "[[lindex $this(list_of_tasks) 0] get_description]";
		this Set_image   			"[[lindex $this(list_of_tasks) 0] get_image]";
		this Update
		
		set task [lindex $this(list_of_tasks) 0]
		  this Start_task [$task get_menu]
		set this(list_of_tasks) [lreplace $this(list_of_tasks) 0 0]
		
		
		$this(rap_TAB_press) Texte "puts \"Timeout... please try again in a moment...\""
		after 100 "$this(rap_TAB_press) Texte \"$objName Go_to_next_task\""
	} elseif {$this(number_of_repeats_to_do) > 0} {
		this Set_up_new_task_list
		incr this(number_of_repeats_to_do) -1
		
		# Informs others that the test case is finished
		set title [[lindex $this(L_test_cases) $this(next_test_case)] get_title]
		if {![string equal $title INFOS]} {
		  this Test_case_finished
		 }
		
		
		this Set_text_title			"Test case [expr $this(next_test_case) + 1]. (id $this(number_of_tests_completed))";
		this Set_text_description   "Take a breath... \nTest the new menu...\nand prepare for a \nnew serie :)";
		this Update
	} else {
		set title [[lindex $this(L_test_cases) $this(next_test_case)] get_title]
		if {![string equal $title INFOS]} {
		  this Test_case_finished
		 } else {set descr [[lindex $this(L_test_cases) $this(next_test_case)] get_description]
		         if {[string equal $descr END]} {
				   this User_finished
				  } else {if {[catch "eval $descr" err]} {
				            puts "objName Go_to_next_task ERROR :\n -- descr : $descr\n$err"
						   } else {puts "Eval of \"$descr\""}
				         }
		        }
		
		
		incr this(next_test_case)
		
		$this(rap_TAB_press) Texte "$objName Go_to_next_test_case"
		
		
		this Go_to_next_test_case
		
		
	}
}

#___________________________________________________________________________________________________________________________________________
method Experiment Read_from_file {file_name} {
	set infile [open $file_name r]
	
	set content_to_parse [read $infile]
	#puts "$content_to_parse"

	$this(parser) parse $content_to_parse
	
	close $infile
}

#___________________________________________________________________________________________________________________________________________
method Experiment EStart {tag attlist args} {
    #puts "EStart - element \"$tag\" started with attributes \"$attlist\""
	array set attr $attlist
	
	switch $tag {
	"Testcase" {
		set unique_name "Test_case_[this get_a_unique_id]"
		Test_case $unique_name
		
		set attr_names [array names attr]
		foreach a $attr_names {
			switch $a {
			"title" {
				$unique_name set_title $attr(title)
			}
			"description" {
				$unique_name set_description $attr(description)
			}
			"pick_random_tasks" {
				$unique_name set_pick_random_tasks $attr(pick_random_tasks)
			}
			"repeats" {
				$unique_name set_repeats $attr(repeats)
			}
			}
		}
		
		lappend this(L_test_cases) $unique_name
	}
	"Task" {
		set unique_name "Task_[this get_a_unique_id]"
		Task $unique_name
	
		set attr_names [array names attr]
		foreach a $attr_names {
			switch $a {
			"menu" {
				$unique_name set_menu $attr(menu)
			}
			"title" {
				$unique_name set_title $attr(title)
			}
			"image" {
				$unique_name set_image $attr(image)
			}
			}
		}
		
		lappend this(list_of_tasks) $unique_name
	}
	default {puts "Error in xml: \"$tag\" not a valid start tag!!!"}
	}
}

#___________________________________________________________________________________________________________________________________________
method Experiment CData {text} {
	#puts "CData - element contains: $text"
	set txt [string trim $text]
	
	if {(![string equal $txt ""]) && ([llength $this(list_of_tasks)] > 0)} {
		[lindex $this(list_of_tasks) end] set_description "$txt"
	}
}

#___________________________________________________________________________________________________________________________________________
method Experiment EEnd {tag args} {
	#puts "EEnd - element \"$tag\""
	
	switch $tag {
	"Testcase" {
		[lindex $this(L_test_cases) end] set_L_tasks $this(list_of_tasks)
		set this(list_of_tasks) ""
	}
	"Task" {
	}
	default {puts "Error in xml: \"$tag\" not a valid end tag!!!"}
	}
}

#___________________________________________________________________________________________________________________________________________
method Experiment Display {x y} {
	$this(n_root) Afficher_noeud 1
	$this(n_root) Origine $x $y

	this Update
}

#___________________________________________________________________________________________________________________________________________
method Experiment Update {} {
	set output_box [$this(output_window) Boite_noeud]
	
	# the title goes on top
	set title_box [$this(text_title) Boite_noeud]
	set delta [expr [$output_box Ty]-[$title_box Ty]]
	$this(text_title) Origine 10 $delta
	
	# the description just bellow the title
	set description_box [$this(text_description) Boite_noeud]
	set delta [expr $delta - [$description_box Ty]]
	$this(text_description) Origine 10 $delta
	
	# put the image on the bottom
	set image_box [$this(image) Boite_noeud]
	set delta [expr $delta - [$image_box Ty]]
	$this(image) Origine 10 0
}

#___________________________________________________________________________________________________________________________________________
method Experiment UnDisplay {} {
	$this(n_root) Afficher_noeud 0
}

#___________________________________________________________________________________________________________________________________________
method Experiment Set_text_title {txt} {
	$this(text_title) Maj_texte $txt
	$this(text_title) Optimiser_dimensions
	$this(text_title) Calculer_boites
}

#___________________________________________________________________________________________________________________________________________
method Experiment Set_text_description {txt} {
	$this(text_description) Maj_texte $txt
	$this(text_description) Optimiser_dimensions
	$this(text_description) Calculer_boites
}

#___________________________________________________________________________________________________________________________________________
method Experiment Set_image {image} {
	if {(![string equal $image ""])} {
		# set new image
		$this(image) maj $image
		# show the image node
		$this(image) Afficher_noeud 1
	} else {
		# hide the image node
		$this(image) Afficher_noeud 0
	}
}
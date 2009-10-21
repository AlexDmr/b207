proc get_Spline {L_pt nb_subdivisions} {
 set L_rep [list ]
 
 while {[llength $L_pt] > 2} {
   set L_pt_tmp  [lrange $L_pt 0 7]
   set L_pt      [lrange $L_pt 6 end]
   set L_rep_tmp [get_Spline_4 $L_pt_tmp $nb_subdivisions]
   set L_rep [concat $L_rep $L_rep_tmp]
  }
  
 return $L_rep
}

proc get_Spline_4 {L_pt nb_subdivisions} {
 set nb_pt [expr [llength $L_pt]/2]
 if {$nb_pt <= 2 || $nb_pt%3 != 1} {
   # Correction des données
   puts "Problème dans le nombre de points fournit"
   return $L_pt
  }
 
 set L_pt_rep [list ]
 
 for {set i 0} {$i <= $nb_subdivisions} {incr i} {
   set     L_pt_tmp $L_pt
   while {[llength $L_pt_tmp] > 2} {
     set px [lindex $L_pt_tmp 0]; set py [lindex $L_pt_tmp 1];
	 set new_L_pt_tmp {}
	 set L_pt_tmp [lrange $L_pt_tmp 2 end]
	 set d [expr $i/$nb_subdivisions.0]
     foreach {x y} $L_pt_tmp {
	   lappend new_L_pt_tmp [expr $px+($x-$px)*$d] [expr $py+($y-$py)*$d]
	   set px $x; set py $y
	  }
	 set L_pt_tmp $new_L_pt_tmp
    }
	
   lappend L_pt_rep [lindex $L_pt_tmp 0] [lindex $L_pt_tmp 1]
  }
 
 return $L_pt_rep
}

proc is_main {} {
	global argv0
	return [string equal [info script] $argv0]
}

if {[is_main]} {
 load "" TK
 canvas .c
 pack .c -fill both -expand 1
 set L [list 0 0 100 100 200 50 300 300]
 eval ".c create line $L"
 
 puts "set L_rep \[get_Spline \$L 8\]; eval \".c create line \$L_rep -fill red\""
}

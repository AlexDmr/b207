#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
method B_2D_plotter constructor {} {
 set this(root) [B_noeud]
 set this(L_pt) {}
   set this(L_pt_circle_pool) {}
   set this(nb_poly_circle)   0
   set this(L_active_poly_circle) {}
 set this(L_poly_pt_line) {}
 set this(line) [B_polygone]
   $this(line) Mode_ligne
   $this(line) Lignes_GL [GL_line_loop]
 $this(root) Ajouter_fils $this(line)
# Form
 set this(L_pt_form) [list -10 -10 10 -10 10 10 -10 10]
# Color
 set this(color) "1 1 1 1"
# Scale
 set this(x_scale) 1
 set this(y_scale) 1
 set this(L_y_elements) {}
 
 set this(var_drawed) ""
}
#___________________________________________________________________________________________________________________________________________
method B_2D_plotter dispose {} {
 Detruire $this(line)
 foreach p $this(L_poly_pt_line) {
   Detruire $p
  }
}

#___________________________________________________________________________________________________________________________________________
method B_2D_plotter get_L_pt_form { } {return $this(L_pt_form)}
#___________________________________________________________________________________________________________________________________________
method B_2D_plotter set_L_pt_form {L} {set this(L_pt_form) $L}
#___________________________________________________________________________________________________________________________________________
method B_2D_plotter get_root {} {return $this(root)}
#___________________________________________________________________________________________________________________________________________
method B_2D_plotter get_poly_line {} {return $this(line)}
#___________________________________________________________________________________________________________________________________________
method B_2D_plotter get_color { } {return $this(color)}
method B_2D_plotter set_color {c} {
 set this(color) $c
 foreach p $this(L_poly_pt_line) {
   eval "$p Couleur $c"
  }
}

#___________________________________________________________________________________________________________________________________________
method B_2D_plotter get_var_drawed { } {return $this(var_drawed)}
method B_2D_plotter set_var_drawed {v} {set this(var_drawed) $v}
#___________________________________________________________________________________________________________________________________________
method B_2D_plotter get_x_scale { } {return $this(x_scale)}
method B_2D_plotter set_x_scale {v} {set this(x_scale) $v}
#___________________________________________________________________________________________________________________________________________
method B_2D_plotter get_y_scale { } {return $this(y_scale)}
method B_2D_plotter set_y_scale {v} {set this(y_scale) $v}
#___________________________________________________________________________________________________________________________________________
method B_2D_plotter Redraw {} {this set_L_pt [this get_var_drawed] [this get_L_pt]}
#___________________________________________________________________________________________________________________________________________
method B_2D_plotter get_L_pt { } {return $this(L_pt)}
#___________________________________________________________________________________________________________________________________________
method B_2D_plotter set_L_y_elements {L} {set this(L_y_elements) $L}

#___________________________________________________________________________________________________________________________________________
method B_2D_plotter Update_var {v} {
 $v set_L_examples ""
 foreach p $this(L_active_poly_circle) {
   if {[string equal $this(L_y_elements) {}]} {
     $v Add_exemple [expr [$p Px]/$this(x_scale)] [expr [$p Py]/$this(y_scale)] <
	} else {$v Add_exemple [expr [$p Px]/$this(x_scale)] [lindex $this(L_y_elements) [expr int([$p Py]/$this(y_scale))]] <} 
  }
}

#___________________________________________________________________________________________________________________________________________
method B_2D_plotter Represent_var {v tx ty} {
 this set_var_drawed $v
 set L_pt [$v get_L_examples]
   set pt [lindex $L_pt 0]
   set y  [lindex $pt   1]
   if {[catch "expr 2*$y"]} {
    # This is a non numerical variable
	 set L_elements {}
	 foreach pt $L_pt {
	   set y [lindex $pt 1]
	   if {[lsearch $L_elements $y] == -1} {lappend L_elements $y}
	  }
	 this set_L_y_elements $L_elements
    }
 this set_L_pt $v $L_pt $tx $ty
}

#___________________________________________________________________________________________________________________________________________
method B_2D_plotter Scale_line {L_pt generate_poly_pt} {
 set L_pt_contour_line {}
 foreach pt $L_pt {
     set x [expr [lindex $pt 0]*$this(x_scale)]
	 if {[string equal $this(L_y_elements) ""]} {
       set y [expr [lindex $pt 1]*$this(y_scale)]
	  } else {set pos [lsearch $this(L_y_elements) [lindex $pt 1]]
	          set y [expr $pos *$this(y_scale)]
	         }
   lappend L_pt_contour_line $x $y
   if {$generate_poly_pt} {
     set p [this get_a_poly_circle]
     $p Origine $x $y
	 lappend this(L_poly_pt_line) $p
	}
  }
 return $L_pt_contour_line
}

#___________________________________________________________________________________________________________________________________________
method B_2D_plotter set_L_pt {v L tx ty} {
 set max_x 0; set max_y 0
 set this(L_pt) $L
 $this(line) Vider
# Add points to the line (just before and after each example, and discretisation points, every alpha...) 
 # Add interesting points (just before and after examples, plus 0 and end point
  set new_L [list "0 [$v Compute_val 0]"]
  set llength_L [llength $L]; set epsilon 0.001
  for {set i 0} {$i < $llength_L} {incr i} {
    set pt_original [lindex $L $i]
	set x_original  [lindex $pt_original 0]
   # Add before point, point, and after point 
    set x [expr $x_original - $epsilon]; set y [$v Compute_val $x]; lappend new_L "$x $y";
	  set max_x [expr $x>$max_x?$x:$max_x]; catch {set max_y [expr $y>$max_y?$y:$max_y]}
    lappend new_L $pt_original                                      
    set x [expr $x_original + $epsilon]; set y [$v Compute_val $x]; lappend new_L "$x $y";
	  set max_x [expr $x>$max_x?$x:$max_x]; catch {set max_y [expr $y>$max_y?$y:$max_y]}
   }
 # Add a last point to prolongate the curve
  set x [expr $x_original * 1.1]; set y [$v Compute_val $x]; lappend new_L "$x $y"
    set max_x [expr $x>$max_x?$x:$max_x]; set err [catch {set max_y [expr $y>$max_y?$y:$max_y]}]
 # Update scale
  if {$err} {
    if {[catch {this set_x_scale [expr double($tx) / double($max_x)]}]} {this set_x_scale 1}
	this set_y_scale 50
   } else {if {[catch {this set_x_scale [expr double($tx) / double($max_x)]}]} {this set_x_scale 1}
           if {[catch {this set_y_scale [expr double($ty) / double($max_y)]}]} {this set_y_scale 1}
		  }
  # Let's now generate the line
 #puts "Let's now generate the line :\n{$L}"
 set L_poly $this(L_poly_pt_line)
   foreach p $L_poly {this Release_a_poly_circle $p}
   set this(L_poly_pt_line) ""
 this Scale_line $L 1
 
# Add points to close the polygon
  # Last point projected to abscissa plus (0 0)
  set last_pt [lindex $new_L end]
  set x [lindex $last_pt 0]
  set y [expr -1.0/[this get_y_scale]]
  lappend new_L "$x $y" "0 $y"
# Update the polygon
 set L_pt_for_line [this Scale_line $new_L 0]
 #puts "L_pt_for_line : {$L_pt_for_line}"
 $this(line) Ajouter_contour [ProcTabDouble $L_pt_for_line]
}

#___________________________________________________________________________________________________________________________________________
method B_2D_plotter get_a_poly_circle {} {
 if {[string equal $this(L_pt_circle_pool) {}]} {
   set rep [B_polygone]
   $rep Ajouter_contour [ProcTabDouble $this(L_pt_form)]
   B_contact ctc_${objName}_$this(nb_poly_circle) "$rep 1"
   incr this(nb_poly_circle)
  } else {set rep [lindex $this(L_pt_circle_pool) 0]
          set this(L_pt_circle_pool) [lreplace $this(L_pt_circle_pool) 0 0]
         }
 $this(root) Ajouter_fils $rep
   eval "$rep Couleur $this(color)"
   lappend this(L_active_poly_circle) $rep
 return $rep
}

#___________________________________________________________________________________________________________________________________________
method B_2D_plotter Release_a_poly_circle {p} {
 lappend this(L_pt_circle_pool) $p
 Sub_element this(L_active_poly_circle) $p
 Sub_element this(L_poly_pt_line)       $p
 $p Vider_peres
}

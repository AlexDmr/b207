package require udp
source {C:\These\Projet Interface\COMETS\devCOMETS\Comets\gml_Object.tcl}
catch {load "" TK}

#_____________________________________________________________________________________________________
proc udp_puts {host port nb L_points} {
 set s [udp_open]
   set msg [binary format i $nb]
   foreach point $L_points {
     append msg [binary format ifffff [lindex $point 0] [lindex $point 1] [lindex $point 2] [lindex $point 3] [lindex $point 4] [lindex $point 5]]
    }
   fconfigure $s -translation binary -remote [list $host $port]
   puts $s $msg
   flush $s
 close $s
}

#_____________________________________________________________________________________________________
#_____________________________________________________________________________________________________
#_____________________________________________________________________________________________________
method Sim_MuTable constructor {ip port} {
 set this(ip)   $ip
 set this(port) $port
 set this(tk_toplevel) "._${objName}_top";   toplevel $this(tk_toplevel)
 set this(canvas) $this(tk_toplevel).canvas; canvas $this(canvas); pack $this(canvas) -expand 1 -fill both
 bind $this(canvas) <ButtonPress>   "$objName Clic_on %x %y %b"
 bind $this(canvas) <ButtonRelease> "if {%b == 1} {$objName set_clic 0}"
 bind $this(canvas) <Motion> "$objName Move %x %y"
 set this(L_pt) {}
 set this(num_pt) 0
 set this(clic) 0
 set this(send_infos) 1
 this set_current {}
}

#_____________________________________________________________________________________________________

#_____________________________________________________________________________________________________
method Sim_MuTable Send_infos {t} {
 if {$this(send_infos) == 0} {set this(send_infos) 1; return}
 set L  [this get_L_pt]
 set nb [llength $L]; set L_pt {}

 foreach pt $L {
   set coords [$this(canvas) coords $pt]
   set x [expr 1024.0*[lindex $coords 0]/[winfo width  $this(canvas)].0]
   set y [expr  768.0*[lindex $coords 1]/[winfo height $this(canvas)].0]
  #puts [list 1000.0 $x $y 10.0 10.0]
   set pt_num [string range $pt 3 end]
   set L_pt [linsert $L_pt 0 [list $pt_num 1000.0 $x $y 10.0 10.0]]
  }
 
 udp_puts $this(ip) $this(port) $nb $L_pt
 set this(last_send) "udp_puts $this(ip) $this(port) $nb $L_pt"
 
 after $t "$objName Send_infos $t"
}

#_____________________________________________________________________________________________________
method Sim_MuTable get_last_send {} {return $this(last_send)}

#_____________________________________________________________________________________________________
method Sim_MuTable get_canvas {} {return $this(canvas)}

#_____________________________________________________________________________________________________
method Sim_MuTable get_L_pt {} {return $this(L_pt)}

#_____________________________________________________________________________________________________
method Sim_MuTable get_a_pointer {} {
 set rep pt_$this(num_pt)
 lappend this(L_pt) $rep
 incr this(num_pt)
 return $rep
}

#_____________________________________________________________________________________________________
method Sim_MuTable Release_pt {pt} {
 set pos [lsearch $this(L_pt) $pt]
 set this(L_pt) [lreplace $this(L_pt) $pos $pos]
}

#_____________________________________________________________________________________________________
method Sim_MuTable get_current {  } {return $this(current_pt)}
method Sim_MuTable set_current {pt} {set this(current_pt) $pt}

#_____________________________________________________________________________________________________
method Sim_MuTable get_clic { } {return $this(clic)}
method Sim_MuTable set_clic {c} {set this(clic) $c}

#_____________________________________________________________________________________________________
method Sim_MuTable Clic_on {x y bt} {
# puts "$objName Clic_on $x $y $bt : [$this(canvas) scan mark $x $y]"
 switch $bt {
   1 {
	  if {[this get_current] == ""} {
	    set pt_name [this get_a_pointer]
        $this(canvas) create oval [expr $x-10] [expr $y-10] [expr $x+10] [expr $y+10] -fill cyan -tags $pt_name
	    $objName set_current $pt_name; this set_clic 1
		puts $pt_name
	    $this(canvas) bind $pt_name <ButtonPress>   "if {%b == 3} {$this(canvas) delete $pt_name; $objName Release_pt $pt_name; $objName set_current {}}; if {%b == 1} {$objName set_clic 1}"
	    $this(canvas) bind $pt_name <ButtonRelease> "if {%b == 1} {$objName set_clic 0}; $objName set_current {}"
	    $this(canvas) bind $pt_name <Enter> "$objName set_current $pt_name"
	    $this(canvas) bind $pt_name <Leave> "if {\[$objName get_clic\] == 0} {$objName set_current {}}"
	   }	
	 }
  }  
}

#_____________________________________________________________________________________________________
method Sim_MuTable Move {x y} {
 set pt [this get_current]
 if {[string equal $pt {}] || ($this(clic) == 0)} {return}
  #set X [expr 1024.0*($x-10)/[winfo width  $this(canvas)].0] 
  #set Y [expr  768.0*($y-10)/[winfo height $this(canvas)].0]
  #binary scan [binary format f $X] f X2
  #puts "X $X => $X2"
 $this(canvas) coords $pt [expr $x-10] [expr $y-10] [expr $x+10] [expr $y+10]
}

#_____________________________________________________________________________________________________
puts "Sim_MuTable sim_table localhost 8910"


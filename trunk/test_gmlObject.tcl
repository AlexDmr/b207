source gml_Object.tcl

#_________________________________________________________
#_________________________________________________________
#_________________________________________________________
method C_0 constructor {p1 p2 args} {

 set this(a0) a
 set this(b0) a
 set this(c0) a
 set this(d0) a

 eval "$objName configure $args"
 return $objName
}
#_________________________________________________________
method C_0 configure args {
 set L_cmd [split $args -]
 foreach cmd $L_cmd {
   if {[string equal $cmd {}]} {continue}
   if {[string equal [string index $cmd 0] | ]} {
     set cmd [string range $cmd 1 end]
     eval $cmd
    } else {eval "$objName $cmd"}
  }
}

#_________________________________________________________
#_________________________________________________________
inherit C_1 C_0
#_________________________________________________________
method C_1 constructor {p1 p2 args} {
 this inherited $p1 $p2

 set this(a1) a
 set this(b1) a
 set this(c1) a
 set this(d1) a

 eval "$objName configure $args"
 return $objName
}


#_________________________________________________________
#_________________________________________________________
inherit C_2 C_1
#_________________________________________________________
method C_2 constructor {p1 p2 args} {
 this inherited $p1 $p2

 set this(a2) a
 set this(b2) a
 set this(c2) a
 set this(d2) a

 eval "$objName configure $args"
 return $objName
}

proc Creation_et_destruction {class nb} {
 set t [clock clicks -millisecond]
 for {set i 0} {$i<$nb} {incr i} {
   $class "Instance_${i}_of_$class" coucou hehehe
  }
 set dt [expr [clock clicks -millisecond] - $t]; puts "Création de $nb $class en : $dt"
 set t [clock clicks -millisecond]
 for {set i 0} {$i<$nb} {incr i} {
    "Instance_${i}_of_$class" dispose
  }
 set dt [expr [clock clicks -millisecond] - $t]; puts "Destruction de $nb $class en : $dt\n"
}

Creation_et_destruction C_0 1000
Creation_et_destruction C_1 1000
Creation_et_destruction C_2 1000


if {[info exists ::env(ROOT_COMETS)]} {
 cd $::env(ROOT_COMETS)/Comets
 source gml_Object.tcl
 set msg "cd $::env(B207_LIBRARY)"
 cd $::env(B207_LIBRARY)
} else {load gmlObject.dll}


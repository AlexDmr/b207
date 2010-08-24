if {[info exists ::env(ROOT_COMETS)]} {
 set path_now [pwd]
 cd $::env(ROOT_COMETS)/Comets
 source gml_Object.tcl
 cd $path_now
} else {load gmlObject.dll}


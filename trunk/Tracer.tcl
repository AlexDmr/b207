method Tracer constructor {} {
 set this(L_traces) {}
}

method Tracer get_L_traces { } {return $this(L_traces)}
method Tracer set_L_traces {L} {set this(L_traces) $L}

method Tracer Add_trace {txt} {
 lappend this(L_traces) [list [clock clicks -milliseconds] $txt]
}

method Tracer Add_trace_new_user {name sex age experienced} {
 this Add_trace "NEW USER : name = $name; sex = $sex; age = $age; experienced = $experienced"
}

method Tracer Insert_tracer_in {c m} {
 set body [gmlObject info body $c $m]
 regexp ".*\n#Insert_tracer_in\n.*\n#/Insert_tracer_in\n(.*)" $body reco body
 set L_args [gmlObject info args $c $m]
 
 set    cmd "method $c $m {$L_args} {\n"
 append cmd "#Insert_tracer_in\n"
 append cmd "  $objName Add_trace \[list \$objName $m"
   foreach a $L_args {append cmd { $} $a}
 append cmd "\]\n"
 append cmd "#/Insert_tracer_in\n"
 append cmd $body
 append cmd "}"
 
 eval $cmd
}

method Tracer Insert_tracer_in_L {c Lm} {
 foreach m $Lm {this Insert_tracer_in $c $m}
}

method Tracer Save_to_file {f_name} {
 set f [open $f_name a]
   foreach t $this(L_traces) {puts $f $t}
 close $f
}
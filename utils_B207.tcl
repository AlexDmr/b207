source fonctions_clipping.tcl
source fonctions_transfo.tcl
source drag_and_drop.tcl
source lists_functions.tcl

#_________________________________________________________________________________________________________
#_________________________________________________________________________________________________________
#_________________________________________________________________________________________________________
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

#_________________________________________________________________________________________________________
#_________________________________________________________________________________________________________
#_________________________________________________________________________________________________________
proc Manage_CallbackList {c L_m pos args} {
 set L_varL_to_declare_in_constr [list]
 foreach m $L_m {
# Init the callback mechanism
   set body   [gmlObject info body    $c $m]
   set L_args [gmlObject info arglist $c $m]
   set argL   [gmlObject info args    $c $m]

   set L_CB_name "this\(L_CB_$m\)"
   lappend L_varL_to_declare_in_constr $L_CB_name
   set cmd "method $c Trigger_L_CB_$m \{L_CB args\} \{\n"
     append cmd " foreach CB \[subst \$\$L_CB\] \{\n"
     append cmd "   eval \""
       set p 0; foreach a $argL {append cmd "set $a \{\[lindex \$args $p\]\}; "; incr p}
                foreach a $args {append cmd "set $a \{\[lindex \$args $p\]\}; "; incr p}
     append cmd "\[lindex \$CB 1\]\"\n"
     append cmd "  \}\n"
   append cmd "\}\n"
   eval $cmd
# Generate the callback accessors
   eval "Generate_accessors $c L_CB_$m"
# Generate the callback accessors for subscribing
   set cmd "method $c Subscribe_to_$m \{id CB \{UNIQUE \{\}\}\} \{\n"
     append cmd " if \{\[string equal \$UNIQUE \{\}\]\} \{\} else \{\n"
     append cmd "   set L \{\}\n"
     append cmd "   foreach CB_current \$$L_CB_name \{\n"
     append cmd "     if \{\[string equal \$id \[lindex \$CB_current 0\]\]\} \{\} else \{lappend L \$CB_current\}\n"
     append cmd "    \}\n"
     append cmd "   set $L_CB_name \$L\n"
     append cmd "  \}\n"
     append cmd " lappend $L_CB_name \[list \$id \$CB\];\n"
   append cmd "\}\n"
   eval $cmd
# Generate the callback accessors for unsubscribing
   set cmd "method $c UnSubscribe_to_$m \{id\} \{\n"
     append cmd " set L \{\}\n"
     append cmd " foreach CB \$$L_CB_name \{\n"
     append cmd "   if \{\[string equal \$id \[lindex \$CB 0\]\]\} \{\} else \{lappend L \$CB\}\n"
     append cmd "  \}\n"
     append cmd " set $L_CB_name \$L;\n"
   append cmd "\}\n"
   eval $cmd
# Generate the callback mechanism
   set cmd "method $c $m \{$L_args\} \{\n"
     if {[regexp "(.*)# INSERT CALLBACKS HERE(.*)" $body rep avant apres]} {
       append cmd $avant {# INSERT CALLBACKS HERE} "\n"
       append cmd " this Trigger_L_CB_$m $L_CB_name"
         foreach a $argL {append cmd " \$$a"}
         foreach a $args {append cmd " \$$a"}
       append cmd "\n" $apres
      } else {switch $pos {
                begin {append cmd " this Trigger_L_CB_$m $L_CB_name"
                         foreach a $argL {append cmd " \$$a"}
                         foreach a $args {append cmd " \$$a"}
                       append cmd "\n" $body "\n"
                      }
                end   {append cmd $body "\n"
                       append cmd " this Trigger_L_CB_$m $L_CB_name"
                         foreach a $argL {append cmd " \$$a"}
                         foreach a $args {append cmd " \$$a"}
                       append cmd "\n"
                      }
               }
             }
   append cmd "\}\n"
   eval $cmd
  }

# Add initialisation of lists of callbacks in the constructor
 set old_body [gmlObject info body    $c constructor]
 set argL [gmlObject info arglist $c constructor]
 set body "\n#___________________________________\n#Definition of some callback lists |\n#___________________________________\n"
 foreach CB $L_varL_to_declare_in_constr {
   append body " set $CB \[list\]\n"
  }
 append body $old_body

 set cmd "method $c constructor \{$argL\} \{\n"
   append cmd $body
 append cmd "\}\n"
 eval $cmd
}

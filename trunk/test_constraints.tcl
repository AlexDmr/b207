source ./Contraintes/Constraint_Engine.tcl
source ./Contraintes/test_1.UI
set cmd_cstr [load_constraints_from_in ./Contraintes/test_1.constraints]

set rap_cstr [B_rappel [Interp_TCL] "eval {$cmd_cstr}"]
  $fen abonner_a_dimension [$rap_cstr Rappel]
  eval $cmd_cstr
  
 proc maj_constraints {rap file_name} {
  set cmd [load_constraints_from_in $file_name]
  $rap Texte "eval {$cmd}"
 }
 
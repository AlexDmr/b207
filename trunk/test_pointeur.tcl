set rap_ptr [$Rappel_TCL $interp_tcl]
set ptr_0 [$sim_ptr Pointeur 0]
$ptr_0 abonner_a_changement [$rap_ptr Rappel]

return "   rap_ptr : Rappel_TCL\n   ptr_0   : alx_pointeur"

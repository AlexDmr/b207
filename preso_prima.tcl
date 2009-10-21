source test_menu_main.tcl
cd ../COMETS/devCOMETS/
  source test_swl.tcl
  set f [open A_drawer_1238407719.B207_DRAWING]
    set cc [B_cc [read $f]]
  close $f
cd ../../BIGre

[CSS++ cr "#cr_PM_P_BIGre > * \\meta/"] Origine 300 700

set poly [A_drawer Get_a_polygone]
  A_drawer Register_drawed_polygone $poly
  $poly Deserialiser_type $cc

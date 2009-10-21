source test_mutable.tcl
Liant_Mutable_sim_ptr Liant_mutable; Liant_mutable startListening 8910
source sim_mutable.tcl
Sim_MuTable sim_table localhost 8920
#sim_table Send_infos 20

set server [socket -server "New_connection Liant_mutable" 1234]


proc New_connection {liant chan ad num} {
 puts "New connection $chan $ad $num"
 fconfigure $chan -blocking 0
 fileevent $chan readable "On_read $liant $chan"
 
 $liant set_CB_new_frame "BBox $chan "
}

proc On_read {liant chan} {
 if {[eof $chan]} {
   puts "Close $chan"
   $liant set_CB_new_frame Nop
   close $chan
  }
}

proc Tournenrond args {}
proc Tournenrond {c id V1 V2 v i dt} {
 #puts "$c move $id $i 0"
 $c move $id $i 0
 #puts "  1"
 set nv [expr $v + $i]
 #puts "  2"
 if {$nv >= $V2 || $nv <= $V1} {set ni [expr -$i]} else {set ni $i}
 #puts "  3"
 
 after $dt "Tournenrond $c $id $V1 $V2 $nv $ni $dt"
 #puts "  4"
}

Tournenrond ._sim_table_top.canvas pt_0 0 50 1 1 20

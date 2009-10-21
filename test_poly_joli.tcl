if {[info exists noeud_partage]} {} else {
 source groupe.tcl
 proc etire_fond args {}
 N_i_mere Afficher_souris
}

if {[info exists delta_a]} {} else {set delta_a 0}
set L {}
set nb_pt 2048
set R     100
set PI    3.14159265
for {set i 0} {$i<$nb_pt} {incr i} {
  set r [expr $R*(1.5+sin(2*$i*$PI/$nb_pt))]
#  set r [expr $R*(1.5+sin(2*$i*$PI/$nb_pt))]
#  set r [expr $R*(0.7+sin(10*$i*$PI/$nb_pt))]

  set a [expr 2*cos($PI*($delta_a+$r)/((300-$r)/10.0+$R.0))+2*$i*$PI/${nb_pt}.0]
#  set a [expr 2*$i*$PI/${nb_pt}.0]
  lappend L [expr $r*cos($a)] \
            [expr $r*sin($a)]
 }

if {[info exists p]} {} else {set p [B_polygone]
                              $noeud_partage Ajouter_fils $p
                              B_contact p_ctc "$p 1"
                             }
$p Vider
$p Ajouter_contour [ProcTabDouble $L]


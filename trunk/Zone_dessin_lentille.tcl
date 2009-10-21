if {[info exists noeud_fen]} {} else {
 source groupe.tcl
}
if {[info exists fzo]} {} else {
 source loupe_ovale.tcl
 #source test_lentille.tcl
}

#______________________________________________________________________________________________________________
set icl_zone_dessin [B_courbe_lentille]
proc Poly_to_lentille {icl z} {
 set L [$z Val L_pt]
 set nb_pt [llength $L]
 set tx [$z Taille_x]
 set ty [$z Taille_y]

 $icl Allouer $nb_pt
 for {set i 0} {$i<$nb_pt} {incr i} {
   set pt [lindex $L $i]
   set X  [lindex $pt 0]
   set Y  [lindex $pt 1]
   $icl Affecter $i [expr $X/$tx] [expr $Y/$ty]
  }
 return $icl
}

#______________________________________________________________________________________________________________
  set tx 320
  set ty 320
set zone_dessin [B_polygone]
  $noeud_fen Ajouter_fils $zone_dessin
  $zone_dessin Ajouter_contour [ProcRect 0 0 $tx $ty]
  $zone_dessin Couleur 0.45 1 0.3 1
  $zone_dessin Origine 0 250
set poly_ligne [B_polygone]
  $zone_dessin Ajouter_fils $poly_ligne
  $poly_ligne  Mode_ligne
  $poly_ligne  Ajouter_contour [ProcTabDouble "0 0 $tx $ty $tx 0"]
  $poly_ligne  Noeud_touchable 0

set bt_apply [B_bouton 100 30 [N_i_mere] $zone_dessin 1]
  $zone_dessin Ajouter_fils $bt_apply
  $bt_apply Origine 220 -40
  [$bt_apply Noeud_fen] Couleur_fond 0 1 0 1
  $bt_apply Mettre_a_jour
set bt_reset [B_bouton 100 30 [N_i_mere] $zone_dessin 1]
  $zone_dessin Ajouter_fils $bt_reset
  $bt_reset Origine 0 -40
  [$bt_reset Noeud_fen] Couleur_fond 1 0 0 1
  $bt_reset Mettre_a_jour
set ztxt_beta [B_texte $tx 25 20 [fonte_Arial] [B_sim_sds]]
  $zone_dessin Ajouter_fils $ztxt_beta
  $ztxt_beta Origine 0 -120
  $ztxt_beta Afficher_fond 1
  $ztxt_beta Couleur_fond 0 0.3 1 0.85
set ztxt_math [B_texte $tx 120 20 [fonte_Arial] [B_sim_sds]]
  $zone_dessin Ajouter_fils $ztxt_math
  $ztxt_math Origine 0 -250
  $ztxt_math Afficher_fond 1
  $ztxt_math Couleur_fond 0 0.3 1 0.85
set bt_math [B_bouton 100 30 [N_i_mere] $zone_dessin 1]
  $ztxt_math Ajouter_fils $bt_math
  $bt_math Origine 210 10
  [$bt_math Noeud_fen] Couleur_fond 0 1 0 1
  $bt_math Mettre_a_jour

set rap_zone [B_rappel $interp_tcl]
  set cmd "FSM_Start $rap_zone $zone_dessin $poly_ligne"
  $rap_zone Texte $cmd

set rap_reset [B_rappel $interp_tcl "$poly_ligne Vider; $poly_ligne Ajouter_contour [ProcTabDouble "0 0 $tx $ty $tx 0"]; $rap_zone Texte \"$cmd\""]
  $bt_reset abonner_a_activation [$rap_reset Rappel]
set rap_apply [B_rappel $interp_tcl "set len \[$fzo Rep_lentille\]\n\
                                     \$len maj_lentille \[Poly_to_lentille $icl_zone_dessin $zone_dessin\] \n\
                                     if \{\[$icl_zone_dessin Nb\]\} \{$fzo maj_lentille\}"]
  $bt_apply abonner_a_activation [$rap_apply Rappel]

set rap_math [B_rappel $interp_tcl]
  set cmd "Generer_courbe $ztxt_math $ztxt_beta $zone_dessin $poly_ligne 60 $rap_apply"
  $rap_math Texte $cmd
  $bt_math abonner_a_activation [$rap_math Rappel]

$zone_dessin abonner_a_detection_pointeur [$rap_zone Rappel] 67

#B_contact ctc_dessin "$zone_dessin 3" -pt_trans 160 120 -pt_rot 160 120

#______________________________________________________________________________________________________________
#______________________________________________________________________________________________________________
#______________________________________________________________________________________________________________
proc Generer_courbe {ztxt ztxt_beta z p nb_pt rap_lentille} {
 set tx [$z Taille_x]
 set ty [$z Taille_y]

 set L_pt {}
 set f    [$ztxt TEXTE]
 set beta [$ztxt_beta TEXTE]
 if {[regexp {^(.*):(.*)->(.*) in (.*)$} $beta reco b_name v1 v2 t]} {
   set transfo [B_transfo $t]
   set rap_anim [B_rappel [Interp_TCL]]
   $rap_anim Param $transfo
   set cmd "set t \[$transfo T_courant\]; Anim_courbe $z $p $tx $ty $nb_pt \{$f\} $b_name $v1 $v2 \$t; eval \[$rap_lentille Texte\]"
   $rap_anim Texte $cmd

   $transfo abonner_a_rappel_pendant [$rap_anim Rappel]
   N_i_mere Ajouter_deformation_dynamique $transfo
   $transfo Demarrer
  } else {Anim_courbe $z $p $tx $ty $nb_pt $f bidon 0 0 1}
}

#______________________________________________________________________________________________________________
proc Anim_courbe {z p tx ty nb_pt f b_name v1 v2 dt} {
 set $b_name [expr $v1+$dt*($v2-$v1)]
 for {set i 0} {$i<$nb_pt} {incr i} {
   set x [expr $i.0/$nb_pt]
   set y [expr $f]
   lappend L_pt [list [expr $tx*$x] [expr $ty*$y]]
  }

 $z Ajouter_au_contexte L_pt $L_pt
 set L {}
   foreach pt $L_pt {lappend L [lindex $pt 0] [lindex $pt 1]}
 lappend L $tx $ty $tx 0
 $p Vider
 $p Ajouter_contour [ProcTabDouble $L]
}

#______________________________________________________________________________________________________________
#______________________________________________________________________________________________________________
#______________________________________________________________________________________________________________
proc FSM_Start {rap z l} {
 set inf [$rap Param]
 set inf [Void_vers_info $inf]
 set evt [$inf EVT]
 set pt  [$inf Point_au_contact]

 set noeud [eval Real_class [$inf NOEUD]]
   if {[string equal $noeud $z]}       {} else {return}


 set tx [$z Taille_x]
 set ty [$z Taille_y]
 switch [$evt Type_evt] {
   1  {$l Vider; $l Ajouter_contour [ProcTabDouble "0 0 $tx $ty $tx 0"]; $rap Texte "FSM_New $rap $z [$inf Ptr] $l \{\{0 0\} \{[$pt X] [$pt Y]\}\} 0"}
   2  {}
   64 {}
  }
}

#______________________________________________________________________________________________________________
proc Compare_pt {p1 p2} {
 set x1 [lindex $p1 0]
 set x2 [lindex $p2 0]
 if {$x1<$x2} {return -1}
 if {$x1>$x2} {return  1}
 return 0
}

#______________________________________________________________________________________________________________
proc Order {L_name} {
 upvar $L_name L
 
 set L [lsort -command Compare_pt $L]
}

#______________________________________________________________________________________________________________
proc FSM_New {rap z ptr poly L_pt quittable} {
 set p   [$rap Param]
 set inf [Void_vers_info $p]
 set evt [$inf EVT]
 set pt  [$inf Point_au_contact]

#if {[string equal $ptr [$inf Ptr]]} {} else {return}

 set noeud [eval Real_class [$inf NOEUD]]
   if {[string equal $noeud $z]}       {} else {return}

 set tx [$z Taille_x]
 set ty [$z Taille_y]
 switch [$evt Type_evt] {
   1  {if {$quittable} {$rap Texte "FSM_Start $rap $z $poly"}
       lappend L_pt "[$pt X] [$pt Y]"; Order L_pt; $rap Texte "FSM_New $rap $z $ptr $poly \{$L_pt \{[$pt X] [$pt Y]\}\} 0";
      }
   2  {lappend L_pt "[$pt X] [$pt Y]"; Order L_pt; $rap Texte "FSM_New $rap $z $ptr $poly \{$L_pt \{[$pt X] [$pt Y]\}\} 0";
      }
   64 {set L_pt [lreplace $L_pt end end "[$pt X] [$pt Y]"]; $rap Texte "FSM_New $rap $z $ptr $poly \{$L_pt\} 0"
       Order L_pt
      }
  }
 $poly Vider
 set L {}
 foreach p $L_pt {
   lappend L [lindex $p 0] [lindex $p 1]
  }
 $z Ajouter_au_contexte L_pt $L_pt
 lappend L $tx $ty $tx 0

 $poly Ajouter_contour [ProcTabDouble $L]
}

#______________________________________________________________________________________________________________
proc svg_len {f_name} {
 global ztxt_math
 global ztxt_beta
 
 set f [open $f_name w]
   puts $f [$ztxt_beta TEXTE]
   puts $f [$ztxt_math TEXTE]
 close $f
}

#______________________________________________________________________________________________________________
proc load_len {f_name} {
 global ztxt_math
 global ztxt_beta
 
 set f [open $f_name r]
   $ztxt_beta Maj_texte [gets $f]
   $ztxt_math Maj_texte [gets $f]
 close $f
}

proc etire_fond args {}

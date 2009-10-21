set noeud_touche 0
set etape 0
set ad coco
set XXX 0

proc faire_zoom {rappel} {
 global r_contient
# set r $rappel
 set r $r_contient
 global noeud_fen
 global fen_editeur_tcl
 global f_obs
 global rep_res
 global n_i_mere
 global f_vue_normale
 global noeud_touche
 global etape
 global ad
 global rep_tmp
 global transfo_dyn_rep
 global XXX

# set fr [$r Rappel]
 set i [$r Param]
 set i [Void_vers_info $i]
 set Lrep [$i L_REPERES]
 set nf [$i NOEUD]
 set noeud_touche $nf
 set etape coco
#Condenser_reperes $L_rep $rep_res deb=0 fin=999999
 Condenser_reperes $Lrep $rep_res 2

 set etape 1
 if {[regexp {^(_[0-9|a-f]*)_} $nf reco ad_nf]} {set ad $ad_nf} else {return}
 set etape 2
 set pos [string first $ad_nf $f_vue_normale 0]
 set etape 3

 set ecran_x [$n_i_mere Largeur]
 set ecran_y [$n_i_mere Hauteur]
 set pc [$i Point_au_contact]
 set X [$pc X]
 set Y [$pc Y]
 set XXX $X
 if {[expr $pos==0]} {
   set etape 4
   $rep_res Exprimer_dans $noeud_fen
   set o [$f_vue_normale Origine]
   set e [$f_vue_normale Etirement]
   set ex [expr 1.0/[$e Dx]]
   set ey [expr 1.0/[$e Dy]]
   if {[expr $X>0]} {set etape 5} else {set etape 6}
   if {[expr [$o X]!=($ecran_x/2.0)]} {append etape ".1"; $rep_res Etirer_de $ex $ey} else {
     if {[expr [$o Y]!=($ecran_y/2.0)]} {append etape ".2"; $rep_res Etirer_de $ex $ey} else {append etape ".3"; }}
   if {[string equal $etape {6.3}]} {
     set v_e [expr -10*$X/[$f_vue_normale Longueur_corp]]
     if {[expr $v_e<1]} {set v_e 1}
     $rep_res Etirer_de [expr $ex*$v_e] [expr $ey*$v_e]} else {if {[string equal $etape {5.3}]} {
       set v_e [expr 1.0/(10*$X/[$f_vue_normale Longueur_corp])]
       if {[expr $v_e>1]} {set v_e 1}
       $rep_res Etirer_de [expr $ex*$v_e] [expr $ey*$v_e]} }
   #set e [$noeud_fen Etirement]
   #set ex [$e Dx]; set ey [$e Dy]
   #$noeud_fen Etirement 1 1
   #$rep_res Exprimer_dans $noeud_fen
   #$noeud_fen Etirement $ex $ey
   } else {
     $rep_res Exprimer_dans $noeud_fen}

 set etirement [$rep_res Etirement]
 set EX [$etirement Dx]
 set EY [$etirement Dy]

 if {[expr $pos==0]} {
   $rep_res Translation_interne [expr $EX*(0-$ecran_x/2.0)] [expr $EY*(0-$ecran_y/2.0)]

   set e [$f_vue_normale Etirement]
   set ex [$e Dx]; set ey [$e Dy]
   $rep_tmp maj [expr $ecran_x/2.0] [expr $ecran_y/2.0] 0 $ex $ey
   transfo_fen $f_vue_normale $rep_tmp 1000

  } else {$rep_res Translation_interne [expr $EX*($X-$ecran_x/2.0)] [expr $EY*($Y-$ecran_y/2.0)]}

 if {[string equal $nf $f_obs]} {return BLAIREAU} else {zoom_sur_fen $noeud_fen $rep_res 1000}
}



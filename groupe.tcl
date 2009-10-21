source cool.tcl

set noeud_partage [$noeud_bigre]
$noeud_fen Intercaler_noeud $noeud_partage
set sim_ptr [$n_i_mere Sim_pointeur]
set ptr     [$sim_ptr Pointeur 0]
set n_ptr   [$ptr Noeud_representation]
$noeud_partage Retirer_fils $n_ptr

set pt_dec_partage [$point_bigre]

proc Positionner_partage {x y {r 0} {ex 1} {ey 1}} {
 global noeud_partage
 global noeud_fen
 global pt_dec_partage

 $noeud_partage Origine $x $y
 $noeud_partage Rotation $r
 $noeud_partage Etirement $ex $ey

 $pt_dec_partage maj 0 0
 $noeud_partage Changer_coordonnees $pt_dec_partage

 $noeud_fen Origine [$pt_dec_partage X] [$pt_dec_partage Y]
 $noeud_fen Rotation -$r
 $noeud_fen Etirement [expr 1.0/$ex] [expr 1.0/$ey]
}

source fonctions_mise_en_page.tcl
source vues.tcl
source observateur.tcl

#Mise_en_segment fmes $noeud_partage 0

set cmd_test {source demo_ecrans.tcl;}
set cmd_test {source demo_ecrans.tcl; source camnote_bigre.tcl; $noeud_partage Retirer_fils $fr4; $noeud_partage Ajouter_fils $CamNote; $noeud_partage Ajouter_fils $fr4;}

# Les noms des noeuds
$noeud_partage Nom_IU "Reference\nnode"
$noeud_fen     Nom_IU "Applicative\nwindows"
$img_fond      Nom_IU "Background\nimage"
$f_obs         Nom_IU "Overview"
$f_vue_normale Nom_IU "Zoom\nwindow"

proc etire_fond  args {}
proc etire_fonds args {}
return "La fonction de mise à jour des coordonnées (en modifiant \$noeud_partage et \$noeud_fen) est Positionner_partage. Pour créer un groupe taper :\n\$n_i_mere Creer_groupe 1 essai \$noeud_fen 1 1"


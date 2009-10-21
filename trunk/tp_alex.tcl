if {[info exists noeud_partage]} {} else {
 source groupe.tcl
 proc etire_fond args {}
 N_i_mere Afficher_souris
}

set poly_root [B_polygone]
$noeud_partage Ajouter_fils $poly_root
set L {}
set nb_pt 1024
set R     100
set PI    3.14159265
for {set i 0} {$i<$nb_pt} {incr i} {
  set r [expr $R*(1.5+sin(16*$i*$PI/$nb_pt))]
  set a [expr 2*$i*$PI/${nb_pt}.0]
  lappend L [expr $r*cos($a)] \
            [expr $r*sin($a)]
 }

$poly_root Ajouter_contour [ProcTabDouble $L]
$poly_root Retirer_contour [ProcOvale 0 0 30 20 60]

set p_o [B_polygone]
  $noeud_partage Ajouter_fils $p_o
$p_o Ajouter_contour [ProcOvale 0 0 200 200 60]
B_contact ctc_p_o "$p_o 3" -pt_trans 0 0 -pt_rot 0 0

set img_reflet [B_image reflet_spherique.png]
  $noeud_partage Ajouter_fils $img_reflet; $img_reflet Afficher_noeud 0; $img_reflet Gerer_contacts 0
set img_chat [B_image chat_souris.png]
  $noeud_partage Ajouter_fils $img_chat; $img_chat Afficher_noeud 0; $img_chat Gerer_contacts 0
  

set p_txt [B_polygone]
  $p_txt Ajouter_contour [ProcTabDouble {0 0 500 0 500 500 0 500}]
#  $noeud_partage Ajouter_fils $p_txt
  B_configure $p_txt -Couleur 0.6 0.25 0 0.8 -Mode_texture 0 -Mode_texture_fils 1
set z_txt [B_texte 500 60 50 [fonte_Arial] [B_sim_sds]]
  B_configure $z_txt -Origine 0 230 -TEXTE "coucou les amis..." -Editable 0
  $p_txt Ajouter_fils $z_txt
  $p_txt PreRendre
B_contact ctc_p_txt "$p_txt 1"

B_configure $p_o -Info_texture  [$img_chat   Info_texture] \
                 -Info_texture2 [$img_reflet Info_texture] \
                 -Combinaison_textures [GLenum2UI [GL_interpolate_ARB]] [GLenum2UI [GL_add]] \
				 -Combinaison_textures_operande2 [GLenum2UI [GL_src_alpha]] \
				 -Combinaison_textures_source_arg2 [GLenum2UI [GL_texture]]

B_configure $p_o -Ajouter_fils $p_txt -Mode_texture 1 -Mode_texture_fils 1 -Mode_texture2 1

set p_test [B_polygone]
$p_test Ajouter_contour [ProcTabDouble {0 0 100 0 100 100 0 100}]
$noeud_partage Ajouter_fils $p_test
$p_test Info_texture [$p_o Info_texture_scene]

set rap_chgmnt_dim_tex_scene_p_o [B_rappel [Interp_TCL] "$p_o Mode_texture2 1"]
[$p_o Info_texture_scene] abonner_a_changement_dim [$rap_chgmnt_dim_tex_scene_p_o Rappel]

#B_configure $p_o -Fct_interpollation [get_Fonction_interpolation_poly_boite_englobante] \
#                 -Info_texture [$img_fond Info_texture]

B_contact ctc_poly_root "$poly_root 3" -pt_trans 0 0 -pt_rot 0 0

#B_configure $poly_root -Fct_interpollation [get_Fonction_interpolation_poly_boite_englobante] \
#					   -Coords_couleurs [[ProcTabDouble {0 0.5 0 1 0 0.5 0 1 0.2 1 1 1 0.2 1 1 1}    ] Tab] \
#					   -maj
					   
set img_poly_root [B_image terrain.png]
set info_texture  [$img_fond Info_texture]
$poly_root Info_texture $info_texture
$poly_root Info_texture2 [$img_poly_root Info_texture]

$img_reflet maj s3.png; 
set img_chat [B_image chat_souris.png]
source zone_dessin_lentille.tcl
B_configure $fzo -Vider_peres -Vider_fils -Origine 0 0 -Ajouter_fils $p_o
B_configure $p_o -Info_texture [$img_chat Info_texture] -Ajouter_fils $fzo -maj -Forcer_pre_rendu_noeud 1

proc escalier {icl f nb} {
 $icl Allouer $nb
 $icl Nb $nb
 for {set i 0} {$i<$nb} {incr i 2} {
  set x [expr $i/$nb.0]; set x2 [expr ($i+1)/$nb.0]; $icl Affecter $i $x $x; $icl Affecter [expr $i+1] $x2 $x;
 }
 [$f Rep_lentille] maj_lentille $icl
 $f maj_lentille
}


#______________________________________________________________________________________________________
#______________________________________ Effets temporels ______________________________________________
#______________________________________________________________________________________________________
 # On définit une fonction de rappel, qui dira quoi faire
proc toto_essai {poly_root} {
 set transfo [B_transfo 5000]
 set rap_transfo_pdt [B_rappel [Interp_TCL] ]
 set rap_transfo_fin [B_rappel [Interp_TCL] "puts \"C'est la fin !!!!\""]
 $rap_transfo_pdt Texte "set v \[$transfo V_courant\]; $poly_root Rotation \[expr [$poly_root Rotation]+\$v*2*3.1415\];"

 # On s'abonne aux appels à la transformation (==deformation dynamique), on demande de déclancher la fonction de rappel rap_transfo_pdt
 $transfo abonner_a_rappel_pendant [$rap_transfo_pdt Rappel]
 $transfo abonner_a_rappel_fin     [$rap_transfo_fin Rappel]
 
# Pour démarrer la transformation, on la place dans l'interface (N_i_mere), ensuite on démarre
 N_i_mere Ajouter_deformation_dynamique $transfo; $transfo Demarrer
}


#______________________________________________________________________________________________________
#______________________________________ Evennements pointeurs__________________________________________
#______________________________________________________________________________________________________
set rap_poly_ovale_relache [B_rappel [Interp_TCL] "toto_essai $poly_root"]
$poly_root abonner_a_detection_pointeur [$rap_poly_ovale_relache Rappel]

# Texte
set txt [B_texte 500 300 290 [fonte_Arial] [B_sim_sds]]


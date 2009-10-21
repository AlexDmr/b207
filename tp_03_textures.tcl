if {[info exists noeud_partage]} {} else {
 source groupe.tcl
 proc etire_fond args {}
 N_i_mere Afficher_souris
}

#______________________________________________________________________________________________________
# ___ Définissons un joli polygone en forme de fleur (à base de non moins joli sinus et cosinus) ______
#______________________________________________________________________________________________________
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

#______________________________________ Evennements pointeurs__________________________________________
# On s'abonne sur ce polygone à un relachement de pointeur, si on en détecte un, on déclanche une animation
# voir la procédure 'toto_essai' définit à la fin de ce fichier.
# Un abonnement se fait TOUJOURS à l'aide d'une méthode de rappel B207 (B_rappel) 
# qui prend TOUJOURS comme premier argument [Interp_TCL] (l'interpréteur TCL courant)
# et OPTIONNELLEMENT un deuxième argument contenant un programme TCL à interpréter.
set rap_poly_ovale_relache [B_rappel [Interp_TCL] "toto_essai $poly_root"]
$poly_root abonner_a_detection_pointeur [$rap_poly_ovale_relache Rappel] 2

#_____________________________ Configuration des textures et déplacements_______________________________
B_contact ctc_poly_root "$poly_root 3" -pt_trans 0 0 -pt_rot 0 0
set img_poly_root [B_image terrain.png]
set info_texture  [$img_fond Info_texture]
B_configure $poly_root -Info_texture $info_texture \
                       -Info_texture2 [$img_poly_root Info_texture] \
                       -Combinaison_textures [GLenum2UI [GL_interpolate_ARB]] [GLenum2UI [GL_add]] \
				       -Combinaison_textures_operande2 [GLenum2UI [GL_src_alpha]] \
				       -Combinaison_textures_source_arg2 [GLenum2UI [GL_texture]]

#______________________________________________________________________________________________________
#____________________ Définissions d'un polygone masquant (clipping) et textures multiples ____________
#______________________________________________________________________________________________________
# Définissons maintenant un polygone en forme d'ovale (p_o) qu'on pourra translater et tourner
set p_o [B_polygone]
  $noeud_partage Ajouter_fils $p_o
$p_o Ajouter_contour [ProcOvale 0 0 200 200 60]
B_contact ctc_p_o "$p_o 3" -pt_trans 0 0 -pt_rot 0 0

#__________________ Définissons les images à utiliser pour les textures _______________________________
# On place ces images dans le graphe de scène mais on ne les affiche pas.
set img_reflet [B_image s3.png]
  $noeud_partage Ajouter_fils $img_reflet; $img_reflet Afficher_noeud 0; $img_reflet Gerer_contacts 0
set img_chat   [B_image chat_souris.png]
  $noeud_partage Ajouter_fils $img_chat  ; $img_chat   Afficher_noeud 0; $img_chat   Gerer_contacts 0
  
#__________________ Définissons quelques élément à placer dans le polygone masquant p_o________________
set p_txt [B_polygone]
  $p_txt Ajouter_contour [ProcTabDouble {0 0 500 0 500 500 0 500}]
  B_configure $p_txt -Couleur 0.6 0.25 0 0.8 -Mode_texture 0 -Mode_texture_fils 1
set z_txt [B_texte 500 300 50 [fonte_Arial] [B_sim_sds]]
  # Le constructeur de la zone de texte prend pour paramètre :
  ### -Largeur de la zone : ici 500
  ### -Hauteur de la zone : ici 400
  ### -Taille de la fonte : ici 50
  ### -la fonte           : ici (et toujours) [fonte_Arial]
  ### -le gestionnaire de dispositifs de saisis (comme le clavier) : ici (et toujours) [B_sim_sds]
  B_configure $z_txt -Origine 0 100 \
                     -TEXTE "Chargez des lentilles dynamiques avec la procedure load_len:\n  -load_len vague.len\n  -load_len formules.len"  \
  				     -Editable 0 \
					 -Noeud_touchable 0
  $p_txt Ajouter_fils $z_txt
  $p_txt PreRendre
  B_contact ctc_p_txt "$p_txt 1"

# On paramètre p_o pour gérer plusieurs textures
# Les deux première lignes donnent les textures à utiliser
# Les trois secondes spécifient (optionnelle) comment elles se combinent
# La suivante Ajoute le noeud contenant le texte comme fils de p_o
# Les trois dernières spécifient que p_o va être rendu comme texture (Mode_texture 1)
#   que cette texture contiendra aussi l'image des descendants de p_o (Mode_texture_fils 1)
#   et enfin Mode_texture2 permet de réaliser le masquage (clipping) en se basant sur la boite englobante de p_o et sur la forme de p_o dans un deuxième temps
B_configure $p_o -Info_texture  [$img_chat   Info_texture] \
                 -Info_texture2 [$img_reflet Info_texture] \
                 -Combinaison_textures [GLenum2UI [GL_interpolate_ARB]] [GLenum2UI [GL_add]] \
				 -Combinaison_textures_operande2 [GLenum2UI [GL_src_alpha]] \
				 -Combinaison_textures_source_arg2 [GLenum2UI [GL_texture]] \
				 -Ajouter_fils $p_txt \
				 -Mode_texture      1 \
				 -Mode_texture_fils 1 \
				 -Mode_texture2     1

# On définit un polygone qui contiendra la texture numéro 1 de p_o pour voir la différence entre Mode_texture2 = 0 ou 1
set p_test [B_polygone]
$p_test Ajouter_contour [ProcTabDouble {0 0 200 0 200 200 0 200}]
B_configure $noeud_partage -Ajouter_fils $p_test -Origine 500 0
$p_test Info_texture [$p_o Info_texture_scene]

set rap_chgmnt_dim_tex_scene_p_o [B_rappel [Interp_TCL] "$p_o Mode_texture2 1"]
[$p_o Info_texture_scene] abonner_a_changement_dim [$rap_chgmnt_dim_tex_scene_p_o Rappel]

source zone_dessin_lentille.tcl
B_configure $fzo -Vider_peres -Vider_fils -Origine 0 0 -Ajouter_fils $p_o
B_configure $p_o -Info_texture [$img_chat Info_texture] -Ajouter_fils $fzo -maj -Forcer_pre_rendu_noeud 1

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




source commandes_bigre.tcl

set ecran_x [$n_i_mere Largeur]
set ecran_y [$n_i_mere Hauteur]

proc Visu_Cam {} {
 global visu_cam
   if {![info exists visu_cam]} {
     set visu_cam [B_camera]
     $visu_cam Afficher_noeud 0
     $visu_cam Gerer_contacts 0
     [N_i_mere Noeud_scene] Ajouter_fils $visu_cam
    }
 return $visu_cam
}

if {[info exists visu_cam]} {} else {set pre_rendeur [$n_i_mere Prerendeur]
                                     set visu_cam [$visu_cam_bigre 320 240 $pre_rendeur]
                                     $visu_cam Afficher_noeud 0
                                     $visu_cam Gerer_contacts 0
                                     $Noeud_scene Ajouter_fils $visu_cam }

set texture_cam [$visu_cam Info_texture]
set img_cam [$image_bigre $texture_cam 320 240]

$Noeud_scene Position_des_fils_changeable 0
$Noeud_scene Ajouter_fils_au_debut $img_cam
$img_cam Etirement [expr [$n_i_mere Largeur]/320.0] [expr [$n_i_mere Hauteur]/240.0]
$img_cam Inverser_x 1
$img_cam Lisser 1
$img_cam Couleur 1 1 1 0.33

set pot_pix_mirroirs [$potentiometre_bigre $n_i_mere 256 16 -1 1]
$pot_pix_mirroirs Valeur [expr round(256*[$img_cam Composante_couleur 3])]

$Noeud_scene Ajouter_fils_au_debut $pot_pix_mirroirs
$pot_pix_mirroirs Origine 10 10
$pot_pix_mirroirs Valeur_min 0
$pot_pix_mirroirs Valeur_max 256
$pot_pix_mirroirs Mettre_a_jour

# Les fonctions de rappel pour la gestion de la position et de la translucidité du potentiomètre controlant les pixels mirroirs
set rap_quand_contient [$Rappel_TCL $interp_tcl ""]

set rap_chgmnt_pix_mir [$Rappel_TCL $interp_tcl "$img_cam Composante_couleur 3 \[expr \[$pot_pix_mirroirs Valeur\]/\[$pot_pix_mirroirs NbDivisions\].0\]"]
$pot_pix_mirroirs abonner_a_valeur [$rap_chgmnt_pix_mir Rappel]


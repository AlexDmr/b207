puts "Visualisateur camera"
set pre_rendeur [$n_i_mere Prerendeur]
puts " 1"
set visu_cam [$visu_cam_bigre 320 240 $pre_rendeur]
puts " 2"
$visu_cam Afficher_noeud 0
$visu_cam Gerer_contacts 0

puts " CamNote"
set CamNote [$CamNote_bigre $n_i_mere $visu_cam $interp_tcl]
puts "  C1"
$Noeud_scene Retirer_fils $CamNote
puts "  C2"

#___________________________________________________________________________
#_______________ Ajout du bouton gérant le plein écran _____________________
#___________________________________________________________________________
puts "Ajout du bouton de control"
set bt_plein_ecran [$bouton_bigre 200 200 $n_i_mere $Noeud_scene 2]
[$bt_plein_ecran Noeud_fen] Fenetre_translucide 1
set f_bt [$bt_plein_ecran Noeud_fen]
#$bt_plein_ecran Translation 100 100
$CamNote Ajouter_fils_au_debut $bt_plein_ecran
#___ Ajout du polygone dessin ____
puts "  Poly couleur OK"
set tab_ovale [ProcOvale 0 0 100 15 60]
puts "   1"
set poly_couleur [B_polygone]
puts "   2"
$poly_couleur Ajouter_contour 60 [$tab_ovale Tab]
puts "   3"
set poly [B_polygone]
$poly Ajouter_contour 60 [$tab_ovale Tab]
set nb_pas 16
for {set i 0} {$i < $nb_pas} {incr i} {
 $poly Rotation [expr $i*2*3.141592/$nb_pas]
 $poly_couleur Union $poly
}
puts "  Poly couleur moins rond central"
set tab_rond [ProcOvale 0 0 25 25 60]
  $poly_couleur Retirer_contour 60 [$tab_rond Tab]
set poly2 [B_polygone];
$poly_couleur maj
  $poly2 Union $poly_couleur
  $poly2 Etirement 0.5 0.5
$poly_couleur Difference $poly2
puts "   COCO"
$poly_couleur Fct_interpollation [get_Fonction_interpolation_poly_boite_englobante]
  set tab_col [B_TabDouble]
  $tab_col Reserver_place 16
  $tab_col Ajouter 0  0 1 0 1
  $tab_col Ajouter 4  1 0 1 0.5
  $tab_col Ajouter 8  1 0 1 0.5
  $tab_col Ajouter 12 0 1 0 1
$poly_couleur Calculer_boites
$poly_couleur Coords_couleurs [$tab_col  Tab]
$poly_couleur maj
$poly_couleur Noeud_touchable 0
$poly_couleur Origine 100 100
$poly_couleur Pivoter_interne [expr -3.14159265/3] 0 0
$poly_couleur Translucidite 0
$f_bt Couleur_fond 0 0 0 0
$f_bt Ajouter_fils $poly_couleur
$bt_plein_ecran Mettre_a_jour
  $bt_plein_ecran Nom_IU "Bouton\nplein ecran"
  $poly_couleur   Nom_IU "Joli\npolygone"

#___________________________________________________________________________
#__ Gestion de la commande de rappel du bouton en fonction du plein écran __
#___________________________________________________________________________
puts "Ajout de la procédure de mise ne plein écran"
source CamNote_Plein_Ecran.tcl

CamNote_Plein_Ecran CPE $fvcn $bt_plein_ecran $n_i_mere
CPE Etat_pas_plein_ecran

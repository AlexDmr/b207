if {[info exists noeud_partage]} {} else {source groupe.tcl}
proc etire_fond args {}
N_i_mere Afficher_souris


#cr�ation des points pour le carre
# On r�serve 8 double puis on les d�finit 2 � 2.
set tab1 [ProcRect 0 0 100 100]
 
#cr�ation des points pour les cercles
set tab_cercle [ProcOvale 0 0 30 30 64]
 
#Cr�ation du carre 
set poly [B_polygone] 
$poly Ajouter_contour $tab1
$poly Couleur 0.1 0.1 0.1 0.7

#Cr�ation des cercles
set tour1 [B_polygone]
$tour1 Ajouter_contour $tab_cercle
 
#Ajout du carre au noeud racine 
$noeud_partage Ajouter_fils $poly 
 
#Ajout du cercle de 4 fa�on diff�rente au noeud carre, en passant par des noeud de 
# transformations interm�diaires tX.
set t1 [B_noeud]; $t1 Origine   0   0; $poly Ajouter_fils $t1; $t1 Ajouter_fils $tour1
set t2 [B_noeud]; $t2 Origine 100   0; $poly Ajouter_fils $t2; $t2 Ajouter_fils $tour1
set t3 [B_noeud]; $t3 Origine 100 100; $poly Ajouter_fils $t3; $t3 Ajouter_fils $tour1
set t4 [B_noeud]; $t4 Origine   0 100; $poly Ajouter_fils $t4; $t4 Ajouter_fils $tour1

# Gestion des contacts
#cont            : nom du gestionnaire
#"$poly 1"       : Les transformations porteront sur $poly, $poly peut effectuer la transormation 1 (translation).
#-add "$tour1 2" : Ajout de $tour qui permettra de faire des operation de type 2 (rotation) qui seront appliqu�es sur $poly.
#-pt_trans 50 50 : Sp�cifier le point de translation par d�faut. Utile en mode 3.
#-pt_rot 50 50   : Sp�cifier le point de rotation par d�faut. Utile en mode 2.
B_contact cont "$poly 9" -add "$tour1 2" -pt_trans 50 50 -pt_rot 50 50



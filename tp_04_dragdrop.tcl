if {[info exists noeud_partage]} {} else {
 source groupe.tcl
 proc etire_fond args {}
 N_i_mere Afficher_souris
}


#______________________________________________________________________________________________________
#________ Exemple minimaliste : 2 éléments draggable, 1 zone de drop ___________________________________
#______________________________________________________________________________________________________
set panel     [B_polygone]
  B_configure $panel     -Ajouter_contour [ProcRect 0 0 200 600] \
                         -Couleur 0.3 0.3 0.3 1

set drag_poly [B_polygone]
  B_configure $drag_poly -Ajouter_contour [ProcOvale 0 0 75 50 60] \
                         -Couleur 0.3 0.7 0.5 1 \
						 -Origine 100 100 \
						 -Calculer_boites \
						 -Ajouter_MetaData_T type AMI
						 
set drag_poly_mauvais [B_polygone]
  B_configure $drag_poly_mauvais -Ajouter_contour [ProcOvale 0 0 75 50 60] \
                                 -Couleur 0.4 0.2 0.8 1 \
						         -Origine 100 300 \
								 -Calculer_boites \
						         -Ajouter_MetaData_T type PAS_AMI

set drop_zone [B_polygone]
  B_configure $drop_zone -Ajouter_contour [ProcRect 0 0 600 600] \
                         -Couleur 0 0 0.5 1 \
						 -Origine 200 0

# Branchons tout ça dans le graphe de scène
set root_panel_drop [B_noeud]
$noeud_partage Ajouter_fils $root_panel_drop
B_configure $root_panel_drop -Ajouter_fils $panel \
                             -Ajouter_fils $drop_zone  \
							 -Position_des_fils_changeable 0
						   
B_configure $panel         -Ajouter_fils $drag_poly \
                           -Ajouter_fils $drag_poly_mauvais 


#______________________________________________________________________________________________________
# Définition des propriétés de grag&drop ______________________________________________________________
# Drag_nodes prend en paramètre une liste de noeuds, ces noeuds deviennent alors draggable ____________
# Drop_zones prend une liste de quintuplet de type <noeud, condition, action, onEnter, onLeave> où : __
#   - noeud     : le noeud zone de drop _______________________________________________________________
#   - condition : la condition à évaluer pour savoir si un élément peut être droppé sur cette zone ____
#                 Cette condition peut s'appuyer sur $n, le noeud en cours de drag ____________________
#   - action    : l'action à déclancher si le drop est effectué (condition précédente vérifiée) _______
#   - onEnter   : Action à déclancher si un draggable compatible passe au dessus de la zone ___________
#   - onLeave   : Action à déclancher si un draggable quitte la zone __________________________________
#______________________________________________________________________________________________________
Drag_nodes   [list $drag_poly $drag_poly_mauvais]
Drop_zones D [list [list $drop_zone "Is_droppable_here \$n" "Add_new_instance" "$drop_zone Couleur 0 0 1 1" "$drop_zone Couleur 0 0 0.5 1"]]

#______________________________________________________________________________________________________
proc Is_droppable_here {node} {
 return [string equal [$node Val_MetaData type] AMI]
}

#______________________________________________________________________________________________________
#______________________________________________________________________________________________________
#______________________________________________________________________________________________________
proc Add_new_instance {drop_zone infos} {
# On récupère le pointeur 
 set ptr   [$infos Ptr]
# On récupère le noeud de placement de l'élément en cours de drag
 set n_plc [$ptr Val_MetaData Noeud_placement_drag]

 puts "BRAVO!!! Drop $infos on $drop_zone"
 set x            [$infos X_au_contact]
 set y            [$infos Y_au_contact]
# On peut récupérer le noeud en cours de drag (pour obtenir des informations sur lui par exemple)
 set dragged_node [Get_node_dragged_in $infos]
 puts "dragged_node : $dragged_node"
 
# Ici, on se contente d'ajouter un polygone de forme identique sur la zone de drop, à l'endroit précis du drop.
 set new_poly [B_polygone]
 B_configure $new_poly -Ajouter_contour [ProcOvale 0 0 75 50 60] \
                       -Couleur 1 1 1 1 \
					   -Origine [expr $x + [$n_plc Px]] [expr $y + [$n_plc Py]]
 $drop_zone Ajouter_fils_au_debut $new_poly
}



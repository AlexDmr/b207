source tp_05_animations.tcl

#______________________________________________________________________________________________________
# Dans ce TP, on ajoute une fonctionnalité de dessin à la zone de drop, l'utilisateur doit pouvoir ____
# dessiner des cercles ou placer de petits carrés. ____________________________________________________
# Ce TP montre comment s'abonner à la souris indirectement (évennements 'PRESS' sur un polygone _______
# et directement (abonnement à toute modification d'un pointeur lui même). ____________________________
# On définit une méthode de rappel qui sera déclanchée à chaque fois qu'un pointeur sera pressé _______
# sur la zone de drop, la procédure Start_drawing est appelée avec la méthode de rappel en argument ___
#______________________________________________________________________________________________________
set rap_PRESS_drop [B_rappel [Interp_TCL]]
  $rap_PRESS_drop Texte "Start_drawing $rap_PRESS_drop"
  $drop_zone abonner_a_detection_pointeur [$rap_PRESS_drop Rappel] [ALX_pointeur_enfonce]
  
#______________________________________________________________________________________________________
# Le rappel aura lieu lors d'un évennement pointeur sur un noeud... ___________________________________
# Le paramètre associé au rappel sera donc du type informations... ____________________________________
#______________________________________________________________________________________________________
proc Start_drawing {rap} {
 set infos [Void_vers_info [$rap Param]]
 set noeud [$infos NOEUD]
 set ptr   [$infos Ptr]
 set x     [$infos X_au_contact]
 set y     [$infos Y_au_contact]
 
# On vérifie si ce pointeur n'est pas déja utilisé sur ce polygone
# On se base sur une métadonnée associée au noeud: Start_drawing_$ptr 
 if {[$noeud Val_MetaData Start_drawing_$ptr] != ""} {puts "$ptr est déja utilisé avec $noeud"; return}

# On peut (par exemple) enregistrer le point dans une métadonnée associé au noeud
 $noeud Ajouter_MetaData_T Start_drawing_$ptr "$x $y"
# Symétriquement, on associe au pointeur le noeud
 $ptr Add_MetaData Start_drawing_on $noeud
 
# On s'abonne ensuite à des changements qui pourraient survenir sur ce pointeur
# De cette façon, on est au courant des modifications même si le pointeur sors du polygone
# La fonction de rappel est ensuite enregistré dans le noeud, de sorte à ne pas avoir à la
# recréer à chaque fois. On commence donc par vérifier si elle n'existe pas déja, on la crée
# le cas échéant.
 set rap_ptr_change [$noeud Val_MetaData rap_ptr_change]
 set L_noeuds       [$noeud Val_MetaData L_noeuds]
 if {$rap_ptr_change == ""} {
   # S'il n'existe pas, il faut le créer
   set rap_ptr_change [B_rappel [Interp_TCL]]
     $noeud Ajouter_MetaData_T rap_ptr_change $rap_ptr_change
   
   set L_noeuds [Liste_alx_repere2D]
     $noeud Ajouter_MetaData_T L_noeuds $L_noeuds
	 $noeud Ajouter_MetaData_T pt_tmp   [B_point]
  }
 $rap_ptr_change Texte "Pointer_has_changed $x $y $rap_ptr_change"
 # On s'abonne effectivement au changements du pointeur.
 $ptr abonner_a_changement [$rap_ptr_change Rappel]
 # On en profite pour enregistrer la liste de repères qui a mené de la racine au noeud
 $L_noeuds maj [$infos L_REPERES]
   Ajouter_noeud_en_fin_de_liste_rep $L_noeuds $noeud
 
 puts "Pointeur $ptr is now associated with node $noeud at <$x;$y>, procedure Pointer_has_changed will be triggered"
}

#______________________________________________________________________________________________________
# Le rappel aura lieu lors d'une modification d'un pointeur... ________________________________________
# Le paramètre associé au rappel sera donc du type pointeur... ________________________________________
#______________________________________________________________________________________________________
proc Pointer_has_changed {x y rap} {
# Retouvons les informations utiles 
 set ptr   [Void_vers_alx_pointeur [$rap Param]]
   set noeud      [$ptr   Val_MetaData Start_drawing_on]
     set pt       [$noeud Val_MetaData pt_tmp]; $pt maj [$ptr Point]
     Repere_transformation $pt [$noeud Val_MetaData L_noeuds]
 
# Calcul de la distance par rapport au premier contact
 set dx [expr [$pt X]-$x]
 set dy [expr [$pt Y]-$y]
 set d  [expr sqrt($dx*$dx + $dy*$dy)]

 # Si le pointeur s'est éloigné de plus de 10 unités, on passe en mode traçage de cercle
 if {$d > 10} {
   set poly_circle [B_polygone]
     $noeud Ajouter_fils_au_debut $poly_circle
     set contour [ProcOvale 0 0 $d $d 60]
	 $poly_circle Origine $x $y
	 $poly_circle Ajouter_contour $contour
	 Detruire $contour
   $rap Texte "if {\[catch {Draw_circle $poly_circle $x $y $rap} err\]} {puts \"ERROR in Draw_circle:\n\$err\"}"
  }
 
 # Si on est là, c'est que le pointeur s'est éloigné de moins de 10 unités
 # si on relache, on pose un petit carré
 if {[$ptr Appuie] == 0} {
   puts "On a complètement relaché le pointeur $ptr !"
   End_process $rap $ptr $noeud
   set poly_carre [B_polygone]
     $poly_carre Ajouter_contour       [ProcRect -10 -10 10 10]
	 $poly_carre Origine $x $y
     $noeud      Ajouter_fils_au_debut $poly_carre
  }
}


#______________________________________________________________________________________________________
proc Draw_circle {poly_circle cx cy rap} {
# Retouvons les informations utiles 
 set ptr   [Void_vers_alx_pointeur [$rap Param]]
   set noeud      [$ptr   Val_MetaData Start_drawing_on]
     set pt       [$noeud Val_MetaData pt_tmp]; $pt maj [$ptr Point]
     Repere_transformation $pt [$noeud Val_MetaData L_noeuds]

# Calcul de la distance par rapport au premier contact
 set dx [expr [$pt X]-$cx]
 set dy [expr [$pt Y]-$cy]
 set d  [expr sqrt($dx*$dx + $dy*$dy)]

# Doit-on arréter le tracé (plus d'appuie) ou le continuer?
 if {[$ptr Appuie] == 0} {
   puts "On a complètement relaché le pointeur $ptr !"
   End_process $rap $ptr $noeud
  } else {set contour [ProcOvale 0 0 $d $d 60]
            $poly_circle Vider
			$poly_circle Ajouter_contour $contour
          Detruire $contour
		 } 
}

#______________________________________________________________________________________________________
# End_process désabonne la fonction de rappel et réinitialise les métadonnées. ________________________
#______________________________________________________________________________________________________
proc End_process {rap ptr noeud} {
 $ptr   desabonner_de_changement [$rap Rappel]
 $noeud Retirer_MetaData_T Start_drawing_$ptr
}

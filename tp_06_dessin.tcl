source tp_05_animations.tcl

#______________________________________________________________________________________________________
# Dans ce TP, on ajoute une fonctionnalit� de dessin � la zone de drop, l'utilisateur doit pouvoir ____
# dessiner des cercles ou placer de petits carr�s. ____________________________________________________
# Ce TP montre comment s'abonner � la souris indirectement (�vennements 'PRESS' sur un polygone _______
# et directement (abonnement � toute modification d'un pointeur lui m�me). ____________________________
# On d�finit une m�thode de rappel qui sera d�clanch�e � chaque fois qu'un pointeur sera press� _______
# sur la zone de drop, la proc�dure Start_drawing est appel�e avec la m�thode de rappel en argument ___
#______________________________________________________________________________________________________
set rap_PRESS_drop [B_rappel [Interp_TCL]]
  $rap_PRESS_drop Texte "Start_drawing $rap_PRESS_drop"
  $drop_zone abonner_a_detection_pointeur [$rap_PRESS_drop Rappel] [ALX_pointeur_enfonce]
  
#______________________________________________________________________________________________________
# Le rappel aura lieu lors d'un �vennement pointeur sur un noeud... ___________________________________
# Le param�tre associ� au rappel sera donc du type informations... ____________________________________
#______________________________________________________________________________________________________
proc Start_drawing {rap} {
 set infos [Void_vers_info [$rap Param]]
 set noeud [$infos NOEUD]
 set ptr   [$infos Ptr]
 set x     [$infos X_au_contact]
 set y     [$infos Y_au_contact]
 
# On v�rifie si ce pointeur n'est pas d�ja utilis� sur ce polygone
# On se base sur une m�tadonn�e associ�e au noeud: Start_drawing_$ptr 
 if {[$noeud Val_MetaData Start_drawing_$ptr] != ""} {puts "$ptr est d�ja utilis� avec $noeud"; return}

# On peut (par exemple) enregistrer le point dans une m�tadonn�e associ� au noeud
 $noeud Ajouter_MetaData_T Start_drawing_$ptr "$x $y"
# Sym�triquement, on associe au pointeur le noeud
 $ptr Add_MetaData Start_drawing_on $noeud
 
# On s'abonne ensuite � des changements qui pourraient survenir sur ce pointeur
# De cette fa�on, on est au courant des modifications m�me si le pointeur sors du polygone
# La fonction de rappel est ensuite enregistr� dans le noeud, de sorte � ne pas avoir � la
# recr�er � chaque fois. On commence donc par v�rifier si elle n'existe pas d�ja, on la cr�e
# le cas �ch�ant.
 set rap_ptr_change [$noeud Val_MetaData rap_ptr_change]
 set L_noeuds       [$noeud Val_MetaData L_noeuds]
 if {$rap_ptr_change == ""} {
   # S'il n'existe pas, il faut le cr�er
   set rap_ptr_change [B_rappel [Interp_TCL]]
     $noeud Ajouter_MetaData_T rap_ptr_change $rap_ptr_change
   
   set L_noeuds [Liste_alx_repere2D]
     $noeud Ajouter_MetaData_T L_noeuds $L_noeuds
	 $noeud Ajouter_MetaData_T pt_tmp   [B_point]
  }
 $rap_ptr_change Texte "Pointer_has_changed $x $y $rap_ptr_change"
 # On s'abonne effectivement au changements du pointeur.
 $ptr abonner_a_changement [$rap_ptr_change Rappel]
 # On en profite pour enregistrer la liste de rep�res qui a men� de la racine au noeud
 $L_noeuds maj [$infos L_REPERES]
   Ajouter_noeud_en_fin_de_liste_rep $L_noeuds $noeud
 
 puts "Pointeur $ptr is now associated with node $noeud at <$x;$y>, procedure Pointer_has_changed will be triggered"
}

#______________________________________________________________________________________________________
# Le rappel aura lieu lors d'une modification d'un pointeur... ________________________________________
# Le param�tre associ� au rappel sera donc du type pointeur... ________________________________________
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

 # Si le pointeur s'est �loign� de plus de 10 unit�s, on passe en mode tra�age de cercle
 if {$d > 10} {
   set poly_circle [B_polygone]
     $noeud Ajouter_fils_au_debut $poly_circle
     set contour [ProcOvale 0 0 $d $d 60]
	 $poly_circle Origine $x $y
	 $poly_circle Ajouter_contour $contour
	 Detruire $contour
   $rap Texte "if {\[catch {Draw_circle $poly_circle $x $y $rap} err\]} {puts \"ERROR in Draw_circle:\n\$err\"}"
  }
 
 # Si on est l�, c'est que le pointeur s'est �loign� de moins de 10 unit�s
 # si on relache, on pose un petit carr�
 if {[$ptr Appuie] == 0} {
   puts "On a compl�tement relach� le pointeur $ptr !"
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

# Doit-on arr�ter le trac� (plus d'appuie) ou le continuer?
 if {[$ptr Appuie] == 0} {
   puts "On a compl�tement relach� le pointeur $ptr !"
   End_process $rap $ptr $noeud
  } else {set contour [ProcOvale 0 0 $d $d 60]
            $poly_circle Vider
			$poly_circle Ajouter_contour $contour
          Detruire $contour
		 } 
}

#______________________________________________________________________________________________________
# End_process d�sabonne la fonction de rappel et r�initialise les m�tadonn�es. ________________________
#______________________________________________________________________________________________________
proc End_process {rap ptr noeud} {
 $ptr   desabonner_de_changement [$rap Rappel]
 $noeud Retirer_MetaData_T Start_drawing_$ptr
}

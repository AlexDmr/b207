#source {C:\Alexandre\MAGISTERE2\Plasticité\Jeu de modeles physiques\essai_borland_son_sdl_opengl\CamNote_evolution.tcl}
# On suppose en entrant ici les choses suivantes:
# Les variables suivantes sont définies :
#     CometCamNote  : Le système interactif CamNote
#     Cam_visu : Le visualisateur de diapo
#     Cam_tele : La télécommande
#     classe   : La classe de CamNote (vis à vis de gmlObject) qui hérite de c_0

source raccourci.tcl

proc centrer {} {
 global fvcn
 $fvcn Origine 0 0
 $fvcn Rotation 0
 $fvcn Longueur_corp 1024
 $fvcn Hauteur_corp 768
 $fvcn Mettre_a_jour
}

method $classe set_Telec_visible {b} {set this(telec_visible) $b}
method $classe get_Telec_visible {} {return $this(telec_visible)}
method $classe Etat_init {} {
 set this(la_deformation) 0
 set this(telec_visible) 1
 return Etat_00
 }

# Les méthodes pour montrer ce qu'on peut faire
method $classe Etat_sans_fusion {} {
 global classe

 set p "${classe}_p${this(nb_props)}"
 alx_proposition $p
 incr this(nb_props)
 $p set_Conviction 1
 $p set_Fonction "Fusionner_telec"
 lappend this(liste_decons) $p
 return Etat_sans_fusion
}

# Le reste
method $classe Proposer_fusion_telec {t c} {
   global classe
   set p "${classe}_p${this(nb_props)}"
   alx_proposition $p
   incr this(nb_props)
   $p set_Conviction $c
   $p set_Fonction "Fusionner_telec $t"
   lappend this(liste_cons) $p
 }
method $classe Etat_00 {} {
# Etat où la télécommande est en mode graphique
 global fvcn
 global ftcn
 global classe
 set tx [$fvcn Longueur_corp]
 set ty [$fvcn Hauteur_corp]

 if {$tx > 800 && $ty > 600 && ($this(la_deformation) == 0) && $this(telec_visible)} {
   this Proposer_fusion_telec 2000 0.8}

 if {!$this(telec_visible)} {return Etat_01}
 return Etat_00;
}

method $classe Proposer_fission_telec {t c} {
   global classe
   set p "${classe}_p${this(nb_props)}"
   alx_proposition $p
   incr this(nb_props)
   $p set_Conviction $c
   $p set_Fonction "Fissionner_telec $t"
   lappend this(liste_cons) $p
 }
method $classe Etat_01 {} {
# Etat où la télécommande est en mode clavier
 global fvcn
 global ftcn
 global classe
 set tx [$fvcn Longueur_corp]
 set ty [$fvcn Hauteur_corp]

 if { ($tx < 800 || $ty < 600) && ($this(la_deformation) == 0) && !$this(telec_visible)} {
   this Proposer_fission_telec 2000 0.8}

 if {$this(telec_visible)} {return Etat_00}
 return Etat_01;
}

# On spécifie les fonctions à appliquer en cas d'évolution, qui ont été citée dans le modèle
method $classe Fusionner_telec {t} {
 global fvcn
 global ftcn
 global nftcn
## On enregistre la position courante de la télécommande en vue de l'utiliser lors de la fission
 set this(translucidite_svg_telec) [$ftcn Translucidite_corp]
 set this(repere_svg_telec) [new_alx_repere2D $nftcn]

## Il faudra initialiser une deformation dynamique avec étirement de fenêtre.
 set this(la_deformation) [new_deformation_dynamique_etirement_fenetre $t $ftcn $this(translucidite_svg_telec) $fvcn 0]
 $this(la_deformation) Demarrer
 return "Fusion_telec $this(la_deformation) [$this(la_deformation) Deformation_dynamique_repere]"
}
method $classe Fusion_telec {def_fen def_rep} {
 # Il faudra faire évoluer la déformation
 set t [$def_rep Position_temporelle_relative]
 $def_fen Simuler $t
 if {$t >= 1} {return "terminer_fusion_telec $def_fen"}

 return "Fusion_telec $def_fen $def_rep"
}

method $classe rien {} {}
method $classe terminer_fusion_telec {def_fen} {
 global fvcn
 global ftcn
 global nccn
 global CometCamNote
 global ccn_bigre

# Tuer la transfo
 set $def_fen ""
 set this(la_deformation) 0
# Fenêtre à ne pas mettre translucide.
 $ftcn Fenetre_translucide 0
 set nftcn [$ftcn Noeud_scene]
 $nccn Retirer_fils $nftcn
 set this(telec_visible) 0

 $ftcn Activer_pilotage_clavier
 $ccn_bigre Emission_66 "SLIDEREMOTE LAUNCH UI"
 for {set i 0} {$i < 100000} {incr i} {set coco $i}
 $ccn_bigre Emission_66 "SLIDEREMOTE CURRENT SLIDE IS [$ccn_bigre Num_transparent]"
 return terminer
}

method $classe Fissionner_telec {t} {
 global nftcn
 global nfvcn
 global ftcn
 global fvcn
 global nccn
 $ftcn Fenetre_translucide 1
 $ftcn Translucidite_globale 0
 $ftcn Mettre_a_jour
 if {[$nccn A_pour_fils $nftcn]} {$nccn Retirer_fils $nftcn}
 $nccn Ajouter_fils_au_debut $nftcn
## Il faudra initialiser une deformation dynamique avec étirement de fenêtre.
 set this(la_deformation) [new_deformation_dynamique_etirement_fenetre $t $ftcn 0 $this(repere_svg_telec) $this(translucidite_svg_telec) 0 0]

 $ftcn  Position_bg [$fvcn Position_bg]
 $nftcn Rotation    [$nfvcn Rotation]
 set ex [expr [$fvcn Longueur_corp] / [$ftcn Longueur_corp]]
 set ey [expr [$fvcn Hauteur_corp] / [$ftcn Hauteur_corp]]
 $nftcn Etirement $ex $ey
 $this(la_deformation) Demarrer
 return "Fission_telec $this(la_deformation) [$this(la_deformation) Deformation_dynamique_repere]";
}
method $classe Fission_telec {def_fen def_rep} {
 # Il faudra faire évoluer la déformation
 set t [$def_rep Position_temporelle_relative]
 $def_fen Simuler $t
 if {$t >= 1} {return "terminer_fission_telec $def_fen"}

 return "Fission_telec $def_fen $def_rep"
}
method $classe terminer_fission_telec {def_fen} {
 global ftcn
 global ccn_bigre
# Tuer la transfo
 set $def_fen ""
 set this(la_deformation) 0
# Fenêtre à ne pas mettre translucide.
 $ftcn Fenetre_translucide 0
 set this(telec_visible) 1

 $ccn_bigre Emission_66 "SLIDEREMOTE CLOSE"
 $ftcn Desactiver_pilotage_clavier

 return terminer
}


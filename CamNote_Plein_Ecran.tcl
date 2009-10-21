source gml_Object.tcl

#___________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________
method CamNote_Plein_Ecran constructor {fvcn bt n_i_mere} {
 set this(fvcn)     $fvcn
 set this(n_i_mere) $n_i_mere
 set this(interp_tcl) [$n_i_mere Interp_TCL]
 set this(fen_svg)  [B_fiche SVG 10 10 $n_i_mere]; $this(fen_svg) Nb_pixels_par_unite 0.000001
 set this(f_cible)  [B_fiche SVG 10 10 $n_i_mere]; $this(fen_svg) Nb_pixels_par_unite 0.000001
 set this(bt)       $bt
 set this(temps_passage_plein_ecran) 2000
 set this(temps_passage_fenetres)    2000

 set this(rap_bt_plein_ecran) [B_rappel $this(interp_tcl)]
 $bt abonner_a_activation [$this(rap_bt_plein_ecran) Rappel]
}

#___________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________
method CamNote_Plein_Ecran get_temps_passage_plein_ecran  {} {return $this(temps_passage_plein_ecran)}
method CamNote_Plein_Ecran set_temps_passage_plein_ecran {t} {set this(temps_passage_plein_ecran) $t}

#___________________________________________________________________________________________________________________________
method CamNote_Plein_Ecran get_temps_passage_fenetres     {} {return $this(temps_passage_fenetres)}
method CamNote_Plein_Ecran set_temps_passage_fenetres    {t} {set this(temps_passage_fenetres) $t}

#___________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________
method CamNote_Plein_Ecran Etat_pas_plein_ecran {} {
 set    cmd ""
 append cmd "$objName Passage_plein_ecran \[$objName get_temps_passage_plein_ecran\];\n"

 $this(rap_bt_plein_ecran) Texte $cmd
}

#___________________________________________________________________________________________________________________________
method CamNote_Plein_Ecran Passage_plein_ecran {t} {
# Création d'une transformation, enregistrement de la position actuelle de la fenêtre...
 $this(rap_bt_plein_ecran) Texte ""
 $this(fen_svg) maj $this(fvcn)
 $this(fen_svg) Longueur_corp [$this(fvcn) Longueur_corp]
 $this(fen_svg) Hauteur_corp  [$this(fvcn) Hauteur_corp]
 $this(f_cible) Origine 0 0
 $this(f_cible) Longueur_corp [$this(n_i_mere) Largeur]
 $this(f_cible) Hauteur_corp  [$this(n_i_mere) Hauteur]

 set transfo [B_transfo_fen $t $this(fvcn) $this(f_cible)]

 set this(rap_fin_plein_ecran) [B_rappel $this(interp_tcl) "$objName Etat_plein_ecran"]
 set this(rap_pdt_plein_ecran) [B_rappel $this(interp_tcl) "set v \[$transfo V_courant\]; set fen [$this(bt) Noeud_fen]; \
                                                           \$fen Fenetre_translucide 1;                                  \
                                                           set r \[expr 200-150*\$v\];                                    \
                                                           \$fen Longueur_corp \$r; \$fen Hauteur_corp \$r;              \
                                                           \$fen Translucidite_globale \[expr 1-0.8*\$v\];               \
                                                           \$fen Mettre_a_jour"]
 $transfo abonner_a_rappel_pendant [$this(rap_pdt_plein_ecran) Rappel]
 $transfo abonner_a_rappel_fin     [$this(rap_fin_plein_ecran) Rappel]
 $transfo Demarrer
 $this(n_i_mere) Ajouter_deformation_dynamique $transfo
}

#___________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________
method CamNote_Plein_Ecran Etat_plein_ecran {} {
 set    cmd ""
 append cmd "$objName Passage_fenetres \[$objName get_temps_passage_fenetres\];\n"

 $this(rap_bt_plein_ecran) Texte $cmd
}

#___________________________________________________________________________________________________________________________
method CamNote_Plein_Ecran Passage_fenetres {t} {
# Création d'une transformation, utilisation de l'enregistrement de la position actuelle de la fenêtre...
 $this(rap_bt_plein_ecran) Texte ""
 set transfo [B_transfo_fen $t $this(fvcn) $this(fen_svg)]

 set this(rap_fin_fenetres) [B_rappel $this(interp_tcl) "$objName Etat_pas_plein_ecran"]
 set this(rap_pdt_fenetres) [B_rappel $this(interp_tcl) "set v \[$transfo V_courant\]; set fen [$this(bt) Noeud_fen]; \
                                                         \$fen Fenetre_translucide 1;                                 \
                                                         set r \[expr 50+150*\$v\];                                  \
                                                         \$fen Longueur_corp \$r; \$fen Hauteur_corp \$r;             \
                                                         \$fen Translucidite_globale \[expr 0.2+0.8*\$v\];            \
                                                         \$fen Mettre_a_jour"]
 $transfo abonner_a_rappel_pendant [$this(rap_pdt_fenetres) Rappel]
 $transfo abonner_a_rappel_fin     [$this(rap_fin_fenetres) Rappel]
 $transfo Demarrer
 $this(n_i_mere) Ajouter_deformation_dynamique $transfo
}


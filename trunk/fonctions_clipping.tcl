proc Clipping {n_clip} {
 global interp_tcl
 
 if {[$n_clip Val_MetaData Clipping] != ""} {return}
 
 set cmd_av    {}
 set cmd_av_f  {}
 set cmd_ap_f  {}
 set cmd_ap_f2 {}
 
 if {[regexp polygone $n_clip]} {
   $n_clip Contenu_dans_fils_ssi_contenu_dans_noeud 1
   set name   "${n_clip}_perimetre"
   set cmd_av    "set $name \[$n_clip Perimetre_a_afficher\]; $n_clip Perimetre_a_afficher 0"
   set cmd_ap_f  "$n_clip Perimetre_a_afficher \$$name; Fct_stencil [GL_lequal] \[Tempon\]; $n_clip Afficher_perimetre; $n_clip Perimetre_a_afficher 0"
   set r_ap [B_rappel [Interp_TCL] "$n_clip Perimetre_a_afficher \$$name"]
     $n_clip abonner_a_LR_parcours [$n_clip LR_Ap_aff] [$r_ap Rappel]
  }

 set r_avant [B_rappel $interp_tcl "Activer_stencil [GL_lequal] [GL_incr] \[Tempon\]; Incr_tempon; $cmd_av"]
   set r_av_fils [B_rappel $interp_tcl "Op_stencil [GL_keep] [GL_keep]; Fct_stencil [GL_lequal] \[Tempon\]; $cmd_av_f"]
   set cmd "Decr_tempon; $cmd_ap_f ; Desactiver_rendu_du_buffer_couleur; Op_stencil [GL_keep] [GL_decr]; $n_clip Dessin; Activer_rendu_du_buffer_couleur; Depiler_stencil; $cmd_ap_f2"
   set r_ap_fils [B_rappel $interp_tcl "if \{\[catch \{$cmd\} e\]\} \{puts \$e\}"]

 set r_av_pre_rendu [B_rappel [Interp_TCL] "set Boite_noeud_pour_texture \[$n_clip Boite_noeud_pour_texture\]; $n_clip Boite_noeud_pour_texture 1"]
 set r_ap_pre_rendu [B_rappel [Interp_TCL] "$n_clip Boite_noeud_pour_texture \$Boite_noeud_pour_texture"]

 $n_clip abonner_a_LR_parcours [$n_clip LR_Av_aff]       [$r_avant Rappel]
 $n_clip abonner_a_LR_parcours [$n_clip LR_Av_aff_fils]  [$r_av_fils Rappel]
 $n_clip abonner_a_LR_parcours [$n_clip LR_Ap_aff_fils]  [$r_ap_fils Rappel]
 $n_clip abonner_a_LR_parcours [$n_clip LR_Av_pre_rendu] [$r_av_pre_rendu Rappel]
 $n_clip abonner_a_LR_parcours [$n_clip LR_Ap_pre_rendu] [$r_ap_pre_rendu Rappel]

 $n_clip Ajouter_MetaData_T Clipping [list $r_avant $r_av_fils $r_ap_fils $r_av_pre_rendu $r_ap_pre_rendu] 
}


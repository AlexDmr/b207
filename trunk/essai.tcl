#source {C:\Alexandre\MAGISTERE2\Plasticité\Jeu de modeles physiques\essai_borland_son_sdl_opengl\essai.tcl}
# set chemin_gml {C:\Alexandre\MAGISTERE2\Plasticité\Jeu de modeles physiques\essai_borland_son_sdl_opengl\gml_Object.tcl}
source gml_Object.tcl
# set chemin_essai {C:\Alexandre\MAGISTERE2\Plasticité\Jeu de modeles physiques\essai_borland_son_sdl_opengl\essai.tcl}
# set chemin_propositions {C:\Alexandre\MAGISTERE2\Plasticité\Jeu de modeles physiques\essai_borland_son_sdl_opengl\propositions.tcl}
#Définition de la liste des propositions


# Définition d'un agent
method c_0 constructor {} {
 set this(nb_props) 0
 set this(L_etats)       [list]
 set this(liste_a_faire) [list]
 set this(liste_decons)  [list]
 set this(liste_cons)    [list] 
}

method c_0 Proposer {} {
 set rep {}
 foreach etat $this(L_etats) {
   set n_etat [this $etat]
   if {$n_etat != {terminer}} {lappend rep $n_etat;}
   }
 set this(L_etats) $rep
 return $rep }

method c_0 Lister_automates {} {
 set rep ""; set i 0
 foreach etat $this(L_etats) {set rep "$rep$i : $etat\n"; incr i}
 return $rep
 }
method c_0 Ajouter_automate {etat_initial} {lappend this(L_etats) $etat_initial; return [expr {[llength $this(L_etats)]-1}] }
method c_0 Retirer_automate {indice} {set this(L_etats) [lreplace $this(L_etats) $indice $indice] }
method c_0 Lister_conseils {} {
 set rep ""
 foreach a $this(liste_cons) {set rep "$rep$a\n"}
 return $rep;
}

method c_0 Lister_deconseils {} {
 set rep ""
 foreach a $this(liste_decons) {set rep "$rep$a\n"}
 return $rep;
}

method c_0 Lister_prop {} {
  set rep "Les conseils à analyser sont :\n";
  foreach a $this(liste_cons) {set rep "$rep$a\n"}
  set rep "${rep}Les déconseils à analyser sont :\n";
  foreach a $this(liste_decons) {set rep "$rep$a\n"}
  return $rep;
  }
method c_0 arbitrage {cons decons} {

 }
method c_0 Analyser {} {
 set nl_cour {}
 set rep "Résultat de l'analyse\n";
 foreach decons $this(liste_decons) {
   set res 1;
   set prop ""
  # On parcourt les fonctions en cours d'exécutions
   foreach p $this(liste_a_faire) {
      if {[$decons get_Conviction] < [$p get_Conviction]} {
        set rep "${rep}   on retient $p pour rester dans les exécution en cours\n"; lappend nl_cour $p;
        continue}
      set res [eval {string first [$decons get_Fonction] [$p get_Fonction]}]
        if {$res==0} {
           set rep "${rep}   on élimine $p des exécution en cours (conflit de fonctions)\n"; $p dispose; continue}

      set rep "${rep}   on retient $p pour rester dans les exécution en cours\n"; lappend nl_cour $p;
      }
  # on parcourt le conseils
   foreach p $this(liste_cons) {
      if {[$decons get_Conviction] < [$p get_Conviction]} {
        set rep "${rep}   on retient $p pour passer dans les exécution en cours\n"; lappend nl_cour $p;
        continue}
      set res [eval {string first [$decons get_Fonction] [$p get_Fonction]}]
        if {$res==0} {
           set rep "${rep}   on élimine $p des propositions en cours (conflit de fonctions)\n"; $p dispose; continue}
      set rep "${rep}   on retient $p pour passer dans les exécution en cours\n"; lappend nl_cour $p;
      }
   }
  if {[llength $this(liste_decons)] == 0} {
     set rep {On retient toutes les propositions!};
     set this(liste_a_faire) [concat $this(liste_a_faire) $this(liste_cons)] } else {
       set this(liste_a_faire) [set $this(liste_a_faire) $nl_cour]}

 #On efface les propositions des listes de conseils et déconseils
  set this(liste_cons)   {}
  foreach p $this(liste_decons) {$p dispose}
  set this(liste_decons) {}
  return $rep;
 }

method c_0 Effectuer {} {
  set rep [llength $this(liste_a_faire)]
  if {$rep == 0} {return schnouf}
  set nl {}
  foreach p $this(liste_a_faire) {
    set rep "${rep}* $p\n";
    set nf [$p get_Fonction]
    set nf [eval this $nf]
    $p set_Fonction $nf
    if {$nf != {terminer}} {lappend nl $p; }
    set rep "$rep$nf\n"
    }
  set this(liste_a_faire) $nl
  return $rep;
  }

method c_0 Lister_executions_courantes {} {
 set rep ""
 foreach p $this(liste_a_faire) {
  set rep "$rep$p avec pour fonction [$p get_Fonction]\n"
  }
 return $rep;
 }
#method c_0 Etat_0 {} {lappend this(liste_cons) {prop2}; return Etat_1}
#method c_0 Etat_1 {} {lappend this(liste_decons) {prop1}; return Etat_0}
#c_0 comet_0


# Les propositions en TCL
method alx_proposition constructor {} {
 set this(conviction) 0
 set this(auteur) utilisateur
 set this(nature) convenance_utilisateur
 set this(fonction) {pas de fonction définit}
 set this(lg_fr) {pas de description fournit en français}
 }
method alx_proposition set_Conviction {c} {set this(conviction) $c}
method alx_proposition get_Conviction {}  {return $this(conviction)}
method alx_proposition set_Auteur {a} {set this(auteur) $a}
method alx_proposition get_Auteur {}  {return $this(auteur)}
method alx_proposition set_Nature {n} {set this(nature) $a}
method alx_proposition get_Nature {}  {return $this(nature)}
method alx_proposition set_Fonction {f} {set this(fonction) $f}
method alx_proposition get_Fonction {}  {return "$this(fonction)"}
method alx_proposition set_Lg_fr {l} {set this(lg_fr) $l}
method alx_proposition get_Lg_fr {}  {return $this(lg_fr)}
method alx_proposition Effectuer {} {return [$this(fonction)]}

# Fonctions d'action
proc renvoyer_message {m} {return $m}

# Procédure d'effaçage
proc Tout_effacer {} {
gmlObject delete class c_0
gmlObject delete class alx_proposition
}


package require http

#_______________________________________________________________________________________________________________________
#_______________________________________________________________________________________________________________________
#_______________________________________________________________________________________________________________________
method Lecteur_FMOD constructor {n_i_mere} {
 set Liste_radios [list [list EuroDance   {http://www.di.fm/mp3/eurodance.pls}   {}] \
                        [list EuroTrance  {http://www.di.fm/mp3/trance.pls}      {}] \
                        [list VocalTrance {http://www.di.fm/mp3/vocaltrance.pls} {}] \
                        [list HardTrance  {http://www.di.fm/mp3/hardtrance.pls}  {}] \
                        [list Wazee       {http://www.wazee.org/128.pls}         {}] \
                  ]
 set this(L_radios) $Liste_radios
 set this(n_i_mere) $n_i_mere
}

#_______________________________________________________________________________________________________________________
method Lecteur_FMOD Lister_radios {} {
 set rep ""
 set index 0
 foreach radio $this(L_radios) {
   set nom     [lindex $radio 0]
   set adresse [lindex $radio 1]
     append rep "$index | $nom => $adresse"
       if {[string equal [lindex $radio 2] {}]} {} else {append rep "  EN COURS"}
     append rep "\n"
   incr index
  }

 return $rep
}

#_______________________________________________________________________________________________________________________
method Lecteur_FMOD Commencer_lecture {index canal h} {
   set L [split [::http::data $h] "\n"]
   foreach e $L {
     if {[regexp -nocase {^File([0-9]*) *= *(.*).$} $e reco num c]} {
       set radio [lindex $this(L_radios) $index]
       set canal [eval $this(n_i_mere) Jouer_musique $c $canal]
       set new_radio      [lreplace $radio 2 2 $canal]
       set this(L_radios) [lreplace $this(L_radios) $index $index $new_radio]
       puts [this Lister_radios]
       return
      }
    }
 puts PB
}

#_______________________________________________________________________________________________________________________
method Lecteur_FMOD Jouer_index {index canal} {
 set radio [lindex $this(L_radios) $index]
   ::http::geturl [lindex $radio 1] -command "$objName Commencer_lecture $index $canal"
   #"$objName Commencer_lecture $index "
}

#_______________________________________________________________________________________________________________________
method Lecteur_FMOD Finir_index {index} {
 set rep ""
 set radio [lindex $this(L_radios) $index]
 set canal [lindex $radio 2]
 if {[string equal $canal {}]} {append rep "Cette radio n'est pas jouée!!!\n"} else {
   $this(n_i_mere) Finir_musique $canal
   set new_radio      [lreplace $radio 2 2 {}]
   set this(L_radios) [lreplace $this(L_radios) $index $index $new_radio]
  }
 append rep [this Lister_radios]
 return $rep
}

#_______________________________________________________________________________________________________________________
method Lecteur_FMOD Aide {} {
 return "Pour obtenir la liste des radios :\n  $objName Lister_radios\nPour lire une radio :\n  $objName Jouer_index index canal.\nFinir une radio :\n  $objName Finir_index index.\nCette page d'aide :\n  $objName Aide"
}

if {[info exists Radio]} {} else {Lecteur_FMOD Radio $n_i_mere}

# Et bientôt des comets ici même !!!!
return "La radio est \"Radio\"\n  Pour avoir de l'aide:\n  Radio Aide"


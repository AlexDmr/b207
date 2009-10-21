#____________________________________________________________________________
proc make_a_B_bouton_perso_from {n cmd_enter cmd_exit cmd_press cmd_release cmd_activate} {
 set name ${n}_B_bouton_perso
 if {[$n Val_MetaData B_bouton_perso] == ""} {
   B_bouton_perso $name $n "" "" "" ""
  }
 $name on_enter    $cmd_enter
 $name on_exit     $cmd_exit
 $name on_press    $cmd_press
 $name on_release  $cmd_release
 $name on_activate B_bouton_perso $cmd_activate
 return $name
}

#____________________________________________________________________________
#____________________________________________________________________________
#____________________________________________________________________________
method B_bouton_perso constructor {noeud cmd_enter cmd_exit cmd_press cmd_unpress {cmd_activate ""}} {
 $noeud Ajouter_MetaData_T B_bouton_perso $objName
 set this(L_ptr)       ""

 set this(cmd_enter)    $cmd_enter
 set this(cmd_exit)     $cmd_exit
 set this(cmd_press)    $cmd_press
 set this(cmd_unpress)  $cmd_unpress
 if {$cmd_activate != ""} {this Subscribe_to_activate $objName $cmd_activate}
 
 set this(rap_enter)    [B_rappel [Interp_TCL]]; $this(rap_enter)    Texte "$objName Ptr_enter       $noeud $this(rap_enter)"
   $noeud abonner_a_detection_pointeur [$this(rap_enter) Rappel] [ALX_pointeur_apparition]

 set this(rap_exit)     [B_rappel [Interp_TCL]]; $this(rap_exit)     Texte "$objName Ptr_exit        $noeud $this(rap_exit)"
   $noeud abonner_a_detection_pointeur [$this(rap_exit)  Rappel] [ALX_pointeur_disparition]

 set this(rap_press)    [B_rappel [Interp_TCL]]; $this(rap_press)    Texte "$objName Ptr_enfonce_on  $noeud $this(rap_press)"
   $noeud abonner_a_detection_pointeur [$this(rap_press)  Rappel] [ALX_pointeur_enfonce]
 
 set this(rap_ptr)      [B_rappel [Interp_TCL]]; $this(rap_ptr)      Texte "$objName Ptr_est_modifie $noeud $this(rap_ptr)"
   $noeud Ajouter_MetaData_T B_bouton_perso__rap_ptr $this(rap_ptr)
}

#____________________________________________________________________________
method B_bouton_perso activate {} {}
#____________________________________________________________________________
method B_bouton_perso Press   {} {}
#____________________________________________________________________________
method B_bouton_perso UnPress {} {}

#____________________________________________________________________________
method B_bouton_perso on_enter   {cmd} {set this(cmd_enter)   $cmd}
#____________________________________________________________________________
method B_bouton_perso on_exit    {cmd} {set this(cmd_exit)    $cmd}
#____________________________________________________________________________
method B_bouton_perso on_press   {cmd} {set this(cmd_press)   $cmd}
#____________________________________________________________________________
method B_bouton_perso on_release {cmd} {set this(cmd_unpress) $cmd}
#____________________________________________________________________________
method B_bouton_perso on_activate {id args} {
 eval "$objName Subscribe_to_activate $id $args"
}

#____________________________________________________________________________
Generate_accessors B_bouton_perso [list L_ptr cmd_enter cmd_exit cmd_press cmd_unpress]

#____________________________________________________________________________
Manage_CallbackList B_bouton_perso [list activate Press UnPress] begin

#____________________________________________________________________________
method B_bouton_perso Ptr_enter {noeud rap} {
 #puts "$objName Ptr_enter $noeud"
 set infos [Void_vers_info [$rap Param]]
 set ptr   [$infos Ptr]
 
   if {$this(L_ptr) == ""} {
     if {[catch "eval $this(cmd_enter)" err]} {puts "ERROR in Ptr_enter :\n$err"}
	}
   if {[$noeud Val_MetaData B_bouton_perso__Ptr] == $ptr} {
     if {[catch "eval $this(cmd_press)" err]} {puts "ERROR in Ptr_enter :\n$err"}
	 this Press
    }
   Add_element this(L_ptr) $ptr
}

#____________________________________________________________________________
method B_bouton_perso Ptr_exit {noeud rap} {
 #puts "$objName Ptr_exit $noeud"
 set infos [Void_vers_info [$rap Param]]
 set ptr   [$infos Ptr]
 
   Sub_element this(L_ptr) $ptr
   if {$this(L_ptr) == ""} {
     if {[catch "eval $this(cmd_exit)" err]} {puts "ERROR in Ptr_exit:\n$err"}
	}
   if {[$noeud Val_MetaData B_bouton_perso__Ptr] == $ptr} {
     if {[catch "eval $this(cmd_unpress)" err]} {puts "ERROR in Ptr_exit:\n$err"}
	 this UnPress
    }
}

#____________________________________________________________________________
method B_bouton_perso Ptr_enfonce_on {noeud rap} {
 #puts "$objName Ptr_enfonce_on $noeud"
 set infos       [Void_vers_info [$rap Param]]
 set ptr         [$infos Ptr]
 
   if {[$ptr Val_MetaData Bouton_pressed] == ""} {
     if {[catch "eval $this(cmd_press)" err]} {puts "ERROR in Ptr_enfonce_on :\n$err"}
	 this Press
     # On vient d'appuyer avec ce pointeur...on s'abonne   
     $noeud Ajouter_MetaData_T B_bouton_perso__Ptr $ptr
     $ptr abonner_a_changement [$this(rap_ptr) Rappel]
	 $ptr Add_MetaData Bouton_pressed $noeud
    }
}

#____________________________________________________________________________
method B_bouton_perso Ptr_est_modifie {noeud rap} {
 set ptr       [Void_vers_alx_pointeur [$rap Param]]
 set ptr_noeud [$ptr Noeud_sous_pointeur]
 
 if {[$ptr Appuie] == 0} {
   if {[catch "eval $this(cmd_unpress) " err]} {puts "ERROR in Ptr_est_modifie - unpress:\n$err"}
   this UnPress
   if {[lsearch $this(L_ptr) $ptr] != -1} {
     if {[catch "eval $objName activate  " err]} {puts "ERROR in Ptr_est_modifie - activate:\n$err"}
    } else {if {[catch "eval $this(cmd_exit)" err]} {puts "ERROR in Ptr_est_modifie - exit:\n$err"}
	       }
   
   $noeud Retirer_MetaData_T B_bouton_perso__Ptr
   $ptr   desabonner_de_changement [$this(rap_ptr) Rappel]
   $ptr   Sub_MetaData Bouton_pressed
  }
}

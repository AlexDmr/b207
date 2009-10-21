
#______________________________________________________________________________
#______________________________________________________________________________
#______________________________________________________________________________
method MuTable_MetaUI_B207 constructor {} {
 set this(root) [B_noeud];
 set this(poly_clic_on) [B_polygone]; 
   B_configure $this(poly_clic_on) -Ajouter_contour [ProcRect 0 0 200 200] -Couleur 0.4 0.5 0.6 1
   $this(root) Ajouter_fils $this(poly_clic_on)
 set this(poly_txt) [B_polygone]; 
   B_configure $this(poly_txt) -Ajouter_contour [ProcRect 0 0 200 50] -Origine 0 200 -Couleur 0 0.8 0.8 1
   $this(root) Ajouter_fils $this(poly_txt)
 set this(zone_txt) [B_texte 200 50 45 [fonte_Arial] [B_sim_sds]]; 
   B_configure $this(zone_txt) -Origine 0 5 -Couleur_texte 1 1 1 1 -Editable 0 -Noeud_touchable 0
   $this(poly_txt) Ajouter_fils $this(zone_txt)

# Abonment to pointer events   
 set this(current_ptr) ""
 set this(rap_press) [B_rappel [Interp_TCL] "$objName Evt_ptr_press"]
   $this(poly_clic_on) abonner_a_detection_pointeur [$this(rap_press) Rappel] [ALX_pointeur_apparition]
 set this(rap_release) [B_rappel [Interp_TCL] "$objName Evt_ptr_release"]
   $this(poly_clic_on) abonner_a_detection_pointeur [$this(rap_release) Rappel] [ALX_pointeur_disparition]
 set this(rap_maj_display) [B_rappel [Interp_TCL] "$objName Maj_Display_Pointer_infos"]
   $this(poly_txt) abonner_a_LR_parcours [$this(poly_txt) LR_Av_aff_fils] [$this(rap_maj_display) Rappel]
   
 this Contact_property 1
}

#______________________________________________________________________________
method MuTable_MetaUI_B207 Plug_under {n} {$n Ajouter_fils $this(root)}
method MuTable_MetaUI_B207 get_current_ptr { } {return $this(current_ptr)}
method MuTable_MetaUI_B207 set_current_ptr {v} {set this(current_ptr) $v}

#______________________________________________________________________________
method MuTable_MetaUI_B207 Contact_property {property} {
 set ctc [$this(poly_txt) Val B_contact_and_capa]
 if {[string equal $ctc ""]} {
   set ctc ${objName}_ctc
   B_contact $ctc "$this(root) 0" -add "$this(poly_txt) $property"
  }
 
 $this(zone_txt) Noeud_touchable 0
}

#______________________________________________________________________________
method MuTable_MetaUI_B207 Maj_Display_Pointer_infos {} {
#puts "$objName Maj_Display_Pointer_infos"
 if {[string equal $this(current_ptr) ""]} {
   set txt ""
  } else {set I [$this(current_ptr) Val_MetaData I]
          set W [$this(current_ptr) Val_MetaData W]
		  set H [$this(current_ptr) Val_MetaData H]
          set PI 3.14159265
		  set txt "[expr $I/($PI*$W*$H)] $I $W $H"
		 }
#puts "$this(zone_txt) TEXTE \"$txt\""
 $this(zone_txt) TEXTE $txt
 $this(zone_txt) Optimiser_dimensions
}

#______________________________________________________________________________
method MuTable_MetaUI_B207 Evt_ptr_press {} {
 puts "$objName Evt_ptr_press (\$this(current_ptr) = \"$this(current_ptr)\""
 if {![string equal $this(current_ptr) ""]} {return}
 puts go
 set inf [$this(rap_press) Param]
 set inf [Void_vers_info $inf]
 set evt [$inf EVT]
 set ptr [$evt Ptr]
 set this(current_ptr) $ptr
 puts "Current pointer is now $ptr"
}

#______________________________________________________________________________
method MuTable_MetaUI_B207 Evt_ptr_release {} {
 set inf [$this(rap_release) Param]
 set inf [Void_vers_info $inf]
 set evt [$inf EVT]
 set ptr [$evt Ptr]
 if {![string equal $ptr $this(current_ptr)]} {return}
 
 set this(current_ptr) ""
}

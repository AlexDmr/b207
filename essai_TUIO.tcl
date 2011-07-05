#_________________________________________________________________________________________________________
#_________________________________________________________________________________________________________
#_________________________________________________________________________________________________________
method Liant_TUIO_sim_ptr constructor {port} {
	package require udp
	set this(udp_port) [udp_open $port]
	set this(nb_trame_to_read) 0
    fconfigure $this(udp_port) -buffering none -translation binary
    fileevent  $this(udp_port) readable "if {\[catch {$objName new_udp_trame $this(udp_port)} err\]} {puts \"Error in new UDP trame:\n\$err\"}"
	
	set this(L_ptr_preso_nodes) [list]
	set this(D_mappings_id_TUIO_B207) [dict create]
	set this(L_alive_ids) [list]
	set this(current_source) ""
	
	set this(B207_screen_TX) [N_i_mere Largeur] 
	set this(B207_screen_TY) [N_i_mere Hauteur]
	catch {set this(sim_ptr_TCL) [B_sim_ptr_TCL [N_i_mere Ecran_X] [N_i_mere Ecran_Y] [N_i_mere Coin_BG] [N_i_mere Vecteur_X] [N_i_mere Vecteur_Y]]}
	if {[catch {set this(id_ptr_TCL)  [$this(sim_ptr_TCL) Ajouter_rapporteur [B_sim_ptr] [N_i_mere Noeud_scene] 0]}]} {
		 set this(B207_on) 0
		} else {$this(sim_ptr_TCL) Noeud_pere [N_i_mere Noeud_scene]
			    set this(B207_on) 1
			    set this(rap_ptr_TCL) [B_rappel [Interp_TCL] "if {\[catch {$objName B207_reading} err\]} {puts \"ERROR B207 reading MuTable:\n\$err\"}"]
			    $this(sim_ptr_TCL) abonner_a_simulation [$this(rap_ptr_TCL) Rappel]
			    set this(B207_evt_ptr)      [new_alx_evennement_pointeur]
			    set this(B207_info_comp)    [new_info_comp_evennement]
			    set this(B207_pt_tmp_coord) [B_point]
			    set this(B207_pt_tmp_dep)   [B_point]
			    set this(L_ptr_preso_nodes) [list]
			   }
}

#_________________________________________________________________________________________________________
method Liant_TUIO_sim_ptr new_udp_trame {sock} {
	incr this(nb_trame_to_read)
	if {$this(nb_trame_to_read) > 1} {return}
	
	while {$this(nb_trame_to_read)} {
		 set pkt [read $sock]
		 binary scan $pkt a8 tag
		 if {[string equal -length 7 $tag "#bundle"]} {
			 if {[catch {this Process_bundle $pkt} err]} {puts "Error while processing bundle:\n\tpkt : $pkt\n\terr : $err"}
			} else {puts "Received a non bundle tag (${tag}) from $pkt"}
		 incr this(nb_trame_to_read) -1
		}
}

#_________________________________________________________________________________________________________
method Liant_TUIO_sim_ptr Process_bundle {pkt} {
	# Time
	binary scan $pkt a8II bundle time_1 time_2
	set pkt [string range $pkt 16 end]
	
	set this(current_source) "Unknown"
	dict set this(new_L_alive_ids) $this(current_source) [list]
	this Process_messages $pkt
	
}

#_________________________________________________________________________________________________________
method Liant_TUIO_sim_ptr Process_messages {pkt} {
	while {[string length $pkt] > 0} {
		 binary scan $pkt I size
		 binary scan $pkt Ia$size size msg
		 set pkt [string range $pkt [expr 4 + $size] end]
		 if {[catch {this Process_[string range $msg 0 10] [string range $msg 12 end]} err]} {puts "Error with message:\n\tmsg : $msg\n\terr : $err"}
		}
}

#_________________________________________________________________________________________________________
method Liant_TUIO_sim_ptr Process_/tuio/2Dcur {msg} {
	binary scan $msg aaaa virgule a1 a2 a3; set msg [string range $msg 4 end]
	set L_args [list $a2 $a3]
	while {[scan $a3 %c] != 0} {
		 binary scan $msg aaaa a0 a1 a2 a3
		 lappend L_args $a0 $a1 $a2 $a3
		 set msg [string range $msg 4 end]
		}
	
	set op [this Decode_s msg]
	# puts "\t op : $op"
	switch $op {
		  source {set s [this Decode_s msg]
				  # puts "\tmessage from source $s"
				  set this(current_source) $s
				  dict set this(new_L_alive_ids) $this(current_source) [list]
				 }
		   alive {set L_ids [list]
				  binary scan $msg I[expr [string length $msg]/4] L_ids
				  # puts "\tmessage alive $L_ids"
				  set L [list]; foreach id $L_ids {lappend L $this(current_source):$id}
				  dict set this(new_L_alive_ids) $this(current_source) $L
				 }
		     set {binary scan $msg IRRRRR id x y vx vy a
				  this add_or_update_cursor $this(current_source):$id $x $y $vx $vy $a
				  # puts "\t cursor $id at <$x ; $y ; $vx ; $vx ; $a >"
				 }
		    fseq {binary scan $msg I f_id
				  # XXX il faut diférencier les sources!!!
				  if {[dict exists $this(L_alive_ids) $this(current_source)]} {set L [dict get $this(L_alive_ids) $this(current_source)]} else {set L [list]}
				  this remove_cursors [this Minus_list $L [dict get $this(new_L_alive_ids) $this(current_source)]]
				  dict set this(L_alive_ids) $this(current_source) [dict get $this(new_L_alive_ids) $this(current_source)]
				  # puts "\tEnd of frame $f_id"
			     }
		 default {error "Unknown operation for /tuio/2Dcur (operation was $op)"} 
		}	
}
# Trace Liant_TUIO_sim_ptr Process_/tuio/2Dcur

#_________________________________________________________________________________________________________
method Liant_TUIO_sim_ptr Minus_list {L1 L2} {
	set L [list]
	foreach e $L1 {
		 if {[lsearch -exact $L2 $e] == -1} {lappend L $e}
		}
	return $L
}

#_________________________________________________________________________________________________________
method Liant_TUIO_sim_ptr Process_/tuio/2Dobj {msg} {

}
# Trace Liant_TUIO_sim_ptr Process_/tuio/2Dobj

#_________________________________________________________________________________________________________
method Liant_TUIO_sim_ptr Process_/tuio/2Dblb {msg} {

}
# Trace Liant_TUIO_sim_ptr Process_/tuio/2Dblb

#_________________________________________________________________________________________________________
#_________________________________________________________________________________________________________
#_________________________________________________________________________________________________________
method Liant_TUIO_sim_ptr Decode_s {msg_name} {
	upvar $msg_name msg
	set rep ""; set i 0; set char [string index $msg 0]
	while {[scan $char %c] != 0} {append rep $char; incr i; set char [string index $msg $i]}
	set msg [string range $msg [expr (int([string length $rep]/4)+1)*4] end]
	return $rep
}

#_________________________________________________________________________________________________________
#_________________________________________________________________________________________________________
#_________________________________________________________________________________________________________
method Liant_TUIO_sim_ptr add_or_update_cursor {id x y vx vy a} {
	if {![dict exists $this(D_mappings_id_TUIO_B207) $id]} {
		 # Generate a new B207 pointer and add a presentation
		 set B207_id [B_sim_ptr Ajouter_pointeur "TUIO pointer" [N_i_mere Noeud_scene] 1]
			set B207_ptr [B_sim_ptr Pointeur_id $B207_id]
			$B207_ptr Noeud_representation [this get_new_presentation_node]
			$this(sim_ptr_TCL) convertir_coordonnees [expr $this(B207_screen_TX) * $x] [expr $this(B207_screen_TY) * $y] $this(B207_pt_tmp_coord)
			$B207_ptr Coord [$this(B207_pt_tmp_coord) X] [$this(B207_pt_tmp_coord) Y]
			[$B207_ptr Noeud_representation] Origine $this(B207_pt_tmp_coord)
		 # Link it to the TUIO id
		 dict set this(D_mappings_id_TUIO_B207) $id $B207_id
		 # Generate a press event
		 this Generate_clic_event_for $B207_id $x $y [ALX_pointeur_enfonce]
		} else {set B207_id [dict get $this(D_mappings_id_TUIO_B207) $id]
			    this Generate_mvt_event_for $B207_id $x $y $vx $vy $a
			   }
}

#_________________________________________________________________________________________________________
method Liant_TUIO_sim_ptr remove_cursors {L_ids} {
	foreach id $L_ids {
		 if {![dict exists $this(D_mappings_id_TUIO_B207) $id]} {puts "Error trying to remove TUIO id $id ... not present in the dictionnary"; continue}
		 set B207_id  [dict get $this(D_mappings_id_TUIO_B207) $id]
		 dict unset this(D_mappings_id_TUIO_B207) $id
		 set B207_ptr [B_sim_ptr Pointeur_id $B207_id]
		 this Generate_clic_event_for $B207_id [$B207_ptr X] [$B207_ptr Y] [ALX_pointeur_relache]
		 this Release_presentation_node [$B207_ptr Noeud_representation]
		 [$B207_ptr Noeud_representation] Vider_peres
		 if {[B_sim_ptr Retirer_pointeur $B207_id] == -1} {
			 puts "__!!!__Problème pour enlever le pointeur $id"
			}
		}
}


#_____________________________________________________________________________________________________
method Liant_TUIO_sim_ptr Generate_clic_event_for {B207_id x y type_evt} {
   $this(sim_ptr_TCL) convertir_coordonnees [expr $this(B207_screen_TX) * $x]  [expr $this(B207_screen_TY) * $y]  $this(B207_pt_tmp_coord)
   $this(B207_info_comp) Coord $this(B207_pt_tmp_coord)
   $this(B207_info_comp) Type_appuie 1
   $this(B207_evt_ptr) maj $B207_id $type_evt $this(B207_info_comp) [$this(sim_ptr_TCL) Noeud_pere]
  # Distribute event
   B_sim_ptr Ajouter_evt $this(B207_evt_ptr)
}

#_____________________________________________________________________________________________________
method Liant_TUIO_sim_ptr Generate_mvt_event_for {B207_id x y dx dy a} {
   $this(sim_ptr_TCL) convertir_coordonnees [expr $this(B207_screen_TX) * $x ] [expr $this(B207_screen_TY) *  $y] $this(B207_pt_tmp_coord)
   $this(sim_ptr_TCL) convertir_coordonnees [expr $this(B207_screen_TX) * $dx] [expr $this(B207_screen_TY) * $dy] $this(B207_pt_tmp_dep)
   $this(B207_info_comp) Dep   $this(B207_pt_tmp_dep)
   $this(B207_info_comp) Coord $this(B207_pt_tmp_coord)
   $this(B207_evt_ptr) maj $B207_id [ALX_pointeur_mouvement] $this(B207_info_comp) [$this(sim_ptr_TCL) Noeud_pere]
   B_sim_ptr Ajouter_evt $this(B207_evt_ptr)
}

#_____________________________________________________________________________________________________
method Liant_TUIO_sim_ptr get_new_presentation_node {} {
 if {[llength $this(L_ptr_preso_nodes)] > 0} {
   set rep [lindex $this(L_ptr_preso_nodes) 0]
   set this(L_ptr_preso_nodes) [lrange $this(L_ptr_preso_nodes) 1 end]
  } else {set rep [B_polygone]
		  $rep Ajouter_contour [ProcOvale 0 0 40 40 60]
          $rep Retirer_contour [ProcOvale 0 0  5  5 60]
		 } 
 $rep Noeud_touchable 0

 B_configure $rep -Vider_MetaData -Vider_fils 
 $rep Couleur 1 1 1 0.7
 $rep Afficher_noeud 1; $rep Gerer_contacts 0

   set transfo [B_transfo 1000]
   set rap_display_pdt [B_rappel [Interp_TCL] "set v \[expr 1-0.5*\[$transfo V_courant\]\]; $rep Couleur_perimetre 3 \$v; $rep Couleur 3 \$v"]
   set rap_display_fin [B_rappel [Interp_TCL] "$rep Afficher_noeud 1; $rep Gerer_contacts 0"]
     $transfo abonner_a_rappel_pendant [$rap_display_pdt Rappel]
	 $transfo abonner_a_rappel_fin     [$rap_display_fin Rappel]
   $transfo Demarrer
   N_i_mere Ajouter_deformation_dynamique $transfo
   
 return $rep
}

#_____________________________________________________________________________________________________
method Liant_TUIO_sim_ptr Release_presentation_node {n} {
 if {[lsearch $this(L_ptr_preso_nodes) $n] != -1} {puts "Liant_Mutable_sim_ptr::Release_presentation_node $n\n  ALLREADY released..."; return}
 lappend this(L_ptr_preso_nodes) [Real_class $n]
}



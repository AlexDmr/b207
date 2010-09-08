source "$::env(ROOT_COMETS)\\Comets\\gml_Object.tcl"

#load MuTable_Pointers.dll
if {[catch "package require Tktable" err]} {
  puts "Error on loading Tktable :\n$err"
 }
package require udp

# port usuel : 8910
# Multitouch_Listener Liant_mutable 8910
proc Nop {args} {}

proc BBox {s L_name} {
 upvar $L_name L
 set bg_x  1000; set bg_y  1000
 set hd_x -1000; set hd_y -1000
 
 foreach pt $L {
   #$id $int [expr $x * $this(x_stretch)] [expr $y * $this(y_stretch)] $w $h
   set x [lindex $pt 2]; set y [lindex $pt 3]
   set w [expr [lindex $pt 4]/2.0]; set h [expr [lindex $pt 5]/2.0]
   
   if {$bg_x > $x-$w} {set bg_x [expr $x-$w]}
   if {$bg_y > $y-$h} {set bg_y [expr $y-$h]}
   if {$hd_x < $x+$w} {set hd_x [expr $x+$w]}
   if {$hd_y < $y+$h} {set hd_y [expr $y+$h]}
  }
 
 puts "$x $y $w $h"
 if {[catch "puts -nonewline $s {bbox [expr int($bg_x)] [expr int($hd_x)] [expr int($bg_y)] [expr int($hd_y)] }; flush $s" err]} {
   close $s
  }
 set L ""
}

#_____________________________________________________________________________________________________
#_____________________________________________________________________________________________________
#_____________________________________________________________________________________________________
method Multitouch_Listener constructor {} {
 set this(L_MuTable_evt) {}
 set this(L_MuTable_ids) {}
		  
 catch {set this(sim_ptr_TCL) [B_sim_ptr_TCL [N_i_mere Ecran_X] [N_i_mere Ecran_Y] [N_i_mere Coin_BG] [N_i_mere Vecteur_X] [N_i_mere Vecteur_Y]]}
 if {[catch {set this(id_ptr_TCL)  [$this(sim_ptr_TCL) Ajouter_rapporteur [B_sim_ptr] [N_i_mere Noeud_scene] 0]}]} {
   set this(B207_on) 0
  } else {$this(sim_ptr_TCL) Noeud_pere [N_i_mere Noeud_scene]
          set this(B207_on) 1
		  set this(rap_ptr_TCL) [B_rappel [Interp_TCL] "if {\[catch {$objName B207_reading} err\]} {puts \"ERROR B207 reading MuTable:\n\$err\"}"]
		  $this(sim_ptr_TCL) abonner_a_simulation [$this(rap_ptr_TCL) Rappel]
		  set this(B207_evt_ptr)   [new_alx_evennement_pointeur]
          set this(B207_info_comp) [new_info_comp_evennement]
		  set this(B207_pt_tmp_coord) [B_point]
		  set this(B207_pt_tmp_dep)   [B_point]
		  set this(L_ptr_preso_nodes) {}
		 }

 
# CallBack on new frame
 set this(CB_new_frame) Nop
 
# Infos
 set this(nb_frame_mutable) 0
 
   
# Variables for multi threading with B207
 set this(B207_is_reading)    0
 set this(MuTable_is_writing) 0

# Coordinate stretching
 set this(y_stretch) 1024
 set this(x_stretch) 768
# Some variables for configuring...
 set this(threshold_mov) 3
}

#_____________________________________________________________________________________________________
method Multitouch_Listener get_CB_new_frame {   } {return $this(CB_new_frame)}
method Multitouch_Listener set_CB_new_frame {txt} {set this(CB_new_frame) $txt}

#_____________________________________________________________________________________________________
method Multitouch_Listener get_stretch {   } {return "$this(x_stretch) $this(y_stretch)"}
method Multitouch_Listener set_stretch {x y} {set this(x_stretch) $x; set this(y_stretch) $y}

#_____________________________________________________________________________________________________
method Multitouch_Listener get_sim_ptr_TCL {} {return $this(sim_ptr_TCL)}
method Multitouch_Listener get_threshold_mov { } {return $this(threshold_mov)}
method Multitouch_Listener set_threshold_mov {v} {set this(threshold_mov) $v}
#_____________________________________________________________________________________________________
method Multitouch_Listener get_L_ptr_preso_nodes { } {return $this(L_ptr_preso_nodes)}
method Multitouch_Listener set_L_ptr_preso_nodes {L} {set this(L_ptr_preso_nodes) $L}
#_____________________________________________________________________________________________________
method Multitouch_Listener get_L_MuTable_ids {} {return $this(L_MuTable_ids)}
method Multitouch_Listener get_L_MuTable_ids {} {return $this(method Multitouch_Listener )}

#_____________________________________________________________________________________________________
method Multitouch_Listener B207_reading {} {
 if {$this(B207_is_reading)} {puts "Problem thread in Multitouch_Listener::B207_reading"}
 while {$this(MuTable_is_writing)} {}
 set this(B207_is_reading) 1
 set nb_frames [llength $this(L_MuTable_evt)]
#if {$nb_frames != 1} {puts "$objName Multitouch_Listener::B207_reading has $nb_frames to read..."}
 while {$this(MuTable_is_writing)} {}
   foreach M_frame $this(L_MuTable_evt) {
     if {[catch {this Frame_for_B207 M_frame} err]} {puts "into B207_reading ERROR:\n$err"}
	}
 set this(L_MuTable_evt) {}
 set this(B207_is_reading) 0
}

#_____________________________________________________________________________________________________
method Multitouch_Listener Update_ptr_representation {ptr id blob} {
 set node [$ptr Noeud_representation]
 if {$node == "NULL"} {puts "$objName Multitouch_Listener Update_ptr_representation {$ptr} ${id} {$blob}\n  le noeud B207 associé au pointeur $ptr est NULL..."; return}
 set node [Real_class $node]
 $node Couleur 1 1 1 0.7; 
}

#_____________________________________________________________________________________________________
method Multitouch_Listener get_new_presentation_node {ptr blob} {
 if {[llength $this(L_ptr_preso_nodes)] > 0} {
   set rep [lindex $this(L_ptr_preso_nodes) 0]
   set this(L_ptr_preso_nodes) [lrange $this(L_ptr_preso_nodes) 1 end]
  } else {set rep [B_polygone]
		  $rep Ajouter_contour [ProcOvale 0 0 40 40 60]
          $rep Retirer_contour [ProcOvale 0 0  5  5 60]
		 } 
# $rep Noeud_touchable 0
 set rap [$rep Val_MetaData rap_double_touch]
 if {[string equal $rap {}]} {
   set rap [B_rappel [Interp_TCL]]
   $rep abonner_a_detection_pointeur [$rap Rappel] [ALX_pointeur_enfonce]
  }
 $rap Texte "puts {Double clic with $ptr}; eval \[$ptr Val_MetaData TODO_when_double_touch\]"
  
   B_configure $rep -Vider_MetaData -Vider_fils 
 $rep Couleur 1 1 1 0.7
 $rep Afficher_noeud 1; $rep Gerer_contacts 0
 $rep Ajouter_MetaData_T rap_double_touch $rap 
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
method Multitouch_Listener Release_presentation_node {n} {
 if {[lsearch $this(L_ptr_preso_nodes) $n] != -1} {puts "Multitouch_Listener::Release_presentation_node $n\n  ALLREADY released..."; return}
 lappend this(L_ptr_preso_nodes) [Real_class $n]
}

#_____________________________________________________________________________________________________
method Multitouch_Listener Frame_for_B207 {f_name} {
 #return
 #puts "Frame_for_B207"
 # set state(0) "BIRTH"
 # set state(1) "DEATH"
 # set state(2) "UPDATE"
 upvar $f_name f

 
 foreach evt $f {
	lassign $evt id intensity x y w h a state 
	
	switch $state {
		0 {
		    set B207_id [B_sim_ptr Ajouter_pointeur "MuTable pointer" [N_i_mere Noeud_scene] 1]
		    #DEBUG set this($B207_id,blob) [list $blob]
		    set this(ptr_id,$id) $B207_id
		    #puts "New pointer : id $B207_id with\n  blob : $blob\n  this(ptr_id,[lindex $blob $this(i_id)]) = $this(ptr_id,[lindex $blob $this(i_id)])"
		  
		    set ptr [B_sim_ptr Pointeur_id $B207_id]
		    if {$ptr == "NULL"} {puts "Pointeur créé mais NULL..."; return}
		    set this(ptr,$B207_id,x) $x
		    set this(ptr,$B207_id,y) $y
			$ptr Coord $x $y
		    $ptr Add_MetaData I $intensity
		    $ptr Add_MetaData W $w
		    $ptr Add_MetaData H $h
		    $ptr Add_MetaData A $a
		    $ptr Noeud_representation [this get_new_presentation_node $ptr $evt]
		    $this(sim_ptr_TCL) convertir_coordonnees $x $y $this(B207_pt_tmp_coord)
			$ptr Coord [$this(B207_pt_tmp_coord) X] [$this(B207_pt_tmp_coord) Y]
		    [$ptr Noeud_representation] Origine $this(B207_pt_tmp_coord)
			$this(B207_info_comp) Coord $this(B207_pt_tmp_coord)
			$this(B207_info_comp) Type_appuie 1
			$this(B207_evt_ptr) maj $B207_id [ALX_pointeur_enfonce] $this(B207_info_comp) [$this(sim_ptr_TCL) Noeud_pere]
			# Distribute event
			B_sim_ptr Ajouter_evt $this(B207_evt_ptr) 
		}
		1 {
			set B207_id $this(ptr_id,$id)
		    set ptr [B_sim_ptr Pointeur_id $B207_id]			
			$this(sim_ptr_TCL) convertir_coordonnees $x $y $this(B207_pt_tmp_coord)
			$this(B207_info_comp) Coord $this(B207_pt_tmp_coord)
			$this(B207_info_comp) Type_appuie 1
			$this(B207_evt_ptr) maj $B207_id [ALX_pointeur_relache] $this(B207_info_comp) [$this(sim_ptr_TCL) Noeud_pere]
			# Distribute event
			B_sim_ptr Ajouter_evt $this(B207_evt_ptr) 
		    this Release_presentation_node [$ptr Noeud_representation]
		    if {[B_sim_ptr Retirer_pointeur $B207_id] == -1} {puts "__!!!__Problème pour enlever le pointeur $B207_id"}
		    [$ptr Noeud_representation] Vider_peres
		}
		2   {
			set B207_id $this(ptr_id,$id)
			set ptr [B_sim_ptr Pointeur_id $B207_id]	
			$ptr Add_MetaData I $intensity
			$ptr Add_MetaData W $w
			$ptr Add_MetaData H $h
			$ptr Add_MetaData A $a
			this Update_ptr_representation $ptr $B207_id $evt   		   
			set dx [expr $this(ptr,$B207_id,x) - $x]; set dy [expr $this(ptr,$B207_id,y) - $y]
			if {($dx*$dx + $dy*$dy) >= $this(threshold_mov)} {
				set this(ptr,$B207_id,x) $x; set this(ptr,$B207_id,y) $y
				$this(sim_ptr_TCL) convertir_coordonnees $x  $y  $this(B207_pt_tmp_coord)
				$this(sim_ptr_TCL) convertir_coordonnees $dx $dy $this(B207_pt_tmp_dep)
				$this(B207_info_comp) Dep   $this(B207_pt_tmp_dep)
				$this(B207_info_comp) Coord $this(B207_pt_tmp_coord)
				$this(B207_evt_ptr) maj $B207_id [ALX_pointeur_mouvement] $this(B207_info_comp) [$this(sim_ptr_TCL) Noeud_pere]
				# Distribute event
				B_sim_ptr Ajouter_evt $this(B207_evt_ptr)
			}
		}
		default {puts "$objName Multitouch_Listener Frame_for_B207 incorrect state : $state" }
	}
	
 }
 
}

#_____________________________________________________________________________________________________
method Multitouch_Listener Chrono_frame_mutable_start {} {
 set this(nb_frame_mutable) 0
 set this(tps_chrono) [clock clicks -milliseconds]
}

#_____________________________________________________________________________________________________
method Multitouch_Listener Chrono_frame_mutable_frame_rate {} {
# puts "$objName Chrono_frame_mutable_frame_rate"
 set t [clock clicks -milliseconds]
 set dt [expr $t - $this(tps_chrono)]
   puts "Chrono :\n  - frames : $this(nb_frame_mutable)\n  -    tps : $dt\n  -   rate : [expr 1000*$this(nb_frame_mutable)/${dt}.0]"
 set this(nb_frame_mutable) 0
 set this(tps_chrono) [clock clicks -milliseconds]
}
 
#_____________________________________________________________________________________________________
method Multitouch_Listener Save_Data {L_ptr_name} {
 if {$this(B207_is_reading)} {puts "Problem thread in Multitouch_Listener::Save_Data"}
 incr this(nb_frame_mutable)
 upvar $L_ptr_name L_ptr
   if {[catch [list $this(CB_new_frame) L_ptr] err]} {
		puts "Error in $objName Multitouch_Listener Save_Data :\n$err"
   }
 #if {[llength $L_ptr] > 0} {
   while {$this(B207_is_reading)} {}
   set this(MuTable_is_writing) 1
   while {$this(B207_is_reading)} {}
   if {[llength $this(L_MuTable_evt)] > 150} {puts "OVERFLOW!!!"; set this(L_MuTable_evt) {}}
   lappend this(L_MuTable_evt) $L_ptr
   set this(MuTable_is_writing) 0
 # }
}

#_____________________________________________________________________________________________________
method Multitouch_Listener Update_ptr_MuTable {nb L_ptr_name} {
 #puts "$objName Update_ptr_MuTable"
	upvar $L_ptr_name L_ptr
	this Save_Data L_ptr
}

#_____________________________________________________________________________________________________
method Multitouch_Listener startListening {port} {
 if {$this(B207_on)} {N_i_mere abonner_a_simulateur [$this(rap_ptr_TCL) Rappel]}

#$this(MuTable) startListening
 set this(udp_srv) [udp_open $port]
 fconfigure $this(udp_srv) -buffering none -translation binary
 fileevent $this(udp_srv) readable [list $objName udpEventHandler $this(udp_srv)]
   #puts "Listening on udp port: [fconfigure $this(udp_srv) -myport]"
   return $this(udp_srv)
}

#_____________________________________________________________________________________________________
method Multitouch_Listener stopListening  {} {
 close $this(udp_srv)
}

#_____________________________________________________________________________________________________

# struct B207_touchPoint
# {       
        # int _id;    //Integer id which uniquely represent a touch point. *NETWORK ENDIAN *
        # float _x;   //normalized value of x-co-ordinate  *NETWORK ENDIAN *
        # float _y;   //normalized value of y-co-ordinate  *NETWORK ENDIAN *
		# float _w ;
		# float _h ; 
		# float _a ; 
        # char _type; //this is the touch point state
# };

method Multitouch_Listener decode_mutable {nb txt} {
 set txt [string range $txt 4 end]
 set L {}
 #puts "Frame: $nb points"
 
 #juste pour le puts
 set state(0) "BIRTH"
 set state(1) "DEATH"
 set state(2) "UPDATE"
 
 for {set n 0} {$n<$nb} {incr n} {
   binary scan $txt ifffffc id x y w h a type; set txt [string range $txt 24 end]
   #puts "  * Point [set state($type)]  $id $x $y $w $h $a"
   lappend L [list $id 1 [expr $x * $this(x_stretch)] [expr $y * $this(y_stretch)] $w $h $a $type]
  }
  
 this Update_ptr_MuTable $nb L 
}

#_____________________________________________________________________________________________________
method Multitouch_Listener udpEventHandler {sock} {
    set pkt [read $sock]; 
    set peer [fconfigure $sock -peer]
    binary scan $pkt i nb; 
	#puts "$peer: [string length $pkt] {$nb}";
	if {[catch {set L_ptr [this decode_mutable $nb $pkt]} err]} {
	  puts "ERROR : $err"
	 }
}

puts "Multitouch_Listener Liant_mutable; Liant_mutable startListening 8910"

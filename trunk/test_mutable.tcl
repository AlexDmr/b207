source {C:\These\Projet Interface\COMETS\devCOMETS\Comets\gml_Object.tcl}

#load MuTable_Pointers.dll
if {[catch "package require Tktable" err]} {
  puts "Error on loading Tktable :\n$err"
 }
package require udp

# port usuel : 8910
# Liant_Mutable_sim_ptr Liant_mutable 8910
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
method Liant_Mutable_sim_ptr constructor {} {
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
## set this(MuTable) "${objName}_MuTable"
##   CoordinateReceiver_TCL $this(MuTable) $port

## set this(rap_ptr_MuTable) [B_rappel [Interp_TCL] "$objName Update_ptr_MuTable"]
##   $this(MuTable) abonner_a_PointUpdate [$this(rap_ptr_MuTable) Rappel]
# 
#   

 set this(i_id) 0
 set this(i_I)  1
 set this(i_x)  2
 set this(i_y)  3
 set this(i_W)  4
 set this(i_H)  5
 
# CallBack on new frame
 set this(CB_new_frame) Nop
 
# Infos
 set this(nb_frame_mutable) 0
 
#Mapping between IDs
 #In TIdMapping(id_Mutable) -> id_ptr_B207
 
# Control with a table
 set this(tk_table_var) "${objName}_tk_table_var"
 global $this(tk_table_var)
 toplevel ._MuTable
 set this(tk_table) "._MuTable._${objName}_tk_table"
   table $this(tk_table); pack $this(tk_table) -expand 1 -fill both
   $this(tk_table) configure -cols 6 -variable $this(tk_table_var)
   set $this(tk_table_var)(0,0) ID
   set $this(tk_table_var)(0,1) X
   set $this(tk_table_var)(0,2) Y
   set $this(tk_table_var)(0,3) INTENSITY
   set $this(tk_table_var)(0,4) WIDTH
   set $this(tk_table_var)(0,5) HEIGHT
   
# Variables for multi threading with B207
 set this(B207_is_reading)    0
 set this(MuTable_is_writing) 0

# Coordinate stretching
 set this(y_stretch) 1
 set this(x_stretch) 1
# Some variables for configuring...
 set this(threshold_mov) 3
}

#_____________________________________________________________________________________________________
method Liant_Mutable_sim_ptr get_CB_new_frame {   } {return $this(CB_new_frame)}
method Liant_Mutable_sim_ptr set_CB_new_frame {txt} {set this(CB_new_frame) $txt}

#_____________________________________________________________________________________________________
method Liant_Mutable_sim_ptr get_stretch {   } {return "$this(x_stretch) $this(y_stretch)"}
method Liant_Mutable_sim_ptr set_stretch {x y} {set this(x_stretch) $x; set this(y_stretch) $y}

#_____________________________________________________________________________________________________
method Liant_Mutable_sim_ptr get_sim_ptr_TCL {} {return $this(sim_ptr_TCL)}
method Liant_Mutable_sim_ptr get_threshold_mov { } {return $this(threshold_mov)}
method Liant_Mutable_sim_ptr set_threshold_mov {v} {set this(threshold_mov) $v}
#_____________________________________________________________________________________________________
method Liant_Mutable_sim_ptr get_L_ptr_preso_nodes { } {return $this(L_ptr_preso_nodes)}
method Liant_Mutable_sim_ptr set_L_ptr_preso_nodes {L} {set this(L_ptr_preso_nodes) $L}
#_____________________________________________________________________________________________________
method Liant_Mutable_sim_ptr get_L_MuTable_ids {} {return $this(L_MuTable_ids)}
method Liant_Mutable_sim_ptr get_L_MuTable_ids {} {return $this(method Liant_Mutable_sim_ptr )}

#_____________________________________________________________________________________________________
method Liant_Mutable_sim_ptr B207_reading {} {
 if {$this(B207_is_reading)} {puts "Problem thread in Liant_Mutable_sim_ptr::B207_reading"}
 while {$this(MuTable_is_writing)} {}
 set this(B207_is_reading) 1
 set nb_frames [llength $this(L_MuTable_evt)]
#if {$nb_frames != 1} {puts "$objName Liant_Mutable_sim_ptr::B207_reading has $nb_frames to read..."}
 while {$this(MuTable_is_writing)} {}
   foreach M_frame $this(L_MuTable_evt) {
     if {[catch {this Frame_for_B207 M_frame} err]} {puts "into B207_reading ERROR:\n$err"}
	}
 set this(L_MuTable_evt) {}
 set this(B207_is_reading) 0
}

#_____________________________________________________________________________________________________
method Liant_Mutable_sim_ptr Generate_clic_event_for {ptr id blob type_evt} {
 set x  [lindex $blob $this(i_x)]    ; set y [lindex $blob $this(i_y)]
  #$ptr Coord $x $y
 set this(ptr,$id,x) $x; set this(ptr,$id,y) $y
   $this(sim_ptr_TCL) convertir_coordonnees $x  $y  $this(B207_pt_tmp_coord)
   $this(B207_info_comp) Coord $this(B207_pt_tmp_coord)
   $this(B207_info_comp) Type_appuie 1
   $this(B207_evt_ptr) maj $id $type_evt $this(B207_info_comp) [$this(sim_ptr_TCL) Noeud_pere]
  # Distribute event
   B_sim_ptr Ajouter_evt $this(B207_evt_ptr)
  #puts "$id Clic\[$type_evt\] ([$this(B207_pt_tmp_coord) X] ; [$this(B207_pt_tmp_coord) Y])"
}

#_____________________________________________________________________________________________________
method Liant_Mutable_sim_ptr Ptr_evt_time {ptr id blob} {
 $ptr Add_MetaData clic 0
 lappend this($id,blob) $blob
 set this($id,blob) [lrange $this($id,blob) end-59 end]; set nb_blob [llength $this($id,blob)]
 set cur_i [lindex $blob $this(i_I)]; set I 0; set max 0; set pos_max 0; set nb [llength $this($id,blob)]
 if {$nb < 10} {$ptr Add_MetaData max ""; return}
 set pos 0; set L_for_mediane {}
 foreach b $this($id,blob) {
   set i [lindex $b $this(i_I)]; lappend L_for_mediane $i
   set I [expr $I + $i]
   if {$i>$max} {
     set pos_max $pos
	 set max $i
    }
   incr pos	
  }
 set moy [expr $I/$nb]
 set L_for_mediane [lsort -real $L_for_mediane]
   set mediane [lindex $L_for_mediane [expr $nb/2]]
 #set dec_x 0; set dec_y 0
  # Fliter on pointer movment (last past 10)
   set dec_x 0; set dec_y 0; set blob_max [lindex $this($id,blob) $pos_max]
   set blob_init [lindex $this($id,blob) [expr ($pos_max-9 >=0)?$pos_max-9:0]]; set prev_x [lindex $blob_init 1]; set prev_y [lindex $blob_init 2]
   foreach blob [lrange $this($id,blob) [expr $pos_max - 8] $pos_max] {
     set dec_x [expr $dec_x + [lindex $blob $this(i_x)] - $prev_x]
	 set dec_y [expr $dec_y + [lindex $blob $this(i_y)] - $prev_y]
    }
 if {$max > 2.5*$mediane && 2*$pos_max > $nb_blob} {
   if {$dec_x*$dec_x + $dec_y*$dec_y < 15} {
     puts "PIPO CLIC"; set this($id,blob) [list [lindex $this($id,blob) end]]
	 #DEBUG $ptr Add_MetaData clic 1
	 #DEBUG this Generate_clic_event_for $ptr $id $blob_max [ALX_pointeur_enfonce]
	} 
  }
 $ptr Add_MetaData moy $moy
 $ptr Add_MetaData max $max
 $ptr Add_MetaData mediane $mediane
 $ptr Add_MetaData delta "$dec_x $dec_y"
#$ptr Add_MetaData pos_max $pos_max
 #puts "[lindex $b 0] on <$max, $moy>"
}

#_____________________________________________________________________________________________________
method Liant_Mutable_sim_ptr Ptr_historic {id} {return $this($id,blob)}

#_____________________________________________________________________________________________________
method Liant_Mutable_sim_ptr Update_ptr_representation {ptr id blob} {
 set node [$ptr Noeud_representation]
 if {[string equal $node NULL]} {return}
 set node [Real_class $node]
 $node Couleur 1 1 1 0.7; 
 #DEBUG $node Etirement 0.1 0.1; 
 set nb [llength $this($id,blob)]
 set max [$ptr Val_MetaData max]; if {[string equal $max {}]} {return}
   set max [lindex [lindex $this($id,blob) end] $this(i_I)]
#set moy [$ptr Val_MetaData moy]
 set mediane [$ptr Val_MetaData mediane]
#set pos_max [$ptr Val_MetaData pos_max]
 set delta   [$ptr Val_MetaData delta]
   set dx [lindex $delta 0]
   set dy [lindex $delta 1]
   if {$dx*$dx + $dy*$dy >= 15} {return}
 set r [expr $max / $mediane] 
 #DEBUG $node Etirement $r $r
# return
 set clic [$ptr Val_MetaData clic]
 set translucidite [expr $max / (2.5*$mediane)]
 if {$clic} {$node Couleur 0 1 0 1} else {$node Couleur 1 1 1 $translucidite}
 return
}

#_____________________________________________________________________________________________________
method Liant_Mutable_sim_ptr Generate_mvt_event_for {ptr id blob} {
 #puts "$objName Generate_mvt_event_for $ptr $id $blob"
 set x  [lindex $blob $this(i_x)]    ; set y [lindex $blob $this(i_y)]
   $ptr Add_MetaData I [lindex $blob $this(i_I)]
   $ptr Add_MetaData W [lindex $blob $this(i_W)]
   $ptr Add_MetaData H [lindex $blob $this(i_H)]
   #this Ptr_evt_time $ptr $id $blob
   this Update_ptr_representation $ptr $id $blob
 set dx [expr $this(ptr,$id,x) - $x]; set dy [expr $this(ptr,$id,y) - $y]
 if {($dx*$dx + $dy*$dy) >= $this(threshold_mov)} {
   set this(ptr,$id,x) $x; set this(ptr,$id,y) $y
   $this(sim_ptr_TCL) convertir_coordonnees $x  $y  $this(B207_pt_tmp_coord)
   $this(sim_ptr_TCL) convertir_coordonnees $dx $dy $this(B207_pt_tmp_dep)
   $this(B207_info_comp) Dep   $this(B207_pt_tmp_dep)
   $this(B207_info_comp) Coord $this(B207_pt_tmp_coord)
   $this(B207_evt_ptr) maj $id [ALX_pointeur_mouvement] $this(B207_info_comp) [$this(sim_ptr_TCL) Noeud_pere]
  # Distribute event
  #puts a
     #for {set i 0} {$i < 5} {incr i} {
	   B_sim_ptr Ajouter_evt $this(B207_evt_ptr)
	 # }
  }
}

#_____________________________________________________________________________________________________
method Liant_Mutable_sim_ptr get_new_presentation_node {ptr blob} {
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
method Liant_Mutable_sim_ptr Release_presentation_node {n} {
 if {[lsearch $this(L_ptr_preso_nodes) $n] != -1} {puts "Liant_Mutable_sim_ptr::Release_presentation_node $n\n  ALLREADY released..."; return}
 lappend this(L_ptr_preso_nodes) [Real_class $n]
}

#_____________________________________________________________________________________________________
method Liant_Mutable_sim_ptr Frame_for_B207 {f_name} {
 #return
 #puts "Frame_for_B207"
 set i_id $this(i_id)
 set i_I  $this(i_I)
 set i_x  $this(i_x)
 set i_y  $this(i_y)
 set i_W  $this(i_W)
 set i_H  $this(i_H)
 
 upvar $f_name f
 set L_desappear_id {}
# Look in existing pointers if there are some mappings...take care a point do not displace itself too much...
 foreach id $this(L_MuTable_ids) {
   set ptr [B_sim_ptr Pointeur_id $id]
   #puts "array ptr_id : [array get this ptr_id,*]"
   #puts "ptr $id = $ptr"
   if {$ptr != "NULL"} {
    # Look in new frame is one point seems to correspond to this one...
	 set num_blob -1; set pos 0; set dist_min 64000
	 foreach blob $f {
	   #puts "  Considering blob num $pos {$blob}"
	   if {[info exists this(ptr_id,[lindex $blob $i_id])]} {
	     set blob_id $this(ptr_id,[lindex $blob $i_id])
	     #puts "blob_id = $blob_id"
	     if {$blob_id == $id} {set num_blob $pos; break;}
	    }
       incr pos
	  }
	if {$num_blob == -1} {
	 # If not found, let's add it to disappearing B207 points
	  #puts "Ptr $id is not in the frame (num_blob == -1)  ... it disappears\n  frame : $f"
	  lappend L_desappear_id $id
	 } else {
	         # If we found, let's move and clic...and release the blob...
			 #puts "Generate_mvt_event_for blob $num_blob : [lindex $f $num_blob]"
			 this Generate_mvt_event_for $ptr $id [lindex $f $num_blob]
			 #puts OK
			 set f [lreplace $f $num_blob $num_blob]
			}
	} else {puts "ptr IS NULL, it disappears"; 
	        lappend L_desappear_id $id
		   }
  }
  
 
 #puts coucou  
# Add points of the frame that are not in B207
 foreach blob $f {
   #puts "New pointeur to add! {$blob}"; continue; 
   set id [B_sim_ptr Ajouter_pointeur "MuTable pointer" [N_i_mere Noeud_scene] 1]
     set this($id,blob) [list $blob]
   set this(ptr_id,[lindex $blob $this(i_id)]) $id
   #puts "New pointer : id $id with\n  blob : $blob\n  this(ptr_id,[lindex $blob $this(i_id)]) = $this(ptr_id,[lindex $blob $this(i_id)])"
   lappend this(L_MuTable_ids) $id
   set ptr [B_sim_ptr Pointeur_id $id]
   if {[string equal $ptr NULL]} {puts "Pointeur créé mais NULL..."; return}
   set this(ptr,$id,x) [lindex $blob $i_x]
   set this(ptr,$id,y) [lindex $blob $i_y]
     $ptr Coord [lindex $blob $i_x] [lindex $blob $i_y]
   $ptr Add_MetaData I [lindex $blob $i_I]
   $ptr Add_MetaData W [lindex $blob $i_W]
   $ptr Add_MetaData H [lindex $blob $i_H]
   $ptr Noeud_representation [this get_new_presentation_node $ptr $blob]
   $this(sim_ptr_TCL) convertir_coordonnees $this(ptr,$id,x) $this(ptr,$id,y) $this(B207_pt_tmp_coord)
     $ptr Coord [$this(B207_pt_tmp_coord) X] [$this(B207_pt_tmp_coord) Y]
   [$ptr Noeud_representation] Origine $this(B207_pt_tmp_coord)
  # Generate also a clic?
     $ptr Coord [$this(B207_pt_tmp_coord) X] [$this(B207_pt_tmp_coord) Y]
   this Generate_clic_event_for $ptr $id $blob [ALX_pointeur_enfonce]
  }
 #puts hein?
# Release B207 points that are no more in MuTable
 foreach id $L_desappear_id {
   #puts "Disappearing pointer : id $id"
   foreach {index val} [array get this ptr_id,*] {
     if {$val == $id} {unset this($index)}
    }
   #puts "L_ptr : {[array get this ptr_id,*]}"
   
   set ptr [B_sim_ptr Pointeur_id $id]
   this Generate_clic_event_for $ptr $id "$id 1000 [$ptr X] [$ptr Y] 10 10" [ALX_pointeur_relache]
   this Release_presentation_node [$ptr Noeud_representation]
   if {[B_sim_ptr Retirer_pointeur $id] == -1} {puts "__!!!__Problème pour enlever le pointeur $id"}
   [$ptr Noeud_representation] Vider_peres
   set pos [lsearch $this(L_MuTable_ids) $id]
   set this(L_MuTable_ids) [lreplace $this(L_MuTable_ids) $pos $pos]
  }
 #puts fin...
}

#_____________________________________________________________________________________________________
method Liant_Mutable_sim_ptr Chrono_frame_mutable_start {} {
 set this(nb_frame_mutable) 0
 set this(tps_chrono) [clock clicks -milliseconds]
}

#_____________________________________________________________________________________________________
method Liant_Mutable_sim_ptr Chrono_frame_mutable_frame_rate {} {
# puts "$objName Chrono_frame_mutable_frame_rate"
 set t [clock clicks -milliseconds]
 set dt [expr $t - $this(tps_chrono)]
   puts "Chrono :\n  - frames : $this(nb_frame_mutable)\n  -    tps : $dt\n  -   rate : [expr 1000*$this(nb_frame_mutable)/${dt}.0]"
 set this(nb_frame_mutable) 0
 set this(tps_chrono) [clock clicks -milliseconds]
}
 
#_____________________________________________________________________________________________________
method Liant_Mutable_sim_ptr Save_Data {L_ptr_name} {
 if {$this(B207_is_reading)} {puts "Problem thread in Liant_Mutable_sim_ptr::Save_Data"}
 incr this(nb_frame_mutable)
 upvar $L_ptr_name L_ptr
   catch "$this(CB_new_frame) L_ptr"
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
method Liant_Mutable_sim_ptr Update_ptr_MuTable {nb L_ptr_name} {
 #puts "$objName Update_ptr_MuTable"
 upvar $L_ptr_name L_ptr

  #if {$this(B207_on)} {
   this Save_Data L_ptr
   return
  #}

# set nb [$this(MuTable) Nb_ptr]
   $this(tk_table) configure -rows [expr $nb+1]
   global $this(tk_table_var)
 set i 1  
 foreach ptr $L_ptr {
   set $this(tk_table_var)($i,0) [lindex $ptr $this(i_id)]
   set $this(tk_table_var)($i,1) [lindex $ptr $this(i_x)]
   set $this(tk_table_var)($i,2) [lindex $ptr $this(i_y)]
   set $this(tk_table_var)($i,3) [lindex $ptr $this(i_I)]
   set $this(tk_table_var)($i,4) [lindex $ptr $this(i_W)]
   set $this(tk_table_var)($i,5) [lindex $ptr $this(i_H)]
   incr i
  }
}

#_____________________________________________________________________________________________________
method Liant_Mutable_sim_ptr startListening {port} {
 if {$this(B207_on)} {N_i_mere abonner_a_simulateur [$this(rap_ptr_TCL) Rappel]}

#$this(MuTable) startListening
 set this(udp_srv) [udp_open $port]
 fconfigure $this(udp_srv) -buffering none -translation binary
 fileevent $this(udp_srv) readable [list $objName udpEventHandler $this(udp_srv)]
   #puts "Listening on udp port: [fconfigure $this(udp_srv) -myport]"
   return $this(udp_srv)
}

#_____________________________________________________________________________________________________
method Liant_Mutable_sim_ptr stopListening  {} {
 close $this(udp_srv)
}

#_____________________________________________________________________________________________________
method Liant_Mutable_sim_ptr decode_mutable {nb txt} {
 
 set txt [string range $txt 4 end]
 set L {}
 #puts "Frame: $nb points"
 for {set n 0} {$n<$nb} {incr n} {
   binary scan $txt ifffff id int x y w h; set txt [string range $txt 24 end]
   #puts "  * $id $int $x $y $w $h"
   lappend L "$id $int [expr $x * $this(x_stretch)] [expr $y * $this(y_stretch)] $w $h"
  }
  
 this Update_ptr_MuTable $nb L 
}

#_____________________________________________________________________________________________________
method Liant_Mutable_sim_ptr udpEventHandler {sock} {
    set pkt [read $sock]; 
    set peer [fconfigure $sock -peer]
    binary scan $pkt i nb; 
	#puts "$peer: [string length $pkt] {$nb}";
	if {[catch {set L_ptr [this decode_mutable $nb $pkt]} err]} {
	  puts "ERROR : $err"
	 }
}

puts "Liant_Mutable_sim_ptr Liant_mutable; Liant_mutable startListening 8910"

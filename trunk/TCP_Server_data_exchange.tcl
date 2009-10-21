method GogoServer constructor {port} {
 set this(L_nodes)             ""
 set this(rap_ask_for_server)  [B_rappel [Interp_TCL]]; $this(rap_ask_for_server)  Texte "$objName Send_infos_server $this(rap_ask_for_server)"
 set this(rap_rep_from_server) [B_rappel [Interp_TCL]]; $this(rap_rep_from_server) Texte "$objName Save_infos_server $this(rap_rep_from_server)"
 set this(port)                $port
 set this(bind_r66_rep)        [new_R66_B_Rappel_TCL_binding [N_i_mere Route_66] \
                                                             "^GogoServer from (.*) is at TCP port (.*)\$" \
							 								 [$this(rap_rep_from_server) Rappel] ]
 set this(bind_r66_ask)        [new_R66_B_Rappel_TCL_binding [N_i_mere Route_66] \
                                                             "^Get GogoServer from [N_i_mere IP]\$" \
								 	 	 					 [$this(rap_ask_for_server) Rappel] ]
 set this(rap_creation_replicated_node) [B_rappel [Interp_TCL]]
   $this(rap_creation_replicated_node) Texte "ggs Add \[Void_vers_noeud \[$this(rap_creation_replicated_node) Param\]\]"
   N_i_mere abonner_a_creation_noeud_replique [$this(rap_creation_replicated_node) Rappel]
   
 set this(TCP_Server)          [new_Serveur_TCP_TCL $port]
   set SlimTcpConnect [$this(TCP_Server) get_SlimTcpConnectionHandler_TCL]
   set buffer [$SlimTcpConnect Allouer_un_Buffer 1024]
   set this(char_buffer) [Void_vers_string $buffer]

   set this(rap_on_TCP_query)  [B_rappel [Interp_TCL]]; $this(rap_on_TCP_query) Texte "$objName Interp_msg $this(rap_on_TCP_query)"
   $this(TCP_Server) abonner_a_nouvelle_connection [$this(rap_on_TCP_query) Rappel]
}

#____________________________________________________________________________
method GogoServer dispose {port} {
 Detruire $this(TCP_Server)
 this inherited
}

#____________________________________________________________________________
method GogoServer get_server  { } {return $this(TCP_Server);}
method GogoServer Add         {n} {set n [Real_class $n]; if {[lsearch $this(L_nodes) $n] == -1} {lappend this(L_nodes) $n}}
method GogoServer get_L_nodes { } {return $this(L_nodes)}

#____________________________________________________________________________
method GogoServer Send_infos_server {} {
 [N_i_mere Route_66] sendMessage "GogoServer from [N_i_mere IP] is at TCP port $this(port)"
}

#____________________________________________________________________________
method GogoServer Send_infos_server {rap} {
 [N_i_mere Route_66] sendMessage "GogoServer from [N_i_mere IP] is at TCP port $this(port)"
}

#____________________________________________________________________________
method GogoServer Save_infos_server {rap} {
 set R66_B_Rap [Void_vers_R66_B_Rappel_TCL_binding [$rap Param]]
 if {[$R66_B_Rap Nb_args] < 2} {return}
 set id [$R66_B_Rap Arg_num 0]
 regexp {^(.*)_.*$} [N_i_mere IP] reco ip
 set this(Server_port,$id) [list $ip [$R66_B_Rap Arg_num 1]]
}

#____________________________________________________________________________
method GogoServer Send_buffer_image {Threaded_TCP_client img} {
 set tempon [$img Tempon_const]
 set taille [$img Taille_Tempon]
 set this(txt) "Send_buffer_image $tempon $taille"
 set s [string length $taille] 
 set txt [string replace "          " 0 [expr $s -1] $taille]
 $Threaded_TCP_client sendData_fromChar $txt 10
 $Threaded_TCP_client sendData_fromVoid $tempon $taille
}

#____________________________________________________________________________
method GogoServer Find_local_node_nammed {name} {
 return [Find_local_node_nammed $name]
}
#____________________________________________________________________________
proc Find_local_node_nammed {name} {
 if {![info exists this(local_node,$name)]} {
   N_i_mere Demande_acces
     set L [Lister_noeuds_depuis [N_i_mere Noeud_scene]]
     set n [Trouver_noeud_nomme $name L]
   N_i_mere Liberer_acces
   if {[string equal $n ""]} {return ""} else {set this(local_node,$name) $n}
  }
 
 return $this(local_node,$name)
}

#____________________________________________________________________________
method GogoServer Interp_msg {rap} {
 set this(txt) "$objName Interp_msg"
 set Threaded_TCP_client [Void_vers_Threaded_TCP_client [$rap Param]]
   #puts "Wait for data on $SlimTcpConnect"
   set taille_buffer 1024
   set buffer [$Threaded_TCP_client Allouer_un_Buffer $taille_buffer]
   set nb [$Threaded_TCP_client waitingForData_fromVoid $buffer $taille_buffer 0 1]
     set this(txt) "Received $nb bytes : \"[Void_vers_string $buffer]\""
     set txt [Void_vers_string $buffer]
	 if {[regexp {^get texture of (.*)$} $txt reco nom_noeud]} {
	   set this(txt) "on demande la texture de $nom_noeud"
	   set noeud [this Find_local_node_nammed $nom_noeud]
	   if {[string equal $noeud ""]} {
	     set this(txt) "noeud $nom_noeud pas trouvé"
		 $Threaded_TCP_client sendData_fromChar "0 " 2
		} else {set this(txt) "noeud $nom_noeud trouvé"
		        this Send_buffer_image $Threaded_TCP_client $noeud
				set this(txt) "Tempon de $nom_noeud émis"
			   }
	  } else {$Threaded_TCP_client sendData_fromChar "6 coucou" 8
	         }
 $Threaded_TCP_client Liberer_buffer $buffer
}

#____________________________________________________________________________
proc Ask_for_texture_msg {ip port img_name local_img} {
 set L_rap [new_Liste_de_rappel]
 set Threaded_TCP_client [new_Threaded_TCP_client 0 $L_rap]
   set rap [B_rappel [Interp_TCL] "Get_texture $Threaded_TCP_client $img_name $local_img"]
   $L_rap Ajouter_methode_a_la_fin [$rap Rappel]
   $Threaded_TCP_client Update_L_rappel $L_rap
   $Threaded_TCP_client getConnectionTo $ip $port
   $Threaded_TCP_client resume
}

#____________________________________________________________________________
proc Get_texture {Threaded_TCP_client img_name local_img} {
 set taille_txt_buffer 10
 set msg "get texture of $img_name"
 set this(txt) $msg
 $Threaded_TCP_client sendData_fromChar $msg [string length $msg]
 set this(txt) "Datas sent"
 set txt_buffer [$Threaded_TCP_client Allouer_un_Buffer [expr $taille_txt_buffer + 1]]
 set this(txt) "Allouer buffer : $txt_buffer"
   $Threaded_TCP_client waitingForData_fromVoid $txt_buffer $taille_txt_buffer 0 1
   set this(txt) "Data received"
   set received [Void_vers_string $txt_buffer]
   if {[regexp {^([0-9]*) } $received reco taille_data_buffer]} {
     set this(txt) "Taille reconnue"
	 set data_buffer [$Threaded_TCP_client Allouer_un_Buffer $taille_data_buffer]
	 set this(txt) "Data alloués"
	 set recut 0
	 #while {$recut < $taille_data_buffer} {
	   incr recut [$Threaded_TCP_client waitingForData_fromVoid $data_buffer $taille_txt_buffer $recut 0]
	 #  if {$recut <= 0} {break}
	   set this(txt) "Recut $recut octets"
	 # }
	 set this(txt) "Tout recut"
	 set tx [$local_img Lg]
	 set ty [$local_img Ht]
	 set sz [$local_img Nb_octets_par_pixel]
	 set or [$local_img Ordonnancement_couleurs]
	 set ti [expr $tx * $ty * $sz]
	 set taille_raw [expr ($ti<$recut)?$ti:$recut]
	# Multi-threads !!!!
	# Fire&Forget d'une mise à jour de l'image succédée d'une libération du data_buffer
	 set this(txt) "Avant demande acces"
	 N_i_mere Demande_acces
	   set cmd "$local_img maj_raw $tx $ty $or $sz $data_buffer"
	   set this(txt) "B_rappel_Fire_Forget"
	   B_rappel_Fire_Forget [N_i_mere] abonner_a_fin_simulation $cmd
	 N_i_mere Liberer_acces
	 set this(txt) "Apres liberation acces"
    } else {set this(txt) "Commande non reconnu"}
 set this(txt) "Liberation texte buffer $txt_buffer"
 $Threaded_TCP_client Liberer_buffer $txt_buffer
}

#___________________________________________________________________________
method GogoServer get_txt {} {return $this(txt)}

#GogoServer ggs 8000


  
  
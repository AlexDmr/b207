source x.tcl
  set server [new_Texture_Server_TCP 8010 [N_i_mere]]
  catch "$canvas Vider_peres"
  set img [B_image chat_souris.png]; $img Origine 320 0; $noeud_partage Ajouter_fils $img
  N_i_mere Se_rajouter_a 1 $noeud_fen 1 1
  #puts "Find_local_node_nammed ...\nnew_Image_net_loader 172.18.15.27 8000 \[N_i_mere\] ..."

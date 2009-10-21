source demo_dsvis2008.tcl
proc Gogo_connection {} {
  global noeud_fen
  set server [new_Texture_Server_TCP 8010 [N_i_mere]]
  N_i_mere Se_rajouter_a 1 $noeud_fen 1 1
  Positionner_partage 1920 1080 3.14159265 0.35 0.35
}
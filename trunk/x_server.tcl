source x.tcl
#source ALX_Drawer.tcl

set server [new_Texture_Server_TCP 8000 [N_i_mere]]
  set img [B_image terrain.png]
  $noeud_partage Ajouter_fils $img
  N_i_mere Creer_groupe 1 essai $noeud_fen 1 1
  puts "Image : [[$img Nom] Texte]"
  #ALX_Drawer drawing_app
  #  $noeud_partage Ajouter_fils [drawing_app get_canvas]
  
  $noeud_partage Position_des_fils_changeable 0

  set poly [B_polygone]; $poly Origine 300 300; $poly Ajouter_contour [ProcOvale 0 0 100 150 60]; $canvas Ajouter_fils $poly
  
  
  proc Super_test_Texture_Server_TCP {img L i t} {
    $img maj [lindex $L $i]
	after $t "Super_test_Texture_Server_TCP $img {$L} [expr ($i+1)%[llength $L]] $t"
   }

  proc Super_test_Texture_Server_TCP2 {pere img L i t} {
    $img Vider_peres
	set img [B_image [lindex $L $i]]
	$pere Ajouter_fils $img
	after $t "Super_test_Texture_Server_TCP $pere $img {$L} [expr ($i+1)%[llength $L]] $t"
   }
      
  set L [list chat_souris.png terrain.png Texture_cyan_006.bmp Texture_multicolor_009.bmp texture_1.bmp]
  puts {Super_test_Texture_Server_TCP $img $L 0 500}
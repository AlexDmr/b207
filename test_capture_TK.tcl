source groupe.tcl
proc etire_fond args {}
set img [B_image]
  B_configure $img -Noeud_touchable 0 -Afficher_boites 1 -Translucidite 0

$noeud_partage Ajouter_fils $img
load {./interfaces/TK_capture_bitmap/Debug/Tk_capture_bitmap.dll}
N_i_mere Afficher_souris

proc capture_tk_win_in {w img} {
 wm deiconify $w 
 set    cmd {}
 append cmd {set L [Get_bitmap_from_TK_win } $w {]; puts $L; }
 append cmd $img { maj_raw [lindex $L 0] [lindex $L 1] [GL_bvra] [lindex $L 2] [get_Buffer_for_HBITMAP_pixels]; wm iconify } $w
 after 100 $cmd
}

return {set L_infos [Get_bitmap_from_TK_win .]}

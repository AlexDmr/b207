set n_i_mere [$i_mere get_Noeud_interface]
set Arbre_scene [$n_i_mere Arbre_scene]
set Noeud_scene [$n_i_mere Noeud_scene]
set ccn $CometCamNote
set ccn_bigre [$ccn get_CamNote_bigre]
set nccn [$ccn_bigre Noeud_scene]
set occn [$ccn_bigre Olfa_comet]
set vcn $Cam_visu
set tcn $Cam_tele
set fvcn  [$vcn get_CamVisu_bigre]
set ftcn  [$tcn get_CamTele_bigre]
set nfvcn [$fvcn Noeud_fenetre_sdl_opengl]
set nftcn [$ftcn Noeud_fenetre_sdl_opengl]


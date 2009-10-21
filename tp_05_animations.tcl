source tp_04_dragdrop.tcl

#______________________________________________________________________________________________________
# On modifie l�g�rement le graphe de sc�ne du TP4 _____________________________________________________
# en intercalant un noeud racine pour le panneau ______________________________________________________
# Ce noeud servira � brancher un bouton pour enrouler/d�rouler ________________________________________
#______________________________________________________________________________________________________
$root_panel_drop Retirer_fils $panel
set panel_root [B_noeud]
$root_panel_drop Ajouter_fils_au_debut $panel_root
$panel_root      Ajouter_fils $panel
$panel_root      Position_des_fils_changeable 0

#______________________________________________________________________________________________________
# On se base sur le TP 4 pour que le panneau devienne d�roulant _______________________________________
# On va ajouter un 'bouton' qui va permettre de d�rouler / enrouler le panneau ________________________
# D�finissons un petit objet qui va g�rer l'enroulement et le d�roulement du panneau __________________
# La liste L_img repr�ente une liste d'image repr�sente les image pour enrouler et pour d�rouler ______
# panel_root repr�sente le noeud racine du panneau ____________________________________________________
# panel repr�sente le panneau _________________________________________________________________________
#______________________________________________________________________________________________________
source B_Panel.tcl

#______________________________________________________________________________________________________
# Le TP proprement dit...on cr�e un panneau, on initialise des images _________________________________
#______________________________________________________________________________________________________
set img_bt_left  [B_image BDec.bmp]
set img_bt_right [B_image BInc.bmp]

Panel P $panel_root $panel [list $img_bt_right $img_bt_left]


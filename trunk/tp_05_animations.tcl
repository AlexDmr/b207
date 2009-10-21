source tp_04_dragdrop.tcl

#______________________________________________________________________________________________________
# On modifie légèrement le graphe de scène du TP4 _____________________________________________________
# en intercalant un noeud racine pour le panneau ______________________________________________________
# Ce noeud servira à brancher un bouton pour enrouler/dérouler ________________________________________
#______________________________________________________________________________________________________
$root_panel_drop Retirer_fils $panel
set panel_root [B_noeud]
$root_panel_drop Ajouter_fils_au_debut $panel_root
$panel_root      Ajouter_fils $panel
$panel_root      Position_des_fils_changeable 0

#______________________________________________________________________________________________________
# On se base sur le TP 4 pour que le panneau devienne déroulant _______________________________________
# On va ajouter un 'bouton' qui va permettre de dérouler / enrouler le panneau ________________________
# Définissons un petit objet qui va gérer l'enroulement et le déroulement du panneau __________________
# La liste L_img repréente une liste d'image représente les image pour enrouler et pour dérouler ______
# panel_root représente le noeud racine du panneau ____________________________________________________
# panel représente le panneau _________________________________________________________________________
#______________________________________________________________________________________________________
source B_Panel.tcl

#______________________________________________________________________________________________________
# Le TP proprement dit...on crée un panneau, on initialise des images _________________________________
#______________________________________________________________________________________________________
set img_bt_left  [B_image BDec.bmp]
set img_bt_right [B_image BInc.bmp]

Panel P $panel_root $panel [list $img_bt_right $img_bt_left]


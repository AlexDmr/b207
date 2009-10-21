if {[info exists noeud_partage]} {} else {source groupe.tcl}
proc etire_fond args {}
#N_i_mere Afficher_souris

load multimouse.dll

set nb_mice [ManyMouse_Init]
set sim_ptr_usb [new_alx_simulateur_pointeur_souris_USB_raw [N_i_mere Ecran_X] [N_i_mere Ecran_Y] [N_i_mere Coin_BG] [N_i_mere Vecteur_X] [N_i_mere Vecteur_Y]]

set id_usb_mouse [$sim_ptr_usb Ajouter_rapporteur [B_sim_ptr] [N_i_mere Noeud_scene] $nb_mice]

$sim_ptr_usb Noeud_pere [N_i_mere Noeud_scene]
for {set i 0} {$i < $nb_mice} {incr i} {
  set ptr [B_sim_ptr Pointeur [expr $i+$id_usb_mouse]]
  $ptr Noeud_representation [new_alx_noeud_pointeur $ptr]
 }

set rap_ptr_usb [B_rappel [Interp_TCL] "$sim_ptr_usb simuler"]
N_i_mere abonner_a_simulateur [$rap_ptr_usb Rappel]

set rap_post_ptr_usb [B_rappel [Interp_TCL] "if \{\[$sim_ptr_usb Need_scan\]\} \{$sim_ptr_usb Scan\}"]
N_i_mere abonner_a_fin_simulation [$rap_post_ptr_usb Rappel]

[N_i_mere Sim_ptr_souris] Retirer_rapporteur [B_sim_ptr]
B_sim_ptr Retirer_pointeur 0

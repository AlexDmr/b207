# On source groupe.tcl si �a n'est pas d�ja fait (pr�sence du noeud noeud_partage)
# groupe.tcl instancie des �l�ments de l'interface comme la fen�tre de navigation et l'image de fond
# groupe.tcl donne acc�s � la console de commande tkcon.
if {[info exists noeud_partage]} {} else {source groupe.tcl}
proc etire_fond args {}
# On demande explicitement � ce que le curseur syst�me de la souris soit affich�
# S'il ne l'est pas, le curseur ne peut pas sortir de la fen�tre openGL...
N_i_mere Afficher_souris

# On va d�finir le contour d'un polygone � l'aide d'un tableau de points
# Une fa�on de faire et de r�server de la place et d'initialiser les valeurs...c'est long...
set tab1 [Tab_double]
$tab1 Reserver_place 16
$tab1 Ajouter 0  -40 0
$tab1 Ajouter 2  200 0
$tab1 Ajouter 4  200 20
$tab1 Ajouter 6  20  20
$tab1 Ajouter 8  20  180
$tab1 Ajouter 10 200 180
$tab1 Ajouter 12 200 200
$tab1 Ajouter 14 0   200

# Une autre fa�on de faire (plus simple!!!) est de passer par la proc�dure ProcTabDouble
# qui va transformer une liste TCL en tableau manipulable par B207.
# D'autre proc�dure comme ProcOvale, ProcOeuf ou ProcRect d�finissent une liste de points 
# correspondant � une forme g�om�trique particuli�re.
set tab2 [ProcTabDouble [list -20 -20 220 -20 220 220 -20 220]]

# On d�finit ensuite un noeud polygone
set poly [B_polygone]
# On lui ajoute un contour
$poly Ajouter_contour $tab2

# On le place ensuite dans le graphe de sc�ne
$noeud_partage Ajouter_fils $poly

# On d�finit un nouveau contour
set tab_ovale [ProcOvale 100 100 75 40 60]
# On soustrait ce contour du polygone
$poly Retirer_contour $tab_ovale

# On texture le polygone avec la texture de l'image de fond
$poly Info_texture [$img_fond Info_texture]

# On g�re ensuite le contact d'un curseur (souris ou autre) avec ce polygone
### 1  : [Capa_T] translation
### 2  : [Capa_R] rotation
### 3  : 1+2 = translation et rotation
### 8  : [Capa_E] etirement
### 9  : 8+1 = etirement et translation (ne maintient pas les proportions)
### 11 : 8+3 = etirement et rotation (maintient les proportions)
B_contact poly_ctc "$poly [Capa_T]"

# D�finition de polygones ovals fils de poly
set pt_milieu_o [B_point]
$pt_milieu_o maj 100 100
for {set i 0} {$i < 4} {incr i} {
  # On trouve un nom de variable, on le stock dans name
  set name "poly_oval_$i"
  puts $name
  # on associe un polygone au nom de variable contenu dans name, on peut d�sormais acc�der au polygone � l'aide de [subst $$name]
  set $name [B_polygone]
  
  if {[expr "($i % 2) == 0"]} {$poly Ajouter_fils [subst "$$name"]
                                } else {eval "\$poly_oval_[expr $i-1] Ajouter_fils \$$name"
                                       }
  [subst $$name] Ajouter_contour $tab_ovale
  B_contact ${name}_ctc "[subst $$name] 3"
  [subst $$name] Couleur [expr rand()] [expr rand()] [expr rand()] 1
 }

# Gestion du polygone avec d�grad� de couleur
set poly_couleur [B_polygone]
$poly Ajouter_fils $poly_couleur
B_configure $poly_couleur -Ajouter_contour $tab_ovale \
                          -Translation 100 100 \
                          -Rotation 1

# M�thode d'interpolation des couleur
# TOUJOURS [get_Fonction_interpolation_poly_boite_englobante]
# Les donn�es dans tab_col correspondent aux couleur RVBA (Rouge Vert Bleu Alpha) 
# des 4 sommets du rectangle englobant le polygone � colorier (Bas gauche, bas droit, haut droit, haut gauche)
# La m�thode Coords_couleurs permet de dire que le coloriage sera non uniforme et bas� sur les couleurs pass�es en param�tre
$poly_couleur Fct_interpollation [get_Fonction_interpolation_poly_boite_englobante]
  set tab_col [ProcTabDouble "0 0 0 0 \
                              1 1 1 1 \
							  1 1 1 1 \
							  0 0 0 0" ]
$poly_couleur Coords_couleurs [$tab_col Tab]
$poly_couleur maj

# Gestion d'une palette � la toolglass
# On d�finit une fentre qui va contenir des outils toolglass
set f_tools [B_fiche "ToolGlass" 240 320 [N_i_mere]]
$noeud_partage Ajouter_fils $f_tools
B_configure $f_tools -Couleur_fond 0 0 1 1 \
                     -Fenetre_translucide 1 \
                     -Empiler_contenant 1

set poly_plus [B_polygone]; set contour   [ProcTabDouble {0 0 100 0 100 100 0 100}]
$f_tools Ajouter_fils $poly_plus
B_configure $poly_plus -Ajouter_contour $contour \
                       -Translucidite 0 \
                       -Couleur 0 1 0 0.5 \
                       -Empiler_contenant 1 

# La proc�dure C_B_Toolglass permet de d�finir un noeud B207 quelconque comme �tant une toolglass
# C'est � dire qu'on va pouvoir cliquer "au travers" et d�clancher une commande TCL (deuxi�me argument)
# Cette commande peut utiliser la variable $noeud qui sera connu lors de son ex�cution SEULEMENT.
# $noeud d�signera le noeud ur lequel on a cliqu� au travers de la toolglass.
C_B_Toolglass $poly_plus {$noeud Etirer_de 1.1 1.1}

# Abonnement � la d�tection d'�vennement d'un pointeur
### 1 : [ALX_pointeur_enfonce]
###  2 : [ALX_pointeur_relache]
### 16 : [ALX_pointeur_apparition]
### 32 : [ALX_pointeur_disparition]
### 64 : [ALX_pointeur_mouvement]
### ** : [ALX_pointeur_ALL]

set rap1 [B_rappel [Interp_TCL]]   ; $rap1 Texte "puts \{ENTER in poly\}"
  $poly abonner_a_detection_pointeur [$rap1 Rappel]     [expr [Rappel_Seulement_si_noeud_contient]+[ALX_pointeur_apparition]]
set rap2 [B_rappel [Interp_TCL]]   ; $rap2 Texte "puts \{ENTER in poly daughters\}"
  $poly abonner_a_detection_pointeur [$rap2 Rappel]     [expr [Rappel_Seulement_si_fils_contient]+[ALX_pointeur_apparition]]
set rap_out [B_rappel [Interp_TCL]]; $rap_out Texte "puts \{LEAVE of poly and poly_daughters\}; Display_infos $rap2"
  $poly abonner_a_detection_pointeur [$rap_out Rappel]  [expr [Pas_rappel_si_fils_contient]+[ALX_pointeur_disparition]]
set rap_out2 [B_rappel [Interp_TCL] "puts \{LEAVE of poly for one daughter\}"]
  $poly abonner_a_detection_pointeur [$rap_out2 Rappel] [expr [Rappel_Seulement_si_fils_contient]+[ALX_pointeur_disparition]]

# Display_infos affiche les donn�es li�es � l'�vennement pointeur
# On prend en param�tre une fonction de rappel (B_rappel)
# Chaque objet B_rappel contient un param�tre accessible via la m�thode Param, cet attribut est typ� void*
# Dans notre cas, cet attribut est un objets 'Information', on fait donc un cast explicit (proc�dure Void_vers_info)
# L'objet 'Information' contient une liste de rep�re (L_REPERES : de la racine jusqu'au noeud cliqu�) et le noeud cliqu� (NOEUD).
proc Display_infos {rap} {
 set infos [Void_vers_info [$rap Param]]
 set L_rep [$infos L_REPERES]
 set noeud [$infos NOEUD]
 puts "  Infos $infos\n    L_rep.Taille() = [$L_rep Taille]\n    L_noeuds : [L_repere_vers_L_noeud_scene $L_rep]\n    noeud ([$noeud Nom_IU]) = $noeud"
}

return {$poly Retirer_contour $tab1 Tab}


source tp1.tcl

$noeud_editeur_tcl Vider_peres
set fond [B_polygone]
  set txt [B_texte 50 "coucou les amis"]; set box [$txt Boite_noeud]
    $txt Mode_texture 1; $txt Couleur_fond 0 0 0 1
  B_configure $fond -Ajouter_contour [ProcRect [expr [$box BG_X] - 5] [expr [$box BG_Y] - 5] [expr [$box HD_X] + 5] [expr [$box HD_Y] + 5] ] \
                    -Ajouter_fils $txt
$noeud_partage Ajouter_fils_au_debut $fond
B_bouton_perso bt $txt \
                  "$txt Couleur_texte 0   1 0   1;" \
			      "$txt Couleur_texte 1   1 1   1;" \
			      "$txt Couleur_texte 0.5 1 0.5 1; $txt Couleur_fond 0 0.7 0 1" \
				  "$txt Couleur_texte 0   1 0   1; $txt Couleur_fond 0 0   0 1" \
				  "puts ACTIVATE!"

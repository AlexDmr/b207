#____________________________________________________________________________
#____________________________________________________________________________
#____________________________________________________________________________
method B_bouton_he_shan constructor {size txt {cmd ""}} {
 set this(fond) [B_polygone]
   set this(B_txt) [B_texte $size $txt]; set box [$this(B_txt) Boite_noeud]
    $this(B_txt) Mode_texture 1; $this(B_txt) Couleur_fond 0 0 0 1
   B_configure $this(fond) -Ajouter_contour [ProcRect [expr [$box BG_X] - 5] [expr [$box BG_Y] - 5] [expr [$box HD_X] + 5] [expr [$box HD_Y] + 5] ] \
                           -Ajouter_fils $this(B_txt)
 set this(bt) ${objName}_BT
 B_bouton_perso $this(bt) $this(B_txt) \
                  "$this(B_txt) Couleur_texte 0   1 0   1;" \
			      "$this(B_txt) Couleur_texte 1   1 1   1;" \
			      "$this(B_txt) Couleur_texte 0.5 1 0.5 1; $this(B_txt) Couleur_fond 0 0.7 0 1" \
				  "$this(B_txt) Couleur_texte 0   1 0   1; $this(B_txt) Couleur_fond 0 0   0 1"

 if {$cmd != ""} {this Subscribe $cmd}
 
 return $objName
}

#____________________________________________________________________________
Generate_accessors B_bouton_he_shan [list bt]

#____________________________________________________________________________
method B_bouton_he_shan Subscribe {cmd} {
 $this(bt) Subscribe_to_activate $objName $cmd
}

#____________________________________________________________________________
method B_bouton_he_shan Accrocher_a {noeud} {
 $noeud Ajouter_fils_au_debut $this(fond)
}

#____________________________________________________________________________
method B_bouton_he_shan Position {x y} {
 $this(fond) Origine $x $y
}

#____________________________________________________________________________
method B_bouton_he_shan Centrer {} {
 set box [$this(fond) Boite_noeud]
 $this(fond) Translation_interne -[$box Cx] -[$box Cy]
}
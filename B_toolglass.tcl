 method B_toolglass constructor {cmd} {
 set this(cmd) $cmd
 set this(img_reflet) [B_image [get_B207_files_root]reflet_transparent.png]

 set this(n_meta)  [B_noeud]
 set this(poly_tg) [B_polygone]
 $this(n_meta) Ajouter_fils $this(poly_tg)
 $this(n_meta) Position_des_fils_changeable 0
 B_configure $this(poly_tg) -Translucidite 1 -Couleur 0 1 0.4 0.5 -Ajouter_contour [ProcOvale 0 0 85 85 60] \
						    -Info_texture  NULL \
						    -Info_texture2 [$this(img_reflet) Info_texture] \
										-Combinaison_textures [GLenum2UI [GL_interpolate_ARB]] [GLenum2UI [GL_add]] \
										-Combinaison_textures_operande2 [GLenum2UI [GL_src_alpha]] \
										-Combinaison_textures_source_arg2 [GLenum2UI [GL_texture]] \
										-Mode_texture 1        \
										-Mode_texture_fils 1   \
										-Mode_texture2 1	   \
										-Nb_pixels_par_unite 1 \
										-Forcer_pre_rendu_noeud 1


 set this(poly_bord) [B_polygone]
   $this(n_meta) Ajouter_fils $this(poly_bord)
   
 B_configure $this(poly_bord) -Ajouter_contour [ProcOvale 0 0 120 120 60] \
                       -Couleur 0.6 0.7 0.8 1 \
					   -Difference $this(poly_tg) \
					   -Info_texture  NULL \
					   -Info_texture2 [$this(img_reflet) Info_texture] \
										-Combinaison_textures [GLenum2UI [GL_interpolate_ARB]] [GLenum2UI [GL_add]] \
										-Combinaison_textures_operande2 [GLenum2UI [GL_src_alpha]] \
										-Combinaison_textures_source_arg2 [GLenum2UI [GL_texture]] \
										-Mode_texture 1        \
										-Mode_texture_fils 1   \
										-Mode_texture2 1	   \
										-Nb_pixels_par_unite 1 \
										-Forcer_pre_rendu_noeud 1
# Configuration of contacts and toolglass callback
 B_contact ctc_$this(n_meta) "$this(n_meta) 0" -add "$this(poly_bord) 1"
 C_B_Toolglass $this(poly_tg) "if {\[catch {$objName Execute_command \$infos_origine \$infos} err]} {puts \"ERROR\n\$err\"}"

}

method B_toolglass Execute_command {infos_origine infos} {
	eval $this(cmd)
}
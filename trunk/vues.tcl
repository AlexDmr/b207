set vue_normale [$noeud_bigre]
set vue_loin    [$noeud_bigre]; $vue_loin Etirement 0.5 0.5
set vue_pres    [$noeud_bigre]; $vue_pres Etirement 2   2

proc zoom_sur_fen {fen rep_cible ms} {
 global noeud_bigre
 global transfo_dyn_rep
 global n_i_mere
 upvar $fen f

 set n_tmp [$noeud_bigre]
 $n_tmp maj $rep_cible
 $n_tmp Inverser_coordonnees

 set transfo [$transfo_dyn_rep $ms $fen $n_tmp]
 $transfo Demarrer
 $n_i_mere Ajouter_deformation_dynamique $transfo
}

proc transfo_fen {dep arr ms} {
 global transfo_dyn_rep
 global n_i_mere

 set transfo_fen [$transfo_dyn_rep $ms $dep $arr]
 $transfo_fen Demarrer
 $n_i_mere Ajouter_deformation_dynamique $transfo_fen
}

proc pixeliser {fen nb_pix ms ms_pas} {
 set dep [clock clicks]
 pixelisation $fen $dep [$fen Nb_pixels_par_unite] $ms $nb_pix $ms_pas
}

proc pixelisation {fen dep nb_dep duree nb_fin ms_pas} {
 set  t "[clock clicks].0"
 set dt [expr ($t-$dep)/$duree]
 if {[expr $dt>1]} {return}
 $fen Nb_pixels_par_unite [expr $nb_dep+$dt*($nb_fin-$nb_dep)]

 after $ms_pas pixelisation $fen $dep $nb_dep $duree $nb_fin $ms_pas
}



set les_fichiers [glob "Slide*"]
proc Transfo {prefixe suffixe L_fichiers new_prefixe new_suffixe} {
 foreach f $L_fichiers {
  regexp "^${prefixe}(\[0-9\]+)$suffixe\$" $f dum num
  set n_num [format %03i $num]
  puts $n_num
  file rename $f "$new_prefixe${n_num}$new_suffixe"
 }
}

set les_fichiers [glob Joelle*]
foreach f $les_fichiers {
 file rename $f "$f.BMP"
}

Le cadre de référence structure l'ingénieurie des sys interactifsplastiques, la première question que se pose le dévellopeur est "quels sont les contextes d'usage que je cible?".
Il les modélise, comme c'est classique, ainsi que le domaine applicatif, comme c'est clasique.
L'ingénieurie est alors structuré selon les 4 étapes traditionnelles rappellées ce matin par Olfa, à savoir;
CT
IA
IC
IF

Mutations algo : 
  william latham
  steven tood
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

Le cadre de r�f�rence structure l'ing�nieurie des sys interactifsplastiques, la premi�re question que se pose le d�vellopeur est "quels sont les contextes d'usage que je cible?".
Il les mod�lise, comme c'est classique, ainsi que le domaine applicatif, comme c'est clasique.
L'ing�nieurie est alors structur� selon les 4 �tapes traditionnelles rappell�es ce matin par Olfa, � savoir;
CT
IA
IC
IF

Mutations algo : 
  william latham
  steven tood
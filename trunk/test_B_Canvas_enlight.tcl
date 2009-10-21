proc Enlight {BC CSS_expr} {
 $BC Stop_Enlight
 
 set L_nodes  [CSS++ cr $CSS_expr]
 set L_L_elmt [list]
 foreach n $L_nodes {
   lappend L_L_elmt [$BC get_nodes_representing $n]
  }
  
 $BC Enlight $L_L_elmt
}

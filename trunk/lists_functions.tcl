# Some list functions

#____________________________________________________________________________________
proc Generate_List_accessor {class L_name suffixe} {
 set cmd "method $class get_$suffixe { } {return \$this($L_name)}"                  ; eval $cmd
 set cmd "method $class set_$suffixe {L} {set this($L_name) \$L}"                   ; eval $cmd
 set cmd "method $class Add_$suffixe {L} {set this($L_name) \[Liste_Union \$this($L_name) \$L\]}"           ; eval $cmd
 set cmd "method $class Sub_$suffixe {L} {return \[Sub_list    this($L_name) \$L\]}"; eval $cmd
 set cmd "method $class Contains_${suffixe} {e} {return \[lsearch \$this($L_name) \$e\]}"; eval $cmd
}

#____________________________________________________________________________________
proc Is_sub_list {L1 L2} {
 foreach e1 $L1 {
   set ok 0
   foreach e2 $L2 {
     if {[string equal $e1 $e2]} {set ok 1; break}
    }
   if {$ok} {set L2 [lremove $L2 $e1]} else {return 0}
  }
 return 1
}

#____________________________________________________________________________________
proc Liste_to_set {L} {
 set rep [list]
 foreach e1 $L {
   set to_be_added 1
   foreach e2 $rep {
     if {[string equal $e1 $e2]} {
       set to_be_added 0
       break
      }
    }
   if {$to_be_added} {
     lappend rep $e1
    }
  }
 return $rep
}

#____________________________________________________________________________________
proc Liste_Union {L1 L2} {
 set rep $L1
 foreach e2 $L2 {
   set to_be_added 1
   foreach e1 $L1 {
     if {[string equal $e1 $e2]} {
       set to_be_added 0
       break
      }
    }
   if {$to_be_added} {
     lappend rep $e2
    }
  }
 return $rep
}

#____________________________________________________________________________________
proc Liste_Intersection {L1 L2} {
 set rep [list]

 foreach e1 $L1 {
   if {[lsearch $L2 $e1]!=-1} {lappend rep $e1}
  }

 return $rep 
}

#____________________________________________________________________________________
proc Invert_list {nom_L} {
 upvar $nom_L L
   set L_tmp $L
   set L {}
 foreach e $L_tmp {
   set L [linsert $L 0 $e]
  }
}

#____________________________________________________________________________________
proc Sub_element {nom_L e} {
 upvar $nom_L L
 set pos [lsearch $L $e]
 if {$pos!=-1} {set L [lreplace $L $pos $pos]
                return 1
               }
 return 0
}
#____________________________________________________________________________________
proc Sub_list {nom_L Le} {
 upvar $nom_L L
 set rep 0
 foreach e $Le {incr rep [Sub_element L $e]}
 return $rep
}

#____________________________________________________________________________________
proc Add_list {nom_L Le {index -1}} {
 upvar $nom_L L
 set rep 0
 if {$index == -1} {
   foreach e $Le {incr rep [Add_element L $e]}
  } else {set pos $index
          foreach e $Le {incr rep [Add_element L $e $pos]
                         incr pos}
         }
 return $rep
}

#____________________________________________________________________________________
proc Add_element {nom_L e {index -1}} {
 upvar $nom_L L
 if {[lsearch $L $e]!=-1} {return 0}

 if {$index==-1} {
   lappend L $e
   return 1}

 set L [linsert $L $index $e]
 return 1
}

#____________________________________________________________________________________
proc Add_element_quick {nom_L e {index -1}} {
 upvar $nom_L L

 if {$index==-1} {
   lappend L $e
   return 1}

 set L [linsert $L $index $e]
 return 1
}

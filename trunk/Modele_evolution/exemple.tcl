source [file join . gmlObject.tcl] 
method mot constructor {valeur defi} { 
	set this(val) $valeur set this(d) $defi return $objName } 
method mot getval {} { 
	return $this(val) } 

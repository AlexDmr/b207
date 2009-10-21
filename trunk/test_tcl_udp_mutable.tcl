package require udp

proc decode_mutable {nb txt} {
 set txt [string range $txt 4 end]
 for {set n 0} {$n<$nb} {incr n} {
   binary scan $txt fffff int x y w h; set pkt [string range $txt 20 end]
   puts "  - {$int $x $y $w $h}"
  }
}

proc udpEventHandler {sock} {
    set pkt [read $sock]; 
    set peer [fconfigure $sock -peer]
    binary scan $pkt i nb; 
	puts "$peer: [string length $pkt] {$nb}";
	if {[catch {decode_mutable $nb $pkt} err]} {puts "ERROR : $err"}
}

proc udp_listen {port} {
    set srv [udp_open $port]
    fconfigure $srv -buffering none -translation binary
    fileevent $srv readable [list ::udpEventHandler $srv]
    puts "Listening on udp port: [fconfigure $srv -myport]"
    return $srv
}

set sock [udp_listen 8910]

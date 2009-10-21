package require udp

proc udpEvent {chan} {
    set data [read $chan]
    set peer [fconfigure $chan -peer]
    puts "$peer [string length $data] '$data'"
    if {[string match "QUIT*" $data]} {
        close $chan
        set ::forever 1
    }
    return
}

set group 224.0.0.80
set port  1500
set s [udp_open $port]
fconfigure $s -buffering none -blocking 0
fconfigure $s -mcastadd $group -remote [list $group $port]
fileevent $s readable [list udpEvent $s]
puts -nonewline $s "hello, world"
set ::forever 0

puts "To be launch BEFORE B207"

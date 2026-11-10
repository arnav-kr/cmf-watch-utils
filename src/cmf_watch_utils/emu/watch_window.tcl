set run [lindex $argv 0]
set stock [expr {[lindex $argv 1] ne "custom"}]
set version [lindex $argv 2]
set frame [file join $run frame.ppm]
set motorLog [file join $run motor.log]
set motorText {Motor: off}
set motorSize 0
set events [open [file join $run input.txt] a]
fconfigure $events -buffering line -translation lf -encoding ascii
set last {}
set touching 0
set motion {}
set motionTimer {}
set pressed 0
set screenInset 16
set showFrame 1
set captureFrame 0
set wheelStart {}
set wheelMoved 0
wm title . [lindex $argv 3]
wm resizable . 0 0
wm attributes . -topmost 1
. configure -background #181818
canvas .panel -width 498 -height 498 -background #181818 -highlightthickness 0
image create photo watch_frame
.panel create image $screenInset $screenInset -image watch_frame -anchor nw
.panel create oval 5 5 493 493 -outline #303030 -width 8 -tags case
.panel create oval 3 3 495 495 -outline #5e5e5e -width 1 -tags case
.panel create oval 12 12 486 486 -outline #050505 -width 8 -tags case
label .status -text $version -background #181818 -foreground white
frame .controls -background #1a1a1a -padx 16 -pady 20
frame .controls.stack -background #1a1a1a
label .controls.label -text "CROWN" -background #1a1a1a -foreground #c4c4c4
canvas .controls.crown -width 72 -height 66 -background #1a1a1a -highlightthickness 0 -cursor hand2
.controls.crown create oval 9 8 63 62 -fill #111111 -outline {}
.controls.crown create oval 9 5 63 59 -fill #2b2b2b -outline #626262 -width 2 -tags crownFace
.controls.crown create oval 15 11 57 53 -outline #222222
.controls.crown create oval 32 28 40 36 -fill #ed642c -outline {}
canvas .controls.wheel -width 72 -height 112 -background #1a1a1a -highlightthickness 0 -cursor hand2
.controls.wheel create oval 14 3 58 39 -fill #2b2b2b -outline #626262
.controls.wheel create oval 14 73 58 109 -fill #2b2b2b -outline #626262
.controls.wheel create rectangle 14 21 58 91 -fill #2b2b2b -outline {}
.controls.wheel create line 14 21 14 91 -fill #626262
.controls.wheel create line 58 21 58 91 -fill #626262
for {set y 29} {$y <= 83} {incr y 6} {
    .controls.wheel create line 21 $y 51 $y -fill #555555 -width 2 -tags ridge
}
.controls.wheel create line 31 19 36 14 41 19 -fill #d6d6d6 -width 2
.controls.wheel create line 31 93 36 98 41 93 -fill #d6d6d6 -width 2
foreach {name caption action} {frame {Hide frame} toggle_frame capture {Save PNG} save_snapshot advanced {Advanced...} open_sensors} {
    button .controls.$name -text $caption -command $action -width 13 -pady 5 -bd 0 \
        -background #2b2b2b -foreground #eeeeee -activebackground #3c3c3c \
        -activeforeground white -highlightbackground #1a1a1a -highlightcolor #b0b0b0
}
pack .controls.label .controls.crown .controls.wheel -in .controls.stack -pady 5
pack .controls.frame -in .controls.stack -fill x -pady {15 4}
checkbutton .controls.captureFrame -text {Include frame} -variable captureFrame \
    -background #1a1a1a -foreground #eeeeee -activebackground #1a1a1a \
    -activeforeground white -selectcolor #2b2b2b
pack .controls.capture .controls.captureFrame -in .controls.stack -fill x -pady 2
pack .controls.advanced -in .controls.stack -fill x -pady 4
pack .controls.stack -expand 1
grid .panel .controls -sticky nsew
grid configure .panel -padx 12 -pady 12
grid .status -columnspan 2 -sticky ew -ipady 5
raise .
after 1500 {wm attributes . -topmost 0}

proc send_event {event} {
    global events
    puts $events $event
}

proc toggle_frame {} {
    global showFrame
    set showFrame [expr {!$showFrame}]
    .panel itemconfigure case -state [expr {$showFrame ? "normal" : "hidden"}]
    .controls.frame configure -text [expr {$showFrame ? "Hide frame" : "Show frame"}]
}

proc save_snapshot {} {
    global captureFrame run screenInset
    if {[image width watch_frame] != 466 || [image height watch_frame] != 466} {return}
    set path [tk_getSaveFile -parent . -initialdir $run -initialfile watch-pro-2.png \
        -defaultextension .png -filetypes {{{PNG image} {.png}}}]
    if {$path eq {}} {return}
    set inset [expr {$captureFrame ? $screenInset : 0}]
    set size [expr {466 + 2 * $inset}]
    set snapshot [image create photo -width $size -height $size]
    try {
        if {$captureFrame} {
            foreach {radius color} {247 #525252 245 #303030 237 #050505} {
                for {set y [expr {249 - $radius}]} {$y < 249 + $radius} {incr y} {
                    set x [expr {int(sqrt($radius * $radius - ($y + 0.5 - 249) ** 2))}]
                    $snapshot put $color -to [expr {249 - $x}] $y [expr {249 + $x}] [expr {$y + 1}]
                }
            }
        }
        for {set y 0} {$y < 466} {incr y} {
            set half [expr {sqrt(233 * 233 - ($y - 232.5) ** 2)}]
            set left [expr {int(ceil(232.5 - $half))}]
            set right [expr {int(floor(232.5 + $half)) + 1}]
            $snapshot copy watch_frame -from $left $y $right [expr {$y + 1}] \
                -to [expr {$left + $inset}] [expr {$y + $inset}]
        }
        $snapshot write $path -format png
    } on error {message} {
        tk_messageBox -parent . -icon error -message $message
    } finally {
        image delete $snapshot
    }
}

proc wheel_drag {y} {
    global wheelStart wheelMoved
    if {$wheelStart eq {}} {return}
    set steps [expr {max(-4, min(4, int(($y - $wheelStart) / 12.0)))}]
    if {$steps != 0} {
        send_event "r [expr {80 * $steps}]"
        incr wheelStart [expr {12 * $steps}]
        set wheelMoved 1
    }
}

proc wheel_end {y} {
    global wheelStart wheelMoved
    if {$wheelStart eq {}} {return}
    if {!$wheelMoved} {
        send_event "r [expr {$y < 56 ? -80 : 80}]"
    }
    set wheelStart {}
}

proc flush_motion {} {
    global motion motionTimer
    if {$motionTimer ne {}} {
        after cancel $motionTimer
        set motionTimer {}
    }
    if {$motion ne {}} {
        send_event $motion
        set motion {}
    }
}

proc touch {state x y} {
    global touching motion motionTimer screenInset
    incr x -$screenInset
    incr y -$screenInset
    set dx [expr {$x - 232.5}]
    set dy [expr {$y - 232.5}]
    set radius [expr {hypot($dx, $dy)}]
    if {$state eq "start"} {
        if {$radius > 233} {return}
        focus .panel
        set touching 1
    } elseif {!$touching} {
        return
    } elseif {$state eq "end"} {
        flush_motion
        set touching 0
    }
    if {$radius > 233} {
        # An in-progress swipe can leave the circle. End it at the rim.
        set x [expr {round(232.5 + 232 * $dx / $radius)}]
        set y [expr {round(232.5 + 232 * $dy / $radius)}]
    }
    if {$state eq "move"} {
        # Keep the latest position instead of queuing every Windows mouse event.
        set motion "t 1 $x $y"
        if {$motionTimer eq {}} {
            set motionTimer [after 10 flush_motion]
        }
    } else {
        send_event "t $touching $x $y"
    }
}

set accelX 0
set accelY 0
set accelZ 1000
set battery 4000
set charging 0
set virtualHealth 0
set heartRate 72
set spo2 98
set virtualGps 0
set gpsLatitude 0
set gpsLongitude 0
set brightness 100

proc apply_sensors {} {
    global accelX accelY accelZ battery charging virtualHealth heartRate spo2 stock
    global virtualGps gpsLatitude gpsLongitude
    set latitude 0
    set longitude 0
    if {$virtualGps} {
        if {![string is double -strict $gpsLatitude] || ![string is double -strict $gpsLongitude] ||
            [catch {expr {round(double($gpsLatitude) * 1000000)}} latitude] ||
            [catch {expr {round(double($gpsLongitude) * 1000000)}} longitude] ||
            abs($latitude) > 90000000 || abs($longitude) > 180000000} {
            tk_messageBox -parent .advanced -icon error -message "Enter a latitude from -90 to 90 and a longitude from -180 to 180."
            return
        }
    }
    send_event "a $accelX $accelY $accelZ"
    send_event "p $battery $charging"
    if {$stock} {
        send_event "h $virtualHealth $heartRate $spo2"
    }
    send_event "g $virtualGps $latitude $longitude"
}

proc open_sensors {} {
    global stock
    if {[winfo exists .advanced]} {
        raise .advanced
        return
    }
    toplevel .advanced
    wm title .advanced "Advanced - Virtual sensors"
    wm transient .advanced .
    wm resizable .advanced 0 0
    scale .advanced.brightness -orient horizontal -length 280 -from 0 -to 100 \
        -label "Brightness (%)" -variable brightness -command {send_brightness}
    pack .advanced.brightness -fill x -padx 12 -pady 4
    foreach axis {X Y Z} {
        scale .advanced.accel$axis -orient horizontal -length 280 -from -2000 -to 2000 \
            -resolution 10 -label "$axis acceleration (mg)" -variable accel$axis
        pack .advanced.accel$axis -fill x -padx 12 -pady 4
    }
    scale .advanced.battery -orient horizontal -length 280 -from 3300 -to 4400 \
        -resolution 10 -label "Battery voltage (mV)" -variable battery
    checkbutton .advanced.charger -text "Charger connected" -variable charging
    pack .advanced.battery .advanced.charger -padx 12 -pady 5
    if {$stock} {
        labelframe .advanced.health -text "Simulated optical results"
        checkbutton .advanced.health.enabled -text "Enable simulated readings" -variable virtualHealth
        scale .advanced.health.hr -orient horizontal -length 280 -from 30 -to 220 \
            -label "Heart rate (bpm)" -variable heartRate
        scale .advanced.health.spo2 -orient horizontal -length 280 -from 70 -to 100 \
            -label "Blood oxygen (%)" -variable spo2
        pack .advanced.health.enabled .advanced.health.hr .advanced.health.spo2 -fill x -padx 4
        pack .advanced.health -padx 12 -pady 5
    }
    labelframe .advanced.gps -text "GPS"
    checkbutton .advanced.gps.enabled -text "Simulate a GPS fix" -variable virtualGps
    grid .advanced.gps.enabled -columnspan 2 -sticky w
    foreach {axis variable} {Latitude gpsLatitude Longitude gpsLongitude} {
        label .advanced.gps.label$axis -text $axis
        entry .advanced.gps.value$axis -textvariable $variable -width 15
        grid .advanced.gps.label$axis .advanced.gps.value$axis -sticky w -padx 4 -pady 2
    }
    pack .advanced.gps -fill x -padx 12 -pady 5
    frame .advanced.buttons
    button .advanced.buttons.apply -text Apply -command apply_sensors
    button .advanced.buttons.close -text Close -command {destroy .advanced}
    pack .advanced.buttons.apply .advanced.buttons.close -side left -padx 4
    pack .advanced.buttons -padx 12 -pady 5
}

proc send_brightness {value} {
    send_event "b $value"
}

proc crown {state} {
    global pressed
    if {$state != $pressed} {
        set pressed $state
        .controls.crown itemconfigure crownFace -fill [expr {$state ? "#434343" : "#2b2b2b"}]
        send_event "c $state"
    }
}

bind .panel <ButtonPress-1> {touch start %x %y}
bind .panel <B1-Motion> {touch move %x %y}
bind .panel <ButtonRelease-1> {touch end %x %y}
bind .controls.crown <ButtonPress-1> {focus .panel; crown 1; break}
bind .controls.crown <ButtonRelease-1> {crown 0; break}
bind .controls.wheel <ButtonPress-1> {focus .panel; set wheelStart %y; set wheelMoved 0; break}
bind .controls.wheel <B1-Motion> {wheel_drag %y; break}
bind .controls.wheel <ButtonRelease-1> {wheel_end %y; break}
bind . <KeyPress-space> {crown 1; break}
bind . <KeyRelease-space> {crown 0; break}
bind . <KeyPress-Up> {send_event "r -80"; break}
bind . <KeyPress-Down> {send_event "r 80"; break}
bind . <MouseWheel> {
    if {%D != 0} {send_event "r [expr {%D > 0 ? -80 : 80}]"}
    break
}
bind . <Button-4> {send_event "r -80"; break}
bind . <Button-5> {send_event "r 80"; break}
bind . <FocusOut> {crown 0}
focus .panel

proc motor_status {} {
    global motorLog motorText motorSize version
    if {[catch {set size [file size $motorLog]}] || $size == $motorSize} {return}
    set motorSize $size
    if {[catch {
        set stream [open $motorLog r]
        try {
            if {$size > 64} {seek $stream -64 end}
            set lines [split [string trim [read $stream]] "\n"]
        } finally {close $stream}
        lassign [lindex $lines end] ms duty freq
        if {$duty eq {} || $duty == 0} {
            set text {Motor: off}
        } else {
            set text [format {Motor: %d%% %d Hz} [expr {$duty / 10}] $freq]
        }
    }]} {return}
    if {$text ne $motorText} {
        set motorText $text
        .status configure -text "$version    $text"
    }
}

proc refresh {} {
    global frame run last
    if {![catch {
        set stream [open $frame rb]
        try {set data [read $stream]} finally {close $stream}
        if {$data ne $last} {
            watch_frame configure -data $data -format ppm
            set last $data
        }
    }]} {}
    if {[file exists [file join $run stopped]]} {
        .panel create rectangle 72 220 426 278 -fill #181818 -outline #5e5e5e
        .panel create text 249 249 -text "QEMU stopped\n[file tail $run]/qemu.log" -fill white
        return
    }
    motor_status
    after 16 refresh
}
refresh

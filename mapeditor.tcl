package require Tk

proc sdlLoopAsync {} {
	sdlLoop
	after 100 [list sdlLoopAsync]
}

button .hello -text "Hello, World!" -command { exit }
pack .hello

sdlLoopAsync
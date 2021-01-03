source sdlcodes.tcl

proc empty {} {
	puts "Entity has no dialogue."
}

proc setup {} {
	puts "Setup ran."
}


proc testEntSpawn {} {
	return {testEntBehav empty 0 0 69 420 42 17 10 {lol asdf lol}}
}

proc testEntBehav {diag x y dx dy xsub ysub health stateList} {
	puts "running!"
	global cameraX
	global cameraY

	image 8 $x $y

	set cameraX $x
	set cameraY $y

	if [key $::SDL_SCANCODE_W] {
		incr y -1
	}
	if [key $::SDL_SCANCODE_S] {
		incr y
	}
	if [key $::SDL_SCANCODE_A] {
		incr x -1
	}
	if [key $::SDL_SCANCODE_D] {
		incr x
	}

	return [list testEntBehav $diag $x $y $dx $dy $xsub $ysub $health $stateList]
}

proc loop {} {
	if [key $::SDL_SCANCODE_SPACE] {
		puts "Space pressed!"
	}
}
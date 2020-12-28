source sdlcodes.tcl

proc setup {} {
	puts "Setup ran."
}

set x -10
set y -10
set cameraX 10

proc testEntSpawn {} {
	return {testEntBehav 69 420 42 17 10 {lol asdf lol}}
}

proc testEntBehav {x y dx dy xsub ysub health stateList} {
	puts "running!"
	image 8 x y
	if [key $::SDL_SCANCODE_W] incr y -1
	if [key $::SDL_SCANCODE_S] incr y
	if [key $::SDL_SCANCODE_A] incr x -1
	if [key $::SDL_SCANCODE_D] incr x

	return {$x $y $dx $dy $xsub $ysub $health $stateList}
}

proc loop {} {
	global cameraX
	image 8 x x
	if [key $::SDL_SCANCODE_SPACE] {
		puts "Space pressed!"
	}
}
source sdlcodes.tcl

proc setup {} {
	puts "Setup ran."
}

set x -10
set y -10

puts "$SDL_SCANCODE_SPACE"

proc loop {} {
	global SDL_SCANCODE_SPACE

	image 8 x x
	if [key $SDL_SCANCODE_SPACE] {
		puts "Space pressed!"
	}
}
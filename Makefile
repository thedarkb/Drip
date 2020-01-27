east:
	gcc main.c -O2 -lSDL2 -lSDL2_image -o drip

noscroll:
	gcc main.c -O2 -DNOSCROLL -lSDL2 -lSDL2_image -o drip

web:
	emcc main.c -O3 -DNOSCROLL -DWEB -s ALLOW_MEMORY_GROWTH=1 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' --preload-file sheet.png -o drip.html

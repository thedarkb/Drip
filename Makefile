east:
	gcc main.c -O2 -lSDL2 -lSDL2_image -o drip

noscroll:
	gcc main.c -O2 -DNOSCROLL -lSDL2 -lSDL2_image -o drip

web:
	mkdir web
	cp index.html web/
	emcc main.c -O3 -DNOSCROLL -DWEB -s MODULARIZE=1 -s ALLOW_MEMORY_GROWTH=1 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' --embed-file sheet.png --preload-file font.png -o web/drip.js

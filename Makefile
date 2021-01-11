east:
	gcc main.c -O2 -lSDL2 -lSDL2_image -ltcl -o drip
fast:
	gcc main.c -O0 -lSDL2 -lSDL2_image -ltcl -g -o drip
map:
	gcc main.c -O0 -DDEV -lSDL2 -lSDL2_image -ltcl -g -o drip
web:
	mkdir web
	cp index.html web/
	emcc main.c -O3 -DNOSCROLL -DWEB -s MODULARIZE=1 -s ALLOW_MEMORY_GROWTH=1 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' --embed-file sheet.png --preload-file font.png -o web/drip.js
editor:
	gcc mapeditor.c -g -lSDL2 -lSDL2_image -ltcl -ltk8.6 -ltkstub8.6 -lsqlite3 -o editor

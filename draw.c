void imageNoTrack(int index, int x, int y, int w, int h) { //Copies an image from the hardware buffer to the screen.
	SDL_Rect scaler = {x,y+HUDHEIGHT,w,h}; //Accounts for HUD but NOT the camera.
	SDL_Rect sprite={(index % (sheetX/tileSize))*tileSize, (index / (sheetX/tileSize))*tileSize, tileSize, tileSize};
	SDL_RenderCopy(r, sheet, &sprite, &scaler);
}

void image(int index, int x, int y, int w, int h) { //Copies an image from the hardware buffer to the screen.
	SDL_Rect scaler = {x+CX,y+CY,w,h}; //Accounts for HUD and camera.
	SDL_Rect sprite={(index % (sheetX/tileSize))*tileSize, (index / (sheetX/tileSize))*tileSize, tileSize, tileSize};
	SDL_RenderCopy(r, sheet, &sprite, &scaler);
}

void tintedImage(SDL_Texture* imgIn, int x, int y, int w, int h, uint32_t colour) { //Copies an image from the hardware buffer to the screen.
	union htmlDecode {
		uint32_t htmlCode;
		unsigned char rgb[3];
	} htmlDecode;
	htmlDecode.htmlCode=colour;
	if(SDL_SetTextureColorMod(imgIn, htmlDecode.rgb[2], htmlDecode.rgb[1], htmlDecode.rgb[0])) printf("Colour Mod not supported.\n");
	SDL_Rect scaler = {x+CX,y+CY,w,h}; //Accounts for HUD and camera.
	SDL_RenderCopy(r, imgIn, NULL, &scaler);
	SDL_SetTextureColorMod(imgIn, 255, 255, 255);
}

/*
void timage(SDL_Texture* imgIn, int x, int y, int w, int h) {
	SDL_Rect scaler = {x,y,w,h};
	SDL_RenderCopy(r, imgIn, NULL, &scaler);
}*/

/*void simage(SDL_Surface* imgIn, int x, int y, int w, int h) { //Ditto above, but from software buffer.
	SDL_Rect scaler = {x,y+HUDHEIGHT,w,h};
	SDL_Texture* imgOut = SDL_CreateTextureFromSurface(r, imgIn);
	SDL_RenderCopy(r, imgOut, NULL, &scaler);
	SDL_DestroyTexture(imgOut);
}*/

void hudDraw(SDL_Texture* imgIn, int x, int y, int w, int h) { //For drawing to the HUD
	SDL_Rect scaler = {x,y,w,h};
	SDL_RenderCopy(r, imgIn, NULL, &scaler);
}

void bgBlit(SDL_Surface* imgIn, int x, int y, int w, int h) { //For drawing to the background layer.
	SDL_Rect scaler = {x,y,w,h};
	//SDL_BlitSurface(imgIn, NULL, bgLayer, &scaler);
}

void bgDraw (view* in) { //Takes a tileset and blits it to the background layer.
	for (int x=0; x<SW; x++) {
		for (int y=0; y<SH; y++) {
			bgBlit(swtileset[in->screen[y][x]],x*TS,y*TS,TS,TS);
		}
	}
}

void drawRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, uint32_t colour) { //Draws filled rectangle
	union htmlDecode {
		uint32_t htmlCode;
		unsigned char rgb[3];
	} htmlDecode;
	htmlDecode.htmlCode=colour;
	SDL_SetRenderDrawColor(r, htmlDecode.rgb[2], htmlDecode.rgb[1], htmlDecode.rgb[0], 255);
	SDL_Rect scaler={x,y,w,h};
	SDL_RenderFillRect(r, &scaler);
}

void drawRectAlpha(unsigned int x, unsigned int y, unsigned int w, unsigned int h, uint32_t colour, uint8_t alpha) { //Draws filled rectangle
	union htmlDecode {
		uint32_t htmlCode;
		unsigned char rgb[3];
	} htmlDecode;
	htmlDecode.htmlCode=colour;
	SDL_SetRenderDrawColor(r, htmlDecode.rgb[2], htmlDecode.rgb[1], htmlDecode.rgb[0], alpha);
	SDL_Rect scaler={x,y,w,h};
	SDL_RenderFillRect(r, &scaler);
}

void drawRectAlphaTrack(unsigned int x, unsigned int y, unsigned int w, unsigned int h, uint32_t colour, uint8_t alpha) { //Draws filled rectangle
	union htmlDecode {
		uint32_t htmlCode;
		unsigned char rgb[3];
	} htmlDecode;
	htmlDecode.htmlCode=colour;
	SDL_SetRenderDrawColor(r, htmlDecode.rgb[2], htmlDecode.rgb[1], htmlDecode.rgb[0], alpha);
	SDL_Rect scaler = {x+CX,y+CY,w,h};
	SDL_RenderFillRect(r, &scaler);
}

void drawRectTrack(unsigned int x, unsigned int y, unsigned int w, unsigned int h, uint32_t colour) { //Draws filled rectangle
	union htmlDecode {
		uint32_t htmlCode;
		unsigned char rgb[3];
	} htmlDecode;
	htmlDecode.htmlCode=colour;
	SDL_SetRenderDrawColor(r, htmlDecode.rgb[2], htmlDecode.rgb[1], htmlDecode.rgb[0], 255);
	SDL_Rect scaler = {x+CX,y+CY,w,h};
	SDL_RenderFillRect(r, &scaler);
}

void emptyRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, uint32_t colour) { //Draws empty rectangle.
	union htmlDecode {
		uint32_t htmlCode;
		unsigned char rgb[3];
	} htmlDecode;
	htmlDecode.htmlCode=colour;
	SDL_SetRenderDrawColor(r, htmlDecode.rgb[2], htmlDecode.rgb[1], htmlDecode.rgb[0], 255);
	SDL_Rect scaler={x,y,w,h};
	SDL_RenderDrawRect(r, &scaler);
}

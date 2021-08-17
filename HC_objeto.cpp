#include "base.h"

void HC_Objeto::setDest(int x, int y, int w, int h)
{
	dest.x=x;
	dest.y=y;
	dest.w=w;
	dest.h=h;
}

void HC_Objeto::setDest(int x, int y)
{
	dest.x=x;
	dest.y=y;
}

void HC_Objeto::setSource(int x, int y, int w, int h)
{
	src.x=x;
	src.y=y;
	src.w=w;
	src.h=h;
}

void HC_Objeto::setImage(string filename, SDL_Renderer* rende)
{
	SDL_Surface* surf;
	if(IMG_Load(filename.c_str())<0)
		throw runtime_error("Erro em IMG_load()");
	surf=IMG_Load(filename.c_str());
	tex=IMG_LoadTexture(rende, filename.c_str());
}

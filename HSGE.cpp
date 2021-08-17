//Hypercube Simple Graphical Engine
#include "base.h"

HC::~HC()
{
	for (int i=0; i<FontHandles.size(); i++)
		TTF_CloseFont(FontHandles[i]);
	TTF_Quit();
	IMG_Quit();
	SDL_DestroyRenderer(rende);
	SDL_DestroyWindow(janela);
	SDL_Quit();
}

HC::HC()
{
	if (SDL_Init(SDL_INIT_EVERYTHING)<0)
		throw runtime_error("erro ao iniciar SDL");
	TTF_Init();
	IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG);
}

void HC::CriarJanela(int lar, int alt, string nome)
{
	altura=alt;
	largura=lar;
	if(SDL_CreateWindowAndRenderer(largura, altura, 0, &janela, &rende)<0)//criando renderizador e janela
		throw runtime_error("erro ao criar janela");
	SDL_SetWindowTitle(janela, nome.c_str());
	SDL_SetRenderDrawBlendMode(rende, SDL_BLENDMODE_BLEND);
}

vector<int> HC::HaInput()
{
	vector<int>tipo(2);
	tipo[0]=0;
	tipo[1]=0;
	if(!SDL_PollEvent(&eventos))
		return tipo;
	SDL_PollEvent(&eventos);
	if (eventos.type==SDL_QUIT)
		tipo[0]=-1;
	else
	{
		if(eventos.type==SDL_KEYDOWN)
		{
			tipo[0]=1;
			tipo[1]=eventos.key.keysym.sym;
		}
		else if(eventos.type==SDL_KEYUP)//retorna o ascii
			{
				tipo[0]=2;
				tipo[1]=eventos.key.keysym.sym;
			}
			else if(eventos.type==SDL_MOUSEBUTTONDOWN)//esquerdo=1 meio=2 direito=3
				{
					tipo[0]=3;
					tipo[1]=eventos.button.button;
				}
	}
	return tipo;
}

void HC::SetBackground(int r, int g, int b, int a)
{
	SDL_SetRenderDrawColor(rende, r, g, b, a);
	SDL_Rect rect;
	rect.x=rect.y=0;
	rect.w=largura;
	rect.h=altura;
	SDL_RenderFillRect(rende, &rect);
}

void HC::Commit(int fps)
{
	frameCount++;
	int timerFPS=SDL_GetTicks()-lastFrame;
	if(timerFPS<(1000/fps)) //tendo certeza de que tem max 60 fps
		SDL_Delay((1000/fps)-timerFPS);
	SDL_RenderPresent(rende);
	lastFrame=SDL_GetTicks();
}

int HC::CalcFps()
{
	if(lastFrame>=(lastTime+1000))//calculando FPS (cada segundo)
	{
		lastTime=lastFrame;
		fps=frameCount;
		frameCount=0;
	}
	return fps;
}

HC_Objeto HC::CriarObj(string img, int x, int y, int l, int a)
{
	HC_Objeto aux;
	aux.setSource(x, y, l, a);
	aux.setDest(0, 0, l, a);
	aux.setImage(img.c_str(), rende);
	return aux;
}

int HC::AbrirFonte(string arqv, int tam)
{
	TTF_Font* font=TTF_OpenFont(arqv.c_str(), tam);
	FontHandles.push_back(font);
	return FontHandles.size();
}

void HC::Desenhar(HC_Objeto o, int x, int y)
{
	o.setDest(x, y);
	SDL_Rect dest=o.getDest();
	SDL_Rect src=o.getSource();
	SDL_RenderCopyEx(rende, o.getTex(), &src, &dest, o.gir, NULL, SDL_FLIP_NONE);
}

void HC::Desenhar(HC_Objeto o)
{
	SDL_Rect dest=o.getDest();
	SDL_Rect src=o.getSource();
	SDL_RenderCopyEx(rende, o.getTex(), &src, &dest, o.gir, NULL, SDL_FLIP_NONE);
}

void HC::Desenhar(string msg, int f, int x, int y, int r, int g, int b)
{
	SDL_Surface* surf;//cria uma superfície
	SDL_Texture* tex;//cria uma textura
	SDL_Color color;

	color.r=r;
	color.g=g;
	color.b=b;
	color.a=255;

	surf=TTF_RenderText_Solid(FontHandles[f-1], msg.c_str(), color); //renderizar o texto

	SDL_Rect rect;
	rect.x=x;
	rect.y=y;
	rect.w=surf->w;
	rect.h=surf->h;

	tex=SDL_CreateTextureFromSurface(rende, surf);

	SDL_FreeSurface(surf);//liberando memória
	SDL_RenderCopy(rende, tex, NULL, &rect);//colocando tudo no renderizador
	SDL_DestroyTexture(tex);
}

void HC::Desenhar(bool fil, int alt, int lar, int x, int y, int r, int g, int b, int a)//fil=0 para não preencher fil=1 para preencher
{
	SDL_Rect reta;
	reta.x=x;
	reta.y=y;
	reta.h=lar;
	reta.w=alt;

	SDL_SetRenderDrawColor(rende, r, g, b, a);

	if (fil)
		SDL_RenderFillRect(rende, &reta);
	else
		SDL_RenderDrawRect(rende, &reta);
}

void HC::Desenhar(int x, int y, int fx, int fy, int r, int g, int b)
{
	SDL_SetRenderDrawColor(rende, r, g, b, 255);
	SDL_RenderDrawLine(rende, x, y, fx, fy);
}

void HC::Desenhar(int x, int y, int r, int g, int b)
{
	SDL_SetRenderDrawColor(rende, r, g, b, 255);
	SDL_RenderDrawPoint(rende, x, y);
}

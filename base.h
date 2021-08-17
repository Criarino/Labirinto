//Hypercube Simple Graphical Engine header
#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include <vector>
#include <string>


using namespace std;

class HC_Objeto //classe de objetos na tela
{
	private:
	SDL_Rect dest; //onde na tela vai ficar
	SDL_Rect src; //escala da imagem original
	SDL_Texture* tex; //textura
	bool solid; //para colisão - se o objeto tem colisão ou não
	int id;

	public:
	HC_Objeto(){solid=1;gir=0;giro=0;}
	//getters e setters
	SDL_Rect getDest() {return dest;}
	SDL_Rect getSource() {return src;}
	SDL_Texture* getTex() {return tex;}
	bool getSolid() {return solid;}
	int getDX() {return dest.x;}//para saber posição do objeto (getDestX)
	int getDY() {return dest.y;}
	int getDW() {return dest.w;}
	int getDH()	{return dest.h;}
	int getID() {return id;}

	void setDest(int x, int y, int w, int h);
	void setDest(int x, int y);//para mover a tela
	void setSource(int x, int y, int w, int h);//de onde vai começar a carregar a imagem, qual a porção da imagem que vai carregar. Pode cortar partes da imagem
	void setImage(string filename, SDL_Renderer* ren);//carrega uma textura
	void setSolid(bool s){solid=s;}
	void setID(int ID){id=ID;}
	int gir, giro;//para girar os objetos
};

class HC_audio //para audio
{
	public:
	~HC_audio();
	void Adicionar(string filename);
	void Play();

	private:
	SDL_AudioSpec wavSpec;//apenas arquivos wave
	Uint32 wavLength; //TEM que ser esse o tipo
	Uint8* wavBuffer;
	SDL_AudioDeviceID deviceID;
};

class HC //engine em si
{
	public:
	~HC();
	HC();
	SDL_Window* getJanela(){return janela;}
	void CriarJanela(int alt, int lar, string nome);
	vector<int> HaInput();//verificar se há input e qual é esse input. Retorna 0 se não tiver, -1 se o usuário clicou no X da janela, retorna um vector de duas posições nos outros casos: o primeiro elemento do vector é o tipo de evento e o segundo o código do evento (ascii caso seja do teclado)
	void SetBackground(int r, int g, int b, int a);//seta um fundo
	void Commit(int fps);//mostra o renderizador (frame) na tela
	int CalcFps();//calcula o fps, o int retornado é o fps
	HC_Objeto CriarObj(string img, int x, int y, int l, int a);//cria um objeto (caminho de diretório para a textura, posção xy de onde vai começar a copiar essa textura, largura e altura dessa textura)
	void Desenhar(HC_Objeto o); //quando o objeto já tem posição
	void Desenhar(HC_Objeto o, int x, int y);//quando quer atribuir posição ao objeto
	void Desenhar(string msg, int f, int x, int y, int r, int g, int b);//para texto (mensagem, codigo da fonte, posição xy, cor rgb)
	void Desenhar(bool fil, int alt, int lar, int x, int y, int r, int g, int b, int a);//para retangulos(se é para preencher, altura x largura, posição xy, cor)
	void Desenhar(int x, int y, int fx, int fy, int r, int g, int b);//Para linhas(ponto xy inicial, ponto xy final,...)
	void Desenhar(int x, int y, int r, int g, int b);//Para pontos
	int AbrirFonte(string arqv, int tam);//abre uma fonte de texto. Retorna o código para essa fonte (diretório do arquivo ttf, tamanho)

	private:
	int altura;//tamanho da janela
	int largura;
	SDL_Renderer* rende;//renderizador
	SDL_Window* janela;//janela
	vector<TTF_Font*>FontHandles;//fontes abertas
	SDL_Event eventos;//fila de eventos
	int frameCount=0, timerFPS, lastFrame=0, fps, lastTime=0;//para calcular e limitar fps
};

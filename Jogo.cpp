//g++ Jogo.cpp HSGE.cpp HC_objeto.cpp HC_audio.cpp -o jog -lSDL2 -lSDL2_ttf -lSDL2_image
//Jogo simples feito para a disciplina de tópicos I da UFVJM
//Para jogar é necessário compilar o jogo, para isso precisa da biblioteca SDL2. Se usar o compilador G++, use a linha de comando acima. Provavelmente não funciona no windows (Windows é uma bosta)
#include <fstream>
#include <iostream>
#include "base.h"

void input();
void render();
void Desenharmap();
void Carregarmap(string file);
void Conferirclique(bool la);
void Rolar(int la, int y, double mult);

HC Engine;
bool rodando=1;
vector<HC_Objeto> formas(10);
vector<HC_Objeto> lmap;
HC_Objeto pt1, pt2;
//em ordem (linha abaixo): handles das fontes (3), tamanho das peças, level (fase) atual, flag e auxiliar usadas na animação, flag e aux usadas na transição entre lvs, contador de movimentos feitos pelo jogador:
int sans, sans2, sans3, tilesz, nv=1, tr=0, tr2=0, aux2=0, aux=0, mov=0;
vector<HC_Objeto>muda;


int main()
{
	Engine.CriarJanela(1280, 680, "Lorem Ipsum");
	/*HC_audio mus;//para música. Por motivos de direitos autorais eu não posso compartilhar essa música aqui, mas você pode facilmente adicionar a sua:
	mus.Adicionar("Through the Sky-Light wing OST.wav");//apenas coloque o arquivo .wav na mesma pasta do jogo, e o nome aqui
	mus.Play();*///então descomente estas linhas
	pt1=Engine.CriarObj("pt.png", 0, 0, 100, 100);
	pt1.setDest(0, 0, 50, 50);
	pt2=Engine.CriarObj("pt2.png", 0, 0, 100, 100);
	pt2.setDest(0, 0, 50, 50);
	for(int i=0; i<10; i++)
		formas[i]=Engine.CriarObj(to_string(i+1)+".png", 0, 0, 100, 100);//informações dos objetos formas
	tilesz=70;
	sans=Engine.AbrirFonte("Sans.ttf", 10);//para fps
	sans2=Engine.AbrirFonte("Sans.ttf", 30);//para contador de movimentos
	sans3=Engine.AbrirFonte("Sans.ttf", 20);//para mensagens
	Carregarmap("level1.txt");

	while(rodando)
	{
		Engine.SetBackground(255, 255, 255, 255);
		switch(nv)
		{
			case 1://nv 1
				Engine.Desenhar(true, 245, 245, 520, 220, 255, 0, 0, 255);//desenhando caixas ao redor do labirinto
				Engine.Desenhar(true, 230, 230, 527, 227, 255, 255, 255, 255);
				Engine.Desenhar(pt1, 760, 390);//desenhando pontos inicial e final
				Engine.Desenhar(pt2, 470, 245);
				Engine.Desenhar(true, 245, 300, 900, 120, 255, 0, 0, 255);//fazendo janela de pontos
				Engine.Desenhar(true, 230, 285, 907, 127, 200, 192, 255, 255);
				Engine.Desenhar("Movimentos: "+to_string(mov), sans2, 915, 260, 0, 0, 0);//mensagens
				Engine.Desenhar("Encontre o caminho do ponto preto ao ponto vermelho", sans3, 350, 180, 200, 0, 0);
				Engine.Desenhar("Clique nos caminhos para girar->", sans3, 200-aux2, 300, 0, 0, 0);
				if(tr2==0)
					aux2++;
				if(tr2==0&&aux2==50)
				{
					tr2=1;
					aux2--;
				}
				if(tr2==1&&aux2<50)
					aux2--;
				if(tr2==1&&aux2==0)
					tr2=0;
				break;

			case 2://nv2
				if(lmap.size()==0)
				{
					mov=0;
					tilesz=52;
					Carregarmap("level2.txt");
					pt1.setDest(0, 0, 30, 30);
					pt2.setDest(0, 0, 30, 30);
				}
				Engine.Desenhar(pt2, 50, 245);
				Engine.Desenhar(pt1, 50, 300);
				Engine.Desenhar(true, 290, 630, 485, 25, 255, 0, 0, 255);//desenhando caixas ao redor do labirinto
				Engine.Desenhar(true, 275, 615, 492, 32, 255, 255, 255, 255);
				Engine.Desenhar(615, 300, 645, 300, 0, 255, 0);
				Engine.Desenhar(pt2, 560, 650);
				Engine.Desenhar(pt1, 610, 0);
				Engine.Desenhar(true, 245, 300, 900, 120, 255, 0, 0, 255);//fazendo janela de pontos
				Engine.Desenhar(true, 230, 285, 907, 127, 200, 192, 255, 255);
				Engine.Desenhar("Movimentos: "+to_string(mov), sans2, 915, 260, 0, 0, 0);//mensagens
				break;
			case 3://nv3
				if(lmap.size()==0)
				{
					mov=0;
					tilesz=40;
					Carregarmap("level3.txt");
					pt1.setDest(0, 0, 20, 20);
					pt2.setDest(0, 0, 20, 20);
				}
				Engine.Desenhar(true, 1010, 650, 40, 7, 255, 0, 0, 255);//desenhando caixas ao redor do labirinto
				Engine.Desenhar(true, 1000, 640, 47, 14, 255, 255, 255, 255);
				Engine.Desenhar(pt2, 1050, 625);
				Engine.Desenhar(pt1, 25, tilesz*2+25);
				Engine.Desenhar(true, 230, 300, 1050, 120, 255, 0, 0, 255);//fazendo janela de pontos
				Engine.Desenhar(true, 215, 285, 1057, 127, 200, 192, 255, 255);
				Engine.Desenhar("Movimentos: "+to_string(mov), sans2, 1065, 260, 0, 0, 0);//mensagens
		}//tinha maneiras melhores de fazer isso
		render();
    	input();
    	//update();
	}
	return 0;
}

void input()
{
	vector<int>asc;
	do
	{
		asc=Engine.HaInput();
		switch(asc[0])//qual o tipo de evento
		{
			case -1://caso seja fechar
				rodando=0;
				break;
			case 1://caso seja botao (teclado) pressionado. asc[1] contém o código do botao pressionado
				if(asc[1]==13)//enter
				{
					tr2=-1;
					tr=1;
					aux=0;
				}
				break;
			case 2://caso seja botao(teclado) solto
				break;
			case 3://caso seja botao (mouse) pressionado
				if (asc[1]==1)//botão esquerdo
				{
					Conferirclique(true);
				}
				if(asc[1]==3)//botao direito
				{
					Conferirclique(false);
				}
				break;
		}
	}while(asc[0]!=0);
}

void render()
{
	Engine.Desenhar("FPS:"+to_string(Engine.CalcFps()), sans, 3, 3, 255, 0, 0);
	Desenharmap();
	if(tr!=0)//fadein/out
	{
		if(aux>=255&&tr==1)
		{
			aux--;
			tr=2;
			nv++;
			lmap.clear();
		}
		if(aux<255&&tr==1)
			aux++;
		if(aux<255&&tr==2)
			aux--;
		if(aux==0&&tr==2)
			tr=0;
		Engine.Desenhar(true, 1280, 780, 0, 0, 126, 192, 238, aux);
		Engine.Desenhar("Tente reduzir a quantidade de movimentos!", sans2, 1280/2-260, 680/2, 0, 0, 0);
	}
	Engine.Commit(60);
}

void Carregarmap(string file)//carregando mapa das imagens
{
	int current, mx, my, mw, mh, aux=0;
	ifstream in;
	in.open(file);
	if(!in.is_open())
	{
		cout<<"erro ao abrir map"<<endl;
		throw runtime_error("erro ao abrir map");
	}
	in>>mw;//tira as informações iniciais do tilemap
	in>>mh;
	in>>mx;
	in>>my;
	for(int i=0; i<mh; i++)
	{
		for(int j=0; j<mw; j++)
		{
			if(in.eof())
			{
				cout<<"chegou ao fim do arquivo quando não deveria"<<endl;
				throw runtime_error("fim de arquivo");
			}
			in>>current;
			if(current!=0)
			{
				if(((i%5)==0) && i!=1 && i!=0 && (j==0))
					aux=aux+20;
				formas[current-1].setDest((j*tilesz)+mx, (i*tilesz)+my+aux, tilesz, tilesz);
				formas[current-1].setID(current);
				lmap.push_back(formas[current-1]);
			}
		}
	}
	in.close();
}

void Desenharmap()//desenhando os objetos
{
	for (int i=0; i<lmap.size(); i++)
	{
		if (lmap[i].giro==1)//para fazer os objetos rodarem
			lmap[i].gir+=2;
		else if (lmap[i].giro==2)
				lmap[i].gir-=2;
		Engine.Desenhar(lmap[i]);
		if(lmap[i].gir>=360||lmap[i].gir<=-360)//para resetar o angulo quando passar de 360
			lmap[i].gir=0;
		if ((lmap[i].gir%90)==0)//para os objetos pararem de rodar
			lmap[i].giro=0;
	}
}

void Conferirclique(bool la)//conferir se o clique feito tem algum efeito
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	/*if(((245<=y)&&(y<=295))&&((50<=x)&&(x<=100)))//não funciona
	{
		Rolar(1, y, 1.5);
	}
	if(((300<=y)&&(y<=350))&&((50<=x)&&(x<=100)))
	{
		Rolar(3, y, 2);
	}*/
	for(int i=0; i<lmap.size(); i++)
	{
		if(((lmap[i].getDX()<=x)&&(x<=(lmap[i].getDX()+tilesz))) && ((lmap[i].getDY()<=y)&&(y<=(lmap[i].getDY()+tilesz))))
		{
			if (la)
			{
				lmap[i].giro=1;
			}
			else
			{
				lmap[i].giro=2;
			}
			mov++;
		}
	}
}

void Rolar(int la, int y, double mult)//não funciona
{
	int aux=0;
	if(la==1)
	{
		muda=lmap;
		for(int i=0; i<lmap.size(); i++)
		{
			if((i%5)==0)
				aux++;
			if(lmap[i].getDY()<y)
			{
				lmap[i].setDest((i*(tilesz*mult))+25-(aux*(tilesz*mult*i)), (aux*(mult*tilesz))+50, tilesz*mult, tilesz*mult);
			}
			if(lmap[i].getDY()>y)
			{
				lmap[i].setDest(lmap[i].getDX(), lmap[i].getDY()+(tilesz*mult));
			}
		}
	}
	else if(la==2)
	{
		muda=lmap;
		for(int i=0; i<lmap.size(); i++)
		{
			if(lmap[i].getDY()<y)
			{
				lmap[i].setDest(lmap[i].getDX()-(tilesz*mult), lmap[i].getDY()-(tilesz*mult), tilesz*mult, tilesz*mult);
			}
			if(lmap[i].getDY()>y)
			{
				lmap[i].setDest(lmap[i].getDX(), lmap[i].getDY()-(tilesz*mult));
			}
		}
	}
	else if(la==3)
	{
		lmap=muda;
	}
}

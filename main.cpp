#include <iostream>
#include <string>

#include "raylib.h"
#include "mouse.cpp"

using namespace std;

enum GameState {
    MainMenu,
    Playing,
	Instr
};

GameState currentState = MainMenu;

void mainMenu();
void gameLoop();
void Instrucoes();

float screenWidth = 1600;
float screenHeight = 800;

Rectangle cx = {543,585,106,85}; //cx registradora
Rectangle desenhoSom = {1515, 15, 60, 60};


//PARA ARRASTAR OBJETOS MORANGO
Vector2 offset[5] = {{0, 0}, {0, 0}, {0, 0},{0,0},{0,0}};
bool arrastando[5]={false};
Rectangle cestas[3] = { {768,521,125,300}, {988,521,125,300}, {1208,521,125,300}};


int main()
{
	InitWindow(screenWidth, screenHeight, "for all the supermarket's lovers");
	SetTargetFPS(60);

	while (!WindowShouldClose()) {
        switch (currentState) {
            case MainMenu:
                mainMenu();
                break;
            case Playing:
                gameLoop();
                break;
			case Instr:
                Instrucoes();
				break;
        }
    }

	return 0;
}



void mainMenu () {

	//TITULO
	Font font = LoadFont("resources/mecha.png");
	const char *messages = "Cashier Simulator";
	Vector2 positions = {80, 60.0f + font.baseSize + 45.0f };

	Image food = LoadImage("resources/food.png");  //Inicia como imagem
	ImageResize(&food, 500,500); //Altera o tamanho
	Texture2D Food = LoadTextureFromImage(food); //Altera de imagem pra textura


	Rectangle opcao[3];
	Color color[3] = {BLACK,BLACK,BLACK};
	opcao[0]={80,350,121,32};
	opcao[1]={80,350+70,267,32};
	opcao[2]={80,350+140,250,32};
	

	BeginDrawing();

	ClearBackground(WHITE);

	for (int i=0;i<3;i++) {
		if (CheckCollisionPointRec(GetMousePosition(), opcao[i])) 
			color[i] = GOLD;
	}

	DrawTextEx(font, messages, positions, font.baseSize*8.0f, 4, BLACK);
	DrawText("Jogar", 80, 350, 40, color[0]);
	DrawText("Instruções", 80, 420, 40, color[1]);		
	DrawText("Sair", 80, 490, 40, color[2]);	
	
	DrawTexture(Food, 1200, 400, WHITE);
	

	if (IsMouseButtonPressed(0) && CheckCollisionPointRec(GetMousePosition(), opcao[0])) {
		currentState = Playing;
    }
	else if (IsMouseButtonPressed(0) && CheckCollisionPointRec(GetMousePosition(), opcao[1])){
        currentState = Instr;
    }
	else if (IsMouseButtonPressed(0) && CheckCollisionPointRec(GetMousePosition(), opcao[2])){
        CloseWindow(); 
    }

	EndDrawing();
	UnloadTexture(Food);
	UnloadFont(font);
}


void gameLoop() {

	//SOM
	InitAudioDevice();
	Sound sound = LoadSound("resources/market_song.mp3");
	Sound sound2 = LoadSound("resources/scanner.mp3");
	bool pause = false;

	//Letra estilizada
	Font font = LoadFont("resources/mecha.png");
	const char *messages = "Cashier Simulator";
	Vector2 positions = {50, 20 };

	//TEXTURAS 
	Image morango = LoadImage("resources/morango.png");  //morango
	ImageResize(&morango, 525, 300); 
	Texture2D strawberry= LoadTextureFromImage(morango); 
	Image balcao1 = LoadImage("resources/balcao.png");  //balcao
	ImageResize(&balcao1, 2000, 800); 
	Texture2D balcao = LoadTextureFromImage(balcao1);	
	Image cesta1 = LoadImage("resources/cesta.png");  //cesta
	ImageResize(&cesta1, 700, 600);
	Texture2D cesta = LoadTextureFromImage(cesta1); 
	Image broc = LoadImage("resources/brocolis.png");  //brocolis
	ImageResize(&broc, 600, 400); 
	Texture2D brocolis = LoadTextureFromImage(broc); 
	Image SoundON = LoadImage("resources/SoundON.png");  //som on
	ImageResize(&SoundON, 100, 100); 
	Texture2D soundON = LoadTextureFromImage(SoundON); 
	Image SoundOFF = LoadImage("resources/SoundOFF.png");  //som off
	ImageResize(&SoundOFF, 100, 100); 
	Texture2D soundOFF = LoadTextureFromImage(SoundOFF);
	Image prateleira = LoadImage("resources/prateleira.png");  //prateleirinha
	ImageResize(&prateleira, 1000, 400); 
	Texture2D Prateleira = LoadTextureFromImage(prateleira);
	Image abacaxi = LoadImage("resources/abacaxi.png"); //abacaxi
	ImageResize(&abacaxi, 800, 500);
	Texture2D Abacaxi = LoadTextureFromImage(abacaxi);
	Image salgad = LoadImage("resources/salgad.png"); //salgadinho
	ImageResize(&salgad,240,260);
	Texture2D salgadinho = LoadTextureFromImage(salgad);
	Image refrig = LoadImage("resources/refrig.png"); //refri
	ImageResize(&refrig,220, 200);
	Texture2D refri = LoadTextureFromImage(refrig);


	Rectangle botaoFim = {700, 30, 205, 50};
	Rectangle botaoTira = {1000, 30, 205, 50};

	//PRODUTOS - cadastro
	Produto m,b,a, s,r;
	m = {"Morango", strawberry, {150, 460, (float)strawberry.width/8, (float)strawberry.height/4}, arrastando[0]};
    b = {"Brócolis", brocolis, {20, 440, (float)brocolis.width/7, (float)brocolis.height/4}, arrastando[1]};
	a = {"Abacaxi", Abacaxi, {253, 540, (float)Abacaxi.width/10, (float)Abacaxi.height/5}, arrastando[2]};
	s = {"Salgadinho", salgadinho, {130,610, (float)salgadinho.width/2, (float)salgadinho.height/2}, arrastando[3]};
	r = {"Refrigerante", refri, {50,545, (float)refri.width/2, (float)refri.height/2}, arrastando[4]};

	///////////////////Parte "global" da função/////////////////////////////////////////////////////////////////////

	//Objetivo: Criar uma fila. Tal lista comparamos com uma já pronta. Se inserida na ordem correta acertou. Senão perdeu
	Fila ListaDeCompra, ListaDeCompra2, ListaDeCompra3, ListaCesta1, ListaCesta2, ListaCesta3;

	Cria(ListaDeCompra);
	Insere(ListaDeCompra, m); 
	Insere(ListaDeCompra, a);
	Insere(ListaDeCompra, b);

	Cria(ListaDeCompra2);
	Insere(ListaDeCompra2, s);

	Cria(ListaDeCompra3);
	Insere(ListaDeCompra3, r);
	
	Cria(ListaCesta1);
	Cria(ListaCesta2);
	Cria(ListaCesta3);

	bool boolCx[5]={false};
	bool jaFoi[5]={false}, jaTirou=false;
	Produto aux;
	
	int estado =0;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	while(!WindowShouldClose())
	{	

		//SOM DA CAIXA REGISTRADORA
		if ((CheckCollisionPointRec(GetMousePosition(), cx)) && (arrastando[0]||arrastando[1]||arrastando[2]||arrastando[3]||arrastando[4])) {
			PlaySound(sound2);
			if (arrastando[0])
				boolCx[0]=true;
			else if (arrastando[1])
				boolCx[1]=true;
			else if (arrastando[2])
				boolCx[2]=true;
			else if (arrastando[3])
				boolCx[3]=true;
			else if (arrastando[4])
				boolCx[4]=true;
		}

		//MUSICA DE MERCADO
		if (CheckCollisionPointRec(GetMousePosition(), desenhoSom) && IsMouseButtonPressed(0)) {
			if (pause) 
				PauseSound(sound);
			else 
				PlaySound(sound);
			pause = !pause;
		}

		//MORANGO
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			arrastando[0] = apertar(&m, &arrastando[0], &offset[0]);
		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
			soltar(&arrastando[0]);

		//BROCOLIS
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			arrastando[1] = apertar(&b, &arrastando[1], &offset[1]);
		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
			soltar(&arrastando[1]);
		
		//ABACAXI
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			arrastando[2] = apertar(&a, &arrastando[2], &offset[2]);
		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
			soltar(&arrastando[2]);

		//salgadinho
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			arrastando[3] = apertar(&s, &arrastando[3], &offset[3]);
		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
			soltar(&arrastando[3]);

		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			arrastando[4] = apertar(&r, &arrastando[4], &offset[4]);
		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
			soltar(&arrastando[4]);

		///////////////////!WindowShouldClose parte/////////////////////////////////////////////////////////////////////


		for (int i=0;i<5;i++) {
			if (arrastando[i] && CheckCollisionPointRec(GetMousePosition(), cestas[0]) && boolCx[i] && !jaFoi[i]) {
				
				jaTirou=false;

				if (i==0) {
					Insere(ListaCesta1, m);
					jaFoi[0]=true;}
				else if (i==1) {
					Insere(ListaCesta1, b);
					jaFoi[1]=true;}
				else if (i==2) {
					Insere(ListaCesta1, a);
					jaFoi[2]=true;}
				else if (i==3) {
					Insere(ListaCesta1, s);
					jaFoi[3]=true;
				}
				else if (i==4) {
					Insere(ListaCesta1, r);
					jaFoi[4]=true;
				}
			}
		}
		for (int i=0;i<5;i++) {
			if (arrastando[i] && CheckCollisionPointRec(GetMousePosition(), cestas[1]) && boolCx[i] && !jaFoi[i]) {
				if (i==0){
					Insere(ListaCesta2, m);
					jaFoi[0]=true;
				}
				else if (i==1) {
					Insere(ListaCesta2, b);
					jaFoi[1]=true;
				}
				else if (i==2){
					Insere(ListaCesta2, a);
					jaFoi[2]=true;
				}
				else if (i==3) {
					Insere(ListaCesta2,s);
					jaFoi[3]=true;
				}
				else if (i==4) {
					Insere(ListaCesta2,r);
					jaFoi[4]=true;
				}
			}
		}
		for (int i=0;i<5;i++) {
			if (arrastando[i] && CheckCollisionPointRec(GetMousePosition(), cestas[2]) && boolCx[i] && !jaFoi[i]) {
				
				if (i==0) {
					Insere(ListaCesta3, m);
					jaFoi[0]=true;
				}
				else if (i==1) {
					Insere(ListaCesta3, b);
					jaFoi[1]=true;
				}
				else if (i==2) {
					Insere(ListaCesta3, a);
					jaFoi[2]=true;
				}
				else if (i==3) {
					Insere(ListaCesta3,s);
					jaFoi[3]=true;
				}
				else if (i==4) {
					Insere(ListaCesta3,r);
					jaFoi[4]=true;
				}
			}
		}

		if (IsMouseButtonDown(0) && CheckCollisionPointRec(GetMousePosition(), botaoTira) && !jaTirou && estado==0) {		
			jaTirou=true;
			Retira(ListaCesta1, aux);

			if (aux.nome == m.nome) 
				jaFoi[0]=false;
			else if (aux.nome == b.nome) 
				jaFoi[1]=false;
			else if (aux.nome == a.nome) 
				jaFoi[2]=false;
			else if (aux.nome == s.nome)
				jaFoi[3]=false;
			else if (aux.nome == r.nome)
				jaFoi[4]=false;

		}
	

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		BeginDrawing();

		ClearBackground(WHITE);
	    DrawTextEx(font, messages, positions, font.baseSize*4.0f, 4, BLACK); 

		if (pause) //TOCAR MUSICA
			DrawTexture(soundON, 1500, 0, WHITE);
		if (!pause)
			DrawTexture(soundOFF, 1500, 0, WHITE);

		if (arrastando[0]) { //morango
			m.rec.x = GetMouseX() - offset[0].x;
			m.rec.y = GetMouseY() - offset[0].y;
		}
		if (arrastando[1]) { //brocolis
			b.rec.x = GetMouseX() - offset[1].x;
			b.rec.y = GetMouseY() - offset[1].y;
		}
		if (arrastando[2]) { //abacaxi
			a.rec.x = GetMouseX() - offset[2].x;
			a.rec.y = GetMouseY() - offset[2].y;
		}
		if (arrastando[3]) { //salg
			s.rec.x = GetMouseX() - offset[3].x;
			s.rec.y = GetMouseY() - offset[3].y;
		}
		if (arrastando[4]) { //salg
			r.rec.x = GetMouseX() - offset[4].x;
			r.rec.y = GetMouseY() - offset[4].y;
		}
	
		///////////////////Drawing Parte////////////////////////////////////////////////////////////////////////////////

	
		nivel1 (ListaDeCompra, ListaCesta1, botaoFim);

//		nivel2 (ListaDeCompra2, ListaCesta2, botaoFim);
	
//		nivel3 (ListaDeCompra3, ListaCesta3, botaoFim);

/*
		if (estado==0) {
			DrawText("Coloque os produtos na primeira cesta", 50,90,30,DARKGRAY);
			printLista(ListaDeCompra);
			printFila(ListaCesta1);
			
			if (IsMouseButtonDown(0) && CheckCollisionPointRec(GetMousePosition(), botaoFim)) {
				if (comparaFila(ListaDeCompra, ListaCesta1)) {
					DrawText("acertou miseravel", 520,300,50, GREEN);
					estado++;
				}
				else {
					DrawText("parabens, voce perdeu!", 520,300,50, RED);
				}
			}
		}
		if (estado==1) {
			
			DrawText("Coloque os produtos na segunda cesta", 50,90,30,DARKGRAY);
			printLista(ListaDeCompra2);
			printFila(ListaCesta2);

			if (IsMouseButtonDown(0) && CheckCollisionPointRec(GetMousePosition(), botaoFim)) {
				if (comparaFila(ListaDeCompra2, ListaCesta2)) {
					DrawText("acertou miseravel", 520,300,50, GREEN);
					estado++;
				}
				else {
					DrawText("parabens, voce perdeu!", 520,300,50, RED);
				}
			}
		}
		if (estado == 2) {
			DrawText("Coloque os produtos na terceira cesta", 50,90,30,DARKGRAY);
			printLista(ListaDeCompra3);
			printFila(ListaCesta3);

			if (IsMouseButtonDown(0) && CheckCollisionPointRec(GetMousePosition(), botaoFim)) {
				if (comparaFila(ListaDeCompra3, ListaCesta3)) {
					DrawText("acertou miseravel", 520,300,50, GREEN);
					estado++;
				}
				else {
					DrawText("parabens, voce perdeu!", 520,300,50, RED);
				}
			}
		}
*/
		

		
		
 
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		

		//desenho da prateleira e cestas
		DrawRectangle(35, 428, 328, 520, BEIGE);
		DrawRectangleLines(35, 428, 328, 520, BROWN);

		//botoes de fim e retirar
		DrawRectangle(700, 30, 205, 50, LIGHTGRAY);
		DrawRectangleLines(700, 30, 205, 50, BLACK);
		DrawRectangle(1000, 30, 205, 50, LIGHTGRAY);
		DrawRectangleLines(1000, 30, 205, 50, BLACK);

		DrawText("FIM DAS COMPRAS", 705, 44, 20, BLACK);
		DrawText("RETIRAR ULTIMO", 1010, 44, 20, BLACK);

		int coordCesta = 480, h = 568;
		for (int i=0;i<3;i++) {
			DrawTexture(cesta, coordCesta, 421, WHITE);
			DrawTexture(Prateleira, -300, h, WHITE);
			coordCesta +=220;
			h -= 110;
		}

		//desenho de mais texturas
		DrawTexture(balcao, -480, 300, WHITE);	
		DrawTexture(salgadinho, s.rec.x-50,s.rec.y-45,WHITE);	
		DrawTexture(refri, r.rec.x-50,r.rec.y-45,WHITE);
		DrawTexture(strawberry, m.rec.x-230, m.rec.y-120, WHITE);
		DrawTexture(brocolis, b.rec.x-245, b.rec.y-160, WHITE); 
		DrawTexture(Abacaxi, a.rec.x-355, a.rec.y-220, WHITE);

		//retangulos para demarcar area da cesta
		DrawRectangleLines(768,521,125,300, SKYBLUE);
		DrawRectangleLines(988,521,125,300, SKYBLUE);
		DrawRectangleLines(1208,521,125,300, SKYBLUE);

		EndDrawing();

	}

	//unload coisas
	UnloadImage(refrig);
	UnloadImage(salgad); 
	UnloadImage(morango); 
	UnloadImage(balcao1);
	UnloadImage(abacaxi);
	UnloadImage(cesta1);
	UnloadImage(broc);
	UnloadImage(SoundOFF);
	UnloadImage(SoundON);
	UnloadImage(prateleira);
	UnloadSound(sound);
	UnloadSound(sound2);
	UnloadFont(font);

	CloseAudioDevice();
	CloseWindow();	


}

void Instrucoes() {
	InitWindow(screenWidth, screenHeight, "Manual de instruções para os perdidos");
	SetTargetFPS(60);

	while (!WindowShouldClose())  {

		BeginDrawing();
		
		ClearBackground(WHITE);
		DrawText("Parabens pelo seu novo emprego como caixa de mercado!", 50, 80, 40, DARKPURPLE);
		DrawText("Aqui estão algumas instruções já que é seu primeiro dia", 50, 120, 40, DARKPURPLE);
	    
		DrawText("Coloque os produtos na ordem certa de acordo com essa lista", 50, 180, 30, BLACK);
		DrawText("Para comprar os produtos, arraste-os pela caixa registradora", 50, 240, 30, BLACK);
		DrawText("ATENÇÃO! Somente o ultimo produto inserido pode ser retirado", 50, 300, 30, BLACK);
		DrawText("Para isso, primeiro tire o produto da área da cesta e depois clique em RETIRAR ULTIMO", 50, 360, 30, BLACK);

		EndDrawing();



	}

	CloseWindow();
}
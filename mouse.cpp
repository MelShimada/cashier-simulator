#ifndef MOUSE_CPP
#define MOUSE_CPP

#include <iostream>
#include "raylib.h"

#include "string.h"

using namespace std;

#define TAM 5

typedef struct {
    string nome; //morango,brocolis,abobora
    Texture2D prod;
    Rectangle rec; //area q fica o produto
    bool arrastando;
} Produto;

//Fila de produtos
typedef struct {
    Produto* ProdutoPtr;
    int ultimo;
} Fila;

void Cria(Fila &F) {
    F.ProdutoPtr = new Produto[TAM];
    F.ultimo=-1;
}

bool getUltimo(Fila &F, Produto& p) {
    if (F.ultimo>=0 && F.ProdutoPtr != NULL) {
        p=F.ProdutoPtr[F.ultimo];
        return true;
    }
    return false;
}

void Insere(Fila &F, Produto p) {
    bool sim=true;
    Produto aux;

    //inserir o produto somente uma vez
    if (getUltimo(F,aux)) {
        if (strcmp(aux.nome.c_str(), p.nome.c_str())==0) 
            sim = false;
    }

    if (sim) {
        if (F.ultimo<TAM) {
            F.ultimo++;
            F.ProdutoPtr[F.ultimo] = p;
        }
    }
}

void Retira(Fila &F, Produto &p) {
    if (F.ultimo>=0 && F.ProdutoPtr != NULL) {
        p = F.ProdutoPtr[F.ultimo];
        F.ultimo--;
    }
}

void Destrutor(Fila &F) {
    delete[] F.ProdutoPtr;
    F.ProdutoPtr =NULL;
}

bool Vazio(Fila F) {
    if (F.ultimo == -1 && F.ProdutoPtr != NULL) 
        return true;
    return false;
}


void printLista(const Fila F) {
    DrawText("O cliente quer: ", 150,150,30,RED);
    int y = 190;
    for (int i = 0; i <= F.ultimo; ++i) {
        const Produto &p = F.ProdutoPtr[i];
        DrawText(p.nome.c_str(), 150, y, 30, DARKGRAY);
        y+=40;
    }
}

void printFila(const Fila F) {
    DrawText("SUA CESTA TEM: ", 550,150,30,RED);
    int y = 190;
    for (int i = 0; i <= F.ultimo; ++i) {
        const Produto &p = F.ProdutoPtr[i];
        DrawText(p.nome.c_str(), 550, y, 30, DARKGRAY);
        y+=40;
    }
}


bool comparaFila(Fila F1, Fila F2) {
    bool igualPorEnquanto=false;
    if (F1.ultimo!=F2.ultimo)
        return false;
    else {
        for (int i=0;i<F2.ultimo+1;i++) {
            if (strcmp(F1.ProdutoPtr[i].nome.c_str(), F2.ProdutoPtr[i].nome.c_str())==0) 
                igualPorEnquanto=true;
            else 
                return false;
        }
        return igualPorEnquanto;
    }
}

bool compara(Fila F1, Fila F2) {
    if (F1.ultimo==F2.ultimo && !comparaFila(F1,F2)) {
        return true;
    }
    return false;
}


bool apertar(Produto* obj, bool* arrastando, Vector2* offset) {
    if (CheckCollisionPointRec(GetMousePosition(), obj->rec)) {
		*arrastando = true;
		offset->x = GetMouseX() - obj->rec.x;
        offset->y = GetMouseY() - obj->rec.y;
	}	
    return *arrastando;
}

void soltar(bool* arrastando) {
    *arrastando=false;
}


void nivel1(Fila ListaDeCompra, Fila ListaCesta1, Rectangle botaoFim) {
  
        DrawText("Coloque os produtos na primeira cesta", 50,90,30,DARKGRAY);
        printLista(ListaDeCompra);
        printFila(ListaCesta1);
                
        if (IsMouseButtonDown(0) && CheckCollisionPointRec(GetMousePosition(), botaoFim)) {
            if (comparaFila(ListaDeCompra, ListaCesta1)) {
                DrawText("acertou miseravel", 520,300,50, GREEN);
               
            }
            else {
                DrawText("parabens, voce perdeu!", 520,300,50, RED);
            }
        }
    
}

void nivel2 (Fila ListaDeCompra2, Fila ListaCesta2, Rectangle botaoFim) {
   
        DrawText("Coloque os produtos na segunda cesta", 50,90,30,DARKGRAY);
        printLista(ListaDeCompra2);
        printFila(ListaCesta2);

        if (IsMouseButtonDown(0) && CheckCollisionPointRec(GetMousePosition(), botaoFim)) {
            if (comparaFila(ListaDeCompra2, ListaCesta2)) {
                DrawText("acertou miseravel", 520,300,50, GREEN);
             
            }
            else {
                DrawText("parabens, voce perdeu!", 520,300,50, RED);
            }
        }
    
}

void nivel3 (Fila ListaDeCompra3, Fila ListaCesta3, Rectangle botaoFim) {
  
        DrawText("Coloque os produtos na terceira cesta", 50,90,30,DARKGRAY);
        printLista(ListaDeCompra3);
        printFila(ListaCesta3);

        if (IsMouseButtonDown(0) && CheckCollisionPointRec(GetMousePosition(), botaoFim)) {
            if (comparaFila(ListaDeCompra3, ListaCesta3)) {
                DrawText("acertou miseravel", 520,300,50, GREEN);
               
            }
            else {
                DrawText("parabens, voce perdeu!", 520,300,50, RED);
            }
        }
    
}


#endif
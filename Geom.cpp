#include "Geom.h"
#include <Windows.h>
#include <iostream>

// Obt�m as dimens�es da tela
int screenX = GetSystemMetrics(SM_CXSCREEN);
int screenY = GetSystemMetrics(SM_CYSCREEN);

// Defini��o de uma cor para inimigos
#define EnemyPen 0x000000FF

// Cria��o de um pincel s�lido para inimigos
HBRUSH EnemyBrush = CreateSolidBrush(0x000000FF);

// Obt�m o contexto do dispositivo gr�fico (DC) da janela do jogo "AssaultCube"
HDC hdc = GetDC(FindWindowA(NULL, "AssaultCube"));

// Estrutura RECT para armazenar as dimens�es da janela do jogo
RECT rect;

// Fun��o para atualizar a estrutura RECT com as dimens�es da janela
void setRect() {
    GetWindowRect(FindWindowA(NULL, "AssaultCube"), &rect);
}

// Converte as coordenadas 3D do mundo para as coordenadas da tela
Vec3 WorldToRender(const Vec3 pos, view_matrix_t matrix) {
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;
    float screenX = (matrix[0][0] * pos.x) + (matrix[1][0] * pos.y) + (matrix[2][0] * pos.z) + matrix[3][0];
    float screenY = (matrix[0][1] * pos.x) + (matrix[1][1] * pos.y) + (matrix[2][1] * pos.z) + matrix[3][1];
    float screenW = (matrix[0][3] * pos.x) + (matrix[1][3] * pos.y) + (matrix[2][3] * pos.z) + matrix[3][3];

    // Posi��o da c�mera (n�vel dos olhos/centro da tela)
    float camX = width / 2.f;
    float camY = height / 2.f;

    // Converte para posi��o homog�nea
    float x = camX + (camX * screenX / screenW);
    float y = camY - (camY * screenY / screenW);

    // Se o ponto estiver muito pr�ximo da c�mera, retorna {0,0,0}
    if (!(screenW > 0.001f)) return { 0,0,0 };

    return { x,y,0 };
}

// Desenha um ret�ngulo preenchido na tela
void DrawFilledRect(int x, int y, int w, int h) {
    RECT rect = { x, y, x + w, y + h };
    FillRect(hdc, &rect, EnemyBrush);
}

// Desenha uma caixa delimitadora na tela com uma borda
void DrawBorderBox(int x, int y, int w, int h, int thickness) {
    DrawFilledRect(x, y, w, thickness); // Linha horizontal superior
    DrawFilledRect(x, y, thickness, h); // Linha vertical esquerda
    DrawFilledRect((x + w), y, thickness, h); // Linha vertical direita
    DrawFilledRect(x, y + h, w + thickness, thickness); // Linha horizontal inferior
}

// Desenha uma linha na tela
void DrawLine(float StartX, float StartY, float EndX, float EndY) {
    int a, b = 0;
    HPEN hOPen;
    HPEN hNPen = CreatePen(PS_SOLID, 2, EnemyPen); // Estilo de caneta s�lida, largura 2, cor EnemyPen
    hOPen = (HPEN)SelectObject(hdc, hNPen);
    MoveToEx(hdc, StartX, StartY, NULL); // Define o ponto inicial
    a = LineTo(hdc, EndX, EndY); // Desenha a linha
    DeleteObject(SelectObject(hdc, hOPen));
}

// Calcula a dist�ncia (Magnitude) quadrada entre dois pontos Vec3
float Vec3SquareDistance(Vec3 v, Vec3 w) {
    Vec3 difference = v - w;
    return (difference.x * difference.x + difference.y * difference.y + difference.z * difference.z);
}

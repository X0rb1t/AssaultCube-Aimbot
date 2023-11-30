#pragma once

// Estrutura representando um vetor tridimensional
struct Vec3 {
    float x, y, z;

    // Sobrecarga do operador de subtra��o para calcular a diferen�a entre dois vetores
    Vec3 operator-(const Vec3& other) const {
        Vec3 result;
        result.x = x - other.x;
        result.y = y - other.y;
        result.z = z - other.z;
        return result;
    }
};

// Estrutura representando uma matriz de visualiza��o 4x4
struct view_matrix_t {
    float* operator[](int index) {
        return matrix[index];
    }

    float matrix[4][4];
};

// Fun��o para converter coordenadas 3D do mundo para as coordenadas da tela
Vec3 WorldToRender(const Vec3 pos, view_matrix_t matrix);

// Fun��o para definir a estrutura RECT com as dimens�es da janela
void setRect();

// Fun��o para desenhar uma linha na tela
void DrawLine(float StartX, float StartY, float EndX, float EndY);

// Fun��o para desenhar uma caixa delimitadora na tela com uma borda
void DrawBorderBox(int x, int y, int w, int h, int thickness);

// Fun��o para desenhar um ret�ngulo preenchido na tela
void DrawFilledRect(int x, int y, int w, int h);

// Fun��o para calcular a dist�ncia quadrada entre dois pontos Vec3
float Vec3SquareDistance(Vec3 v, Vec3 w);

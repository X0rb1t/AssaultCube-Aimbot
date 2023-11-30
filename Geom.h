#pragma once

// Estrutura representando um vetor tridimensional
struct Vec3 {
    float x, y, z;

    // Sobrecarga do operador de subtração para calcular a diferença entre dois vetores
    Vec3 operator-(const Vec3& other) const {
        Vec3 result;
        result.x = x - other.x;
        result.y = y - other.y;
        result.z = z - other.z;
        return result;
    }
};

// Estrutura representando uma matriz de visualização 4x4
struct view_matrix_t {
    float* operator[](int index) {
        return matrix[index];
    }

    float matrix[4][4];
};

// Função para converter coordenadas 3D do mundo para as coordenadas da tela
Vec3 WorldToRender(const Vec3 pos, view_matrix_t matrix);

// Função para definir a estrutura RECT com as dimensões da janela
void setRect();

// Função para desenhar uma linha na tela
void DrawLine(float StartX, float StartY, float EndX, float EndY);

// Função para desenhar uma caixa delimitadora na tela com uma borda
void DrawBorderBox(int x, int y, int w, int h, int thickness);

// Função para desenhar um retângulo preenchido na tela
void DrawFilledRect(int x, int y, int w, int h);

// Função para calcular a distância quadrada entre dois pontos Vec3
float Vec3SquareDistance(Vec3 v, Vec3 w);

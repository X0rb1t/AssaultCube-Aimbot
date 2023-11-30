#pragma once
#include <string>
#include "Memory.h"
#include "Offsets.h"
#include "Geom.h"

// Classe para representar uma entidade no jogo
class Entity {
private:
    uint32_t address; // Endere�o da entidade na mem�ria
    Vec3 position; // Posi��o da entidade
    float horizontal; // �ngulo de rota��o horizontal da entidade
    float vertical; // �ngulo de rota��o vertical da entidade
    int health; // Pontos de vida da entidade
    int team; // N�mero que representa a equipe da entidade
    std::string name; // Nome da entidade
    Memory* memory; // Ponteiro para a classe Memory que manipula a mem�ria do processo

public:
    // Construtor: Inicializa uma inst�ncia da classe Entity com o ponteiro para a classe Memory e o endere�o da entidade
    Entity(Memory* memory, uint32_t address);

    // Retorna o nome da entidade
    std::string getName() {
        return name;
    }

    // Retorna o n�mero da equipe da entidade
    int getTeam() {
        return team;
    }

    // Retorna a quantidade de pontos de vida da entidade
    int getHealth();

    // Retorna a posi��o da cabe�a da entidade como um objeto Vec3
    Vec3 getHeadPos();

    // Retorna a posi��o dos p�s da entidade como um objeto Vec3
    Vec3 getFeetPos();

    // Define o �ngulo de rota��o horizontal da entidade
    void setHorizontal(float yaw);

    // Define o �ngulo de rota��o vertical da entidade
    void setVertical(float pitch);
};

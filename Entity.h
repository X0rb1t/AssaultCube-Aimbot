#pragma once
#include <string>
#include "Memory.h"
#include "Offsets.h"
#include "Geom.h"

// Classe para representar uma entidade no jogo
class Entity {
private:
    uint32_t address; // Endereço da entidade na memória
    Vec3 position; // Posição da entidade
    float horizontal; // Ângulo de rotação horizontal da entidade
    float vertical; // Ângulo de rotação vertical da entidade
    int health; // Pontos de vida da entidade
    int team; // Número que representa a equipe da entidade
    std::string name; // Nome da entidade
    Memory* memory; // Ponteiro para a classe Memory que manipula a memória do processo

public:
    // Construtor: Inicializa uma instância da classe Entity com o ponteiro para a classe Memory e o endereço da entidade
    Entity(Memory* memory, uint32_t address);

    // Retorna o nome da entidade
    std::string getName() {
        return name;
    }

    // Retorna o número da equipe da entidade
    int getTeam() {
        return team;
    }

    // Retorna a quantidade de pontos de vida da entidade
    int getHealth();

    // Retorna a posição da cabeça da entidade como um objeto Vec3
    Vec3 getHeadPos();

    // Retorna a posição dos pés da entidade como um objeto Vec3
    Vec3 getFeetPos();

    // Define o ângulo de rotação horizontal da entidade
    void setHorizontal(float yaw);

    // Define o ângulo de rotação vertical da entidade
    void setVertical(float pitch);
};

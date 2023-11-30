#include <iostream>
#include <iomanip>
#include <sstream>
#include "Memory.h"
#include "Offsets.h"
#include <thread>
#include <chrono>
#include "Entity.h"
#include <vector>
#include <cmath>

const double PI = 3.14159265358979323846;
int screenViewX = GetSystemMetrics(SM_CXSCREEN);
int screenViewY = GetSystemMetrics(SM_CYSCREEN);

int main()
{
    // Inicializa a instância da classe Memory, que é usada para acessar a memória do processo.
    auto memory = Memory("ac_client.exe");

    // Obtém o endereço base do módulo "ac_client.exe".
    auto baseModuleAddress = memory.GetModuleAddress("ac_client.exe");

    // Calcula o offset do jogador dentro do processo.
    uintptr_t playerOffset = baseModuleAddress + offsets::player_base_ptr;

    // Lê o ponteiro de base do jogador.
    uintptr_t player = memory.Read<uint32_t>(playerOffset);

    // Cria uma instância da classe Entity para representar o jogador.
    Entity playerEntity = Entity(&memory, player);

    // Obtém o endereço base da lista de entidades no jogo.
    const uint32_t entityListBase = baseModuleAddress + offsets::entity_list_base_ptr;
    const uint32_t entityList = memory.Read<uint32_t>(entityListBase);

    // Declara um vetor para armazenar as entidades no jogo.
    std::vector<Entity> entities;

    // Loop para iterar sobre as entidades no jogo.
    for (int i = 0; i < 32; i++) { // 32 é o número máximo de jogadores - 1 (o próprio jogador).
        const uint32_t entityOffset = entityList + ((i + 1) * 4);
        const uint32_t entityBase = memory.Read<uint32_t>(entityOffset);

        // Verifica se o endereço da entidade é válido.
        if (!memory.isValidAddress(entityBase)) break;

        // Verifica se a entidade é válida.
        if (!memory.isValidEntity(entityBase)) break;

        // Cria uma instância da classe Entity para a entidade atual e a adiciona ao vetor.
        Entity entity = Entity(&memory, entityBase);
        entities.push_back(entity);
    }

    // Configuração de resolução da tela.
    setRect();

    // Loop principal do programa.
    while (true)
    {
        // Lê a matriz de visão do jogo.
        view_matrix_t viewMatrix = memory.Read<view_matrix_t>(baseModuleAddress + offsets::matrix_base_ptr);

        // Inicializa variáveis para encontrar a entidade mais próxima.
        int closestEntityIdx = -1;
        float closestEntityDistance = -1;
        Vec3 playerEntityHead = playerEntity.getHeadPos();

        // Loop para iterar sobre as entidades e realizar a renderização (ESP).
        for (int i = 0; i < entities.size(); i++) {
            if (entities[i].getTeam() == playerEntity.getTeam()) continue;
            if (entities[i].getHealth() < 1) continue;

            // Obtenção das posições da cabeça e dos pés da entidades.
            Vec3 headPosition = entities[i].getHeadPos();
            Vec3 screenHeadPos = WorldToRender(headPosition, viewMatrix);
            Vec3 feetPosition = entities[i].getFeetPos();
            Vec3 screenFeetPos = WorldToRender(feetPosition, viewMatrix);

            // Cálculo e renderização da caixa delimitadora ao redor da cabeça da entidade.
            float h = screenFeetPos.y - screenHeadPos.y;
            float w = h / 2;
            DrawBorderBox(screenHeadPos.x - w / 2, screenHeadPos.y - 30, w, h, 2);

            // Pré-processamento para o AIMBOT: Encontrar a entidade mais próxima.
            if (closestEntityIdx == -1) {
                closestEntityIdx = i;
                closestEntityDistance = Vec3SquareDistance(playerEntityHead, headPosition);
            }
            else {
                if (closestEntityDistance > Vec3SquareDistance(playerEntityHead, headPosition)) {
                    closestEntityIdx = i;
                    closestEntityDistance = Vec3SquareDistance(playerEntityHead, headPosition);
                }
            }
        }

        // AIMBOT: Se o botão direito do mouse estiver pressionado e houver uma entidade próxima, ajusta o ângulo de visão para a entidade mais próxima.
        // Verifica se o botão direito do mouse está pressionado e há uma entidade próxima.
        if (GetAsyncKeyState(VK_RBUTTON) && closestEntityIdx != -1) {
            // Obtém a posição da cabeça da entidade mais próxima.
            Vec3 enemyPosition = entities[closestEntityIdx].getHeadPos();

            // Calcula a diferença vetorial entre a posição da cabeça da entidade e a do jogador.
            Vec3 distance = enemyPosition - playerEntityHead;

            // Calcula o ângulo horizontal (yaw) entre o jogador e a entidade mais próxima.
            float horizontalAngle = (float)std::atan2(distance.y, distance.x) * 180 / PI + 90;

            // Calcula o ângulo vertical (pitch) entre o jogador e a entidade mais próxima.
            float verticalAngle = (float)std::atan2(distance.z, std::sqrt(distance.x * distance.x + distance.y * distance.y)) * 180 / PI;

            // Verifica se o ângulo horizontal não é zero antes de ajustar a direção da visão.
            if (horizontalAngle != 0)
                playerEntity.setHorizontal(horizontalAngle); // Ajusta o ângulo horizontal da entidade do jogador.

            // Ajusta o ângulo vertical da entidade do jogador com base no ângulo vertical calculado.
            playerEntity.setVertical(verticalAngle);
        }
    }
    return 0;
}

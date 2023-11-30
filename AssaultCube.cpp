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
    // Inicializa a inst�ncia da classe Memory, que � usada para acessar a mem�ria do processo.
    auto memory = Memory("ac_client.exe");

    // Obt�m o endere�o base do m�dulo "ac_client.exe".
    auto baseModuleAddress = memory.GetModuleAddress("ac_client.exe");

    // Calcula o offset do jogador dentro do processo.
    uintptr_t playerOffset = baseModuleAddress + offsets::player_base_ptr;

    // L� o ponteiro de base do jogador.
    uintptr_t player = memory.Read<uint32_t>(playerOffset);

    // Cria uma inst�ncia da classe Entity para representar o jogador.
    Entity playerEntity = Entity(&memory, player);

    // Obt�m o endere�o base da lista de entidades no jogo.
    const uint32_t entityListBase = baseModuleAddress + offsets::entity_list_base_ptr;
    const uint32_t entityList = memory.Read<uint32_t>(entityListBase);

    // Declara um vetor para armazenar as entidades no jogo.
    std::vector<Entity> entities;

    // Loop para iterar sobre as entidades no jogo.
    for (int i = 0; i < 32; i++) { // 32 � o n�mero m�ximo de jogadores - 1 (o pr�prio jogador).
        const uint32_t entityOffset = entityList + ((i + 1) * 4);
        const uint32_t entityBase = memory.Read<uint32_t>(entityOffset);

        // Verifica se o endere�o da entidade � v�lido.
        if (!memory.isValidAddress(entityBase)) break;

        // Verifica se a entidade � v�lida.
        if (!memory.isValidEntity(entityBase)) break;

        // Cria uma inst�ncia da classe Entity para a entidade atual e a adiciona ao vetor.
        Entity entity = Entity(&memory, entityBase);
        entities.push_back(entity);
    }

    // Configura��o de resolu��o da tela.
    setRect();

    // Loop principal do programa.
    while (true)
    {
        // L� a matriz de vis�o do jogo.
        view_matrix_t viewMatrix = memory.Read<view_matrix_t>(baseModuleAddress + offsets::matrix_base_ptr);

        // Inicializa vari�veis para encontrar a entidade mais pr�xima.
        int closestEntityIdx = -1;
        float closestEntityDistance = -1;
        Vec3 playerEntityHead = playerEntity.getHeadPos();

        // Loop para iterar sobre as entidades e realizar a renderiza��o (ESP).
        for (int i = 0; i < entities.size(); i++) {
            if (entities[i].getTeam() == playerEntity.getTeam()) continue;
            if (entities[i].getHealth() < 1) continue;

            // Obten��o das posi��es da cabe�a e dos p�s da entidades.
            Vec3 headPosition = entities[i].getHeadPos();
            Vec3 screenHeadPos = WorldToRender(headPosition, viewMatrix);
            Vec3 feetPosition = entities[i].getFeetPos();
            Vec3 screenFeetPos = WorldToRender(feetPosition, viewMatrix);

            // C�lculo e renderiza��o da caixa delimitadora ao redor da cabe�a da entidade.
            float h = screenFeetPos.y - screenHeadPos.y;
            float w = h / 2;
            DrawBorderBox(screenHeadPos.x - w / 2, screenHeadPos.y - 30, w, h, 2);

            // Pr�-processamento para o AIMBOT: Encontrar a entidade mais pr�xima.
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

        // AIMBOT: Se o bot�o direito do mouse estiver pressionado e houver uma entidade pr�xima, ajusta o �ngulo de vis�o para a entidade mais pr�xima.
        // Verifica se o bot�o direito do mouse est� pressionado e h� uma entidade pr�xima.
        if (GetAsyncKeyState(VK_RBUTTON) && closestEntityIdx != -1) {
            // Obt�m a posi��o da cabe�a da entidade mais pr�xima.
            Vec3 enemyPosition = entities[closestEntityIdx].getHeadPos();

            // Calcula a diferen�a vetorial entre a posi��o da cabe�a da entidade e a do jogador.
            Vec3 distance = enemyPosition - playerEntityHead;

            // Calcula o �ngulo horizontal (yaw) entre o jogador e a entidade mais pr�xima.
            float horizontalAngle = (float)std::atan2(distance.y, distance.x) * 180 / PI + 90;

            // Calcula o �ngulo vertical (pitch) entre o jogador e a entidade mais pr�xima.
            float verticalAngle = (float)std::atan2(distance.z, std::sqrt(distance.x * distance.x + distance.y * distance.y)) * 180 / PI;

            // Verifica se o �ngulo horizontal n�o � zero antes de ajustar a dire��o da vis�o.
            if (horizontalAngle != 0)
                playerEntity.setHorizontal(horizontalAngle); // Ajusta o �ngulo horizontal da entidade do jogador.

            // Ajusta o �ngulo vertical da entidade do jogador com base no �ngulo vertical calculado.
            playerEntity.setVertical(verticalAngle);
        }
    }
    return 0;
}

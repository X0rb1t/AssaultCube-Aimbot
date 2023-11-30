#pragma once

namespace offsets {
    // Offset para encontrar o ponteiro base do jogador
    static int player_base_ptr = 0x195404;

    // Offsets para obter informações sobre as entidades
    static int entity_health = 0xEC;
    static int entity_name = 0x205;
    static int entity_head = 0x4;
    static int entity_feet = 0x28;
    static int entity_team = 0x30C;
    static int entity_horizontal = 0x34;
    static int entity_vertical = 0x38;

    // Offset para encontrar o ponteiro base da lista de entidades
    static int entity_list_base_ptr = 0x18AC04;

    // Offset para encontrar o ponteiro base da matriz de visualização (view matrix)
    // Detalhes da matriz de visualização:
    // - Endereço inicial da matriz: 0x17E094
    // - Subtrai 0x104 para ajustar o ponteiro para um ponto específico dentro da matriz
    // - Adiciona (16 * 0x4) para deslocar o ponteiro para a posição correta dentro da matriz
    static int matrix_base_ptr = 0x17E094 - 0x104 + (16 * 0x4);
}

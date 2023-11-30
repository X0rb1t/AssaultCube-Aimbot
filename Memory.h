#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include "Geom.h"

// Classe para manipulação de memória do processo
class Memory {
private:
    DWORD id = 0; // ID do processo
    HANDLE process = NULL; // Handle do processo

public:
    // Construtor: Inicializa a classe e obtém o ID do processo
    Memory(const std::string processName);

    // Destrutor: Libera recursos quando o objeto é destruído
    ~Memory();

    // Retorna o ID do processo
    DWORD GetProcessId();

    // Retorna o handle do processo
    HANDLE GetProcessHandle();

    // Obtém o endereço base de um módulo pelo nome
    uintptr_t GetModuleAddress(const std::string moduleName);

    // Encontra o endereço final após seguir uma série de offsets em um ponteiro
    uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets);

    // Verifica se um endereço de memória é válido
    bool isValidAddress(uintptr_t address);

    // Verifica se um endereço contém uma entidade válida
    bool isValidEntity(uint32_t address);

    // Lê um valor de um endereço de memória
    template <typename T>
    T Read(uintptr_t address, int size = -1);

    // Lê uma string de um endereço de memória
    std::string ReadString(uintptr_t address, int size);

    // Lê um vetor 3D de um endereço de memória
    Vec3 ReadVec3(uintptr_t address);

    // Escreve um valor em um endereço de memória
    template <typename T>
    bool Write(uintptr_t address, T value);
};

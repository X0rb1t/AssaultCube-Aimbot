#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include "Geom.h"

// Classe para manipula��o de mem�ria do processo
class Memory {
private:
    DWORD id = 0; // ID do processo
    HANDLE process = NULL; // Handle do processo

public:
    // Construtor: Inicializa a classe e obt�m o ID do processo
    Memory(const std::string processName);

    // Destrutor: Libera recursos quando o objeto � destru�do
    ~Memory();

    // Retorna o ID do processo
    DWORD GetProcessId();

    // Retorna o handle do processo
    HANDLE GetProcessHandle();

    // Obt�m o endere�o base de um m�dulo pelo nome
    uintptr_t GetModuleAddress(const std::string moduleName);

    // Encontra o endere�o final ap�s seguir uma s�rie de offsets em um ponteiro
    uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets);

    // Verifica se um endere�o de mem�ria � v�lido
    bool isValidAddress(uintptr_t address);

    // Verifica se um endere�o cont�m uma entidade v�lida
    bool isValidEntity(uint32_t address);

    // L� um valor de um endere�o de mem�ria
    template <typename T>
    T Read(uintptr_t address, int size = -1);

    // L� uma string de um endere�o de mem�ria
    std::string ReadString(uintptr_t address, int size);

    // L� um vetor 3D de um endere�o de mem�ria
    Vec3 ReadVec3(uintptr_t address);

    // Escreve um valor em um endere�o de mem�ria
    template <typename T>
    bool Write(uintptr_t address, T value);
};

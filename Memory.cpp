#include "Memory.h"
#include <TlHelp32.h>
#include <vector>

// Construtor: Inicializa a classe Memory com o nome do processo
Memory::Memory(const std::string processName) {
    // Inicializa a estrutura de dados para obter informa��es sobre processos
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    // Cria um snapshot dos processos em execu��o
    const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    // Itera sobre os processos para encontrar o processo desejado pelo nome
    while (Process32Next(snapshot, &entry)) {
        if (!strcmp(processName.c_str(), entry.szExeFile)) {
            this->id = entry.th32ProcessID;
            this->process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, this->id);
            break;
        }
    }

    // Fecha o snapshot se estiver aberto
    if (snapshot) {
        CloseHandle(snapshot);
    }
};

// Destrutor: Libera recursos quando o objeto � destru�do
Memory::~Memory() {
    // Fecha o handle do processo se estiver aberto
    if (this->process) CloseHandle(this->process);
};

// Retorna o ID do processo
DWORD Memory::GetProcessId() {
    return this->id;
};

// Retorna o handle do processo
HANDLE Memory::GetProcessHandle() {
    return this->process;
};

// Obt�m o endere�o base de um m�dulo pelo nome
uintptr_t Memory::GetModuleAddress(const std::string moduleName) {
    // Inicializa a estrutura de dados para obter informa��es sobre m�dulos
    MODULEENTRY32 entry;
    entry.dwSize = sizeof(MODULEENTRY32);

    // Cria um snapshot dos m�dulos do processo
    const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, this->id);

    uintptr_t res = 0;

    // Itera sobre os m�dulos para encontrar o m�dulo desejado pelo nome
    while (Module32Next(snapshot, &entry)) {
        if (!strcmp(moduleName.c_str(), entry.szModule)) {
            res = reinterpret_cast<uintptr_t>(entry.modBaseAddr);
            break;
        }
    }

    // Fecha o snapshot se estiver aberto
    if (snapshot) {
        CloseHandle(snapshot);
    }
    return res;
};

// Encontra o endere�o final ap�s seguir uma s�rie de offsets em um ponteiro
uintptr_t Memory::FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets) {
    uintptr_t addr = ptr;
    for (unsigned int i = 0; i < offsets.size(); ++i) {
        // L� o pr�ximo n�vel do ponteiro e ajusta o endere�o
        ReadProcessMemory(this->process, (BYTE*)addr, &addr, sizeof(addr), 0);
        addr += offsets[i];
    }
    return addr;
}

// Verifica se um endere�o de mem�ria � v�lido
bool Memory::isValidAddress(uintptr_t address) {
    // Verifica se o conte�do no endere�o � igual a 3435973836 (hex: CCCCCCCC)
    if (Read<uint32_t>(address) == 3435973836) {
        return false;
    }
    return true;
}

// Verifica se um endere�o cont�m uma entidade v�lida
bool Memory::isValidEntity(uint32_t address) {
    // Verifica se o conte�do no endere�o � igual a 0x0054D07C (Vtable something?)
    if (Read<uint32_t>(address) == 0x0054D07C) {
        return true;
    }
    return false;
}

#include <iostream>
#include "memtool.h"

int main()
{
    DWORD processID = GetProcessId(L"ac_client.exe");

    std::cout << processID << std::endl;
    std::cout << "----" << std::endl;

    uintptr_t base_address = GetModuleBaseAddress(processID, L"ac_client.exe");

    std::cout << "0x" << std::hex << base_address << std::endl;
    std::cout << "sizeof(base_address) = "
              << sizeof(base_address) << std::endl;

    std::cout << "----" << std::endl;
    // get handle
    HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);

    std::cout << processHandle << std::endl;

    std::cout << "----" << std::endl;
    uintptr_t player_address = base_address + 0x0018AC00;

    std::cout << "0x" << std::hex << player_address << std::endl;
    std::cout << "sizeof(player_address) = "
              << sizeof(player_address) << std::endl;
    std::cout << "----" << std::endl;

    uintptr_t health_address = GetDynamicAddress(processHandle, player_address, {0xEC});
    std::cout << "health_address = "
              << "0x" << std::hex << health_address << std::endl;
    std::cout << "----" << std::endl;
    short healthValue = 0;
    ReadProcessMemory(processHandle, (BYTE *)health_address, &healthValue, sizeof(WORD), nullptr);
    std::cout << "health = "
              << std::dec << healthValue << std::endl;

    // healthValue = 9999;
    // WriteProcessMemory(processHandle, (BYTE *)health_address, &healthValue, sizeof(WORD), nullptr);
    // std::cout << "health = "
    //           << std::dec << healthValue << std::endl;

    // close handle
    CloseHandle(processHandle);
    std::cin.get();

    return 0;
}

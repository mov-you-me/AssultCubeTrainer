#include <iostream>
#include "memtool.h"

int main()
{
    DWORD proc_id = GetProcessId(L"ac_client.exe");

    if (proc_id == 0)
    {
        std::cout << "Process not found" << std::endl;
        return 0;
    }
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, proc_id);
    // Get the module base address of the process
    uintptr_t module_base_address = GetModuleBaseAddress(proc_id, L"ac_client.exe");

    // Get address of player base
    uintptr_t player_base_address = module_base_address + 0x0018AC00;

    BOOL is_Recoil = FALSE;

    DWORD exit_code;
    while (GetExitCodeProcess(hProcess, &exit_code) && exit_code == STILL_ACTIVE)
    {

        // health
        if (GetAsyncKeyState(0x35) & 1)
        {

            std::cout << "change health!" << std::endl;
            uintptr_t health_address = GetDynamicAddress(hProcess, player_base_address, {0xEC});
            short health = 9999;
            WriteProcessMemory(hProcess, (BYTE *)health_address, &health, sizeof(health), nullptr);
        }
        // ammo
        if (GetAsyncKeyState(0x36) & 1)
        {
            std::cout << "change ammo!" << std::endl;
            uintptr_t ammo_address = GetDynamicAddress(hProcess, player_base_address, {0x364, 0x14, 0x0});
            int ammo = 9999;
            WriteProcessMemory(hProcess, (BYTE *)ammo_address, &ammo, sizeof(ammo), nullptr);
        }
        // recoil
        if (GetAsyncKeyState(0x37) & 1)
        {
            std::cout << "Recoil" << is_Recoil << std::endl;
            is_Recoil = !is_Recoil;
            if (is_Recoil)
            {
                // recoil z velocity off
                NopEx((BYTE *)(module_base_address + 0xC8DD2), 5, hProcess);

                // recoil x velocity off
                NopEx((BYTE *)(module_base_address + 0xC8DFB), 5, hProcess);

                // recoil weapon spread off
                PatchEx((BYTE *)(module_base_address + 0xC8C51), (BYTE *)"\xE9\x9A\x00\x00\x00\x90", 6, hProcess);

                // recoil view y pitch off
                NopEx((BYTE *)(module_base_address + 0xC8E52), 5, hProcess);
            }
            else
            {
                // recoil z velocity on
                PatchEx((BYTE *)(module_base_address + 0xC8DD2), (BYTE *)"\xF3\x0F\x11\x40\x10", 5, hProcess);
                // recoil x velocity on
                PatchEx((BYTE *)(module_base_address + 0xC8DFB), (BYTE *)"\xF3\x0F\x11\x40\x14", 5, hProcess);
                // recoil weapon spread on
                PatchEx((BYTE *)(module_base_address + 0xC8C51), (BYTE *)"\x0F\x8E\x99\x00\x00\x00", 6, hProcess);
                // recoil view y pitch on
                PatchEx((BYTE *)(module_base_address + 0xC8E52), (BYTE *)"\xF3\x0F\x11\x50\x40", 5, hProcess);
            }
        }

        Sleep(10);
    }
    // close handle
    CloseHandle(hProcess);
    return 0;
}
// dll main function
#include <windows.h>
#include <iostream>
#include <TlHelp32.h>
#include "memtool.h"

DWORD WINAPI HackThread(HINSTANCE hinstDLL)
{
    AllocConsole();
    FILE *f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    std::cout << "YEEE!" << std::endl;

    // Get the module base address of the process
    uintptr_t module_base_address = (uintptr_t)GetModuleHandle(L"ac_client.exe");

    // Get address of player base
    uintptr_t player_base_address = module_base_address + 0x0018AC00;

    BOOL is_Recoil = FALSE;

    DWORD exit_code;
    while (true)
    {
        // health
        if (GetAsyncKeyState(0x35) & 1)
        {

            std::cout << "change health!" << std::endl;
            uintptr_t health_address = GetDynamicAddress(player_base_address, {0xEC});
            *(short *)health_address = 9999;
        }
        // ammo
        if (GetAsyncKeyState(0x36) & 1)
        {
            std::cout << "change ammo!" << std::endl;
            uintptr_t ammo_address = GetDynamicAddress(player_base_address, {0x364, 0x14, 0x0});
            *(int *)ammo_address = 9999;
        }
        // recoil
        if (GetAsyncKeyState(0x37) & 1)
        {
            std::cout << "Recoil" << is_Recoil << std::endl;
            is_Recoil = !is_Recoil;
            if (is_Recoil)
            {
                // recoil z velocity off
                Nop((BYTE *)(module_base_address + 0xC8DD2), 5);

                // recoil x velocity off
                Nop((BYTE *)(module_base_address + 0xC8DFB), 5);

                // recoil weapon spread off
                Patch((BYTE *)(module_base_address + 0xC8C51), (BYTE *)"\xE9\x9A\x00\x00\x00\x90", 6);

                // recoil view y pitch off
                Nop((BYTE *)(module_base_address + 0xC8E52), 5);
            }
            else
            {
                // recoil z velocity on
                Patch((BYTE *)(module_base_address + 0xC8DD2), (BYTE *)"\xF3\x0F\x11\x40\x10", 5);
                // recoil x velocity on
                Patch((BYTE *)(module_base_address + 0xC8DFB), (BYTE *)"\xF3\x0F\x11\x40\x14", 5);
                // recoil weapon spread on
                Patch((BYTE *)(module_base_address + 0xC8C51), (BYTE *)"\x0F\x8E\x99\x00\x00\x00", 6);
                // recoil view y pitch on
                Patch((BYTE *)(module_base_address + 0xC8E52), (BYTE *)"\xF3\x0F\x11\x50\x40", 5);
            }
        }

        Sleep(10);
    }
    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hinstDLL, 0);
    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD ul_reason_for_call, LPVOID lpReserved)
{

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hinstDLL, 0, nullptr));
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
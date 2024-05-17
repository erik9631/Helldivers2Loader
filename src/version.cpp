#include "proxies/version.h"

#include "GameGuardPatcher.h"
#include <cstdio>
#include <thread>
#include <Windows.h>


void InitConsole() {
    FILE* pFile = nullptr;
    AllocConsole();
    freopen_s(&pFile, "CONOUT$", "w", stdout);
}

void DestroyConsole()
{
    DestroyWindow(GetConsoleWindow());
    FreeConsole();
}


BOOL WINAPI DllMain(HMODULE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch (fdwReason){
        case DLL_PROCESS_ATTACH: {
            DisableThreadLibraryCalls(hinstDLL); // The dll is in the app permanently, some apps send DLL_PROCESS_DETACH and other crap to kill us
            InitConsole();
            auto* patch = new byteMender::datatypes::Patch{sizeof(helldiversLoader::gameGuardPatch), helldiversLoader::gameGuardPatch};
            std::thread(helldiversLoader::PatchGameGuard, patch).detach();
            break;
        }

    }
    return TRUE;
}
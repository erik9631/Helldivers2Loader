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

            unsigned char* imageBase = reinterpret_cast<unsigned char*>(GetModuleHandle(NULL));
            printf("Image base at: %p\n", imageBase);

            auto* patches = new helldiversLoader::datatypes::TargetedPatch[2];
            patches[0].targetAddr = imageBase + 0x8D8B10; // offset 0x8D8B10 abs: 0x1408D8B10
            patches[0].patch.patchAddr = const_cast<unsigned char*>(helldiversLoader::gameGuardPatch);
            patches[0].patch.size = 6;

            patches[1].targetAddr = imageBase + 0x8D9000; // offset 0x8D9000 abs: 0x1408D9000
            patches[1].patch.patchAddr = const_cast<unsigned char*>(helldiversLoader::gameGuardPatch);
            patches[1].patch.size = 6;
            std::thread(helldiversLoader::PatchGameGuard, patches, 2).detach();
            break;
        }

    }
    return TRUE;
}
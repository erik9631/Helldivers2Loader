//
// Created by erik9 on 5/12/2024.
//
#include "GameGuardPatcher.h"
#include <Windows.h>
#include <tlhelp32.h>
#include <iostream>


void helldiversLoader::SuspendAllThreads() {
    HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
    THREADENTRY32 te32;

    // Take a snapshot of all running threads
    hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hThreadSnap == INVALID_HANDLE_VALUE) return;

    te32.dwSize = sizeof(THREADENTRY32);

    // Retrieve information about the first thread,
    // and exit if unsuccessful
    if (!Thread32First(hThreadSnap, &te32)) {
        CloseHandle(hThreadSnap);
        return;
    }

    // Now walk the thread list of the system,
    // and suspend each thread
    do {
        if (te32.th32OwnerProcessID == GetCurrentProcessId()) {
            if (te32.th32ThreadID != GetCurrentThreadId()) {
                HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te32.th32ThreadID);
                if (hThread != NULL) {
                    SuspendThread(hThread);
                    CloseHandle(hThread);
                }
            }
        }
    } while (Thread32Next(hThreadSnap, &te32));

    CloseHandle(hThreadSnap);
}

void helldiversLoader::ResumeAllThreads() {
    HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
    THREADENTRY32 te32;

    // Take a snapshot of all running threads
    hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hThreadSnap == INVALID_HANDLE_VALUE) return;

    te32.dwSize = sizeof(THREADENTRY32);

    // Walk the thread snapshot to resume them all, except the calling one.
    if (Thread32First(hThreadSnap, &te32)) {
        do {
            if (te32.th32OwnerProcessID == GetCurrentProcessId()) {
                if (te32.th32ThreadID != GetCurrentThreadId()) {
                    HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te32.th32ThreadID);
                    if (hThread != NULL) {
                        ResumeThread(hThread);
                        CloseHandle(hThread);
                    }
                }
            }
        } while (Thread32Next(hThreadSnap, &te32));
    }
}

void helldiversLoader::MonitorAddr(const volatile unsigned char *addr) {
    auto startValue = *addr;
    while (*addr == startValue) {
    }
    std::cout << "Detected write at: " << std::hex << (void*)addr <<" from " << (int)startValue << " to " << (int)*addr << std::dec <<std::endl;
}


//TODO Search for a pattern1 in the binnary instead: 48 8B 0D ? ? ? ? 48 85 C9 75 ? 33 C0 C3 E9 ? ? ? ? CC CC CC CC CC CC CC CC CC CC CC CC 48 83 EC ? E8 ? ? ? ?
//TODO Search for a pattern2 in the binnary instead: 48 8B 0D ? ? ? ? 48 85 C9 75 ? 33 C0 C3 E9 ? ? ? ? CC CC CC CC CC CC CC CC CC CC CC CC 48 83 EC ? 48 8B 0D ? ? ? ?
// Do it as soon as the DLL is loaded, find the address of this pattern from the exe
// Then wait till it is unpacked and patch it based on the addresses that we retrieved.
void helldiversLoader::PatchGameGuard(const datatypes::TargetedPatch *patches, int count) {
    auto largestAddrPatch = patches[count - 1];
    MonitorAddr(largestAddrPatch.targetAddr + largestAddrPatch.patch.size - 1);

    SuspendAllThreads();
    std::cout << "Suspended all threads, starting patch..." << std::endl;
    for (int i = 0; i < count; i++) {
        DWORD oldProtect;
        VirtualProtect(patches[i].targetAddr, patches[i].patch.size, PAGE_EXECUTE_READWRITE, &oldProtect);

        memcpy(patches[i].targetAddr, patches[i].patch.patchAddr, patches[i].patch.size);
        printf("Patch %p applied\n", patches[i].targetAddr);

        VirtualProtect(patches[i].targetAddr, patches[i].patch.size, oldProtect, &oldProtect);
    }
    ResumeAllThreads();
    std::cout << "Patches successful, resumed all threads" << std::endl;
    delete[] patches;
}
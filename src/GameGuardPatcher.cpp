//
// Created by erik9 on 5/12/2024.
//
#include "GameGuardPatcher.h"
#include <Windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <algorithms/KmpSearcher.h>
#include <utils/Utils.h>

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

size_t GetImageBaseSize(HMODULE hModule) {
    // Get the base address of the DOS header
    PIMAGE_DOS_HEADER pDosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(hModule);
    if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
        // Not a valid PE file
        return 0;
    }

    // Get the address of the NT headers
    PIMAGE_NT_HEADERS pNtHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(
        reinterpret_cast<char*>(hModule) + pDosHeader->e_lfanew);
    if (pNtHeaders->Signature != IMAGE_NT_SIGNATURE) {
        // Not a valid PE file
        return 0;
    }

    // Get the size of the image
    return pNtHeaders->OptionalHeader.SizeOfImage;
}

DWORD GetFirstSectionMappedSize(HMODULE hModule) {
    // Get the base address of the DOS header
    PIMAGE_DOS_HEADER pDosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(hModule);
    if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
        std::cerr << "Not a valid PE file." << std::endl;
        return 0;
    }

    // Get the address of the NT headers
    PIMAGE_NT_HEADERS pNtHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(
        reinterpret_cast<char*>(hModule) + pDosHeader->e_lfanew);
    if (pNtHeaders->Signature != IMAGE_NT_SIGNATURE) {
        std::cerr << "Not a valid PE file." << std::endl;
        return 0;
    }

    // Get the section alignment from the optional header
    DWORD sectionAlignment = pNtHeaders->OptionalHeader.SectionAlignment;

    // Get the pointer to the first section header
    PIMAGE_SECTION_HEADER pFirstSectionHeader = IMAGE_FIRST_SECTION(pNtHeaders);

    // Get the Virtual Address and Virtual Size of the first section
    DWORD firstSectionVA = pFirstSectionHeader->VirtualAddress;
    DWORD firstSectionVS = pFirstSectionHeader->Misc.VirtualSize;

    // Calculate the aligned size
    DWORD alignedFirstSectionVS = (firstSectionVS + sectionAlignment - 1) & ~(sectionAlignment - 1);

    return alignedFirstSectionVS;
}

static const unsigned char pattern1[] = {
    0x48, 0x8B, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x48, 0x85, 0xC9, 0x75, 0x00,
    0x33, 0xC0, 0xC3, 0xE9, 0x00, 0x00, 0x00, 0x00, 0xCC, 0xCC, 0xCC, 0xCC,
    0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x48, 0x83, 0xEC, 0x00, 0xE8,
    0x00, 0x00, 0x00, 0x00
};

static const unsigned char pattern2[] = {
    0x48, 0x8B, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x48, 0x85, 0xC9, 0x75, 0x00,
    0x33, 0xC0, 0xC3, 0xE9, 0x00, 0x00, 0x00, 0x00, 0xCC, 0xCC, 0xCC, 0xCC,
    0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x48, 0x83, 0xEC, 0x00, 0x48, 0x8B, 0x0D, 0x00, 0x00, 0x00, 0x00
};

//TODO Search for a pattern1 in the binnary instead: 48 8B 0D ? ? ? ? 48 85 C9 75 ? 33 C0 C3 E9 ? ? ? ? CC CC CC CC CC CC CC CC CC CC CC CC 48 83 EC ? E8 ? ? ? ?
//TODO Search for a pattern2 in the binnary instead: 48 8B 0D ? ? ? ? 48 85 C9 75 ? 33 C0 C3 E9 ? ? ? ? CC CC CC CC CC CC CC CC CC CC CC CC 48 83 EC ? 48 8B 0D ? ? ? ?
// Do it as soon as the DLL is loaded, find the address of this pattern from the exe
// Then wait till it is unpacked and patch it based on the addresses that we retrieved.
void helldiversLoader::PatchGameGuard(const byteMender::datatypes::Patch *patch) {
    auto imageBase = reinterpret_cast<unsigned char*>(GetModuleHandle(nullptr));
    auto textSectionEnd = imageBase + GetFirstSectionMappedSize(GetModuleHandle(nullptr));
    std::cout << "Text section end: " <<(void*)textSectionEnd << std::endl;

    MonitorAddr(textSectionEnd);

    SuspendAllThreads();
    byteMender::algorithms::KmpSearcher searcher1{pattern1, byteMender::utils::ArraySize(pattern1)};
    byteMender::algorithms::KmpSearcher searcher2{pattern2, byteMender::utils::ArraySize(pattern2)};
    auto results1 = searcher1.Search(imageBase, textSectionEnd - imageBase);
    auto results2 = searcher2.Search(imageBase, textSectionEnd - imageBase);

    results2.insert(results2.end(), results1.begin(), results1.end());
    std::cout << "Found " << results2.size() << " matches" << std::endl;
    for (auto& result : results2)
        std::cout << "Match at: " << (void*)result.data << std::endl;

    std::cout << "Suspended all threads, starting patch..." << std::endl;
    for (auto& result : results2) {
        DWORD oldProtect;
        std::cout << "Patching " << (void*)result.data << " " <<patch->size <<std::endl;
        VirtualProtect(result.data, patch->size, PAGE_EXECUTE_READWRITE, &oldProtect);

        memcpy(result.data, patch->patchAddr, patch->size);
        printf("Patch %p applied\n", result.data);

        VirtualProtect(result.data, patch->size, oldProtect, &oldProtect);
    }
    ResumeAllThreads();
    std::cout << "Patches successful, resumed all threads" << std::endl;
}
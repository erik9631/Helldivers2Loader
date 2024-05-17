//
// Created by erik9 on 5/12/2024.
//
#include "GameGuardPatcher.h"
#include <Windows.h>
#include "Utils.h"
#include <tlhelp32.h>
#include <iostream>
#include <algorithms/KmpSearcher.h>
#include <utils/Utils.h>


void helldiversLoader::MonitorAddr(const volatile unsigned char *addr) {
    auto startValue = *addr;
    while (*addr == startValue) {
    }
    std::cout << "Detected write at: " << std::hex << (void*)addr <<" from " << (int)startValue << " to " << (int)*addr << std::dec <<std::endl;
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


void helldiversLoader::PatchGameGuard(const byteMender::datatypes::Patch *patch) {
    auto module = GetModuleHandle(nullptr);
    auto firstSectionAlignedSize = helldiversLoader::utils::GetFirstSectionMappedSize(module);
    auto imageBase = reinterpret_cast<unsigned char*>(module);
    auto firstSecionAddr =  imageBase + firstSectionAlignedSize;

    std::cout << "Text section end: " <<(void*)firstSecionAddr << std::endl;

    MonitorAddr(firstSecionAddr);

    byteMender::utils::SuspendAllThreads();
    byteMender::algorithms::KmpSearcher searcher1{pattern1, byteMender::utils::ArraySize(pattern1)};
    byteMender::algorithms::KmpSearcher searcher2{pattern2, byteMender::utils::ArraySize(pattern2)};
    auto results1 = searcher1.Search(imageBase, firstSecionAddr - imageBase);
    auto results2 = searcher2.Search(imageBase, firstSecionAddr - imageBase);

    results2.insert(results2.end(), results1.begin(), results1.end());
    std::cout << "Found " << results2.size() << " matches" << std::endl;

    std::cout << "Suspended all threads, starting patch..." << std::endl;
    for (auto& result : results2) {
        DWORD oldProtect;

        VirtualProtect(result.data, patch->size, PAGE_EXECUTE_READWRITE, &oldProtect);
        memcpy(result.data, patch->patchAddr, patch->size);
        printf("Patch %p applied\n", result.data);

        VirtualProtect(result.data, patch->size, oldProtect, &oldProtect);
    }
    byteMender::utils::ResumeAllThreads();
    std::cout << "Patches successful, resumed all threads" << std::endl;
    delete patch;
}
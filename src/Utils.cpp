//
// Created by erik9 on 5/17/2024.
//

#include "Utils.h"
#include <iostream>
size_t helldiversLoader::utils::GetImageBaseSize(HMODULE hModule) {
    // Get the base address of the DOS header
    auto pDosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(hModule);
    if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
        // Not a valid PE file
        return 0;
    }

    // Get the address of the NT headers
    auto pNtHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(
        reinterpret_cast<char*>(hModule) + pDosHeader->e_lfanew);
    if (pNtHeaders->Signature != IMAGE_NT_SIGNATURE) {
        // Not a valid PE file
        return 0;
    }

    // Get the size of the image
    return pNtHeaders->OptionalHeader.SizeOfImage;
}


DWORD helldiversLoader::utils::GetFirstSectionMappedSize(HMODULE hModule) {
    auto pDosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(hModule);
    if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
        std::cerr << "Not a valid PE file." << std::endl;
        return 0;
    }

    auto pNtHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(
        reinterpret_cast<char*>(hModule) + pDosHeader->e_lfanew);
    if (pNtHeaders->Signature != IMAGE_NT_SIGNATURE) {
        std::cerr << "Not a valid PE file." << std::endl;
        return 0;
    }

    DWORD sectionAlignment = pNtHeaders->OptionalHeader.SectionAlignment;

    PIMAGE_SECTION_HEADER pFirstSectionHeader = IMAGE_FIRST_SECTION(pNtHeaders);

    DWORD firstSectionSize = pFirstSectionHeader->Misc.VirtualSize;

    // Round up to the closest multiple larger than the firstSectionSize
    DWORD alignedFirstSectionVS = (firstSectionSize + sectionAlignment - 1) & ~(sectionAlignment - 1);

    return alignedFirstSectionVS;
}

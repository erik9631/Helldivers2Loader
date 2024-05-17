//
// Created by erik9 on 5/17/2024.
//

#pragma once

#include <windows.h>

namespace helldiversLoader::utils {
    DWORD GetFirstSectionMappedSize(HMODULE hModule);
    size_t GetImageBaseSize(HMODULE hModule);
}


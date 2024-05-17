//
// Created by erik9 on 5/17/2024.
//

#ifndef HELLDIVERS_LOADER_UTILS_H
#define HELLDIVERS_LOADER_UTILS_H
#include <windows.h>

namespace helldiversLoader::utils {
    DWORD GetFirstSectionMappedSize(HMODULE hModule);
    size_t GetImageBaseSize(HMODULE hModule);
}
#endif //HELLDIVERS_LOADER_UTILS_H

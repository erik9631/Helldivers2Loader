//
// Created by erik9 on 4/28/2024.
//

#pragma once

#include <memory>
#include <string>

namespace byteMender::datatypes {
    struct Patch {
        unsigned char* patchAddr = nullptr;
        size_t size = 0;
        explicit Patch(size_t size);
        explicit Patch(size_t size, const unsigned char* patchAddr);
        ~Patch();
    };

    Patch LoadPatch(const std::wstring& path);


}






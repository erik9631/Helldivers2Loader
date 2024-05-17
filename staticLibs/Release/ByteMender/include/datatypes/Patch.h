//
// Created by erik9 on 4/28/2024.
//

#ifndef PATCH_H
#define PATCH_H
#include <memory>
#include <string>

namespace byteMender::datatypes {
    struct Patch {
        unsigned char* patchAddr = nullptr;
        size_t size = 0;
        explicit Patch(size_t size);
        ~Patch();
    };

    Patch LoadPatch(const std::wstring& path);


}




#endif //PATCH_H

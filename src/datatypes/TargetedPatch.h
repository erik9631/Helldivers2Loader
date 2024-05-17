//
// Created by erik9 on 5/12/2024.
//

#ifndef TARGETEDPATCH_H
#define TARGETEDPATCH_H
#include <datatypes/Patch.h>
namespace helldiversLoader::datatypes {
    struct TargetedPatch {
        unsigned char* targetAddr = nullptr;
        byteMender::datatypes::Patch patch{0};
    };
}


#endif //TARGETEDPATCH_H

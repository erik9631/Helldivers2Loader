//
// Created by erik9 on 4/28/2024.
//

#pragma once

#include "datatypes/Patch.h"

namespace byteMender::memory {
    unsigned char *CreateDetour(unsigned char *startAddress, unsigned char *newAddress);

    /// Last 4 or 8 bytes of the patch should be 0x00 and will be replaced with the return address
    void PatchMemory(unsigned char *address, const datatypes::Patch& patch, long long continueAddrOffset = 0, bool relative = true);
}




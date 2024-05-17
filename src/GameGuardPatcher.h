//
// Created by erik9 on 5/12/2024.
//

#pragma once

#include <datatypes/Patch.h>

namespace helldiversLoader {
    constexpr unsigned char gameGuardPatch[] = {0xb8, 0x55, 0x07, 0x00, 0x00, 0xc3};
    void MonitorAddr(const volatile unsigned char* addr);
    void PatchGameGuard(const byteMender::datatypes::Patch *patch);
}


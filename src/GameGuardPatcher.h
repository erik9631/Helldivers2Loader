//
// Created by erik9 on 5/12/2024.
//

#ifndef GAMEGUARDPATCHER_H
#define GAMEGUARDPATCHER_H
#include "datatypes/TargetedPatch.h"

namespace helldiversLoader {
    constexpr unsigned char gameGuardPatch[] = {0xb8, 0x55, 0x07, 0x00, 0x00, 0xc3};
    void SuspendAllThreads();
    void ResumeAllThreads();

    void MonitorAddr(const volatile unsigned char* addr);
    void PatchGameGuard(const byteMender::datatypes::Patch *patch);
}
#endif //GAMEGUARDPATCHER_H

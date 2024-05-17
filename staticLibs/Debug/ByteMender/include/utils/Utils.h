//
// Created by erik9 on 5/7/2024.
//

#ifndef UTILS_H
#define UTILS_H
#include <functional>
#include <windows.h>

namespace byteMender::utils {
 template <typename T, size_t N>
    constexpr size_t ArraySize(T (&)[N]) { return N; }
    void IterateThreads(const std::function<void(HANDLE)>& threadAction);
    void SuspendAllThreads();
    void ResumeAllThreads();
}


#endif //UTILS_H

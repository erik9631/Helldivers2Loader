//
// Created by erik9 on 5/6/2024.
//

#ifndef KMPRESULT_H
#define KMPRESULT_H

namespace byteMender::algorithms {

template <typename T>
struct KmpResult {
    T* data;
    size_t offset;
};

} // algorithms

#endif //KMPRESULT_H

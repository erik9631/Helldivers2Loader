//
// Created by erik9 on 4/28/2024.
//

#ifndef KMP_H
#define KMP_H
#include <future>

#include "KmpResult.h"
#include <vector>
#include "concurrentqueue.h"
#include <future>

namespace byteMender::algorithms{

    template <typename U>
    U GetDefaultWildCard() {
        if constexpr (std::is_same_v<U, char>) {
            return 0;
        }
        if constexpr (std::is_same_v<U, wchar_t>) {
            return 0;
        }
        if constexpr (std::is_same_v<U, char16_t>) {
            return 0;
        }
        if constexpr (std::is_same_v<U, char32_t>) {
            return 0;
        }
        if constexpr (std::is_same_v<U, unsigned char>) {
            return 0;
        }
        if constexpr (std::is_same_v<U, unsigned short>) {
            return 0;
        }
        return std::numeric_limits<U>::min();

    }

    template <typename T>
    class KmpSearcher {
        unsigned short* patternIndexTable;
        T wildCard;

        void BuildFailureTable() {
            const T* lastMatch = pattern;
            const T* pivot = pattern + 1; // We don't compare the first with the first as it is always a match
            const T* patternEnd = pattern + patternSize;
            unsigned short* patternIndexTablePtr = patternIndexTable + 1;

            while(pivot != patternEnd) {

                if ( (*pivot != *lastMatch) && (*pivot != wildCard))
                    lastMatch = pattern;
                else {
                    ++lastMatch;
                    *patternIndexTablePtr = *(patternIndexTablePtr - 1) + 1;
                }
                ++patternIndexTablePtr;
                ++pivot;

            }
        }
        void SearchTask(const T* data, size_t size, concurrentQueue::ConcurrentQueue<KmpResult<T>>* results){
            const T* originalData = data;
            const T* patternPivot = pattern;
            const T* patternEnd = pattern + patternSize;
            const T* dataEnd = data + size;
            int failureTableOffset = 0;
            while(data != dataEnd) {
                if (*data == *patternPivot || *patternPivot == wildCard){
                    ++data;
                    ++patternPivot;
                    ++failureTableOffset;

                    if (patternPivot == patternEnd) {

                        results->enqueue({const_cast<T*>(data - patternSize), static_cast<size_t>(data - originalData)});
                        patternPivot = pattern;
                    }
                    continue;
                }
                if (failureTableOffset) {
                    patternPivot = pattern + patternIndexTable[failureTableOffset -1];
                    failureTableOffset = patternIndexTable[failureTableOffset -1];
                    continue;
                }
                ++data;

            }
        }

    public:
        const T* pattern = nullptr;
        const size_t& patternSize;
        explicit KmpSearcher(const T* pattern, const size_t& patternSize, T wildCard = GetDefaultWildCard<T>()):
            pattern(pattern),
            patternSize(patternSize),
            wildCard(wildCard)
        {
            patternIndexTable = new unsigned short[this->patternSize]();
            BuildFailureTable();
        }

        [[nodiscard]] unsigned short const* GetPatternIndexTable() const {
            return patternIndexTable;
        }

        concurrentQueue::ConcurrentQueue<KmpResult<T>> ParallelSearch(const T* data, size_t size) {
            size_t maxThreads = std::thread::hardware_concurrency();
            auto chunkSize = std::div(static_cast<long long>(size), maxThreads);
            auto* dataEnd = data + size;
            concurrentQueue::ConcurrentQueue<KmpResult<T>> results;
            std::vector<std::future<void>> tasks;
            tasks.reserve(maxThreads);

            for (size_t i = 0; i < maxThreads - 1; i++) {
                auto future = std::async(std::launch::async, &KmpSearcher::SearchTask, this, data, chunkSize.quot, &results);
                tasks.push_back(std::move(future));
                data += chunkSize.quot - patternSize;
            }

            // Last chunk
            auto lastChunkSize = chunkSize.quot + chunkSize.rem + (patternSize*(maxThreads-1));
            tasks.push_back(std::async(std::launch::async, &KmpSearcher::SearchTask, this, data, lastChunkSize, &results));


            for (auto& task : tasks) {
                task.get();
            }

            return results;
        }


        std::vector<KmpResult<T>> Search(const T* data, size_t size) {
            const T* originalData = data;
            const T* patternPivot = pattern;
            const T* patternEnd = pattern + patternSize;
            const T* dataEnd = data + size;
            std::vector<KmpResult<T>> results;
            int failureTableOffset = 0;
            while(data != dataEnd) {
                if (*data == *patternPivot || *patternPivot == wildCard) {
                    ++data;
                    ++patternPivot;
                    ++failureTableOffset;

                    if (patternPivot == patternEnd) {

                        results.push_back({const_cast<T*>(data - patternSize), static_cast<size_t>(data - originalData)});
                        patternPivot = pattern;
                    }
                    continue;
                }
                if (failureTableOffset) {
                    patternPivot = pattern + patternIndexTable[failureTableOffset -1];
                    failureTableOffset = patternIndexTable[failureTableOffset -1];
                    continue;
                }
                ++data;

            }
            return results;
        }
    };

}

#endif //KMP_H

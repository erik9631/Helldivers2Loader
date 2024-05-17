//
// Created by erik9 on 5/16/2024.
//

#pragma once

#include <windows.h>
#include <tlhelp32.h>
#include <set>

struct HeapEntryCompare {
    bool operator()(const HEAPENTRY32& lhs, const HEAPENTRY32& rhs) const {
        return lhs.dwAddress < rhs.dwAddress;
    }
};

std::set<HEAPENTRY32, HeapEntryCompare> GetHeapList(HANDLE targetProcess = nullptr);



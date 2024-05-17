//
// Created by erik9 on 5/7/2024.
//

#pragma once

#include <string>
#include <chrono>
#include <iostream>
namespace byteMender::utils {
    class Profiler
    {
        std::chrono::time_point<std::chrono::high_resolution_clock> start;
        std::chrono::time_point<std::chrono::high_resolution_clock> finish;
        std::string label;
    public:
        explicit Profiler(const std::string label) : start{std::chrono::high_resolution_clock::now()}, label{label} {};
        void print() {
            finish = std::chrono::high_resolution_clock::now();
            auto elapsed_ms = std::chrono::duration_cast<std::chrono::duration<double>>(finish - start).count() * 1000;
            std::cout << label << "=" << elapsed_ms << "ms" << std::endl;
        }

        ~Profiler() { print(); }
    };
}



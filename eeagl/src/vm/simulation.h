#pragma once
#include <iostream>
#include "memory/memory.h"
namespace eeagl {
    namespace vm {

        class Simulation {
        public:
            static const int InfiniteNumberOfSteps = -1;

            Simulation(memory::Memory& memory);

            struct RunResult {
                int stepsPassed;
            };

            RunResult run(long steps = InfiniteNumberOfSteps);
            void stop();
        private:
            memory::Memory& memory;
        };
    }
}
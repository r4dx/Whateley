#include <iostream>
#include "gtest/gtest.h"

#include <memory>
#include <sstream>
#include "vm/memory/memory.h"
#include "vm/memory/memory_dump.h"
#include "vm/simulation.h"
#include "memory/memory_dump_test.h"

namespace eeagl::vm {
    class SimulationTest : public ::testing::Test {
    protected:
        virtual void SetUp() override {
            auto dump = memory::MemoryDumpTest::createSimpleMemoryDump();
            memory = std::make_shared<memory::Memory>(dump);
        }
        std::shared_ptr<memory::Memory> memory;
    };


    TEST_F(SimulationTest, RunZeroSteps) {
        auto simulation = Simulation(*memory);
        auto result = simulation.run(0);
        EXPECT_EQ(result.stepsPassed, 0);
    }
}
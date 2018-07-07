#pragma once
#include <iostream>
#include "memory/memory.h"
namespace eeagl {
	namespace vm {
		class Simulation {
			static const int InfiniteNumberOfSteps = -1;

			Simulation(memory::Memory& memory);

			void run(long steps = InfiniteNumberOfSteps);
			void stop();
			void save(std::ostream& os);
		};
	}
}
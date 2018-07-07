#include "simulation.h"

namespace eeagl {
	namespace vm {
		Simulation::Simulation(memory::Memory& memory) : memory(memory) {
		}

		Simulation::RunResult Simulation::run(long steps) {
			Simulation::RunResult result;
			result.stepsPassed = 0;
			return result;
		}

		void Simulation::stop() {
		}

	}
}


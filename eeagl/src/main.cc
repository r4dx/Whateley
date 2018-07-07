#include <iostream>

int main() {
    /* Usage:
      Command line arguments:
        --random - generate random simulation
        --width <0-256> - memory width
        --height <0-256> - memory height
        --file <path> - load simulation from file
        --run - Automatically run in background mode (saves simulation to ./simulation/current.<int> on interrupt)

      Interactive (default):
        run
        stop
        save <path>

          struct GeneratorParameters {
            int xDimension; 
            int yDimension;
          }

          class MemoryDumpGenerator {
            MemoryDump generateRandom(const GeneratorParameters parameters);
          }

          class Simulation {
            Simulation(Memory& memory);
            
            void run(long steps = Simulation::InfiniteNumberOfSteps);
            void stop();
            void save(std::ostream& os);
          }
        
    */
    std::cin.get();
}
#include "aggregator.h"
#include <iostream>
#include <cstdlib>   // atoi(), srand()
#include <ctime>     // time()

int main(int argc, char* argv[]) {
    int window_size = 4096; // default, must be >= 4096 per assignment
    int run_seconds = 10;   // default runtime

    if (argc >= 2) {
        window_size = std::atoi(argv[1]);
        if (window_size < 4096) {
            std::cerr << "Warning: window_size < 4096, clamping to 4096." << std::endl;
            window_size = 4096;
        }
    }

    if (argc >= 3) {
        run_seconds = std::atoi(argv[2]);
        if (run_seconds <= 0) {
            std::cerr << "Warning: run_seconds must be > 0, using 10." << std::endl;
            run_seconds = 10;
        }
    }

    std::cout << "Starting fusion pipeline: window=" << window_size
              << "  duration=" << run_seconds << "s" << std::endl;

    srand(static_cast<unsigned int>(time(NULL)));

    Aggregator aggregator(window_size, run_seconds);
    aggregator.run();

    return 0;
}

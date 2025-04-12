#pragma once

#include <atomic>
#include <csignal>
#include <iostream>

namespace SignalHandler {

    inline std::atomic<bool> interrupted{false};

    inline void handleSignal(int signum) {
        if (signum == SIGINT) {
            interrupted = true;
            std::cerr << "\nOperation interrupted by user (SIGINT)" << std::endl;
        }
    }
    
    inline void setupSignalHandler() {
        std::signal(SIGINT, handleSignal);
    }

}

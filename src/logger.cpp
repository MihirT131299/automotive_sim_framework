#include "logger.h"
#include <iostream>

void Logger::open(std::string filename) {
    log_file.open(filename);
    if (!log_file.is_open()) {
        std::cerr << "Error opening log file: " << filename << "\n";
    } else {
        std::cout << "Log file opened successfully\n";
    }
}

void Logger::writeHeader() {
    if (log_file.is_open()) {
        log_file << "Time (s)," << "Velocity (m/s)" << "\n";
    }
}

void Logger::writeRow(double t_current, double v_current) {
    if (log_file.is_open()) {
        log_file << t_current << "," << v_current << "\n";
    }
}

void Logger::close() {
    if (log_file.is_open()) {
        log_file.close();
    }
}
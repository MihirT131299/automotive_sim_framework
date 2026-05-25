#include "signal_bus.h"

bool SignalBus::registerWriter(const std::string& signal_name, const std::string& writer_identity) {
    if (signals.count(signal_name)) {
        if (signals[signal_name].writer != writer_identity) {
            std::cerr << "Signal conflict: '" << signal_name << "' already owned by '"
                      << signals[signal_name].writer << "', rejected registration from '"
                      << writer_identity << "'\n";
            return false;
        }
    }
    signals[signal_name].writer = writer_identity;
    return true;
}

void SignalBus::write(const std::string& signal_name, double value, const std::string& writer_identity) {
    if (!signals.count(signal_name)) {
        std::cerr << "Write failed: signal '" << signal_name << "' not registered\n";
        return;
    }
    if (signals[signal_name].writer != writer_identity) {
        std::cerr << "Write failed: '" << writer_identity << "' is not the owner of '" << signal_name << "'\n";
        return;
    }
    signals[signal_name].value = value;
}

double SignalBus::read(const std::string& signal_name) const {
    if (!signals.count(signal_name)) {
        std::cerr << "Read failed: signal '" << signal_name << "' not found on bus\n";
        return 0.0;
    }
    return signals.at(signal_name).value;
}

bool SignalBus::has(const std::string& signal_name) const {
    return signals.count(signal_name) > 0;
}

void SignalBus::initializeSignal(const std::string& signal_name, double default_value, const std::string& writer_identity) {
    signals[signal_name] = {default_value, writer_identity};
}
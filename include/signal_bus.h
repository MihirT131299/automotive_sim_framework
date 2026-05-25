#ifndef SIGNAL_BUS_H
#define SIGNAL_BUS_H

#include <string>
#include <map>
#include <iostream>

struct SignalEntry {
    double value;
    std::string writer;
};

class SignalBus {
private:
    std::map<std::string, SignalEntry> signals;

public:
    bool registerWriter(const std::string& signal_name, const std::string& writer_identity);
    void write(const std::string& signal_name, double value, const std::string& writer_identity);
    double read(const std::string& signal_name) const;
    bool has(const std::string& signal_name) const;
    void initializeSignal(const std::string& signal_name, double default_value, const std::string& writer_identity);
};

#endif
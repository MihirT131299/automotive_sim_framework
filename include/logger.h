#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>

class Logger {
private:
    std::ofstream log_file;    
    
public:

    void open(std::string filename);
    void writeHeader();
    void writeRow(double t_current, double v_current);
    void close();
    
};


#endif
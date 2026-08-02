#ifndef SCORING_H
#define SCORING_H

#include <string>
#include <vector>

namespace scoring {

struct TimeSeries {
    std::vector<double> time;
    std::vector<double> value;
};

// Parses a CSV, extracting the named time and value columns.
// Throws std::runtime_error if the file can't be opened or a column is missing.
TimeSeries readCsv(const std::string& path,
                   const std::string& time_column,
                   const std::string& value_column);

// Root mean square error between reference and simulation.
// Simulation is linearly interpolated onto the reference time base.
// Throws std::runtime_error if a reference time falls outside the simulation range.
double rmse(const TimeSeries& reference, const TimeSeries& simulation);

// Pearson correlation coefficient between reference and simulation.
// Simulation is linearly interpolated onto the reference time base.
double correlation(const TimeSeries& reference, const TimeSeries& simulation);

}  // namespace scoring

#endif
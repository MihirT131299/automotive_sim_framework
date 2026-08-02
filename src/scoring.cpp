#include "scoring.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cmath>

namespace scoring {

namespace {

// Splits a single CSV line into trimmed fields.
std::vector<std::string> splitLine(const std::string& line) {
    std::vector<std::string> fields;
    std::stringstream ss(line);
    std::string field;
    while (std::getline(ss, field, ',')) {
        // Trim leading/trailing whitespace
        size_t start = field.find_first_not_of(" \t\r\n");
        size_t end = field.find_last_not_of(" \t\r\n");
        if (start == std::string::npos) {
            fields.push_back("");
        } else {
            fields.push_back(field.substr(start, end - start + 1));
        }
    }
    return fields;
}

// Linearly interpolates a TimeSeries to a target time.
// Throws if target_time is outside the series' range.
double interpolate(const TimeSeries& series, double target_time) {
    const auto& t = series.time;
    const auto& v = series.value;

    if (target_time < t.front() || target_time > t.back()) {
        throw std::runtime_error(
            "Interpolation target time outside simulation range — simulation is incomplete");
    }

    // Find the bracketing interval [t[i], t[i+1]]
    for (size_t i = 0; i < t.size() - 1; ++i) {
        if (target_time >= t[i] && target_time <= t[i + 1]) {
            double span = t[i + 1] - t[i];
            if (span == 0.0) {
                return v[i];
            }
            double frac = (target_time - t[i]) / span;
            return v[i] + frac * (v[i + 1] - v[i]);
        }
    }

    // Should be unreachable given the range check above
    throw std::runtime_error("Interpolation failed to find bracketing interval");
}

}  // anonymous namespace

TimeSeries readCsv(const std::string& path,
                   const std::string& time_column,
                   const std::string& value_column) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open CSV file: " + path);
    }

    std::string header_line;
    if (!std::getline(file, header_line)) {
        throw std::runtime_error("CSV file is empty: " + path);
    }

    std::vector<std::string> headers = splitLine(header_line);

    // Resolve column indices by name
    int time_idx = -1;
    int value_idx = -1;
    for (size_t i = 0; i < headers.size(); ++i) {
        if (headers[i] == time_column) time_idx = static_cast<int>(i);
        if (headers[i] == value_column) value_idx = static_cast<int>(i);
    }

    if (time_idx == -1) {
        throw std::runtime_error("Time column '" + time_column + "' not found in " + path);
    }
    if (value_idx == -1) {
        throw std::runtime_error("Value column '" + value_column + "' not found in " + path);
    }

    TimeSeries series;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::vector<std::string> fields = splitLine(line);
        if (static_cast<int>(fields.size()) <= time_idx ||
            static_cast<int>(fields.size()) <= value_idx) {
            throw std::runtime_error("Malformed data row in " + path + ": " + line);
        }
        series.time.push_back(std::stod(fields[time_idx]));
        series.value.push_back(std::stod(fields[value_idx]));
    }

    return series;
}

double rmse(const TimeSeries& reference, const TimeSeries& simulation) {
    double sum_squared_error = 0.0;
    size_t n = reference.time.size();

    for (size_t i = 0; i < n; ++i) {
        double sim_value = interpolate(simulation, reference.time[i]);
        double error = reference.value[i] - sim_value;
        sum_squared_error += error * error;
    }

    return std::sqrt(sum_squared_error / n);
}

double correlation(const TimeSeries& reference, const TimeSeries& simulation) {
    size_t n = reference.time.size();

    // Build the interpolated simulation series aligned to reference time points
    std::vector<double> sim_aligned(n);
    for (size_t i = 0; i < n; ++i) {
        sim_aligned[i] = interpolate(simulation, reference.time[i]);
    }

    // Means
    double mean_ref = 0.0;
    double mean_sim = 0.0;
    for (size_t i = 0; i < n; ++i) {
        mean_ref += reference.value[i];
        mean_sim += sim_aligned[i];
    }
    mean_ref /= n;
    mean_sim /= n;

    // Pearson correlation
    double cov = 0.0;
    double var_ref = 0.0;
    double var_sim = 0.0;
    for (size_t i = 0; i < n; ++i) {
        double dr = reference.value[i] - mean_ref;
        double ds = sim_aligned[i] - mean_sim;
        cov += dr * ds;
        var_ref += dr * dr;
        var_sim += ds * ds;
    }

    double denom = std::sqrt(var_ref * var_sim);
    if (denom == 0.0) {
        return 0.0;  // undefined correlation (flat signal) — report as zero
    }
    return cov / denom;
}

}  // namespace scoring
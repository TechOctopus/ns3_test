/**
 * @file time_utils.hpp
 * @brief Utility functions for time-related operations
 */

#ifndef TIME_UTILS_HPP
#define TIME_UTILS_HPP

#include <chrono>
#include <ctime>
#include <string>

namespace vanetza_ns3 {
namespace utils {

/**
 * @brief Get current timestamp in milliseconds
 * @return Current timestamp in milliseconds
 */
inline uint64_t getCurrentTimeMillis() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

/**
 * @brief Format timestamp as ISO 8601 string
 * @param timestamp Timestamp in milliseconds
 * @return Formatted timestamp string
 */
inline std::string formatTimestamp(uint64_t timestamp) {
    time_t time = timestamp / 1000;
    char buffer[30];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S", std::localtime(&time));
    return std::string(buffer);
}

} // namespace utils
} // namespace vanetza_ns3

#endif // TIME_UTILS_HPP
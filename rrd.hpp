#pragma once

#include <ctime>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

namespace rrd
{

/**
 * Abstract interface for an object that provide librrd functionality.
 */
class RrdInterface
{
  public:
    virtual ~RrdInterface() = default;

    // librrd interface, 1:1 mapping to rrd_*_r functions
    virtual time_t last(const std::string& filename) const = 0;
    virtual std::tuple<time_t, std::vector<std::pair<std::string, std::string>>>
        lastupdate(const std::string& filename) const = 0;
};

/**
 * Concrete implementation for the RRD interface.
 */
class Rrd : public RrdInterface
{
  public:
    // librrd interface, 1:1 mapping to rrd_*_r functions
    time_t last(const std::string& filename) const override;
    std::tuple<time_t, std::vector<std::pair<std::string, std::string>>>
        lastupdate(const std::string& filename) const override;
};

/**
 * Global instance to access RRD interface without the need to instantiate the
 * concrete class. Useful as default when allowing dependency injection.
 */
extern const Rrd rrd;

} // namespace rrd

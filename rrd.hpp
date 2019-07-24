#pragma once

#include <ctime>
#include <string>

namespace rrd
{

class RrdInterface
{
  public:
    virtual ~RrdInterface() = default;

    virtual time_t last(const std::string& filename) const = 0;
};

class Rrd : public RrdInterface
{
  public:
    time_t last(const std::string& filename) const override;
};

extern const Rrd rrd;

} // namespace rrd

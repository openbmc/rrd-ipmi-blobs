#include "rrd.hpp"

#include <rrd.h>

namespace rrd
{

const Rrd rrd;

time_t Rrd::last(const std::string& filename) const
{
    return rrd_last_r(filename.c_str());
}

} // namespace rrd

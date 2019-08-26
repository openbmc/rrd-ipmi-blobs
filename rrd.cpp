#include "rrd.hpp"

#include <rrd.h>

namespace rrd
{

const Rrd rrd;

/**
 * Check the last time an RRD file was updated.
 *
 * @param[in] filename - File to query.
 *
 * @return UNIX Timestamp.
 */
time_t Rrd::last(const std::string& filename) const
{
    return rrd_last_r(filename.c_str());
}

} // namespace rrd

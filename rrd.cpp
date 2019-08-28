#include "rrd.hpp"

#include <rrd.h>

#include <stdexcept>

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

std::tuple<time_t, std::vector<std::pair<std::string, std::string>>>
    Rrd::lastupdate(const std::string& filename) const
{
    time_t time;
    unsigned long count;
    char **ds_names, **last_ds;
    if (rrd_lastupdate_r(filename.c_str(), &time, &count, &ds_names, &last_ds))
        throw std::runtime_error(rrd_get_error());

    std::vector<std::pair<std::string, std::string>> data_sources;
    data_sources.reserve(count);

    for (size_t i = 0; i < count; ++i)
    {
        data_sources.emplace_back(ds_names[i], last_ds[i]);
        free(ds_names[i]);
        free(last_ds[i]);
    }

    free(ds_names);
    free(last_ds);
    return {time, data_sources};
}

} // namespace rrd

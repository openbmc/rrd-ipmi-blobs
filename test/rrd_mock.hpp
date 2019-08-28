#pragma once

#include "rrd.hpp"

#include <gmock/gmock.h>

namespace rrd
{

class RrdMock : public RrdInterface
{
  public:
    MOCK_CONST_METHOD1(last, time_t(const std::string& filename));
    MOCK_CONST_METHOD1(
        lastupdate,
        std::tuple<time_t, std::vector<std::pair<std::string, std::string>>>(
            const std::string& filename));
};

} // namespace rrd

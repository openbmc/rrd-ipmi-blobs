#pragma once

#include "rrd_mock.hpp"
#include "service.hpp"

#include <gtest/gtest.h>

using rrd::RrdMock;
using rrd::RrdService;

class RrdServiceTest : public ::testing::Test
{
  protected:
    RrdMock rrd;
    RrdService service{&rrd};

    rrd::Request req;
    rrd::Response res;
    std::string reqBuffer;
    std::string resBuffer;
};

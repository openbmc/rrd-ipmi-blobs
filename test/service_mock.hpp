#pragma once

#include "service.hpp"

#include <gmock/gmock.h>

namespace rrd
{

class RrdServiceMock : public RrdServiceInterface
{
  public:
    MOCK_METHOD2(handle, bool(const std::string& req, std::string& res));
    MOCK_METHOD2(handle, bool(const Request& req, Response& res));

    MOCK_METHOD2(last, bool(const LastRequest& req, LastResponse& res));
};

} // namespace rrd

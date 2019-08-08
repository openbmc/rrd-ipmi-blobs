#pragma once

#include "rrd.hpp"

#include <string>

#include "rrd.pb.h"

namespace rrd
{

class RrdServiceInterface
{
  public:
    virtual ~RrdServiceInterface() = default;

    virtual bool handle(const std::string& req, std::string& res) = 0;
    virtual bool handle(const Request& req, Response& res) = 0;

    virtual bool last(const LastRequest& req, LastResponse& res) = 0;
};

class RrdService : public RrdServiceInterface
{
  public:
    explicit RrdService(const RrdInterface* rrd = &rrd::rrd) : rrd_(rrd)
    {
    }

    bool handle(const std::string& req, std::string& res) override;
    bool handle(const Request& req, Response& res) override;

    bool last(const LastRequest& req, LastResponse& res) override;

  private:
    const RrdInterface* const rrd_;
};

} // namespace rrd

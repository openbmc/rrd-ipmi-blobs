#pragma once

#include "rrd.hpp"

#include <string>

#include "rrd.pb.h"

namespace rrd
{

/**
 * Abstract interface for an object capable of taking RRD requests, processing
 * them, and then return the results by updating a given response object.
 */
class RrdServiceInterface
{
  public:
    virtual ~RrdServiceInterface() = default;

    // General handlers, should delegate to relevant methods
    virtual bool handle(const std::string& req, std::string& res) = 0;
    virtual bool handle(const Request& req, Response& res) = 0;

    // RRD interface, 1:1 mapping to librrd methods
    virtual bool last(const LastRequest& req, LastResponse& res) = 0;
};

/**
 * Concrete implementation for the RRD Service interface. It uses dependency
 * injection to be able to mock the underlying RRD interface.
 */
class RrdService : public RrdServiceInterface
{
  public:
    explicit RrdService(const RrdInterface* rrd = &rrd::rrd) : rrd_(rrd)
    {
    }

    // General handlers, should delegate to relevant methods
    bool handle(const std::string& req, std::string& res) override;
    bool handle(const Request& req, Response& res) override;

    // RRD interface, 1:1 mapping to librrd methods
    bool last(const LastRequest& req, LastResponse& res) override;

  private:
    const RrdInterface* const rrd_;
};

} // namespace rrd

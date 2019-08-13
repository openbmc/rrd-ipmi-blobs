#pragma once

#include "handler.hpp"
#include "service_mock.hpp"

#include <gtest/gtest.h>

using blobs::RrdBlobHandler;
using rrd::RrdServiceMock;

class RrdBlobHandlerTest : public ::testing::Test
{
  protected:
    std::shared_ptr<RrdServiceMock> service =
        std::make_shared<RrdServiceMock>();
    RrdBlobHandler handler{service};

    const std::string& blobId = RrdBlobHandler::blobId;
    const uint16_t openFlags = RrdBlobHandler::requiredOpenFlags;
    const uint16_t sess = 1337;
};

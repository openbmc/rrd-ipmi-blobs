#pragma once

#include "handler.hpp"

#include <gtest/gtest.h>

using blobs::RrdBlobHandler;

class RrdBlobHandlerTest : public ::testing::Test
{
  protected:
    RrdBlobHandler handler;
    const std::string& blobId = RrdBlobHandler::blobId;
    const uint16_t openFlags = RrdBlobHandler::requiredOpenFlags;
    const uint16_t sess = 1337;
};

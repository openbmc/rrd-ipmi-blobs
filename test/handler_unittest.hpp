#pragma once

#include "handler.hpp"

#include <gtest/gtest.h>

using blobs::RrdBlobHandler;

class RrdBlobHandlerTest : public ::testing::Test
{
  protected:
    RrdBlobHandler handler;
    const std::string& blobId = RrdBlobHandler::blobId;
};

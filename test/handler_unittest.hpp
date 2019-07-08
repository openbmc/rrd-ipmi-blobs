#pragma once

#include "handler.hpp"

#include <gtest/gtest.h>

class RrdBlobHandlerTest : public ::testing::Test
{
  protected:
    blobs::RrdBlobHandler handler;
};

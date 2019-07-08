#include "handler_unittest.hpp"

TEST_F(RrdBlobHandlerTest, CanHandleBlob)
{
    EXPECT_FALSE(handler.canHandleBlob("asdf")) << "Reject invalid name";
}

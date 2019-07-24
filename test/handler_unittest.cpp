#include "handler_unittest.hpp"

using namespace std::string_literals;

TEST_F(RrdBlobHandlerTest, CanHandleBlob)
{
    // Test CanHandleBlob to only accept valid path names

    EXPECT_TRUE(handler.canHandleBlob(blobId)) << "Accept valid name";
    EXPECT_FALSE(handler.canHandleBlob("asdf")) << "Reject invalid name";
    EXPECT_FALSE(handler.canHandleBlob(blobId + "\0no"s)) << "Handle null";
}

TEST_F(RrdBlobHandlerTest, GetBlobIds)
{
    // Test getBlobIds to only report handler's blob id

    auto blobIds = handler.getBlobIds();
    ASSERT_EQ(blobIds.size(), 1) << "Only one well-known blob id";
    EXPECT_EQ(blobIds[0], blobId) << "Blob id matches constant";
}

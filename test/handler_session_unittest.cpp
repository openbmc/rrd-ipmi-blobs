#include "handler_unittest.hpp"

using blobs::OpenFlags;

class RrdBlobHandlerSessionTest : public RrdBlobHandlerTest
{
  protected:
    const uint16_t maxSessions = RrdBlobHandler::maxSessions;

    const uint16_t r = OpenFlags::read;
    const uint16_t w = OpenFlags::write;
};

TEST_F(RrdBlobHandlerSessionTest, OpenCloseValid)
{
    // Test open and close of a single session

    ASSERT_TRUE(handler.open(sess, openFlags, blobId)) << "Open new";
    EXPECT_TRUE(handler.close(sess)) << "Close existing";
}

TEST_F(RrdBlobHandlerSessionTest, OpenCloseInvalid)
{
    // Test open and close error checking

    EXPECT_FALSE(handler.open(sess, openFlags, "")) << "Reject invalid path";

    EXPECT_FALSE(handler.open(sess, 0, blobId)) << "Reject no flags";
    EXPECT_FALSE(handler.open(sess, w, blobId)) << "Reject no read flag";
    EXPECT_FALSE(handler.open(sess, r, blobId)) << "Reject no write flag";

    EXPECT_FALSE(handler.close(sess)) << "Close unexisting";
    ASSERT_TRUE(handler.open(sess, openFlags, blobId)) << "Open preexisting";
    EXPECT_FALSE(handler.open(sess, openFlags, blobId)) << "Reject duplicate";
}

TEST_F(RrdBlobHandlerSessionTest, OpenCloseMultiple)
{
    // Test open and close multi-session support / tracking

    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < maxSessions; ++j)
        {
            EXPECT_TRUE(handler.open(j, openFlags, blobId))
                << "Open session #" << j;
        }

        EXPECT_FALSE(handler.open(maxSessions, openFlags, blobId))
            << "Enforce max sessions";

        for (int j = 0; j < maxSessions; ++j)
        {
            EXPECT_TRUE(handler.close(j)) << "Close session #" << j;
        }
    }
}

TEST_F(RrdBlobHandlerSessionTest, ExpireAsClose)
{
    // Test expire to have the same side effects as close

    EXPECT_FALSE(handler.expire(sess)) << "Expire unexisting";
    ASSERT_TRUE(handler.open(sess, openFlags, blobId)) << "Open new";
    EXPECT_TRUE(handler.expire(sess)) << "Expire existing";
    EXPECT_FALSE(handler.close(sess)) << "Close expired";
}

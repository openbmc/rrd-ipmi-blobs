#include "handler_unittest.hpp"

#include <vector>

class RrdBlobHandlerBufferTest : public RrdBlobHandlerTest
{
  protected:
    const std::vector<uint8_t> data = {0x2, 0x3, 0x0, 0x1, 0x1};

    void SetUp()
    {
        ASSERT_TRUE(handler.open(sess, openFlags, blobId));
    }

    void TearDown()
    {
        ASSERT_TRUE(handler.close(sess));
    }
};

// NOTE: All of the tests below use the handler write and read interface, since
// the parameters might not always be clear from the tests, here they are:
//
//    bool write(session, offset, data);
//    vector<uint8_t> read(session, offset, requestedSize);
//

TEST_F(RrdBlobHandlerBufferTest, WriteReadValid)
{
    // Test write and read in a single session

    const int chunks = 3;

    for (int i = 0; i < chunks; ++i)
    {
        EXPECT_TRUE(handler.write(sess, i * data.size(), data))
            << "Write chunk #" << i;
    }

    for (int i = 0; i < chunks; ++i)
    {
        EXPECT_EQ(handler.read(sess, i * data.size(), data.size()), data)
            << "Read and verify chunk #" << i;
    }

    EXPECT_EQ(handler.read(sess, chunks * data.size(), 1).size(), 0)
        << "Buffer did not grow past data";
}

TEST_F(RrdBlobHandlerBufferTest, WriteReadBoundaryCheck)
{
    // Test write and read checks at or past the buffer boundary
    //
    // Write: First must be at the beginning (offset = 0) and the rest must
    //        start at or before the end of the buffer.
    // Read: Must start within the buffer and will be trimmed once the end is
    //       reached (otherwise there is an empty return).

    EXPECT_FALSE(handler.write(sess, 1, data))
        << "First write not at the beginning of buffer";
    EXPECT_EQ(handler.read(sess, 0, 1).size(), 0)
        << "Read data from empty buffer";

    ASSERT_TRUE(handler.write(sess, 0, data))
        << "Write sample data at the beginning buffer";

    EXPECT_FALSE(handler.write(sess, data.size() + 1, data))
        << "Write after the end of the buffer";
    EXPECT_EQ(handler.read(sess, data.size(), 1).size(), 0)
        << "Read starting at the end of buffer";

    EXPECT_EQ(handler.read(sess, 0, data.size() + 1), data)
        << "Read more data than available in buffer";
}

TEST_F(RrdBlobHandlerBufferTest, WriteReadMultiple)
{
    // Test write and read multi-session independence

    const uint16_t sess2 = sess + 1;
    const std::vector<uint8_t> data2 = {0xB, 0xA, 0xD};

    EXPECT_TRUE(handler.open(sess2, openFlags, blobId)) << "Open session 2";

    EXPECT_TRUE(handler.write(sess, 0, data)) << "Write data";
    EXPECT_TRUE(handler.write(sess2, 0, data2)) << "Write data 2";

    EXPECT_EQ(handler.read(sess, 0, data.size()), data) << "Verify read";
    EXPECT_EQ(handler.read(sess2, 0, data2.size()), data2) << "Verify read 2";

    EXPECT_TRUE(handler.close(sess2)) << "Close session 2";
}

TEST_F(RrdBlobHandlerBufferTest, WriteReadIdempotent)
{
    // Test write and read are idempotent under duplicate calls

    for (int i = 0; i < 3; ++i)
    {
        EXPECT_TRUE(handler.write(sess, 0, data))
            << "Write data (x" << i << ")";
    }

    EXPECT_EQ(handler.read(sess, data.size(), 1).size(), 0)
        << "Buffer did not grow past data";

    for (int i = 0; i < 3; ++i)
    {
        EXPECT_EQ(handler.read(sess, 0, data.size()), data)
            << "Verify read (x" << i << ")";
    }
}

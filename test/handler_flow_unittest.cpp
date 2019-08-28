#include "handler_unittest.hpp"

using blobs::BlobMeta;
using blobs::StateFlags;
using ::testing::_;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgReferee;

class RrdBlobHandlerFlowTest : public RrdBlobHandlerTest
{
  protected:
    const std::vector<uint8_t> input = {0x2, 0x3, 0x0, 0x1, 0x1};
    const std::vector<uint8_t> output = {0xB, 0xA, 0xD, 0xB, 0xE, 0xE, 0xF};

    // Convenience to mock service input/output
    const std::string expect_in = {input.begin(), input.end()};
    const std::string give_out = {output.begin(), output.end()};

    void SetUp()
    {
        ON_CALL(*service, handle(expect_in, _))
            .WillByDefault(DoAll(SetArgReferee<1>(give_out), Return(true)));

        ASSERT_TRUE(handler.open(sess, openFlags, blobId));
    }

    void TearDown()
    {
        ASSERT_TRUE(handler.close(sess));
    }
};

TEST_F(RrdBlobHandlerFlowTest, HandleCommandFlowCheck)
{
    // Test that handler enforces required command flow.
    //
    //    WRITE -> COMMIT -> READ

    ASSERT_TRUE(handler.write(sess, 0, input)) << "Write some data";

    EXPECT_EQ(handler.read(sess, 0, 1).size(), 0) << "Read before valid commit";

    EXPECT_CALL(*service, handle(expect_in, _))
        .WillOnce(Return(false)) // Force first service call to fail
        .WillOnce(Return(true));

    EXPECT_FALSE(handler.commit(sess, {})) << "Commit invalid data";

    EXPECT_EQ(handler.read(sess, 0, 1).size(), 0) << "Read before valid commit";

    ASSERT_TRUE(handler.commit(sess, {})) << "Commit written data";

    EXPECT_FALSE(handler.write(sess, 0, output)) << "Write after valid commit";
}

TEST_F(RrdBlobHandlerFlowTest, IdempotentCommands)
{
    // Test write, commit, and read are idempotent under duplicate calls

    for (int i = 0; i < 3; ++i)
    {
        EXPECT_TRUE(handler.write(sess, 0, input))
            << "Write data (x" << i << ")";
    }

    for (int i = 0; i < 3; ++i)
    {
        // Verifies written data matches input
        EXPECT_CALL(*service, handle(expect_in, _));

        ASSERT_TRUE(handler.commit(sess, {}))
            << "Commit data with mock service (x" << i << ")";
    }

    for (int i = 0; i < 3; ++i)
    {
        EXPECT_EQ(handler.read(sess, 0, output.size()), output)
            << "Verify read (x" << i << ")";
    }
}

TEST_F(RrdBlobHandlerFlowTest, StatMetadata)
{
    // Test that stat updates metadata appropriately

    BlobMeta meta;
    using StateFlags::committed;

    EXPECT_TRUE(handler.stat(sess, &meta)) << "Stat before commit";
    EXPECT_FALSE(meta.blobState & committed) << "Blob state before commit";
    EXPECT_EQ(0, meta.size) << "Blob size before commit";

    ASSERT_TRUE(handler.write(sess, 0, input));
    ASSERT_TRUE(handler.commit(sess, {}));

    EXPECT_TRUE(handler.stat(sess, &meta)) << "Stat after commit";
    EXPECT_TRUE(meta.blobState & committed) << "Blob state after commit";
    EXPECT_EQ(output.size(), meta.size) << "Blob size after commit";
}

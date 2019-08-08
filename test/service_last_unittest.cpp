#include "service_unittest.hpp"

using ::testing::_;
using ::testing::Return;

class RrdServiceLastTest : public RrdServiceTest
{
  protected:
    rrd::LastRequest& last_req = *req.mutable_last();
    rrd::LastResponse& last_res = *res.mutable_last();

    std::string filename = "/path/to/file.rrd";
    time_t timstamp = 880689600; // UNIX timestamp
};

TEST_F(RrdServiceLastTest, LastValid)
{
    // Test valid last request invokes underlying rrd.

    last_req.set_filename(filename);
    EXPECT_CALL(rrd, last(filename)).WillOnce(Return(timstamp));

    ASSERT_TRUE(service.last(last_req, last_res));
    ASSERT_EQ(last_res.time().seconds(), timstamp);
}

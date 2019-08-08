#include "service_unittest.hpp"

using ::testing::_;
using ::testing::Return;

namespace rrd
{
class RrdServiceHandleMock : public RrdService
{
  public:
    using RrdService::RrdService;

    // Mock every method but the handle overload set.
    MOCK_METHOD2(last, bool(const LastRequest& req, LastResponse& res));
};
} // namespace rrd

class RrdServiceHandleTest : public RrdServiceTest
{
  protected:
    rrd::RrdServiceHandleMock service{&rrd};
};

// NOTE: the following test the interaction between RrdService::handle and the
// rest of the interface provided by RrdService. The handle method should always
// delegate a request to the appropriate method or signal an error.

TEST_F(RrdServiceHandleTest, HandleLast)
{
    EXPECT_CALL(service, last(_, _)).Times(2).WillRepeatedly(Return(true));

    req.mutable_last();
    ASSERT_TRUE(service.handle(req, res))
        << "Handle request with last method set";

    req.SerializeToString(&reqBuffer);
    ASSERT_TRUE(service.handle(reqBuffer, resBuffer))
        << "Handle buffer with last method set";
}

TEST_F(RrdServiceHandleTest, HandleInvalid)
{
    // Test overloaded handle with invalid input

    ASSERT_FALSE(service.handle(reqBuffer, resBuffer))
        << "Handle buffer with empty buffer";

    ASSERT_FALSE(service.handle(req, res))
        << "Handle request with no method set on request";

    req.SerializeToString(&reqBuffer);
    ASSERT_FALSE(service.handle(reqBuffer, resBuffer))
        << "Handle buffer with no method set on request";
}

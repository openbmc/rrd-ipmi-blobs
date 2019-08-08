#include "service_unittest.hpp"

TEST_F(RrdServiceTest, HandleInvalid)
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

#include "service.hpp"

namespace rrd
{

/**
 * Parse an RRD request and delegate the method switch to the overload.
 *
 * @param[in]  req - Serialized rrd:Request.
 * @param[out] res - Serialized rrd:Response if successful call
 *
 * @return Whether the call was performed.
 */
bool RrdService::handle(const std::string& req, std::string& res)
{
    Request request;

    if (!request.ParseFromString(req))
    {
        return false;
    }

    Response response;

    if (!handle(request, response))
    {
        return false;
    }

    response.SerializeToString(&res);
    return true;
}

/**
 * Determine the type of request and delegate to appropriate method.
 *
 * @param[in]  req - Request to be fullfilled.
 * @param[out] res - Response if successful call.
 *
 * @return Whether the call was performed.
 */
bool RrdService::handle(const Request& req, Response& res)
{
    switch (req.method_case())
    {
        case Request::kLast:
            return last(req.last(), *res.mutable_last());
        case Request::METHOD_NOT_SET:
            return false;
    }
    return false;
}

/**
 * Extract parameters for rrd::last call and pack response.
 *
 * @param[in]  req - Parameters for library call.
 * @param[out] res - Result if successful call.
 *
 * @return Whether the call was performed.
 */
bool RrdService::last(const LastRequest& req, LastResponse& res)
{
    auto time = rrd_->last(req.filename());
    res.mutable_time()->set_seconds(time);
    return true;
}

} // namespace rrd

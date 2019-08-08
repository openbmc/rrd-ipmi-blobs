#include "service.hpp"

namespace rrd
{

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

bool RrdService::last(const LastRequest& req, LastResponse& res)
{
    auto time = rrd_->last(req.filename());
    res.mutable_time()->set_seconds(time);
    return true;
}

} // namespace rrd

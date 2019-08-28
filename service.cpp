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
        case Request::kLastUpdate:
            return lastupdate(req.last_update(), *res.mutable_last_update());
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

bool RrdService::lastupdate(const LastUpdateRequest& req,
                            LastUpdateResponse& res)
{
    auto [time, data_sources] = rrd_->lastupdate(req.filename());
    res.mutable_time()->set_seconds(time);

    res.clear_data_sources();
    for (auto& ds : data_sources)
    {
        auto res_ds = res.add_data_sources();
        res_ds->set_name(ds.first);
        res_ds->set_last(ds.second);
    }

    return true;
}

} // namespace rrd

#include "handler.hpp"

#include <algorithm>
#include <phosphor-logging/elog.hpp>

namespace blobs
{

using namespace phosphor::logging;

const std::string RrdBlobHandler::blobId = "/rrd";

bool RrdBlobHandler::canHandleBlob(const std::string& path)
{
    return path == blobId;
}

std::vector<std::string> RrdBlobHandler::getBlobIds()
{
    return {blobId};
}

bool RrdBlobHandler::deleteBlob(const std::string& path)
{
    // TODO: implement
    return false;
}

bool RrdBlobHandler::stat(const std::string& path, struct BlobMeta* meta)
{
    // TODO: implement
    return false;
}

bool RrdBlobHandler::open(uint16_t session, uint16_t flags,
                          const std::string& path)
{
    if ((flags & requiredOpenFlags) != requiredOpenFlags)
    {
        log<level::ERR>("Read or write flag not specified when opening");
        return false;
    }

    if (sessions_.size() >= maxSessions)
    {
        log<level::ERR>("Max sessions already open",
                        entry("MAX_SESSIONS=%d", maxSessions));
        return false;
    }

    log<level::NOTICE>("Opening new session", entry("SESSION=%d", session));
    return sessions_.emplace(session, SessionState{}).second;
}

std::vector<uint8_t> RrdBlobHandler::read(uint16_t session, uint32_t offset,
                                          uint32_t requestedSize)
{
    auto it = sessions_.find(session);

    if (it == sessions_.end())
    {
        log<level::ERR>("Session not found", entry("SESSION=%d", session));
        return {};
    }

    auto& state = it->second;

    if (!state.committed)
    {
        log<level::ERR>("Request has not been committed");
        return {};
    }

    auto& buffer = state.response;

    if (offset >= buffer.size())
    {
        log<level::ERR>("Read past the end of the buffer",
                        entry("BUFFER_SIZE=%d", buffer.size()),
                        entry("REQUESTED_OFFSET=%d", offset));
        return {};
    }

    auto readBegin = buffer.begin() + offset;
    uint32_t remainingSize = buffer.end() - readBegin;

    return {readBegin, readBegin + std::min(requestedSize, remainingSize)};
}

bool RrdBlobHandler::write(uint16_t session, uint32_t offset,
                           const std::vector<uint8_t>& data)
{
    auto it = sessions_.find(session);

    if (it == sessions_.end())
    {
        log<level::ERR>("Session not found", entry("SESSION=%d", session));
        return false;
    }

    auto& state = it->second;

    if (state.committed)
    {
        log<level::ERR>("Request has already been committed");
        return false;
    }

    auto& buffer = state.request;

    if (offset > buffer.size())
    {
        log<level::ERR>("Write offset past end of buffer",
                        entry("BUFFER_SIZE=%d", buffer.size()),
                        entry("REQUESTED_OFFSET=%d", offset));
        return false;
    }

    auto requiredSize = offset + data.size();

    if (requiredSize > buffer.size())
    {
        log<level::NOTICE>("Resizing buffer to accomodate write",
                           entry("BUFFER_SIZE=%d", buffer.size()),
                           entry("REQUIRED_SIZE=%d", requiredSize));
        buffer.resize(requiredSize);
    }

    std::copy(data.begin(), data.end(), buffer.begin() + offset);
    return true;
}

bool RrdBlobHandler::writeMeta(uint16_t session, uint32_t offset,
                               const std::vector<uint8_t>& data)
{
    // TODO: implement
    return false;
}

bool RrdBlobHandler::commit(uint16_t session, const std::vector<uint8_t>& data)
{
    auto it = sessions_.find(session);

    if (it == sessions_.end())
    {
        log<level::ERR>("Session not found", entry("SESSION=%d", session));
        return false;
    }

    auto& state = it->second;

    if (!rrd_->handle(state.request, state.response))
    {
        log<level::ERR>("Unable to handle request");
        return false;
    }

    state.committed = true;
    return true;
}

bool RrdBlobHandler::close(uint16_t session)
{
    auto it = sessions_.find(session);

    if (it == sessions_.end())
    {
        log<level::ERR>("Session not found", entry("SESSION=%d", session));
        return false;
    }

    log<level::NOTICE>("Closing session", entry("SESSION=%d", session));
    sessions_.erase(it);
    return true;
}

bool RrdBlobHandler::stat(uint16_t session, struct BlobMeta* meta)
{
    // TODO: implement
    return false;
}

bool RrdBlobHandler::expire(uint16_t session)
{
    // TODO: implement
    return false;
}

} // namespace blobs

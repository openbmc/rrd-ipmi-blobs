#include "handler.hpp"

#include <algorithm>
#include <phosphor-logging/elog.hpp>

namespace blobs
{

using namespace phosphor::logging;

const std::string RrdBlobHandler::blobId = "/rrd";

/**
 * Used to check if the handler can manage a file path.
 *
 * @param[in] blobId - Path to test for.
 *
 * @return Whether the path matches the handler's blobId.
 */
bool RrdBlobHandler::canHandleBlob(const std::string& path)
{
    return path == blobId;
}

/**
 * Used to get the names of all available blobs for this handler.
 *
 * @return The blobId this handler manages.
 */
std::vector<std::string> RrdBlobHandler::getBlobIds()
{
    return {blobId};
}

/**
 * Attempt to delete the blob specified by the path.
 *
 * Always unsuccessful, the blobId represents the handler.
 *
 * @param[in] path - The blobId to try and delete.
 *
 * @return Whether it was successful.
 */
bool RrdBlobHandler::deleteBlob(const std::string& path)
{
    return false;
}

/**
 * Return metadata about a given blob.
 *
 * Always unsuccessful, handler uses session semantics.
 *
 * @param[in]     path - The blobId for metadata.
 * @param[in,out] meta - A pointer to a metadata struct.
 *
 * @return Whether it was successful.
 */
bool RrdBlobHandler::stat(const std::string& path, struct BlobMeta* meta)
{
    return false;
}

/**
 * Attempt to open a session on this path.
 *
 * @param[in] session - The session id to identify the new session.
 * @param[in] flags - Options to use to open the path.
 * @param[in] path - The blobId to open.
 *
 * @return Whether it was successful.
 */
bool RrdBlobHandler::open(uint16_t session, uint16_t flags,
                          const std::string& path)
{
    if (!canHandleBlob(path))
    {
        log<level::ERR>("Invalid path for handler");
        return false;
    }

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

    if (!sessions_.emplace(session, SessionState{}).second)
    {
        log<level::ERR>("Session is already open",
                        entry("SESSION=%d", session));
        return false;
    }

    log<level::NOTICE>("Opening new session", entry("SESSION=%d", session));
    return true;
}

/**
 * Attempt to read from a blob.
 *
 * @param[in] session - The session id.
 * @param[in] offset - Byte index to start reading at.
 * @param[in] requestedSize - Number of bytes to read.
 *
 * @return Bytes read (0 length on error).
 */
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

/**
 * Attempt to write to a blob.
 *
 * @param[in] session - The session id.
 * @param[in] offset -  Byte index to start writing at.
 * @param[in] data - The data to write.
 *
 * @return Whether it was successful.
 */
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

/**
 * Attempt to write metadata to a blob.
 *
 * Always unsuccessful, command currently unused by this handler.
 *
 * @param[in] session - The session id.
 * @param[in] offset - Offset into the blob.
 * @param[in] data - The data to write.
 *
 * @return Whether it was successful.
 */
bool RrdBlobHandler::writeMeta(uint16_t session, uint32_t offset,
                               const std::vector<uint8_t>& data)
{
    return false;
}

/**
 * Attempt to commit to a blob.
 *
 * @param[in] session - The session id.
 * @param[in] data - Optional commit data (unused).
 *
 * @return Whether it was successful.
 */
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

/**
 * Attempt to close your session.
 *
 * @param[in] session - The session id.
 *
 * @return Whether it was successful.
 */
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

/**
 * Attempt to return metadata for the session's view of the blob.
 *
 * @param[in]     session - The session id.
 * @param[in,out] meta - A pointer to a metadata struct.
 *
 * @return Whether it was successful.
 */
bool RrdBlobHandler::stat(uint16_t session, struct BlobMeta* meta)
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
        meta->blobState |= StateFlags::committed;
        meta->size = state.response.size();
    }

    // If not committed nothing will be set.
    return true;
}

/**
 * Attempt to expire a session.
 *
 * Used when a session has been inactive for at least 10 minutes.
 *
 * @param[in] session - The session id.
 *
 * @return Whether it was successful.
 */
bool RrdBlobHandler::expire(uint16_t session)
{
    return close(session);
}

} // namespace blobs

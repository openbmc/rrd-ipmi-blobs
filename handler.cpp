#include "handler.hpp"

namespace blobs
{

bool RrdBlobHandler::canHandleBlob(const std::string& path)
{
    // TODO: implement
    return false;
}

std::vector<std::string> RrdBlobHandler::getBlobIds()
{
    // TODO: implement
    std::vector<std::string> result;
    return result;
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
    // TODO: implement
    return false;
}

std::vector<uint8_t> RrdBlobHandler::read(uint16_t session, uint32_t offset,
                                          uint32_t requestedSize)
{
    // TODO: implement
    std::vector<uint8_t> result;
    return result;
}

bool RrdBlobHandler::write(uint16_t session, uint32_t offset,
                           const std::vector<uint8_t>& data)
{
    // TODO: implement
    return false;
}

bool RrdBlobHandler::writeMeta(uint16_t session, uint32_t offset,
                               const std::vector<uint8_t>& data)
{
    // TODO: implement
    return false;
}

bool RrdBlobHandler::commit(uint16_t session, const std::vector<uint8_t>& data)
{
    // TODO: implement
    return false;
}

bool RrdBlobHandler::close(uint16_t session)
{
    // TODO: implement
    return false;
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

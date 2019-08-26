#pragma once

#include "service.hpp"

#include <blobs-ipmid/blobs.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

using std::size_t;
using std::uint16_t;
using std::uint32_t;
using std::uint64_t;
using std::uint8_t;

namespace blobs
{

/**
 * Implement the IPMI Blob interface to handle RRD requests.
 *
 * This handler provides a blob interface to interact with an RRD service,
 * allowing a client to *write* a request to a temporary buffer, then *commit*
 * it to the service, and finally *read* a response back.
 */
class RrdBlobHandler : public GenericBlobInterface
{
  public:
    explicit RrdBlobHandler(std::shared_ptr<rrd::RrdServiceInterface> rrd) :
        rrd_(std::move(rrd))
    {
    }

    // Avoid expensive copies, but allow moves.
    RrdBlobHandler(const RrdBlobHandler&) = delete;
    RrdBlobHandler& operator=(const RrdBlobHandler&) = delete;
    RrdBlobHandler(RrdBlobHandler&&) = default;
    RrdBlobHandler& operator=(RrdBlobHandler&&) = default;

    // Concrete implemention of GenericBlobInterface.
    bool canHandleBlob(const std::string& path) override;
    std::vector<std::string> getBlobIds() override;
    bool deleteBlob(const std::string& path) override;
    bool stat(const std::string& path, struct BlobMeta* meta) override;
    bool open(uint16_t session, uint16_t flags,
              const std::string& path) override;
    std::vector<uint8_t> read(uint16_t session, uint32_t offset,
                              uint32_t requestedSize) override;
    bool write(uint16_t session, uint32_t offset,
               const std::vector<uint8_t>& data) override;
    bool writeMeta(uint16_t session, uint32_t offset,
                   const std::vector<uint8_t>& data) override;
    bool commit(uint16_t session, const std::vector<uint8_t>& data) override;
    bool close(uint16_t session) override;
    bool stat(uint16_t session, struct BlobMeta* meta) override;
    bool expire(uint16_t session) override;

    /**
     * Single, well-known blob id or path that can be handled.
     */
    static const std::string blobId;

    /**
     * Limit for concurrent open sessions known to the handler.
     */
    static constexpr uint16_t maxSessions = 10;

    /**
     * Bitmask for the minimum flags needed to open a blob.
     */
    static const uint16_t requiredOpenFlags =
        OpenFlags::read | OpenFlags::write;

  private:
    struct SessionState
    {
        std::string request;
        std::string response;
        bool committed;
    };

    /**
     * Map from sessionId to all state for a session.
     */
    std::unordered_map<uint16_t, SessionState> sessions_;

    /**
     * RRD service to process requests when committing.
     */
    std::shared_ptr<rrd::RrdServiceInterface> rrd_;
};

} // namespace blobs

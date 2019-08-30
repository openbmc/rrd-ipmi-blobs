#pragma once

#include <blobs-ipmid/blobs.hpp>
#include <cstdint>
#include <string>
#include <unordered_set>
#include <vector>

using std::size_t;
using std::uint16_t;
using std::uint32_t;
using std::uint64_t;
using std::uint8_t;

namespace blobs
{

class RrdBlobHandler : public GenericBlobInterface
{
  public:
    RrdBlobHandler() = default;
    ~RrdBlobHandler() = default;
    RrdBlobHandler(const RrdBlobHandler&) = delete;
    RrdBlobHandler& operator=(const RrdBlobHandler&) = delete;
    RrdBlobHandler(RrdBlobHandler&&) = default;
    RrdBlobHandler& operator=(RrdBlobHandler&&) = default;

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

    static const std::string blobId;
    static const uint16_t requiredOpenFlags =
        OpenFlags::read | OpenFlags::write;

  private:
    /* set of open sessionIds. */
    std::unordered_set<uint16_t> sessions_;
};

} // namespace blobs

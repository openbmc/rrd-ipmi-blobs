#include "handler.hpp"
#include "service.hpp"

#include <blobs-ipmid/blobs.hpp>
#include <memory>

std::unique_ptr<blobs::GenericBlobInterface> createHandler()
{
    return std::make_unique<blobs::RrdBlobHandler>(
        std::make_shared<rrd::RrdService>());
}

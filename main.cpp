#include "handler.hpp"

#include <blobs-ipmid/blobs.hpp>
#include <memory>

std::unique_ptr<blobs::GenericBlobInterface> createHandler()
{
    return std::make_unique<blobs::RrdBlobHandler>();
}

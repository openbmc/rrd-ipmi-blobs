#include "handler.hpp"
#include "service.hpp"

#include <blobs-ipmid/blobs.hpp>
#include <memory>

/**
 * Factory to create a new blob handler, IPMI Blobs library entry point.
 *
 * @return a new RRD blob handler initialized with a new RRD service.
 */
std::unique_ptr<blobs::GenericBlobInterface> createHandler()
{
    return std::make_unique<blobs::RrdBlobHandler>(
        std::make_shared<rrd::RrdService>());
}

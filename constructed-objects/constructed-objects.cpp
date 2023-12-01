#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>

struct shm_context
{
    int a {0};
    int b {0};

    boost::interprocess::interprocess_mutex mutex;
};

auto main() -> int
{
    using namespace boost::interprocess;
    const auto shm_filename = "constructed-objects";

    shared_memory_object::remove(shm_filename);
    managed_shared_memory managed_shm {create_only, shm_filename, 65536};

    auto* context = managed_shm.find_or_construct<shm_context>("context")();   // Find or construct my type <shm_context> in shared memory

    context->a = 0x55555555;
    context->b = 0xAAAAAAAA;

    context->mutex.lock();
    context->mutex.unlock();

    shared_memory_object::remove(shm_filename);
    return 0;
}

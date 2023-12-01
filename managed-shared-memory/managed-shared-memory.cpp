#include <boost/interprocess/managed_shared_memory.hpp>
#include <cassert>

auto main() -> int
{
    using namespace boost::interprocess;
    const auto shm_filename = "managed_shm";

    shared_memory_object::remove(shm_filename);

    managed_shared_memory managed_shm {create_only, shm_filename, 65536};   // Managed memory segment that allocates portions of a shared memory segment

    void* ptr = managed_shm.allocate(100);   // Allocate 100 bytes of memory from segment, throwing version
    // void* ptr = managed_shm.allocate(100, std::nothrow);   // Non throwing version

    const auto handle = managed_shm.get_handle_from_address(ptr);
    const auto* ptr_back = managed_shm.get_address_from_handle(handle);

    assert(*reinterpret_cast<int*>(ptr) == *reinterpret_cast<const int*>(ptr_back));

    managed_shm.deallocate(ptr);   // Deallocate it

    shared_memory_object::remove(shm_filename);
    return 0;
}

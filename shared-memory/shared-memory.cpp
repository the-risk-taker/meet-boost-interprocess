#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

using namespace boost::interprocess;

// (!) can be hexdump'ed
const auto shm_filename = "basic_shm";

auto run_parent(const char* child_program_name) -> int
{
    std::cout << "Starting parent process\n";
    shared_memory_object::remove(shm_filename);   // Remove shared memory on start (if some garbage left)

    shared_memory_object shm {create_only, shm_filename, read_write};   // Only create a shared memory object.
    shm.truncate(0x200);                                                // Set size
    mapped_region region {shm, read_write};                             // Map the whole shared memory in this process
    std::memset(region.get_address(), 1, region.get_size());            // Write all the memory to 1

    // Launch child process
    std::string s(child_program_name);
    s += " child ";
    if (0 != std::system(s.c_str()))
    {
        shared_memory_object::remove(shm_filename);   // Remove shared memory on exit
        return 1;
    }

    shared_memory_object::remove(shm_filename);   // Remove shared memory on exit
    return 0;
}

auto run_child() -> int
{
    std::cout << "Starting child process\n";
    shared_memory_object shm {open_only, shm_filename, read_only};   // Only open a shared memory object.
    mapped_region region(shm, read_only);                            // Map the whole shared memory in this process

    // Check that memory was initialized to 1
    char* mem = static_cast<char*>(region.get_address());
    for (std::size_t i = 0; i < region.get_size(); ++i)
    {
        if (*mem++ != 1)
        {
            return 1;   // Error checking memory
        }
    }
    return 0;
}

auto main(int argc, char* argv[]) -> int
{
    return (argc == 1) ? run_parent(argv[0]) : run_child();
}

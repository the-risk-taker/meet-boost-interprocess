#include "config.hpp"
#include "context.hpp"
#include "message.hpp"
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <iostream>

using namespace boost::interprocess;

#define LOG(x) std::cout << "[CLIENT] " << x << '\n';

auto main() -> int
{
    LOG("Started...");

    try
    {
        // (1) managed shared memory
        managed_shared_memory managed_shm {open_only, shared_memory_filename};

        // (2) construct context
        auto* context = managed_shm.find_or_construct<shm_context>(context_name)();

        // (3) create message queue
        message_queue mq {open_or_create, queue_filename, 100, sizeof(message)};
        auto send = [&](const message& msg) {
            mq.send(&msg, sizeof(message), 0);
        };

        // (4) send messages
        send({message_type::HELLO});
        send({message_type::SET_PARAMETER, 10});

        // (5) perform blocking read
        auto* ptr = managed_shm.allocate(0x1000);
        const auto handle = managed_shm.get_handle_from_address(ptr);
        send({message_type::READ_DATA, 0, true, handle});
        {
            scoped_lock lock(context->mutex);
            context->condition.wait(lock);
        }
        LOG("Got data, for example data[500] = 0x" << std::hex << static_cast<int>(*reinterpret_cast<uint8_t*>(ptr)) << std::dec);

        send({message_type::GOODBYE});
    }
    catch (const interprocess_exception& e)
    {
        LOG("Exception occurred: " << e.what());
        return 1;
    }
    catch (...)
    {
        LOG("Unnamed exception occurred\n");
        return 1;
    }

    LOG("Exited with success");
    return 0;
}

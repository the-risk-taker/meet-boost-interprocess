#include "config.hpp"
#include "context.hpp"
#include "message.hpp"
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <csignal>
#include <iostream>
#include <thread>

using namespace boost::interprocess;
using namespace boost::posix_time;

#define LOG(x) std::cout << "[SERVER] " << x << '\n';

bool run_flag = true;

auto main() -> int
{
    LOG("Started...");
    std::signal(SIGINT, [](int) { run_flag = false; });

    try
    {
        // (1) managed shared memory
        managed_shared_memory managed_shm {open_or_create, shared_memory_filename, 65536};

        // (2) construct context
        auto* context = managed_shm.find_or_construct<shm_context>(context_name)();

        // (3) create message queue
        message_queue mq {open_or_create, queue_filename, 100, sizeof(message)};

        uint8_t server_internal_data[0x1000] = {0x55};
        while (run_flag)
        {
            message message;
            message_queue::size_type received_size;
            unsigned int priority;
            const auto timeout = ptime {microsec_clock::universal_time()} + milliseconds(2500);

            // (4) check message queue
            if (mq.timed_receive(&message, sizeof(message), received_size, priority, timeout))
            {
                switch (message.type)
                {
                    case message_type::HELLO:
                        LOG("Client said HELLO");
                        break;
                    case message_type::GOODBYE:
                        LOG("Client said GOODBYE");
                        run_flag = false;
                        break;
                    case message_type::SET_PARAMETER:
                        LOG("Client SET_PARAMETER to: " << message.parameter);
                        break;
                    case message_type::READ_DATA:
                        LOG("Client requested READ_DATA");
                        if (message.has_handle)
                        {
                            // (5) handle read request
                            auto* ptr = managed_shm.get_address_from_handle(message.handle);
                            {
                                scoped_lock lock(context->mutex);
                                std::memcpy(ptr, server_internal_data, sizeof(server_internal_data));
                            }
                            context->condition.notify_one();
                        }
                        break;
                    default:
                        break;
                }
            }
            else
            {
                LOG("Waiting for request...");
            }
        }

        shared_memory_object::remove(shared_memory_filename);
        message_queue::remove(queue_filename);
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

#include "mq-message.hpp"
#include <boost/interprocess/ipc/message_queue.hpp>
#include <csignal>
#include <iostream>

using namespace boost::interprocess;
using namespace boost::posix_time;

bool run_flag = true;

auto main() -> int
{
    std::cout << "Server started\n";
    std::signal(SIGINT, [](int) { run_flag = false; });

    const auto queue_filename = "message_queue";
    message_queue::remove(queue_filename);
    message_queue mq {open_or_create, queue_filename, 100, sizeof(mq_message)};

    unsigned int priority;
    message_queue::size_type received_size;

    while (run_flag)
    {
        mq_message message;
        const auto timeout = ptime {microsec_clock::universal_time()} + milliseconds(1000);
        if (mq.timed_receive(&message, sizeof(mq_message), received_size, priority, timeout))
        {
            std::cout << "Got: {" << message.a << "," << message.b << "}\n";
        }
        else
        {
            std::cout << "Timeout\n";
        }
    }

    std::cout << "Server exited\n";
}

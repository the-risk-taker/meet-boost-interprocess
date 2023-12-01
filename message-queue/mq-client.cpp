#include "mq-message.hpp"
#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream>

using namespace boost::interprocess;

auto main() -> int
{
    std::cout << "Client started\n";

    const auto queue_filename = "message_queue";
    message_queue mq {open_or_create, queue_filename, 100, sizeof(mq_message)};

    mq_message message;
    for (int i = 0; i < 100; ++i)
    {
        message.a = i;
        mq.send(&message, sizeof(mq_message), 0);
    }

    std::cout << "Client exited\n";
}

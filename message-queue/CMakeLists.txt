add_executable(mq-client mq-client.cpp mq-message.hpp)
add_executable(mq-server mq-server.cpp mq-message.hpp)

add_custom_target(run-mq-client COMMAND $<TARGET_FILE:mq-client> USES_TERMINAL)
add_custom_target(run-mq-server COMMAND $<TARGET_FILE:mq-server> USES_TERMINAL)

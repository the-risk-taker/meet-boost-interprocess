add_executable(client client.cpp)
add_executable(server server.cpp)

add_custom_target(run-client COMMAND $<TARGET_FILE:client> USES_TERMINAL)
add_custom_target(run-server COMMAND $<TARGET_FILE:server> USES_TERMINAL)

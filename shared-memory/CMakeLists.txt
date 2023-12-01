add_executable(shared-memory shared-memory.cpp)

add_custom_target(run-shared-memory COMMAND $<TARGET_FILE:shared-memory> USES_TERMINAL)

add_executable(managed-shared-memory managed-shared-memory.cpp)

add_custom_target(run-managed-shared-memory COMMAND $<TARGET_FILE:managed-shared-memory> USES_TERMINAL)

add_executable(constructed-objects constructed-objects.cpp)

add_custom_target(run-constructed-objects COMMAND $<TARGET_FILE:constructed-objects> USES_TERMINAL)

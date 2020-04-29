set(SOURCE_FILES main.cpp)
add_executable(xlctestapp ${SOURCE_FILES})
add_executable(rc_test rc_test.cpp)

target_link_libraries(xlctestapp xlc)
target_link_libraries(rc_test xlc)
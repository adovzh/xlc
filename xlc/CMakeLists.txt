project(xlc)

set(SOURCE_FILES dummy.cpp number_utils.cpp src/remote_client.cpp)

message("system: ${CMAKE_SYSTEM_PREFIX_PATH}")
find_library(THRIFT_LIBRARY thrift)
find_file(THRIFT_COMPILER thrift )
message("thrift: ${THRIFT_LIBRARY}")
message("compiler: ${THRIFT_COMPILER}")
message("includes: ${CMAKE_PREFIX_PATH}")
message("build directory: ${CMAKE_CURRENT_BINARY_DIR}")

add_library(xlc STATIC ${SOURCE_FILES})
target_include_directories(xlc PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        /usr/local/include)
target_link_libraries(xlc INTERFACE ${THRIFT_LIBRARY})
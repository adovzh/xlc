# project(imrclient)

set(SOURCE_FILES src/imr_client.cpp)

message("system: ${CMAKE_SYSTEM_PREFIX_PATH}")
find_library(THRIFT_LIBRARY thrift)
find_file(THRIFT_COMPILER thrift )
message("thrift: ${THRIFT_LIBRARY}")
message("compiler: ${THRIFT_COMPILER}")
message("includes: ${CMAKE_PREFIX_PATH}")
message("build directory: ${CMAKE_CURRENT_BINARY_DIR}")

set(THRIFT_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/gen-cpp)

set(THRIFT_GENERATED_FILES
        ${THRIFT_OUTPUT_DIR}/imr_constants.cpp
        ${THRIFT_OUTPUT_DIR}/imr_types.cpp
        ${THRIFT_OUTPUT_DIR}/InMemoryRepo.cpp)

add_custom_command(OUTPUT generated.timestamp
        COMMAND ${THRIFT_COMPILER} -r --gen cpp:no_skeleton ${CMAKE_CURRENT_SOURCE_DIR}/protocols/imr.thrift
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating Thrift sources")


add_custom_target(thrift_compiler
        DEPENDS generated.timestamp
        COMMENT "Checking if Thrift generation is required")

add_library(imrclient STATIC generated.timestamp ${SOURCE_FILES} ${THRIFT_GENERATED_FILES})
target_include_directories(imrclient PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        /usr/local/include
        ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(imrclient PUBLIC xlc ${THRIFT_LIBRARY})
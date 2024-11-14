cmake_minimum_required(VERSION 3.10)

project(Request-queue VERSION 1.0)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

find_package(CURL REQUIRED)

add_library(request_queue STATIC request_queue.c)

target_link_libraries(request_queue CURL::libcurl)

add_executable(http_request_queue_example main.c)

target_link_libraries(http_request_queue_example request_queue CURL::libcurl)

target_include_directories(request_queue PUBLIC "${PROJECT_SOURCE_DIR}")
target_include_directories(http_request_queue_example PUBLIC "${PROJECT_SOURCE_DIR}")

install(TARGETS request_queue
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)

install(FILES request_queue.h DESTINATION include)

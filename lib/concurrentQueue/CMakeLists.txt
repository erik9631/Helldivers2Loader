project(concurrent_queue)

set(CMAKE_CXX_STANDARD 20)

set(headers
    blockingconcurrentqueue.h
    concurrentqueue.h
    lightweightsemaphore.h
)

add_library(${PROJECT_NAME}  INTERFACE ${headers})
target_include_directories(${PROJECT_NAME}  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
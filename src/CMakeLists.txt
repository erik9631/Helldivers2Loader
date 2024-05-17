set(libName version)

set(BYTEMENDER_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/staticLibs/$<CONFIG>/ByteMender/include")
set(BYTEMENDER_LIB_PATH "${CMAKE_SOURCE_DIR}/staticLibs/$<CONFIG>/ByteMender/ByteMender.lib")

set(all_headers
        GameGuardPatcher.h
        libcurl.h
        utils.h)
set(all_sources
        version.cpp
        GameGuardPatcher.cpp
        utils.cpp
)

set(all_files
    ${all_sources}
    ${all_headers}
        GameGuardPatcher.cpp
)

add_library(${libName} SHARED ${all_files})

target_link_libraries(${libName} PRIVATE ${BYTEMENDER_LIB_PATH})
target_link_libraries(${libName} PUBLIC concurrent_queue)
target_include_directories(${libName} PRIVATE ${BYTEMENDER_INCLUDE_DIR})
target_include_directories(${libName} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
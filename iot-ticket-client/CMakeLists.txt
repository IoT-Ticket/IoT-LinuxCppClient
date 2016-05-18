cmake_minimum_required(VERSION 2.6)

set(IOTAPI_MAJOR 0)
set(IOTAPI_MINOR 0)
set(IOTAPI_PATCH 1)
set(IOTAPI_VERSION "${IOTAPI_MAJOR}.${IOTAPI_MINOR}.${IOTAPI_PATCH}")

project(IOT_API)

include (jsoncpp/Files.cmake)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(IOTAPI_HEADERS 
    IOT_WriteData.h
    IOT_ReadData.h
    IOT_ReadDataFilter.h
    IOT_defines.h
    IOT_RegDevice.h
    IOT_GetDevice.h
    IOT_RestClient.h
    IOT_Base64.h
    IOT_Quota.h
    IOT_QuotaDevice.h
    IOT_API.h
)

set(IOTAPI_SOURCES
    ${JSONCPP_SRCS}
    IOT_WriteData.cpp
    IOT_ReadData.cpp
    IOT_ReadDataFilter.cpp
    IOT_RegDevice.cpp
    IOT_GetDevice.cpp
    IOT_RestClient.cpp
    IOT_Base64.cpp
    IOT_Quota.cpp
    IOT_QuotaDevice.cpp
    IOT_API.cpp
)

# Shared library
add_library(IOT_API SHARED ${IOTAPI_SOURCES})
set_target_properties(IOT_API PROPERTIES VERSION ${IOTAPI_VERSION} SOVERSION ${IOTAPI_MAJOR})
target_link_libraries(IOT_API ${Utils_LIBRARIES} curl)

install(TARGETS IOT_API
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
    
# Tests
if(BUILD_TESTS)
    include (tests/Files.cmake)
endif()

# Demoapp
if(BUILD_DEMO)
    add_executable(iot-ticket-demo demo.cpp)
    target_link_libraries(iot-ticket-demo IOT_API)

    install(TARGETS iot-ticket-demo
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
endif()

    
install(FILES ${JSONCPP_HDRS} DESTINATION include/json)
install(FILES ${IOTAPI_HEADERS} DESTINATION include/IOT_API)

configure_file(FindIOT_API.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/FindIOT_API.cmake @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/FindIOT_API.cmake DESTINATION share/cmake/modules)

cmake_minimum_required(VERSION 2.8.3)
project(odva_ethernetip)

find_package(catkin REQUIRED)

find_package(Boost 1.47 REQUIRED COMPONENTS system)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES ${PROJECT_NAME}
  DEPENDS boost
)

include_directories(
  include
  ${Boost_INCLUDE_DIRS}
)

## Declare a cpp library for just Ethernet/IP generic stuff
add_library(${PROJECT_NAME}
  src/copy_serializable.cpp
  src/encap_header.cpp
  src/encap_packet.cpp
  src/cpf_item.cpp
  src/cpf_packet.cpp
  src/identity_item_data.cpp
  src/io_scanner.cpp
  src/session.cpp
  src/path.cpp
  src/rr_data.cpp
  src/message_router_response.cpp
  src/connection.cpp
)
target_link_libraries(${PROJECT_NAME}
  ${Boost_LIBRARIES}
)

## Mark executables and libraries for installation
install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION})

if (CATKIN_ENABLE_TESTING)
  catkin_add_gtest(${PROJECT_NAME}-test
    test/serialization/buffer_writer_test.cpp
    test/serialization/buffer_reader_test.cpp
    test/serialization/stream_writer_test.cpp
    test/serialization/stream_reader_test.cpp
    test/serialization/serializable_buffer_test.cpp
    test/serialization/serializable_primitive_test.cpp
    test/test_socket_test.cpp
    test/encap_header_test.cpp
    test/encap_packet_test.cpp
    test/cpf_item_test.cpp
    test/cpf_packet_test.cpp
    test/sequenced_address_item_test.cpp
    test/identity_item_data_test.cpp
    test/path_test.cpp
    test/rr_data_test.cpp
    test/message_router_request_test.cpp
    test/message_router_response_test.cpp
    test/rr_data_request_test.cpp
    test/rr_data_response_test.cpp
    test/forward_open_request_test.cpp
    test/forward_open_success_test.cpp
    test/forward_close_request_test.cpp
    test/forward_close_success_test.cpp
    test/session_test.cpp
    test/test_main.cpp
  )
  target_link_libraries(${PROJECT_NAME}-test ${Boost_LIBRARIES} ${PROJECT_NAME})
endif()


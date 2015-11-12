cmake_minimum_required(VERSION 2.8.3)
project(os32c)

find_package(catkin REQUIRED roscpp)

find_package(Boost REQUIRED COMPONENTS system)

catkin_package(
  INCLUDE_DIRS include
  CATKIN_DEPENDS roscpp
  DEPENDS boost
)

include_directories(
  include
  ${Boost_INCLUDE_DIRS}
)

## Declare a cpp library for just Ethernet/IP generic stuff
add_library(eip
  src/eip/copy_serializable.cpp
  src/eip/encap_header.cpp
  src/eip/encap_packet.cpp
  src/eip/cpf_item.cpp
  src/eip/cpf_packet.cpp
  src/eip/identity_item_data.cpp
  src/eip/io_scanner.cpp
  src/eip/session.cpp
  src/eip/path.cpp
  src/eip/rr_data.cpp
  src/eip/message_router_response.cpp
  src/eip/connection.cpp
)
target_link_libraries(eip
  ${Boost_LIBRARIES}
)

add_library(os32c_lib src/os32c.cpp)
target_link_libraries(os32c_lib
  eip
)

## Declare a cpp executable
add_executable(scanner src/scanner_node.cpp)
target_link_libraries(scanner
  eip
  ${catkin_LIBRARIES}
  ${Boost_LIBRARIES}
)

add_executable(os32c src/os32c_node.cpp)
target_link_libraries(os32c
  eip
  os32c_lib
  ${catkin_LIBRARIES}
  ${Boost_LIBRARIES}
)

#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# install(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark executables and/or libraries for installation
# install(TARGETS os32c os32c_node
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark cpp header files for installation
# install(DIRECTORY include/${PROJECT_NAME}/
#   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )


if (CATKIN_ENABLE_TESTING)
  ## Add gtest based cpp test target and link libraries
  catkin_add_gtest(eip-test
    test/eip/serialization/buffer_writer_test.cpp
    test/eip/serialization/buffer_reader_test.cpp
    test/eip/serialization/stream_writer_test.cpp
    test/eip/serialization/stream_reader_test.cpp
    test/eip/serialization/serializable_buffer_test.cpp
    test/eip/serialization/serializable_primitive_test.cpp
    test/eip/test_socket_test.cpp
    test/eip/encap_header_test.cpp
    test/eip/encap_packet_test.cpp
    test/eip/cpf_item_test.cpp
    test/eip/cpf_packet_test.cpp
    test/eip/sequenced_address_item_test.cpp
    test/eip/identity_item_data_test.cpp
    test/eip/path_test.cpp
    test/eip/rr_data_test.cpp
    test/eip/message_router_request_test.cpp
    test/eip/message_router_response_test.cpp
    test/eip/rr_data_request_test.cpp
    test/eip/rr_data_response_test.cpp
    test/eip/forward_open_request_test.cpp
    test/eip/forward_open_success_test.cpp
    test/eip/forward_close_request_test.cpp
    test/eip/forward_close_success_test.cpp
    test/eip/session_test.cpp
    test/eip/test_main.cpp
  )
  target_link_libraries(eip-test ${Boost_LIBRARIES} eip)

  catkin_add_gtest(os32c-test
    test/os32c/measurement_report_config_test.cpp
    test/os32c/measurement_report_header_test.cpp
    test/os32c/measurement_report_test.cpp
    test/os32c/range_and_reflectance_measurement_test.cpp
    test/os32c/os32c_test.cpp
    test/os32c/test_main.cpp
  )
  target_link_libraries(os32c-test ${Boost_LIBRARIES} eip os32c_lib)
endif()


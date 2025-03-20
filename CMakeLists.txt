cmake_minimum_required(VERSION 3.10.0)
project(
  win-ntp-client
  VERSION 0.1.0
  LANGUAGES C CXX)

add_executable(win-ntp-client ntp_client.cpp main.cpp icon.rc)

add_compile_options(/utf-8)

set_target_properties(win-ntp-client PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(win-ntp-client PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                                ${CMAKE_BINARY_DIR}/bin)

target_link_libraries(win-ntp-client ws2_32)

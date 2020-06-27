cmake_minimum_required(VERSION 3.0)
project(zerodb
  VERSION 0.1
  DESCRIPTION "NoSQL database service using leveldb for storage engine and zeromq for transport"
  LANGUAGES C CXX)

add_subdirectory(apps)
add_subdirectory(extern/libzmq)
add_subdirectory(extern/czmq)
add_subdirectory(extern/leveldb)

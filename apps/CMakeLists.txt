add_executable(hello hello.cpp)
target_link_libraries(hello
  PRIVATE
  libzmq
  czmq
  leveldb)

set_target_properties(hello
    PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

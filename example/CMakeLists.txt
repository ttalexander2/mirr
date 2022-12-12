add_executable(example example.cpp)
target_compile_features(example PRIVATE cxx_std_20)

target_link_libraries(example PRIVATE mirr)

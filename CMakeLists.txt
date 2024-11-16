cmake_minimum_required(VERSION 3.22)

project(lr2 LANGUAGES C)

add_executable(lr2 src/main.c src/fifo.c src/fscan.c src/memreq.c)

set(CMAKE_C_FLAGS "-Wall -g")
set(CMAKE_CXX_FLAGS "-Wall -g")

target_include_directories(lr2 PUBLIC include/)

target_compile_options(lr2 PUBLIC -Wall -g)

target_link_libraries(lr2 m)


# cmake_minimum_required(VERSION 3.22)

# project(lr2)

# include(FetchContent)

# FetchContent_Declare(
#   googletest
#   GIT_REPOSITORY https://github.com/google/googletest.git
#   GIT_TAG        main
# )

# FetchContent_MakeAvailable(googletest)

# include(GoogleTest)

# enable_testing()

# add_executable(lr2 src/main.c test/lr2_test.cpp)

# target_include_directories(gtests_lr1 PUBLIC include)

# target_link_libraries(gtests_lr1 PUBLIC gtest_main)

# set(CMAKE_C_FLAGS "-Wall")

# target_compile_options(gtests_lr1 PRIVATE -Wall)
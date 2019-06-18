cmake_minimum_required(VERSION 3.13)
project(mapreduce)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)
find_package(LibArchive REQUIRED)

find_package(ICU REQUIRED COMPONENTS uc  i18n)
set (Boost_USE_STATIC_LIBS OFF)
set (Boost_USE_MULTITHREAD ON)
find_package(Boost REQUIRED COMPONENTS locale system filesystem thread)
set(files headers/time.h cpp/map_functions.cpp headers/map_functions.h headers/map.h)
#add_library(mapReduce STATIC ${files})
add_library(mapReduce SHARED ${files})
target_link_libraries(mapReduce Boost::system Boost::locale)
add_executable(words project.cpp)
target_link_libraries(words  mapReduce)

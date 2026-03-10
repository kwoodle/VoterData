# VoterData
# 12 Aug 2025 -D CMAKE_POLICY_VERSION_MINIMUM=3.5
# 5 Feb 2026 Needed to install MariaDB Connector/C package

cmake_minimum_required(VERSION 3.10)
project(VoterData)

set(CMAKE_CXX_STANDARD 11)

add_executable(Stats Stats.cpp Stats.h Voter.h Voter.cpp)
add_executable(Insert Insert.cpp Voter.h Voter.cpp)
include(/usr/local/lib/ktrade/cmake/ktrade-targets.cmake)

# boost
find_package(Boost CONFIG COMPONENTS system filesystem program_options date_time)

# JsonCpp
# See https://github.com/open-source-parsers/jsoncpp/issues/455

find_package(jsoncpp REQUIRED)
get_target_property(JSON_INC_PATH jsoncpp_lib INTERFACE_INCLUDE_DIRECTORIES)
include_directories(${JSON_INC_PATH})
target_link_libraries(Insert jsoncpp_lib)
target_link_libraries(Insert Ktrd)

target_link_libraries(Stats ${Boost_LIBRARIES})
target_link_libraries(Stats Ktrd)
target_link_libraries(Stats jsoncpp_lib)

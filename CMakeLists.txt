# VoterData
# Finally figured out that manually copying .cmake files to /usr/share/cmake/Modules
# (or other standard locations) is a bad idea. Use a "local" directory for nonstandard
# installations
cmake_minimum_required(VERSION 3.9)
project(VoterData)

list(APPEND CMAKE_MODULE_PATH /home/kwoodle/CLionProjects/cmake)
set(CMAKE_CXX_STANDARD 11)

add_executable(CreateJSON CreateJSON.cpp Voter.h Voter.cpp)
add_executable(CreateCD1 CreateCD1.cpp Voter.h Voter.cpp)
add_executable(Stats Stats.cpp Stats.h Voter.h Voter.cpp)
add_executable(Insert Insert.cpp Voter.h Voter.cpp)
include(/usr/local/lib/ktrade/cmake/ktrade-targets.cmake)

# boost
find_package(Boost 1.64 COMPONENTS system filesystem program_options)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(CreateJSON ${Boost_LIBRARIES})
target_link_libraries(CreateCD1 ${Boost_LIBRARIES})
target_link_libraries(Insert ${Boost_LIBRARIES})

# MySQL
set(MYSQL_EXTRA_LIBRARIES mysqlcppconn)
find_package(MySQL)
include_directories(${MYSQL_INCLUDE_DIR})
target_link_libraries(CreateJSON ${MYSQL_LIBRARIES} ${MYSQL_EXTRA_LIBRARIES})
target_link_libraries(CreateCD1 ${MYSQL_LIBRARIES} ${MYSQL_EXTRA_LIBRARIES})
target_link_libraries(Insert ${MYSQL_LIBRARIES} ${MYSQL_EXTRA_LIBRARIES})

# JsonCpp
# See https://github.com/open-source-parsers/jsoncpp/issues/455
find_package(jsoncpp REQUIRED)
get_target_property(JSON_INC_PATH jsoncpp_lib INTERFACE_INCLUDE_DIRECTORIES)
include_directories(${JSON_INC_PATH})
target_link_libraries(CreateJSON jsoncpp_lib)
target_link_libraries(Insert jsoncpp_lib)

target_link_libraries(CreateCD1 jsoncpp_lib)
target_link_libraries(CreateCD1 Ktrd)
target_link_libraries(Insert Ktrd)

target_link_libraries(Stats ${Boost_LIBRARIES})
target_link_libraries(Stats Ktrd)
target_link_libraries(Stats jsoncpp_lib)


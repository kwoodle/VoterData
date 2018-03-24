
cmake_minimum_required(VERSION 3.9)
project(VoterData)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "/usr/share/cmake/Modules")
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "../cmake")
set(CMAKE_CXX_STANDARD 11)

add_executable(CreateJSON CreateJSON.cpp Voter.h Voter.cpp)
add_executable(CreateCD1 CreateCD1.cpp Voter.h Voter.cpp)
add_executable(Stats Stats.cpp Stats.h)
add_executable(Insert Insert.cpp Voter.h Voter.cpp)
include(/usr/local/lib/ktrade/cmake/ktrade-targets.cmake)

# boost
set(BOOST_INCLUDEDIR "/usr/include")
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
find_package(JsonCpp)
include_directories(${JSONCPP_INCLUDE_DIRS})
target_link_libraries(CreateJSON ${JSONCPP_LIBRARY})
target_link_libraries(Insert ${JSONCPP_LIBRARY})

target_link_libraries(CreateCD1 ${JSONCPP_LIBRARY})
target_link_libraries(CreateCD1 Ktrd)
target_link_libraries(Insert Ktrd)

target_link_libraries(Stats ${Boost_LIBRARIES})
target_link_libraries(Stats Ktrd)
target_link_libraries(Stats ${JSONCPP_LIBRARY})


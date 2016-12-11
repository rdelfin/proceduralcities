SET(pwd ${CMAKE_CURRENT_LIST_DIR})

include_directories(${pwd}/../include)

FILE(GLOB streetsrc ${pwd}/../src/generation/street/*.cpp)

add_executable(parsetest ${pwd}/parseTest.cpp ${pwd}/../src/Area.cpp ${streetsrc})
message(STATUS "parsetest added")

target_link_libraries(parsetest ${stdgl_libraries})

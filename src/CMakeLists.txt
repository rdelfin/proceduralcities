SET(pwd ${CMAKE_CURRENT_LIST_DIR})

include_directories(${pwd}/../include)

FILE(GLOB src ${pwd}/*.cpp)
FILE(GLOB streetsrc ${pwd}/generation/street/*.cpp)
FILE(GLOB buildingsrc ${pwd}/generation/building/*.cpp)

add_executable(cities ${src} ${streetsrc} ${buildingsrc})
message(STATUS "cities added")

target_link_libraries(cities ${stdgl_libraries})

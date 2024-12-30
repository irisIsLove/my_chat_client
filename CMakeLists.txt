cmake_minimum_required(VERSION 3.15.0)
project(MyChatClient)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_compile_options("/utf-8")

set(CMAKE_AUTOMOC TRUE)
set(CMAKE_AUTOUIC TRUE)
set(CMAKE_AUTORCC TRUE)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE RESOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.qrc")

set(RC_ICONS "icon.rc")

add_executable(MyChatClient ${SOURCES} ${RESOURCES} ${RC_ICONS})

find_package(Qt5 COMPONENTS Widgets Network REQUIRED)
target_link_libraries(MyChatClient PRIVATE Qt5::Widgets Qt5::Network)

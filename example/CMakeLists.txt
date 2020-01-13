set(CMAKE_PREFIX_PATH ${QTDIR})
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(Catmull-RomSpline ${CMAKE_CURRENT_SOURCE_DIR}/Catmull-RomSpline.cpp)

target_link_libraries(Catmull-RomSpline PRIVATE Qt5::Widgets CatmullRomSpline)

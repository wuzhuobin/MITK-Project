project(PlanningSoftware)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Sql REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB_RECURSE SRC ../THA/view/PlanningSoftware/*.c*)
file(GLOB_RECURSE INC ../THA/view/PlanningSoftware/*.h*)
file(GLOB_RECURSE UI ../THA/view/PlanningSoftware/*.c*)
file(GLOB_RECURSE QRC ../THA/view/PlanningSoftware/*.qrc)

add_executable(
  ${PROJECT_NAME}
  main.cpp
  ${INC}
  ${SRC}
  ${QRC}
)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE
  Qt5::Core
  Qt5::Widgets
  Qt5::Sql
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/../THA/view/PlanningSoftware
    ${PROJECT_SOURCE_DIR}/../THA/view/PlanningSoftware/case_manager
    ${PROJECT_SOURCE_DIR}/../THA/view/PlanningSoftware/account_manager
)
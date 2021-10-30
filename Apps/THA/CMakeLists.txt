project(THA)

find_package(Qt5Sql REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(
  GLOB_RECURSE H
  *.h*
)

file(
  GLOB_RECURSE CPP
  *.c*
)

file(
  GLOB_RECURSE UI
  *.c*
)

file(
  GLOB_RECURSE QRC
  *.qrc
)

add_library(
  ${PROJECT_NAME}
  ${H}
  ${CPP}
  ${QRC}
)

target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC
  MitkCore
  MitkQtWidgets  
  MitkSceneSerialization
  MitkSegmentation
  MitkDICOMUI
  Qt5::Sql
)

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC
  ${PROJECT_SOURCE_DIR}/view
  ${PROJECT_SOURCE_DIR}/model
  ${PROJECT_SOURCE_DIR}/controller
  ${PROJECT_SOURCE_DIR}/algorithm
)

target_compile_definitions(
  ${PROJECT_NAME}
  PRIVATE
  US_MODULE_NAME=main
)
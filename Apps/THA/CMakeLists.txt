project(THA)

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

add_executable(
  ${PROJECT_NAME}
  ${H}
  ${CPP}
  ${QRC}
)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE
  MitkCore
  MitkQtWidgets
)

target_include_directories(
  ${PROJECT_NAME}
  PRIVATE
  ${PROJECT_SOURCE_DIR}/View
#   ${PROJECT_BINARY_DIR}/View
)
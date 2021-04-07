project(SpineRobotic)

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
  MitkSceneSerialization
  # MitkSegmentation
)

target_include_directories(
  ${PROJECT_NAME}
  PRIVATE
  ${PROJECT_SOURCE_DIR}/view
  ${PROJECT_SOURCE_DIR}/model
  ${PROJECT_SOURCE_DIR}/controller
  ${PROJECT_SOURCE_DIR}/algorithm
)
project(BoneSegmentation)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(
  GLOB_RECURSE INC
  ${PROJECT_SOURCE_DIR}/*.h*
)

file(
  GLOB_RECURSE SRC
  ${PROJECT_SOURCE_DIR}/*.c*
)

file(
  GLOB_RECURSE QRC
  ${PROJECT_SOURCE_DIR}/*.qrc
)

file(
  GLOB_RECURSE UI
  ${PROJECT_SOURCE_DIR}/*.ui
)
add_executable(
  ${PROJECT_NAME}
  ${INC}
  ${SRC}
  ${QRC}
  ${UI}
)

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC
  ${PROJECT_SOURCE_DIR}
)

target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC
  MitkCore
  MitkQtWidgets
  MitkSceneSerialization
  MitkBoundingShape
)
project(THA)

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

add_library(
  ${PROJECT_NAME}
  ${SRC}
  ${QRC}
  ${INC}
  ${UI}
)

target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC
  MitkCore
  MitkQtWidgets  
  MitkSceneSerialization
  MitkSegmentation
  ${VTK_LIBRARIES}
  view
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

vtk_module_autoinit(
  TARGETS 
  ${PROJECT_NAME}
  MODULES
  ${VTK_LIBRARIES}
)
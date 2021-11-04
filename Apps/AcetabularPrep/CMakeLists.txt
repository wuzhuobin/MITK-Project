project(AcetabularPrep)

# find_package(VTK REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

# include(${VTK_USE_FILE})

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# set(ABSTRACT_PANEL ${PROJECT_SOURCE_DIR}/../svpk_lib/abstract_panel.h)
file(GLOB SRC *.c*)
file(GLOB INC *.h*)
file(GLOB UI *.ui)
file(GLOB QRC *.qrc)

add_library(${PROJECT_NAME} STATIC ${SRC} ${INC} ${UI} ${QRC} ${ABSTRACT_PANEL})
target_link_libraries(
  ${PROJECT_NAME} 
  PUBLIC Qt5::Core Qt5::Widgets MitkCore MitkQtWidgets MitkSegmentation
  # MitkSegmentationUtilities
  )
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/../svpk_lib)

add_executable(${PROJECT_NAME}_main main/main.cpp)
target_link_libraries(${PROJECT_NAME}_main PRIVATE ${PROJECT_NAME})
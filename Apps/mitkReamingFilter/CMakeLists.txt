project(mitkReamingFilter)

set(
  OTHERS 
  ../THA/algorithm/mitkReamingFilter.cpp 
  ../THA/algorithm/mitkReamingFilter.h
  ../THA/algorithm/mitkReamerSource.h
  ../THA/algorithm/mitkReamerSource.cpp
)
file(GLOB SRC *.c*)
file(GLOB INC *.h*)

add_executable(${PROJECT_NAME} ${SRC} ${INC} ${OTHERS})
target_link_libraries(${PROJECT_NAME} PRIVATE MitkCore MitkSceneSerialization MitkSegmentation)
target_include_directories(${PROJECT_NAME} PRIVATE ../THA/algorithm)
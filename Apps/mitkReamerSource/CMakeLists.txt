project(mitkReamerSource)

set(OTHERS ../THA/algorithm/mitkReamerSource.cpp ../THA/algorithm/mitkReamerSource.h)
file(GLOB SRC *.c*)
file(GLOB INC *.h*)

add_executable(${PROJECT_NAME} ${SRC} ${INC} ${OTHERS})
target_link_libraries(${PROJECT_NAME} PRIVATE MitkCore)
target_include_directories(${PROJECT_NAME} PRIVATE ../THA/algorithm)
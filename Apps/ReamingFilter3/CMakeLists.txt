project(ReamingFilter3)

file(GLOB SRC ${PROJECT_SOURCE_DIR}/*.c*)
file(GLOB INC ${PROJECT_SOURCE_DIR}/*.h*)

add_executable(${PROJECT_NAME} ${SRC} ${INC})
target_link_libraries(${PROJECT_NAME} PRIVATE THA)
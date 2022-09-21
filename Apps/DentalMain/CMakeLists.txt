project(DentalMain)

file(GLOB SRC ${PROJECT_SOURCE_DIR}/*.c*)

add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} PUBLIC Dental)
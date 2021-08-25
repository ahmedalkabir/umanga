cmake_minimum_required(VERSION 3.8)
project(uManga)

set(CMAKE_C_STANDARD 11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(SOURCE_FILES src/umanga.c main.c src/ulist.c include/ulist.h src/uvector.c include/uvector.h src/ustring.c
        include/ustring.h include/ufile.h src/ufile.c include/umanga.h)

link_libraries("-luv")

add_executable(uManga ${SOURCE_FILES})

target_link_libraries(uManga curl)
target_link_libraries(uManga json-c)
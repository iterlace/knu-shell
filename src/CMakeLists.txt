file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.tpp *.cpp)
set(SOURCES ${SOURCES})

add_executable(shell_run main.cpp ${SOURCES})
add_library(shell_lib STATIC ${SOURCES})


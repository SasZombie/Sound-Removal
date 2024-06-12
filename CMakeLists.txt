cmake_minimum_required(VERSION 3.10)

project(Joculet)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wformat-nonliteral -Wcast-align -Wpointer-arith -Wmissing-declarations -Winline -Wundef -Wcast-qual -Wshadow -Wwrite-strings -Wno-unused-parameter -Wfloat-equal -pedantic -ansi -fsanitize=address -fsanitize=leak")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

# file(GLOB SOURCES "include/*.cpp")


# add_executable(main src/main.cpp ${SOURCES})
# add_executable(test src/test.cpp include/Pugilist.cpp include/Character.cpp include/Weapon.cpp include/Knight.cpp include/Mage.cpp include/Animation.cpp)

add_executable(main main.cpp ../fft/fft.cpp)
# target_link_libraries(test raylib GL m pthread dl rt X11)
target_link_libraries(main raylib GL m pthread dl rt X11)
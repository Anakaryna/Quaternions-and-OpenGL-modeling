cmake_minimum_required(VERSION 3.28)
project(Quaternions_and_OpenGL_modeling)

find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} C:/msys64/mingw64/include)

link_directories(C:/msys64/mingw64/lib)

add_executable(main main.cpp Camera.cpp Map.cpp Block.cpp
        include/Quaternion.h
        Quaternion.cpp)

target_link_libraries(main ${OPENGL_LIBRARIES} -lfreeglut -lSOIL -lopengl32 -lglu32)

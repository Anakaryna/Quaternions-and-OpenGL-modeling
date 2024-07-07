cmake_minimum_required(VERSION 3.28)
project(Quaternions_and_OpenGL_modeling)

find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} C:/msys64/mingw64/include)

# Ajoutez cette partie pour trouver GLEW
find_package(GLEW REQUIRED)

if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_directories(${GLEW_LIBRARY_DIRS})
    add_definitions(${GLEW_DEFINITIONS})
else()
    message(ERROR "GLEW not found!")
endif()

link_directories(C:/msys64/mingw64/lib)

add_executable(main main.cpp Camera.cpp Map.cpp Block.cpp
        include/Quaternion.h
        Quaternion.cpp
)

# Ajoutez ${GLEW_LIBRARIES} aux bibliothèques à lier
target_link_libraries(main ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} -lfreeglut -lSOIL -lopengl32 -lglu32)

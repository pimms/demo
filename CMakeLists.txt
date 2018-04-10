cmake_minimum_required(VERSION 3.10)
project(demo)

include_directories(src)

if (${APPLE})
    # Assuming Homebrew
    #link_directories("/usr/local/lib")
    include_directories(SYSTEM
            /System/Library/Frameworks/OpenGL.framework/Headers/
            /System/Library/Frameworks/Cocoa.framework/Headers/
            /System/Library/Frameworks/GLUT.framework/Headers/
    )
endif()

add_executable(demo
        src/acore.c
        src/acore.h
        src/adsr.c
        src/adsr.h
        src/context.c
        src/context.h
        src/main.c
        src/vcore.c
        src/vcore.h
        makefile)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(demo ${OPENGL_LIBRARIES})

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})
target_link_libraries(demo ${GLUT_LIBRARIES})

target_link_libraries(demo portaudio)


set_property(TARGET demo PROPERTY C_STANDARD 99)

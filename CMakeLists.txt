cmake_minimum_required(VERSION 3.22.0)
project(PhotoMagic)

# Find SFML components
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_library(
    FibLFSR
    FibLFSR.cpp
    FibLFSR.h
)

# Add your source files
set(SOURCES
    PhotoMagic.cpp
    # Add more source files here if needed
)

# Add an executable target
add_executable(${PROJECT_NAME} ${SOURCES})

# Link SFML libraries
target_link_libraries(${PROJECT_NAME}
    FibLFSR
    sfml-graphics
    sfml-window
    sfml-system
)

# Copy SFML DLLs to the output directory (Windows only)
if(WIN32)
    foreach(SFML_COMPONENT graphics window system)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:SFML::${SFML_COMPONENT}>
                $<TARGET_FILE_DIR:${PROJECT_NAME}>
        )
    endforeach()
endif()

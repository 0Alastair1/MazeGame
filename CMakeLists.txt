cmake_minimum_required(VERSION 3.8)

project(mazeGame)

set(BUILD_SHARED_LIBS OFF)
set(ASSIMP_BUILD_ZLIB ON)

add_subdirectory(vendor/SDL EXCLUDE_FROM_ALL)
#add_subdirectory(vendor/portaudio EXCLUDE_FROM_ALL)
add_subdirectory(vendor/libsamplerate EXCLUDE_FROM_ALL)
add_subdirectory(vendor/assimp EXCLUDE_FROM_ALL)

add_executable(mazeGame src/main.cpp)

include_directories(mazeGame PUBLIC src/include src/
    /home/computer/Documents/GitHub/MazeGame/vendor/stb/ 
    vendor 

    vendor/ 


    vendor/libsamplerate/include/ 
    vendor/portaudio/include/ 
    res/textures/
    vendor/imgui/backends/ 
    vendor/glad/include/ 
    vendor/glad/src/ 
    vendor/glm/ 
    vendor/imgui/ 
    vendor/assimp/include/
    vendor/assimp/include/assimp/Compiler/ 
    vendor/assimp/include/assimp/port/AndroidJNI/ 
    build/vendor/assimp/include 
)

target_link_libraries(mazeGame PRIVATE SDL2-static)
#target_link_libraries(mazeGame PRIVATE PortAudio)
target_link_libraries(mazeGame PRIVATE samplerate)
target_link_libraries(mazeGame PRIVATE assimp)
#target_link_libraries(mazeGame PRIVATE ${OPENGL_gl_LIBRARY})

#add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0) 
#set(CMAKE_BUILD_TYPE Release)

target_compile_features(mazeGame PUBLIC cxx_std_17)

add_custom_command( #todo check if the file actually exists
    TARGET mazeGame POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/build/vendor/assimp/bin/$<CONFIGURATION>/assimp-vc143-mtd.dll" 
        $<TARGET_FILE_DIR:mazeGame>)  
 
add_custom_command(
        TARGET mazeGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/res/textures/
                ${CMAKE_CURRENT_BINARY_DIR}/textures)

add_custom_command(TARGET mazeGame PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res/textures/ $<TARGET_FILE_DIR:mazeGame>/textures/)

#sets compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    
    #remove these later
    set(CMAKE_CXX_EXTENSIONS ON)
    set(CXX_EXTENSIONS ON)

    #set performance flags
    #SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O3")
    #SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Ofast")
    #SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -flto")
    #SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -march=native")
    #SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -finline-functions")

    #set error checking flags
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wextra")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Werror")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wfatal-errors")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wpedantic")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-unused-parameter")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-unused-variable")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-unused-function")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-unused-label")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-unused-value")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wunused")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wextra")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wshadow")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wsuggest-override")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wsuggest-final-types")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wsuggest-final-methods")

    #set release flags
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast")
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto")
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native")
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -finline-functions")
    #SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-exceptions")


endif()


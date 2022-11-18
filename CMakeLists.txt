cmake_minimum_required(VERSION 3.8)

project(mazeGame)

set(BUILD_SHARED_LIBS OFF)

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 

add_subdirectory(vendor/SDL EXCLUDE_FROM_ALL)
add_subdirectory(vendor/portaudio EXCLUDE_FROM_ALL)
add_subdirectory(vendor/libsamplerate EXCLUDE_FROM_ALL)
add_subdirectory(vendor/boost EXCLUDE_FROM_ALL)

add_executable(mazeGame src/main.cpp)

include_directories(mazeGame PUBLIC src/include src/
    /home/computer/Documents/GitHub/MazeGame/vendor/stb/ 
    vendor 

    vendor/ 
    vendor/boost/libs/config/include/ 
    vendor/boost/libs/dll/include/ 
    vendor/boost/libs/filesystem/include/ 
    vendor/boost/libs/assert/include/ 
    vendor/boost/libs/system/include/ 
    vendor/boost/libs/type_traits/include/ 
    vendor/boost/libs/iterator/include/ 
    vendor/boost/libs/mpl/include/ 
    vendor/boost/libs/preprocessor/include/ 
    vendor/boost/libs/static_assert/include/ 
    vendor/boost/libs/core/include/ 
    vendor/boost/libs/detail/include/ 
    vendor/boost/libs/io/include/ 
    vendor/boost/libs/functional/include/ 

    vendor/container_hash/include/ 

    vendor/boost/libs/smart_ptr/include/ 
    vendor/boost/libs/throw_exception/include/ 
    vendor/boost/libs/predef/include/ 


    vendor/libsamplerate/include/ 
    vendor/portaudio/include/ 
    res/textures/
    vendor/imgui/backends/ 
    vendor/glad/include/ 
    vendor/glad/src/ 
    vendor/glm/ 
    vendor/imgui/ 
)

target_link_libraries(mazeGame PRIVATE SDL2-static)
target_link_libraries(mazeGame PRIVATE PortAudio)
target_link_libraries(mazeGame PRIVATE samplerate)
target_link_libraries(mazeGame PRIVATE Boost::filesystem Boost::system)
#target_link_libraries(mazeGame PRIVATE ${OPENGL_gl_LIBRARY})

#add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0) 
#add_definitions(-DBOOST_NO_CXX11_SCOPED_ENUMS)
#set(CMAKE_BUILD_TYPE Release)

target_compile_features(mazeGame PUBLIC cxx_std_17)
 
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

    #remove this when vendor fixes this warning
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-deprecated")
    set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)

    #set release flags
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast")
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto")
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native")
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -finline-functions")
    #SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-exceptions")


endif()


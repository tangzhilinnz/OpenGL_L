#需求的最低cmake程序版本
cmake_minimum_required(VERSION 3.12)
#往项目中加入一个全局的预编译宏
add_definitions (-DDEBUG)

#本工程的名字
project(OpenGL_Lecture)

# Enable verbose output for makefile generation
set(CMAKE_VERBOSE_MAKEFILE ON)

#本工程支持的C++版本
set(CMAKE_CXX_STANDARD 17)

file(GLOB ASSETS "./assets" )
file(COPY ${ASSETS} DESTINATION ${CMAKE_BINARY_DIR})

include_directories(
	STSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/include
)

#link_directories(
#	STSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/lib
#)

add_subdirectory(wrapper)
add_subdirectory(application)
add_subdirectory(glframework)
add_subdirectory(renderer)
add_subdirectory(imgui)

add_executable(OpenGL_L "main.cpp" "glad.c")

target_link_directories(OpenGL_L PRIVATE 
    $<$<CONFIG:Debug>:${PROJECT_SOURCE_DIR}/thirdParty/lib/Debug>
    $<$<CONFIG:Release>:${PROJECT_SOURCE_DIR}/thirdParty/lib/Release>
)

target_link_libraries(OpenGL_L 
    PRIVATE
    $<$<CONFIG:Debug>:glfw3d.lib assimp-vc143-mtd.lib zlibstaticd.lib>
    $<$<CONFIG:Release>:glfw3.lib assimp-vc143-mt.lib zlibstatic.lib>
    wrapper app fw rd imguilib)
cmake_minimum_required(VERSION 3.29)
# Set the project Name
project(OpenglWindow)

# Set the C++ Standard to compile against
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set C++ Flags
set(OPTIMIZE_FLAG "-O2")

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	# Clang compiler
	set(WARNING_FLAGS "-Werror -Wpedantic -Wall -Wextra -Wno-zero-as-null-pointer-constant -Wno-unsafe-buffer-usage -Wno-c++98-compat-pedantic -Wno-documentation -Wno-documentation-unknown-command -Wno-nonportable-system-include-path -Wno-sign-conversion -Wno-shadow -Wno-cast-function-type-strict -Wno-old-style-cast -Wno-unused-parameter")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	# MSVC compiler
	set(WARNING_FLAGS "/Wall /Wv:19.31")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OPTIMIZE_FLAG} ${WARNING_FLAGS}")

# Set Directories for GLEW
set(GLEW_INCLUDE_DIRS "C:\\vclib\\glew-2.1.0\\include")
set(GLEW_LIB_DIR "C:\\vclib\\glew-2.1.0\\lib\\Release\\x64")
set(GLEW_LIBS "glew32.lib"
"glew32s.lib"
)

# Set Directories for GLFW
set(GLFW_INCLUDE_DIRS "C:\\vclib\\glfw-3.4.bin.WIN64\\include")
set(GLFW_LIB_DIR "C:\\vclib\\glfw-3.4.bin.WIN64\\lib-vc2022")

set(GLFW_LIBS "glfw3.lib"
"glfw3_mt.lib"
"glfw3dll.lib"
)

# Set Directories for SDL2
set(SDL2_DIR "C:\\vclib\\SDL2-2.30.5\\cmake")
set(SDL2_INCLUDE_DIRS "C:\\vclib\\SDL2-2.30.5\\include")
set(SDL2_LIB_DIR "C:\\vclib\\SDL2-2.30.5\\lib\\x64")

# Set Directories for SDL2_mixer
set(SDL2_mixer_DIR "C:\\vclib\\SDL2_mixer-2.8.0\\cmake")
set(SDL2_MIXER_INCLUDE_DIR "C:\\vclib\\SDL2_mixer-2.8.0\\include")

# Set the library path for the SDL2_mixer.lib file
set(SDL2_MIXER_LIB "C:\\vclib\\SDL2_mixer-2.8.0\\lib\\x64\\SDL2_mixer.lib")

find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)

# Find OpenGL package
find_package(OpenGL REQUIRED)

include_directories(SYSTEM ${GLFW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIR})
link_directories(${GLEW_LIB_DIR} ${GLFW_LIB_DIR})

file(COPY src/shader DESTINATION ${CMAKE_BINARY_DIR})
file(COPY lint_codebase.ps1 DESTINATION ${CMAKE_BINARY_DIR})
file(COPY audio DESTINATION ${CMAKE_BINARY_DIR})

add_executable(${PROJECT_NAME}
	src/opengl_window.cpp
	src/audio.cpp
	src/audio.hpp
)

target_link_libraries(${PROJECT_NAME} ${GLEW_LIBS} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} ${GLFW_LIBS} ${SDL2_LIBRARIES} ${SDL2_MIXER_LIB})


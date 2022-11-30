cmake_minimum_required(VERSION 3.17)

#Compiler Stuff
set(C_COMPILER_PATH  "D:/MinGW/GCC12.2_64/bin")
set(CXX_COMPILER_PATH  "D:/MinGW/GCC12.2_64/bin")
set(GENERATOR_NAME "MinGW Makefiles")
set(CMAKE_CXX_STANDARD 17)

# SDL2 Stuff
set(SDL2_VERSION "2.26.0")
set(SDL2_IMAGE_VERSION "2.6.2")
set(SDL2_MIXER_VERSION "2.6.2")
set(SDL2_TTF_VERSION "2.20.1")

set(CMAKE_C_COMPILER "${C_COMPILER_PATH}/gcc.exe")
set(CMAKE_CXX_COMPILER "${CXX_COMPILER_PATH}/g++.exe")

# set(CMAKE_BUILD_TYPE Release)

project(MTR VERSION 5)

# Output
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(MTR
		${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/dependencies/SDL2-${SDL2_VERSION}/x86_64-w64-mingw32/include/SDL2
		${PROJECT_SOURCE_DIR}/dependencies/SDL2_image-${SDL2_IMAGE_VERSION}/x86_64-w64-mingw32/include/SDL2
		${PROJECT_SOURCE_DIR}/dependencies/SDL2_mixer-${SDL2_MIXER_VERSION}/x86_64-w64-mingw32/include/SDL2
		${PROJECT_SOURCE_DIR}/dependencies/SDL2_ttf-${SDL2_TTF_VERSION}/x86_64-w64-mingw32/include/SDL2
)

# file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
# file(GLOB NESTEDSOURCES ${PROJECT_SOURCE_DIR}/src/*/*.cpp)
# file(GLOB NESTEDSOURCES2 ${PROJECT_SOURCE_DIR}/src/*/*/*.cpp)
# file(GLOB HEADERS ${PROJECT_SOURCE_DIR}/include/*.h)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.h)


set(SDL2_DIR ${PROJECT_SOURCE_DIR}/dependencies/SDL2-${SDL2_VERSION}/cmake/)
set(SDL2_image_DIR ${PROJECT_SOURCE_DIR}/dependencies/SDL2_image-${SDL2_IMAGE_VERSION}/cmake/)
set(SDL2_mixer_DIR ${PROJECT_SOURCE_DIR}/dependencies/SDL2_mixer-${SDL2_MIXER_VERSION}/cmake/)
set(SDL2_ttf_DIR ${PROJECT_SOURCE_DIR}/dependencies/SDL2_ttf-${SDL2_TTF_VERSION}/cmake/)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

# set(CMAKE_CXX_STANDARD_LIBRARIES "-lmingw32 -lSDL2main -lSDL2 ${CMAKE_CXX_STANDARD_LIBRARIES}")


configure_file("${PROJECT_SOURCE_DIR}/assets/uPref.ini" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/uPref.ini" COPYONLY)

if(NOT STATIC_BUILD)
	if(CMAKE_SIZEOF_VOID_P EQUAL 4)
			set(sdl_p_folder i686-w64-mingw32)
	elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
			set(sdl_p_folder x86_64-w64-mingw32)
	endif()
	file(COPY ${PROJECT_SOURCE_DIR}/dependencies/SDL2-${SDL2_VERSION}/${sdl_p_folder}/bin/SDL2.dll DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/)
	file(COPY ${PROJECT_SOURCE_DIR}/dependencies/SDL2_image-${SDL2_IMAGE_VERSION}/${sdl_p_folder}/bin/SDL2_image.dll DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/)
	file(COPY ${PROJECT_SOURCE_DIR}/dependencies/SDL2_mixer-${SDL2_MIXER_VERSION}/${sdl_p_folder}/bin/SDL2_mixer.dll DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/)
	file(COPY ${PROJECT_SOURCE_DIR}/dependencies/SDL2_ttf-${SDL2_TTF_VERSION}/${sdl_p_folder}/bin/SDL2_ttf.dll DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/)

	add_library(MTR ${SOURCES})
	target_link_libraries(MTR SDL2::SDL2 SDL2_image::SDL2_image SDL2_mixer::SDL2_mixer SDL2_ttf::SDL2_ttf)
else()
	add_library(MTR STATIC ${SOURCES})
	target_link_libraries(MTR SDL2::SDL2-static SDL2_image::SDL2_image-static SDL2_mixer::SDL2_mixer-static SDL2_ttf::SDL2_ttf-static)
endif()

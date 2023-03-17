cmake_minimum_required(VERSION 3.21)

project(BDSpyrunnerW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)

file(GLOB_RECURSE SRC_FILES_DIR
    ${PROJECT_SOURCE_DIR}/src/*.c
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/src/*.h
)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
)

link_directories(
    ${CMAKE_SOURCE_DIR}/lib
)

add_definitions(
    -D"NDEBUG"
    -D"NOMINMAX"
    -D"TEMPLATE_EXPORTS"
    -D"UNICODE"
    -D"WIN32_LEAN_AND_MEAN"
    -D"_AMD64_"
    -D"_CRT_SECURE_NO_WARNINGS"
    -D"_UNICODE"
    -D"_USRDLL"
    -D"_WINDLL"
    -D"_WINDOWS"
)

add_compile_options(
    /diagnostics:column
    /EHsc
    /FC
    /GL
    /MD
    /nologo
    /permissive-
    /sdl
    /utf-8
    /Zc:inline
    /Zi
)

add_link_options(
    /LTCG
    /INCREMENTAL:NO
)

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /NODEFAULTLIB:MSVCRTD")

set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")

add_library(BDSpyrunnerW SHARED ${SRC_FILES_DIR})

set_target_properties(BDSpyrunnerW PROPERTIES OUTPUT_NAME "BDSpyrunnerW")

target_link_libraries(BDSpyrunnerW 
	"${CMAKE_SOURCE_DIR}/lib/python39.lib"
	"${CMAKE_SOURCE_DIR}/lib/minhook.x64.lib"
)

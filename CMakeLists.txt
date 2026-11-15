# CMakeList.txt : CMake project for cmd, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project (
	cmd
	VERSION 0.0.1
	LANGUAGES C CXX
)

# File paths

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src/")
set(LIB_DIR "${PROJECT_SOURCE_DIR}/lib/")

file (GLOB_RECURSE C_CXX_FILES  CMAKE_CONFIGURE_DEPENDS
	"${SRC_DIR}/*.cpp"
	"${SRC_DIR}/*.c"  
)

file (GLOB_RECURSE H_HXX_FILES  CMAKE_CONFIGURE_DEPENDS
	"${SRC_DIR}/*.hpp"
	"${SRC_DIR}/*.h"
)

file (GLOB_RECURSE LIB_FILES CMAKE_CONFIGURE_DEPENDS
	"${LIB_DIR}/*.h"
	"${LIB_DIR}/*.c"
)

# include/src directories

add_executable(cmd 
${C_CXX_FILES} 
${H_HXX_FILES} 
${LIB_FILES} 
"assets/resource.rc"
)
target_include_directories(cmd PUBLIC ${SRC_DIR})
target_include_directories(cmd PUBLIC ${LIB_DIR})

set(ASSET_DIR "${CMAKE_SOURCE_DIR}/assets/")
target_compile_definitions(cmd PRIVATE ASSET_PATH="${ASSET_DIR}")

# language specifications
set_property(TARGET cmd PROPERTY CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

target_link_libraries(cmd PRIVATE PowrProf)

# libraries

include(FetchContent) 

# SFML
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

target_link_libraries(cmd PRIVATE SFML::Graphics SFML::Audio)
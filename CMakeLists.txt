cmake_minimum_required(VERSION 3.9)

project(StreamMaster)

set(SM_3RD "${CMAKE_SOURCE_DIR}/3rd")

set(OBS_INC "${SM_3RD}/obs/includes/libobs")
set(OBS_LIB "${SM_3RD}/obs/libraries_x86")

set(SM_ROOT "${CMAKE_SOURCE_DIR}/src")

set(WS_INCLUDE_DIRS

    "${OBS_INC}"
    "${SM_ROOT}"
)

include_directories(WS_INCLUDE_DIRS)

file(GLOB_RECURSE SOURCES
    "${SM_ROOT}/*.h"
	"${SM_ROOT}/*.c"
	"${SM_ROOT}/*.cpp"
)

# add_library(StreamMaster SHARED "${SOURCES}")
add_executable(StreamMaster "${SOURCES}")

set_property(TARGET StreamMaster PROPERTY LINKER_LANGUAGE CXX)
set_property(TARGET StreamMaster PROPERTY CXX_STANDARD 17)

# hardcode
find_library(LIB_OBS NAMES "obs" PATHS "${OBS_LIB}")
target_link_libraries(StreamMaster "${LIB_OBS}")

# hardcode
# set_property(TARGET wot_stream PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${OBS_SLN}/rundir/Debug/bin/32bit") #?
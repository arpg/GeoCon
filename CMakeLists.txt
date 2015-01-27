project(GeoCon)
cmake_minimum_required(VERSION 2.8)
enable_testing()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules)
list(APPEND CMAKE_CXX_FLAGS "-std=c++11")

include(def_library)
include(export_package)

set(GEOCON_C_FLAGS "-fPIC -Wall -Wextra -Wno-unused-parameter -Wno-deprecated")
set(GEOCON_CXX_FLAGS "${GEOCON_C_FLAGS} -std=c++11")
if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
  set(GEOCON_CXX_FLAGS " ${GEOCON_CXX_FLAGS} -stdlib=libc++ -Qunused-arguments")
endif()

set(RELEASE_FLAGS "-O3 -g0 -DNDEBUG -UDEBUG -funroll-loops -finline-functions")

# Enable both optimization and default debugging
set(RELWITHDEBINFO_FLAGS "-O3 -g -funroll-loops -finline-functions")
set(DEBUG_FLAGS "-g -O0 -UNDEBUG -DDEBUG")	      # No optimization, debugging
set(SIMD_FLAGS "-mmmx -msse -msse2 -msse3 -mssse3 -msse4")

if(NOT ANDROID)
  set(GEOCON_C_FLAGS "${GEOCON_C_FLAGS} ${SIMD_FLAGS}")
  set(GEOCON_CXX_FLAGS "${GEOCON_CXX_FLAGS} ${SIMD_FLAGS}")
  set(RELEASE_FLAGS "${RELEASE_FLAGS} -march=corei7 -mtune=corei7")
  set(RELWITHDEBINFO_FLAGS "${RELWITHDEBINFO_FLAGS} -march=corei7 -mtune=corei7")
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GEOCON_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GEOCON_CXX_FLAGS}")

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${GEOCON_C_FLAGS} ${RELEASE_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${GEOCON_CXX_FLAGS} ${RELEASE_FLAGS}")

set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} ${GEOCON_C_FLAGS} ${RELWITHDEBINFO_FLAGS}")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${GEOCON_CXX_FLAGS} ${RELWITHDEBINFO_FLAGS}")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${GEOCON_C_FLAGS} ${DEBUG_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${GEOCON_CXX_FLAGS} ${DEBUG_FLAGS}")

find_package(MINIGLOG)
find_package(Eigen3)

add_subdirectory(geotrans)

set(MSPCCS_DATA_PATH ${CMAKE_CURRENT_SOURCE_DIR}/geotrans/data)
configure_file(src/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/geocon/config.h)

set(GEOCON_INC_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_BINARY_DIR}
  ${GEOTRANS_INCLUDE_DIRS}
  ${MINIGLOG_INCLUDE_DIRS}
  ${EIGEN3_INCLUDE_DIR})
set(GEOCON_LINK_DIRS ${GEOTRANS_LINK_DIRS})

set(GEOCON_SRCS
  src/geodetic2local.cpp
  src/geocon_init.cpp
  src/geodetic2ecef.cpp
  )

include_directories(${GEOCON_INC_DIRS})
def_library(geocon
  SOURCES ${GEOCON_SRCS}
  DEPENDS MSPdtcc msp_ccs
  LINK_LIBS ${MINIGLOG_LIBRARIES}
  PACKAGE GeoCon
  CONDITIONS MINIGLOG_FOUND EIGEN3_FOUND
  )

export_package(GeoCon
  TARGETS geocon MSPdtcc msp_ccs
  LIBRARIES geocon MSPdtcc msp_ccs
  INCLUDE_DIRS ${GEOCON_INC_DIRS}
  LINK_DIRS ${GEOCON_LINK_DIRS}
  )

add_subdirectory(testing)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake"
    DESTINATION "lib/cmake/${PROJECT_NAME}" )

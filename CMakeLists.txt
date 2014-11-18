project(GeoCon)
cmake_minimum_required(VERSION 2.8)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules)
list(APPEND CMAKE_CXX_FLAGS "-std=c++11")

include(def_library)
include(export_package)

find_package(MINIGLOG)

add_subdirectory(geotrans)

set(GEOCON_INC_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${GEOTRANS_INCLUDE_DIRS}
  ${MINIGLOG_INCLUDE_DIRS})
set(GEOCON_LINK_DIRS ${GEOTRANS_LINK_DIRS})

include_directories(${GEOCON_INC_DIRS})
def_library(geocon
  SOURCES src/geodetic2local.cpp
  DEPENDS MSPdtcc msp_ccs
  LINK_LIBS ${MINIGLOG_LIBRARIES}
  PACKAGE GeoCon
  )

export_package(GeoCon
  TARGETS geocon
  LIBRARIES geocon
  INCLUDE_DIRS ${GEOCON_INC_DIRS}
  LINK_DIRS ${GEOCON_LINK_DIRS}
  )

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake"
    DESTINATION "lib/cmake/${PROJECT_NAME}" )

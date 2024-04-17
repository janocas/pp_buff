#=================================================================
# Project Setup
#=================================================================
cmake_minimum_required (VERSION 3.0)
project(gnb CXX)


set (GNB_DIR  $ENV{GNB_DIR})
set (CMAKE_INCLUDE_PATH ${GNB_DIR}/include/ ${CMAKE_INCLUDE_PATH})
set (CMAKE_SOURCE_DIR ${GNB_DIR}/src/)
set (CMAKE_LIBRARY_PATH ${GNB_DIR}/lib/ ${CMAKE_LIBRARY_PATH})

message ( STATUS "CMAKE_INCLUDE_PATH is " ${CMAKE_INCLUDE_PATH} )
message ( STATUS "CMAKE_LIBRARY_PATH is " ${CMAKE_LIBRARY_PATH} )
message ( STATUS "CMAKE_SOURCE_DIR is " ${CMAKE_SOURCE_DIR} )

set(CMAKE_C_FLAGS
  "${CMAKE_C_FLAGS} ${C_FLAGS_PROCESSOR} ${commonOpts} -std=gnu99 -funroll-loops")
set(CMAKE_CXX_FLAGS
  "${CMAKE_CXX_FLAGS} ${C_FLAGS_PROCESSOR}  ${commonOpts} -std=c++11 ")


set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -ggdb3 -Wl,-rpath=${CMAKE_LIBRARY_PATH} -Wl,--disable-new-dtags")

set(debugOpt "-ggdb3 -DMALLOC_CHECK_=3 -fno-delete-null-pointer-checks")
set(CMAKE_C_FLAGS_DEBUG "${debugOpt} -O0")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "${debugOpt} -O2")
set(CMAKE_C_FLAGS_RELEASE "-O3")

#=================================================================
# INCLUDES
#=================================================================
include_directories(${CMAKE_INCLUDE_PATH})


#=================================================================
# Libraries
#=================================================================



#=================================================================
# Libraries
#=================================================================

add_executable(pp 
  ${CMAKE_SOURCE_DIR}/pp.cpp 
  #${CMAKE_SOURCE_DIR}/pp_buf_sem.cpp 
  #${CMAKE_SOURCE_DIR}/pp_buf_atom.cpp 
  
  
  )

target_link_libraries(pp pthread) 


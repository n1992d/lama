###
 # @file CMakeLists.txt
 #
 # @license
 # Copyright (c) 2009-2015
 # Fraunhofer Institute for Algorithms and Scientific Computing SCAI
 # for Fraunhofer-Gesellschaft
 #
 # Permission is hereby granted, free of charge, to any person obtaining a copy
 # of this software and associated documentation files (the "Software"), to deal
 # in the Software without restriction, including without limitation the rights
 # to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 # copies of the Software, and to permit persons to whom the Software is
 # furnished to do so, subject to the following conditions:
 #
 # The above copyright notice and this permission notice shall be included in
 # all copies or substantial portions of the Software.
 #
 # THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 # IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 # FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 # AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 # LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 # OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 # SOFTWARE.
 # @endlicense
 #
 # @brief CMake configuration file for scai_logging examples
 # @author Thomas Brandes
 # @date 16.06.2015
###

# Try to find OpenMP compile and link flags for the OpenMP example

# LogLevels example, here we compile also LOG_TRACE macros

add_executable( LogLevels LogLevels.cpp )
target_link_libraries( LogLevels scai_logging )
set_target_properties( LogLevels PROPERTIES COMPILE_FLAGS -DSCAI_LOG_LEVEL_TRACE )

# ThreadTest example, here we need the Boost Thread library

# Boost thread required

set ( Boost_DEBUG TRUE )
set ( Boost_DETAILED_FAILURE_MSG TRUE )

find_package ( Boost ${SCAI_FIND_PACKAGE_FLAGS} COMPONENTS thread system )

if    ( Boost_THREAD_FOUND )
    if    ( Boost_SYSTEM_FOUND )
        include_directories ( ${Boost_INCLUDE_DIR} )
        add_executable ( LogThreads LogThreads.cpp )
        target_link_libraries ( LogThreads scai_logging ${Boost_THREAD_LIBRARY} ${Boost_SYSTEM_LIBRARY} )
    else  ( Boost_SYSTEM_FOUND )
        message ( STATUS "Boost system library not found" )
    endif ( Boost_SYSTEM_FOUND )
else  ( Boost_THREAD_FOUND )
    message ( STATUS "Boost thread library not found" )
endif ( Boost_THREAD_FOUND )

# OpenMPTest example

include ( Package/OpenMP )

if    ( OPENMP_FOUND )
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    # Adding this example only possible if OpenMP is available
    add_executable( LogOpenMP LogOpenMP.cpp )
    target_link_libraries( LogOpenMP scai_logging )
else  ( OPENMP_FOUND )
    message ( STATUS "LogOpenMP example not built, no compiler option found" )
endif ( OPENMP_FOUND )

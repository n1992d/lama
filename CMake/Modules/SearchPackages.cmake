###
 # @file SearchPackages.cmake
 #
 # @license
 # Copyright (c) 2013
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
 # @brief List of required and optional packages
 # @author Jan Ecker
 # @date 25.04.2013
###

# Find required packages
set ( REQUIRED_PACKAGES_TO_FIND
        OpenMP
        LAMA_BLAS
        #add required packages here
    )
    
# Find optional packages
set ( OPTIONAL_PACKAGES_TO_FIND
        Doxygen
        Threads
        #add optional packages here
    )
    
#CUDA Only works with GCC on Linux
#TODO: This needs to be checked on windows
if ( CMAKE_COMPILER_IS_GNUCC )

###
    set ( OPTIONAL_PACKAGES_TO_FIND
          ${OPTIONAL_PACKAGES_TO_FIND}
          CUDA
    )
    mark_as_advanced ( CUDA_TOOLKIT_ROOT_DIR )
###    
endif ( CMAKE_COMPILER_IS_GNUCC )
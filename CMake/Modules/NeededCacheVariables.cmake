###
 # @file LAMAdefines.cmake
 #
 # @license
 # Copyright (c) 2009-2013
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
 # @brief Whitelist ( Cache Variables that should be passed to subprojects )
 # @author Lauretta Schubert
 # @date 17.08.2015
 # @since 2.0.0
###

set ( WHITELIST
		ADDITIONAL_CXX_FLAGS
		ADDITIONAL_CXX_FLAGS_RELEASE
		ADDITIONAL_LINKER_FLAGS
		ADDITIONAL_NVCC_FLAGS
		ADDITIONAL_NVCC_RELEASE_FLAGS
		ADDITIONAL_WARNING_FLAGS
		##BOOST
		Boost_INCLUDE_DIR
		Boost_LIBRARY_DIRS
		Boost_REGEX_LIBRARY
		Boost_REGEX_LIBRARY_DEBUG
		Boost_REGEX_LIBRARY_RELEASE
		Boost_SYSTEM_LIBRARY
		Boost_SYSTEM_LIBRARY_DEBUG
		Boost_SYSTEM_LIBRARY_RELEASE
		Boost_THREAD_LIBRARY
		Boost_THREAD_LIBRARY_DEBUG
		Boost_THREAD_LIBRARY_RELEASE
		Boost_UNIT_TEST_FRAMEWORK_LIBRARY
		Boost_UNIT_TEST_FRAMEWORK_LIBRARY_DEBUG
		Boost_UNIT_TEST_FRAMEWORK_LIBRARY_RELEASE
		##
		CMAKE_BUILD_TYPE
        CMAKE_INSTALL_PREFIX
        CMAKE_CXX_COMPILER
        CMAKE_C_COMPILER
        CMAKE_SKIP_INSTALL_RPATH
        CMAKE_SKIP_RPATH
        #
        GFORTRAN_LIBRARY
		LAMA_ADDITIONAL_LINK_FLAGS
		LAMA_ADDITIONAL_LINK_LIBRARIES
		LAMA_BLAS_LIBRARY
		LAMA_BUILD_SHARED
		BUILD_TEST
		USE_CODE_COVERAGE
		USE_BLAS
		USE_CUDA
		USE_CUSPARSE
		USE_GPI2
		USE_MIC
		USE_LAPACK
		USE_MKL
		USE_MPI
		USE_OPENMP
		USE_SCALAPACK
		## Metis
		METIS_INCLUDE_DIR
		METIS_LIBRARY
		## MKL
		MKL_Is64
		MKL_LIBRARY_CORE
		MKL_LIBRARY_GNU
		MKL_LIBRARY_LP64
		## OpenMP
		OpenMP_CXX_FLAGS
		OpenMP_C_FLAGS
		##MPI
        #
        ## ParMetis
        PARMETIS_INCLUDE_DIR
        PARMETIS_LIBRARY
        ## SCAI
        SCAI_ASSERT_LEVEL
        SCAI_LOG_LEVEL
        SCAI_TRACING        
    )
    
set ( CUDA_WHITELIST
		CUDA_64_BIT_DEVICE_CODE
		#CUDA_ATTACH_VS_BUILD_RULE_TO_CUDA_FILE
		CUDA_BUILD_CUBIN
		CUDA_BUILD_EMULATION
		CUDA_COMPUTE_CAPABILITY
        CUDA_CUDART_LIBRARY
        CUDA_CUDA_LIBRARY
        CUDA_GENERATE_CODE
        CUDA_HAVE_GPU
        CUDA_HOST_COMPILATION_CPP
        CUDA_HOST_COMPILER
        CUDA_NVCC_EXECUTABLE
        CUDA_NVCC_FLAGS
        CUDA_NVCC_FLAGS_DEBUG
        CUDA_NVCC_FLAGS_RELEASE
        CUDA_PROPAGATE_HOST_FLAGS
        CUDA_SEPARABLE_COMPILATION
        #CUDA_TARGET_CPU_ARCH
        CUDA_TOOLKIT_INCLUDE
        CUDA_TOOLKIT_ROOT_DIR
        #CUDA_TOOLKIT_TARGET_DIR
        CUDA_VERBOSE_BUILD
        CUDA_VERSION
        CUDA_cublas_LIBRARY
        #CUDA_cufft_LIBRARY
        #CUDA_cupti_LIBRARY
        #CUDA_curand_LIBRARY
        CUDA_cusparse_LIBRARY
        #CUDA_nppc_LIBRARY
        #CUDA_nppi_LIBRARY
        #CUDA_npps_LIBRARY
    )

set ( MPI_WHITELIST )
set ( MIC_WHITELIST )
    
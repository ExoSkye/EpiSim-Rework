# CMakeList.txt : CMake project for EpiSim, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.13)

project ("EpiSim")

# Add source to this project's executable.

set(CMAKE_CXX_STANDARD 20)

include(FindOpenCL)

set(PLATFORM "EpiSim" CACHE STRING "Platform selection (affects output executable name, used for CI artifacts)")
set(OPENCL "no" CACHE STRING "Choose to use OpenCL or not")
set(TRACY "no" CACHE STRING "Choose if tracy is disabled")
set(OPENMP "yes" CACHE STRING "Choose if OpenMP is used")
set(MPI "no" CACHE STRING "Choose if MPI is used")

if (${CMAKE_VERSION} VERSION_GREATER "3.13")
    cmake_policy(SET CMP0076 NEW)
endif()

add_subdirectory("libs/SDL2")
add_executable (EpiSim "EpiSim.cpp" "EpiSim.h" "render/render.h" "render/sdlRender.cpp" "render/sdlRender.h" "tracy/TracyClient.cpp" "algorithm.h" "algononnaive.h" algononnaive.cpp grapher.hpp)
target_link_libraries(EpiSim PUBLIC SDL2 PUBLIC SDL2main PUBLIC pthread PUBLIC dl)
target_link_directories(EpiSim PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/libs/SDL2)
target_include_directories(EpiSim PUBLIC "libs/SDL2/include/")
find_file(BCMHostInclude bcm_host.h PATHS /opt/vc/include)

if (WIN32)
    list(APPEND EXTRA_LIBS vcruntime user32 gdi32 winmm imm32 ole32 oleaut32 version uuid advapi32 shell32)
endif()

if(NOT BCMHostInclude)
    message("Couldn't find bcm_host, assuming this is not a Raspberry Pi")
else()
    message("Enabling Raspberry Pi specific stuff")
    target_link_directories(EpiSim PUBLIC "/opt/vc/lib")
    target_include_directories(EpiSim PUBLIC /opt/vc/include)
endif()


IF(TRACY MATCHES yes)
    target_compile_definitions(EpiSim PUBLIC TRACY_ENABLE)
ENDIF()

IF(OPENMP MATCHES yes)
    find_package(OpenMP REQUIRED)
    target_compile_definitions(EpiSim PUBLIC OMPEnable)
    target_link_libraries(EpiSim PRIVATE OpenMP::OpenMP_CXX)
ENDIF()

IF(MPI MATCHES yes)
    target_compile_definitions(EpiSim PUBLIC MPIEnable)
ENDIF()

IF(CMAKE_BUILD_TYPE MATCHES Debug AND TRACY MATCHES yes)
    message("Enabling Tracy support as this is debug mode")
    target_compile_definitions(EpiSim PUBLIC TRACY_ENABLE _DEBUG)
ENDIF()

IF(OPENCL MATCHES yes)
    message("OpenCL is enabled")
    target_sources(EpiSim PUBLIC oclAlgo.cpp oclAlgo.h)
    target_compile_definitions(EpiSim PUBLIC OpenCL_Enable)
    target_link_libraries(EpiSim PUBLIC ${OPENCL_LIBRARIES} PUBLIC OpenCL)
    target_include_directories(EpiSim PUBLIC ${OPENCL_INCLUDE_DIRS})
    configure_file(kernel.cl ${CMAKE_CURRENT_BINARY_DIR}/kernel.cl COPYONLY)
ENDIF()

IF(CUDA MATCHES yes)
    message("CUDA is enabled")
    target_compile_definitions(EpiSim PUBLIC CUDA_Enable)
    target_link_libraries(EpiSim PUBLIC CUDA::toolkit)
ENDIF()

IF (OPENCL MATCHES no)
    target_sources(EpiSim PUBLIC multiAlgo.cpp multiAlgo.h)
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PLATFORM})
configure_file(conf.ini ${CMAKE_CURRENT_BINARY_DIR}/conf.ini COPYONLY)
# TODO: Add tests and install targets if needed

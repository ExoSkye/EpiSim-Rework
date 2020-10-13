# CMakeList.txt : CMake project for EpiSim, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("EpiSim")

# Add source to this project's executable.

set(CMAKE_CXX_STANDARD 20)

include(FindOpenCL)

set(platform "EpiSim" CACHE STRING "Platform selection")
set(opencl "no" CACHE STRING "Choose to use OpenCL or not")
set(cuda "no" CACHE STRING "Choose to use CUDA or not")
set(tracy "yes" CACHE STRING "Choose if tracy is disabled")

if(${CMAKE_VERSION} VERSION_LESS "3.18.4")
    message("Disabling CUDA support as it is impossible to detect automatically on CMake < 3.18.4")
    set(cuda "no")
else()
    include(FindCUDAToolkit)
endif()

if (${CMAKE_VERSION} VERSION_GREATER "3.13")
    cmake_policy(SET CMP0076 NEW)
endif()

add_subdirectory("libs/SDL2")
add_executable (EpiSim "EpiSim.cpp" "EpiSim.h" "render/render.h" "render/sdlRender.cpp" "render/sdlRender.h" "tracy/TracyClient.cpp" "algorithm.h" "algorithm.cpp" "algomulti.h" "algomulti.cpp"  "algononnaive.h" algononnaive.cpp grapher.hpp)
target_link_libraries(EpiSim PUBLIC SDL2-static PUBLIC SDL2main)
target_include_directories(EpiSim PUBLIC "libs/SDL2/include/")

IF(tracy MATCHES yes)
    target_compile_definitions(EpiSim PUBLIC TRACY_ENABLE)
ENDIF()

IF(CMAKE_BUILD_TYPE MATCHES Debug AND tracy MATCHES yes)
    message("Enabling Tracy support as this is debug mode")
    target_compile_definitions(EpiSim PUBLIC TRACY_ENABLE _DEBUG)
ENDIF()

IF(opencl MATCHES yes)
    message("OpenCL is enabled")
    target_sources(EpiSim PUBLIC oclAlgo.cpp oclAlgo.h)
    target_compile_definitions(EpiSim PUBLIC OpenCL_Enable)
    target_link_libraries(EpiSim PUBLIC ${OPENCL_LIBRARIES} PUBLIC OpenCL)
    target_include_directories(EpiSim PUBLIC ${OPENCL_INCLUDE_DIRS})
    configure_file(kernel.cl ${CMAKE_CURRENT_BINARY_DIR}/kernel.cl COPYONLY)
ENDIF()

IF(cuda MATCHES yes)
    message("CUDA is enabled")
    target_compile_definitions(EpiSim PUBLIC CUDA_Enable)
    target_link_libraries(EpiSim PUBLIC CUDA::toolkit)
ENDIF()

IF (opencl MATCHES no AND cuda MATCHES no)
    target_sources(EpiSim PUBLIC multiAlgo.cpp multiAlgo.h)
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${platform})
configure_file(conf.ini ${CMAKE_CURRENT_BINARY_DIR}/conf.ini COPYONLY)
# TODO: Add tests and install targets if needed.

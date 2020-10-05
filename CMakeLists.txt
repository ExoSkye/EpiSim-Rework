# CMakeList.txt : CMake project for EpiSim, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("EpiSim")

# Add source to this project's executable.

set(CMAKE_CXX_STANDARD 20)

add_subdirectory("libs/SDL2")
set(platform "EpiSim" CACHE STRING "Platform selection")
add_executable (EpiSim "EpiSim.cpp" "EpiSim.h" "render/render.h" "render/sdlRender.cpp" "render/sdlRender.h" "tracy/TracyClient.cpp" "algorithm.h" "algorithm.cpp" "algomulti.h" "algomulti.cpp"  "algononnaive.h" algononnaive.cpp grapher.hpp multiAlgo.cpp multiAlgo.h)
target_link_libraries(EpiSim SDL2-static SDL2main)
target_include_directories(EpiSim PUBLIC "libs/SDL2/include/")
IF(CMAKE_BUILD_TYPE MATCHES Debug)
    message("Enabling Tracy support as this is debug mode")
    target_compile_definitions(EpiSim PUBLIC TRACY_ENABLE _DEBUG)
ENDIF()
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${platform})
configure_file(conf.ini ${CMAKE_CURRENT_BINARY_DIR}/conf.ini COPYONLY)
# TODO: Add tests and install targets if needed.

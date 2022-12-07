cmake_minimum_required(VERSION 2.6)

# Name of project
set(PROJ_NAME Noir_Project)
project(${PROJ_NAME})

# Specify project files: header files and source files
set(HDRS
    asteroid.h camera.h game.h model_loader.h resource.h resource_manager.h scene_graph.h scene_node.h streetlamp.h branch.h cig.h
)
 
set(SRCS
   asteroid.cpp camera.cpp game.cpp main.cpp resource.cpp resource_manager.cpp scene_graph.cpp scene_node.cpp cig.cpp 
   material_fp.glsl material_vp.glsl metal_fp.glsl metal_vp.glsl plastic_fp.glsl plastic_vp.glsl textured_material_fp.glsl textured_material_vp.glsl three-term_shiny_blue_fp.glsl three-term_shiny_blue_vp.glsl 
   normal_map_vp.glsl normal_map_fp.glsl screen_space_vp.glsl screen_space_fp.glsl particle_vp.glsl particle_fp.glsl particle_gp.glsl noir_vp.glsl noir_fp.glsl road_noir_vp.glsl road_noir_fp.glsl streetlamp.cpp 
   rain_particles_vp.glsl rain_particles_gp.glsl rain_particles_fp.glsl branch.cpp cigarette_fp.glsl cigarette_gp.glsl cigarette_vp.glsl noir_filter_vp.glsl noir_filter_fp.glsl tiled_brick_vp.glsl tiled_brick_fp.glsl
)

# Add path name to configuration file
configure_file(path_config.h.in path_config.h)

# Add executable based on the source files
add_executable(${PROJ_NAME} ${HDRS} ${SRCS})

# Require OpenGL library
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(${PROJ_NAME} ${OPENGL_gl_LIBRARY})

# Other libraries needed
set(LIBRARY_PATH C:\Software\GameDesignLibraries\Libraries)
include_directories(${LIBRARY_PATH}/include)
if(NOT WIN32)
    find_library(GLEW_LIBRARY GLEW)
    find_library(GLFW_LIBRARY glfw)
    find_library(SOIL_LIBRARY SOIL)
elseif(WIN32)
    find_library(GLEW_LIBRARY glew32s HINTS ${LIBRARY_PATH}/lib)
    find_library(GLFW_LIBRARY glfw3 HINTS ${LIBRARY_PATH}/lib)
    find_library(SOIL_LIBRARY SOIL HINTS ${LIBRARY_PATH}/lib)
endif(NOT WIN32)
target_link_libraries(${PROJ_NAME} ${GLEW_LIBRARY})
target_link_libraries(${PROJ_NAME} ${GLFW_LIBRARY})
target_link_libraries(${PROJ_NAME} ${SOIL_LIBRARY})
target_link_libraries(${PROJ_NAME} ${OPENAL_LIBRARY})
target_link_libraries(${PROJ_NAME} ${ALUT_LIBRARY})

# The rules here are specific to Windows Systems
if(WIN32)
    # Avoid ZERO_CHECK target in Visual Studio
    set(CMAKE_SUPPRESS_REGENERATION TRUE)
 
    # This will use the proper libraries in debug mode in Visual Studio
    set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)
endif(WIN32)

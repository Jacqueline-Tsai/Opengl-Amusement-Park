cmake_minimum_required(VERSION 2.8)

project(Amusement_park)
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src/)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/)
set(LIB_DIR ${PROJECT_SOURCE_DIR}/lib/)

set(SRC_RENDER_UTILITIES
    ${SRC_DIR}RenderUtilities/BufferObject.h
    ${SRC_DIR}RenderUtilities/Shader.h
    ${SRC_DIR}RenderUtilities/Texture.h
    ${SRC_DIR}RenderUtilities/Mesh.h)

include_directories(${INCLUDE_DIR})
include_directories(${INCLUDE_DIR}glad4.6/include/)
include_directories(${INCLUDE_DIR}glm-0.9.8.5/glm/)

add_Definitions("-D_XKEYCHECK_H")

add_executable(Amusement_park
    ${SRC_DIR}CallBacks.h
    ${SRC_DIR}Camera.h
    ${SRC_DIR}Scene.h
    ${SRC_DIR}Skybox.h
    ${SRC_DIR}Terrain.h
    ${SRC_DIR}SinWave.h
    ${SRC_DIR}HeightMapWave.h
    ${SRC_DIR}Control.h
    ${SRC_DIR}Global.h
    ${SRC_DIR}InteractiveWave.h
    ${SRC_DIR}Box.h
    ${SRC_DIR}BillboardObject.h
    ${SRC_DIR}Particles.h
    ${SRC_DIR}Model.h

    ${SRC_DIR}main.cpp
    ${SRC_DIR}CallBacks.cpp
    ${SRC_DIR}Camera.cpp
    ${SRC_DIR}Scene.cpp
    ${SRC_DIR}Skybox.cpp
    ${SRC_DIR}Terrain.cpp
    ${SRC_DIR}SinWave.cpp
    ${SRC_DIR}HeightMapWave.cpp
    ${SRC_DIR}Control.cpp
    ${SRC_DIR}Global.cpp
    ${SRC_DIR}InteractiveWave.cpp
    ${SRC_DIR}Box.cpp
    ${SRC_DIR}BillboardObject.cpp
    ${SRC_DIR}Particles.cpp

    ${SRC_DIR}FerrisWheel/FerrisWheel.h
    ${SRC_DIR}FerrisWheel/Pillar.h
    ${SRC_DIR}FerrisWheel/Cabin.h
    ${SRC_DIR}FerrisWheel/FerrisWheel.cpp
    ${SRC_DIR}FerrisWheel/Pillar.cpp
    ${SRC_DIR}FerrisWheel/Cabin.cpp

    ${SRC_DIR}RollerCoaster/RollerCoaster.h
    ${SRC_DIR}RollerCoaster/Car.h
    ${SRC_DIR}RollerCoaster/Rail.h
    ${SRC_DIR}RollerCoaster/RollerCoaster.cpp
    ${SRC_DIR}RollerCoaster/Car.cpp
    ${SRC_DIR}RollerCoaster/Rail.cpp

    ${SRC_SHADER}
    ${SRC_RENDER_UTILITIES}

    ${INCLUDE_DIR}glad4.6/src/glad.c
)
source_group("shaders" FILES ${SRC_SHADER})
source_group("RenderUtilities" FILES ${SRC_RENDER_UTILITIES})


target_link_libraries(Amusement_park
    debug ${LIB_DIR}Debug/fltk_formsd.lib      optimized ${LIB_DIR}Release/fltk_forms.lib
    debug ${LIB_DIR}Debug/fltk_gld.lib         optimized ${LIB_DIR}Release/fltk_gl.lib
    debug ${LIB_DIR}Debug/fltk_imagesd.lib     optimized ${LIB_DIR}Release/fltk_images.lib
    debug ${LIB_DIR}Debug/fltk_jpegd.lib       optimized ${LIB_DIR}Release/fltk_jpeg.lib
    debug ${LIB_DIR}Debug/fltk_pngd.lib        optimized ${LIB_DIR}Release/fltk_png.lib
    debug ${LIB_DIR}Debug/fltk_zd.lib          optimized ${LIB_DIR}Release/fltk_z.lib
    debug ${LIB_DIR}Debug/fltkd.lib            optimized ${LIB_DIR}Release/fltk.lib
    debug ${LIB_DIR}Debug/opencv_world341d.lib optimized ${LIB_DIR}Release/opencv_world341.lib)

target_link_libraries(Amusement_park
    ${LIB_DIR}OpenGL32.lib
    ${LIB_DIR}glu32.lib
    ${LIB_DIR}common.lib
    ${LIB_DIR}ex-common.lib
    ${LIB_DIR}OpenAL32.lib
    ${LIB_DIR}alut.lib
    ${LIB_DIR}alut_static.lib
    ${LIB_DIR}assimp-vc142-mtd.lib)

    

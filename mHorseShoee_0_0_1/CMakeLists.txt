cmake_minimum_required(VERSION 3.16)

project(mHorseShoe)
set(CMAKE_CXX_STANDARD 17) 
set(CMAKE_CXX_STANDARD_REQUIRED On)

add_compile_options($<$<CONFIG:Release>:/MT>)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMAKE_MODS")

include(addOgre)
include(addPhysx)
include(addSDL3)
#include(addIMGUI)


set(PHYSX_BUILD_TYPE "release" CACHE STRING "The build type of PhysX")
set_property(CACHE PHYSX_BUILD_TYPE PROPERTY STRINGS debug checked profile release)


if(NOT CMAKE_BUILD_TYPE)
    if(PHYSX_BUILD_TYPE STREQUAL "debug" OR PHYSX_BUILD_TYPE STREQUAL "checked")
        set(CMAKE_BUILD_TYPE "Debug")
    endif()
endif()


link_directories("${physx_SOURCE_DIR}/physx/bin/win.x86_64.vc143.mt/release")


add_compile_definitions(NDEBUG)# FROM PHYSX 


set(SOURCE_FILES_LOC "${CMAKE_SOURCE_DIR}/../src")
set(SOURCE_FILES 
	main.cpp
	${SOURCE_FILES_LOC}/feel/Feel.cpp
	${SOURCE_FILES_LOC}/feel/KeyHandler.cpp
	${SOURCE_FILES_LOC}/feel/playerMovement.cpp
	${SOURCE_FILES_LOC}/GDHandler/Creator.cpp
	${SOURCE_FILES_LOC}/GDHandler/GDHandler.cpp
	${SOURCE_FILES_LOC}/GDHandler/meshHandler.cpp
	${SOURCE_FILES_LOC}/GDHandler/ResourceHandler.cpp
	${SOURCE_FILES_LOC}/GDHandler/stuff/Stuff.cpp
	${SOURCE_FILES_LOC}/GDHandler/stuff/StuffDynamic.cpp
	${SOURCE_FILES_LOC}/GDHandler/stuff/StuffMesh.cpp
	${SOURCE_FILES_LOC}/GDHandler/stuff/StuffStatic.cpp
	${SOURCE_FILES_LOC}/Gui/Gui.cpp
	${SOURCE_FILES_LOC}/Gui/GuiButtons.cpp
	${SOURCE_FILES_LOC}/Gui/GuiTabs.cpp
	${SOURCE_FILES_LOC}/Gui/mediator/GuiMediator.cpp
	${SOURCE_FILES_LOC}/kint/Kint.cpp
	${SOURCE_FILES_LOC}/kint/KintVisualizer.cpp
	${SOURCE_FILES_LOC}/monster/Monster.cpp
	${SOURCE_FILES_LOC}/timer/glock.cpp
	${ogre_SOURCE_DIR}/Components/Overlay/include/imgui_stdlib.cpp
	)

add_executable(MHorseShoe ${SOURCE_FILES})


target_include_directories(MHorseShoe PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(MHorseShoe PRIVATE ${SOURCE_FILES_LOC})
target_include_directories(MHorseShoe PRIVATE ${physx_SOURCE_DIR}/physx/include)
target_include_directories(MHorseShoe PRIVATE ${ogre_SOURCE_DIR}/Components/Bites/include)
target_include_directories(MHorseShoe PRIVATE ${ogre_SOURCE_DIR}/Components/Overlay/include)
target_include_directories(MHorseShoe PRIVATE ${ogre_SOURCE_DIR}/Components/RTShaderSystem/include)
target_include_directories(MHorseShoe PRIVATE ${ogre_SOURCE_DIR}/Components/OgreMain/include)


target_compile_options(OgreXMLConverter PRIVATE $<$<CONFIG:Release>:/MD>)
target_compile_options(OgreBullet PRIVATE $<$<CONFIG:Release>:/MD>)
target_compile_options(Codec_Assimp PRIVATE $<$<CONFIG:Release>:/MD>)
target_compile_options(Plugin_DotScene PRIVATE $<$<CONFIG:Release>:/MD>)

target_link_libraries(MHorseShoe
    
    PhysXExtensions_static_64
    PhysX_64
    PhysXPvdSDK_static_64
    PhysXCommon_64
	PhysXCooking_64
    PhysXFoundation_64
    OgreBites
    SDL3::SDL3
	
)



find_file(
	post_builder
	NAME postbuilder.py
	PATHS "${CMAKE_SOURCE_DIR}"
	RESULT_VARIABLE result
)


SET(POST_SCRIPT "${post_builder}")
SET(POST_ARGS "${ogre_SOURCE_DIR}" "${sdl3_SOURCE_DIR} "${CMAKE_BINARY_DIR}")
find_package(Python3 COMPONENTS Interpreter REQUIRED)

add_custom_command(
	TARGET mHorseShoe
	POST_BUILD
	COMMAND ${Python3_EXECUTABLE} ${POST_SCRIPT} ${POST_ARGS}
	COMMENT "Copying files"
)







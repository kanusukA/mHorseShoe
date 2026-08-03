# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.28)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("mHorseShoee_C")
message ("c build type ${CMAKE_CONFIGURATION_TYPES}")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)




set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

if(MSVC)
    add_compile_options(/Od )
endif()


#set(CMAKE_CONFIGURATION_TYPES "Debug" CACHE STRING "" FORCE)

#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")

message("build type ${CMAKE_BUILD_TYPE}")


include(FetchContent)
include(CPM.cmake)

# VULKAN
find_package(slang CONFIG HINTS "$ENV{VULKAN_SDK}/lib/cmake")

find_package(Vulkan REQUIRED)


# GLM - OPEN-GL MATHAMATICS LIBRARY - RECOMMANDED BY VULKAN
FetchContent_Declare(
	glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG 0af55ccecd98d4e5a8d1fad7de25ba429d60e863 
)

FetchContent_MakeAvailable(glm)








#FetchContent_Declare(
#  ogre
#  GIT_REPOSITORY https://github.com/OGRECave/ogre.git
#  GIT_TAG v14.5.2  
#)


FetchContent_Declare(
  yaml_cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG yaml-cpp-0.9.0
)

FetchContent_Declare(
  sdl
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-3.4.8
)



set(OGRE_BUILD_COMPONENT_OVERLAY TRUE CACHE BOOL "" FORCE)
set(OGRE_BUILD_RENDERSYSTEM_VULKAN TRUE CACHE BOOL "" FORCE)
set(OGRE_BUILD_RENDERSYSTEM_D3D9 FALSE CACHE BOOL "" FORCE)
set(OGRE_BUILD_RTSHADERSYSTEM_SHADERS TRUE CACHE BOOL "" FORCE)
set(OGRE_BUILD_PLUGIN_GLSLANG TRUE CACHE BOOL "" FORCE)
set(OGRE_BUILD_COMPONENT_BITES TRUE CACHE BOOL "" FORCE)
set(OGRE_BUILD_COMPONENT_RTSHADERSYSTEM TRUE CACHE BOOL "" FORCE)
set(OGRE_BUILD_COMPONENT_BULLET FALSE CACHE BOOL "" FORCE)
set(OGRE_BUILD_PLUGIN_DOT_SCENE TRUE CACHE BOOL "" FORCE)
set(OGRE_BUILD_PLUGIN_ASSIMP TRUE CACHE BOOL "" FORCE)
set(OGRE_BUILD_COMPONENT_OVERLAY_IMGUI TRUE CACHE BOOL "" FORCE)
set(OGRE_BUILD_PLUGIN_STBI TRUE CACHE BOOL "" FORCE)
set(OGRE_BUILD_TOOLS TRUE CACHE BOOL "" FORCE)
set(OGRE_COPY_DEPENDENCIES TRUE CACHE BOOL "" FORCE)
set(OGRE_INSTALL_DEPENDENCIES TRUE CACHE BOOL "" FORCE)
set(OGRE_INSTALL_TOOLS TRUE CACHE BOOL "" FORCE)

# set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)

#CPMADDPACKAGE(
#	NAME glslang
#	GIT_REPOSITORY "https://github.com/KhronosGroup/glslang.git"
#	GIT_TAG "16.3.0"
#)


#CPMADDPACKAGE(
#	NAME ogre
#	GIT_REPOSITORY "https://github.com/OGRECave/ogre"
#	GIT_TAG "v14.5.2"	
#)






#target_compile_options(Plugin_GLSLangProgramManager PRIVATE $<$<CONFIG:Debug>:/MD>)

#target_compile_options(OgreGLSupport PRIVATE $<$<CONFIG:Release>:/MD>)
#target_compile_options(OgreXMLConverter PRIVATE $<$<CONFIG:Release>:/MD>)
#target_compile_options(Codec_Assimp PRIVATE $<$<CONFIG:Release>:/MD>)
#target_compile_options(Plugin_DotScene PRIVATE $<$<CONFIG:Release>:/MD>)


#FetchContent_MakeAvailable(ogre)
FetchContent_MakeAvailable(yaml_cpp)
FetchContent_MakeAvailable(sdl)


#set_target_properties(Plugin_GLSLangProgramManager PROPERTIES
   # MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
#    MSVC_RUNTIME_LIBRARY "MultiThreadedDLL"
#)



#set_target_properties(Codec_Assimp PROPERTIES
#		MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL"
#)
#set_target_properties(OgreXMLConverter PROPERTIES
#		MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL"
#)

#set_target_properties(Plugin_GLSLangProgramManager PROPERTIES
#        MSVC_RUNTIME_LIBRARY "MultiThreadedDLL"
#)

#set_target_properties(Plugin_DotScene PROPERTIES
#		MSVC_RUNTIME_LIBRARY "MultiThreadedDLL"
#)




#target_compile_options(Plugin_DotScene PRIVATE $<$<CONFIG:Release>:/MD>)


set(SOURCES
	#main.cpp
	#src/imgui_stdlib.cpp
	#	src/base/Mediator.cpp
	#src/CaseHandler/CaseHandler.cpp
	#src/CaseHandler/CaseSaver.cpp
	#	src/feel/Feel.cpp
	#src/feel/KeyHandler.cpp
	#src/feel/Keys.cpp
	#src/feel/playerMovement.cpp
	#src/GDHandler/GDHandler.cpp
	#src/GDHandler/GDContext.cpp
	#src/GDHandler/GDNotifier.cpp
	#src/GDHandler/GDNotifierHandler.cpp
	#src/GDHandler/GDSystem.cpp
	#src/GDHandler/ResourceHandler.cpp
	
	#src/Gui/Gui.cpp
	#src/Gui/GuiButtons.cpp
	#src/Gui/GuiTabs.cpp
	#src/Gui/GuiComponents/AddTabComponent.cpp
	
	#src/Gui/GuiComponents/ObjectTabComponent.cpp
	#src/Gui/GuiComponents/ResourceTabComponent.cpp
	#src/Gui/GuiComponents/RSUSTabComponent.cpp
	#src/Gui/GuiComponents/StatusTabComponent.cpp
	#src/Gui/GuiComponents/ToastComponent.cpp
	#src/Gui/GuiComponents/ToastTabComponent.cpp
	#src/Gui/GuiComponents/_SceneTabComponent.cpp
	#src/monster/Monster.cpp
	#src/ResourceHandler/ResourceLoader.cpp
	
	#src/ResourceHandler/ResourceObjects.cpp
	#src/ResourceHandler/ResourceReader.cpp
	#src/ResourceHandler/ResourceSaver.cpp
	#src/Stuffs/CaseObject.cpp
	#src/Stuffs/ColliderMesh.cpp
	#src/Stuffs/EntityObject.cpp
	#src/Stuffs/ImageObject.cpp
	#src/Stuffs/Material.cpp
	#src/Stuffs/RenderMesh.cpp
	#src/Stuffs/SceneObject.cpp
	#src/Stuffs/ShaderObject.cpp
	#src/timer/glock.cpp
	src/vktest/main.cpp
	src/vktest/Monster.cpp
	src/vktest/MonsterSDL.cpp
	src/vktest/MonsterVulkan.cpp
)

add_executable(mHorseShoee ${SOURCES}  "src/vktest/VulkanReader.h" "src/vktest/vk_mem_alloc.h" "src/vktest/MonsterRapid.cpp" "src/vktest/feel/feel.h" "src/vktest/feel/feel.cpp" "src/vktest/feel/keys.h" "src/vktest/Camera.h" "src/vktest/Camera.cpp" "src/vktest/glock.h")

# target_compile_features(mHorseShoee PUBLIC cxx_std_20)

#target_compile_definitions(mHorseShoee PUBLIC 
#	VULKAN_HPP_DISPATCH_LOCADER_DYNAMIC=1
#	VULKAN_HPP_NO_STRUCT_CONSTRUCTORS=1
#)

#target_sources(mHorseShoee PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES BASE_DIRS "${Vulkan_INCLUDE_DIR}"
#FILES "${Vulkan_INCLUDE_DIR}/vulkan/vulkan.cppm")

#target_compile_definitions(Plugin_GLSLangProgramManager PUBLIC $<$<CONFIG:Debug>:/O2 /Ob2>)




target_include_directories(mHorseShoee PRIVATE "${Vulkan_INCLUDE_DIR}")

#set_target_properties(mHorseShoee PROPERTIES CXX_STANDARD 20)


target_link_libraries(mHorseShoee PRIVATE Vulkan::Vulkan)

target_include_directories(mHorseShoee PUBLIC "src/vktest" )

target_link_libraries(mHorseShoee PRIVATE glm::glm)


#target_link_libraries(mHorseShoee PRIVATE OgreBites)
#target_link_libraries(mHorseShoee PRIVATE OgreMain)
#target_link_libraries(mHorseShoee PRIVATE OgreOverlay)
#target_link_libraries(mHorseShoee PRIVATE OgreRTShaderSystem)
#target_link_libraries(mHorseShoee PRIVATE RenderSystem_Direct3D11)
#target_link_libraries(mHorseShoee PRIVATE RenderSystem_Direct3D9)
#target_link_libraries(mHorseShoee PRIVATE RenderSystem_Vulkan)
#target_link_libraries(mHorseShoee PRIVATE Codec_STBI)
#target_link_libraries(mHorseShoee PRIVATE Codec_Assimp)
#target_link_libraries(mHorseShoee PRIVATE Plugin_OctreeSceneManager)
target_link_libraries(mHorseShoee PRIVATE yaml-cpp)
target_link_libraries(mHorseShoee PRIVATE SDL3::SDL3)
#target_link_libraries(mHorseShoee PRIVATE Codec_FreeImage)
#target_link_libraries(mHorseShoee PRIVATE Plugin_DotScene)
#target_link_libraries(mHorseShoee PRIVATE Plugin_GLSLangProgramManager)


message("Path ${ogre_BINARY_DIR}/bin/release")

if(WIN32)
    add_custom_command(TARGET mHorseShoee POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:mHorseShoee>
			#"${ogre_BINARY_DIR}/bin/release/plugins.cfg" COPY THE PLUGIN AND RESOURCE FILES MANUALLY AS THEY GET OVERWRITTEN ON EVERY RUN.
			#"${ogre_BINARY_DIR}/bin/release/resources.cfg"
			$<TARGET_FILE_DIR:mHorseShoee>
        COMMAND_EXPAND_LISTS
    )
endif()


# Include sub-projects.










include(FetchContent)
FetchContent_Declare(
    physx
    GIT_REPOSITORY https://github.com/NVIDIA-Omniverse/PhysX
    GIT_TAG 107.3-physx-5.6.1
)
FETCHCONTENT_MAKEAVAILABLE(physx)


#Check if file exists
find_file(
	PhysX_64
	NAME PhysX_64.lib
	PATHS "${physx_SOURCE_DIR}/physx/bin/win.x86_64.vc143.mt/release"
	RESULT_VARIABLE result
)
if(PhysX_64)
	message("Physx library files found. skipping build")
else()
	message("Physx library files not found. Building...")
	#run a python script
	find_package(Python3 COMPONENTS Interpreter REQUIRED)

	SET(PYTHON_SCRIPT "${CMAKE_SOURCE_DIR}/physxbuilder.py")
	SET(PYTHON_ARGS "${physx_SOURCE_DIR}" "${CMAKE_BINARY_DIR}")
	message("Running physx generate_projects.bat")
	execute_process(COMMAND ${Python3_EXECUTABLE} ${PYTHON_SCRIPT} ${PYTHON_ARGS}
		RESULT_VARIABLE script_result
		OUTPUT_VARIABLE script_output
		)
	message("Physx output - ${script_output}")
	if (NOT script_result EQUAL 0)
		message(FATAL_ERROR "PHYSX WAS NOT BUILT ${CMAKE_SOURCE_DIR}")
	endif()
	message("PHYSX WAS BUILT")
	
endif()







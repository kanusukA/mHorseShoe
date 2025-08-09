include(FetchContent)
FetchContent_Declare(
    ogre
    GIT_REPOSITORY https://github.com/OGRECave/ogre
    GIT_TAG v14.4.0
)

FetchContent_MakeAvailable(ogre)

message("cmake source ${CMAKE_SOURCE_DIR}")

find_file(
	ogreBuilder
	NAME ogrebuilder.py
	PATHS "${CMAKE_SOURCE_DIR}"
	RESULT_VARIABLE result
)
message("Ogre py path - ${ogreBuilder}")

SET(OGRE_SCRIPT "${ogreBuilder}")
SET(OGRE_ARGS "${ogre_SOURCE_DIR}" "${CMAKE_BINARY_DIR}")
message("Running ogre builder ${ogre_SOURCE_DIR}")
find_package(Python3 COMPONENTS Interpreter REQUIRED)
execute_process(COMMAND ${Python3_EXECUTABLE} ${OGRE_SCRIPT} ${OGRE_ARGS}
	RESULT_VARIABLE script_result
	OUTPUT_VARIABLE script_output
	)
message("ogre build output - ${script_output}")
if (NOT script_result EQUAL 0)
	
	message(FATAL_ERROR "OGRE WAS NOT BUILT")
endif()
message("OGRE WAS BUILT")

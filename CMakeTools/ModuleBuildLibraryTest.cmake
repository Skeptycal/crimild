# This module configures how to build a library
# The following arguments are valid:
# 	CRIMILD_LIBRARY_NAME: (Required) Name of the library
#	CRIMILD_LIBRARY_SOURCE_FILES: (Optional) Collection of files that need to be included as part of the library
#	CRIMILD_LIBRARY_LINK_LIBRARIES: (Optional) External libraries that need to be linked with this one
#	CRIMILD_LIBRARY_DEPENDENCIES: (Optional) Any dependencies that are required in order to build the library
#	CRIMILD_INCLUDE_DIRECTORIES: (Optional) Additional include directories for dependencies
#	CRIMILD_LINK_DIRECTORIES: (Optional) Additional link directories for dependencies

MESSAGE( "   Adding tests" )

FILE( GLOB_RECURSE CRIMILD_TESTS_SOURCE_FILES "${CRIMILD_SOURCE_DIR}/${CRIMILD_LIBRARY_NAME}/test/*.cpp" )

# set default dependencies
SET( CRIMILD_TESTS_DEPENDENCIES
	crimild_${CRIMILD_LIBRARY_NAME}
	${CRIMILD_LIBRARY_DEPENDENCIES}
	gtest gtest_main
	gmock gmock_main
)

INCLUDE_DIRECTORIES(
	${CRIMILD_SOURCE_DIR}/${CRIMILD_LIBRARY_NAME}/src
	${CRIMILD_SOURCE_DIR}/${CRIMILD_LIBRARY_NAME}/test
	${CRIMILD_LIBRARY_DEPENDENCIES}
	${gtest_SOURCE_DIR}/include 
	${gtest_SOURCE_DIR}
	${gmock_SOURCE_DIR}/include 
	${gmock_SOURCE_DIR}
)

SET( CRIMILD_TEST_EXECUTABLE_NAME crimild_${CRIMILD_LIBRARY_NAME}_test )

ADD_EXECUTABLE( ${CRIMILD_TEST_EXECUTABLE_NAME} ${CRIMILD_TESTS_SOURCE_FILES} )

TARGET_LINK_LIBRARIES( ${CRIMILD_TEST_EXECUTABLE_NAME} ${CRIMILD_TESTS_DEPENDENCIES} )

ADD_TEST( ${CRIMILD_TEST_EXECUTABLE_NAME} ${CRIMILD_TEST_EXECUTABLE_NAME} )

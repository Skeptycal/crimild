CMAKE_MINIMUM_REQUIRED( VERSION 2.8.10 FATAL_ERROR )

PROJECT( CRIMILD )

# Register custom modules
SET ( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CRIMILD_SOURCE_DIR}/CMakeTools" )

INCLUDE( MacroExtractArchive )
INCLUDE( MacroConfigureSystem )

ADD_DEFINITIONS( -DCRIMILD_VERSION_MAJOR=4 )
ADD_DEFINITIONS( -DCRIMILD_VERSION_MINOR=9 )
ADD_DEFINITIONS( -DCRIMILD_VERSION_PATCH=0 )

# Configuration options
OPTION( CRIMILD_ENABLE_TESTS "Would you like to enable unit tests?" OFF )
OPTION( CRIMILD_ENABLE_IMPORT "Build importers?" OFF )
OPTION( CRIMILD_ENABLE_PHYSICS "Would you like to build the crimild-physics extension?" OFF )
OPTION( CRIMILD_ENABLE_SCRIPTING "Would you like to build the crimild-scripting extension?" OFF )
OPTION( CRIMILD_ENABLE_SFML "Would you like to build the crimild-sfml extension" OFF )
OPTION( CRIMILD_ENABLE_GLFW "Would you like to build the crimild-glfw extension?" OFF )
OPTION( CRIMILD_ENABLE_SDL "Would you like to build the crimild-sdl extension?" OFF )

IF ( ${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten" )
   SET( CRIMILD_ENABLE_EMSCRIPTEN ON )
ELSE ()
   SET( CRIMILD_ENABLE_EMSCRIPTEN OFF )
ENDIF ()

IF ( CRIMILD_ENABLE_TESTS )
	ENABLE_TESTING()

	ADD_DEFINITIONS( -DGTEST_LANG_CXX11=1 )
	ADD_DEFINITIONS( -DGTEST_USE_OWN_TR1_TUPLE=1 )

	EXTRACT_ARCHIVE( gmock-1.6.0.zip gmock-1.6.0 ${CRIMILD_SOURCE_DIR}/third-party )

	ADD_SUBDIRECTORY( ${CRIMILD_SOURCE_DIR}/third-party/gmock-1.6.0 "${CMAKE_CURRENT_BINARY_DIR}/third-party/gmock-1.6.0" )
ENDIF ( CRIMILD_ENABLE_TESTS )

# Add core sources
ADD_SUBDIRECTORY( core )
ADD_SUBDIRECTORY( raytracing )

IF ( CRIMILD_ENABLE_IMPORT )
	ADD_DEFINITIONS( -DCRIMILD_ENABLE_IMPORT=1 )

	SET( ASSIMP_BUILD_STATIC_LIB ON CACHE BOOL "Build Static libs" )
	SET( BUILD_SHARED_LIBS OFF CACHE BOOL "Build Shared Libs" )
	SET( ASSIMP_BUILD_TESTS OFF CACHE BOOL "Build Assimp tests" )
	SET( ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "Build Assimp tools" )
	
	ADD_SUBDIRECTORY( ${CRIMILD_SOURCE_DIR}/third-party/assimp )
	ADD_SUBDIRECTORY( import )
ENDIF ( CRIMILD_ENABLE_IMPORT )

IF ( CRIMILD_ENABLE_PHYSICS )
	ADD_DEFINITIONS( -DCRIMILD_ENABLE_PHYSICS=1 )
	ADD_SUBDIRECTORY( physics )
ENDIF ( CRIMILD_ENABLE_PHYSICS )

# Add scripting sources
iF ( CRIMILD_ENABLE_SCRIPTING )
	ADD_DEFINITIONS( -DCRIMILD_ENABLE_SCRIPTING=1 )

	# Extract lua sources if needed
	EXTRACT_ARCHIVE( lua-5.3.4.tar.gz lua ${CRIMILD_SOURCE_DIR}/third-party )

	OPTION( CRIMILD_SCRIPTING_LOG_VERBOSE "Enable verbose logs for scripting system" OFF )
	IF ( CRIMILD_SCRIPTING_LOG_VERBOSE )
		ADD_DEFINITIONS( -DCRIMILD_SCRIPTING_LOG_VERBOSE=1 )
	ENDIF ()
	ADD_SUBDIRECTORY( scripting )
ENDIF ()

IF ( CRIMILD_ENABLE_SFML ) 
	ADD_DEFINITIONS( -DCRIMILD_ENABLE_SFML=1 )

	IF ( APPLE ) 
		SET( SFML_BUILD_FRAMEWORKS OFF CACHE BOOL "Build SFML as frameworks" )
		SET( SFML_INSTALL_XCODE_TEMPLATES OFF CACHE BOOL "Build SFML templates" )
	ENDIF ( APPLE )

	SET( SFML_BUILD_EXAMPLES OFF CACHE BOOL "Build SFML examples" )
	SET( SFML_BUILD_DOC OFF CACHE BOOL "Build SFML docs" )

	ADD_SUBDIRECTORY( ${CRIMILD_SOURCE_DIR}/third-party/sfml )

	ADD_SUBDIRECTORY( sfml )
ENDIF ( CRIMILD_ENABLE_SFML )

IF ( CRIMILD_ENABLE_GLFW OR CRIMILD_ENABLE_SDL )
	ADD_DEFINITIONS( -DCRIMILD_ENABLE_OPENGL=1 )
	ADD_SUBDIRECTORY( opengl )
ENDIF ()

IF ( CRIMILD_ENABLE_GLFW )
	ADD_DEFINITIONS( -DCRIMILD_ENABLE_GLFW=1 )

	IF ( NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten" )
	   	SET( GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation" )
	   	SET( GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs" )
		SET( GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs" )
	
		ADD_SUBDIRECTORY( ${CRIMILD_SOURCE_DIR}/third-party/glfw )
	ELSE ()
   		 SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_GLFW=3" )
	ENDIF ()

	ADD_SUBDIRECTORY( glfw )
	
ENDIF ( CRIMILD_ENABLE_GLFW )

IF ( CRIMILD_ENABLE_SDL )
   ADD_DEFINITIONS( -DCRIMILD_ENABLE_SDL=1 )
   ADD_SUBDIRECTORY( sdl )
ENDIF ()

OPTION( CRIMILD_BUILD_TOOLS "Build tools?" OFF )
IF ( CRIMILD_BUILD_TOOLS )
	ADD_SUBDIRECTORY( tools )
ENDIF ( CRIMILD_BUILD_TOOLS )

# Options for app bundles
OPTION( CRIMILD_BUNDLE_APPS "Enable bundles for apps" OFF )


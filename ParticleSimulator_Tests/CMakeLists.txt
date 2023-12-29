cmake_minimum_required(VERSION 3.14)

project(ParticleSimulator_Tests)

set( CMAKE_BUILD_TYPE Debug )

if( WIN32 )
	set( CMAKE_CXX_FLAGS "/DWIN32 /D_WINDOWS /W3 /GR /EHsc /std:c++20 /D_UNICODE /DUNICODE" )
	set( CMAKE_CXX_FLAGS_DEBUG "/MDd /Zi /Ob0 /Od /RTC1 /std:c++20 /D_UNICODE /DUNICODE" )
	message( "Win settings chosen..." )
elseif( ${CMAKE_SYSTEM_NAME} STREQUAL "Darwin" )
	set( CMAKE_CXX_FLAGS "-std=c++20 -Wall" )
	set( CMAKE_CXX_FLAGS_DEBUG "-g -std=c++20 -Wall" )
	message( "Mac settings chosen..." )
elseif( UNIX )
	set( CMAKE_CXX_FLAGS "-std=c++20 -Wall" )
	set( CMAKE_CXX_FLAGS_DEBUG "-g -std=c++20 -Wall" )
	message( "Linux settings chosen..." )
endif()

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


include_directories( ../ParticleSimulator_PhysicsLib/include )
include_directories( ./include )
include_directories( ../SDL2/include )


link_directories( ../SDL/lib/Debug )
link_directories( ../SDL/lib/Release )

# Automatically add all *.cpp files to the project
file ( GLOB SOURCES "./src/*.cpp" )

enable_testing()

add_executable( ${PROJECT_NAME} ${SOURCES} )

find_library( 	PARTICLE_SIMULATOR_PHYSICS_LIB 
		NAMES 			ParticleSimulator_PhysicsLib 
		PATHS 			../ParticleSimulator_PhysicsLib/build_win/Debug
					../ParticleSimulator_PhysicsLib/build_linux/Debug )
					
target_link_libraries(
	
	ParticleSimulator_Tests
	${PARTICLE_SIMULATOR_PHYSICS_LIB}
	GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(ParticleSimulator_Tests)

# Set the default project 
set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME} )


message( "CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}" )

#################################################################################
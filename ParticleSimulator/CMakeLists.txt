cmake_minimum_required( VERSION 3.6.2 )

# For a new project it is sufficient to change only its name in the following line
set( PROJECT_NAME ParticleSimulator )

project( ${PROJECT_NAME} )

#set( CMAKE_BUILD_TYPE Debug )
#set( CMAKE_BUILD_TYPE Release )


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


#add_subdirectory( ../ParticleSimulator_PhysicsLib/src )

# Inform CMake where the header files are
include_directories( ../ParticleSimulator_PhysicsLib/include )
include_directories( ./include )
include_directories( ../SDL2/include )
include_directories( ../imgui )

link_directories( ../SDL/lib/Debug )
link_directories( ../SDL/lib/Release )

# Automatically add all *.cpp files to the project
file ( GLOB SOURCES "./src/*.cpp" "../imgui/*.cpp" "../imgui/backends/imgui_impl_sdl2.cpp" "../imgui/backends/imgui_impl_opengl2.cpp" )

# Add source file to the build executable
add_executable( ${PROJECT_NAME} ${SOURCES} )


# Some system libraries
if( WIN32 )

# Assuming Windows SDK is installed, so we do not need to call find_library()
	#set_target_properties(${project} PROPERTIES LINK_FLAGS ${MACHINE:X86})

	target_link_libraries( 	${PROJECT_NAME} 
				PUBLIC 
					#wsock32 #potrzebne?? sieciowe sprawy
					#comctl32
			)

elseif( ${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")

	find_library( COCOA_LIB Cocoa )
	target_link_libraries( ${PROJECT_NAME} PUBLIC ${COCOA_LIB} )

endif()



# Currency Calc library
find_library( 	PARTICLE_SIMULATOR_PHYSICS_LIB 
		NAMES 			ParticleSimulator_PhysicsLib 
		PATHS 			../ParticleSimulator_PhysicsLib/build_win/Debug
					../ParticleSimulator_PhysicsLib/build_linux/Debug
					../ParticleSimulator_PhysicsLib/build_mac/Debug
					../ParticleSimulator_PhysicsLib/build_win/Release
					../ParticleSimulator_PhysicsLib/build_linux/Release
					../ParticleSimulator_PhysicsLib/build_mac/Release )

find_package(OpenGL)


target_link_libraries( 	${PROJECT_NAME} 
			PUBLIC 
				${PARTICLE_SIMULATOR_PHYSICS_LIB}
				${CMAKE_SOURCE_DIR}/../SDL2/lib/Debug/SDL2.lib
				${CMAKE_SOURCE_DIR}/../SDL2/lib/Debug/SDL2main.lib
				${CMAKE_SOURCE_DIR}/../SDL2/lib/Release/SDL2.lib
				${CMAKE_SOURCE_DIR}/../SDL2/lib/Release/SDL2main.lib
				${OPENGL_gl_LIBRARY}
		)

# Set the default project 
set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME} )


message( "CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}" )




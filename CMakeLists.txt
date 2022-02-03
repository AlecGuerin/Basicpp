cmake_minimum_required(VERSION 3.10.0)
project(Basicpp VERSION 1.0.0 )

## Set the build type
set(CMAKE_BUILD_TYPE "Debug")	# Release/Debug Mind to set Release for deployement

## Set the installation directory (also provide path to find already generated libraries)
if(APPLE)
    set(INSTALL_DIR "")
	find_library(IOKIT_LIBRARY IOKit)
    find_library(FOUNDATION_LIBRARY Foundation)
elseif(UNIX)
	set(INSTALL_DIR "/usr")
	# Set default ubuntu paths
	set(INS_LIB_DIR "${INSTALL_DIR}/lib/MyLib/${CMAKE_BUILD_TYPE}")
	set(INS_INC_DIR "${INSTALL_DIR}/include/MyLib/${CMAKE_BUILD_TYPE}")

	set(rt_LIBRARIES rt)
    set(pthread_LIBRARIES pthread)

elseif(WIN32)
    set(INSTALL_DIR "C:/MyLib/${CMAKE_BUILD_TYPE}")

	set(INS_LIB_DIR "${INSTALL_DIR}/lib")
	set(INS_INC_DIR "${INSTALL_DIR}/include")
	set(WINDOWS_EXPORT_ALL_SYMBOLS "TRUE")	#Add the symbols for windows

	add_compile_definitions("_SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING") # Use std::experimental::filesystem for C++11
	
else()
    set(INSTALL_DIR "") #TODO: if other define there
endif()

## Sources
set(basicpp_SRCS
    src/applog.cpp
    src/inifile.cpp
    include/basicpp/applog.h
    include/basicpp/basicpp.h
    include/basicpp/inifile.h
)

## Allow the developer to select if Dynamic or Static libraries are built (Shared is prefered)
#set (BUILD_SHARED_LIBS "ON")  # empty for false

IF (BUILD_SHARED_LIBS)
	# User wants to build Dynamic Libraries, so change the LIB_TYPE variable to CMake keyword 'SHARED'
	SET (LIB_TYPE SHARED)
ELSE()
	# Set the LIB_TYPE variable to STATIC
	SET (LIB_TYPE STATIC)
	add_compile_definitions("${PROJECT_NAME}_BUILT_AS_STATIC") # Use std::experimental::filesystem for C++11
ENDIF (BUILD_SHARED_LIBS)

message (STATUS "Exit type: ${LIB_TYPE}")



## Add/Create library with thhe project name : basicpp
add_library(${PROJECT_NAME} ${LIB_TYPE} ${basicpp_SRCS})

## Link system libraries
if(APPLE)
	target_link_libraries(${PROJECT_NAME} ${FOUNDATION_LIBRARY} ${IOKIT_LIBRARY})
elseif(UNIX)
	target_link_libraries(${PROJECT_NAME} rt pthread)	# Get pthread and RT	
	target_link_libraries(${PROJECT_NAME} stdc++fs)	# Get std::experimental::filesystem
elseif(WIN32)
	target_link_libraries(${PROJECT_NAME} setupapi)		# Get setup API
else()
    set(INSTALL_DIR "") #TODO: if other define there
endif()

## Uncomment for example
add_executable(basicpp_example examples/basicpp_example.cpp)	# Create an executable from the cpp file
add_dependencies(basicpp_example ${PROJECT_NAME})				# Add executable dependency to the project
target_link_libraries(basicpp_example ${PROJECT_NAME})			# Link executable to the generated library

## Include headers
include_directories("include")		#Include the headers from include folder

## Install library
install(TARGETS ${PROJECT_NAME} DESTINATION "${INS_LIB_DIR}")

## Install headers
install(FILES include/basicpp/applog.h include/basicpp/basicpp.h include/basicpp/inifile.h DESTINATION "${INS_INC_DIR}/basicpp")
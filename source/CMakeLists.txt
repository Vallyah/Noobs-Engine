# Adds convenience methods, see cmake/cleanCppExtensions.cmake
include(cleanCppExtensions)

# Creates options to turn on sanitizers, see cmake/sanitizers.cmake
include(sanitizers)

# Enable helpfull warnings and C++14 for all files
if(MSVC)
    add_compile_options(/std:c++14 /W3 /WX )
else()
    add_compile_options(-std=c++14 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wunused -pedantic)
endif()

# configure version.cpp.in with selected version
configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/version.cpp.in"
  "${CMAKE_CURRENT_BINARY_DIR}/version.cpp"
  )
# If you want to have date and time of the build, your targets has to depend on this.
# This will force recompilation of version.o and thus forcing gcc to update __DATE__ macro.
add_custom_target(versionFileTouchForRebuild
    COMMAND ${CMAKE_COMMAND} -E touch "${CMAKE_CURRENT_BINARY_DIR}/version.cpp")


#------------------------------------------------------------------------------
# Libraries dependencies
find_package (Threads REQUIRED) #threading

#------------------------------------------------------------------------------
# By using macro to add common dependencies you can avoid repetition when you have
# multiple binaries.
# Use with `addCommonDependencies(noobsEngine)`
# macro(addCommonDependencies TARGET)
#     add_dependencies(${TARGET} versionFileTouchForRebuild) # We want precise time of build in version
#     target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
#     target_link_libraries (${TARGET} PRIVATE Threads::Threads)
#     target_compile_definitions(${TARGET} PRIVATE -DSOME_DEFINE)
# endmacro()

#-------------------------------------------------------------------------------
# noobsEngine executable setup

set(ICON_RESOURCE "")
if(WIN32) #windows icon resource file, provided from /packaging directory
    set(ICON_RESOURCE "${PROJECT_BINARY_DIR}/noobsEngine.icon.rc")
elseif (APPLE) #osx icon ressource file, provided from /packaging directory
    set(ICON_RESOURCE "${PROJECT_SOURCE_DIR}/packaging/noobsEngine.icns")
endif()

file(GLOB_RECURSE sceneutils_sources  ${CMAKE_CURRENT_SOURCE_DIR}/SceneUtils/*.cpp)
file(GLOB_RECURSE sceneutils_headers  ${CMAKE_CURRENT_SOURCE_DIR}/SceneUtils/*.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/SceneUtils)

SOURCE_GROUP("SceneUtils Sources" FILES ${sceneutils_sources})
SOURCE_GROUP("SceneUtils Headers" FILES ${sceneutils_headers})

add_executable(noobsEngine MACOSX_BUNDLE
    ${ICON_RESOURCE}
    ${CMAKE_CURRENT_BINARY_DIR}/version.cpp
    ${sceneutils_sources}
    ${sceneutils_headers}
    scene.h
    scene.cpp
    main.cpp)
add_dependencies(noobsEngine versionFileTouchForRebuild) # We want precise time of build in version
target_include_directories(noobsEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries (noobsEngine PRIVATE Threads::Threads)

if(APPLE)
    set_target_properties(noobsEngine PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "noobsEngine")
    set_target_properties(noobsEngine PROPERTIES MACOSX_BUNDLE_BUNDLE_GUI_IDENTIFIER "com.noobsEngine.noobsEngine")
    set_target_properties(noobsEngine PROPERTIES MACOSX_BUNDLE_ICON_FILE noobsEngine.icns)
    set_target_properties(noobsEngine PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${PROJECT_SOURCE_DIR}/packaging/MacOSXBundleInfo.plist.in)
    set_source_files_properties(${PROJECT_SOURCE_DIR}/packaging/noobsEngine.icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
endif()

install(TARGETS noobsEngine
    BUNDLE DESTINATION "."
    RUNTIME DESTINATION bin)

# Using macro from cmake/cleanCppExtensions.cpp included on the top
# Provides noobsEngine-run and noobsEngine-dbg
addRunAndDebugTargets(noobsEngine)

# short convenience target from cmake/cleanCpppExtensions.cpp
add_custom_target(run DEPENDS noobsEngine-run)

# Manually specified convenience target for running the binary, we use macro
# add_custom_target(noobsEngine-run
#     WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin
#     USES_TERMINAL
#     COMMAND ./noobsEngine
#     DEPENDS Install_)

# And this would add -DBAR_ENABLE_FEATURE or equivalent to build command
# target_compile_definitions(noobsEngine PRIVATE BAR_ENABLE_FEATURE)

# Add compile time definition based on option
# option(OPTION_NAME "description for end user" OFF)
# if(OPTION_NAME)
#     # you can do anything here, eg. setup new optional executable
#     target_compile_definitions(noobsEngine PRIVATE DEFINE_NAME)
# endif()

# Add building new shared library
# add_library(exampleLibrary SHARED ${LIBRARY_SOURCES})
# target_link_libraries (exampleLibrary PRIVATE ${CMAKE_THREAD_LIBS_INIT})
# set_property(TARGET exampleLibrary PROPERTY CXX_STANDARD 14) # we want C++14

# How to include and link OpenGL and GLUT libraries
find_package(OpenGL REQUIRED)
# find_package(GLUT REQUIRED)
target_include_directories(noobsEngine PRIVATE SYSTEM ${OPENGL_INCLUDE_DIRS} )
target_link_libraries(noobsEngine PRIVATE ${OPENGL_LIBRARIES} )

# How to use some other standard packages
# find_package(ZLIB REQUIRED) 
# target_link_libraries (noobsEngine PRIVATE ZLIB::ZLIB)
# 
# find_package(OpenSSL REQUIRED)
# target_link_libraries (noobsEngine PRIVATE OpenSSL::SSL OpenSSL::Crypto)
#
# find_package(CURL REQUIRED)
# target_link_libraries (noobsEngine PRIVATE CURL::libcurl)

target_link_libraries (noobsEngine PRIVATE glfw)
target_link_libraries (noobsEngine PRIVATE GLAD)
target_link_libraries (noobsEngine PRIVATE glm)

#------------------------------------------------------------------------------
# Unit tests

add_subdirectory(unittest)

#-------------------------------------------------------------------------------
# Copy MINGW needed libraries for building on windows

if(MINGW)
    message(STATUS "MinGW detected")
    get_filename_component(GCC_PATH ${CMAKE_C_COMPILER} PATH)
    if(${GCC_PATH} MATCHES "mingw64/bin")
        set(libgcc "libgcc_s_seh-1.dll") #64bit
        message(STATUS "  64bit dlls is building")
    else()
        set(libgcc "libgcc_s_dw2-1.dll") #32bit
        message(STATUS "  32bit dlls is building")
    endif()

    install(FILES ${GCC_PATH}/${libgcc}
        ${GCC_PATH}/libwinpthread-1.dll
        ${GCC_PATH}/libstdc++-6.dll
        DESTINATION ./bin/
    )
endif(MINGW)


#------------------------------------------------------------------------------
# Other MISC targets

# Adds misc targets: format, cppcheck, tidy, see cmake/cleanCppExtensions.cmake
addMiscTargets()


